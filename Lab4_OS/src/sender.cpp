#include <iostream>
#include <string>
#include "file_queue.h"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian"); 

    if (argc < 2) {
        std::cerr << "Ошибка Sender: не передано имя файла\n";
        return 1;
    }

    FileQueue queue(argv[1]);
    queue.signal_sender_ready(); 

    while (true) {
        std::cout << "[Sender] Команды: 1 - отправить сообщение | 2 - выйти\n";

        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "1") {
            std::cout << "Введите текст сообщения (до 20 символов): ";
            std::string msg;
            std::getline(std::cin, msg);

            if (msg.empty()) {
                std::cout << "Ошибка: сообщение пустое\n";
                continue;
            }

            if (msg.size() > 20) {
                std::cout << "Ошибка: длина сообщения больше 20 символов\n";
                continue;
            }

            try {
                queue.push(msg); 
                std::cout << "Сообщение отправлено\n";
            }
            catch (const std::exception& e) {
                std::cout << "Ошибка отправки: " << e.what() << "\n";
            }
        }
        else if (cmd == "2") {
            break; 
        }
        else {
            std::cout << "Неизвестная команда. Попробуйте снова.\n";
        }
    }

    return 0;
}