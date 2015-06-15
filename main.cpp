#include "PhpLinphoneCore.h"
#include "PhpLinphoneAuth.h"
#include "PhpLinphoneProxy.h"
#include "PhpLinphoneCall.h"
#include "PhpLinphoneLog.h"
#include "PhpLinphoneFactoryConfig.h"
#include "PhpLinphoneConfig.h"

/**
 * Switch to C context so that the get_module() function can be
 * called by C programs (which the Zend engine is)
 */
extern "C" {
    // Startup function for the extension
    PHPCPP_EXPORT void *get_module() {
        // Create static instance of the extension object
        static Php::Extension extension("phplinphone", "1.0");


        // Add auth class.
        Php::Class<PhpLinphoneAuth> auth("PhpLinphoneAuth");
        auth.method("__construct",&PhpLinphoneAuth::__construct,{
                Php::ByRef("core","PhpLinphoneCore",false,true)
        });
        auth.method("setUsername",&PhpLinphoneAuth::setUsername,{
                Php::ByVal("username",Php::Type::String)
        });
        auth.method("setPassword",&PhpLinphoneAuth::setPassword,{
                Php::ByVal("password",Php::Type::String)
        });
        auth.method("build",&PhpLinphoneAuth::build);
        auth.method("__destruct",&PhpLinphoneAuth::__destruct);
        extension.add(std::move(auth));


        // Add proxy class.
        Php::Class<PhpLinphoneProxy> proxy("PhpLinphoneProxy");
        proxy.method("__construct",&PhpLinphoneProxy::__construct,{
                Php::ByRef("core","PhpLinphoneCore",false,true),
                Php::ByRef("auth","PhpLinphoneAuth",false,true)
        });
        proxy.method("disable",&PhpLinphoneProxy::disable);
        proxy.method("configure",&PhpLinphoneProxy::configure);
        proxy.method("isRegistered",&PhpLinphoneProxy::isRegistered);
        proxy.method("isClear",&PhpLinphoneProxy::isClear);
        proxy.method("hasError",&PhpLinphoneProxy::hasError);
        proxy.method("getError",&PhpLinphoneProxy::getError);
        proxy.method("authenticate",&PhpLinphoneProxy::authenticate);
        proxy.method("clear",&PhpLinphoneProxy::clear);
        extension.add(std::move(proxy));


        //Add call class.
        Php::Class<PhpLinphoneCall> call("PhpLinphoneCall");
        call.method("__construct",&PhpLinphoneCall::__construct,{
                Php::ByVal("core","PhpLinphoneCore",false,true)
        });
        call.method("__destruct",&PhpLinphoneCall::__destruct);
        call.method("invite",&PhpLinphoneCall::invite,{
                Php::ByVal("dest",Php::Type::String)
        });
        call.method("start",&PhpLinphoneCall::start);
        call.method("setRingTimeout",&PhpLinphoneCall::setRingTimeout);
        call.method("setCallTimeout",&PhpLinphoneCall::setCallTimeout);
        call.method("hangup",&PhpLinphoneCall::hangup);
        call.method("isEnded",&PhpLinphoneCall::isEnded);
        call.method("hasError",&PhpLinphoneCall::hasError);
        call.method("getError",&PhpLinphoneCall::getError);
        call.method("isReleased",&PhpLinphoneCall::isReleased);
        call.method("isRingExpired",&PhpLinphoneCall::isRingExpired);
        call.method("isCallExpired",&PhpLinphoneCall::isCallExpired);
        call.method("getUser",&PhpLinphoneCall::getUser);
        call.method("getDtmf",&PhpLinphoneCall::getDtmf);
        extension.add(std::move(call));


        // Add core class.
        Php::Class<PhpLinphoneCore> core("PhpLinphoneCore");
        core.method("__construct", &PhpLinphoneCore::__construct, {
                Php::ByVal("factoryConfig","PhpLinphoneFactoryConfig",true,false),
                Php::ByVal("config","PhpLinphoneConfig",true,false)
        });
        core.method("__destruct",&PhpLinphoneCore::__destruct);
        core.method("iterate",&PhpLinphoneCore::iterate);
        extension.add(std::move(core));


        // Add log file class.
        Php::Class<PhpLinphoneLog> log("PhpLinphoneLog");
        log.method("__construct",&PhpLinphoneLog::__construct);
        log.method("__destruct",&PhpLinphoneLog::__destruct);
        log.method("set",&PhpLinphoneLog::set,{
                Php::ByVal("filePath",Php::Type::String)
        });
        extension.add(std::move(log));


        // Add factory config file class.
        Php::Class<PhpLinphoneFactoryConfig> factoryConfig("PhpLinphoneFactoryConfig");
        factoryConfig.method("__construct",&PhpLinphoneFactoryConfig::__construct);
        factoryConfig.method("__destruct",&PhpLinphoneFactoryConfig::__destruct);
        factoryConfig.method("set",&PhpLinphoneFactoryConfig::set,{
                Php::ByVal("configPath",Php::Type::String)
        });
        extension.add(std::move(factoryConfig));


        // Add config file class.
        Php::Class<PhpLinphoneConfig> config("PhpLinphoneConfig");
        config.method("__construct",&PhpLinphoneConfig::__construct);
        config.method("__destruct",&PhpLinphoneConfig::__destruct);
        config.method("set",&PhpLinphoneConfig::set,{
                Php::ByVal("configPath",Php::Type::String)
        });
        extension.add(std::move(config));

        // Return the extension to PHP
        return extension;
    }
}
