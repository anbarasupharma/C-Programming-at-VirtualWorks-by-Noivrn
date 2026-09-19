#ifndef HOTEL_H
#define HOTEL_H

#include "Room.h"
#include "Customer.h"
#include "Billing.h"
#include <vector>
#include <string>

class Hotel {
private:
    std::vector<Room> rooms;
    std::vector<Customer> customers;
    std::vector<Billing> bills;
    int nextCustomerId;
    int nextBillId;

    // Data file paths
    const std::string ROOMS_FILE    = "rooms.txt";
    const std::string CUSTOMERS_FILE = "customers.txt";
    const std::string BILLING_FILE  = "billing.txt";

    // Helpers
    int findRoomIndex(int roomNumber) const;
    int findAvailableRoom(const std::string& type) const;
    std::string getCurrentDate() const;
    int calculateNights(const std::string& checkIn, const std::string& checkOut) const;
    void initializeDefaultRooms();

public:
    Hotel();

    // Data persistence
    void loadData();
    void saveData() const;

    // Menu operations
    void displayAvailableRooms() const;
    void searchRoom() const;
    void checkIn();
    void checkOut();
    void displayGuestDetails() const;
    void viewAllCustomers() const;
    void generateBill() const;
    void viewBillingHistory() const;

    // Menu
    void showMenu() const;
    void run();
};

#endif // HOTEL_H
