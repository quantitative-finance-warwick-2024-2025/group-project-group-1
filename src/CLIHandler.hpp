#pragma once
#include <string>
#include <vector>

class CLIHandler {
    public:
        // Constructor
        CLIHandler();

        // Print Help
        void displayHelp();

        // Parse Command Line Arguments
        std::vector<std::string> parseArguments(std::string &command);

        // Run CLIHandler
        void run();
};