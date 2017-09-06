/************************************************************/
/*    NAME: Arjun Gupta                                     */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                 */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <list>
#include "MBUtils.h"
#include "PrimeFactor.h"
#include <cstdlib>
#include "PrimeNumber.h"
#include <stdint.h>
#include <sstream>

using namespace std;
int EVAL=500000;//number of iterations for primes per iterate() call. 
//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  m_num_queries=0;
  m_num_eval=0;
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString();
    if(key=="NUM_VALUE"){
      char *ptr;
      unsigned long int value=strtoul(sval.c_str(), NULL, 0); //convert from string to unsigned long int
      stringstream converter;
      converter<< value;
      string new_repr= converter.str();
      double start_time=MOOSTime();
      m_num_queries++;
      //Instantiate instance of PrimeNumber to hold all the data 
      PrimeNumber new_prime(value, m_num_queries, sval, start_time, new_repr);
      m_eval_queue.push_back(new_prime);
   }
  }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
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

bool PrimeFactor::Iterate()
{
  int eval=EVAL;
  if(m_eval_queue.size()>0){
     eval=int(EVAL/m_eval_queue.size());
  }
  //iterate through eval queue, run each thing for eval iterations
  list<PrimeNumber>::iterator p;
  for(p=m_eval_queue.begin();p!=m_eval_queue.end();){
    PrimeNumber& temp= *p;
    if(temp.completed==true){
      m_num_eval++;
      temp.calculated=m_num_eval;
      string result= temp.format();
      Notify("PRIME_RESULT",result);
      p=m_eval_queue.erase(p);
    }
    else{

      if(temp.primes.size()>0){
	//	Notify("PRIME_STATUS", temp.format());
      }
      temp.GetPrimes(eval);
      ++p;
    }
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
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

void PrimeFactor::RegisterVariables()
{
   Register("NUM_VALUE", 0);
}



/*
void PrimeFactor::FindPrimes(vector<unsigned long int> &primes, int num)
{
  unsigned long int iterate_to=(unsigned long int)(pow(current_eval,.5));
  for(unsigned long int i =2; i<iterate_to;i++){ 
    if(!m_got_new){//take a second to check if we have gotten any new numbers
      if(num%i==0){
	primes.push_back(i);
	PrimeFactor::FindPrimes(primes,num/i);
	bool prime=false;
	break;
      }
    }
    else{//if we get a new number 
      if(m_recent<iterate_to){
	//suspend and go to the smaller prime to be handled only if it is smaller than square root of this one
      PrimeFactor:Iterate();
      }
    }
  }
}
*/  
