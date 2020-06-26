
#include "API.h"

using namespace std;
int main(int argc, char* argv[]) {
PrivateMethods *n = new PrivateMethods();
string k = n->returnBalances();
cout<<n->getAPIkey();
cout<<n->getSecret();
cout<<k;
}
