#pragma once

class Request;
class Server;
class Location;

bool	isCgiRequest(const Request& request, const Server& server);
