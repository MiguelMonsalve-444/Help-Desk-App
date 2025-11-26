#include "security.h"
#include <iostream>
#include <cstring>
using namespace std;

bool validateText(const string &s,const int maxLen){

    if(s.empty() || (int)s.size() > maxLen){ 
        return false;
    }
    
    if(s.find(";") != string::npos || s.find("--") != string::npos)
    return false;
    return true;
}

string preventCSVInjection(string s) {
    if (!s.empty() && (s[0] == '=' || s[0] == '+' || s[0] == '-' || s[0] == '@')) {
        s.insert(0, "'");
    }
    return s;
}