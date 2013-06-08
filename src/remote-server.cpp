#include <boost/lexical_cast.hpp>
#include <ccnx-cpp.h>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <vector>
#include "remote-server.hpp"
#include "organizersession.h"
#include "participantsession.h"
#include "context.h"

using namespace std;

/*
remoteServer remoteServer::instance() {
    if (_instance == NULL) {
        _instance = new remoteServer();
    }
    return _instance;
}
*/


int remoteServer::do_encrypt(char **to, char *from, unsigned char *key,int len)
{
    RSA *keypair;
    char *key2 = (char *)key;                                                
    d2i_RSAPublicKey(&keypair,  &(const unsigned char *)key, len);
//    cout <<RSA_size(keypair)<<endl;                                        
    *to = (char *)malloc(RSA_size(keypair));
    int encrypt_len = RSA_public_encrypt(strlen(from), (unsigned char*)from,
		(unsigned char*)(*to), keypair, RSA_PKCS1_OAEP_PADDING);
                                                                             
    return encrypt_len;
} 


Ccnx::Name remoteServer::parseSharedKey(Ccnx::Name name, std::string &ret,
	    OrganizerSession *oSession,	ParticipantSession *pSession){
    int flag;

     std::string consumer = name.getCompAsString(2); //consumer
    std::string producer = name.getCompAsString(1); //producer
    std::string endPoint = name.getCompAsString(3); //endpoint
    std::string action = name.getCompAsString(4); //action
//    std::string prefix = name.getCompAsString(0);

    int size = name.size();
    int version = 0;
    int chunkNum = 0;
    int chunkSize = 0;
    Ccnx::Name dataName = name;
    if (action.compare("fetch") == 0){
        std::string lastComponent = name.getCompAsString(size-2);
        if (boost::starts_with(lastComponent,"chunk="))
        {
            flag = 1;  //later packet
        }
        else{
            flag = 0;  //para
        }
        //  
        if (flag == 0){
        	  chunkSize = 0;
        	  chunkNum = 0;        	  
        	  if (oSession)
        	  {
                  oSession->recvFetchSharedKeyRemote(consumer, version, chunkNum,chunkSize, ret);
              }
            char *from = (char *)ret.c_str();
            char *to = NULL ;
            char *key = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDuSJtsqO38w2PQFrX7ZJZDZGP16hrnDhmoTqz3jk4d62e1ne2709ZxzMMIgIGEooR9xOHcBz9fUmzQu4k92KrFU7HEuNPRgtDlJlYiW49FMezn+AcOItMx0ec+wpVEbWNaBO4bIS9EkGYioXqK6LJ8FWE6HZIP9K4Z3rE/zV7W8wIDAQAB";
            do_encrypt(&to, from,(unsigned char *)key,512);
            ret = std::string(to);
            std::string tmp = "code=0,version=";
            std::string v = boost::lexical_cast <string>(version);
            tmp.append(v);
            tmp.append(",chunk=");
            std::string ch = boost::lexical_cast <string>(chunkSize);
            tmp.append(ch);
            dataName.appendComp(tmp);
        }
        else{
            dataName= name;
            vector<std::string> str1;
            boost::split(str1,lastComponent,boost::is_any_of("="));
            std::string versionComponent = name.getCompAsString(size-3);
            vector<std::string> str2;
            boost::split(str2,versionComponent,boost::is_any_of("="));
            version = atoi(str2[1].c_str());
            chunkNum = atoi(str1[1].c_str());
            if (oSession)
            {
                oSession->recvFetchSharedKeyRemote(consumer, version, chunkNum, chunkSize, ret);
            }
            char *from = (char *)ret.c_str();
            char *to = NULL ;
            //          get public key
            //           do_encrypt(&to, from,key,len);
        }
    }
    else{
        if (boost::starts_with(action,"update")){
            dataName.appendComp("code=0");
            vector<std::string> tmp;
            boost::split(tmp,action,boost::is_any_of("_"));            
            version = atoi(tmp[1].c_str());
            if (pSession)
            {
            	 pSession->recvRenewSharedKeyRemote(version);
            }
//           
//						orgsession->recvRenewSharedKeyRemote(version);
        }
    }
    return dataName;
};

Ccnx::Name remoteServer::parsePublicKey(Ccnx::Name name, std::string &ret,
	 OrganizerSession *oSession,	ParticipantSession *pSession){
    int flag;
//    std::string consumer = name.getCompAsString(1); //consumer
//    std::string producer = name.getCompAsString(2); //producer
 //   std::string endPoint = name.getCompAsString(3); //endpoint
  //  std::string action = name.getCompAsString(4); //action
    std::string consumer = name.getCompAsString(2); //consumer
    std::string producer = name.getCompAsString(1); //producer
    std::string endPoint = name.getCompAsString(3); //endpoint
    std::string action = name.getCompAsString(4); //action

    int size = name.size();
    int version = 0;
    int chunkNum = 0;
    int chunkSize = 0;
    Ccnx::Name dataName = name;
    if (action.compare("fetch") == 0){
        std::string lastComponent = name.getCompAsString(size-2);
        if (boost::starts_with(lastComponent,"chunk="))
        {
            flag = 1;
        }
        else{
            flag = 0;  //para
        }
        //  
        if (flag == 0){
        	  chunkSize = 0;
        	  chunkNum = 0;        	  
        	  if (oSession)
        	  {
						  oSession->recvFetchPublicKeyRemote(consumer, version, chunkNum,chunkSize, ret);
            }
            if (pSession)
        	  {
						  pSession->recvFetchPublicKeyRemote(consumer, version, chunkNum,chunkSize, ret);
            }
            std::string tmp = "code=0,version=";
            std::string v = boost::lexical_cast <string>(version);
            tmp.append(v);
            tmp.append(",chunk=");
            std::string ch = boost::lexical_cast <string>(chunkNum);
            tmp.append(ch);
            dataName.appendComp(tmp);
        }
        else{
            dataName= name;
            vector<std::string> str1;
            boost::split(str1,lastComponent,boost::is_any_of("="));
            std::string versionComponent = name.getCompAsString(size-3);
            vector<std::string> str2;
            boost::split(str2,versionComponent,boost::is_any_of("="));
            version = atoi(str2[1].c_str());
            chunkNum = atoi(str1[1].c_str());
            if (oSession)
        	  {
						  oSession->recvFetchPublicKeyRemote(consumer, version, chunkNum,chunkSize, ret);
            }
            if (pSession)
        	  {
						  pSession->recvFetchPublicKeyRemote(consumer, version, chunkNum,chunkSize, ret);
            }
        }
    }
    return dataName;
};

Ccnx::Name remoteServer::parseMembership(Ccnx::Name name,	 OrganizerSession *oSession,	ParticipantSession *pSession){
    std::string action = name.getCompAsString(4); //action
    std::string consumer = name.getCompAsString(2); //consumer
    Ccnx::Name dataName = name;
    if (action.compare("join") == 0 ||
        action.compare("accept") == 0 ||
        action.compare("reject") == 0){
        dataName.appendComp("code=0");
    }
    std::clog<<"action:  "<<action<<endl;
    if (action.compare("join") == 0){
//     	  std::string consumer = name.getCompAsString(1); //consumer
     	  if (oSession){
              std::clog<<"consumer   "<<consumer<<endl;
    			oSession->recvJoinRemote(consumer);
    		}
    }
    if (action.compare("accept") == 0){
    	if (pSession){
    		pSession->recvAcceptJoinRemote();
    	}
    }
    if (action.compare("reject") == 0){
    	if (pSession){
				pSession->recvRejectJoinRemote();
			}
    }
    return dataName;
};



void remoteServer::OnInterest (Ccnx::Name name, Ccnx::Selectors selectors){
    Ccnx::Name dataName;
    clog<<"on interest data name    "<<name<<endl;
    std::string consumer = name.getCompAsString(2); //consumer
    std::string producer = name.getCompAsString(1); //producer
    std::string endPoint = name.getCompAsString(3); //endpoint
    std::string action = name.getCompAsString(4); //action
    std::string prefix = name.getCompAsString(0);
    std::string app;
    std::string session;
    vector <string> app_session;
    boost::split(app_session, prefix,boost::is_any_of("_"));        	
    app = app_session[0];
    session = app_session[1];	
    OrganizerSession *oSession;
    ParticipantSession *pSession;
    Context::instance()->retrieveSession(app, session, &oSession, &pSession);    	
    std::string msg("");
    //resolving name
    if (endPoint.compare("shared-key") == 0){
        dataName = parseSharedKey(name,msg,oSession,pSession);
    }
    if (endPoint.compare("membership") == 0){
        dataName = parseMembership(name,oSession,pSession);
    }
    if (endPoint.compare("public-key") == 0){
        dataName = parsePublicKey(name, msg,oSession,pSession);
    }
    std::clog<<"dataname back to consumer  "<<dataName<<std::endl;
    std::clog<<"sent data msg" <<msg<<std::endl;
    
    handler.publishData (dataName, msg, 5);
}

int  remoteServer::init(std::string app, std::string session, std::string consumer){
	std::string prefix(app);
	prefix.append("_");
	prefix.append(session);
	Ccnx::Name interestBaseName = Ccnx::Name();
    interestBaseName.appendComp(prefix);
    interestBaseName.appendComp(consumer);
	handler.setInterestFilter (interestBaseName, boost::bind (&remoteServer::OnInterest, this, _1, _2));

    return 0;
}



