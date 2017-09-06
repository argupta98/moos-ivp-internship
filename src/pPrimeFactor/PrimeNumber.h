#ifndef PRIMENUMBER
#define PRIMENUMBER

#include <iostream>
#include <vector>
#include <stdint.h>

using namespace std;

class PrimeNumber
{
  
 public: //member variables
  unsigned long int value;
  vector<unsigned long int> primes;
  int recieved;
  int calculated;
  string value_rep;
  double time_start;
  double time_end;
  int start;
  bool completed;
  unsigned long int left;
  string used_val;
  
 public://member functions
  PrimeNumber(int val, int rec, string val_rep, double st_time, string usd);
  string format();
  void GetPrimes(unsigned long int end);
  
};
#endif
