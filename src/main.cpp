

#include <iostream>
#include <fstream>
#include <string>
#include "logger.h"  // Custom logging system
#include "log_storer/log_storer.h"


int main() {
    log_enabled(TRUE);
    log_set_level(LOG_INFO);

    write_log(std::string("test"));

}