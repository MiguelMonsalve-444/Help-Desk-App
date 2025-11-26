#ifndef SECURITY_H
#define SECURITY_H

#include <string>
using namespace std;

bool validateText(const string &s,int maxLen);
void vulnerableDemo();
void safeDemo();
string preventCSVInjection(string s);

#endif