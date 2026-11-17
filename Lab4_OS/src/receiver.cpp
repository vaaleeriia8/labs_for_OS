#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "file_queue.h"

static PROCESS_INFORMATION start_sender(const std::string& exe, const std::string& file) {
    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    std::string cmd = exe + " " + file;
    std::vector<char> buffer(cmd.begin(), cmd.end());
    buffer.push_back('\0');

    CreateProcessA(
        nullptr,
        buffer.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NEW_CONSOLE, 
        nullptr,
        nullptr,
        &si,
        &pi
    );

    return pi;
}

static unsigned int read_positive_uint(const std::string& prompt) {
    std::regex reg("^[1-9][0-9]*$");
    std::string input;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (!std::regex_match(input, reg)) {
            std::cout << "Неверный ввод. Введите положительное целое число.\n";
            continue;
        }

        try {
            return static_cast<unsigned int>(std::stoul(input));
        }
        catch (...) {
            std::cout << "Слишком большое число.\n";
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian"); 

    std::string file;
    std::cout << "Введите имя бинарного файла: ";
    std::getline(std::cin, file);

    unsigned int capacity = read_positive_uint("Введите количество записей в файле: ");
    unsigned int sender_count = read_positive_uint("Введите количество процессов Sender: ");

    FileQueue queue(file, capacity);

    std::vector<PROCESS_INFORMATION> senders;
    for (unsigned int i = 0; i < sender_count; ++i)
        senders.push_back(start_sender("sender.exe", file));

    queue.wait_all_senders(sender_count);
    std::cout << "Все Sender'ы готовы к работе.\n";

    while (true) {
        std::cout << "[Receiver] Команды: 1 - прочитать сообщение | 2 - выйти\n";

        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "1") {
            std::string msg = queue.pop(); 
            std::cout << "Получено сообщение: " << msg << "\n";
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