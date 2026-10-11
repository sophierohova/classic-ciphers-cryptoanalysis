#pragma once
#include <string>

class CryptoAttacker {
public:
    static void brute_force_caesar(const std::wstring& ciphertext);
    static int frequency_analysis_caesar(const std::wstring& ciphertext);
};
