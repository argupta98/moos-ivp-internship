/************************************************************/
/*    NAME: Arjun Gupta                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactorTester.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactorTester.h"
#include <vector>
#include <stdio.h>
#include <cstdlib>

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactorTester::PrimeFactorTester()
{
}

//---------------------------------------------------------
// Destructor

PrimeFactorTester::~PrimeFactorTester()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactorTester::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 
    double mtime = msg.GetTime();
    if(key=="PRIME_RESULT"){
      m_to_test.push_back(sval);
   }
  }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactorTester::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactorTester::Iterate()
{
  //iterate through list of terms to test, get strings
  for(int i=0; i<m_to_test.size();i++){
    bool same;
    string eval=m_to_test[i];
    unsigned long int original;
    vector<unsigned long int> primes;
    //splice string at commas and make list of terms
    vector<string> spltlist=parseString(eval,',');
    //iterate through terms and find "orig=" and "primes="
    for(int term=0; term<spltlist.size();term++){
      if(spltlist[term].find("orig=")<spltlist.size()){
	string orig_str=spltlist[term];
	biteStringX(orig_str,'=');
	original=strtoul(orig_str.c_str(),NULL,0);
      }
      else if(spltlist[term].find("primes=")<spltlist.size()){
	//splice the substring after primes at ":" and get list of numbers
	string prime_string=spltlist[term];
	biteStringX(prime_string,'=');
	vector<string> prime_string_list;
	prime_string_list=parseString(prime_string,':');
	for(int prime=0; prime<prime_string_list.size();prime++){
	  primes.push_back(strtoul(prime_string_list[prime].c_str(),NULL,0));
	}
      }
    }
    
    //multiply numbers together and see if they equal the number after "orig="
    unsigned long int result=1;
    for(int prime=0;prime<primes.size();prime++){
      result*=primes[prime];
    }
    same= (original==result);
    //post result
    string output=m_to_test[i]+",valid="+to_string(same);
    m_Comms.Notify("PRIME_RESULT_VALID",output);
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactorTester::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactorTester::RegisterVariables()
{
  Register("PRIME_RESULT", 0);
}

