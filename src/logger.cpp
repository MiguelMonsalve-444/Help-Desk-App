#include "logger.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

void writeLog(const string &user, const string &action, const string &details){
    
    ofstream log("Data/Audit.log", ios :: app);

    if(!log.is_open()){

     return;
    
    }

    //fecha y hora
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    log << "["  << (1900 + ltm->tm_year) << "_"
        << setw(2) << setfill('0') << (ltm->tm_mon + 1) <<"-"
        << setw(2) << setfill('0') << ltm->tm_mday << " "
        << setw(2) << setfill('0') << ltm->tm_hour << ";"
        << setw(2) << setfill('0') << ltm->tm_min  << ";"
        << setw(2) << setfill('0') << ltm->tm_sec  << "]";
        
    log << "User:" << user << "| action:" << action << "| details:" << details << "\n";
    log.close();

}
