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
    static localServer *instance() {
        if (!_instance) {
     		_instance = new localServer();
        }
        return _instance;
    }
	Ccnx::Name InterestBaseName;
	Ccnx::Wrapper handler;
	void OnInterest (Ccnx::Name name, Ccnx::Selectors selectors);

  Ccnx::Name parseSession(Ccnx::Name name);
  Ccnx::Name parseSharedKey(Ccnx::Name name, std::string &ret);
  int  init(std::string appName);
private:
  OrganizerSession *oSession;
  ParticipantSession *pSession;
  int chunkSize;
  int chunkNum;
  int version;
  static localServer *_instance;
//private:
//	map<std::string,std::string> sessionInfo;
};

#endif//local-server
