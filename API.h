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
#include <vector>
#include <map>
#include "json.hpp"
using namespace std;

static size_t write_data(char *ptr, size_t size, size_t nmemb, string* data);
class PrivateMethods{
private:
    string APIkey;
    string Secret;
    string getSettings(string paramName, string filename = "config.settings");
    string doCommand(string command, string Secret, string APIkey);
    string Sign(string command, string Secret);

    struct completeBalances{
        double available;
        double onOrders;
        double btcValue;
    };
    struct newAdress{
        bool success;
        string response;
    };
    struct depositsWithdrawals{
        struct deposit {
            long long int depositNumber;
            string currency;
            string address;
            double amount;
            long long int confirmations;
            string txid;
            long long int timestamp;
            string status;
        };
        struct withdrawal{
            long long int withdrawalNumber;
            string currency;
            string address;
            double amount;
            double fee;
            long long int timestamp;
            string status;
            string ipAddress;
            string paymentID;
        };
        struct adjustment{
            string currency;
            double amount;
            long long int timestamp;
            string status;
            string category;
            string adjustmentTitle;
            string adjustmentDesc;
            string adjustmentHelp;
        };
        vector<deposit> deposits;
        vector<withdrawal> withdrawals;
        vector<adjustment> adjustments;
    };
    struct openOrders{
        long long int orderNumber;
        string type;
        double rate;
        double startingAmount;
        double amount;
        double total;
        string date;
        bool margin;
    };
    struct singleTradeHistory{
        long long int globalTradeID;
        long long int tradeID;
        string date;
        string type;
        double rate;
        double amount;
        double total;
        long long int orderNumber;
    };
    struct allTradeHistory{
        long long int globalTradeID;
        long long int tradeID;
        string date;
        double rate;
        double amount;
        double total;
        double fee;
        long long int orderNumber;
        string type;
        string category;
    };
    struct orderTrades{
        long long int globalTradeID;
        long long int tradeID;
        pair<string, string> currencyPair;
        string type;
        double rate;
        double amount;
        double total;
        double fee;
        string date;
    };
    struct orderStatus{
        string status;
        double rate;
        double amount;
        pair<string, string> currencyPair;
        string date;
        double total;
        string type;
        double startingAmount;
    };
    struct buySellInfo{
        long long int orderNumber;
        struct resultingTrades{
            double amount;
            string date;
            double rate;
            double total;
            long long int tradeID;
            string type;
        };
        double fee;
        int64_t clientOrderId;
        pair<string, string> currencyPair;
        double tokenFee;
        string tokenFeeCurrency;
    };
    struct cancelOrderInfo{
        bool success;
        double amount;
        string message;
        int64_t clientOrderId;
    };
    struct cancelAllOrdersInfo{
        bool success;
        string message;
        vector<long long int> orderNumbers;
    };
    struct moveOrderInfo{
        bool success;
        double amount;
        string message;
        int64_t clientOrderId;
    };
public:
    PrivateMethods();
    string getAPIkey();
    string getSecret();
    map<string, double> returnBalances();
    map<string, completeBalances> returnCompleteBalances();
    map<string, string> returnDepositAddresses();
    newAdress generateNewAddress(string currency);
    depositsWithdrawals returnDepositsWithdrawals(long long int start, long long int end);
    vector<openOrders> returnOpenOrders(pair<string, string> currencyPair);
    multimap<pair<string, string>, openOrders> returnOpenOrders(string currencyPair);
    vector<singleTradeHistory> returnTradeHistory(pair<string, string> currencyPair);
    multimap<pair<string, string>, allTradeHistory> returnTradeHistory(string currencyPair);
    vector<orderTrades> returnOrderTrades(int orderNumber);
    orderStatus returnOrderStatus(int orderNumber);
    buySellInfo buy(pair<string, string> currencyPair, double rate, double amount,
            bool fillOrKill = 0, bool immediateOrCancel = 0,
            bool postOnly = 0, int64_t clientOrderId = NULL);
    buySellInfo sell(pair<string, string> currencyPair, double rate, double amount,
                bool fillOrKill = 0, bool immediateOrCancel = 0,
                bool postOnly = 0, int64_t clientOrderId = NULL);
    cancelOrderInfo cancelOrder(long long int orderNumber);
    cancelOrderInfo cancelOrder(int64_t clientOrderId);
    cancelAllOrdersInfo cancelAllOrders();
    cancelAllOrdersInfo cancelAllOrders(pair<string, string> currencyPair);


};
#endif //POLONIEXTRADEBOT_API_H
