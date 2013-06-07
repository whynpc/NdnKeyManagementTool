
#ifndef REMOTE_SERVER
#define REMOTE_SERVER

#include <ccnx-cpp.h>
#include <iostream>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "organizersession.h"
#include "participantsession.h"
#include "context.h"

using namespace std;

class remoteServer
{
public:
//    static remoteServer *_instance;
    static remoteServer& instance()
    {
        static remoteServer _instance;
        return _instance;
    }
    remoteServer() {}
    remoteServer(remoteServer const&);
    void operator=(remoteServer const&);
    //  int do_encrypt(char **to, char *from, unsigned char *key,int len);
//  int do_decrypt(char *to, char *from, unsigned char *key,int len);
  Ccnx::Name parseSharedKey(Ccnx::Name name, std::string &ret, OrganizerSession *oSession,	ParticipantSession *pSession);
	Ccnx::Name parsePublicKey(Ccnx::Name name, std::string &ret, OrganizerSession *oSession,	ParticipantSession *pSession);
	Ccnx::Name parseMembership(Ccnx::Name name, OrganizerSession *oSession,	ParticipantSession *pSession);
	Ccnx::Name InterestBaseName;
	void OnInterest (Ccnx::Name name, Ccnx::Selectors selectors);
	int  init(std::string app, std::string session, std::string consumer);
private:
	Ccnx::Wrapper handler;
    //   void
//    std::string consumer;
//    std::string producer;
//    std::string endPoint;
 //   std::string action;
 //   std::string prefix;
//	static remoteServer *_instance;
//	OrganizerSession *oSession;
//	ParticipantSession *pSession;
};

#endif//remote-server
