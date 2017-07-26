# Introduction
Current version of `HoudiniBifrost` is a very simple `GEO_IOTranslator`. But, "what does it mean?", one could ask. `GEO_IOTranslator` is a way to extend Houdini's Geometry IO system. By creating new `GEO_IOTranslator` we provide ability to translate some geometry format to native `GU_Detail`. After plugin installation, `File SOP` will gain a new feature to read `*.bif` files.

Unfortunately, `HoudiniBifrost` requires Maya dependency. You must have access to Maya and Bifrost libraries, but to use `HoudiniBifrost`, you do not require a Maya license.

### If you want to express your appreciation by buying me a coffee, here is a way:
 * [paypal](https://www.paypal.me/bareya/5)


# Installation
For now, I am providing compiled binaries to following operating systems:
* Linux
* Windows 10

Binaries can be found [here](https://github.com/bareya/HoudiniBifrost/releases). As soon as I get acccess to osx, I will release binaries for it.

#### Linux installation

Copy `libHoudiniBifrost.so` library to home `houdini16.0/dso`. 

Also, you have to add some environment variables to make it work. Here is an example of how to set it up:
```bash
export HFS=/opt/hfs16.0
export MAYA_APP_DIR=/home/bareya/Downloads/Maya/usr/autodesk/maya2017
export LD_LIBRARY_PATH=${HFS}/dsolib:${MAYA_APP_DIR}/plug-ins/bifrost/lib/
```

Released shared object was built under `Mint Linux 4.4.0-81-generic Ubuntu x86_64 GNU/Linux`. If you are having any difficulties with the distributed shared object, you can build it from sources, or post a question in Issues section.

#### Windows installation
Copy `HoudiniBifrost.dll` library to home `houdini16.0/dso`. 

Also, you have to append few entries to `PATH` environment variable:
 * C:\Program Files\Autodesk\Maya2017\plug-ins\bifrost\bin
 * C:\Program Files\Autodesk\Maya2017\bin

[How to add environment variables on Windows](https://www.computerhope.com/issues/ch000549.htm)

In addition you might need to install [Visual C++ Redistributable for Visual Studio 2012 Update 4](https://www.microsoft.com/en-ca/download/confirmation.aspx?id=30679), details can be found [here](https://github.com/bareya/HoudiniBifrost/issues/5)

# Compilation

To pull it:

`git clone git@github.com:bareya/HoudiniBifrost.git --recurse`

To compile it:

```shell
mkdir build
cd build
cmake .. -DCMAKE_CXX_COMPILER=g++-4.8 -DCMAKE_VERBOSE_MAKEFILE=True -DMAYA_DIR=/home/bareya/Downloads/Maya/usr/autodesk/maya2017
make install
```

# License
MIT License

Copyright (c) 2017 Piotr Barejko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
