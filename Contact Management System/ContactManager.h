#ifndef CONTACTMANAGER_H
#define CONTACTMANAGER_H

#include "Contact.h"
#include <vector>
#include <string>

class ContactManager {
private:
    std::vector<Contact> contacts;
    std::string filename;
    int nextId;

    // Internal helpers
    void loadContacts();
    void saveContacts() const;
    int findIndexById(int id) const;
    void generateNextId();

public:
    ContactManager(const std::string& filename = "contacts.dat");

    // Core operations
    void addContact();
    void viewAllContacts() const;
    void searchContacts() const;
    void editContact();
    void deleteContact();

    // Menu
    void run();
};

#endif // CONTACTMANAGER_H
