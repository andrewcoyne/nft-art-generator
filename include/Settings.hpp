#pragma once
#include <string>
#include <vector>

// Data class that enables fast access to the settings in the provided JSON file
class Settings {
    private:
        std::string fpath;

        // name of NFT collection
        std::string name;

        // name of individual NFTs in collection, ex. "WarDawg"
        std::string ind_name;

        // number of NFTs to generate
        int num_nft;

        // names of the folders for each layer
        std::vector<std::string> layer_folder_names;

        // the names of each rarity level, equal to the names of the subfolders for each layer
        std::vector<std::string> rarity_level_names;

        // the rarity of each level, expressed as a percentage; ordered wrt RARITY_LEVEL_NAMES
        std::vector<double> rarity_level_pct;

        // make sure that all layers above each layer in this list don't obscure that layer
        std::vector<std::string> check_above;

        // special ordering cases
        std::vector<std::vector<std::string> > layer_exceptions;
    public:
        Settings ();
        
        Settings (char* filepath);

        // Access JSON file, initialize settings variables
        void init ();

        // Getters

        // Get name of NFT collection
        std::string get_name ();

        // Get name of individual NFTs in collection, ex. "WarDawg"
        std::string get_ind_name ();

        // Get number of NFTs to generate
        int get_num_nft ();

        // Get names of the folders for each layer
        std::vector<std::string> get_layer_folder_names ();

        // Get the names of each rarity level, equal to the names of the subfolders for each layer
        std::vector<std::string> get_rarity_level_names ();

        // Get the rarity of each level, expressed as a percentage; ordered wrt rarity_level_names
        std::vector<double> get_rarity_level_pct ();

        // Get all layers to be checked for visibility in final image
        std::vector<std::string> get_check_above ();

        // Get special ordering cases
        std::vector<std::vector<std::string> > get_layer_exceptions ();
};