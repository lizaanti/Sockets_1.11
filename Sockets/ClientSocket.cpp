#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "������ ������������� Winsock.\n";
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "�� ������� ������� �����.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "�������� IP �����.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "�� ������� ������������ � �������.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::string equation;
    std::cout << "������� ��������� � ������� ax + b = c: ";
    std::getline(std::cin, equation);

    send(clientSocket, equation.c_str(), equation.size() + 1, 0);

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "����� �� �������: " << buffer << std::endl;
    }
    else {
        std::cerr << "������ ��� ��������� ������ �� �������.\n";
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
