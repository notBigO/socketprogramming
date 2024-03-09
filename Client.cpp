#include "Client.h"
#include <Ws2tcpip.h>

Client::Client(const std::string& serverAddress) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		Common::error("Failed to initialise Winsock.");
		return;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		Common::error("Failed to create client socket.");
		WSACleanup();
		return;
	}

	sockaddr_in serverAddressInfo;
	serverAddressInfo.sin_family = AF_INET;
	serverAddressInfo.sin_port = htons(DEFAULT_PORT);
	inet_pton(AF_INET, serverAddress.c_str(), &serverAddressInfo.sin_addr);

	if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddressInfo), sizeof(serverAddressInfo)) == SOCKET_ERROR) {
		Common::error("Failed to connect to the server.");
		closesocket(clientSocket);
		WSACleanup();
		return;
	}
}

Client::~Client() {
	closesocket(clientSocket);
	WSACleanup();
}

void Client::Start() {
	std::string message;
	char buffer[BUFFER_SIZE];

	while (true) {
		std::cout << "You: ";
		std::getline(std::cin, message);

		send(clientSocket, message.c_str(), message.size(), 0);

		int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
		if (bytesRead > 0) {
			buffer[bytesRead] = '\0'; // Null-terminate the received data
			std::cout << "Server: " << buffer << "\n";
		}
		else {
			Common::error("Error receiving data from server.");
			break;
		}
	}
}