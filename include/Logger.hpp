#pragma once
#include <string>
#include <memory>
#include <unordered_map>

// Handles logging NFT image information
class Logger {
    private:
        // Tracks the number of occurrences of each trait
        std::unordered_map<std::string, int> trait_count;

        // The number of NFTs that have been created
        int nft_count;
    public:
        Logger ();

        Logger (int& nft_count_ref);

        // Logs the NFT's traits in both a text file and the trait_count map; also prints success message to CLI
        void log_nft (std::string traits []);

        // Write trait_count map to text file
        void log_trait_count ();
};