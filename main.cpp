#include <iostream>
#include <curl/curl.h>
#include <string>
#include "hmac.hpp"
#include <ctime>
using namespace std;
int main() {
    CURL *curl_handle = curl_easy_init();
    if(curl_handle) {
        /* Формирование  запроса  */
        string post_data="";
        struct curl_slist *headers=NULL;
        string command = "command=returnBalances&nonce=" + to_string(time(0)*1000);
        cout<<command<<endl;

        string Secret = "300963b179f1cda60cf6efed0bc0bb4030b3fcb42ff45119372acbdb6cf595aff618b6950abfd97b382e099fa092bcd7ea400a10a4641935e5b9c1a086e23a0f";
        string Sign = "Sign: "+ hmac::get_hmac(Secret, command, hmac::TypeHash::SHA512);
        cout<<Sign<<endl;
        post_data += command;
        cout<<post_data<<endl;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "Key: GYSZM48J-17YQSS42-UMNC5YBJ-8XJ0WXV3");
        headers = curl_slist_append(headers, Sign.c_str());

        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://poloniex.com/tradingApi");
        curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, post_data.length());
        curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    }
    curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);

        //  сохраняем html код cтраницы в строку content
    return 0;
}
