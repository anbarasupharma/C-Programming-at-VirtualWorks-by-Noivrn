#ifndef BILLING_H
#define BILLING_H

#include <string>
#include <fstream>

class Billing {
private:
    int billId;
    std::string customerName;
    int roomNumber;
    std::string checkInDate;
    std::string checkOutDate;
    int totalNights;
    double roomCharge;
    double additionalCharges;
    double totalAmount;

public:
    // Constructors
    Billing();
    Billing(int billId, const std::string& customerName, int roomNumber,
            const std::string& checkInDate, const std::string& checkOutDate,
            int totalNights, double pricePerNight, double additionalCharges);

    // Getters
    int getBillId() const;
    std::string getCustomerName() const;
    int getRoomNumber() const;
    double getTotalAmount() const;

    // Display
    void displayInvoice() const;
    void displayBrief() const;

    // File I/O
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
};

#endif // BILLING_H
