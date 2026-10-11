#include "encryption.h"
#include <stdexcept>
#include "CryptoAttacker.h"
#include <map>
#include <iostream>

wchar_t mtowupper(wchar_t c) {
    if ((c >= L'a') && (c <= L'z')) {
        return static_cast<wchar_t>(c - (L'a' - L'A'));
    }
    if (c == 0x0451) return 0x0401;
    if ((c >= 0x0430) && (c <= 0x044F)) {
        return static_cast<wchar_t>(c - (0x0430 - 0x0410));
    }
    return c;
}

wchar_t mtowlower(wchar_t c) {
    if ((c >= L'A') && (c <= L'Z')) {
        return static_cast<wchar_t>(c + (L'a' - L'A'));
    }
    if (c == 0x0401) return 0x0451; 
    if ((c >= 0x0410) && (c <= 0x042F)) {
        return static_cast<wchar_t>(c + (0x0430 - 0x0410));
    }
    return c;
}

int get_pos(wchar_t c) {
    wchar_t upperc = mtowupper(c);
    if ((upperc >= L'A') && (upperc <= L'Z')) return upperc - L'A';
    if (upperc == 0x0401) return 6;
    if ((upperc >= 0x0410) && (upperc <= 0x042F)) {
        int pos = upperc - 0x0410;
        if (pos >= 6) return pos + 1; 
        return pos;
    }
    return -1;
}

static wchar_t get_char_from_pos(int pos, bool isEng) {
    if (isEng) {
        return static_cast<wchar_t>(L'A' + pos);
    }
    if (pos == 6) return 0x0401; 
    if (pos > 6){
        return static_cast<wchar_t>(0x0410 + (pos - 1));
    }
    return static_cast<wchar_t>(0x0410 + pos);
}

int CaesarCipher::norm_key(int key, int alphabet_size) const {
    return ((key % alphabet_size) + alphabet_size) % alphabet_size;
}

std::wstring CaesarCipher::encrypt(const std::wstring& text) const {
    std::wstring result;
    result.reserve(text.size());

    for (const wchar_t& c : text) {
        int pos = get_pos(c);
        if (pos == -1) {
            result += c;
            continue;
        }
        bool islower = ((c >= L'a') && (c <= L'z')) || ((c >= 0x0430) && (c <= 0x044F)) || (c == 0x0451);
        bool isEng = (mtowupper(c) >= L'A' && mtowupper(c) <= L'Z');
        int alph_size = isEng ? 26 : 33;
        int shift = norm_key(key_, alph_size);
        int posnew = (pos + shift) % alph_size;
        wchar_t resc = get_char_from_pos(posnew, isEng);
        if (islower) resc = mtowlower(resc);
        result += resc;
    }

    return result;
}

std::wstring CaesarCipher::decrypt(const std::wstring& text) const {
    CaesarCipher temp_cipher(-key_);
    return temp_cipher.encrypt(text);
}

VigenereCipher::VigenereCipher(const std::wstring& key) {
    if (key.empty()) {
        throw std::invalid_argument("Ключ не может быть пустым");
    }
    key_ = key;
}

std::wstring VigenereCipher::encrypt(const std::wstring& text) const { 
    return vigenere_func(text, true); 
}

std::wstring VigenereCipher::decrypt(const std::wstring& text) const { 
    return vigenere_func(text, false); 
}

std::wstring VigenereCipher::vigenere_func(const std::wstring& text, bool isEncrypt) const {
    std::wstring result;
    result.reserve(text.size());
    size_t key_size = key_.size();
    size_t key_i = 0;

    for (const wchar_t& c : text) {
        int pos = get_pos(c);
        if (pos == -1) {
            result += c;
            continue;
        }
        int num_key = get_pos(key_[key_i]);
        if (num_key == -1) {
            num_key = 0; 
        }
        bool islower = ((c >= L'a') && (c <= L'z')) || ((c >= 0x0430) && (c <= 0x044F)) || (c == 0x0451);
        bool isEng = ((mtowupper(c) >= L'A') && (mtowupper(c) <= L'Z'));
        int alph_size = isEng ? 26 : 33;
        int shift = isEncrypt ? num_key : -num_key;
        int final_shift = ((shift % alph_size) + alph_size) % alph_size;
        int posnew = (pos + final_shift) % alph_size;
        wchar_t resc = get_char_from_pos(posnew, isEng);

        if (islower){
            resc = mtowlower(resc);
        }
        result += resc;
        key_i = (key_i + 1) % key_size; 
    }
    return result;
}

void CryptoAttacker::brute_force_caesar(const std::wstring& ciphertext) {
    for (int shift = 1; shift < 33; ++shift) {
        CaesarCipher cracker(shift);
        std::wstring decrypted_variant = cracker.decrypt(ciphertext);
        std::wcout << L"Ключ " << shift << L": " << decrypted_variant << std::endl;
    }
}

int CryptoAttacker::frequency_analysis_caesar(const std::wstring& ciphertext) {
    std::map<wchar_t, int> freq_map;
    for (wchar_t c : ciphertext) {
        int pos = get_pos(c);
        if (pos != -1) {
            freq_map[mtowupper(c)]++;
        }
    }

    if (freq_map.empty()) return 0;
    wchar_t most_frequent_char = L'А';
    int max_count = 0;

    for (const auto& pair : freq_map) {
        if (pair.second > max_count) {
            max_count = pair.second;
            most_frequent_char = pair.first;
        }
    }

    int encrypted_pos = get_pos(most_frequent_char);
    int target_pos = 15;
    int guessed_key = (encrypted_pos - target_pos + 33) % 33;
    return guessed_key;
}

