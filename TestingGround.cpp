#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <algorithm>

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
        clearScreen();

        // Send processed JSON data to "http://192.168.4.1/led" as a curl POST request

        int firstLoc = result.find_first_of("[") + 1; // Trims the front of the data string
        int lastLoc = result.find_first_of("]"); // Trims the back of the data string
        std::vector<char> resultsVector; // Puts the string data into a vector
        for (int i = firstLoc; i < lastLoc; i++) {
            resultsVector.push_back(result[i]);
            i++;
        }

        std::reverse(resultsVector.begin(), resultsVector.end()); // Reverses the vector so it goes from most to least recent

        std::ofstream myfile("LEDData.json"); // Accesses/creates a JSON file that I can output to

        if (!myfile.is_open()) {
            std::cout << "Error! Exiting program...";
            exit(1);
        }

        myfile << "{\n\"count\":" << resultsVector.size() << ",\n\"clear\" : false,\n\"cords\" : [\n";

        std::string coordinateData;
        int k = 0;
        int roy, gee, biv;
        for (int i = 0; i < 32; i++) { // This is for X (NOTE: I am not sure these values are right, please double check)
            for (int r = 0; r < 8; r++) { // This is for Y
                coordinateData = "\t[" + std::to_string(i); // Adds the X coord to the string to go into the JSON file
                coordinateData.append(", " + std::to_string(r)); // Adds the Y coord to the string to go into the JSON file
                switch (resultsVector.at(k)){
                case '0': // Why are these chars? I dunno... seriously. I'm losing it.
                    roy = 0;
                    gee = 0;
                    biv = 0;
                    break;
                case '1':
                    roy = 56;
                    gee = 221;
                    biv = 52;
                    break;
                case '2':
                    roy = 69;
                    gee = 160;
                    biv = 69;
                    break;
                case '3':
                    roy = 4;
                    gee = 117;
                    biv = 38;
                    break;
                case '4':
                    roy = 10;
                    gee = 66;
                    biv = 8;
                    break;
                default:
                    roy = 10;
                    gee = 66;
                    biv = 8;
                    break;
                }
                // Why so many appends? Because it seems to break after one +... so tired
                coordinateData.append(", " + std::to_string(roy)); // R
                coordinateData.append(", " + std::to_string(gee)); // G
                coordinateData.append(", " + std::to_string(biv)); // B
                coordinateData.append("],"); // Closing bracket
                k++;
                if (k == 256) { // Last loop
                    coordinateData.erase(coordinateData.find_last_of(","));
                    myfile << coordinateData << std::endl;
                    coordinateData = "\t]";
                    myfile << coordinateData << std::endl;
                    coordinateData = "}";
                }
                myfile << coordinateData << std::endl;
                coordinateData = ""; // Resets the coordinateData string so it can accept more data.
            }
        }


        std::string ipAdd = "192.168.0.1";
        //std::string ipAdd = "http://192.168.4.1/led";
        std::string JSONoutput = "curl -d @LEDData.json " + ipAdd;
        system(JSONoutput.c_str());

        clearScreen();
        if (result.length() > 0) {
            std::cout << "Success!\n" << std::endl;
        }

        myfile.close(); // Closes file

        return; // Returns to main menu
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

/* FOR REFERENCE
{
    "count":4,
        "clear" : false,
        "cords" : [
                    X  Y  R   G    B
                [0, 0, 0, 0, 0],        0
                [0, 0, 56, 221, 52],    1
                [0, 1, 0, 160, 0],      2
                [0, 2, 4, 117, 38],     3
                [0, 3, 0, 160, 0]       4
        ]
}
*/
