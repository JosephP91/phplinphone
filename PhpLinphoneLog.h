#ifndef PHPLINPHONE_PHPLINPHONELOG_H
#define PHPLINPHONE_PHPLINPHONELOG_H

#include <phpcpp.h>

/**
 * This object rapresent a linphone log object.
 */
class PhpLinphoneLog : public Php::Base {
private:
    /**
     * A pointer toe log file.
     */
    FILE *filePath;
public:
    /**
     * The constructor will give a default value to the log path.
     */
    PhpLinphoneLog() : filePath(nullptr) {}

    /**
     * This method is directly invoked by the PHP interpreter to build an istance of this class.
     */
    void __construct() {}

    /**
     * Virtual destructor.
     */
    virtual ~PhpLinphoneLog() {}

    /**
     * This method is directly invoked by the PHP intepreter to destroy an istance of this class.
     */
    void __destruct();

    /**
     * This method allow to set the config file path.
     * @param parameters an array with the accepted parameters.
     */
    void set(Php::Parameters &);

    /**
     * This methos is not visible from PHP but only from C++ and it is used to return the log file path.
     */
    FILE *get() const;
};

#endif