//
// Created by d-qql on 25.06.2020.
//
#include "API.h"
static size_t write_data(char *ptr, size_t size, size_t nmemb, string* data){
    if (data)
    {
        data->append(ptr, size*nmemb);
        return size*nmemb;
    }
    else
        return 0;  // будет ошибка
}
string PrivateMethods::Sign(string command, string Secret, string APIkey){
    return hmac::get_hmac(Secret, command, hmac::TypeHash::SHA512);
}
string PrivateMethods::doCommand(string command, string Secret, string APIkey){
    CURL *curl_handle = curl_easy_init();
    string result;
    if(curl_handle) {
        struct curl_slist *headers=NULL;
        command += to_string(time(0)*1000);
        string Sign = "Sign: "+ hmac::get_hmac(Secret, command, hmac::TypeHash::SHA512);
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://poloniex.com/tradingApi");
        curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, command.length());
        curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, command.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, ("Key: " + APIkey).c_str());
        headers = curl_slist_append(headers, ("Sign: " + Sign).c_str());
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &result);
    }
    curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);
    return result;
}
