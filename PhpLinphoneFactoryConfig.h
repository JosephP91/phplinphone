#ifndef PHPLINPHONE_PHPLINPHONEFACTORYCONFIG_H
#define PHPLINPHONE_PHPLINPHONEFACTORYCONFIG_H

#include <phpcpp.h>

/**
 * This class rapresent a linphone factory configuration object.
 */
class PhpLinphoneFactoryConfig : public Php::Base {
private:
    /**
     * The path to the factory config file.
     */
    std::string filePath;
public:
    /**
     * Destructor. It does nothing.
     */
    PhpLinphoneFactoryConfig() {}

    /**
     * PHP constructor. It does nothing.
     */
    void __construct() {}

    /**
     * Virtual destructor.
     */
    virtual ~PhpLinphoneFactoryConfig() {}

    /**
     * PHP destructor. It does nothing.
     */
    void __destruct() {}

    /**
     * This method allow to specify the factory config path.
     * @param parameters an array with the accepted parameters.
     */
    void set(Php::Parameters &);

    /**
     * This method returns the factory config file path.
     * @return the file path.
     */
    const char *get() const;
};

#endif