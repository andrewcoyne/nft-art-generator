#pragma once
#include <thread>
#include <vector>
#include <memory>
#include "Settings.hpp"

// Manages the creation and execution of program threads
class Manager {
    private:
        // Array of threads for image generation
        std::vector<std::thread> threads;

        std::shared_ptr<Settings> settings;
    public:
        Manager (std::shared_ptr<Settings> settings_ptr);

        // Initialize the "threads" and "img_count" arrays, then begin execution
        void init_threads (uint32_t numPerThread, uint32_t extraImg);
};