#  OpenGL Setup Example


Install [Homebrew](https://brew.sh/) if you don't have it already.
There are different versions and paths they use for Intel binaries and Apple Silicon binaries.
These release notes at https://brew.sh/2021/02/05/homebrew-3.0.0/ describe how/why for the two paths.

    brew install glfw3
    brew install glbinding

Create an xcode cli project, pick C++.
In build settings: search for paths, header, click +, 
Then add /usr/local/include or /opt/homebrew/include (or wherever they are)

Add the header search path in Build Settings tab for it to find the binaries.

- Intel homebrew: /usr/local/include
- Apple Silicon Homebrew: /opt/homebrew/Cellar

Add the binaries you need to link with.
To do this, go to project settings, select your target, then
Click "Build  Phases" tab, and expand "Link Binary with Libraries"
Click +

Add openGl framework (built in)

Then click + again
Select "Add Other..." 
Then select "Add files..." from the drop down.
Hit Command + shift + G
Type the path i.e. /usr/local/lib
Go into the glbinding and glfw dirctories

Add the libraries files
The files will be named something like /opt/homebrew/Cellar/glfw/<version>/something.dylib
Pick the .dylib files that do not show as an alias. There should be one each for each package version.

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


