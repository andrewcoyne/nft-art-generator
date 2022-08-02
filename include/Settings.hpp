#pragma once
#include <string>
#include <vector>

// Data class that enables fast access to the settings in the provided JSON file
class Settings {
    public:
        Settings (char* filepath);

        // name of NFT collection
        const std::string NAME;

        // name of individual NFTs in collection, ex. "WarDawg"
        const std::string IND_NAME;

        // number of NFTs to generate
        const int NUM_NFT;

        // names of the folders for each layer
        const std::vector<std::string> LAYER_FOLDER_NAMES;

        // the names of each rarity level, equal to the names of the subfolders for each layer
        const std::vector<std::string> RARITY_LEVEL_NAMES;

        // the rarity of each level, expressed as a percentage; ordered wrt RARITY_LEVEL_NAMES
        const std::vector<double> RARITY_LEVEL_PCT;

        // make sure that all layers above each layer in this list don't obscure that layer
        const std::vector<std::string> CHECK_ABOVE;

        // special ordering cases
        const std::vector<std::string[4]> LAYER_EXCEPTIONS;
};