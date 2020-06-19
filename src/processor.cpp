#include "processor.h"
#include "linux_parser.h"
#include <string>

using std::string;
using std::stoi;

// TODO: Return the aggregate CPU utilization  :: Done
float Processor::Utilization() { 
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }

  int idletime = stoi(idle) + stoi(iowait);
  int nonidletime = stoi(user) + stoi(nice) + stoi(system) + stoi(irq) + stoi(softirq) + stoi(steal);
  float totaltime = idletime + nonidletime;
  
  return (totaltime - idletime)/totaltime;
 }