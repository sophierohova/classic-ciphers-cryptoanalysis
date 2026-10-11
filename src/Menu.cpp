#include "Menu.h"
#include "FileHandler.h"  
#include "encryption.h" 
#include "CryptoAttacker.h"
#include <iostream>
#include <cstdlib>  
#include <string>
#include <limits>      
#include <memory>       

int Menu::get_variant(int min, int max) {
    int choice = 0;
    while (true) {
        std::wcin >> choice;
        if (std::wcin.fail() || (choice < min) || (choice > max)) {
            std::wcout << L"Некорректный пункт меню. Повторите попытку " << std::endl;
            std::wcin.clear();
            std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
        }
        else {
            std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
            break;
        }
    }
    return choice;
}

std::wstring Menu::get_text() {
    std::wstring text;
    std::wcout << L"Введите текст: ";
    std::getline(std::wcin, text);
    return text;
}

void Menu::run() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    std::setlocale(LC_ALL, ".UTF8");
    while (true) {
        std::wcout << L"- Меню -" << std::endl;
        std::wcout << L"1 - шифр Цезаря" << std::endl;
        std::wcout << L"2 - шифр Виженера" << std::endl;
        std::wcout << L"3 - криптоанализ" << std::endl;
        std::wcout << L"4 - выход из программы" << std::endl;
        int cipher_choice = get_variant(1, 4);
        if (cipher_choice == 4) {
            std::wcout << L"Программа завершена. До свидания!" << std::endl;
            break;
        }
        std::unique_ptr<ICipher> cipher = nullptr;
        int choice = 0;

        while (choice != 4) {

            if (cipher_choice == 3) {
                std::wcout << L"\nКриптоанализ шифра Цезаря" << std::endl;
                std::wcout << L"1 - полный перебор" << std::endl;
                std::wcout << L"2 - частотный криптоанализ" << std::endl;
                std::wcout << L"3 - выход" << std::endl;
                choice = get_variant(1, 5);
                if (choice == 3) break;
                std::wstring ciphertext = get_text();

                if (choice == 1) {
                    CryptoAttacker::brute_force_caesar(ciphertext);
                }
                else if (choice == 2) {
                    int broken_key = CryptoAttacker::frequency_analysis_caesar(ciphertext);
                    std::wcout << L"Наиболее вероятный сдвиг: " << broken_key << std::endl;
                    CaesarCipher cracker(broken_key);
                    std::wcout << L"Расшифрованный текст: " << cracker.decrypt(ciphertext) << std::endl;
                }
                continue;
            }

            if (cipher_choice == 1) {
                std::wcout << L"\n1 - задать сдвиг" << std::endl;
            }
            else {
                std::wcout << L"\n1 - задать ключ" << std::endl;
            }
            std::wcout << L"2 - зашифровать текст" << std::endl;
            std::wcout << L"3 - расшифровать текст" << std::endl;
            std::wcout << L"4 - выход" << std::endl;
            std::wcout << L"Выберите: ";
            choice = get_variant(1, 5);

            switch (choice) {
            case 1:
                if (cipher_choice == 1) {
                    std::wcout << L"Введите число-сдвиг: ";
                    int shift = get_variant(-2147483647, 2147483647);
                    cipher = std::make_unique<CaesarCipher>(shift);
                }
                else {
                    std::wcout << L"Введите строку-ключ без пробелов: ";
                    std::wstring key;
                    std::getline(std::wcin, key);

                    try {
                        cipher = std::make_unique<VigenereCipher>(key);
                    }
                    catch (const std::invalid_argument& e) {
                        std::cerr << "Ошибка: " << e.what() << std::endl;
                    }
                }
                break;
            case 2:
            case 3:
            {
                if (cipher == nullptr) {
                    std::wcout << L"Сначала необходимо задать сдвиг/ключ в пункте 1\n";
                    break;
                }

                std::wcout << L"1 - ввести текст вручную\n2 - прочитать из файла" << std::endl;
                int src = get_variant(1, 2);
                std::wstring working_text;

                if (src == 1) {
                    working_text = get_text();
                }
                else {
                    std::wstring path;
                    std::wcout << L"Введите имя файла: ";
                    std::getline(std::wcin, path);
                    try {
                        working_text = FileHandler::read_file(path);
                        std::wcout << L"Исходный текст: " << working_text << std::endl;
                    }
                    catch (const std::runtime_error& e) {
                        std::cerr << "Ошибка: " << e.what() << std::endl;
                        break;
                    }
                }

                std::wstring result = (choice == 3) ? cipher->encrypt(working_text) : cipher->decrypt(working_text);
                std::wcout << L"Результат:\n" << result << std::endl;
                std::wcout << L"Записать результат в файл? 1 - да, 2 - нет: ";
                if (get_variant(1, 2) == 1) {
                    std::wstring out_path;
                    std::wcout << L"Введите имя файла: ";
                    std::getline(std::wcin, out_path);
                    try {
                        FileHandler::write_file(out_path, result);
                    }
                    catch (const std::runtime_error& e) {
                        std::cerr << "Ошибка: " << e.what() << std::endl;
                    }
                }
                break;
            }
            }
        }
    }
}
