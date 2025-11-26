#ifndef TICKETS_H
#define TICKETS_H

#include <string>
#include <vector>
#include "auth.h"

using namespace std;

struct Ticket {
    int id;
    string title;
    string description;
    string status;       // "open" / "closed"
    string priority;     // "low" / "medium" / "high"
    string createdBy;
    string assignee;
    string dateCreated;
};

vector<Ticket> loadTickets(const string &path);
bool saveTicket(const vector<Ticket> &tickets, const string &path);

void createTicket(vector<Ticket> &tickets, const User &u);
void viewTickets(const vector<Ticket> &tickets, const User &u);
void updateTicket(vector<Ticket> &tickets, const User &u);
void deleteTicket(vector<Ticket> &tickets, const User &u);
void reassignTicket(vector<Ticket> &tickets, const User &u);

#endif
