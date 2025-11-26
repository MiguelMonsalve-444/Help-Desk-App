#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <vector>

using namespace std;

struct User{
    string username;
    string passwordHash;
    string role;
};

vector<User> loadUsers(const string &path);

bool saveUsers(const vector<User> &users, const string &path);

bool registerUser(vector<User> &users, const string &path);

bool loginUser(const vector<User> &users, User &outUser);

string hashPassword(const string &password);

#endif