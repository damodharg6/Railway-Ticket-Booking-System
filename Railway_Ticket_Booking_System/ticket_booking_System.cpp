#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

long long generatePNR() {
    static long long basePNR = 4521378901;   // DevC++ safe
    return basePNR++;
}

int calculateFare(const string& coach) {
    if (coach == "AC") return 500;
    if (coach == "Sleeper") return 200;
    return 100;
}

struct Ticket {
    long long pnr;
    string name;
    int age;
    string source, destination;
    string date;
    string trainNumber;
    string coach;
    int platform;
    int fare;

    void input() {

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter Age: ";
        while (!(cin >> age)) {
            cout << "Invalid input. Enter a valid age: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // SOURCE
        cout << "Enter Source Location: ";
        cin.ignore();
        getline(cin, source);

        // DESTINATION
        cout << "Enter Destination Location: ";
        getline(cin, destination);

        cout << "Enter Date (DD/MM/YYYY): ";
        cin >> date;

        cout << "Enter Train Number: ";
        cin >> trainNumber;

        do {
            cout << "Coach Type (AC/Sleeper/General): ";
            cin >> coach;
            if (coach != "AC" && coach != "Sleeper" && coach != "General")
                cout << "Invalid coach type. Try again.\n";
        } while (coach != "AC" && coach != "Sleeper" && coach != "General");

        platform = rand() % 10 + 1;
        pnr = generatePNR();
        fare = calculateFare(coach);
    }

    void display() const {
        cout << left << setw(12) << pnr
             << setw(15) << name
             << setw(5) << age
             << setw(15) << source
             << setw(15) << destination
             << setw(12) << date
             << setw(10) << trainNumber
             << setw(10) << coach
             << setw(10) << platform
             << "Rs. " << fare << endl;
    }
};

class TicketSystem {
private:
    vector<Ticket> tickets;
    map<string, int> seatAvailability;

public:
    TicketSystem() {
        srand(time(0));
        seatAvailability["AC"] = 5;
        seatAvailability["Sleeper"] = 5;
        seatAvailability["General"] = 5;
    }

    void bookTicket() {
        Ticket t;
        cout << "\n--- Book a Ticket ---\n";
        t.input();

        if (seatAvailability[t.coach] <= 0) {
            cout << "No seats available in " << t.coach << " coach.\n";
            return;
        }

        tickets.push_back(t);
        seatAvailability[t.coach]--;

        ofstream fout("tickets.txt", ios::app);
        if (fout) {
            fout << t.pnr << " "
                 << t.name << " "
                 << t.age << " "
                 << t.source << " "
                 << t.destination << " "
                 << t.date << " "
                 << t.trainNumber << " "
                 << t.coach << " "
                 << t.platform << " "
                 << t.fare << endl;
            fout.close();
        }

        cout << "\nTicket Booked Successfully!\n";
        cout << "PNR: " << t.pnr 
             << " | Platform: " << t.platform 
             << " | Fare: Rs. " << t.fare << "\n";
    }

    void cancelTicket() {
        long long pnr;
        cout << "Enter PNR to cancel: ";
        cin >> pnr;

        for (vector<Ticket>::iterator it = tickets.begin(); it != tickets.end(); ++it) {
            if (it->pnr == pnr) {
                seatAvailability[it->coach]++;
                cout << "\nTicket Cancelled Successfully:\n";
                it->display();
                tickets.erase(it);
                return;
            }
        }
        cout << "PNR not found.\n";
    }

    void displayTickets() {
        if (tickets.empty()) {
            cout << "No tickets booked yet.\n";
            return;
        }

        cout << "\n--- Booked Tickets ---\n";
        cout << left << setw(12) << "PNR"
             << setw(15) << "Name"
             << setw(5) << "Age"
             << setw(15) << "Source"
             << setw(15) << "Destination"
             << setw(12) << "Date"
             << setw(10) << "Train No"
             << setw(10) << "Coach"
             << setw(10) << "Platform"
             << "Fare\n";

        for (size_t i = 0; i < tickets.size(); i++) {
            tickets[i].display();
        }
    }

    void searchByPNR() {
        long long pnr;
        cout << "Enter PNR to search: ";
        cin >> pnr;

        for (size_t i = 0; i < tickets.size(); i++) {
            if (tickets[i].pnr == pnr) {
                cout << "\nTicket Found:\n";
                tickets[i].display();
                return;
            }
        }
        cout << "Ticket not found.\n";
    }

    void showAvailability() {
        cout << "\n--- Seat Availability ---\n";
        cout << "AC: " << seatAvailability["AC"] << " seats left\n";
        cout << "Sleeper: " << seatAvailability["Sleeper"] << " seats left\n";
        cout << "General: " << seatAvailability["General"] << " seats left\n";
    }

    void adminPanel() {
        string pass;
        cout << "Enter Admin Password: ";
        cin >> pass;

        if (pass != "admin123") {
            cout << "Wrong password.\n";
            return;
        }

        cout << "\n--- Admin Panel ---\n";
        displayTickets();
    }
};

int main() {
    TicketSystem system;
    int choice = 0;

    do {
        cout << "\n========== TICKET BOOKING MENU ==========\n";
        cout << "1. Book Ticket\n";
        cout << "2. Cancel Ticket\n";
        cout << "3. Display Booked Tickets\n";
        cout << "4. Search Ticket by PNR\n";
        cout << "5. Show Seat Availability\n";
        cout << "6. Admin Panel\n";
        cout << "7. Exit\n";
        cout << "Choose an option: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter numbers only.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: system.bookTicket(); break;
            case 2: system.cancelTicket(); break;
            case 3: system.displayTickets(); break;
            case 4: system.searchByPNR(); break;
            case 5: system.showAvailability(); break;
            case 6: system.adminPanel(); break;
            case 7: cout << "Thank you for using the system!\n"; break;
            default: cout << "Invalid option! Try again.\n";
        }

    } while (choice != 7);

    return 0;
}

