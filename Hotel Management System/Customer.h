#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <fstream>

class Customer {
private:
    int id;
    std::string name;
    std::string phone;
    std::string address;

public:
    // Constructors
    Customer();
    Customer(int id, const std::string& name, const std::string& phone, const std::string& address);

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getAddress() const;

    // Display
    void displayInfo() const;

    // File I/O
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

#endif // CUSTOMER_H
