#define WIN32_LEAN_AND_MEAN // Макрос WIN32_LEAN_AND_MEAN уменьшает объем включаемых заголовочных файлов Windows.h для оптимизации сборки

#include <iostream> // Включаем заголовочный файл для ввода-вывода
#include <Windows.h> // Заголовочный файл для функций Windows API
#include <WinSock2.h> // Заголовочный файл для Winsock API
#include <WS2tcpip.h> // Заголовочный файл для структур и функций TCP/IP

using namespace std;

int main() {
    SetConsoleCP(1251); // Устанавливаем кодировку консоли для поддержки кириллицы
    setlocale(LC_ALL, "Russian"); // Устанавливаем русскую локаль для корректного отображения русских символов

    WSADATA wsaData; // Структура для хранения информации о реализации Winsock
    ADDRINFO hints; // Структура для задания параметров соединения
    ADDRINFO* addrResult; // Указатель на структуру для хранения результата getaddrinfo
    SOCKET ClientSocket = INVALID_SOCKET; // Сокет для клиента
    SOCKET ListenSocket = INVALID_SOCKET; // Сокет для прослушивания входящих соединений

    const char* sendBuffer = "Hello from Server"; // Буфер для отправляемых данных
    char recvBuffer[512]; // Буфер для принимаемых данных

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Инициализация Winsock с версией 2.2

    if (result != 0) {
        cout << "WSAStartup failed" << endl; // Если инициализация не удалась, выводим сообщение об ошибке
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints)); // Обнуляем структуру hints
    hints.ai_family = AF_INET; // Указываем семейство адресов (IPv4)
    hints.ai_socktype = SOCK_STREAM; // Указываем тип сокета (потоковый)
    hints.ai_protocol = IPPROTO_TCP; // Указываем протокол (TCP)
    hints.ai_flags = AI_PASSIVE; // Указываем флаг, чтобы сокет мог принимать входящие соединения

    result = getaddrinfo(NULL, "788", &hints, &addrResult); // Получаем информацию об адресе

    if (result != 0) {
        cout << "getaddrinfo failed" << endl; // Если получение информации об адресе не удалось, выводим сообщение об ошибке
        return 1;
    }

    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // Создаем сокет для прослушивания входящих соединений

    if (ListenSocket == INVALID_SOCKET) {
        cout << "socket creation with error" << endl; // Если создание сокета не удалось, выводим сообщение об ошибке
        freeaddrinfo(addrResult); // Освобождаем ресурсы
        WSACleanup(); // Завершаем использование Winsock
        return 1;
    }

    result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // Привязываем сокет к адресу

    if (result == SOCKET_ERROR) {
        cout << "binding connect failed" << endl; // Если привязка сокета не удалась, выводим сообщение об ошибке
        freeaddrinfo(addrResult); // Освобождаем ресурсы
        WSACleanup(); // Завершаем использование Winsock
        return 1;
    }

    result = listen(ListenSocket, SOMAXCONN); // Переводим сокет в режим прослушивания

    if (result == SOCKET_ERROR) {
        cout << "Listening failed" << endl; // Если прослушивание не удалось, выводим сообщение об ошибке
        freeaddrinfo(addrResult); // Освобождаем ресурсы
        WSACleanup(); // Завершаем использование Winsock
        return 1;
    }

    ClientSocket = accept(ListenSocket, NULL, NULL); // Принимаем входящее соединение

    if (result == SOCKET_ERROR) {
        cout << "Accepting failed" << endl; // Если принятие соединения не удалось, выводим сообщение об ошибке
        freeaddrinfo(addrResult); // Освобождаем ресурсы
        WSACleanup(); // Завершаем использование Winsock
        return 1;
    }

    closesocket(ListenSocket); // Закрываем сокет для прослушивания

    do {
        ZeroMemory(recvBuffer, 512); // Обнуляем буфер приема
        result = recv(ClientSocket, recvBuffer, 512, 0); // Принимаем данные от клиента

        if (result > 0) {
            cout << "Получено " << result << " байт" << endl; // Выводим количество принятых байт
            cout << "Получено " << recvBuffer << endl; // Выводим принятые данные
result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), MSG_DONTROUTE); // Отправляем данные клиенту

if (result == SOCKET_ERROR) {
    cout << "Send failed" << endl; // Если отправка не удалась, выводим сообщение об ошибке
    freeaddrinfo(addrResult); // Освобождаем ресурсы
    WSACleanup(); // Завершаем использование Winsock
    return 1;
}
        }
        else if (result == 0) {
            cout << "Коннект закончен" << endl; // Если соединение завершено, выводим соответствующее сообщение
        }
        else {
            cout << "Сообщение не было получено. Ошибка." << endl; // Если прием данных не удался, выводим сообщение об ошибке
        }
    } while (result > 0); // Повторяем до тех пор, пока > 0

    // Попытка отключить отправку данных через сокет
    result = shutdown(ClientSocket, SD_SEND);
    // Проверка на ошибку выполнения функции shutdown
    if (result == SOCKET_ERROR) {
        // Вывод сообщения об ошибке в консоль
        cout << "shutdown error" << endl;
        // Освобождение выделенной памяти для информации об адресе
        freeaddrinfo(addrResult);
        // Завершение использования библиотеки 
        WSACleanup();
        // Возврат 1 для индикации ошибки
        return 1;
    }
    closesocket(ClientSocket); // Закрываем сокет клиента
    freeaddrinfo(addrResult); // Освобождаем ресурсы
    WSACleanup(); // Завершаем использование Winsock
    return 0;

}