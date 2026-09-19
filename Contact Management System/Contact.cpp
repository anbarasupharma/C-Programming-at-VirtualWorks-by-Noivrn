#include "Contact.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ── Constructors ─────────────────────────────────────────────

Contact::Contact() : id(0) {}

Contact::Contact(int id, const std::string& name, const std::string& phone,
                 const std::string& email, const std::string& address)
    : id(id), name(name), phone(phone), email(email), address(address) {}

// ── Getters ──────────────────────────────────────────────────

int         Contact::getId()      const { return id; }
std::string Contact::getName()    const { return name; }
std::string Contact::getPhone()   const { return phone; }
std::string Contact::getEmail()   const { return email; }
std::string Contact::getAddress() const { return address; }

// ── Setters ──────────────────────────────────────────────────

void Contact::setName(const std::string& n)    { name = n; }
void Contact::setPhone(const std::string& p)   { phone = p; }
void Contact::setEmail(const std::string& e)   { email = e; }
void Contact::setAddress(const std::string& a) { address = a; }

// ── Display ──────────────────────────────────────────────────

void Contact::displayInfo() const {
    std::cout << "\n  +----------------------------------+\n";
    std::cout << "  | Contact Details                  |\n";
    std::cout << "  +----------------------------------+\n";
    std::cout << "  | ID      : " << id      << "\n";
    std::cout << "  | Name    : " << name    << "\n";
    std::cout << "  | Phone   : " << phone   << "\n";
    std::cout << "  | Email   : " << email   << "\n";
    std::cout << "  | Address : " << address << "\n";
    std::cout << "  +----------------------------------+\n";
}

void Contact::displayShort() const {
    std::cout << "  " << std::left
              << std::setw(6)  << id
              << std::setw(22) << name
              << std::setw(16) << phone
              << email << "\n";
}

// ── File I/O (pipe-delimited, one contact per line) ──────────

void Contact::saveToFile(std::ofstream& out) const {
    out << id << "|" << name << "|" << phone << "|"
        << email << "|" << address << "\n";
}

bool Contact::loadFromFile(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line) || line.empty())
        return false;

    std::istringstream ss(line);
    std::string idStr;

    std::getline(ss, idStr, '|');
    std::getline(ss, name, '|');
    std::getline(ss, phone, '|');
    std::getline(ss, email, '|');
    std::getline(ss, address);

    try {
        id = std::stoi(idStr);
    } catch (...) {
        return false;
    }
    return true;
}
