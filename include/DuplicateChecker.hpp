#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Checks if a generated NFT has already been made
class DuplicateChecker {
    private:
        // Hash map for storing each NFT's details
        std::unordered_map<std::string, int> trait_map;

        // Insert provided image layer combination into hash map
        void insert (std::string& traits);

    public:

        DuplicateChecker ();

        /* Search for duplicate
           Returns true if provided image layer combination has been created already
           Returns false if not
        */
        bool search (std::vector<std::string>& traits);
};