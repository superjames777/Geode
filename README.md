# Geode

Geode is a small, interpreted, and statically typed programming language built in C++. I made it for fun so don't expect to be blown away by it or anything.

## Features

- print
- variables
- calculating
- comparison operators
- if & else
- while
- strings
- ints
- bools
- static typing
- stuff idk what to call it man just look at the source code its RIGHT there

### Coming Soon

- floats (not the yummy ones D: )

## How to Run With Geode

To run Geode files with Geode, first download the latest Geode version of geode.exe from the [GitHub Releases](https://github.com/superjames777/Geode/releases) page. It is HIGHLY recommended to add the folder containing geode.exe to your system's PATH. It will make it much easier and less of a hassle to run Geode files as you will not have to manually navigate to Geode's location on your system. Without PATH adding, you will have to run something like:

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

## Geode Documentation

For Documentation on Geode, go to the [GitHub Pages](https://superjames777.github.io/Geode/) site. It does not have much yet, but once the language gets some more features I will make sure to fully flesh it out.
