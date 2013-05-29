#include <remote.hpp>
#include <ccnx-cpp.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <vector>
#include "remote.hpp"
using namespace std;


int remote::split( string& str, vector<string>& ret_, string sep)
{
    if (str.empty())
    {
        return 0;
    }
    
    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;
    
    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }
        
        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

void remote::fetchSharedKey(std::string prefix,
                            std::string consumer,std::string organizer)
{
    std::string endpoint("shared-key");
    std::string action("fetch");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::fetchPublicKey(std::string prefix,
                            std::string consumer,std::string organizer)
{
    std::string endpoint("public-key");
    std::string action("fetch");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::updateSharedKey(std::string prefix,
                             std::string consumer,std::string organizer)
{
    std::string endpoint("shared-key");
    std::string action("update");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::joinMembership(std::string prefix,
                            std::string consumer,std::string organizer)
{
    std::string endpoint("membership");
    std::string action("join");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::acceptMembership(std::string prefix,
                              std::string consumer,std::string organizer)
{
    std::string endpoint("membership");
    std::string action("accept");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::rejectMembership(std::string prefix,
                              std::string consumer,std::string organizer)
{
    std::string endpoint("membership");
    std::string action("reject");
    Init(prefix,consumer,organizer,endpoint,action);
}

int remote::Init(std::string prefix,
                 std::string consumer,std::string organizer,std::string
                 endpoint,std::string action){
    
    InterestBaseName = Ccnx::Name();
    InterestBaseName.appendComp(prefix);
    InterestBaseName.appendComp(consumer);
    InterestBaseName.appendComp(organizer);
    InterestBaseName.appendComp(endpoint);
    InterestBaseName.appendComp(action); //action
    InterestBaseName.appendComp("xxx"); //rand+auth_token/
    //    InterestBaseName = Ccnx::Name (interestName);
    cerr << ">> C++ " << InterestBaseName << endl;
    Ccnx::Wrapper handler;
    handler.sendInterest (InterestBaseName,
                          Ccnx::Closure (boost::bind (&remote::runDataCallback, this, _1, _2),
                                         boost::bind (&remote::runTimeoutCallback, this, _1, _2, _3)),
                          Ccnx::Selectors ().scope (Ccnx::Selectors::SCOPE_LOCAL_HOST));
    
    sleep (3);
    return 0;
    
}


void remote::runDataCallback(Name name, Ccnx::PcoPtr pco)
{
    Ccnx::BytesPtr content = pco->contentPtr ();
    // TODO: call upper layer
    
    int size = name.size();
    cout<<"data name"<<name<<endl;
    std::string lastComponent =name.getCompAsString(size-1);
    vector <string> splitPara;
    split(lastComponent, splitPara, ",");
    if (splitPara.size() == 1 && splitPara[0].compare("code=0") == 0)
        return;
    
    int code;
    int version;
   	int flag; //1: first packet 0:later packets
    int seqnum = 0;
    
    if (splitPara.size() == 3){
        flag = 1;
        vector <string> tmp1,tmp2,tmp3;
        split(splitPara[0], tmp1, "=");
        code = atoi(tmp1[1].c_str());
        split(splitPara[1], tmp2, "=");
        version = atoi(tmp2[1].c_str());
        split(splitPara[2], tmp3, "=");
        chunk = atoi(tmp3[1].c_str());
        //      cout<<splitPara[0]<<"  "<<splitPara[1]<<"  "<<splitPara[2]<<endl;
        //     cout<<code<<"  "<<version<<"  "<<chunk<<endl;
        seqnum = 1;
    }
    else{
        flag = 0;
        std::string tmp = name.getCompFromBackAsString(1);
        vector <string> chunkTmp;
        split(tmp, chunkTmp, "=");
        seqnum = atoi(chunkTmp[1].c_str());
        cout<<"seqnum chunk"<<seqnum<<"  "<<chunk<<endl;
        if (seqnum >= chunk){
            return;
        }
    }
    Ccnx::Name interestName;
    
    interestName = InterestBaseName;
    interestName.appendComp("v="+boost::lexical_cast <string>(version));
    interestName.appendComp("chunk="+(boost::lexical_cast <string>(seqnum+1)));
    interestName.appendComp("xxx");
    
    cerr << ">> interest name " << interestName << endl; // a shortcut to construct name
    handler.sendInterest (interestName,
                          Ccnx::Closure (boost::bind (&remote::runDataCallback, this, _1, _2),
                                         boost::bind (&remote::runTimeoutCallback, this, _1, _2, _3)),
                          Ccnx::Selectors ().scope (Ccnx::Selectors::SCOPE_LOCAL_HOST));
    
}

void remote::runTimeoutCallback(Name interest, const Closure &closure, Selectors selectors)
{
    // re-express interest
    handler.sendInterest (interest, closure, selectors);
}
