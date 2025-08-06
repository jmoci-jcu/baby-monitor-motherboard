
#include "log_storer.h"

void write_log(std::string logString)   {
    std::string filename = "output.txt";

    std::ofstream outfile(filename,std::ios::app);
    if (outfile.is_open()) {
        outfile << logString << std::endl;
        outfile.close();
    } else {
        log_error("LOG", "Failed to write file");
    }



}