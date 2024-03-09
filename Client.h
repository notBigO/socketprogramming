#pragma once

#include <WinSock2.h>
#include "Common.h"

class Client {
public:
	Client(const std::string& serverAddress);
	~Client();

	void Start();

private:
	SOCKET clientSocket;
};