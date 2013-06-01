
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

void remote::fetchSharedKey(std::string app, std::string session,
                            std::string consumer,std::string organizer)
{
		std::string prefix(app);
		prefix.append("_");
		prefix.append(session);
    std::string endpoint("shared-key");
    std::string action("fetch");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::fetchPublicKey(std::string app, std::string session,
                            std::string consumer,std::string organizer)
{		
		std::string prefix(app);
		prefix.append("_");
		prefix.append(session);
    std::string endpoint("public-key");
    std::string action("fetch");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::updateSharedKey(std::string app, std::string session,
                             std::string consumer,std::string organizer)
{
 		std::string prefix(app);
		prefix.append("_");
		prefix.append(session);
		std::string endpoint("shared-key");
    std::string action("update");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::joinMembership(std::string app, std::string session,
                            std::string consumer,std::string organizer)
{
 		std::string prefix(app);
		prefix.append("_");
		prefix.append(session);
		std::string endpoint("membership");
    std::string action("join");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::acceptMembership(std::string app, std::string session,
                              std::string consumer,std::string organizer)
{
 		std::string prefix(app);
		prefix.append("_");
		prefix.append(session);
		std::string endpoint("membership");
    std::string action("accept");
    Init(prefix,consumer,organizer,endpoint,action);
}

void remote::rejectMembership(std::string app, std::string session,
                              std::string consumer,std::string organizer)
{
 		std::string prefix(app);
		prefix.append("_");
		prefix.append(session);
		std::string endpoint("membership");
    std::string action("reject");
    Init(prefix,consumer,organizer,endpoint,action);
}

int remote::init(std::string prefix,
                 std::string consumer,std::string organizer,std::string
                 endpoint,std::string action){
    
		Ccnx::Name interestBaseName = Ccnx::Name();
    interestBaseName.appendComp(prefix);
    interestBaseName.appendComp(consumer);
    interestBaseName.appendComp(organizer);
    interestBaseName.appendComp(endpoint);
    interestBaseName.appendComp(action); //action
    interestBaseName.appendComp("xxx"); //rand+auth_token/
    //    InterestBaseName = Ccnx::Name (interestName);
    Ccnx::Wrapper handler;
    handler.sendInterest (interestBaseName,
                          Ccnx::Closure (boost::bind (&remote::runDataCallback, this, _1, _2),
                                         boost::bind (&remote::runTimeoutCallback, this, _1, _2, _3)),
                          Ccnx::Selectors ().scope (Ccnx::Selectors::SCOPE_LOCAL_HOST));
    
    sleep (3);
    return 0;
    
}


Ccnx::Name remote::getBaseName(Ccnx::Name name)
{
    Ccnx::Name base = Ccnx::Name();
	  for (int i = 0; i <=4 ;i++)
	  {
	  	base.appendComp(name.getCompAsString(i));
	  }
	  return base;
}

void remote::runDataCallback(Name name, Ccnx::PcoPtr pco)
{
    Ccnx::BytesPtr content = pco->contentPtr ();
    std::string action = name.getCompAsString(4); //action
    std::string consumer = name.getCompAsString(1); //consumer
    std::string producer = name.getCompAsString(2); //producer
    std::string endPoint = name.getCompAsString(3); //endpoint
    std::string prefix = name.getCompAsString(0);
    std::string app;
    std::string session;
    vector <string> app_session;
    split(prefix, app_session, "_");        	
    app = app_session[0];
    session = app_session[1];
    
    int size = name.size();    
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
//        seqnum = 1;
    }
/*    else{
        flag = 0;
        std::string tmp = name.getCompFromBackAsString(1);
        vector <string> chunkTmp;
        split(tmp, chunkTmp, "=");
        seqnum = atoi(chunkTmp[1].c_str());
//        cout<<"seqnum chunk"<<seqnum<<"  "<<chunk<<endl;
    }
 */
    OrganizerSession *oSession;
		ParticipantSession *pSession;
		Context::instance()->retrieveSession(app, session, &oSession, &pSession);

    if (action.compare("fetch") == 0)
    {
    	if (endPoint.compare("public-key") == 0)
    	{
				if (pSession)  
				{
    			pSession->recvPublicKeyRemote(version, sequm, chunk,
        	string ((char*)Ccnx::head (*content), content->size ()));    			
        }
				if (oSession)  
				{
    			oSession->recvPublicKeyRemote(version, sequm, chunk,
        	string ((char*)Ccnx::head (*content), content->size ()));    			
        }		  		
    	}
    	 
   		if (endPoint.compare("shared-key") == 0)
      {
    		pSession->recvSharedKeyRemote(version, sequm, chunk,
        string ((char*)Ccnx::head (*content), content->size ()));    			
      }
    }

    for (seqnum = 1; seqnum < chunk; seqnum++)
    {
	    interestName = getBaseName(name);
  	  interestName.appendComp("v="+boost::lexical_cast <string>(version));
    	interestName.appendComp("chunk="+(boost::lexical_cast <string>(seqnum+1)));
    	interestName.appendComp("xxx");
      handler.sendInterest (interestName,
                          Ccnx::Closure (boost::bind (&remote::runDataCallback, this, _1, _2),
                                         boost::bind (&remote::runTimeoutCallback, this, _1, _2, _3)),
                          Ccnx::Selectors ().scope (Ccnx::Selectors::SCOPE_LOCAL_HOST));  
    }
}

void remote::runTimeoutCallback(Name interest, const Closure &closure, Selectors selectors)
{
    // re-express interest
    handler.sendInterest (interest, closure, selectors);
}

