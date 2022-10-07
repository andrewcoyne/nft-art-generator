#include <thread>
#include "Manager.hpp"
#include "ImageBuilder.hpp"
#include "DuplicateChecker.hpp"
#include "Logger.hpp"
#include "Settings.hpp"

Manager::Manager (std::shared_ptr<Settings> settings_ptr) {
    settings = settings_ptr;
}

void Manager::init_threads (uint32_t numPerThread, uint32_t extraImg) {
    uint32_t thread_count = std::thread::hardware_concurrency();
    auto nft_count = std::make_shared<int>(0);
    auto dc = std::make_shared<DuplicateChecker>();
    auto logger = std::make_shared<Logger>(settings);
    
    // generate threads
    for (int i = 0; i < thread_count; ++i) {
        int img_to_gen = numPerThread;

        if (extraImg > 0) {
            ++img_to_gen;
            --extraImg;
        }
        
        ImageBuilder *ib  = new ImageBuilder(img_to_gen, nft_count, dc, settings, logger);
        std::thread gen_thread (&ImageBuilder::generate, ib);
        threads.push_back(std::move(gen_thread));
    }

    // wait for the threads to finish
    for (std::thread& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}