#include <ccnx-cpp.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>

//#include "remote-client.hpp"

using namespace std;
using namespace Ccnx;


class remote
{
public:
	int chunk;
	int split( string& str, vector<string>& ret_, string sep);
	int Init(std::string prefix,
             std::string consumer,std::string organizer,std::string
             endpoint,std::string action);
    //	Ccnx::Name sharedKeyOnData(Ccnx::Name name);
	void runDataCallback(Name name, Ccnx::PcoPtr pco);
	void runTimeoutCallback(Name interest, const Closure &closure, Selectors selectors);
	void fetchSharedKey(std::string prefix,
                        std::string consumer,std::string organizer);
	void fetchPublicKey(std::string prefix,
                        std::string consumer,std::string organizer);
	void updateSharedKey(std::string prefix,
                         std::string consumer,std::string organizer);
    void joinMembership(std::string prefix,
                        std::string consumer,std::string organizer);
	void acceptMembership(std::string prefix,
                          std::string consumer,std::string organizer);
    void rejectMembership(std::string prefix,
                          std::string consumer,std::string organizer);
private:
	Ccnx::Name InterestBaseName;
    Ccnx::Wrapper handler;
    //  std::string payload;
};