#  OpenGL Setup Example

## Overview

These instructions and sample code are for setting up OpenGL on Mac in Xcode as of MacOS Monterey and 2022.

They use glew and glfw3 and also glm. And libpng for loading textures.

## Dependencies

- glfw3
- glew
- glm
- libpng

## Setup Instructions

### Install Homebrew
Install [Homebrew](https://brew.sh/) if you don't have it already.
There are different versions and paths they use for Intel binaries and Apple Silicon binaries.

For Apple silicon homebrew, it is typically underneath /opt/homebrew. For Intel it is under /usr/local.


These release notes at https://brew.sh/2021/02/05/homebrew-3.0.0/ describe how/why for the two paths.

### Install Libraries

Install glfw3

    /opt/homebrew/bin/brew install glfw3
    
And install glew

    /opt/homebrew/bin/brew install glew

And also glm 

    /opt/homebrew/bin/brew install glm

And libpng

    /opt/homebrew/bin/brew install libpng

## Project Setup

Create an Xcode MacOS cli project. Choose C++ for the language.

Add the header search path in Build Settings tab for it to find the binaries.

In build settings: search for paths, header, click +, 

Then add /usr/local/include or /opt/homebrew/include (or wherever they are based on Intel/Apple silicon)

- Intel homebrew: /usr/local/include
- Apple Silicon Homebrew: /opt/homebrew/Cellar


Add the binaries you need to link with.
To do this, 

1. Go to project settings, 
2. Select your target
3. Then click "Build  Phases" tab, 
4. and expand "Link Binary with Libraries"
5. Click +

Search for and add the OpenGL framework which comes with Xcode.

1. Then click + again
2. Select "Add Other..." 
3. Then select "Add files..." from the drop down.
4. Hit Command + shift + G
5. Type the path for your homebrew libraries: /opt/homebrew/Cellar for Apple silicon
6. Go into the direcotory named for the specific binary(i.e. glm)
7. Go into the version subdirectory, /opt/homebrew/Cellar/libpng/1.6.39
8. Go into the "lib" subdirectory within that version.
9. Find and choose the .dylib files that does NOT show as an alias.

For example, one of the files on my system is named /opt/homebrew/Cellar/glfw/3.3.8/lib/libglfw.3.3.dylib.

Repeat the above steps for glew, glfw, glm, and libpng libraries.

## Code Signing

If you get a bunch of warnings about signing and team mapping ID when you try to launch
you will have to sort out the code signing settings.
I had to sign the library files and also double check some Xcode settings.

In particular, if CODE_SIGN_IDENTITY gets set to '-' in the pbproject file settings it 
will not launch even if I have signed the libraries with the instructions below.

Check how a library file is signed or not with this:

    codesign -d -v /opt/homebrew/opt/glfw/lib/libglfw.3.dylib

find your identity with this:

    security find-identity -v -p codesigning

Then sign it with your identity like this:

    codesign -s "Apple Development: Your Name (blablah)" <lib_path>

If you get an "already signed" error then add "-f" flag in front of the library path and re-run.



## Credits


- The rocky.png texture is from https://opengameart.org/content/seamless-2

