#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

struct Train {
    int trainNumber;
    string name;
    string route;
    string departureTime;
    string arrivalTime;
    int totalSeats;
    int availableSeats;
};

struct Ticket {
    int ticketNumber;
    int trainNumber;
    string passengerName;
    int seatNumber;
};

vector<Train> trains;
vector<Ticket> tickets;
int ticketIdCounter = 1;

void saveTrainsToFile();
void loadTrainsFromFile();
void saveTicketsToFile();
void loadTicketsFromFile();
void addTrain();
void manageSchedule();
void bookTicket();
void checkAvailability();

int main() {
    loadTrainsFromFile();
    loadTicketsFromFile();

    int choice;
    while (true) {
        cout << "\n--- Railway Management System ---\n";
        cout << "1. Add Train\n";
        cout << "2. Manage Schedule\n";
        cout << "3. Book Ticket\n";
        cout << "4. Check Seat Availability\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addTrain();
            break;
        case 2:
            manageSchedule();
            break;
        case 3:
            bookTicket();
            break;
        case 4:
            checkAvailability();
            break;
        case 5:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

void saveTrainsToFile() {
    ofstream file("trains.txt");
    for (const auto &t : trains) {
        file << t.trainNumber << ',' << t.name << ',' << t.route << ','
             << t.departureTime << ',' << t.arrivalTime << ','
             << t.totalSeats << ',' << t.availableSeats << '\n';
    }
    file.close();
}

void loadTrainsFromFile() {
    ifstream file("trains.txt");
    if (!file) return;
    Train t;
    while (file >> t.trainNumber) {
        file.ignore();
        getline(file, t.name, ',');
        getline(file, t.route, ',');
        getline(file, t.departureTime, ',');
        getline(file, t.arrivalTime, ',');
        file >> t.totalSeats;
        file.ignore();
        file >> t.availableSeats;
        file.ignore();
        trains.push_back(t);
    }
    file.close();
}

void saveTicketsToFile() {
    ofstream file("tickets.txt");
    for (const auto &t : tickets) {
        file << t.ticketNumber << ',' << t.trainNumber << ','
             << t.passengerName << ',' << t.seatNumber << '\n';
    }
    file.close();
}

void loadTicketsFromFile() {
    ifstream file("tickets.txt");
    if (!file) return;
    Ticket t;
    while (file >> t.ticketNumber) {
        file.ignore();
        file >> t.trainNumber;
        file.ignore();
        getline(file, t.passengerName, ',');
        file >> t.seatNumber;
        file.ignore();
        tickets.push_back(t);
        ticketIdCounter = max(ticketIdCounter, t.ticketNumber + 1);
    }
    file.close();
}

void addTrain() {
    Train t;
    cout << "Enter train number: ";
    cin >> t.trainNumber;
    cin.ignore();
    cout << "Enter train name: ";
    getline(cin, t.name);
    cout << "Enter route: ";
    getline(cin, t.route);
    cout << "Enter departure time: ";
    getline(cin, t.departureTime);
    cout << "Enter arrival time: ";
    getline(cin, t.arrivalTime);
    cout << "Enter total seats: ";
    cin >> t.totalSeats;
    t.availableSeats = t.totalSeats;
    trains.push_back(t);
    saveTrainsToFile();
    cout << "Train added successfully.\n";
}

void manageSchedule() {
    cout << "\n--- Train Schedule ---\n";
    for (const auto &t : trains) {
        cout << "Train Number: " << t.trainNumber << "\n";
        cout << "Name: " << t.name << "\n";
        cout << "Route: " << t.route << "\n";
        cout << "Departure: " << t.departureTime << "\n";
        cout << "Arrival: " << t.arrivalTime << "\n";
        cout << "Available Seats: " << t.availableSeats << "/" << t.totalSeats << "\n\n";
    }
}

void bookTicket() {
    int trainNumber;
    cout << "Enter train number: ";
    cin >> trainNumber;
    cin.ignore();
    auto it = find_if(trains.begin(), trains.end(), [trainNumber](const Train &t) {
        return t.trainNumber == trainNumber;
    });
    if (it == trains.end() || it->availableSeats <= 0) {
        cout << "Train not found or no seats available.\n";
        return;
    }
    Ticket ticket;
    ticket.ticketNumber = ticketIdCounter++;
    ticket.trainNumber = trainNumber;
    cout << "Enter passenger name: ";
    getline(cin, ticket.passengerName);
    ticket.seatNumber = it->totalSeats - it->availableSeats + 1;
    tickets.push_back(ticket);
    it->availableSeats--;
    saveTicketsToFile();
    saveTrainsToFile();
    cout << "Ticket booked successfully. Ticket Number: " << ticket.ticketNumber << " Seat Number: " << ticket.seatNumber << "\n";
}

void checkAvailability() {
    int trainNumber;
    cout << "Enter train number: ";
    cin >> trainNumber;
    auto it = find_if(trains.begin(), trains.end(), [trainNumber](const Train &t) {
        return t.trainNumber == trainNumber;
    });
    if (it == trains.end()) {
        cout << "Train not found.\n";
        return;
    }
    cout << "Available Seats for Train " << trainNumber << ": " << it->availableSeats << "/" << it->totalSeats << "\n";
}
