#include <iostream>

#include "PhpLinphoneFactoryConfig.h"

// Implementation of the set method.
void PhpLinphoneFactoryConfig::set(Php::Parameters &parameters) {
    std::string filePath = parameters[0];
    if (filePath.empty()) {
        Php::error<<"The factory config file specified does not exists."<<std::endl;
    }
    (this->filePath).assign(filePath);
}

// Implemenation of the get method.
const char *PhpLinphoneFactoryConfig::get() const {
    return (this->filePath).c_str();
}