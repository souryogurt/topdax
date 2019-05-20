topdax
======

Topdax is the simple Linux application that renders triangle using Vulkan API.

[![Build Status](https://travis-ci.org/souryogurt/topdax.svg?branch=master)](https://travis-ci.org/souryogurt/topdax)
[![Quality Status](https://sonarcloud.io/api/project_badges/measure?project=souryogurt_topdax&metric=alert_status)](https://sonarcloud.io/dashboard?id=souryogurt_topdax)

Features
--------

- Minimal dependencies
- Vulkan rendering

Installation
------------

Install topdax from sources, by running:

```sh
apt install gcc autoconf libtool libglfw3-dev
git clone https://github.com/souryogurt/topdax.git
cd topdax
autoreconf -if
./configure
make
make install
```

Development
-----------

To build topdax for development:
```sh
mkdir debug
cd debug
CFLAGS="-Wall -Wextra --std=gnu18 -pedantic -Og -g -fsanitize=address,undefined -fno-common -fsanitize-address-use-after-scope -fno-omit-frame-pointer " ../configure
make
```

Contribute
----------
- Read [How to submit an issue or feature request into tracker](https://github.com/souryogurt/topdax/wiki/How-to-submit-an-issue-or-feature-request)
- Issue Tracker: https://github.com/souryogurt/topdax/issues
- Source Code: https://github.com/souryogurt/topdax

Support
-------

If you are having issues, please let me know.
* Egor Artemov <egor.artemov@gmail.com>
