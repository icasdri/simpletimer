# simpletimer
**simpletimer** is a lightweight timer/stopwatch program that allows multiple timers to be run at once in a compact fashion. It is written in C with GTK.

## Usage
Launch "Simple Timer" from an XDG-compliant desktop (.desktop file is provided), or run `simpletimer` from the terminal, then...
* Use the "+" button to start a new timer. It will be appended to the bottom of the list.
* Use the "pause" button next to each timer to pause a timer, and the corresponding "play" button to resume it.
* Click on a timer to select it. Then use the "trash" button to remove the selected timer.

## Building
The following dependencies are needed to build:
* gtk3
* glib2
* pkg-config
* make
* gcc or clang (compatible with gcc flags) -- the build has been tested with both, change the `CC` line in the `Makefile` to choose between the compilers

From there, the standard `make` and `make install` build & installation process can be followed.

Note: the default compiler flags put correctness and security first (the flags for aggressive warnings, stack protectors, and positional independence are active). These flags will fail the build on any number of warnings or errors. As such, the `CFLAGS` line of the `Makefile` may need to be changed to something different if these flags become too aggressive during e.g. development.

## Packages
Currently, packages only exist for Arch Linux in the AUR: [simpletimer-git](https://aur.archlinux.org/packages/simpletimer-git/).

However, users on other distributions may simply `make` `make install` to get the program installed: the dependencies are very minimal.

Additional packages are welcome: please let me know if you package **simpletimer** for a different distribution!
