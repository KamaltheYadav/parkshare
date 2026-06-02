#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// ─────────────────────────────────────────────
// DATA STRUCTURES
// ─────────────────────────────────────────────

// Holds one owner record
struct Owner {
    string username;
    string password;
    string address;
    int totalSpaces;
    int bookedSpaces;
    int availableSpaces;
    int pricePerHour;
};

// Holds one booking record
struct Booking {
    string ownerUsername;
    string riderName;
    string vehicleNumber;
    int bookedHours;
};

// ─────────────────────────────────────────────
// FILE NAMES
// ─────────────────────────────────────────────

const string OWNERS_FILE   = "owners.txt";
const string BOOKINGS_FILE = "bookings.txt";

// ─────────────────────────────────────────────
// OWNER FILE FUNCTIONS
// ─────────────────────────────────────────────

// Read all owners from owners.txt into a vector
vector<Owner> loadOwners() {
    vector<Owner> owners;
    ifstream file(OWNERS_FILE);
    if (!file.is_open()) return owners; // file may not exist yet

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Each line: username|password|address|totalSpaces|bookedSpaces|availableSpaces|pricePerHour
        stringstream ss(line);
        string token;
        Owner o;

        getline(ss, o.username,       '|');
        getline(ss, o.password,       '|');
        getline(ss, o.address,        '|');
        getline(ss, token, '|'); o.totalSpaces     = stoi(token);
        getline(ss, token, '|'); o.bookedSpaces    = stoi(token);
        getline(ss, token, '|'); o.availableSpaces = stoi(token);
        getline(ss, token, '|'); o.pricePerHour    = stoi(token);

        owners.push_back(o);
    }
    file.close();
    return owners;
}

// Write the entire owners vector back to owners.txt (overwrites)
void saveOwners(const vector<Owner>& owners) {
    ofstream file(OWNERS_FILE);
    for(int i=0;i<owners.size();i++) {
        const Owner& o = owners[i];
        file << o.username      << "|"
             << o.password      << "|"
             << o.address       << "|"
             << o.totalSpaces   << "|"
             << o.bookedSpaces  << "|"
             << o.availableSpaces << "|"
             << o.pricePerHour  << "\n";
    }
    file.close();
}

// Append a single new owner to owners.txt
void appendOwner(const Owner& o) {
    ofstream file(OWNERS_FILE, ios::app);
    file << o.username      << "|"
         << o.password      << "|"
         << o.address       << "|"
         << o.totalSpaces   << "|"
         << o.bookedSpaces  << "|"
         << o.availableSpaces << "|"
         << o.pricePerHour  << "\n";
    file.close();
}

// Check if a username already exists in owners.txt
bool ownerExists(const string& username) {
    vector<Owner> owners = loadOwners();
    for (const Owner& o : owners) {
        if (o.username == username) return true;
    }
    return false;
}

// Find and return an owner by username; returns empty owner if not found
Owner findOwner(const string& username) {
    vector<Owner> owners = loadOwners();
    for (const Owner& o : owners) {
        if (o.username == username) return o;
    }
    Owner empty;
    empty.username = "";
    return empty;
}

// Update one owner's record in owners.txt (match by username)
void updateOwner(const Owner& updated) {
    vector<Owner> owners = loadOwners();
    for (Owner& o : owners) {
        if (o.username == updated.username) {
            o = updated;
            break;
        }
    }
    saveOwners(owners);
}

// ─────────────────────────────────────────────
// BOOKING FILE FUNCTIONS
// ─────────────────────────────────────────────

// Read all bookings from bookings.txt into a vector
vector<Booking> loadBookings() {
    vector<Booking> bookings;
    ifstream file(BOOKINGS_FILE);
    if (!file.is_open()) return bookings;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Each line: ownerUsername|riderName|vehicleNumber|bookedHours
        stringstream ss(line);
        string token;
        Booking b;

        getline(ss, b.ownerUsername,  '|');
        getline(ss, b.riderName,      '|');
        getline(ss, b.vehicleNumber,  '|');
        getline(ss, token,            '|'); b.bookedHours = stoi(token);

        bookings.push_back(b);
    }
    file.close();
    return bookings;
}

// Append a single booking record to bookings.txt
void appendBooking(const Booking& b) {
    ofstream file(BOOKINGS_FILE, ios::app);
    file << b.ownerUsername << "|"
         << b.riderName     << "|"
         << b.vehicleNumber << "|"
         << b.bookedHours   << "\n";
    file.close();
}

// ─────────────────────────────────────────────
// OWNER DASHBOARD FUNCTIONS
// ─────────────────────────────────────────────

// Step 5 - View owner details
void viewDetails(const string& username) {
    Owner o = findOwner(username);
    cout << "\n" << o.username << ", your details are:\n";
    cout << "Total spaces:     " << o.totalSpaces     << "\n";
    cout << "Booked spaces:    " << o.bookedSpaces    << "\n";
    cout << "Available spaces: " << o.availableSpaces << "\n";
    cout << "Address:          " << o.address         << "\n";
    cout << "Price per hour:   " << o.pricePerHour    << "\n";
}

// Step 6 - Modify owner details
void modifyDetails(const string& username) {
    Owner o = findOwner(username);

    cout << "\nAddress: ";
    cin.ignore();
    getline(cin, o.address);

    cout << "Available vehicle spaces: ";
    cin >> o.availableSpaces;

    cout << "Price per hour: ";
    cin >> o.pricePerHour;

    // totalSpaces = bookedSpaces + new availableSpaces
    // bookedSpaces stays unchanged
    o.totalSpaces = o.bookedSpaces + o.availableSpaces;

    updateOwner(o);
    cout << "\nDetails updated.\n";
}

// Step 7 - Show bookings for this owner
void showBookedDetails(const string& username) {
    vector<Booking> bookings = loadBookings();

    bool found = false;
    for (const Booking& b : bookings) {
        if (b.ownerUsername == username) {
            if (!found) cout << "\nBookings for " << username << ":\n";
            found = true;
            cout << "  Rider Name:     " << b.riderName     << "\n";
            cout << "  Vehicle Number: " << b.vehicleNumber << "\n";
            cout << "  Booked Hours:   " << b.bookedHours   << "\n";
            cout << "  ---------------------------\n";
        }
    }

    if (!found) {
        cout << "\nNo bookings found.\n";
    }
}

// Step 4 - Owner dashboard loop
void ownerDashboard(const string& username) {
    int choice;
    while (true) {
        cout << "\n1. View Details\n";
        cout << "2. Modify Details\n";
        cout << "3. Show Booked Details\n";
        cout << "4. Quit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            viewDetails(username);
        } else if (choice == 2) {
            modifyDetails(username);
        } else if (choice == 3) {
            showBookedDetails(username);
        } else if (choice == 4) {
            cout << "\nGoodbye!\n";
            exit(0);
        } else {
            cout << "\nInvalid choice.\n";
        }
    }
}

// ─────────────────────────────────────────────
// OWNER FLOW
// ─────────────────────────────────────────────

// Step 3 - Register a new owner
void registerOwner() {
    Owner o;
    o.bookedSpaces = 0;

    cout << "\nUsername: ";
    cin >> o.username;

    // Make sure username is unique
    if (ownerExists(o.username)) {
        cout << "Username already taken. Please try again.\n";
        return;
    }

    cout << "Password: ";
    cin >> o.password;

    cout << "Address: ";
    cin.ignore();
    getline(cin, o.address);

    cout << "Available vehicle spaces: ";
    cin >> o.availableSpaces;

    cout << "Price per hour: ";
    cin >> o.pricePerHour;

    // For a new owner, totalSpaces = availableSpaces, bookedSpaces = 0
    o.totalSpaces = o.availableSpaces;

    appendOwner(o);
    cout << "\nRegistration successful.\n";
    ownerDashboard(o.username);
}

// Step 8 - Existing owner login
void loginOwner() {
    string username, password;
    cout << "\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    Owner o = findOwner(username);
    if (o.username.empty() || o.password != password) {
        cout << "\nInvalid username or password.\n";
        return;
    }

    ownerDashboard(username);
}

// Step 2 - Space owner entry point
void spaceOwnerFlow() {
    int choice;
    cout << "\n1. New User\n";
    cout << "2. Existing User\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        registerOwner();
    } else if (choice == 2) {
        loginOwner();
    } else {
        cout << "\nInvalid choice.\n";
    }
}

// ─────────────────────────────────────────────
// RIDER FLOW
// ─────────────────────────────────────────────

// Steps 9, 10, 11 - Full rider flow
void riderFlow() {
    string riderName, riderAddress, vehicleNumber;

    // Step 9 - Collect rider info
    cout << "\nName: ";
    cin.ignore();
    getline(cin, riderName);

    cout << "Address: ";
    getline(cin, riderAddress);

    cout << "Vehicle Number: ";
    cin >> vehicleNumber;

    // Step 10 - Find matching available parking spaces
    vector<Owner> owners = loadOwners();
    vector<int> matchIndexes; // indexes of matching owners in the vector

    cout << "\n";
    int displayNum = 1;
    for (int i = 0; i < (int)owners.size(); i++) {
        if (owners[i].address == riderAddress && owners[i].availableSpaces > 0) {
            cout << displayNum << ". Username: "       << owners[i].username       << "\n";
            cout << "   Address: "          << owners[i].address         << "\n";
            cout << "   Available Spaces: " << owners[i].availableSpaces << "\n";
            cout << "   Price Per Hour: "   << owners[i].pricePerHour    << "\n\n";
            matchIndexes.push_back(i);
            displayNum++;
        }
    }

    if (matchIndexes.empty()) {
        cout << "No parking spaces found in your area.\n";
        return;
    }

    // Step 11 - Book a parking space
    int spaceChoice;
    cout << "Select parking space number: ";
    cin >> spaceChoice;

    // Validate selection
    if (spaceChoice < 1 || spaceChoice > (int)matchIndexes.size()) {
        cout << "\nInvalid selection.\n";
        return;
    }

    int ownerIndex = matchIndexes[spaceChoice - 1]; // real index in owners vector

    int bookedHours;
    cout << "Booked Hours: ";
    cin >> bookedHours;

    // Update owner's spaces
    owners[ownerIndex].bookedSpaces    += 1;
    owners[ownerIndex].availableSpaces -= 1;

    // Save updated owners back to file
    saveOwners(owners);

    // Save booking to bookings.txt
    Booking b;
    b.ownerUsername = owners[ownerIndex].username;
    b.riderName     = riderName;
    b.vehicleNumber = vehicleNumber;
    b.bookedHours   = bookedHours;
    appendBooking(b);

    cout << "\nBooking successful.\n";
}

// ─────────────────────────────────────────────
// MAIN - Entry point
// ─────────────────────────────────────────────

int main() {
    int choice;

    // Step 1 - Show main menu
    cout << "\nWho are you?\n\n";
    cout << "1. Space Owner\n";
    cout << "2. Rider\n";
    cout << "\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        spaceOwnerFlow();
    } else if (choice == 2) {
        riderFlow();
    } else {
        cout << "\nInvalid choice.\n";
    }

    return 0;
}
