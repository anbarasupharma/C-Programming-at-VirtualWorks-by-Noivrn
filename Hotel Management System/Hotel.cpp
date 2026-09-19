#include "Hotel.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <limits>

// =====================================================================
//  Constructor
// =====================================================================

Hotel::Hotel() : nextCustomerId(1), nextBillId(1) {}

// =====================================================================
//  Helper — initialise 30 default rooms
// =====================================================================

void Hotel::initializeDefaultRooms() {
    rooms.clear();
    // Rooms 101-110  : Single ($50)
    for (int i = 1; i <= 10; ++i)
        rooms.emplace_back(100 + i, "Single", 50.0);
    // Rooms 201-210  : Double ($80)
    for (int i = 1; i <= 10; ++i)
        rooms.emplace_back(200 + i, "Double", 80.0);
    // Rooms 301-310  : Suite  ($150)
    for (int i = 1; i <= 10; ++i)
        rooms.emplace_back(300 + i, "Suite", 150.0);
}

// =====================================================================
//  Helper — look-ups
// =====================================================================

int Hotel::findRoomIndex(int roomNumber) const {
    for (size_t i = 0; i < rooms.size(); ++i)
        if (rooms[i].getRoomNumber() == roomNumber)
            return static_cast<int>(i);
    return -1;
}

int Hotel::findAvailableRoom(const std::string& type) const {
    for (size_t i = 0; i < rooms.size(); ++i)
        if (rooms[i].getType() == type && !rooms[i].isOccupied())
            return static_cast<int>(i);
    return -1;
}

// =====================================================================
//  Helper — date utilities
// =====================================================================

std::string Hotel::getCurrentDate() const {
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    char buf[11];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
    return std::string(buf);
}

int Hotel::calculateNights(const std::string& checkIn,
                           const std::string& checkOut) const {
    // Parse YYYY-MM-DD
    auto parse = [](const std::string& d) -> std::time_t {
        std::tm tm = {};
        std::istringstream ss(d);
        char dash;
        ss >> tm.tm_year >> dash >> tm.tm_mon >> dash >> tm.tm_mday;
        tm.tm_year -= 1900;
        tm.tm_mon  -= 1;
        return std::mktime(&tm);
    };

    std::time_t t1 = parse(checkIn);
    std::time_t t2 = parse(checkOut);
    double diff = std::difftime(t2, t1) / (60.0 * 60.0 * 24.0);
    int nights = static_cast<int>(diff);
    return (nights < 1) ? 1 : nights;   // minimum 1 night
}

// =====================================================================
//  Data Persistence
// =====================================================================

void Hotel::loadData() {
    // ---------- Rooms ----------
    std::ifstream roomFile(ROOMS_FILE);
    if (roomFile.is_open()) {
        rooms.clear();
        while (roomFile.peek() != EOF) {
            Room r;
            r.loadFromFile(roomFile);
            if (r.getRoomNumber() != 0)
                rooms.push_back(r);
        }
        roomFile.close();
    }
    if (rooms.empty()) {
        initializeDefaultRooms();
    }

    // ---------- Customers ----------
    std::ifstream custFile(CUSTOMERS_FILE);
    if (custFile.is_open()) {
        customers.clear();
        while (custFile.peek() != EOF) {
            Customer c;
            c.loadFromFile(custFile);
            if (c.getId() != 0) {
                customers.push_back(c);
                if (c.getId() >= nextCustomerId)
                    nextCustomerId = c.getId() + 1;
            }
        }
        custFile.close();
    }

    // ---------- Billing ----------
    std::ifstream billFile(BILLING_FILE);
    if (billFile.is_open()) {
        bills.clear();
        while (billFile.peek() != EOF) {
            Billing b;
            b.loadFromFile(billFile);
            if (b.getBillId() != 0) {
                bills.push_back(b);
                if (b.getBillId() >= nextBillId)
                    nextBillId = b.getBillId() + 1;
            }
        }
        billFile.close();
    }
}

void Hotel::saveData() const {
    // ---------- Rooms ----------
    std::ofstream roomFile(ROOMS_FILE, std::ios::trunc);
    if (roomFile.is_open()) {
        for (const auto& r : rooms)
            r.saveToFile(roomFile);
        roomFile.close();
    }

    // ---------- Customers ----------
    std::ofstream custFile(CUSTOMERS_FILE, std::ios::trunc);
    if (custFile.is_open()) {
        for (const auto& c : customers)
            c.saveToFile(custFile);
        custFile.close();
    }

    // ---------- Billing ----------
    std::ofstream billFile(BILLING_FILE, std::ios::trunc);
    if (billFile.is_open()) {
        for (const auto& b : bills)
            b.saveToFile(billFile);
        billFile.close();
    }
}

// =====================================================================
//  1. Display Available Rooms
// =====================================================================

void Hotel::displayAvailableRooms() const {
    std::cout << "\n+========================================+\n";
    std::cout << "|          AVAILABLE ROOMS                |\n";
    std::cout << "+========================================+\n\n";

    std::string types[] = {"Single", "Double", "Suite"};

    for (const auto& type : types) {
        std::cout << "  --- " << type << " Rooms ---\n";
        std::cout << "  " << std::left << std::setw(8)  << "Room#"
                  << std::setw(10) << "Type"
                  << std::setw(10) << "Price"
                  << std::setw(12) << "Status" << "\n";
        std::cout << "  " << std::string(40, '-') << "\n";

        bool found = false;
        for (const auto& room : rooms) {
            if (room.getType() == type && !room.isOccupied()) {
                room.displayBrief();
                found = true;
            }
        }
        if (!found)
            std::cout << "  (No available rooms of this type)\n";
        std::cout << "\n";
    }
}

// =====================================================================
//  2. Search Room
// =====================================================================

void Hotel::searchRoom() const {
    int num;
    std::cout << "\n  Enter room number: ";
    std::cin >> num;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int idx = findRoomIndex(num);
    if (idx == -1) {
        std::cout << "  [!] Room " << num << " not found.\n";
        return;
    }
    std::cout << "\n";
    rooms[idx].displayInfo();
}

// =====================================================================
//  3. Check-In
// =====================================================================

void Hotel::checkIn() {
    std::cout << "\n+========================================+\n";
    std::cout << "|             GUEST CHECK-IN             |\n";
    std::cout << "+========================================+\n\n";

    std::string name, phone, address;
    std::cout << "  Guest Name    : ";
    std::getline(std::cin, name);
    std::cout << "  Phone Number  : ";
    std::getline(std::cin, phone);
    std::cout << "  Address       : ";
    std::getline(std::cin, address);

    std::cout << "\n  Room Types:  1) Single ($50)  2) Double ($80)  3) Suite ($150)\n";
    std::cout << "  Select type (1-3): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string type;
    switch (choice) {
        case 1: type = "Single"; break;
        case 2: type = "Double"; break;
        case 3: type = "Suite";  break;
        default:
            std::cout << "  [!] Invalid choice.\n";
            return;
    }

    int idx = findAvailableRoom(type);
    if (idx == -1) {
        std::cout << "  [!] No available " << type << " rooms.\n";
        return;
    }

    std::string date = getCurrentDate();
    rooms[idx].checkIn(name, date);

    Customer cust(nextCustomerId++, name, phone, address);
    customers.push_back(cust);

    std::cout << "\n  [OK] Check-in successful!\n";
    std::cout << "  Assigned Room : " << rooms[idx].getRoomNumber() << "\n";
    std::cout << "  Check-In Date : " << date << "\n";
    std::cout << "  Customer ID   : " << cust.getId() << "\n\n";
}

// =====================================================================
//  4. Check-Out
// =====================================================================

void Hotel::checkOut() {
    std::cout << "\n+========================================+\n";
    std::cout << "|            GUEST CHECK-OUT             |\n";
    std::cout << "+========================================+\n\n";

    int num;
    std::cout << "  Enter room number: ";
    std::cin >> num;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int idx = findRoomIndex(num);
    if (idx == -1) {
        std::cout << "  [!] Room " << num << " not found.\n";
        return;
    }
    if (!rooms[idx].isOccupied()) {
        std::cout << "  [!] Room " << num << " is not occupied.\n";
        return;
    }

    std::string checkOutDate = getCurrentDate();
    int nights = calculateNights(rooms[idx].getCheckInDate(), checkOutDate);

    double additional = 0.0;
    std::cout << "  Any additional charges (e.g. minibar, laundry)? $";
    std::cin >> additional;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Billing bill(nextBillId++, rooms[idx].getGuestName(), num,
                 rooms[idx].getCheckInDate(), checkOutDate,
                 nights, rooms[idx].getPricePerNight(), additional);
    bills.push_back(bill);

    bill.displayInvoice();

    rooms[idx].checkOut();
    std::cout << "  [OK] Check-out complete. Room " << num << " is now available.\n\n";
}

// =====================================================================
//  5. Display Guest Details (by room)
// =====================================================================

void Hotel::displayGuestDetails() const {
    int num;
    std::cout << "\n  Enter room number: ";
    std::cin >> num;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int idx = findRoomIndex(num);
    if (idx == -1) {
        std::cout << "  [!] Room " << num << " not found.\n";
        return;
    }
    if (!rooms[idx].isOccupied()) {
        std::cout << "  [!] Room " << num << " is currently empty.\n";
        return;
    }

    // Find matching customer record
    const std::string& guest = rooms[idx].getGuestName();
    std::cout << "\n";
    rooms[idx].displayInfo();

    for (auto it = customers.rbegin(); it != customers.rend(); ++it) {
        if (it->getName() == guest) {
            it->displayInfo();
            break;
        }
    }
}

// =====================================================================
//  6. View All Customers
// =====================================================================

void Hotel::viewAllCustomers() const {
    std::cout << "\n+========================================+\n";
    std::cout << "|           ALL CUSTOMERS                |\n";
    std::cout << "+========================================+\n\n";

    if (customers.empty()) {
        std::cout << "  No customer records found.\n\n";
        return;
    }

    std::cout << "  " << std::left << std::setw(6)  << "ID"
              << std::setw(20) << "Name"
              << std::setw(15) << "Phone"
              << "Address" << "\n";
    std::cout << "  " << std::string(60, '-') << "\n";

    for (const auto& c : customers) {
        std::cout << "  " << std::left << std::setw(6)  << c.getId()
                  << std::setw(20) << c.getName()
                  << std::setw(15) << c.getPhone()
                  << c.getAddress() << "\n";
    }
    std::cout << "\n  Total customers: " << customers.size() << "\n\n";
}

// =====================================================================
//  7. Generate Bill (for occupied room)
// =====================================================================

void Hotel::generateBill() const {
    int num;
    std::cout << "\n  Enter room number: ";
    std::cin >> num;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int idx = findRoomIndex(num);
    if (idx == -1) {
        std::cout << "  [!] Room " << num << " not found.\n";
        return;
    }
    if (!rooms[idx].isOccupied()) {
        std::cout << "  [!] Room " << num << " is not occupied.\n";
        return;
    }

    std::string today = getCurrentDate();
    int nights = calculateNights(rooms[idx].getCheckInDate(), today);

    // Preview bill (not saved — actual billing happens at check-out)
    Billing preview(0, rooms[idx].getGuestName(), num,
                    rooms[idx].getCheckInDate(), today,
                    nights, rooms[idx].getPricePerNight(), 0.0);
    std::cout << "\n  ** Estimated Bill (additional charges added at check-out) **";
    preview.displayInvoice();
}

// =====================================================================
//  8. View Billing History
// =====================================================================

void Hotel::viewBillingHistory() const {
    std::cout << "\n+========================================+\n";
    std::cout << "|          BILLING HISTORY               |\n";
    std::cout << "+========================================+\n\n";

    if (bills.empty()) {
        std::cout << "  No billing records found.\n\n";
        return;
    }

    std::cout << "  " << std::left << std::setw(8)  << "Bill#"
              << std::setw(20) << "Customer"
              << std::setw(8)  << "Room"
              << std::setw(12) << "Total"
              << "Check-Out" << "\n";
    std::cout << "  " << std::string(58, '-') << "\n";

    for (const auto& b : bills)
        b.displayBrief();

    std::cout << "\n  Total invoices: " << bills.size() << "\n\n";
}

// =====================================================================
//  Menu & Main Loop
// =====================================================================

void Hotel::showMenu() const {
    std::cout << "\n";
    std::cout << "+========================================+\n";
    std::cout << "|      HOTEL MANAGEMENT SYSTEM           |\n";
    std::cout << "+========================================+\n";
    std::cout << "|  1. Display Available Rooms             |\n";
    std::cout << "|  2. Search Room by Number               |\n";
    std::cout << "|  3. Check-In Guest                      |\n";
    std::cout << "|  4. Check-Out Guest                     |\n";
    std::cout << "|  5. Display Guest Details                |\n";
    std::cout << "|  6. View All Customers                  |\n";
    std::cout << "|  7. Generate Bill / Invoice              |\n";
    std::cout << "|  8. View Billing History                 |\n";
    std::cout << "|  9. Save & Exit                          |\n";
    std::cout << "+========================================+\n";
    std::cout << "  Enter your choice (1-9): ";
}

void Hotel::run() {
    loadData();
    std::cout << "\n  [i] Hotel Management System loaded successfully.\n";
    std::cout << "  [i] " << rooms.size() << " rooms | "
              << customers.size() << " customers | "
              << bills.size() << " bills on file.\n";

    bool running = true;
    while (running) {
        showMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: displayAvailableRooms();  break;
            case 2: searchRoom();             break;
            case 3: checkIn();                break;
            case 4: checkOut();               break;
            case 5: displayGuestDetails();    break;
            case 6: viewAllCustomers();       break;
            case 7: generateBill();           break;
            case 8: viewBillingHistory();     break;
            case 9:
                saveData();
                std::cout << "\n  [OK] Data saved. Thank you for using the Hotel Management System!\n\n";
                running = false;
                break;
            default:
                std::cout << "  [!] Invalid choice. Please select 1-9.\n";
                break;
        }
    }
}
