#include <iostream>
#include <filesystem>
#include <thread>
#include "Manager.hpp"
#include "Settings.hpp"

int main (int argc, char* argv[]) {
    if (argc < 2 || std::filesystem::exists(argv[1])) {
        std::cerr << "Error: No settings file provided." << std::endl;
        return 1;
    }

    Settings* settings = new Settings(argv[1]);
    if (!settings) {
        std::cerr << "Error: Could not read provided settings file." << std::endl;
        return 1;
    }

    uint numPerThread = settings->get<int>("num") / std::thread::hardware_concurrency();
    uint extra = settings->get<int>("num") % std::thread::hardware_concurrency();
    
    Manager m (settings);
    m.init_threads(numPerThread, extra);
    m.run();

    return 0;
}