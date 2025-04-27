## Setup Instructions
### Building PHP (Windows)
If you want to go down the dark rabbit hole of building PHP on Windows, follow [this document](https://wiki.php.net/internals/windows/stepbystepbuild_sdk_2).    Because PHP's build tools for Windows are horrible to deal with, [binary downloads are available](https://windows.php.net/download/).  Make sure to download non-thread-safe builds.

TODO—you should be able to download the zip and development package.  Try to extract them into the same root.
### Building PHP (Linux/Unix-like)
* Clone PHP and check out a recent tag: `git clone https://github.com/php/php-src; cd php-src; git switch --detach php-8.4.6`
  * Alternatively, download source code from https://www.php.net/downloads
* Run buildconf: `./buildconf --force`
  * This step is not necessary if using a downloaded version of PHP
* Run `./configure --prefix=${PHP_ROOT}`
  * I like to install PHP versions in my home directory (i.e. $HOME/php-8.4.6) and symlink php-latest to them (`ln -s $HOME/php-latest $HOME/php-8.4.6`), this way I can add `$HOME/php-latest/bin` to my path)
  * You'll need to install any dependencies (`sudo apt install build-essential autoconf libtool bison re2c pkg-config` should work on Ubuntu/Debian)
  * This library is probably not compatible with thread-safe PHP (untested) - `get_args` will need a revision since args are passed through thread-local storage in ZTS mode
* Run ``make -j`nproc` ``
* Run `make install`
  * You will need `sudo make install` if your prefix isn't writable by your user
* Add `${PHP_ROOT}/bin` to your path
  * Alternatively, provide the full path to `${PHP_ROOT}/bin/php` later rather than `php`
### Downloading Composer
* Run `php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');"`
* Run `php composer-setup.php`
* Run `php composer.phar global require composer/composer` 
* Add `$HOME/.config/composer/vendor/bin` to your path
  * Alternatively, provide the full path to `$HOME/.config/composer/vendor/bin/composer` later rather than `composer`
### Building & Running Extension
* In the parent project, build with CMake with the following variables:
  * PHP_ROOT being the root path to a PHP 8.4+ installation with header files (/home/iggyvolz/php-8.4.6)
    * The file `${PHP_ROOT}/include/php/main/php.h` should exist
    * The path should be a realpath without symlinks when using CLion on WSL to avoid [CPP-12261](https://youtrack.jetbrains.com/issue/CPP-12261/Include-directories-not-resolved-if-using-symlink-and-WSL-toolchain)
  * DISCORD_SOCIAL_SDK_ROOT being the root path to a Discord Social SDK installation (/mnt/c/Users/iggyvolz/Downloads/DiscordSocialSdk-1.1.8318/discord_social_sdk)
    * The file `${DISCORD_SOCIAL_SDK_ROOT}/include/discordpp.h` should exist
* Inside the php directory, run `composer update`
* Run `php -dextension=path/to/libdiscordsocial.so php/test.php` (substituting the first `php` with the full path to PHP if you didn't add it to your path)
  * On Windows, you would run `path\to\php.exe -dextension=path\to\libdiscordsocial.so php\test.php`
  * In CLion, you can set the executable to `path/to/php`, the program arguments to `-dextension=$CMakeCurrentProductFile$ php/test.php`, and the working directory to the root of the project
  * In GDB, prefix with `gdb --args`
## Code Snippets

JS to pull enum from Discord's documentation into PHP format:
```js
console.log(Array.from($0.querySelectorAll("tr")).filter(x => x.querySelector("td")).map(x => "    case " + x.querySelector("td").innerText.trim() + " = " + x.querySelector("td:nth-child(2)").innerText.trim() + ";").join("\n"))
```

