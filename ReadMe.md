# LibCorsairRGB #
This is an attempt at a windows based library to develop effects for the Corsair RGB keyboard range.
Corsair said they would release a LUA library but I haven't heard anything for a while, so feel free 
to fork and use this until they do.

I only have an ISO (UK) Layout Corsair K70 RGB so the only keymap I have tested in keymaps.h is 
for that model. Anyone with other models (K95/K65) or other layouts from around the world are 
welcome to come up with some keymaps and test them.

## Current status ##
The code is very rough. This is my first time making a library with C++, and I barely know C++ as 
it is. So if you have any feedback on how to structure code feel free to raise an issue or send 
me a pull request as I'm treating this as a learning exercise too.

## Example ##
See the example folder in the pre-built.zip to see some code and a pre-built binary to do some stuff with the library as it is now.

See it in action over on youtube: [https://www.youtube.com/watch?v=VFE7zAsrOJI](https://www.youtube.com/watch?v=VFE7zAsrOJI)

## How to use ##
If you want to build the library yourself, see below for build instructions, otherwise unzip 
pre-built.zip somewhere on your disk (C:\dev\LibCorsairRGB for example).

Assuming C++/Visual Studio 2012 use:

- Make a new C++ win32 console application
- Add the includes (Project Properties -> Configuration Properties -> C/C++ -> General -> Additional Include Directories = C:\dev\LibCorsairRGB\includes)
- Add the lib (Project Properties -> Configuration Properties -> Linker -> General -> Additional Library Directories = C:\dev\LibCorsairRGB\lib)
- Add the dependency (Project Properties -> Configuration Properties -> Linker -> General -> Additional Dependencies = LibCorsairRGB.lib)

When you build, make sure the LibCorsairRGB.dll is in the output directory, you can do this in the 
properties too (Project Properties -> Configuration Properties -> Build Events -> Post-Build Event 
-> Command Line = 'xcopy /y "C:\dev\LibCorsairRGB\LibCorsairRGB.dll" "$(OutDir)") and when running 
set it to run in the OutDir context (Project Properties -> Configuration Properties -> Debugging -> 
Working Directory = $(OutDir))

### Current methods ###
I prefixed them all with lcrgb_ so they should be easy to find:

- `lcrgb_initialise();`
 - Hook the keyboard, this should be run at the start and the return int checked for !=0 if errors
- `lcrgb_deinitialise();`
 - Give up control of the keyboard, ALWAYS call this at exit
- `lcrgb_set_keymap(keymap);`
 - Set the keymap to use when setting a key by enum (`lcrgb_key_enum`). Valid keymap params:
 - UK Keymap = `lcrgb_ISO_UK` *(default)*
 - American Keymap = `lcrgb_ASCII_US`
- `lcrgb_set_position(unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b);`
 - Given a matrix (currently only k70_uk_matrix) set a colour at an x,y coord
- `lcrgb_set_key(lcrgb_key_enum key, unsigned char r, unsigned char g, unsigned char b);`
 - Set the colour of a key by a given key name from the `lcrgb_key_enum` enum
- `lcrgb_set_key_code(unsigned char key_code, unsigned char r, unsigned char g, unsigned char b);`
 - Set a key directly by its USB keycode
- `lcrgb_flush_light_buffer();`
 - After setting some keys, apply the changes to the keyboard by flushing the buffer
- `lcrgb_set_verbosity_level();`
 - Set to > 0 to see some extra debug when initialising, useful if it doesn't find your keyboard

**Important:** Before running anything using libusb for the first time you need to install some 
drivers for libusb to work with, see here for information on how to set up the libusb drivers for windows: 
[https://github.com/libusb/libusb/wiki/Windows#Driver_Installation](https://github.com/libusb/libusb/wiki/Windows#Driver_Installation) 
(I personally used the Zadig automated driver installer for WinUSB on Windows 7).

## How to build ##
I'm using VS2012 and [libusb-1.0.19](https://github.com/libusb/libusb/tree/v1.0.19). I'd suggest 
the easiest way to build my library is to grab the pre-built binaries for libusb: 
[http://sourceforge.net/projects/libusb/files/libusb-1.0/libusb-1.0.19/libusb-1.0.19.7z/download](http://sourceforge.net/projects/libusb/files/libusb-1.0/libusb-1.0.19/libusb-1.0.19.7z/download) 
and link against the static lib in `libusb-1.0.19\MS32\static`

Link libusb and then build the solution and you should get a dll, lib and have the header files 
handy to do with whatever you like.

## Thanks ##
All of the hard USB work had been done by others, all I'm trying to do with this is package it into a single library which I can anywhere to set the colours of keys.

Most credit goes to: [https://github.com/CalcProgrammer1/CorsairKeyboardSpectrograph/](https://github.com/CalcProgrammer1/CorsairKeyboardSpectrograph/)
Where I have just straight up ripped the USB protocol work and colour setting code. I'm planning to look in to this and see if it can be tweaked for my uses later.