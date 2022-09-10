#include <random>
#include <iostream>
#include "Layer.hpp"
#include "opencv2/imgcodecs.hpp"

Layer::Layer () {}

Layer::Layer (std::string layer_type_, std::shared_ptr<Settings> settings_ptr) {
    layer_type = layer_type_;
    settings = settings_ptr;
}

void Layer::select_layer () {
    // generate a random number for selecting this layer's rarity
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng (seed);
    std::uniform_int_distribution<int> dist (1, settings->get_num_nft());
    int rarity = dist(rng);

    // get rarity level folder name
    std::string rarity_folder_name;
    std::vector<double> rarity_pct = settings->get_rarity_level_pct();
    double rarity_compare = 0;
    for (int i = 0; i < rarity_pct.size(); ++i) {
        rarity_compare += rarity_pct.at(i) * settings->get_num_nft();
        if (rarity <= rarity_compare) {
            rarity_folder_name = settings->get_rarity_level_names().at(i);
            break;
        }
    }

    // randomly select the actual layer file
    // for further optimization, add reservoir sampling for very large folders?
    std::string folder = std::filesystem::current_path().string() + "/" + layer_type + "/" + rarity_folder_name;
    std::vector<std::filesystem::path> files_in_dir;

    // check if the folder exists - if not, make this a blank layer
    if (!std::filesystem::exists(folder)) {
        set_blank_layer();
        return;
    }

    // Put all the paths of the files in the folder into files_in_dir
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        files_in_dir.push_back(entry.path());
    }
    
    // If there are files in the chosen folder, pick one and make that the layer
    // If not, make this a blank layer
    if (files_in_dir.size() != 0) {
        std::uniform_int_distribution<int> dist_file_select (0, files_in_dir.size() - 1);
    
        // use layer_type, folder, and img_file to initialize filepath, id, and img
        filepath = files_in_dir.at(dist_file_select(rng));

        // If there's an image file BLANK.png, just make this a blank layer
        if (filepath.stem().string() == "BLANK") {
            set_blank_layer();
            return;
        }

        id = layer_type + "_" + filepath.stem().string();

        // read image file with cv::IMREAD_UNCHANGED to preserve alpha channel
        img = cv::imread(filepath, cv::IMREAD_UNCHANGED);
    } else {
        set_blank_layer();
    }
    
}

void Layer::set_blank_layer () {
    id = layer_type + "_" + "BLANK";
    isBlank = true;
}

cv::Mat Layer::get_layer () { return img; }

std::string Layer::get_id () { return id; }

std::string Layer::get_layer_type () { return layer_type; }

bool Layer::get_blank () { return isBlank; }