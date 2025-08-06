


#include "log_storer.h"

void write_log(std::string logString)   {
    log_info("LOG", "Program started");

    std::string filename = "output.txt";

    std::cout << "Saving message to file: " << filename << std::endl;

    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << logString << std::endl;
        outfile.close();
        std::cout << "Message successfully written." << std::endl;
        log_info("LOG", "File written successfully");
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
        log_error("LOG", "Failed to write file");
    }



}