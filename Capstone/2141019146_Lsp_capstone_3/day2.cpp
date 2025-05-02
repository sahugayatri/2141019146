//#include<sys/sysinfo.h>
#include<iostream>
#include<unistd.h>
#include<fstream>
#include<sstream>
#include<thread>
#include<chrono>


using namespace std;

           struct CPUData {
              long user , nice , system , idle , iowait , irq , softirq , steal , guest , guest_nice; 
                    
           };
           
           CPUData  getCPUData(){
                         ifstream file("/proc/stat");  // opens from obj file /proc/stat
                         
                         string line;
                         
                         CPUData cpu = {}; // initialize struct with zero value
                         
                         if(file.is_open()){
                               getline(file , line); // read the ist line
                          
                               istringstream ss(line);  // ss is object
                               string CPULabel;
                               
                               ss >> CPULabel >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle >> cpu.iowait >> cpu.irq >> cpu.softirq >> cpu.steal >> cpu.guest >> cpu.guest_nice;
                          }     
                         return cpu;
          }
          
          double calculateCPUUsage(CPUData prev , CPUData current){
           long previdle = prev.idle + prev.iowait;
           
           long curridle = current.idle + current.iowait;
           
           long prevTotal = prev.user + prev.nice + prev.system + prev.idle + prev.iowait + prev.irq + prev.softirq + prev.steal;
           long currTotal = current.user + current.nice + current.system + current.idle + current.iowait + current.irq + current.softirq + current.steal;
           
           long Totaldiff = currTotal - prevTotal;
           long idleDiff = curridle - previdle;
           
           return (Totaldiff - idleDiff) * 100 / Totaldiff;
          }
int main(){
     CPUData cpu =  getCPUData();
     
      //double cpu_usage = ((total_cputime - cpu.idle) / total_cputime) * 100;
     
     cout << "user time: " << cpu.user << "\n";
     cout << "system time: " << cpu.system << "\n";
     cout << "idle time: " << cpu.idle << "\n";
     cout << "iowait time: " << cpu.iowait << "\n";
     cout << "irq time: " << cpu.irq << "\n";
     cout << "softirq time: " << cpu.softirq << "\n";
     cout << "steal time: " << cpu.steal << "\n";
     cout << "guest time: " << cpu.guest << "\n";
     cout << "guest nice time: " << cpu.guest_nice << "\n";
     
     int total_cputime = cpu.user + cpu.nice + cpu.system + cpu.idle + cpu.iowait + cpu.irq + cpu.softirq + cpu.steal + cpu.guest + cpu.guest_nice;
     cout << "Total cpu time: " << total_cputime << "\n";
     
     int idleTime = cpu.idle + cpu.iowait;
     cout << "idle time" << idleTime << "\n" ;
     
    // cout << "Total cpu time: " << total_cputime << "\n";
   //  cout << "Total cpu usage time: " << cpu_usage << "\n";
   double cpu_usage =(total_cputime - idleTime)*100/ total_cputime;
  cout << "cpu usage : " << cpu_usage << "\n"<< endl;
     
     CPUData prevData =  getCPUData();
     this_thread::sleep_for(chrono::seconds(1)); // wait for 1 seconds
     
     CPUData currData =  getCPUData(); // after 1 second
     double cpuusage = calculateCPUUsage(prevData , currData);
     cout << "cpu usage : " << cpuusage << "\n";
     
     
      return 0;
}
