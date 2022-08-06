#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Settings.hpp"
#include "Layer.hpp"
#include "Logger.hpp"
#include "DuplicateChecker.hpp"

// Constructs the NFT images
class ImageBuilder {
    private:
        int nfts_to_gen;
        int nft_count;
        DuplicateChecker dc;
        Settings settings;
        Logger logger;

        // Remove layers if they can't be seen
        void prune_invis_layers (std::vector<Layer>& layers);

        // Take a Layer vector and return a corresponding array of their ids
        std::string* to_id_arr (std::vector<Layer>& layers);
    public:
        ImageBuilder ();

        ImageBuilder (int to_gen, int& nft_count_ref, DuplicateChecker& dc_ref, Settings& settings_ref, Logger& logger_ref);

        void generate ();
};