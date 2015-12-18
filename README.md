# simpletimer
**simpletimer** is a lightweight timer/stopwatch program that allows multiple timers to be run at once in a compact fashion. It is written in C with GTK.

## Usage
Simply launch **simpletimer** from the binary `simpletimer`.
* Use the "+" button to start a new timer. It will be appended to the bottom of the list.
* Use the "pause" button next to each timer to pause a timer, and the corresponding "play" button to resume it.
* Click on a timer to select it. Then use the "trash" button to remove the selected timer.

## Building
**simpletimer** requires the following dependencies to build:
* gtk3
* glib2
* pkg-config
* make
* gcc or clang (compatible with gcc flags) -- the build has been tested with both, change the `CC` line in the `Makefile` to choose between the compilers

From there, **simpletimer** follows the standard `make` and `make install` build and installation process.

Note: **simpletimer** is built with compiler flags that put correctness and security first (such as those for aggressive warnings, stack protectors, and positional independence). These will fail the build on any number of warnings or errors -- as such the `CFLAGS` line of the `Makefile` may need to be changed to something different if these flags become too aggressive during development or otherwise.
