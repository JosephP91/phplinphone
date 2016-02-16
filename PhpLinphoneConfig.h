#ifndef PHPLINPHONE_PHPLINPHONECONFIG_H
#define PHPLINPHONE_PHPLINPHONECONFIG_H

#include <phpcpp.h>

/**
 * This class rapresent a linphone configuration object.
 */
class PhpLinphoneConfig : public Php::Base {
private:
    /**
     * The path to the config file.
     */
    std::string filePath;
public:
    /**
     * Destructor. It does nothing.
     */
    PhpLinphoneConfig() {}

    /**
     * PHP constructor. It does nothing.
     */
    void __construct() {}

    /**
     * Virtual destructor.
     */
    virtual ~PhpLinphoneConfig() {}

    /**
     * PHP destructor. It does nothing.
     */
    void __destruct() {}

    /**
     * This method allow to specify the config path.
     * @param parameters an array with the accepted parameters.
     */
    void set(Php::Parameters &);

    /**
     * This method returns the config file path.
     * @return the file path.
     */
    const char *get() const;
};

#endif