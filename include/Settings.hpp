#pragma once
#include <string>
#include "boost/property_tree/ptree.hpp"

class Settings {
    private:
        struct nft_settings;
    public:
        Settings (char* filepath);

        template <typename T>
        T get (std::string setting_path);
};