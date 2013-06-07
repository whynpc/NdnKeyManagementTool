#ifndef REMOTE
#define REMOTE

#include <ccnx-cpp.h>
#include <iostream>
#include <vector>
#include "organizersession.h"
#include "participantsession.h"
#include "context.h"
//#include "remote-client.hpp"

using namespace std;
using namespace Ccnx;


class remote
{
public:
    static remote& instance()
    {
        static remote _instance;
        return _instance;
    }
	int split( string& str, vector<string>& ret_, string sep);
	int init(std::string prefix,
             std::string consumer,std::string organizer,std::string
             endpoint,std::string action);
//    int do_decrypt(char *to, char *from, unsigned char *key,int keylen, int encrypt_len);
    //	Ccnx::Name sharedKeyOnData(Ccnx::Name name);
	void runDataCallback(Name name, Ccnx::PcoPtr pco);
    Ccnx::Name getBaseName(Ccnx::Name name);
	void runTimeoutCallback(Name interest, const Closure &closure, Selectors selectors);
    void fetchSharedKey(std::string app, std::string session,
                                std::string consumer,std::string organizer);
    void fetchPublicKey(std::string app, std::string session,
                                std::string consumer,std::string organizer);
    void updateSharedKey(std::string app, std::string session,
                                 std::string consumer,std::string organizer,int version);
    void joinMembership(std::string app, std::string session,
                                std::string consumer,std::string organizer);
    void acceptMembership(std::string app, std::string session,
                                  std::string consumer,std::string organizer);
    void rejectMembership(std::string app, std::string session,
                                  std::string consumer,std::string organizer);
private:
   remote() {}
   remote(remote const&);
   void operator=(remote const&);
   Ccnx::Name interestBaseName;
   Ccnx::Name interestName;
   Ccnx::Wrapper handler;
   static remote* _instance;
   OrganizerSession *oSession;
	 ParticipantSession *pSession;
//	 int chunk;
 //  std::string myname;
    //  std::string payload;
};

#endif //remote
