#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "Settings.hpp"

// Handles logging NFT image information
class Logger {
    private:
        // Tracks the number of occurrences of each trait
        std::unordered_map<std::string, int> trait_count;

        Settings settings;

        // The name of the file containing the traits of all NFTs in collection
        std::ofstream traits_file;
    public:
        Logger ();

        Logger (Settings& settings_ref);

        // Logs the NFT's traits in both a text file and the trait_count map; also prints success message to CLI
        void log_nft (std::vector<std::string>& traits, int nft_count);

        // Write trait_count map to text file
        void log_trait_count ();
};