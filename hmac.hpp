#ifndef HMAC_HPP_INCLUDED
#define HMAC_HPP_INCLUDED

#include "sha256.hpp"
#include "sha512.hpp"
#include <string>

namespace hmac {

    /** \brief Тип используемой хеш-функции
     */
	enum class TypeHash {
        SHA256 = 0, /**< Использовать SHA256 */
        SHA512 = 1, /**< Использовать SHA512 */
    };

    /** \brief Преобразование строки в шестнадцетиричный формат
     * \param input Исходная строка
     * \param is_upper Флаг верхнего регистра
     * \return Строка в шестнадцетиричном формате
     */
    std::string to_hex(const std::string& input, const bool is_upper = false);

    /** \brief Получить хеш строки
     * \param input Исходная строка
     * \param type Тип хеш-функции
     * \return Хеш строки
     */
    std::string get_hash(const std::string &input, const TypeHash type) ;

    /** \brief Получить HMAC
     *
     * \param key Ключ
     * \param msg Сообщение
     * \param type Тип хеш-функции
     * \param is_hex Использовать шестнадцетиричное представление
     * \param is_upper Флаг верхнего регистра
     */
    std::string get_hmac(std::string key, const std::string &msg, const TypeHash type, const bool is_hex = true, const bool is_upper = false);
}

#endif // HMAC_H_INCLUDED
