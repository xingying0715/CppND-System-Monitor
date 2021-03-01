#include <string>

#include "format.h"
#include "system.h"
#define HOUR 3600
#define MIN 60

using std::string;
using std::to_string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS


string format_digit(long val){
    string two_digit_val{""};
    if (val < 10){
        two_digit_val = "0" + std::to_string(val);
    }
    else{
        two_digit_val = to_string(val);
    }
    return two_digit_val;
}


string Format::ElapsedTime(long times) {

  	long hour = times/HOUR;
  	long min = times%HOUR/MIN;
  	long sec = times%HOUR%MIN;
  	string time = format_digit(hour) + ":" +  format_digit(min) + ":" + format_digit(sec);  
      
  return time; 

}