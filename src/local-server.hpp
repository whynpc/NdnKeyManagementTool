#include <ccnx-cpp.h>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <map>
#include "remote.hpp" 

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
	remote r;
//private:
//	map<std::string,std::string> sessionInfo;
};
