#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

int main() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    char playAgain = 'y';

    std::cout << "========================================\n";
    std::cout << "     Welcome to the Number Guessing Game!\n";
    std::cout << "========================================\n";

    while (playAgain == 'y' || playAgain == 'Y') {
        // Generate a random number between 1 and 100
        int secretNumber = (std::rand() % 100) + 1;
        int guess = 0;
        int attempts = 0;

        std::cout << "\nI'm thinking of a number between 1 and 100.\n";
        std::cout << "Can you guess what it is?\n\n";

        // Game loop — keep going until the user guesses correctly
        while (guess != secretNumber) {
            std::cout << "Enter your guess: ";

            // Validate input
            if (!(std::cin >> guess)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter a number.\n\n";
                continue;
            }

            attempts++;

            if (guess < 1 || guess > 100) {
                std::cout << "Please guess a number between 1 and 100.\n\n";
            } else if (guess < secretNumber) {
                std::cout << "Too Low! Try again.\n\n";
            } else if (guess > secretNumber) {
                std::cout << "Too High! Try again.\n\n";
            } else {
                std::cout << "\nCongratulations! You guessed the number " 
                          << secretNumber << " in " << attempts << " attempt"
                          << (attempts == 1 ? "" : "s") << "!\n";
            }
        }

        // Rating based on number of attempts
        std::cout << "\n--- Performance ---\n";
        if (attempts <= 3) {
            std::cout << "Outstanding! You're a mind reader!\n";
        } else if (attempts <= 7) {
            std::cout << "Great job! Very impressive!\n";
        } else if (attempts <= 10) {
            std::cout << "Good effort! Not bad at all.\n";
        } else {
            std::cout << "You got there eventually! Keep practicing.\n";
        }

        std::cout << "\nWould you like to play again? (y/n): ";
        std::cin >> playAgain;
    }

    std::cout << "\nThanks for playing! Goodbye.\n";

    return 0;
}
