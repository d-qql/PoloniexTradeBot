//
// Created by d-qql on 25.06.2020.
//
#include "API.h"

/**
 * Функция получающая параметр из файла filename
 * paramName - имя поля,
 * Пример:  "APIkey="
 * */
string PrivateMethods::getSettings(string paramName, string filename){
    try {
        ifstream fin(filename); // открыли файл для чтения
        if(!fin.is_open()) throw 10001;
        string buf;
        while (getline(fin, buf)) { // пока не достигнут конец файла класть очередную строку в переменную (s)
            cout << buf << endl;
            string compare = buf.substr(0, paramName.length());
            cout << compare << endl;
            if (compare == paramName) {
                return buf.erase(0, paramName.length());
            }
        }
        throw 10002;
    }
    catch (int i) {
        if(i==10001) cout<<"Не удалось открыть файл настроек"<<endl;
        if(i==10002) cout<<"Не удалось найти " + paramName + " в настройках"<<endl;
    }
    return NULL;
}
/**
 * Методы, получающие APIkey и  Secret из конфига
 */
string PrivateMethods::getAPIkey() {
    return PrivateMethods::getSettings("APIkey=");
}
string PrivateMethods::getSecret() {
    return PrivateMethods::getSettings("SecretCode=");
}
/**
 * Конструктор, для подключения нескольких
 * аккаунтов необходимо добавить уникальный
 * АйДи для ключей и секретов в файл настроек
 */
PrivateMethods::PrivateMethods(){
    APIkey=getAPIkey();
    Secret=getSecret();
};

/**
 * Функция "Писарь"
 * При указании в CURLOPT_WRITEFUNCTION
 * Получаемая cURL информация будет записана в
 * объект, указанный ссылкой в CURLOPT_WRITEDATA
 */
static size_t write_data(char *ptr, size_t size, size_t nmemb, string* data){
    if (data){
        data->append(ptr, size*nmemb);
        return size*nmemb;
    }
    else
        return 0;  // будет ошибка
}
/**
 * Шифрование с помощью SHA512(HMAC)
 * @param command - Комманда, по которой обращаемся к API
 * @param Secret - Наш секретный ключ
 * @return Возвращает зашифрованную приватным ключем комманду
 */
string PrivateMethods::Sign(string command, string Secret){
    string res = hmac::get_hmac(Secret, command,hmac::TypeHash::SHA512);
    return res;
}
/**
 * Функция формирования POST-запроса на сервер биржи
 * @param command - Комманда, по которой обращаемся к API
 * @param Secret - Наш секретный ключ
 * @param APIkey
 * @return Возвращает ответ сервера по запросу в json формате
 */
string PrivateMethods::doCommand(string command, string Secret, string APIkey){
    CURL *curl_handle = curl_easy_init();
    string result;
    if(curl_handle) {
        struct curl_slist *headers=NULL;
        command += to_string(time(0)*1000);                         //добавляем к команде целое число, которое должно возрастать
        string Sign = "Sign: "+ PrivateMethods::Sign(command, Secret);        //подписываем комманду секретным ключем
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://poloniex.com/tradingApi");
        curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, command.length());
        curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, command.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, ("Key: " + APIkey).c_str());
        headers = curl_slist_append(headers, Sign.c_str());
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
/**
 * Далее идет описание паблик методов для взаимодействия с приватным API
 */

map<string, double> PrivateMethods::returnBalances() {
    string answerStr = doCommand("command=returnBalances&nonce=",PrivateMethods::Secret, PrivateMethods::APIkey);
    cout<<answerStr<<endl;
    nlohmann::json answerJson = nlohmann::json::parse(answerStr);
    map<string, double> balances;
    for (auto& x : answerJson.items())
    {
        char * ptrEnd;
        balances.insert({x.key(), strtod(to_string(x.value()).c_str(), &ptrEnd)});
    }
    return balances;
}
/*
string PrivateMethods::returnCompleteBalances() {
    return doCommand("command=returnCompleteBalances&nonce=",PrivateMethods::Secret, PrivateMethods::APIkey);
}
string PrivateMethods::returnDepositAddresses() {
    return doCommand("command=returnDepositAddresses&nonce=",PrivateMethods::Secret, PrivateMethods::APIkey);
}
string PrivateMethods::generateNewAddress() {
    return doCommand("command=generateNewAddress&nonce=",PrivateMethods::Secret, PrivateMethods::APIkey);
}
string PrivateMethods::returnDepositsWithdrawals(long long int start, long long int end) {      //start и end даты периода в UNIX формате
    string command = "command=returnDepositsWithdrawals&start=" + to_string(start) + "&end=" + to_string(end) + "&nonce=";
    return doCommand(command,PrivateMethods::Secret, PrivateMethods::APIkey);
}
string PrivateMethods::returnOpenOrders(pair<string, string> currencyPair) {
    string command = "command=returnOpenOrders&currencyPair=" + currencyPair.first + "_" + currencyPair.second + "&nonce=";
    return doCommand(command,PrivateMethods::Secret, PrivateMethods::APIkey);
}
string PrivateMethods::returnTradeHistory(pair<string, string> currencyPair) {
    string command = "command=returnTradeHistorys&currencyPair=" + currencyPair.first + "_" + currencyPair.second + "&nonce=";
    return doCommand(command,PrivateMethods::Secret, PrivateMethods::APIkey);
}
string PrivateMethods::returnOrderTrades(int orderNumber) {
    string command = "command=returnOrderTrades&orderNumber=" + to_string(orderNumber) + "&nonce=";
    return doCommand(command,PrivateMethods::Secret, PrivateMethods::APIkey);
}
string PrivateMethods::returnOrderStatus(int orderNumber) {
    string command = "command=returnOrderStatus&orderNumber=" + to_string(orderNumber) + "&nonce=";
    return doCommand(command,PrivateMethods::Secret, PrivateMethods::APIkey);
}
*/