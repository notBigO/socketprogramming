#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <WinSock2.h>
#include "Common.h"

class Server {
public:
	Server();
	~Server();

	void Start();
	void Stop();

private:
	void HandleClient(SOCKET clientSocket);

	SOCKET serverSocket;
	std::vector<std::thread> clientThreads;
	std::mutex mtx;
	
	bool isRunning;
};

