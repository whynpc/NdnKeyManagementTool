#include <ccnx-cpp.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>
using namespace std;

class remoteServer
{
public:
  static remoteServer *instance() {
     if (_instance == NULL) {
     		_instance = new RemoteServer();
     }         
     return _instance;
  }   
	Ccnx::Name parseSharedKey(Ccnx::Name name，std::string &ret);
	Ccnx::Name parsePublicKey(Ccnx::Name name，std::string &ret);
	Ccnx::Name parseMembership(Ccnx::Name name);
	Ccnx::Name InterestBaseName;
	Ccnx::Wrapper handler;
	void OnInterest (Ccnx::Name name, Ccnx::Selectors selectors);
	int  init(std::string prefix);
private:
	int version; //shared-key version
	int chunkSize;
	int chunkNum;
	static remoteServer *_instance = NULL;
	OrganizerSession *oSession;
	ParticipantSession *pSession;
};
