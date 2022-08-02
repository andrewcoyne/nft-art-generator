#pragma once
#include <string>
#include <memory>
#include "Settings.hpp"
#include "opencv2/core.hpp"

// Representation of a layer within an NFT image
class Layer {
    private:
        std::string filepath;

        // Unique, short identifier of this layer
        // layer type + "_" + (filename - extension)
        std::string id;

        cv::Mat img;

        std::shared_ptr<Settings> settings;
    public:
        Layer (std::string layerType, std::shared_ptr<Settings> settings_ptr);

        // Check if this layer, given the layers above it, will be visible
        bool check_above (cv::Mat above_imgs []);

        // Returns the OpenCV representation (cv::Mat) of this layer
        // Prevents accidental modification of the cv::Mat object
        cv::Mat get_layer ();
};