#include "Common.h"
#include "Server.h"
#include "Client.h"
#include <thread>

int main() {
    std::thread serverThread([]() {
        Server server;
        server.Start();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Client client("127.0.0.1");
    client.Start();

    serverThread.join();

    return 0;
}
