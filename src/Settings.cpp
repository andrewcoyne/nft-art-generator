#include <string>
#include <iostream>
#include <fstream>
#include "Settings.hpp"
#include "nlohmann/json.hpp"

// Class that provides easy access to the settings in the JSON file
Settings::Settings (char* filepath) {
    fpath = (std::string) filepath;
}

void Settings::init () {
    std::ifstream settings_file (fpath);
    nlohmann::json settings = nlohmann::json::parse(settings_file);

    // Load variables from JSON file
    name = settings["name"].get<std::string>();
    ind_name = settings["ind_name"].get<std::string>();
    num_nft = settings["num"].get<int>();
    layer_folder_names = settings["layers"].get<std::vector<std::string>>();
    rarity_level_names = settings["rarity_level_names"].get<std::vector<std::string>>();
    rarity_level_pct = settings["rarity_levels"].get<std::vector<double>>();
    check_above = settings["check_above"].get<std::vector<std::string>>();
    layer_exceptions = settings["exceptions"].get<std::vector<std::vector<std::string>>>();
}

std::string Settings::get_name () { return name; }

std::string Settings::get_ind_name () { return ind_name; }

int Settings::get_num_nft () { return num_nft; }

std::vector<std::string> Settings::get_layer_folder_names () { return layer_folder_names; }

std::vector<std::string> Settings::get_rarity_level_names () { return rarity_level_names; }

std::vector<double> Settings::get_rarity_level_pct () { return rarity_level_pct; }

std::vector<std::string> Settings::get_check_above () { return check_above; }

std::vector<std::vector<std::string>> Settings::get_layer_exceptions () { return layer_exceptions; }