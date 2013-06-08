#ifndef LOCAL_SERVER
#define LOCAL_SERVER

#include <ccnx-cpp.h>
#include <iostream>
#include <vector>
#include <map>
#include "application.h"
#include "organizersession.h"
#include "participantsession.h"

using namespace std;
using namespace Ccnx;

class localServer
{
public:
    static localServer& instance()
    {
        static localServer _instance;
        return _instance;
    }
    localServer() {}
    localServer(localServer const&);
    void operator=(localServer const&);
	void OnInterest (Ccnx::Name name, Ccnx::Selectors selectors);
    Ccnx::Name parseSession(Ccnx::Name name);
    Ccnx::Name parseSharedKey(Ccnx::Name name, std::string &ret);
    int  init(std::string appName);
private:
	Ccnx::Name InterestBaseName;
    OrganizerSession *oSession;
    ParticipantSession *pSession;
    Ccnx::Wrapper handler;
    int chunkSize;
    int chunkNum;
    int version;
    //private:
    //	map<std::string,std::string> sessionInfo;
};

#endif//local-server
