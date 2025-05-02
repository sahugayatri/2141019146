#include<iostream>
#include<filesystem>
#include<fstream>
#include<sstream>
#include<algorithm>
using namespace std;

namespace fs = std::filesystem;

//function to check if the directory name is a number (eg pID)
bool isNumber(const std::string &s){
             return !s.empty() & all_of(s.begin() , s.end() , ::isdigit);  // checks if all the characters in string s is digit or not
             
}

//function to read process name from /proc/[PID]/stat
string getProcessName(int pid){
             std::ifstream file("/proc/" + std::to_string(pid) + "/stat"); 
             std::string line , processName;
             if(file.is_open()){
                               std::getline(file , line); // read the ist line into a string
                          
                               std::istringstream ss(line);  // ss is object
                               
                               std::string token;
                               int count = 0;
                               while(ss >> token){
                                        count++;
                                        if(count == 2){ //process name is in second field
                                                 processName = token;
                                                 break;
                                        }
                               }
                }          
                return processName;          
}

int main(){
       std::cout << "active processes: \n";
       //extract process list
       //we check the directory list
       //check if it is number
       
       for(const auto &entry : fs::directory_iterator("/proc")){ //check if the directory is a pid
             if(entry.is_directory()){ //filters only numeric directory valid pid
                    std::string filename = entry.path().filename().string();
                    if(isNumber(filename)){
                            int pid = stoi(filename); // convert directory name to integer
                            std::string processName = getProcessName(pid);
                            std::cout << "PID: " << pid << " | Name" << processName << "\n";
                    
                    }
             }
       
       }
       return 0;
}
