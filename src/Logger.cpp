#include <iostream>
#include <fstream>
#include <ctime>
#include <mutex>
#include "Logger.hpp"
#include "Settings.hpp"

Logger::Logger () {}

Logger::Logger (std::shared_ptr<Settings> settings_ptr) {
    settings = settings_ptr;

    std::time_t time = std::time(nullptr);
    traits_fname = settings->get_name() + "_Traits_" + std::ctime(&time) + ".txt";
}

// Prevent concurrent modification of trait_count or traits_file
std::mutex log_nft_mutex;

void Logger::log_nft (std::vector<std::string>& traits, int nft_count) {
    log_nft_mutex.lock();

    std::ofstream traits_file (traits_fname, std::ios::app | std::ios::ate | std::ios::out);

    std::string init_out = settings->get_ind_name() + " #" + std::to_string(nft_count) + ": ";

    // write to CLI
    std::cout << init_out << std::endl;

    // write to file
    traits_file << init_out << std::endl;

    for (auto i: traits) {
        // update trait_count
        if (trait_count.find(i) != trait_count.end()) {
            trait_count[i] += 1;
        } else {
            trait_count[i] = 1;
        }

        // write trait to CLI
        //size_t pos = i.find("_");
        std::cout << /*i.substr(0, pos) << ": " << i.substr(pos + 2, i.size())*/ i << std::endl;

        // write trait to file
        traits_file << i << std::endl;
    }

    std::cout << std::endl;
    traits_file << std::endl;

    log_nft_mutex.unlock();
}

void Logger::log_trait_count () {
    std::time_t time = std::time(nullptr);
    std::ofstream trait_count_file (settings->get_name() + "_Trait_Count_" + std::ctime(&time) + ".txt", std::ios::app | std::ios::ate | std::ios::out);

    for (auto i: trait_count) {
        trait_count_file << i.first + ": " + std::to_string(i.second) << std::endl;
    }

    trait_count_file.close();
}