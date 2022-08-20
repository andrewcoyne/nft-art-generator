#include "ImageBuilder.hpp"
#include "opencv2/imgcodecs.hpp"

ImageBuilder::ImageBuilder () {}

ImageBuilder::ImageBuilder (int to_gen, int& nft_count_ref, DuplicateChecker& dc_ref, Settings& settings_ref, Logger& logger_ref) {
    nfts_to_gen = to_gen;
    nft_count = nft_count_ref;
    dc = dc_ref;
    settings = settings_ref;
    logger = logger_ref;
}

std::vector<std::string> ImageBuilder::to_id_vec (std::vector<Layer>& layers) {
    std::vector<std::string> res;
    for (Layer& i : layers) { res.push_back(i.get_id()); }
    return res;
}

void ImageBuilder::remove_invis_layers (std::vector<Layer>& layers) {
    cv::Mat above;
    std::vector<std::string> layers_to_check = settings.get_check_above();

    for (auto it = layers.rbegin(); it != layers.rend(); ) {
        // obtain this layer's alpha channel for comparison
        std::vector<cv::Mat> channels;
        cv::split(it->get_layer(), channels);
        cv::Mat alpha = channels[3];

        // if there are no layers above this layer, make above = the layer's alpha channel
        if (above.empty()) {
            above = alpha;
            ++it;
            continue;
        }

        // if this layer's type req's it to be checked, check the layer
        std::string layer_id = it->get_id();
        std::string layer_type = layer_id.substr(0, layer_id.find("_"));
        if (std::any_of(layers_to_check.begin(), layers_to_check.end(), [layer_type](std::string s){ return s == layer_type; })) {
            // if a pixel in the current layer is visible, and no pixels are visible above it
            cv::Mat inv_above;
            cv::Mat result;
            cv::bitwise_not(above, inv_above);
            cv::bitwise_and(alpha, inv_above, result);

            // if less than 1% of the layer is visible beneath the layers above it, remove it
            if (cv::countNonZero(result) / (double) cv::countNonZero(alpha) < 0.01) {
                it = decltype(it)(layers.erase((++it).base()));
                continue;
            }
        }

        // "add" the current layer to the layers above, for subsequent evals
        cv::bitwise_or(above, alpha, above);
        ++it;
    }
}

void ImageBuilder::generate () {
    for (int i = 0; i < nfts_to_gen; ) {
        std::vector<Layer> layers;
        cv::Mat img;

        // Generate layers and add them to layer vector
        for (std::string& layer_type : settings.get_layer_folder_names()) {
            Layer l (layer_type, settings);
            l.select_layer();
            layers.push_back(l);
        }

        remove_invis_layers(layers);

        // Construct image
        // This is done before checking for uniqueness, because not doing so allows for the
        // increased possibility that an identical image is generated and logged in the meantime
        img = layers[0].get_layer();
        for (int j = 1; j < layers.size(); ++j) {
            cv::Mat layer_to_add = layers[j].get_layer();
            cv::Rect roi (cv::Point (0, 0), layer_to_add.size());
            layer_to_add.copyTo(img (roi));
        }

        // If the generated image is unique, save and log it
        std::vector<std::string> traits = to_id_vec(layers);
        if (!dc.search(traits)) {
            std::string filepath = std::filesystem::current_path().string() + "/finished_images/" + settings.get_ind_name() + std::to_string(nft_count) + ".png";
            cv::imwrite(filepath, img);
            logger.log_nft(traits, nft_count);
            ++i;
        }
    }
}