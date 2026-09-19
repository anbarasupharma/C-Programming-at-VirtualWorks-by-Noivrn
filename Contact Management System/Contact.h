#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <fstream>

class Contact {
private:
    int id;
    std::string name;
    std::string phone;
    std::string email;
    std::string address;

public:
    // Constructors
    Contact();
    Contact(int id, const std::string& name, const std::string& phone,
            const std::string& email, const std::string& address);

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getEmail() const;
    std::string getAddress() const;

    // Setters
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setAddress(const std::string& address);

    // Display
    void displayInfo() const;
    void displayShort() const;

    // File I/O — pipe-delimited format
    void saveToFile(std::ofstream& out) const;
    bool loadFromFile(std::ifstream& in);
};

#endif // CONTACT_H
