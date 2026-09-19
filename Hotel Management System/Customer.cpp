#include "Customer.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ---------- Constructors ----------

Customer::Customer()
    : id(0), name(""), phone(""), address("") {}

Customer::Customer(int id, const std::string& name,
                   const std::string& phone, const std::string& address)
    : id(id), name(name), phone(phone), address(address) {}

// ---------- Getters ----------

int Customer::getId() const { return id; }
std::string Customer::getName() const { return name; }
std::string Customer::getPhone() const { return phone; }
std::string Customer::getAddress() const { return address; }

// ---------- Display ----------

void Customer::displayInfo() const {
    std::cout << "+----------------------------------------+\n";
    std::cout << "|          CUSTOMER INFORMATION           |\n";
    std::cout << "+----------------------------------------+\n";
    std::cout << "| Customer ID : " << std::left << std::setw(24) << id << "|\n";
    std::cout << "| Name        : " << std::left << std::setw(24) << name << "|\n";
    std::cout << "| Phone       : " << std::left << std::setw(24) << phone << "|\n";
    std::cout << "| Address     : " << std::left << std::setw(24) << address << "|\n";
    std::cout << "+----------------------------------------+\n";
}

// ---------- File I/O ----------

void Customer::saveToFile(std::ofstream& out) const {
    out << id << "|"
        << name << "|"
        << phone << "|"
        << address << "\n";
}

void Customer::loadFromFile(std::ifstream& in) {
    std::string line;
    if (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, '|'); id = std::stoi(token);
        std::getline(ss, name, '|');
        std::getline(ss, phone, '|');
        std::getline(ss, address, '|');
    }
}
