#pragma once
#include <string>

class ICipher {
public:
    virtual ~ICipher() = default;
    virtual std::wstring encrypt(const std::wstring& text) const = 0;
    virtual std::wstring decrypt(const std::wstring& text) const = 0;
};