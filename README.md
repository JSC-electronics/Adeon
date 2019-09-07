# ADEON
[![Build Status](https://travis-ci.com/JSC-electronics/Adeon.svg?branch=master)](https://travis-ci.com/JSC-electronics/Adeon)
[![GitHub release](https://img.shields.io/github/release/JSC-electronics/ObjectButton.svg?maxAge=3600)](https://github.com/JSC-electronics/Adeon/releases)
[![License: Apache 2.0](https://img.shields.io/badge/license-Apache--2.0-green.svg)](https://github.com/JSC-electronics/Adeon/blob/master/LICENSE)

<img align="right" src="images\adeon-logo-192x192.png">

ADEON is a software solution for controlling GSM devices (GSM gates, relays, modules etc.) via SMS commands. It consists of two main parts:

1. ADEON android mobile app available on Google Play
2. Library for embedded development kits (open-source)

ADEON app allows user to create a list of GSM devices with predefined commands. These commands can be sent in form of SMS message to the target GSM device and thus control its behaviour, e.g. switch on/off a relay, open/close a gate, de/activate alarm etc.

The library for embedded development kits provides user an API for receiving commands from the ADEON app. It is primarily intended for use with Arduino-like boards but can be also easily integrated to other C++ projects.

For more information please visit our [Wiki page](https://github.com/JSC-electronics/adeon/wiki).
