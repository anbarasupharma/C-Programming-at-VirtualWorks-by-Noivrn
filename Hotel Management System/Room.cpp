#include "Room.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ---------- Constructors ----------

Room::Room()
    : roomNumber(0), type(""), pricePerNight(0.0),
      occupied(false), guestName(""), checkInDate("") {}

Room::Room(int roomNumber, const std::string& type, double pricePerNight)
    : roomNumber(roomNumber), type(type), pricePerNight(pricePerNight),
      occupied(false), guestName(""), checkInDate("") {}

// ---------- Getters ----------

int Room::getRoomNumber() const { return roomNumber; }
std::string Room::getType() const { return type; }
double Room::getPricePerNight() const { return pricePerNight; }
bool Room::isOccupied() const { return occupied; }
std::string Room::getGuestName() const { return guestName; }
std::string Room::getCheckInDate() const { return checkInDate; }

// ---------- Operations ----------

void Room::checkIn(const std::string& guest, const std::string& date) {
    occupied = true;
    guestName = guest;
    checkInDate = date;
}

void Room::checkOut() {
    occupied = false;
    guestName = "";
    checkInDate = "";
}

// ---------- Display ----------

void Room::displayInfo() const {
    std::cout << "+----------------------------------------+\n";
    std::cout << "|           ROOM INFORMATION             |\n";
    std::cout << "+----------------------------------------+\n";
    std::cout << "| Room Number   : " << std::left << std::setw(22) << roomNumber << "|\n";
    std::cout << "| Room Type     : " << std::left << std::setw(22) << type << "|\n";
    std::cout << "| Price / Night : $" << std::left << std::setw(21)
              << std::fixed << std::setprecision(2) << pricePerNight << "|\n";
    std::cout << "| Status        : " << std::left << std::setw(22)
              << (occupied ? "Occupied" : "Available") << "|\n";
    if (occupied) {
        std::cout << "| Guest Name    : " << std::left << std::setw(22) << guestName << "|\n";
        std::cout << "| Check-In Date : " << std::left << std::setw(22) << checkInDate << "|\n";
    }
    std::cout << "+----------------------------------------+\n";
}

void Room::displayBrief() const {
    std::cout << "  " << std::left << std::setw(8) << roomNumber
              << std::setw(10) << type
              << "$" << std::setw(9) << std::fixed << std::setprecision(2) << pricePerNight
              << std::setw(12) << (occupied ? "Occupied" : "Available")
              << "\n";
}

// ---------- File I/O ----------

void Room::saveToFile(std::ofstream& out) const {
    out << roomNumber << "|"
        << type << "|"
        << pricePerNight << "|"
        << occupied << "|"
        << guestName << "|"
        << checkInDate << "\n";
}

void Room::loadFromFile(std::ifstream& in) {
    std::string line;
    if (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, '|'); roomNumber = std::stoi(token);
        std::getline(ss, type, '|');
        std::getline(ss, token, '|'); pricePerNight = std::stod(token);
        std::getline(ss, token, '|'); occupied = (token == "1");
        std::getline(ss, guestName, '|');
        std::getline(ss, checkInDate, '|');
    }
}
