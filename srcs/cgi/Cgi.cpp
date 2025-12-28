/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 20:02:54 by pmateo            #+#    #+#             */
/*   Updated: 2025/12/29 00:00:26 by pmateo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Response*	cgiHandler( const Request& request, const ServerConfig& server )
{
	std::map<std::string, Location>::const_iterator it;
	it = server.findMatchingLocation(request);

	std::string root;
	if (it != server.getLocations().end() && it->second.getRoot().empty() == false)
		root = it->second.getRoot();
	else
		root = server.getRoot();
	
	std::string clean_uri = request.getUri();
	size_t pos = clean_uri.find('?');
	if (pos != std::string::npos)
		clean_uri = clean_uri.substr(0, pos);

	std::string path = root + clean_uri;

	struct stat st;
	if (stat(path.c_str(), &st) != 0)
	{
		err_msg( "cgiHandler()", "CGI file not found" );
		return (new Response(404, "Not Found"));
	}
	if (!S_ISREG(st.st_mode))
	{
		err_msg( "cgiHandler()", "CGI file not regular" );
		return (new Response(404, "Not Found"));
	}
	if (!(st.st_mode & S_IXUSR))
	{
		err_msg( "cgiHandler()", "Script exists but is not executable" );
		return (new Response(403, "Forbidden"));
	}
	return (doCgi( request, server, path ));
	
}

Response*	doCgi( const Request& request, const ServerConfig& server, const std::string& path )
{
	std::string output;
	int	pipe_parent[2];
	int	pipe_children[2];

	if (pipe(pipe_parent) == -1 || pipe(pipe_children) == -1)
		return (new Response(500, "Internal Server Error"));
	
	pid_t pid = fork();
	if (pid == -1)
	{
		close(pipe_parent[0]), close (pipe_parent[1]);
		close(pipe_children[0]), close (pipe_children[1]);
		return (new Response(500, "Internal Server Error"));
	}

	if (pid == 0)
	{
		int pipes[2][2];
		pipes[0][0] = pipe_parent[0], pipes[0][1] = pipe_parent[1];
		pipes[1][0] = pipe_children[0], pipes[1][1] = pipe_children[1];
		childExec(request, server, path, pipes);
	}
	else
	{
		close(pipe_parent[0]), close(pipe_children[1]);
		if (request.getBody().empty() == false)
			write(pipe_parent[1], request.getBody().c_str(), request.getBody().size());
		close(pipe_parent[1]);

		time_t start = time(NULL);
		time_t last_read = time(NULL);
		char buffer[4096];
		ssize_t bytes;
		
		while ((bytes = read(pipe_children[0], buffer, sizeof(buffer))) > 0)
		{
			time_t tmp = time(NULL);
			if (tmp - last_read > CGI_TIMEOUT)
			{
				kill(pid, SIGKILL);
				waitpid(pid, NULL, 0);
				close(pipe_children[0]);
				return (new Response(504, "Gateway Timeout"));
			}
			if (tmp - start > SLOWLORIS_TIMEOUT)
			{
				kill(pid, SIGKILL);
				waitpid(pid, NULL, 0);
				close(pipe_children[0]);
				return (new Response(504, "Gateway Timeout"));
			}
			output.append(buffer, bytes);
			last_read = tmp;
		}
		close(pipe_children[0]);

		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) == false || WEXITSTATUS(status) != 0)
			return (new Response(500, "Internal Server Error"));
	}
	return (handleOutput(output));
}

void	childExec( const Request& request, const ServerConfig& server, \
	const std::string& path, int (&pipes)[2][2] )
{
	char *argv[2];
	char **envp;
	std::string path_dir;
	
	close(pipes[0][1]);
	if (dup2(pipes[0][0], STDIN_FILENO) == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		close(pipes[0][0]);
		close(pipes[1][0]), close(pipes[1][1]);
		exit(FAILURE);
	}
	close(pipes[0][0]);
	close(pipes[1][0]);
	if (dup2(pipes[1][1], STDOUT_FILENO) == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		close(pipes[1][1]);
		exit(FAILURE);
	}
	close(pipes[1][1]);

	size_t pos = path.find_last_of('/');
	if (pos == std::string::npos)
		path_dir = ".";
	else if (pos == 0)
		path_dir = "/";
	else
		path_dir = path.substr(0, pos);

	if (chdir(path_dir.c_str()) == -1)
	{
		std::cerr << "chdir failed" << std::endl;
		exit(FAILURE);
	}

	std::string	file_name;
	if (pos == std::string::npos)
		file_name = path;
	else
		file_name = path.substr(pos + 1);
	
	argv[0] = const_cast<char*>(file_name.c_str());
	argv[1] = NULL; 
	try
	{
		envp = createEnvp(request, server, path);
		std::cerr << "path = " << path << std::endl;
		std::cerr << "argv[0] = " << argv[0] << std::endl;
		if (execve(file_name.c_str(), argv, envp) == -1)
			throw std::runtime_error("execve failed");
	}
	catch (std::bad_alloc& e)
	{
		std::cerr << "bad_alloc: " << e.what() << std::endl;
		exit(FAILURE);
	}
	catch (std::runtime_error& e)
	{
		std::string reason(strerror(errno));
		std::cerr << "runtime_error: " << e.what() << " : "  << reason << std::endl;
		if (envp != NULL)
		{
			for (size_t i = 0; envp[i] != NULL; ++i)
				delete [] envp[i];
			delete [] envp;
		}
		exit(FAILURE);
	}
}

char**	createEnvp( const Request& request, const ServerConfig& server, const std::string& path )
{
	std::vector<std::string> envp_vec;

	envp_vec.push_back("REQUEST_METHOD=" + request.getMethod());
	envp_vec.push_back("QUERY_STRING=" + extractQueryString(request.getUri()));
	envp_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envp_vec.push_back("SERVER_SOFTWARE=WebServ/1.0");
	if (server.getServerName().empty() == false)
		envp_vec.push_back("SERVER_NAME=" + server.getServerName());
	else
		envp_vec.push_back("SERVER_NAME=localhost");
	envp_vec.push_back("SERVER_PORT=" + toString(server.getPort()));
	std::string clean_uri = request.getUri();
	size_t pos = clean_uri.find('?');
	if (pos != std::string::npos)
		clean_uri = clean_uri.substr(0, pos);
	envp_vec.push_back("SCRIPT_NAME=" + clean_uri);
	envp_vec.push_back("SCRIPT_FILENAME=" + path);
	if (request.getBody().empty() == false)
		envp_vec.push_back("CONTENT_LENGTH=" + request.getHeaderValue("content-length"));
	if (request.hasHeader("content-type") == true)
		envp_vec.push_back("CONTENT_TYPE=" + request.getHeaderValue("content-type"));

	std::multimap<std::string, std::string>::const_iterator it = request.getHeaderMap().begin();
	for (; it != request.getHeaderMap().end(); ++it)
	{
		if (it->first == "content-length" || it->first == "content-type")
			continue;
		std::string name = "HTTP_" + toUpperCgi(it->first);
		envp_vec.push_back(name + "=" + it->second);
	}
	
	char** envp = NULL;
	try
	{
		envp = new char*[envp_vec.size() + 1];
		std::memset(envp, 0, (envp_vec.size() + 1) * sizeof(char *));
		for (size_t i = 0; i < envp_vec.size(); ++i)
		{
			size_t length = envp_vec[i].length();
			envp[i] = new char[length + 1];
			std::strcpy(envp[i], envp_vec[i].c_str());
		}
		envp[envp_vec.size()] = NULL;
		return (envp);
	}
	catch (std::bad_alloc& e)
	{
		if (envp != NULL)
		{
			for (size_t i = 0; envp[i] != NULL; ++i)
				delete [] envp[i];
			delete [] envp;
		}
		throw;
	}
}

Response*	handleOutput( const std::string& output )
{
	std::string	headers_str;
	std::string	body_str;
	size_t		end_headers;
	
	end_headers = output.find("\r\n\r\n");
	if (end_headers != std::string::npos)
	{
		headers_str = output.substr(0, end_headers);
		body_str = output.substr(end_headers + 4);
	}
	else if (output.find("\n\n") != std::string::npos)
	{
		end_headers = output.find("\n\n");
		headers_str = output.substr(0, end_headers);
		body_str = output.substr(end_headers + 2);
	}
	if (end_headers == std::string::npos)
		return (new Response(502, "Bad Gateway"));
	
	std::multimap<std::string, std::string> headers = handleOutputHeaders(headers_str);
	if (headers.empty() == true)
		return (new Response(502, "Bad Gateway"));

	Response* response = new Response(200, "OK");
	response->setBody(body_str);

	std::multimap<std::string, std::string>::iterator it = headers.find("status");
	if (it != headers.end())
	{
		int			status_code = std::atoi(it->second.c_str());
		std::string status_name = Response::getStatusNameFromStatusCode(status_code);
		if (status_code < 200 || status_code > 505)
			return (new Response(502, "Bad Gateway"));
		else if (status_name == "Unknown")
			return (new Response(502, "Bad Gateway"));
		response->setStatusCode(std::atoi(it->second.c_str()));
	}
	
	it = headers.find("location");
	if (it != headers.end() && response->getStatusCode() == 200)
	{
		response->setStatusCode(302);
		response->setStatusName("Found");
	}
	
	it = headers.begin();
	for (; it != headers.end(); it++)
	{
		if (it->first == "status")
			continue;
		response->getHeaderMap().insert(*it);
	}

	if (headers.find("content-length") == headers.end())
		response->getHeaderMap().insert(std::make_pair("content-length", toString(body_str.size())));
	
	return response;
}

std::multimap<std::string, std::string>	handleOutputHeaders( const std::string& headers_str )
{
	std::multimap<std::string, std::string>	headers;
	std::vector<std::string> lines = split(headers_str, '\n');

	for (size_t i = 0; i < lines.size(); ++i)
	{
		std::string line = lines[i];

		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		
		if (line.empty())
			continue;

		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;
		std::pair<std::string, std::string>	header_pair;
		header_pair.first = toLower(line.substr(0, colon)); 
		header_pair.second = line.substr(colon + 1);
		
		size_t j = 0;
		while (j < header_pair.second.length() && \
			(header_pair.second[j] == ' ' || header_pair.second[j] == '\t'))
			j++;
		header_pair.second = header_pair.second.substr(j);

		headers.insert(header_pair);
	}

	return (headers);
}

std::string	toUpperCgi( const std::string& name )
{
	std::string formated_name;

	for (size_t i = 0; i < name.length(); ++i)
	{
		if (name[i] == '-')
			formated_name += '_';
		else
			formated_name += std::toupper(static_cast<unsigned char>(name[i]));
	}
	return (formated_name);
}

bool	isCgiRequest( const Request& request, const ServerConfig& server )
{
	const std::vector<std::string>*	cgi_exts;
	std::map<std::string, Location>::const_iterator it;
	
	it = server.findMatchingLocation(request);
	if (it == server.getLocations().end())
		std::cerr << "findMatchLocation failed\n";
	if (it != server.getLocations().end() && it->second.getCgiExtension().empty() == false)
		cgi_exts = &it->second.getCgiExtension();
	else if (server.getCgiExtension().empty() == false)
		cgi_exts = &server.getCgiExtension();
	else
		return (false);

	std::cerr << "findMatchingLocation good\n";

	std::string clean_path = request.getUri();
	size_t pos = clean_path.find('?');
	if (pos != std::string::npos)
		clean_path = clean_path.substr(0, pos);

	std::vector<std::string>::const_iterator it_exts = cgi_exts->begin();
	for (; it_exts != cgi_exts->end(); ++it_exts)
	{
		if (clean_path.size() <= it_exts->size())
			continue;
		else
		{
			if (clean_path.compare(clean_path.size() - it_exts->size(), it_exts->size(), *it_exts) == 0)
				return true;
		}
	}
	return (false);
}