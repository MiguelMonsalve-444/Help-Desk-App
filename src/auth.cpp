#include "auth.h"
#include "logger.h"
#include "security.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <functional>
#include <cctype>
#include <openssl/sha.h>

using namespace std;


// Validaciones necesarias
bool isSafeText(const string &txt) {
    string bad = "',;\"\\--";
    for (char c : txt) {
        if (c == ',' || c == ';' || c == '"' || c == '\'' || c == '\\')
        
        return false;
    }
    return true;
}

bool validateUsername(const string &u) {
    if (u.size() < 3 || u.size() > 20) return false;
    if (!isSafeText(u)) return false;

    // Solo letras y números
    for (char c : u) {
        if (!isalnum(c)) return false;
    }

    return true;
}

bool validatePassword(const string &p) {
    if (p.size() < 6 || p.size() > 30){ 
        return false;
    }
    if (!isSafeText(p)){
     return false;
    }
    return true;
}


// cifrado SHA-256 
//Alex. (2016). setw and setfill as it's used in a SHA256 hash function [Respuesta en foro]. Stack Overflow.
//https://stackoverflow.com/questions/41205960/setw-and-setfill-as-its-used-in-a-sha256-hash-function

string hashPassword(const string &password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.size(), hash);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];

    return ss.str();
}


// Cargar usuarios
vector<User> loadUsers(const string &path) {
    vector<User> users;
    ifstream file(path);
    if (!file.is_open()) return users;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string username, hash, role;
        getline(ss, username, ',');
        getline(ss, hash, ',');
        getline(ss, role, ',');

        if (!username.empty() && !hash.empty() && !role.empty()) {
            users.push_back({username, hash, role});
        }
    }
    file.close();
    return users;
}

// Guardar usuarios
bool saveUsers(const vector<User> &users, const string &path) {
    ofstream file(path, ios::trunc);
    if (!file.is_open()) return false;

    for (auto &u : users) {
        file << preventCSVInjection(u.username) << "," << u.passwordHash << "," << preventCSVInjection(u.role) << "\n";
    }

    file.close();
    return true;
}


// Registrar usuario
bool registerUser(vector<User> &users, const string &path) {
    string username, password, role;

    cout << "Nuevo usuario\nUsuario: ";
    getline(cin, username);

    if (!validateUsername(username)) {
        cout << "Nombre no valido.\n";
        return false;
    }

    for (auto &u : users)
        if (u.username == username) {
            cout << "El usuario ya existe.\n";
            return false;
        }

    cout << "Contrasena (min 6 caracteres): ";
    getline(cin, password);

    if (!validatePassword(password)) {
        cout << "Contrasena invalida.\n";
        return false;
    }

    cout << "Rol (user/agent): ";
    getline(cin, role);

    if (role != "user" && role != "agent") {
        cout << "Rol invalido.\n";
        return false;
    }

    User newUser{username, hashPassword(password), role};
    users.push_back(newUser);

    saveUsers(users, path);
    writeLog(username, "register", "Nuevo registro con rol " + role);

    cout << "Usuario registrado correctamente.\n";
    return true;
}


// Login de usuario
bool loginUser(const vector<User> &users, User &outUser) {
    string username, password;

    cout << "Usuario: ";
    getline(cin, username);

    if (!validateUsername(username)) {
        cout << "Usuario invalido.\n";
        writeLog(username, "login_fail", "Intento con entrada invalida");
        return false;
    }

    cout << "Contrasena: ";
    getline(cin, password);

    if (!validatePassword(password)) {
        cout << "Contrasena invalida.\n";
        writeLog(username, "login_fail", "Contrasena con formato invalido");
        return false;
    }

    string hashed = hashPassword(password);

    for (auto &u : users) {
        if (u.username == username && u.passwordHash == hashed) {
            outUser = u;
            writeLog(username, "login", "Inicio de sesion exitoso");
            return true;
        }
    }

    writeLog(username, "login_fail", "Credenciales incorrectas");
    cout << "Credenciales incorrectas.\n";
    return false;
}
