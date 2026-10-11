#pragma once
#include <string>

class FileHandler {
public:
    static std::wstring read_file(const std::wstring& path);
    static void write_file(const std::wstring& path, const std::wstring& text);
};