#include <string>
#include <iostream>
#include <fstream>
#include "Settings.hpp"
#include "boost/json/src.hpp"

// Class that provides easy access to the settings in the JSON file
Settings::Settings (char* filepath) {
    const boost::json::value json = file_read(filepath);
}

// Reads the provided JSON file into the JSON parser
boost::json::value Settings::file_read (char* filepath) {
    std::ifstream settings_file ((std::string) filepath, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    boost::json::stream_parser p;
    boost::json::error_code ec;

    settings_file.seekg(0, std::ios::beg);

    if (settings_file.is_open()) {
        do {
            const int BUFFER_SIZE = 4096;
            char buf [BUFFER_SIZE];
            settings_file.read(buf, BUFFER_SIZE);
            p.write(buf, BUFFER_SIZE, ec);
        } while (!settings_file.eof());
    }

    settings_file.close();

    if (ec) {
        return nullptr;
    }

    p.finish(ec);

    if (ec) {
        return nullptr;
    }

    return p.release();
}