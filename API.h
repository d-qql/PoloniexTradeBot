//
// Created by d-qql on 25.06.2020.
//

#ifndef POLONIEXTRADEBOT_API_H
#define POLONIEXTRADEBOT_API_H
#include <iostream>
#include <curl/curl.h>
#include <string>
#include "hmac.hpp"
#include <ctime>
#include <fstream>
using namespace std;

static size_t write_data(char *ptr, size_t size, size_t nmemb, string* data);
class PrivateMethods{
private:
    string APIkey;
    string Secret;
    string getSettings(string paramName, string filename = "config.settings");
    string doCommand(string command, string Secret, string APIkey);
    string Sign(string command, string Secret);
public:
    string getAPIkey();
    string getSecret();
    string returnBalances();
    string returnCompleteBalances();
    string returnDepositAddresses();
    string generateNewAddress();
    string returnDepositsWithdrawals(int start, int end);
    string returnOpenOrders(string currencyPair);
    string returnTradeHistory(string currencyPair);
    string returnOrderTrades(int orderNumber);
    string returnOrderStatus(int orderNumber);

};
#endif //POLONIEXTRADEBOT_API_H
