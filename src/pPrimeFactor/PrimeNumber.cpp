#include <iostream>
#include <vector>
#include "PrimeNumber.h"
#include "MBUtils.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include <stdint.h>

using namespace std;

PrimeNumber::PrimeNumber(int val, int rec, string val_rep, double st_time, string usd)
{
  value=val;
  recieved=rec;
  calculated=0;
  value_rep=val_rep;
  time_start= st_time;
  start=2;
  completed=false;
  left=val;
  used_val=usd;
}
  
void PrimeNumber::GetPrimes(unsigned long int end)
{
  end+=start;
  bool recurred = false;
  
  if(left==1 and !completed){//base case, totally factored
    time_end=MOOSTime();//get timestamp
    completed=true;
  }
  
  unsigned long int upto=(unsigned long int)(sqrt(left));
  int fin = min(end, upto);
  if(!completed){
    for(int i=start; i<=end;i++){
      if(left%i==0){
	left=left/i;
	recurred= true;
	primes.push_back(i);
	start=2;
	GetPrimes(end-i);
	break;
      }   
    }
    if(!recurred){
      start=fin;
    }
  }
}

string PrimeNumber::format()
{
  string output;
  output="orig="+value_rep+",";
  output+="Used Value="+used_val+",";
  output+="recieved="+to_string(recieved)+",";
  output+="calculated="+to_string(calculated)+",";
  output+="solve_time="+to_string(time_end-time_start)+",";
  output+="primes=";
  for(int i=0; i<primes.size();i++){
    if(i!=primes.size()-1){
      output+=to_string(primes[i])+":";
    }
    else{
      output+=to_string(primes[i]);
    }
  }
  return output;
}
