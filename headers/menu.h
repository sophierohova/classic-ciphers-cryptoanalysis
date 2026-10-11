#pragma once
#include <string>

class Menu {
private:
    int get_variant(int min, int max);
    std::wstring get_text();

public:
    Menu() = default;
    void run();
};
