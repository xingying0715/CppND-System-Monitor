#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  	long tot_time = LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK);
  	long seconds = Process::UpTime();
  	float ucpu = (float) tot_time / (float)seconds;
  	return ucpu; 
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// : Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() const  { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a)  const {
	//return (stol(Process::Ram()) > stol(a.Ram())); 

	return Process::CpuUtilization() > a.CpuUtilization(); 
}