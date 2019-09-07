# ADEON
[![Build Status](https://travis-ci.com/JSC-electronics/Adeon.svg?branch=master)](https://travis-ci.com/JSC-electronics/Adeon)
[![GitHub release](https://img.shields.io/github/release/JSC-electronics/ObjectButton.svg?maxAge=3600)](https://github.com/JSC-electronics/Adeon/releases)
[![License: Apache 2.0](https://img.shields.io/badge/license-Apache--2.0-green.svg)](https://github.com/JSC-electronics/Adeon/blob/master/LICENSE)
[![Donate](https://img.shields.io/badge/donate-PayPal-blueviolet.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SESX9ABM7V8KA&source=url)

<img align="right" src="images\adeon-logo-192x192.png">

ADEON is a software solution for controlling GSM devices (GSM gates, relays, modules etc.) via SMS commands. It consists of two main parts:

1. ADEON android mobile app [available on Google Play](https://play.google.com/store/apps/details?id=cz.jscelectronics.adeon)
2. Library for embedded development kits (open-source)

ADEON app allows user to create a list of GSM devices with predefined commands. These commands can be sent in form of SMS message to the target GSM device and thus control its behaviour, e.g. switch on/off a relay, open/close a gate, de/activate alarm etc.

The library for embedded development kits serves to process commands from the ADEON app. It is primarily intended for use with Arduino-like boards but can be also easily integrated to other C++ projects.

For more information please visit our [Wiki page](https://github.com/JSC-electronics/adeon/wiki).

## Documentation
- [GitHub Wiki][adeon-wiki]
- [Extended Doxygen Documentation][adeon-doxygen]
- [Examples](examples)

## Contributing
There are many ways in which you can participate in the project, for example:

* [Submit bugs and feature requests](https://github.com/JSC-electronics/Adeon/issues), and help us verify as they are checked in
* Review [source code changes](https://github.com/JSC-electronics/ObjectButton/pulls)
* Review the [documentation](https://github.com/JSC-electronics/Adeon/wiki) and make pull requests for anything from typos to new content

Please read [contributing rules](CONTRIBUTING.md) for more details.

## License

Copyright (c) JSC electronics. All rights reserved.

Licensed under the [Apache-2.0](LICENSE) license.

## Support

We hope our library helped to speed up your project development. You can support our effort to convert coffe and pizza into a code with a small donation. Any support is much appreciated.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SESX9ABM7V8KA&source=url)

[//]: # (Used references)
[adeon-wiki]: https://github.com/JSC-electronics/adeon/wiki
[adeon-doxygen]: https://jsc-electronics.github.io/Adeon
