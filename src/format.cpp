#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function   :: Done
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  string time;
  
  //getting hours,minutes& seconds from given parameter

  int hour = seconds/3600;
  seconds = seconds%3600;
  
  int min = seconds/60;
  seconds = seconds%60;
  
  string h = std::to_string(hour);
  string m = std::to_string(min);
  string s = std::to_string(seconds);
  
  if(h.length()<2) h='0'+h;
  if(m.length()<2) m='0'+m;
  if(s.length()<2) s='0'+s;
  
  time = h+':'+m+':'+s;
  
  return time;
  
}
