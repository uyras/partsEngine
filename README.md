# partsEngine
This is the library for easy create different types of many-body systems and quickly manipulate it.
The problem is that there is parameter - Energy (E) which depends on between-particle interaction. In some cases particles may interact all-with-all, and in some cases only via neighbours. The interaction type is also variable.
When you calculate E, you should enumerate and calculate all pairs, which is very slow approach.

Usually when you perform modelling you rotate one particle per step. This library controls which particle is changed and automatically updates energy with fast algorithm without recalculating everything.

In addition it supports saving|loading of the system to file. Saved files may be opened via [QtViewer](https://github.com/uyras/QtViewer).

This library is not updated for a long time.
But, if you have interest to it, don't heistate to contact me!

Also, checkout [wiki](https://github.com/uyras/partsEngine/wiki). There are some examples.

And, read the [documentation](http://uyras.github.io/partsEngine/). I hope, in some future the code will be fully documented.

For another examples you can checkout `tests` folder. There is something.

## Features

* Array of particles (`PartArray` class) is the container for particles
* Particle (`Part` class) with the ability to inherit this.
* Control of states of particles via `StateMachine` class. You can save states for the whole system separately, also you may convert it to string to transfer via MPI processes
* Replacable hamiltonians realised as lambda-functions
* Ready realisations of amorphous, square, hexagonal, square Ising, square spin ice arrays. Realisation simplifies array creation and provide some geometry-specific functions.
* Realised Wang-Landau algorithm for calculaiting DOS
* Switching between 2D and 3D modes
* All functions are optimised for quick calculations
* save/load magnetic system to/from file

## Short todo

* generate documentation
* check pull requests
* provide normal examples
* Add DMI hamiltonian
* Translate everything to english as much as possible
* Transfer to cmake and make the lib independent from QT
* Provide different names for debug and release builds

## Compillation
Copy the repository:
```
git clone https://github.com/uyras/partsEngine.git
```
Go to the project folder:
```
cd partsEngine
```
Create make-files:
```
qmake partsEngine.pro
```
Compile library:
```
make release
```
You will see the `libPartsEngine.a` which is ready to add to your project

## How to connect the library to the new project (using Qt Creator)

Create new empty repository via GitHub, check potions that you need .gitignore for qt and readme.md.

Go to the folder which containts PartsEngine folder. Do not open it. 

Open bash and write:
```
git clone https://github.com/user/repo_name.git
```
replace the `repo_name` above and below to your actual repository name.
go to your new folder `repo_name` and create `repo_name.pro` with text:
```
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -L$$PWD/../PartsEngine -lPartsEngine

INCLUDEPATH += ../PartsEngine/

OTHER_FILES += \
    README.md \
    .gitignore
```
Create main.cpp with text (just some example):
```
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "config.h"
#include "PartArray.h"
#include <ctime>

using namespace std;

int main(){
    config::Instance()->srand(time(NULL));

    //write your code here

    cout<<"finish";
        return 0;
}

```
