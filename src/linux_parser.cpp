#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

//read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

//read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >>kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;

  float value_tot;
  float value_free;
  float MemUtilazation;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  
  if (filestream.is_open()) {
	while (std::getline(filestream, line)){
      	std::replace(line.begin(), line.end(), ':', ' ');
      	std::istringstream linestream(line);
      
      	 while (linestream >> key >> value) {
        if (key == "MemTotal") {
          value_tot = stof(value);
        }
        if (key == "MemFree"){
          value_free = stof(value);
        }
      }
    }
  }
    
  MemUtilazation = (value_tot - value_free) / value_tot;
  return MemUtilazation;
}

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string value;
  float uptime_f;
  long uptime_l;
  
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  
  if(filestream.is_open()){
  	std::getline(filestream, line);
    std::istringstream linestream (line);

    linestream >> value;
    
    uptime_f = stof (value);  
    uptime_l = (long) uptime_f;
  }
  return uptime_l; 
}

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); }

//Read and return the number of active jiffies for a PID// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  	string line;
  	string value;
  	long u_totjif = 0;
  
  	std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  	if(filestream.is_open()){
      while(std::getline(filestream, line)){
       std::istringstream linestream(line); 
       for(int i = 1; i < 18; ++i){
         linestream>> value;
         if(i == 14 || i == 15 || i == 16 || i == 17)
           u_totjif += stol (value);
       }
      }
     }
  	return u_totjif; 
}

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
	string cpu;
	string s_user, s_nice, s_system, s_idle, s_iowait, s_irq, s_softirq, s_steal, s_guest, s_guest_nice;
  	long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
	long active;
  	string line;
  	std::ifstream stream(kProcDirectory + kStatFilename);
  	if (stream.is_open()){
    	std::getline(stream, line);
    	std::istringstream linestream(line);
      	while (linestream >> cpu >> s_user >> s_nice >> s_system >> s_idle >> s_iowait >> s_irq >> s_softirq >> s_steal
               >>s_guest>> s_guest_nice){
          if(cpu == "cpu"){
        	user = stol(s_user);
 	 		nice = stol(s_nice);
 	 		system = stol(s_system);
  			idle = stol(s_idle);
 	 		iowait = stol(s_iowait);
 	 		irq = stol(s_irq);
 	 		softirq = stol(s_softirq);
  			steal = stol(s_steal);
  			guest = stol(s_guest);
  			guest_nice = stol(s_guest_nice);
  			active = user + nice + system + irq + softirq + steal;
      
      		return active;
          }
        }
      }

	return active;
}

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
	string cpu;
	string s_user, s_nice, s_system, s_idle, s_iowait, s_irq, s_softirq, s_steal, s_guest, s_guest_nice;
  	long  idle, iowait;
  	string line;
  	std::ifstream stream(kProcDirectory + kStatFilename);
  	if (stream.is_open()){
    	std::getline(stream, line);
    	std::istringstream linestream(line);
      	while (linestream >> cpu >> s_user >> s_nice >> s_system >> s_idle >> s_iowait >> s_irq >> s_softirq >> s_steal
               >> s_guest>> s_guest_nice){
        	if (cpu == "cpu"){
            	idle = stol(s_idle);
  				iowait = stol(s_iowait);
          		idle = idle + iowait;
          
          		return idle;
            }
          }
      }


	return idle;
}

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string value;
  string key;
  int tot_processes;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) {
	while (std::getline(filestream, line)){
      	std::istringstream linestream(line);
      
      	 while (linestream >> key >> value) {
        if (key == "processes") {
          tot_processes = stoi(value);
        }
      }
    }
  }
  return tot_processes; 
}

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string value;
  string key;
  int running_processes;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) {
	while (std::getline(filestream, line)){
      	std::istringstream linestream(line);
      
      	 while (linestream >> key >> value) {
        if (key == "procs_running") {
          running_processes = stoi(value);
        }
      }
    }
  }
  return running_processes; 
}

//Read and return the command associated with a process// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
	string line;
  	std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename );
  
  	if (filestream.is_open())
      std::getline(filestream, line);
  	return line; 
}

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
 	string line;
	string key;
  	int memKB;
  	int memMB;
  
  	std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename ); 

  	if (filestream.is_open()) {
	while (std::getline(filestream, line)){
      	std::istringstream linestream(line);
      
      	 while (linestream >> key >> memKB) {
        if (key == "VmSize:") {
          memMB = memKB / 1000;
        }
      }
    }
  }
	return to_string(memMB); 
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
	string line;
	string value;
	string key;
  string uid;
  
  	std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename ); 

  	if (filestream.is_open()) {
	while (std::getline(filestream, line)){
      	std::istringstream linestream(line);
      
      	 while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
	return uid; 
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) {
  	string uid = " ";
  	string x;
  	string user; 
  	string line;

  	std::ifstream filestream(kPasswordPath);
  	if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return user;
        }
      }
    }
  } else return "bad";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  	string line;
  	string value;
  	long int uptime_tick;
  	
  	std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename );
  	
  	if (filestream.is_open()) {
      while (std::getline(filestream, line)){
      	std::istringstream linestream(line);
        for(int i = 0; i < 22; ++i){
        	linestream >> value;
          	if(i == 21)
              uptime_tick = stol(value);
        }
      }
    }

  	return (LinuxParser::UpTime() - (uptime_tick / sysconf(_SC_CLK_TCK))); 
}