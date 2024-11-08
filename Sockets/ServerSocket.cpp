#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

// Функция для решения уравнения вида ax + b = c
std::string solveEquation(const std::string& equation) {
    double a, b, c;
    char x;

    // Попробуем разобрать строку в формате "ax + b = c"
    std::istringstream iss(equation);
    if (!(iss >> a >> x >> x >> b >> x >> c)) {
        return "Ошибка: Неверный формат уравнения.";
    }

    // Решение уравнения ax + b = c
    if (a == 0) {
        return (b == c) ? "Решение: бесконечно много решений" : "Решение: нет решений";
    }

    double result = (c - b) / a;
    return "Решение: x = " + std::to_string(result);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock.\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Не удалось создать сокет.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, SOMAXCONN);
    std::cout << "Сервер запущен. Ожидание клиентов...\n";

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка подключения клиента.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Ошибка при получении данных.\n";
    }
    else {
        buffer[bytesReceived] = '\0';
        std::string equation(buffer);

        // Решаем уравнение
        std::string result = solveEquation(equation);

        // Отправляем результат клиенту
        send(clientSocket, result.c_str(), result.size() + 1, 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}