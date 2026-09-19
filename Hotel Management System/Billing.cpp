#include "Billing.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ---------- Constructors ----------

Billing::Billing()
    : billId(0), customerName(""), roomNumber(0),
      checkInDate(""), checkOutDate(""), totalNights(0),
      roomCharge(0.0), additionalCharges(0.0), totalAmount(0.0) {}

Billing::Billing(int billId, const std::string& customerName, int roomNumber,
                 const std::string& checkInDate, const std::string& checkOutDate,
                 int totalNights, double pricePerNight, double additionalCharges)
    : billId(billId), customerName(customerName), roomNumber(roomNumber),
      checkInDate(checkInDate), checkOutDate(checkOutDate),
      totalNights(totalNights),
      roomCharge(totalNights * pricePerNight),
      additionalCharges(additionalCharges),
      totalAmount(totalNights * pricePerNight + additionalCharges) {}

// ---------- Getters ----------

int Billing::getBillId() const { return billId; }
std::string Billing::getCustomerName() const { return customerName; }
int Billing::getRoomNumber() const { return roomNumber; }
double Billing::getTotalAmount() const { return totalAmount; }

// ---------- Display ----------

void Billing::displayInvoice() const {
    std::cout << "\n";
    std::cout << "+================================================+\n";
    std::cout << "|                  HOTEL INVOICE                  |\n";
    std::cout << "+================================================+\n";
    std::cout << "| Bill ID        : " << std::left << std::setw(30) << billId << "|\n";
    std::cout << "| Customer Name  : " << std::left << std::setw(30) << customerName << "|\n";
    std::cout << "| Room Number    : " << std::left << std::setw(30) << roomNumber << "|\n";
    std::cout << "+------------------------------------------------+\n";
    std::cout << "| Check-In Date  : " << std::left << std::setw(30) << checkInDate << "|\n";
    std::cout << "| Check-Out Date : " << std::left << std::setw(30) << checkOutDate << "|\n";
    std::cout << "| Total Nights   : " << std::left << std::setw(30) << totalNights << "|\n";
    std::cout << "+------------------------------------------------+\n";

    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << roomCharge;
    std::cout << "| Room Charge        : " << std::left << std::setw(27) << oss.str() << "|\n";

    oss.str(""); oss.clear();
    oss << "$" << std::fixed << std::setprecision(2) << additionalCharges;
    std::cout << "| Additional Charges : " << std::left << std::setw(27) << oss.str() << "|\n";

    std::cout << "+------------------------------------------------+\n";

    oss.str(""); oss.clear();
    oss << "$" << std::fixed << std::setprecision(2) << totalAmount;
    std::cout << "| TOTAL AMOUNT       : " << std::left << std::setw(27) << oss.str() << "|\n";

    std::cout << "+================================================+\n\n";
}

void Billing::displayBrief() const {
    std::cout << "  " << std::left << std::setw(8) << billId
              << std::setw(20) << customerName
              << std::setw(8) << roomNumber
              << "$" << std::setw(11) << std::fixed << std::setprecision(2) << totalAmount
              << checkOutDate << "\n";
}

// ---------- File I/O ----------

void Billing::saveToFile(std::ofstream& out) const {
    out << billId << "|"
        << customerName << "|"
        << roomNumber << "|"
        << checkInDate << "|"
        << checkOutDate << "|"
        << totalNights << "|"
        << roomCharge << "|"
        << additionalCharges << "|"
        << totalAmount << "\n";
}

void Billing::loadFromFile(std::ifstream& in) {
    std::string line;
    if (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, '|'); billId = std::stoi(token);
        std::getline(ss, customerName, '|');
        std::getline(ss, token, '|'); roomNumber = std::stoi(token);
        std::getline(ss, checkInDate, '|');
        std::getline(ss, checkOutDate, '|');
        std::getline(ss, token, '|'); totalNights = std::stoi(token);
        std::getline(ss, token, '|'); roomCharge = std::stod(token);
        std::getline(ss, token, '|'); additionalCharges = std::stod(token);
        std::getline(ss, token, '|'); totalAmount = std::stod(token);
    }
}
