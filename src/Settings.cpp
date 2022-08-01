#include <string>
#include <iostream>
#include <fstream>
#include "Settings.hpp"
#include "nlohmann/json.hpp"

// Class that provides easy access to the settings in the JSON file
Settings::Settings (char* filepath) {
    std::ifstream settings_file (filepath);
    nlohmann::json settings = nlohmann::json::parse(settings_file);

    for (auto& [k, v] : settings.items()) {
        switch (k) {
            case "name": NAME = v; break;

            case "ind_name": IND_NAME = v; break;

            case "num": NUM_NFT = v; break;

            case "layers": LAYER_FOLDER_NAMES = v; break;

            case "rarity_level_names": RARITY_LEVEL_NAMES = v; break;

            case "rarity_levels": RARITY_LEVEL_PCT = v; break;

            case "check_above": CHECK_ABOVE = v; break;

            case "exceptions": LAYER_EXCEPTIONS = v; break;
        }
    }
}