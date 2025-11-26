#include "auth.h"
#include "logger.h"
#include "security.h"
#include "tickets.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//Miguel Angel Monsalve Valemcia

int main() {

    // Cargar usuarios y tickets
    vector<User> users = loadUsers("data/users.txt");
    vector<Ticket> tickets = loadTickets("data/tickets.txt");

    User currentUser;
    int option = 0;

    // Inicio de sesion y registro
    while (true) {
        cout << R"(
=====================================================
            Sistema de Tickets de Soporte
=====================================================

            1. Iniciar sesion
            2. Registrarse
            3. Salir

=====================================================
Seleccione una opcion: )";

        cin >> option;
        cin.ignore();

        switch (option) {
        case 1: {
            if (loginUser(users, currentUser)) {
                cout << "\nInicio de sesion exitoso.\n";
                writeLog(currentUser.username, "login", "Inicio de sesion exitoso");
            } else {
                cout << "\nCredenciales invalidas.\n";
                writeLog("unknown", "login_fail", "Intento fallido de inicio de sesion");
                continue;
            }
            break;
        }

        case 2:
            if (registerUser(users, "data/users.txt")) {
                cout << "Registro completado correctamente.\n";
            }
            continue;

        case 3:
            cout << "\nGracias por usar el sistema. Hasta luego.\n";
            return 0;

        default:
            cout << "\nOpcion invalida.\n";
            continue;
        }

        break;
        
       system("cls");
    }

 
    // Menu
    do {
        cout << R"(
=====================================================
                    MENU PRINCIPAL
=====================================================

1. Crear ticket
2. Ver tickets
3. Actualizar ticket
4. Eliminar ticket
5. Reasignar ticket (Agentes)
6. Cerrar sesion

=====================================================
Seleccione una opcion: )";

        cin >> option;
        cin.ignore();

        switch (option) {
        case 1:
            createTicket(tickets, currentUser);
            break;

        case 2:
            viewTickets(tickets, currentUser);
            break;

        case 3:
            updateTicket(tickets, currentUser);
            break;

        case 4:
            deleteTicket(tickets, currentUser);
            break;

        case 5:
            reassignTicket(tickets, currentUser);
            break;

        case 6:
            writeLog(currentUser.username, "logout", "Cierre de sesion");
            saveTicket(tickets, "data/tickets.txt");
            cout << "\nSesion cerrada.\n";
            return 0;

        default:
            cout << "Opcion invalida.\n";
            break;
        }

    } while (true);

    return 0;
}
