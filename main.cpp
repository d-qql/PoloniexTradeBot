
#include "API.h"

using namespace std;
int main(int argc, char* argv[]) {
PrivateMethods *n = new PrivateMethods();
cout<<n->returnBalances()["BTC"];
cout<<n->getAPIkey();
cout<<n->getSecret();

}
