#ifndef SECURITY_H
#define SECURITY_H

#include <string>
using namespace std;

bool validateText(const string &s,int maxLen);

string preventCSVInjection(string s);

#endif