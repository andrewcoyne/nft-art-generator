#pragma once
#include <thread>
#include "Settings.hpp"

// Manages the creation and execution of program threads
class Manager {
    private:
        // Array of threads for image generation
        std::thread* threads;

        // Numbers of images to be created by each thread
        int* img_count;

        Settings settings;
    public:
        Manager (Settings& settings_ref);

        // Initialize the "threads" and "img_count" arrays, then begin execution
        void init_threads (uint numPerThread, uint extraImg);
};