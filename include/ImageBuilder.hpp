#pragma once
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
        void remove_invis_layers (std::vector<Layer>& layers);

        // Take a Layer vector and return a corresponding vector of their ids
        std::vector<std::string> to_id_vec (std::vector<Layer>& layers);
    public:
        ImageBuilder ();

        ImageBuilder (int to_gen, int& nft_count_ref, DuplicateChecker& dc_ref, Settings& settings_ref, Logger& logger_ref);

        // Generate [nfts_to_gen] images
        void generate ();
};