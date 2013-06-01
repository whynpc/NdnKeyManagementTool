#include <ccnx-cpp.h>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <map>

using namespace std;
using namespace Ccnx;

class localServer
{
public:
	Ccnx::Name InterestBaseName;
	Ccnx::Wrapper handler;
	void OnInterest (Ccnx::Name name, Ccnx::Selectors selectors);

  Ccnx::Name parseSession(Ccnx::Name name);
  Ccnx::Name parseSharedKey(Ccnx::Name name, char *dataContent);
  int  init(std::string prefix);
private:
	std::string mystatus;
	OrganizerSession *oSession;
	ParticipantSession *pSession;
	int chunkSize;
  int chunkNum;        	  
  int version;
//private:
//	map<std::string,std::string> sessionInfo;
};
