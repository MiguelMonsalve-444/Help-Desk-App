#include "tickets.h"
#include "logger.h"
#include "security.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

// Cargar tickets
vector<Ticket> loadTickets(const string &path) {
    vector<Ticket> tickets;
    ifstream file(path);
    if(!file.is_open()) return tickets;

    string line;
    while(getline(file, line)){
        if(line.empty() || line.find_first_not_of(" \t\n\r") == string::npos)
            continue;

        stringstream ss(line);
        Ticket t;
        string idStr;

        getline(ss, idStr, ',');
        try { t.id = stoi(idStr); }
        catch(...) { continue; }

        getline(ss, t.title, ',');
        getline(ss, t.description, ',');
        getline(ss, t.status, ',');
        getline(ss, t.priority, ',');
        getline(ss, t.createdBy, ',');
        getline(ss, t.assignee, ',');
        getline(ss, t.dateCreated, ',');

        tickets.push_back(t);
    }

    file.close();
    return tickets;
}

// Guardar tickets
bool saveTicket(const vector<Ticket> &tickets, const string &path) {
    ofstream file(path, ios::trunc);
    if(!file.is_open()) return false;

    for(const auto &t : tickets) {
        file << t.id << "," 
             << preventCSVInjection(t.title) << ","
             << preventCSVInjection(t.description) << ","
             << preventCSVInjection(t.status) << ","
             << preventCSVInjection(t.priority) << ","
             << preventCSVInjection(t.createdBy) << ","
             << preventCSVInjection(t.assignee) << ","
             << preventCSVInjection(t.dateCreated) << "\n";
    }

    file.close();
    return true;
}

//  Crear tickts
void createTicket(vector<Ticket> &tickets, const User &u) {
    Ticket t;
    if (tickets.empty()){
        t.id = 1;
    } else {
        t.id = tickets.back().id + 1;
    }
    
    cin.ignore();
    cout << "Titulo: ";
    getline(cin, t.title);
    if(!validateText(t.title, 30)) {
        cout << "Titulo invalido.\n";
        return;
    }

    cout << "Descripcion: ";
    getline(cin, t.description);
    if(!validateText(t.description, 100)) {
        cout << "Descripcion invalida.\n";
        return;
    }

    t.status = "open";
    t.priority = "medium";
    t.createdBy = u.username;
    t.assignee = "";
    
    // Fecha  
    time_t now = time(nullptr);
    t.dateCreated = ctime(&now);
    t.dateCreated.erase(remove(t.dateCreated.begin(), t.dateCreated.end(), '\n'), t.dateCreated.end());

    tickets.push_back(t);
    saveTicket(tickets, "data/tickets.txt");
    writeLog(u.username, "create_ticket", "Ticket ID = " + to_string(t.id));

    cout << "Ticket creado correctamente.\n";
}

// Mostrar tickets
void viewTickets(const vector<Ticket> &tickets, const User &u) {
    cout << "\n--- Lista de Tickets ---\n";
    for(const auto &t : tickets){
        if(u.role == "agent" || t.createdBy == u.username){
            cout << "ID: " << t.id 
                 << " | Titulo: " << t.title
                 << " | Estado: " << t.status
                 << " | Prioridad: " << t.priority
                 << " | Creador: " << t.createdBy
                 << " | Asignado a: " << t.assignee
                 << " | Creado: " << t.dateCreated
                 << "\n----------------------\n";
        }
    }
}

//  Actualizar ticket
void updateTicket(vector<Ticket> &tickets, const User &u) {
    int id;
    cout << "ID del ticket a actualizar: ";
    cin >> id;
    cin.ignore();

    for(auto &t : tickets) {
        if(t.id == id){
            if(u.role == "user" && t.createdBy != u.username) {
                cout << "No puede modificar este ticket.\n";
                return;
            }

            cout << "Nueva descripcion: ";
            string desc;
            getline(cin, desc);
            if(!validateText(desc, 100)) {
                cout << "Descripcion invalida.\n";
                return;
            }
            t.description = desc;

            cout << "Nuevo estado (open/closed): ";
            string status;
            getline(cin, status);
            if(status != "open" && status != "closed") {
                cout << "Estado invalido.\n";
                return;
            }
            t.status = status;

            saveTicket(tickets, "data/tickets.txt");
            writeLog(u.username, "update_ticket", "Ticket ID = " + to_string(t.id));
            cout << "Ticket actualizado.\n";
            return;
        }
    }
    cout << "Ticket no encontrado.\n";
}

//  Eliminar ticket
void deleteTicket(vector<Ticket> &tickets, const User &u) {
    int id;
    cout << "ID del ticket a eliminar: ";
    cin >> id;
    cin.ignore();

    for(auto it = tickets.begin(); it != tickets.end(); ++it){
        if(it->id == id){
            if(u.role == "user" && it->createdBy != u.username){
                cout << "No puede eliminar este ticket.\n";
                return;
            }
            if(u.role == "agent" && it->status != "closed") {
                cout << "Solo tickets cerrados pueden ser eliminados por agentes.\n";
                return;
            }

            writeLog(u.username, "delete_ticket", "Ticket ID = " + to_string(id));
            tickets.erase(it);
            saveTicket(tickets, "data/tickets.txt");
            cout << "Ticket eliminado.\n";
            return;
        }
    }
    cout << "Ticket no encontrado.\n";
}

// Reasignar ticket
void reassignTicket(vector<Ticket> &tickets, const User &u) {
    if(u.role != "agent"){
        cout << "Solo agentes pueden reasignar tickets.\n";
        return;
    }

    int id;
    cout << "ID del ticket a reasignar: ";
    cin >> id;
    cin.ignore();

    for(auto &t : tickets){
        if(t.id == id){
            cout << "Nuevo usuario asignado: ";
            string newAssignee;
            getline(cin, newAssignee);

            if(!validateText(newAssignee, 20)){
                cout << "Nombre invalido.\n";
                return;
            }

            t.assignee = newAssignee;
            saveTicket(tickets, "data/tickets.txt");
            writeLog(u.username, "reassign_ticket", "Ticket ID = " + to_string(t.id) + " -> " + newAssignee);
            cout << "Ticket reasignado.\n";
            return;
        }
    }
    cout << "Ticket no encontrado.\n";
}
