#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Settings.hpp"
#include "Layer.hpp"
#include "DuplicateChecker.hpp"

// Constructs the NFT images
class ImageBuilder {
    private:
        int nfts_to_gen;
        std::shared_ptr<int> nft_count;
        std::shared_ptr<DuplicateChecker> dc;
        Settings settings;

        // Remove layers if they can't be seen
        void prune_invis_layers (std::vector<Layer>& layers);

        // Take a Layer vector and return a corresponding array of their ids
        std::string* to_id_arr (std::vector<Layer>& layers);
    public:
        ImageBuilder (int to_gen, std::shared_ptr<int> nft_count_ptr, std::shared_ptr<DuplicateChecker> dc_ptr, Settings& settings_ref);

        void generate ();
};