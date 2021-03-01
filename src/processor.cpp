#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  float utilization{0};
  long active_time = LinuxParser::ActiveJiffies();
  long idle_time = LinuxParser::IdleJiffies();
  long total_time = active_time + idle_time;
   utilization = 1.0 * (active_time)/(total_time);
  return utilization;
	}