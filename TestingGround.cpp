#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

void clearScreen() {
    system("CLS"); // Clear screen
    std::cout << std::flush; // Flushes the iostream
}

int displayMenu() {
    std::string menuString;
    int menuNumber;
    std::cout << "Do you want to....\n\t1. Display your Github Activity Calendar\n\t2. Activate Rainbow Mode\n\t3. View the time\n\t4. Set a timer\n\t5. Exit\n\nYour Choice: "; // Outputs the main menu
    getline(std::cin, menuString);

    try {
        menuNumber = std::stoi(menuString);
        if (menuNumber < 1 || menuNumber > 5) {
            throw std::invalid_argument("");
        }
    } // Checks that the user input is a valid number
    catch (std::exception& e) {
        std::cout << "Invalid input! Ending program...";
        exit(1);
    }

    clearScreen();
    return menuNumber;
}

void menuExecution(int optionNumber) {
    std::string input, output, result;
    std::array<char, 128> buffer;

    if (optionNumber == 1) { // Why is this not in the switch case? Because the initialization of "pipe" was throwing an error I didn't know how to resolve and this was a good side-step.
        std::cout << "Enter GitHub username: ";
        getline(std::cin, input);
        output = "curl http://github-calendar.herokuapp.com/commits/" + input;
        remove(output.c_str());
        std::shared_ptr<FILE> pipe(_popen(output.c_str(), "r"), _pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        std::cout << result << std::endl;
        clearScreen();
        if (result.length() > 0) {
            std::cout << "Success!" << std::endl;
        }

        // Execute more code to change the LED array here with the JSON data from "result".

        return;
    }

    switch (optionNumber) {
    case 2: // Rainbow Mode
        std::cout << "This has yet to be implemented... Returning to menu!\n" << std::endl;
        break;
    case 3: // Clock
        std::cout << "This has yet to be implemented... Returning to menu!\n" << std::endl;
        break;
    case 4: // Timer
        std::cout << "This has yet to be implemented... Returning to menu!\n" << std::endl;
        break;
    case 5: // Exit
        std::cout << "See you next time! Exiting program..." << std::endl;
        exit(0);
        break;
    default:
        std::cout << "An error ocurred! Ending program..." << std::endl;
        exit(1);
    }
}

int main()
{
    int optionNumber;
    while (true) {
        optionNumber = displayMenu();
        menuExecution(optionNumber);
    }
    return 0;
}