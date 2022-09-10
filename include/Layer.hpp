#pragma once
#include <string>
#include <filesystem>
#include "Settings.hpp"
#include "opencv2/core.hpp"

// Representation of a layer within an NFT image
class Layer {
    private:
        std::shared_ptr<Settings> settings;

        std::string layer_type;

        // Unique, short identifier of this layer
        // layer type + "_" + (filename - extension)
        std::string id;

        bool isBlank;

        std::filesystem::path filepath;

        cv::Mat img;
    public:
        Layer ();

        Layer (std::string layer_type_, std::shared_ptr<Settings> settings_ptr);

        // Randomly select the specific layer based on settings and layer type
        void select_layer ();

        // Make this layer a blank layer
        void set_blank_layer ();

        // Returns the OpenCV representation (cv::Mat) of this layer
        // Prevents accidental modification of the cv::Mat object
        cv::Mat get_layer ();

        std::string get_id ();

        std::string get_layer_type ();

        // Returns true if this is a blank layer, false otherwise
        bool get_blank ();
};