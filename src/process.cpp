#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;


  Process::Process(int pid) {
  pid_ = pid;
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
  cpuUtilization_ = Process::CpuUtilization();
  ram_=   LinuxParser::Ram(pid_);
  upTime_= LinuxParser::UpTime(); 
  LinuxParser::UpTime(pid_);
}


// TODO: Return this process's ID  :: Done
int Process::Pid() { 
    //The process identifier (PID) is accessible from the /proc directory. 
    return pid_;
 }

// TODO: Return this process's CPU utilization :: Done
float Process::CpuUtilization() { 
string line,value;
vector<string> data;
  
int Hertz = sysconf(_SC_CLK_TCK);
    std::ifstream filestream(LinuxParser::kProcDirectory+to_string(pid_)+LinuxParser::kStatFilename);
    if(filestream.is_open()){
        std::getline(filestream,line);
        std::istringstream linestream(line) ; 
        for (int i = 0; i < 22 ; i++)
        {
            linestream >> value ; 
            data.push_back(value);

        }
    }
int totalTime = std::stoi(data[13])+std::stoi(data[14])+std::stoi(data[15])+std::stoi(data[16]);
int seconds = LinuxParser::UpTime()-(std::stoi(data[21])/Hertz);
float cpuU = (((float)totalTime/(float)Hertz)/(float)seconds);

return cpuU ;
 }

// TODO: Return the command that generated this process :: Done
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization :: Done
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process :: Done
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds) :: Done
long int Process::UpTime() { return upTime_; }

// TODO: Overload the "less than" comparison operator for Process objects :: Done
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
     return (this->cpuUtilization_ > a.cpuUtilization_);
 }
