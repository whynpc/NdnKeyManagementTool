#include <ccnx-cpp.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>
using namespace std;

class remoteServer
{
public:
    
	Ccnx::Name parseSharedKey(Ccnx::Name name);
	Ccnx::Name parsePublicKey(Ccnx::Name name);
	Ccnx::Name parseMembership(Ccnx::Name name);
	Ccnx::Name InterestBaseName;
	Ccnx::Wrapper handler;
	void OnInterest (Ccnx::Name name, Ccnx::Selectors selectors);
	int  init(std::string prefix);
private:
	int version;
	int chunk;
};
