#pragma once
#include <string>

class Settings {
    private:
        struct nft_settings;
    public:
        Settings (char* filepath);

        template <typename T>
        T get (std::string setting_path);
};