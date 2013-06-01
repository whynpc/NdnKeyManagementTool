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
    static remote *instance() {
        if (_instance == NULL) {
     		_instance = new remote();
        }
        return _instance;
    }
	int split( string& str, vector<string>& ret_, string sep);
	int init(std::string prefix,
             std::string consumer,std::string organizer,std::string
             endpoint,std::string action);
    //	Ccnx::Name sharedKeyOnData(Ccnx::Name name);
	void runDataCallback(Name name, Ccnx::PcoPtr pco);
    Ccnx::Name getBaseName(Ccnx::Name name);
	void runTimeoutCallback(Name interest, const Closure &closure, Selectors selectors);
    void fetchSharedKey(std::string app, std::string session,
                                std::string consumer,std::string organizer);
    void fetchPublicKey(std::string app, std::string session,
                                std::string consumer,std::string organizer);
    void updateSharedKey(std::string app, std::string session,
                                 std::string consumer,std::string organizer);
    void joinMembership(std::string app, std::string session,
                                std::string consumer,std::string organizer);
    void acceptMembership(std::string app, std::string session,
                                  std::string consumer,std::string organizer);
    void rejectMembership(std::string app, std::string session,
                                  std::string consumer,std::string organizer);
private:
//	 Ccnx::Name InterestBaseName;
   Ccnx::Wrapper handler;
//   OrganizerSession *orgsession;
//	 ParticipantSession *partsession;
//	 int chunk;
 //  std::string myname;
    //  std::string payload;
};