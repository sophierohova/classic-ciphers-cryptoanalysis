#include "FileHandler.h"
#include <string>
#include <stdexcept>
#include <locale>
#include <fstream>

std::wstring FileHandler::read_file(const std::wstring& path) {
    std::wifstream in;
    in.imbue(std::locale(".UTF8"));
    in.open(path);
    if (!in.is_open()) {
        throw std::runtime_error("Не удалось открыть файл");
    }
    std::wstring text;
    std::wstring line;

    while (std::getline(in, line)) {
        text += line + L"\n";
    }

    in.close();
    if (!text.empty()) {
        text.pop_back();
    }
    return text;
}

void FileHandler::write_file(const std::wstring& path, const std::wstring& text) {
    std::wofstream out;
    out.imbue(std::locale(".UTF8"));
    out.open(path);
    if (!out.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }
    out << text;
    out.close();
}
