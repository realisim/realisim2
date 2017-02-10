Repo that holds my personal code base. It is home to experimentation and learning, mostly c++.

--------------
# Building with CMake
Simply run CMake (v2.8.11 or higher).

## CMake Options
* BUILD_PROTOTYPE:
  When activated, the prototype specified by path 'Prototype_to_build' will be added to the solution. This is simple a convenient way of selecting a prototype to build.
* BUILD_UNIT_TESTS: When activated, all unit tests will be built.

## Installing
An install target is present and will copy everything necessary (per project) to a folder bin. The folder bin will be created if needed.

--------------
# Notes
Some Projects/Prototypes require Qt 5.6.2 which can be dowloaded at https://www.qt.io/qt5-6/.
CMake will ask for some configuration. It will ask to configure each Qt module by specifiyng a path to Qt's cmake modules.

On windows, all Qt's cmake module are located in '[QT5 install path]/msvc2015_64/lib/cmake/'. So to configure Qt5Core, simply point to F:/Qt/Qt5.6.1/5.6/msvc2015_64/lib/cmake/Qt5Core

Each Qt module in use will need to be configure in cmake.
