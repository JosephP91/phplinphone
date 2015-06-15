# phplinphone

phplinphone is a C++ liblinphone library implementation, callable from PHP. Liblinphone is a high level library integrating all the SIP video calls feature into a single easy to use API. 

If you want to know a bit more about linphone and liblinphone, you can visit linphone website: http://www.linphone.org/

Requirements
------------

phplinphone, of course, need liblinphone to be installed, so you should install the latest version of liblinphone (at the time I'm writing, it's the 3.8.2), downlodable from here: http://mirror.lihnidos.org/GNU/savannah/linphone/3.8.x/sources/

To install liblinphone, you can follow this guide http://sagar.se/linphone-3.7.0-wheezy.html . It works for liblinphone 3.7.0, but you can easly modify it for 3.8.2 version.

After you have installed liblinphone, you have to install phpcpp. phpcpp is a C++ library used to write PHP extension using C++ as main language. You can easly install it following phpcpp's website guide: http://www.php-cpp.com/documentation/install

Install
-------

Once you have installed the required libraries and downloaded phplinphone sources, you can install phplinphone library simply typing:

```bash
make && sudo make install && sudo php5enmod phplinphone
```

If everything works without errors, you can begin to use phplinphone classes from your PHP code.

Simple Usage
------------

Linphone can log its actions, so let's create a simple log from our PHP application:

```php
$log = new PhpLinphoneLog();
$log->set('/path/to/my/log');
```

Linphone can also configure itself starting from a factory configuration path. If you know what it is, how it is builded, and if you have one, you can specify it in this way:

```php
$factoryConfig= new PhpLinphoneFactoryConfig();
$factoryConfig->set('/path/to/my/factory/config');
```

If you do not have a configuration file, don't panic. You can specifiy a file where liblinphone will write your settings.

```php
$config= new PhpLinphoneConfig();
$config->set('/path/to/my/future/config');
```

Now we need to instantiate the main linphone object, which is the core object. Nothing easier than:

```php
$core = new PhpLinphoneCore();
```

If you have created configuration files (as I showed in the previous code snippet) you can pass them to the core object, allowing it to use them.

```php
// If you have both of them
$core = new PhpLinphoneCore($factoryConfig,$config);
// If you have only the factory config
$core = new PhpLinphoneCore($factoryConfig);
// If you have only the config
$core = new PhpLinphoneCore(NULL,$config);
```

Now we have the logs and core object. What now? We must provide our SIP account authentication credentials. I have created a SIP account on linphone website, so you can do like me and get your account for free: https://www.linphone.org/free-sip-service.html
Once you have registered your account, open back your beautiful terminal and keep writing:

```php
// We must provide our core object.
$auth = new PhpLinphoneAuth($core);
$auth->setUsername('sip:myusername@sip.linphone.org');
$auth->setPassword('myverycomplexpassword');
$auth->build();
```

Linphone authenticate you throught proxies, so let's create one and authenticate on it with:

```php
// We must provide core object and authentication object.
$proxy = new PhpLinphoneProxy($core,$auth);
$proxy->configure();
echo "Registration in progres...";
$proxy->authenticate();
if ($proxy->hasError()) {
  echo "failed with reason: ".$proxy->getError()."\n";
  exit;
}
echo "done.\nNow disabling and clearing the proxy config... ";
$proxy->disable();
$proxy->clear();
if ($proxy->isClear()) {
  echo "cleared.\n";
} else {
  echo "could not be cleared.\n";
}
```

Now we can begin to call our SIP friends!

```php
// Our buddie
$userToCall = 'sip:myfriend@sip.linphone.org';
// Let's create a call object.
$call = new PhpLinphoneCall($core);
/* We can also set a ring timeout, but it's not mandatory.*/
$call->setRingTimeout(10);
/* And you can even specify a call timeout. The difference between call timeout and ring timeout is that a ring
timeout is the number of seconds from when you begin to call until the user picks up the call. The call timeout,
instead, is the time from when the user picks up until he do something like press a key. Use the call timeout only if you want that the user press a number or something like that. */
$call->setCallTimeout(10);
$call->invite($userToCall);
$call->start();
while (!$call->isEnded() and !$call->hasError() and !$call->isReleased() and !$call->isRingExpired() and !$call->isCallExpired()) {
      // Don't forget to call this method!! It must be called periodically to allow linphone core to do background work!
      $core->iterate();
}
echo "User ".$call->getUser()." ended the call!\n";
$call->hangup();
exit;
```

