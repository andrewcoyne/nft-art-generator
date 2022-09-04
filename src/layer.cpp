#include <random>
#include <filesystem>
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
    std::vector<std::string> files_in_dir;
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        files_in_dir.push_back(entry.path());
    }

    // If there aren't any files in the chosen folder, restart the layer selection process
    bool restarted = false;
    if (files_in_dir.size() == 0) { 
        select_layer(); 
        restarted = true;
    }

    if (!restarted) {
        std::uniform_int_distribution<int> dist_file_select (0, files_in_dir.size() - 1);
        std::string img_file = files_in_dir.at(dist_file_select(rng));

        // use layer_type, folder, and img_file to initialize filepath, id, and img
        filepath = /*folder + "/" +*/ img_file;
        id = layer_type + "_" + img_file.substr(/*0*/img_file.find_last_of('/'), img_file.find('.'));
        // read image file with cv::IMREAD_UNCHANGED to preserve alpha channel
        img = cv::imread(filepath, cv::IMREAD_UNCHANGED);
    }
}

cv::Mat& Layer::get_layer () { return img; }

std::string Layer::get_id () { return id; }