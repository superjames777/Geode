# Geode

Geode is a small, interpreted, and __statically typed*__ programming language built in C++. I made it for fun so don't expect to be blown away by it or anything.

## Features

- print
- variables
- calculating
- comparison operators
- if & else
- while
- stuff idk what to call it man just look at the source code its RIGHT there

### Coming Soon

- strings

## How to Run With Geode

To run Geode files with Geode, it is HIGHLY recommended to add the folder containing geode.exe to your system's PATH. It will make it much easier and less of a hassle to run Geode files as you will not have to manually navigate to Geode's location on your system. Without PATH adding, you will have to run something like:

```sh
cd C:\path\to\Geode
.\geode.exe geodefile.geo
```

I know, yucky. But if you add Geode to your system's PATH it would look something like:

```sh
geode geodefile.geo
```

Much better, right?

## How to Build Geode from Source

If you want to build Geode from the source code, you will need at least CMake v3.23. To build, run:

```sh
cmake -S . -B build
cmake --build build
```

___*Not true YET. This project is very WIP so some stuff here may not be exactly correct yet. (COMING EVENTUALLY!!!)___
