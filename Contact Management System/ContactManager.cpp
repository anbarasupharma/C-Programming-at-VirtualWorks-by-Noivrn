#include "ContactManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

// ── Constructor ──────────────────────────────────────────────

ContactManager::ContactManager(const std::string& filename)
    : filename(filename), nextId(1) {
    loadContacts();
}

// ── File persistence ─────────────────────────────────────────

void ContactManager::loadContacts() {
    contacts.clear();
    std::ifstream fin(filename);
    if (!fin.is_open()) return;          // first run — no file yet

    Contact c;
    while (c.loadFromFile(fin))
        contacts.push_back(c);

    fin.close();
    generateNextId();
}

void ContactManager::saveContacts() const {
    std::ofstream fout(filename, std::ios::trunc);
    if (!fout.is_open()) {
        std::cerr << "\n  [!] Error: Could not open file for writing.\n";
        return;
    }
    for (const auto& c : contacts)
        c.saveToFile(fout);
    fout.close();
}

// ── Helpers ──────────────────────────────────────────────────

int ContactManager::findIndexById(int id) const {
    for (size_t i = 0; i < contacts.size(); ++i)
        if (contacts[i].getId() == id) return static_cast<int>(i);
    return -1;
}

void ContactManager::generateNextId() {
    nextId = 1;
    for (const auto& c : contacts)
        if (c.getId() >= nextId) nextId = c.getId() + 1;
}

// ── Helper to read a non-empty trimmed line ──────────────────

static std::string readLine(const std::string& prompt) {
    std::string value;
    do {
        std::cout << prompt;
        std::getline(std::cin, value);
    } while (value.empty());
    return value;
}

// ── Add Contact ──────────────────────────────────────────────

void ContactManager::addContact() {
    std::cout << "\n  === Add New Contact ===\n";

    std::string name    = readLine("  Name    : ");
    std::string phone   = readLine("  Phone   : ");
    std::string email   = readLine("  Email   : ");
    std::string address = readLine("  Address : ");

    Contact c(nextId++, name, phone, email, address);
    contacts.push_back(c);
    saveContacts();

    std::cout << "\n  [+] Contact added successfully (ID: " << c.getId() << ").\n";
}

// ── View All ─────────────────────────────────────────────────

void ContactManager::viewAllContacts() const {
    if (contacts.empty()) {
        std::cout << "\n  [i] No contacts found.\n";
        return;
    }

    std::cout << "\n  === All Contacts (" << contacts.size() << ") ===\n\n";
    std::cout << "  " << std::left
              << std::setw(6)  << "ID"
              << std::setw(22) << "Name"
              << std::setw(16) << "Phone"
              << "Email" << "\n";
    std::cout << "  " << std::string(60, '-') << "\n";

    for (const auto& c : contacts)
        c.displayShort();
}

// ── Search ───────────────────────────────────────────────────

void ContactManager::searchContacts() const {
    if (contacts.empty()) {
        std::cout << "\n  [i] No contacts to search.\n";
        return;
    }

    std::cout << "\n  === Search Contacts ===\n";
    std::cout << "  Search by:\n";
    std::cout << "    1. Name\n";
    std::cout << "    2. Phone\n";
    std::cout << "    3. Email\n";
    std::cout << "  Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string query = readLine("  Enter search term: ");

    // Convert query to lowercase for case-insensitive search
    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

    std::vector<const Contact*> results;
    for (const auto& c : contacts) {
        std::string field;
        switch (choice) {
            case 1: field = c.getName();  break;
            case 2: field = c.getPhone(); break;
            case 3: field = c.getEmail(); break;
            default:
                std::cout << "  [!] Invalid choice.\n";
                return;
        }
        std::string fieldLower = field;
        std::transform(fieldLower.begin(), fieldLower.end(), fieldLower.begin(), ::tolower);

        if (fieldLower.find(queryLower) != std::string::npos)
            results.push_back(&c);
    }

    if (results.empty()) {
        std::cout << "\n  [i] No matching contacts found.\n";
    } else {
        std::cout << "\n  === Search Results (" << results.size() << ") ===\n";
        for (const auto* cp : results)
            cp->displayInfo();
    }
}

// ── Edit Contact ─────────────────────────────────────────────

void ContactManager::editContact() {
    if (contacts.empty()) {
        std::cout << "\n  [i] No contacts to edit.\n";
        return;
    }

    viewAllContacts();
    std::cout << "\n  Enter contact ID to edit: ";
    int id;
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int idx = findIndexById(id);
    if (idx == -1) {
        std::cout << "  [!] Contact with ID " << id << " not found.\n";
        return;
    }

    Contact& c = contacts[idx];
    c.displayInfo();

    std::cout << "\n  Enter new values (press Enter to keep current):\n";

    std::string input;
    std::cout << "  Name    [" << c.getName()    << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) c.setName(input);

    std::cout << "  Phone   [" << c.getPhone()   << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) c.setPhone(input);

    std::cout << "  Email   [" << c.getEmail()   << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) c.setEmail(input);

    std::cout << "  Address [" << c.getAddress() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) c.setAddress(input);

    saveContacts();
    std::cout << "\n  [+] Contact updated successfully.\n";
}

// ── Delete Contact ───────────────────────────────────────────

void ContactManager::deleteContact() {
    if (contacts.empty()) {
        std::cout << "\n  [i] No contacts to delete.\n";
        return;
    }

    viewAllContacts();
    std::cout << "\n  Enter contact ID to delete: ";
    int id;
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int idx = findIndexById(id);
    if (idx == -1) {
        std::cout << "  [!] Contact with ID " << id << " not found.\n";
        return;
    }

    contacts[idx].displayInfo();
    std::cout << "\n  Are you sure? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        contacts.erase(contacts.begin() + idx);
        saveContacts();
        std::cout << "  [-] Contact deleted successfully.\n";
    } else {
        std::cout << "  [i] Deletion cancelled.\n";
    }
}

// ── Main Menu Loop ───────────────────────────────────────────

void ContactManager::run() {
    int choice = 0;

    while (true) {
        std::cout << "\n  ==========================================\n";
        std::cout << "       C O N T A C T   M A N A G E R       \n";
        std::cout << "  ==========================================\n";
        std::cout << "    1. Add Contact\n";
        std::cout << "    2. View All Contacts\n";
        std::cout << "    3. Search Contacts\n";
        std::cout << "    4. Edit Contact\n";
        std::cout << "    5. Delete Contact\n";
        std::cout << "    0. Exit\n";
        std::cout << "  ------------------------------------------\n";
        std::cout << "  Choice: ";

        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addContact();       break;
            case 2: viewAllContacts();  break;
            case 3: searchContacts();   break;
            case 4: editContact();      break;
            case 5: deleteContact();    break;
            case 0:
                std::cout << "\n  Goodbye!\n\n";
                return;
            default:
                std::cout << "  [!] Invalid option. Try again.\n";
        }
    }
}
