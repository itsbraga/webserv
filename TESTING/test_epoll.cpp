#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>

#define FAILURE 1
#define SUCCESS 0

int main(void)
{
	int			epoll_instance;
	int			serverSocket;
	sockaddr_in	serverAddr;
	int			clientSocket;

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (0 > bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
		std::perror("bind"), exit(FAILURE);
	if (0 > listen(serverSocket, SOMAXCONN)) // verifier SOMAXCONN avec valgrind
		std::perror("listen"), exit(FAILURE);

	epoll_instance = epoll_create(0);
	clientSocket = accept(serverSocket, NULL, NULL);

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << buffer << std::endl;

	close(serverSocket);
}