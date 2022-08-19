#include <thread>
#include "Manager.hpp"
#include "ImageBuilder.hpp"
#include "DuplicateChecker.hpp"
#include "Logger.hpp"
#include "Settings.hpp"

Manager::Manager (Settings& settings_ref) {
    settings = settings_ref;
}

void Manager::init_threads (uint32_t numPerThread, uint32_t extraImg) {
    uint32_t thread_count = std::thread::hardware_concurrency();
    int nft_count = 0;
    DuplicateChecker dc;
    Logger logger (std::ref(settings));
    
    // generate threads
    for (int i = 0; i < thread_count; ++i) {
        int img_to_gen = numPerThread;
        if (extraImg > 0) {
            ++img_to_gen;
            --extraImg;
        }

        ImageBuilder ib (img_to_gen, std::ref(nft_count), std::ref(dc), std::ref(settings), std::ref(logger));
        std::thread gen_thread (&ImageBuilder::generate, std::ref(ib));
        threads.push_back(gen_thread);
    }

    // wait for the threads to finish
    for (int i = 0; i < thread_count; ++i) {
        threads.at(i).join();
    }
}