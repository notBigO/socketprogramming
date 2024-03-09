#include "Server.h"

Server::Server() :isRunning(false) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		Common::error("Failed to initialise Winsock.");
		return;
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		Common::error("Failed to create server socket.");
		WSACleanup();
		return;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(DEFAULT_PORT);

	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
		Common::error("Failed to bind server socket.");
		closesocket(serverSocket);
		WSACleanup();
		return;
	}
}

Server::~Server() {
	Stop();
}

void Server::Start() {
	isRunning = true;

	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		Common::error("Error listening on server socket.");
		return;
	}

	std::cout << "Server listening on port: " << DEFAULT_PORT << std::endl;

	while (isRunning) {
		SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			Common::error("Error accepting connection.");
			break;
		}

		std::thread clientThread(&Server::HandleClient, this, clientSocket);
		clientThreads.push_back(std::move(clientThread));
	}
}

void Server::Stop() {
	isRunning = false;

	for (auto& thread : clientThreads) {
		if (thread.joinable())
			thread.join();
	}

	closesocket(serverSocket);
	WSACleanup();
}

void Server::HandleClient(SOCKET clientSocket) {
	char buffer[BUFFER_SIZE];
	int bytesRead;

	do {
		bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);

		if (bytesRead > 0) {
			buffer[bytesRead] = '\0';
			std::cout << "Client: " << buffer << "\n";
			send(clientSocket, buffer, bytesRead, 0);
		}

		else if (bytesRead == 0) {
			std::cout << "Client disconnected.";
		}

		else {
			Common::error("Error receiving data from client.");
		}
	} while (bytesRead > 0);

	closesocket(clientSocket);
}