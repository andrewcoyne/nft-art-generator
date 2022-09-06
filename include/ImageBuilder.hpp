#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "Settings.hpp"
#include "Layer.hpp"
#include "Logger.hpp"
#include "DuplicateChecker.hpp"

// Constructs the NFT images
class ImageBuilder {
    private:
        int nfts_to_gen;
        std::shared_ptr<int> nft_count;
        std::shared_ptr<DuplicateChecker> dc;
        std::shared_ptr<Settings> settings;
        std::shared_ptr<Logger> logger;

        // Remove layers if they can't be seen
        void remove_invis_layers (std::vector<Layer>& layers);

        // Take a Layer vector and return a corresponding vector of their ids
        std::vector<std::string> to_id_vec (std::vector<Layer>& layers);
    public:
        ImageBuilder ();

        ImageBuilder (int to_gen, std::shared_ptr<int> nft_count_ptr, std::shared_ptr<DuplicateChecker> dc_ptr, std::shared_ptr<Settings> settings_ptr, std::shared_ptr<Logger> logger_ptr);

        // Generate [nfts_to_gen] images
        void generate ();
};