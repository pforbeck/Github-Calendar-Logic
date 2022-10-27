#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "cordConstruct.h"
#include "jsonConstruct.h"

void clearScreen() {
#ifdef _WIN32 // Includes both 32 bit and 64 bit
    system("CLS"); // Clear screen
#else
    system("clear"); // Clear screen
#endif
    std::cout << std::flush; // Flushes the iostream
}

int displayMenu() {
    std::string menuString; // Stores the input.
    int menuNumber; // Stores the scrubbed input.
    std::cout << "Do you want to....\n\t1. Display your Github Activity Calendar\n\t2. Activate Rainbow Mode\n\t3. View the time\n\t4. Set a timer\n\t5. Clear\n\t0. Exit\n\nYour Choice: "; // Outputs the main menu
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

std::vector<int> gitData() {
    std::string input, output, result;
    std::array<char, 128> buffer;
    // Why is this not in the switch case? Because the initialization of "pipe" was throwing an error I didn't know how to resolve and this was a good side-step.
    std::cout << "Enter GitHub username: ";
    getline(std::cin, input);
    output = "curl http://github-calendar.herokuapp.com/commits/" + input;  // Sends a curl command to the command line
    remove(output.c_str());                                                 // that grabs the GitHub calendar data in a JSON format
#ifdef _WIN32 // Includes both 32 bit and 64 bit                        // and returns it as a string to 'result'
    std::shared_ptr<FILE> pipe(_popen(output.c_str(), "r"), _pclose);   // Don't worry about this code, it works and even I don't get it
#else
    std::shared_ptr<FILE> pipe(popen(output.c_str(), "r"), pclose);
#endif
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    clearScreen();

    // Send processed JSON data to "http://192.168.4.1/led" as a curl POST request
    int firstLoc = result.find_first_of("[") + 1; // Trims the front of the data string
    int lastLoc = result.find_first_of("]"); // Trims the back of the data string
    std::vector<int> resultsVector; // Puts the string data into a vector
    std::istringstream split(result.substr(firstLoc, lastLoc - firstLoc));
    for (std::string each; std::getline(split, each, ',');) {
        resultsVector.push_back(std::stoi(each));
    }

    //std::reverse(resultsVector.begin(), resultsVector.end()); // Reverses the vector so it goes from most to least recent
    return resultsVector;
}

std::ofstream openFileStream() {
    std::ofstream myfile("LEDData.json"); // Accesses/creates a JSON file that I can output to

    if (!myfile.is_open()) { // Checks the file is open
        std::cout << "Error! Exiting program...";
        exit(1);
    }
    return myfile;
}
void sendJson() {
    //std::string ipAdd = "192.168.0.1";  // IP address that the system POSTS the JSON file to.
    std::string ipAdd = "http://192.168.4.1/led";
    std::string JSONoutput = "curl --json @LEDData.json " + ipAdd;
    system(JSONoutput.c_str());
    return;
}

void clearLed() {
    std::ofstream myfile = openFileStream();
    std::vector<std::string> blank;
    jsonConstruct json = jsonConstruct(blank);
    myfile << json.clear();
    myfile.close();
    sendJson();
    clearScreen();
    return;

}

void gitCalendar() {
    clearLed();
    std::vector<int> resultsVector = gitData();
    std::ofstream myfile = openFileStream();

    int k = 0; // k is the index of the vector
    std::vector<std::string> cordsbuffer;
    int roy{}, gee{}, biv{}; // The values for R, G, and B in the JSON file
    for (int x = 0; x < 32; x++) { // This is for X (NOTE: I am not sure these values are right, please double check)
        for (int y = 0; y < 7; y++) { // This is for Y
            if (!(resultsVector.at(k) == 0)) {

                if (1 <= resultsVector.at(k) && resultsVector.at(k) <= 5) {
                    roy = 56;
                    gee = 221;
                    biv = 52;
                }
                if (6 <= resultsVector.at(k) && resultsVector.at(k) <= 10) {
                    roy = 69;
                    gee = 160;
                    biv = 69;
                }
                if (11 <= resultsVector.at(k) && resultsVector.at(k) <= 15) {
                    roy = 4;
                    gee = 117;
                    biv = 38;
                }
                if (16 <= resultsVector.at(k)) {
                    roy = 10;
                    gee = 66;
                    biv = 8;
                }
                cordConstruct cords = cordConstruct(x, y, roy, gee, biv);
                cordsbuffer.push_back(cords.data());
            }
            k++;
        }
    }
    jsonConstruct json = jsonConstruct(cordsbuffer);
    myfile << json.data();

    myfile.close(); // Closes file

    sendJson();

    clearScreen();
    if (resultsVector.size() > 0) {
        std::cout << "Success!\n" << std::endl; // The GitHub calendar was successfully fetched and sent to the microcontroller
    }

    return; // Returns to main menu
}

void rainbow() {
    clearLed();
    std::ofstream myfile = openFileStream();
    int k = 0; // k is the index of the vector
    std::vector<std::string> cordsbuffer;
    int roy{}, gee{}, biv{}; // The values for R, G, and B in the JSON file
    for (int x = 0; x < 32; x++) { // This is for X (NOTE: I am not sure these values are right, please double check)
        for (int y = 0; y < 8; y++) { // This is for Y
            roy = std::rand() % 256;
            gee = std::rand() % 256;
            biv = std::rand() % 256;

            cordConstruct cords = cordConstruct(x, y, roy, gee, biv);
            cordsbuffer.push_back(cords.data());
        }
    }
    jsonConstruct json = jsonConstruct(cordsbuffer);
    myfile << json.data();
    myfile.close(); // Closes file
    sendJson();
    clearScreen();
}

void timer() {
    clearLed();
    std::ofstream myfile = openFileStream();
    int k = 0; // k is the index of the vector
    std::vector<std::string> cordsbuffer;
    int roy{}, gee{}, biv{}; // The values for R, G, and B in the JSON file
    for (int x = 0; x < 32; x++) { // This is for X (NOTE: I am not sure these values are right, please double check)
        for (int y = 0; y < 8; y++) { // This is for Y
            roy = 128;
            gee = 0;
            biv = 0;
            cordConstruct cords = cordConstruct(x, y, roy, gee, biv);
            cordsbuffer.push_back(cords.data());
        }
    }
    jsonConstruct json = jsonConstruct(cordsbuffer);
    myfile << json.timer();
    myfile.close(); // Closes file
    sendJson();
    clearScreen();
}

void menuExecution(int optionNumber) {
    std::string notDone = "This has yet to be implemented... Returning to menu!\n";

    switch (optionNumber) {
    case 1:
        gitCalendar();
        break;
    case 2: // Rainbow Mode
        rainbow();
        break;
    case 3: // Clock
        std::cout << notDone << std::endl;
        break;
    case 4: // Timer
        timer();
        break;
    case 5: // Clear
        clearLed();
		break;
    case 0: // Exit
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

/* FOR REFERENCE
{
    "count":4,
        "clear" : false,
        "cords" : [
                    X  Y  R   G    B
                [0, 0, 0, 0, 0],        0
                [0, 1, 56, 221, 52],    1
                [0, 2, 0, 160, 0],      2
                [0, 3, 4, 117, 38],     3
                [0, 4, 0, 160, 0]       4
        ]
}
*/
