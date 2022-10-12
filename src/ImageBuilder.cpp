#include <iostream>
#include <algorithm>
#include "ImageBuilder.hpp"
#include "opencv2/imgcodecs.hpp"

ImageBuilder::ImageBuilder () {}

ImageBuilder::ImageBuilder (int to_gen, std::shared_ptr<int> nft_count_ptr, std::shared_ptr<DuplicateChecker> dc_ptr, std::shared_ptr<Settings> settings_ptr, std::shared_ptr<Logger> logger_ptr) {
    nfts_to_gen = to_gen;
    nft_count = nft_count_ptr;
    dc = dc_ptr;
    settings = settings_ptr;
    logger = logger_ptr;
}

std::vector<std::string> ImageBuilder::to_id_vec (std::vector<Layer>& layers) {
    std::vector<std::string> res;
    for (Layer& i : layers) { res.push_back(i.get_id()); }
    return res;
}

void ImageBuilder::remove_invis_layers (std::vector<Layer>& layers) {
    cv::Mat above;
    std::vector<std::string> layers_to_check = settings->get_check_above();

    for (auto it = layers.rbegin(); it != layers.rend(); ) {
        // obtain this layer's alpha channel for comparison
        std::vector<cv::Mat> channels;
        cv::split(it->get_layer().clone(), channels);
        cv::Mat alpha;

        if (channels.size() >= 4){
            alpha = channels[3];
        } else {
            std::cerr << "WARNING: Layer " + it->get_id() + " does not have an alpha channel" << std::endl;
            ++it;
            continue;
        }

        // if there are no layers above this layer, make above = the layer's alpha channel
        if (above.empty()) {
            above = alpha;
            ++it;
            continue;
        }

        // if this layer's type req's it to be checked, check the layer
        std::string layer_id = it->get_id();
        std::string layer_type = it->get_layer_type();
        if (std::any_of(layers_to_check.begin(), layers_to_check.end(), [layer_type](std::string s){ return s == layer_type; })) {
            // if a pixel in the current layer is visible, and no pixels are visible above it
            cv::Mat inv_above;
            cv::Mat result;
            cv::bitwise_not(above, inv_above);
            cv::bitwise_and(alpha, inv_above, result);

            // if less than 1% of the layer is visible beneath the layers above it, remove it
            if (cv::countNonZero(result) / (double) cv::countNonZero(alpha) < 0.01) {
                //std::cout << "remove layer" << std::endl;
                it = decltype(it)(layers.erase(std::next(it).base()));
                continue;
            }
        }

        // "add" the current layer to the layers above, for subsequent evals
        cv::bitwise_or(above, alpha, above);
        ++it;
    }
}

void ImageBuilder::move_layer (std::vector<Layer>& layers, size_t curr_index, size_t new_index) {
    new_index = std::min(new_index, layers.size() - 1);

    if (curr_index > new_index) {
        std::rotate(layers.rend() - curr_index - 1, layers.rend() - curr_index, layers.rend() - new_index);
    } else {      
        std::rotate(layers.begin() + curr_index, layers.begin() + curr_index + 1, layers.begin() + new_index + 1);
    }
}

void ImageBuilder::handle_layer_exc (std::vector<Layer>& layers) {
    
    std::vector<std::vector<std::string>> exc = settings->get_layer_exceptions();

    for (int i = 0; i < layers.size(); ++i) {
        for (int j = 0; j < exc.size(); ++j) {
            if (layers.at(i).get_id() == exc.at(j).at(0) + "_" + exc.at(j).at(1)) {
                if (exc.at(j).at(2) == "MoveBehind") {
                    // Find the layer specified by the exception, and move this layer behind it
                    for (int k = 0; k < layers.size(); ++k) {
                        if (layers.at(k).get_layer_type() == exc.at(j).at(3)) {
                            move_layer(layers, i, k);
                            break;
                        }
                    }
                } 
                else if (exc.at(j).at(2) == "MoveInFront") {
                    // Find the layer specified by the exception, and move this layer in front of it
                    for (int k = 0; k < layers.size(); ++k) {
                        if (layers.at(k).get_layer_type() == exc.at(j).at(3)) {
                            move_layer(layers, i, k + 1);
                            break;
                        }
                    }
                } 
                else if (exc.at(j).at(2) == "DeleteIf") {
                    // If the layer specified by the exception is in layers, delete this layer
                    for (int k = 0; k < layers.size(); ++k) {
                        if (layers.at(k).get_id() == exc.at(j).at(3) + "_" + exc.at(j).at(4)) {
                            layers.erase(layers.begin() + i);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void ImageBuilder::generate () {
    for (int i = 0; i < nfts_to_gen; ) {
        std::vector<Layer> layers;
        cv::Mat img;

        // Generate layers and add them to layer vector
        for (std::string& layer_type : settings->get_layer_folder_names()) {
            Layer l (layer_type, settings);
            l.select_layer();
            if (!l.get_blank()) {
                layers.push_back(std::ref(l));
            }
        }

        remove_invis_layers(layers);
        
        handle_layer_exc(layers);
        
        // Construct image
        // This is done before checking for uniqueness, because not doing so allows for the
        // increased possibility that an identical image is generated and logged in the meantime
        img = layers.at(0).get_layer();
        for (int j = 1; j < layers.size(); ++j) {
            cv::Mat layer_to_add = layers.at(j).get_layer();

            // Add layer_to_add on top of img
            uint8_t* img_pixel = (uint8_t*) img.data;
            uint8_t* layer_pixel = (uint8_t*) layer_to_add.data;
            int channels = img.channels();
            for (int k = 0; k < img.rows; ++k) {
                for (int l = 0; l < img.cols; ++l) {
                    int index = k * img.cols * channels + l * channels;
                    double alpha = layer_pixel[index + 3] / 255.0; // convert to value from 0.0-1.0
                    img_pixel[index] = img_pixel[index] * (1 - alpha) + layer_pixel[index] * alpha; // Modify B channel value
                    img_pixel[index + 1] = img_pixel[index + 1] * (1 - alpha) + layer_pixel[index + 1] * alpha; // Modify G channel value
                    img_pixel[index + 2] = img_pixel[index + 2] * (1 - alpha) + layer_pixel[index + 2] * alpha; // Modify R channel value
                }
            }
        }

        // If the generated image is unique, save and log it
        std::vector<std::string> traits = to_id_vec(layers);
        if (!dc->search(traits)) {
            (*nft_count)++;
            int curr_nft_count = *nft_count;
            logger->log_nft(traits, curr_nft_count);
            std::string filepath = std::filesystem::current_path().string() + "/finished_images/" + settings->get_ind_name() + std::to_string(curr_nft_count) + ".png";
            cv::imwrite(filepath, img);
            ++i;
        }
    }
}