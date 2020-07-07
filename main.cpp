
#include "API.h"

using namespace std;
int main(int argc, char* argv[]) {
PrivateMethods *n = new PrivateMethods();
PrivateMethods::completeBalances c;
//cout<<'\n'<<n->returnBalances()["BTC"];

c=n->returnCompleteBalances()["BTC"];

cout<<endl<<c.btcValue<<endl<<c.onOrders<<endl<<c.available;

}
