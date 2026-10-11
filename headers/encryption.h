#pragma once
#include <string>
#include "ICipher.h"

wchar_t mtowupper(wchar_t c);
wchar_t mtowlower(wchar_t c);
int get_pos(wchar_t c);

class CaesarCipher : public ICipher {
private:
    int key_;
    int norm_key(int key, int alphabet_size) const;

public:
    CaesarCipher(int key) : key_(key) {}
    std::wstring encrypt(const std::wstring& text) const override;
    std::wstring decrypt(const std::wstring& text) const override;
};

class VigenereCipher : public ICipher {
private:
    std::wstring key_;
    std::wstring vigenere_func(const std::wstring& text, bool isEncrypt) const;

public:
    VigenereCipher(const std::wstring& key);
    std::wstring encrypt(const std::wstring& text) const override;
    std::wstring decrypt(const std::wstring& text) const override;
};
