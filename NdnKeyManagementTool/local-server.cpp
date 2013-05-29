#include <ccnx-cpp.h>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <map>
#include "local-server.hpp" 

using namespace std;
using namespace Ccnx;

Ccnx::Name localServer::parseSession(Ccnx::Name name){
    std::string action = name.getCompAsString(5); //action
    Ccnx::Name dataName = name;
    if (boost::starts_with(action,"create="))
    {
      vector<std::string> tmp;
      	//TODO: add to map
//      boost::split(tmp,action,boost::is_any_of("="));
 //   	sessionInfo.insert(pair<std::string,std::string>(tmp[1],""));
    	dataName.appendComp("code=0");
    }
    if (boost::starts_with(action,"destroy="))
    {
      vector<std::string> tmp;
      //TODO: add to map
//      boost::split(tmp,action,boost::is_any_of("="));
 //     sessionInfo.erase(tmp[1]);
    	dataName.appendComp("code=0");
    }
    if (boost::starts_with(action,"join="))
    {
    	//TODO: send join request to remote
    }
    return dataName;
}

Ccnx::Name localServer::parseSharedKey(Ccnx::Name name, 
																	char *dataContent)
{
    std::string action = name.getCompAsString(5); //action
    Ccnx::Name dataName = name;
    if (boost::starts_with(action,"create="))
    {
      vector<std::string> tmp;
      boost::split(tmp,action,boost::is_any_of("="));
      //TODO: generate a shared-key
      //TODO: update shared-key      
//      sessionInfo.erase(tmp[1]);
//    	sessionInfo.insert(pair<std::string,std::string>(tmp[1],""));
		  dataContent = "xxxx"; //shared-key
    	dataName.appendComp("code=0");
    }
    if (boost::starts_with(action,"update="))
    {
      vector<std::string> tmp;
      boost::split(tmp,action,boost::is_any_of("="));
      //TODO: generate a shared-key
      //TODO: update shared-key      
//      sessionInfo.erase(tmp[1]);
//    	sessionInfo.insert(pair<std::string,std::string>(tmp[1],""));
		  dataContent = "xxxx"; //shared-key
    	dataName.appendComp("code=0");
    }
    if (action.compare("fetch") == 0)
    {
    	//TODO: send fetch request to remote
    	
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
        dataName = parseSharedKey(name,&dataContent);
  }
    //TODO: fetch file if needed; change parse functions
   ostringstream os;
  os << "C++ LINE #"  << endl;
  cout<<"dataName   "<<dataName<<endl;
  handler.publishData (dataName, os.str (), 5);
}

int  localServer::init(std::string prefix){	
	
	InterestBaseName.appendComp(prefix);
	cout<<"interstbasename  "<<InterestBaseName<<endl;
	handler.setInterestFilter (InterestBaseName, boost::bind (&remoteServer::OnInterest, this, _1, _2));
  while (true)
    {
      sleep (1);
    }
  return 0;
}