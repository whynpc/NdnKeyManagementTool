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
    static localServer *instance() {
        if (_instance == NULL) {
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
  static localServer *_instance = NULL;
//private:
//	map<std::string,std::string> sessionInfo;
};
