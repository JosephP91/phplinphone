#include "PhpLinphoneLog.h"

#include <iostream>
#include <linphone/linphonecore.h>

// Implementation of the PHP destructor.
void PhpLinphoneLog::__destruct() {
    if (this->filePath != nullptr) {
        fclose(this->filePath);
        this->filePath = nullptr;
    }
}

// Implementation of the set method.
void PhpLinphoneLog::set(Php::Parameters &parameters) {
    this->filePath = fopen(parameters[0],"wr");
    if (this->filePath == nullptr) {
        Php::warning<<"Can not open log file specified. Will use stdout."<<std::endl;
    } else {
        linphone_core_set_log_file(this->filePath);
    }
}

// Implementation of the get method.
FILE *PhpLinphoneLog::get() const {
    return this->filePath;
}
