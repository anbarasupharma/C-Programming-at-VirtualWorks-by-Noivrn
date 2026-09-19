#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <fstream>

class Room {
private:
    int roomNumber;
    std::string type;        // "Single", "Double", "Suite"
    double pricePerNight;
    bool occupied;
    std::string guestName;
    std::string checkInDate; // "YYYY-MM-DD"

public:
    // Constructors
    Room();
    Room(int roomNumber, const std::string& type, double pricePerNight);

    // Getters
    int getRoomNumber() const;
    std::string getType() const;
    double getPricePerNight() const;
    bool isOccupied() const;
    std::string getGuestName() const;
    std::string getCheckInDate() const;

    // Operations
    void checkIn(const std::string& guestName, const std::string& date);
    void checkOut();

    // Display
    void displayInfo() const;
    void displayBrief() const;

    // File I/O
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

#endif // ROOM_H
