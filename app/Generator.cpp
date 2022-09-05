#include <iostream>
#include <filesystem>
#include <thread>
#include "Manager.hpp"
#include "Settings.hpp"

int main (int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No settings file provided." << std::endl;
        return 1;
    }

    auto settings = std::make_shared<Settings>(argv[1]);
    try {
        settings->init();
    } catch (std::runtime_error& e) {
        std::cerr << "Error: Settings file could not be processed - " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Loaded settings file." << std::endl;

    // if there's a space in the filename, change it to an underscore
    // if it's a .DS_Store file, delete it
    for (std::filesystem::recursive_directory_iterator i("."), end; i != end; ++i) { 
        if (!is_directory(i->path())) {
            if (i->path().filename().string().find(" ") != std::string::npos) {
                std::string new_path = i->path().filename().string();
                std::replace(new_path.begin(), new_path.end(), ' ', '_');
                std::string old_path = i->path().string();
                new_path = old_path.substr(0, old_path.find_last_of('/')) + "/" + new_path;
                std::filesystem::rename(i->path(), new_path);
                std::cout << "Renamed " << new_path << std::endl;
            }
            if (i->path().filename().string().find(".DS_Store") != std::string::npos) {
                std::filesystem::remove(i->path());
                std::cout << "Deleted .DS_Store file" << std::endl;
            }
        }
    }

    // create finished_images folder if it doesn't already exist
    std::string finished_images = std::filesystem::current_path().string() + "/finished_images";
    if (!std::filesystem::exists(finished_images)) {
        std::filesystem::create_directory(finished_images);
        std::cout << "Created finished_images folder." << std::endl;
    }

    // create metadata folder if it doesn't already exist
    std::string metadata = std::filesystem::current_path().string() + "/metadata";
    if (!std::filesystem::exists(metadata)) {
        std::filesystem::create_directory(metadata);
        std::cout << "Created metadata folder." << std::endl;
    }

    // Evenly distribute the number of images to be generated by each thread
    uint32_t numPerThread = settings->get_num_nft() / std::thread::hardware_concurrency(); 
    // Get remainder, to be distributed evenly among available threads
    uint32_t extraImg = settings->get_num_nft() % std::thread::hardware_concurrency();
    
    std::cout << "Initializing image generator threads..." << std::endl;
    Manager m (settings);
    m.init_threads(numPerThread, extraImg);

    return 0;
}