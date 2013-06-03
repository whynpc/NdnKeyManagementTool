#include <ccnx-cpp.h>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>
#include <vector>
#include <map>
#include "local-server.hpp" 
#include "application.h"
#include "organizersession.h"
#include "participantsession.h"
#include "context.h"

using namespace std;
using namespace Ccnx;

Ccnx::Name localServer::parseSession(Ccnx::Name name){
  std::string action = name.getCompAsString(6); //action
  std::string organizer = name.getCompAsString(4); //organizer
  std::string participant = name.getCompAsString(5); //participant
  std::string appname = name.getCompAsString(2); //app
  Ccnx::Name dataName = name;
  Application *application;
  Context::instance()->retrieveApplication(appname, &application);
  	 	
  vector<std::string> tmp;
  std::string session = tmp[1];
  boost::split(tmp,action,boost::is_any_of("_"));            
  if (boost::starts_with(action,"create="))
    {
      dataName.appendComp("code=0");
      application->recvCreateOrganizerSession(session,organizer);
    }
  if (boost::starts_with(action,"participate="))
    {
      dataName.appendComp("code=0");
      application->recvCreateParticipantSession(session,participant,organizer);
    }
  if (boost::starts_with(action,"destroy="))
    {
      dataName.appendComp("code=0");
      application->recvDestroyOrganizerSession(session);
    }
  if (boost::starts_with(action,"quit="))
    {
      dataName.appendComp("code=0");
      application->recvDestroyParticipantSession(session);
    }
  if (boost::starts_with(action,"join="))
    {
      Context::instance()->retrieveSession(appname, session, &oSession, &pSession);
			if (pSession)
			{
				pSession->recvJoinLocal();
			}
      dataName.appendComp("code=0");
    }
  return dataName;
}

Ccnx::Name localServer::parseSharedKey(Ccnx::Name name, 
                                       std::string &ret)
{
	std::string appname = name.getCompAsString(2); //app
  std::string action = name.getCompAsString(7); //action
  Ccnx::Name dataName = name;
  int size = name.size();
  vector<std::string> tmp;
  boost::split(tmp,action,boost::is_any_of("="));
  std::string session = tmp[1];
  Context::instance()->retrieveSession(appname, session, &oSession, &pSession);
  if (boost::starts_with(action,"create="))
    {
      if (oSession)
      {
      	oSession->recvCreateSharedKeyLocal();
      }
      dataName.appendComp("code=0");
    }
  if (boost::starts_with(action,"update="))
    {
      int version = atoi(tmp[2].c_str());
      if (oSession)
      {
      	oSession->recvRenewSharedKeyLocal(version);
      }
 //     orgsession->recvRenewSharedKeyLocal(version);
      dataName.appendComp("code=0");
    }
  if (action.compare("fetch") == 0)
    {
        std::string lastComponent = name.getCompAsString(size-2);
        int flag = 0;
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
        	  version;
        	  if (oSession)
        	  {
						  oSession->recvFetchSharedKeyLocal(version, chunkNum, chunkSize, ret);
            }
        	  if (pSession)
        	  {
						  pSession->recvFetchSharedKeyLocal(version, chunkNum, chunkSize, ret);
            }
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
						  oSession->recvFetchSharedKeyLocal(version, chunkNum, chunkSize, ret);
            }
        	  if (pSession)
        	  {
						  pSession->recvFetchSharedKeyLocal(version, chunkNum, chunkSize, ret);
            }
        }

    }
  return dataName;
}

void localServer::OnInterest (Ccnx::Name name, Ccnx::Selectors selectors){
  Ccnx::Name dataName;
  //    cout<<name<<endl;
  std::string endPoint = name.getCompAsString(4); //endpoint
  std::string dataContent;
  if (endPoint.compare("session") == 0){
    dataName = parseSession(name);
  }
  if (endPoint.compare("shared-key") == 0){
     dataName = parseSharedKey(name, dataContent);
  }
  handler.publishData (dataName, dataContent, 5);
}

int  localServer::init(std::string appName){

	Ccnx::Name interestBaseName = Ccnx::Name();
    interestBaseName.appendComp("Local");
    interestBaseName.appendComp("KeyManagementTool");
    interestBaseName.appendComp(appName);
    handler.setInterestFilter (interestBaseName, boost::bind (&localServer::OnInterest, this, _1, _2));
    while (true)
    {
      sleep (1);
    }
    return 0;
}

