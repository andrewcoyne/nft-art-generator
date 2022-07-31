#pragma once
#include <string>
#include <vector>
#include "boost/json/src.hpp"

class Settings {
    public:
        Settings (char* filepath);
        boost::json::value file_read (char* filepath);

        std::string name;
        std::string ind_name;
        int num;
        std::vector<std::string> layers;
        std::vector<int> rarity_levels;
        std::vector<std::string> check_above;
        std::vector<std::string[4]> exceptions;
};