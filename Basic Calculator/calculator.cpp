#include <iostream>
#include <limits>
using namespace std;

void displayMenu() {
    cout << "\n===========================\n";
    cout << "     Basic Calculator\n";
    cout << "===========================\n";
    cout << "  1. Addition       (+)\n";
    cout << "  2. Subtraction    (-)\n";
    cout << "  3. Multiplication (*)\n";
    cout << "  4. Division       (/)\n";
    cout << "  5. Exit\n";
    cout << "===========================\n";
    cout << "  Select an option: ";
}

int main() {
    int choice;
    double num1, num2, result;

    do {
        displayMenu();
        cin >> choice;

        // Validate menu input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  Invalid input. Please enter a number (1-5).\n";
            continue;
        }

        if (choice == 5) {
            cout << "\n  Goodbye!\n";
            break;
        }

        if (choice < 1 || choice > 5) {
            cout << "\n  Invalid option. Please choose 1-5.\n";
            continue;
        }

        // Get operands
        cout << "\n  Enter first number:  ";
        cin >> num1;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  Invalid number.\n";
            continue;
        }

        cout << "  Enter second number: ";
        cin >> num2;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  Invalid number.\n";
            continue;
        }

        // Perform the selected operation
        switch (choice) {
            case 1:
                result = num1 + num2;
                cout << "\n  " << num1 << " + " << num2 << " = " << result << endl;
                break;
            case 2:
                result = num1 - num2;
                cout << "\n  " << num1 << " - " << num2 << " = " << result << endl;
                break;
            case 3:
                result = num1 * num2;
                cout << "\n  " << num1 << " * " << num2 << " = " << result << endl;
                break;
            case 4:
                if (num2 == 0) {
                    cout << "\n  Error: Division by zero is not allowed.\n";
                } else {
                    result = num1 / num2;
                    cout << "\n  " << num1 << " / " << num2 << " = " << result << endl;
                }
                break;
        }

    } while (true);

    return 0;
}
