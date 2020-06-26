//
// Created by d-qql on 26.06.2020.
//
#include "hmac.hpp"
using namespace hmac;
std::string hmac::to_hex(const std::string& input, const bool is_upper) {
    static const char *lut = "0123456789abcdef0123456789ABCDEF";
    const char *symbol = is_upper ? lut + 16 : lut;
    const size_t length = input.size();
    std::string output;
    output.reserve(2 * length);
    for (size_t i = 0; i < length; ++i) {
        const uint8_t c = input[i];
        output.push_back(symbol[c >> 4]);
        output.push_back(symbol[c & 15]);
    }
    return output;
}
std::string hmac::get_hash(const std::string &input, const TypeHash type) {
    switch(type) {
        case TypeHash::SHA256: {
            uint8_t digest[hmac_hash::SHA256::DIGEST_SIZE];
            std::fill(digest, digest + hmac_hash::SHA256::DIGEST_SIZE, '\0');
            hmac_hash::SHA256 ctx = hmac_hash::SHA256();
            ctx.init();
            ctx.update((uint8_t*)input.c_str(), input.size());
            ctx.final(digest);
            return std::string((const char*)digest, hmac_hash::SHA256::DIGEST_SIZE);
        }
            break;
        case TypeHash::SHA512: {
            uint8_t digest[hmac_hash::SHA512::DIGEST_SIZE];
            std::fill(digest, digest + hmac_hash::SHA512::DIGEST_SIZE, '\0');
            hmac_hash::SHA512 ctx = hmac_hash::SHA512();
            ctx.init();
            ctx.update((uint8_t*)input.c_str(), input.size());
            ctx.final(digest);
            return std::string((const char*)digest, hmac_hash::SHA512::DIGEST_SIZE);
        }
            break;
        default:
            return std::string();
            break;
    };
    return std::string();
}
std::string hmac::get_hmac(std::string key, const std::string &msg, const TypeHash type, const bool is_hex, const bool is_upper) {
    size_t block_size = 0;
    switch(type) {
        case TypeHash::SHA256:
            block_size = hmac_hash::SHA256::SHA224_256_BLOCK_SIZE;
            break;
        case TypeHash::SHA512:
            block_size = hmac_hash::SHA512::SHA384_512_BLOCK_SIZE;
            break;
        default:
            return std::string();
            break;
    };

    if(key.size() > block_size) {
        /* Если длина ключа K больше размера блока,
         * то к ключу K применяем хеш-функцию H,
         * получаем строку размером в L байт,
         * добавляем нули к правой части этой строки
         * для создания строки размером в b байт,
         */
        key = hmac::get_hash(key, type);
    }
    if(key.size() < block_size) {
        /* Если длина ключа K меньше размера блока,
         * то добавляем нули к правой части K
         * для создания строки размером в b байт,
         * копируем результат в K0
         * (например, если length( К ) = 20 (в байтах)
         * и b = 64 (в байтах), то к правой части К
         * будет добавлено 64 - 20 = 44 нулевых байта (0x00))
         */
        key.resize(block_size, '\0');
    }
    /* Если длина ключа K равна размеру блока,
     * то копируем K в K0 без изменений
     * и переходим к шагу 2
      */
    std::string ikeypad;
    std::string okeypad;
    ikeypad.reserve(block_size);
    okeypad.reserve(block_size);
    for(size_t i = 0; i < block_size; ++i) {
        ikeypad.push_back('\x36' ^ key[i]);
        okeypad.push_back('\x5c' ^ key[i]);
    }
    if(is_hex) return hmac::to_hex(hmac::get_hash(okeypad + hmac::get_hash(ikeypad + msg, type), type), is_upper);
    return hmac::get_hash(okeypad + hmac::get_hash(ikeypad + msg, type), type);
}