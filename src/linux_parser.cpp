#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel>>version;
  }
  return version;
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

// TODO: Read and return the system memory utilization  :: Done
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float memFree = 0;
  float memTotal = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = std::stof(value);
      }else if (key == "MemFree:"){
        memFree = std::stof(value);
      }
    }
  }
  float utilization = ((memTotal - memFree) / memTotal);
  return utilization;
}

// TODO: Read and return the system uptime :: Done
long LinuxParser::UpTime() { 
  string uptime;
  string line;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  long time = std::stol(uptime);
  return time;
 }

// TODO: Read and return the number of jiffies for the system :: Done
long LinuxParser::Jiffies() { 
   return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK); 
 }

// TODO: Read and return the number of active jiffies for a PID :: Done
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string jiffies;
  string line;
  //jiffies will be calculated through /proc/[pid]/stat
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    std::istringstream linestream(line);
    for (int i=0; i<22; i++){
      linestream >> jiffies;
    }
  }
  return std::stol(jiffies);

}

// TODO: Read and return the number of active jiffies for the system :: Done
long LinuxParser::ActiveJiffies() { 
  string line;
  string key,user,nice,system,idle,iowait, irq, softirq, steal,guest ,guest_nice;
  long response;

  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice) {
        if (key == "cpu") {
          response = std::stol(user)+std::stol(nice)+std::stol(system)+std::stol(idle)+std::stol(iowait)+std::stol(irq)+std::stol(softirq)+std::stol(steal)+std::stol(guest)+std::stol(guest_nice);
          return response;}
      }
    }
  }
  return response;
 }

// TODO: Read and return the number of idle jiffies for the system :: Done
long LinuxParser::IdleJiffies() { 
  string line;
  string key,user,nice,system,idle,iowait, irq, softirq, steal,guest ,guest_nice;
  long idlejiffies;

  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice) {
        if (key == "cpu") {
          idlejiffies = std::stol(idle)+std::stol(iowait);
          return idlejiffies;}
      }
    }
  }
  return idlejiffies;
  
  return 0; }

// TODO: Read and return CPU utilization :: Done
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, number;
  vector<string> utilization{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> number){
      utilization.push_back(number);
    }
  }
  return utilization;
 }

// TODO: Read and return the total number of processes :: Done
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  int processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = std::stoi(value);
        return processes;
      }
    }
  }
  return processes;
 }

// TODO: Read and return the number of running processes :: Done
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  int running = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        running = std::stoi(value);
        return running;
      }
    }
  }
  return running;
 }

// TODO: Read and return the command associated with a process :: Done
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  //Linux stores the command used to launch the function in the /proc/[pid]/cmdline file.
  //reading command from file
  string command;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, command);
    std::istringstream linestream(command);
    return command;
  }
  return command;

 }

// TODO: Read and return the memory used by a process :: Done
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  //using command /proc/[PID]/status. we get VmSize in KB.
  string line, key, value;
  int ramKb;
  string ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:"){
        ramKb = stoi(value);
        ram = to_string(ramKb / 1000);
        return ram;
      }
    }
  }
  return ram;
 }

// TODO: Read and return the user ID associated with a process :: Done
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  //user id is UID for a process is stored in /proc/[PID]/status.
  string line, key, value, uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:"){
        return value;
      }
    }
  }
  return uid;

 }

// TODO: Read and return the user associated with a process :: Done
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
//location /etc/passwd contains the information necessary to match the UID to a username.   

  string uid = LinuxParser::Uid(pid);
  string line, filler, value, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> filler >> value;
      if (value  ==  uid){
        return user;
      }
    }
  }
  return user;
   
}

// TODO: Read and return the uptime of a process :: Done
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  //overloded uptime()
  string line,value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int index = 1;
    while (linestream >> value && index < 22) {
      ++index;
    }
  }
  return std::stol(value) / sysconf(_SC_CLK_TCK);
 }