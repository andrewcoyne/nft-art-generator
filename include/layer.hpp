#pragma once
#include <string>
#include "Settings.hpp"
#include "opencv2/core.hpp"

// Representation of a layer within an NFT image
class Layer {
    private:
        Settings settings;

        std::string layer_type;

        // Unique, short identifier of this layer
        // layer type + "_" + (filename - extension)
        std::string id;

        std::string filepath;

        cv::Mat img;
    public:
        Layer ();

        Layer (std::string layer_type_, Settings& settings_ref);

        // Randomly select the specific layer based on settings and layer type
        void select_layer ();

        // Returns the OpenCV representation (cv::Mat) of this layer
        // Prevents accidental modification of the cv::Mat object
        cv::Mat get_layer ();

        std::string get_id ();
};