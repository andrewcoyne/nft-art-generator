#pragma once
#include <memory>
#include <thread>
#include "Settings.hpp"

class Manager {
    private:
        // Array of threads for image generation
        std::thread* threads;

        // Numbers of images to be created by each thread
        int* img_count;
    public:
        Manager (std::shared_ptr<Settings> settings);

        // Initialize the "threads" and "img_count" arrays, then begin execution
        void init_threads (uint numPerThread, uint extraImg);
};