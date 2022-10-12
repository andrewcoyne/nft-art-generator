#include <string>
#include <iostream>
#include <fstream>
#include "Settings.hpp"
#include <nlohmann/json.hpp>

// Class that provides easy access to the settings in the JSON file
Settings::Settings () {}

Settings::Settings (char* filepath) {
    fpath = (std::string) filepath;
}

void Settings::init () {
    std::ifstream settings_file (fpath);
    nlohmann::json settings = nlohmann::json::parse(settings_file);

    // Load variables from JSON file
    name = settings["name"].get<std::string>();
    if (name.length() == 0) { throw std::runtime_error("Name was not provided."); }

    ind_name = settings["ind_name"].get<std::string>();
    if (ind_name.length() == 0) { throw std::runtime_error("Individual name was not provided."); }
    
    num_nft == -1;
    num_nft = settings["num"].get<int>();
    if (num_nft == -1) { throw std::runtime_error("Number of NFTs to generate was not provided."); }
    
    layer_folder_names = settings["layers"].get<std::vector<std::string>>();
    if (layer_folder_names.size() == 0) { throw std::runtime_error("Layer folder names (\"layers\") were not provided."); }
    
    rarity_level_names = settings["rarity_level_names"].get<std::vector<std::string>>();
    if (rarity_level_names.size() == 0) { throw std::runtime_error("Rarity level names were not provided."); }
    
    rarity_level_pct = settings["rarity_levels"].get<std::vector<double>>();
    if (rarity_level_pct.size() == 0) { throw std::runtime_error("Rarity level percentages (\"rarity_levels\") were not provided."); }
    
    check_above = settings["check_above"].get<std::vector<std::string>>();
    if (check_above.size() == 0) { throw std::runtime_error("Layers to check (\"check_above\") were not provided."); }
    
    layer_exceptions = settings["exceptions"].get<std::vector<std::vector<std::string>>>();
    if (layer_exceptions.size() == 0) { throw std::runtime_error("Layer exceptions (primary array) were not provided."); }
    if (layer_exceptions.at(0).size() == 0) { throw std::runtime_error("Layer exceptions (secondary array) were not provided."); }
}

std::string Settings::get_name () { return name; }

std::string Settings::get_ind_name () { return ind_name; }

int Settings::get_num_nft () { return num_nft; }

std::vector<std::string> Settings::get_layer_folder_names () { return layer_folder_names; }

std::vector<std::string> Settings::get_rarity_level_names () { return rarity_level_names; }

std::vector<double> Settings::get_rarity_level_pct () { return rarity_level_pct; }

std::vector<std::string> Settings::get_check_above () { return check_above; }

std::vector<std::vector<std::string>> Settings::get_layer_exceptions () { return layer_exceptions; }