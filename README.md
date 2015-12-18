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
* gcc
* pkg-config
* make

From there, **simpletimer** follows the standard `make` and `make install` build and installation process.
