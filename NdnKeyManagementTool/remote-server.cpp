#include <ccnx-cpp.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>
#include "remote-server.hpp"

using namespace std;

Ccnx::Name remoteServer::parseSharedKey(Ccnx::Name name){
    int flag;
    std::string action = name.getCompAsString(4); //action
    int size = name.size();
    Ccnx::Name dataName = name;
    if (action.compare("fetch") == 0){
        std::string lastComponent = name.getCompAsString(size-2);
        if (boost::starts_with(lastComponent,"chunk="))
        {
            flag = 1;
        }
        else{
            flag = 0;
        }
        //            cout<<"flag:  "<<flag<<endl;
        if (flag == 0){
            //TODO: get version and chunk number
            version = 1;
            chunk = 10;
            std::string tmp = "code=0,version=";
            std::string v = boost::lexical_cast <string>(version);
            tmp.append(v);
            tmp.append(",chunk=");
            std::string ch = boost::lexical_cast <string>(chunk);
            tmp.append(ch);
            dataName.appendComp(tmp);
        }
        else{
            dataName= name;
        }
    }
    else{
        if (boost::starts_with(action,"update")){
            dataName.appendComp("code=0");
        }
    }
    return dataName;
};

Ccnx::Name remoteServer::parsePublicKey(Ccnx::Name name){
    return parseSharedKey(name); //same as shared key
};

Ccnx::Name remoteServer::parseMembership(Ccnx::Name name){
    std::string action = name.getCompAsString(4); //action
    Ccnx::Name dataName = name;
    if (action.compare("join") == 0 ||
        action.compare("accept") == 0 ||
        action.compare("reject") == 0){
        dataName.appendComp("code=0");
    }
    return dataName;
};



void remoteServer::OnInterest (Ccnx::Name name, Ccnx::Selectors selectors){
    Ccnx::Name dataName;
    //    cout<<name<<endl;
    std::string endPoint = name.getCompAsString(3); //endpoint
    if (endPoint.compare("shared-key") == 0){
        dataName = parseSharedKey(name);
    }
    if (endPoint.compare("membership") == 0){
        dataName = parseMembership(name);
    }
    if (endPoint.compare("public-key") == 0){
        dataName = parsePublicKey(name);
    }
    //    ServerCrypto sc;
    std::string msg("abcd");
    //    unsigned char *encMsg = NULL;
    //	  int encMsgLen = sc.rsaEncrypt(msg, &encMsg);
    //	  cout<<encMsg<<endl;
    //TODO: fetch file if needed; change parse functions
    ostringstream os;
    os << "C++ LINE #"  << endl;
    cout<<"dataName   "<<dataName<<endl;
    handler.publishData (dataName, msg, 5);
}

int  remoteServer::init(std::string prefix){
	
	InterestBaseName.appendComp(prefix);
	cout<<"interstbasename  "<<InterestBaseName<<endl;
	handler.setInterestFilter (InterestBaseName, boost::bind (&remoteServer::OnInterest, this, _1, _2));
    while (true)
    {
        sleep (1);
    }
    return 0;
}
