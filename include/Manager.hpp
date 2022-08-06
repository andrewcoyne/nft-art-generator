#pragma once
#include <thread>
#include <vector>
#include "Settings.hpp"

// Manages the creation and execution of program threads
class Manager {
    private:
        // Array of threads for image generation
        std::vector<std::thread> threads;

        Settings settings;
    public:
        Manager (Settings& settings_ref);

        // Initialize the "threads" and "img_count" arrays, then begin execution
        void init_threads (uint32_t numPerThread, uint32_t extraImg);
};