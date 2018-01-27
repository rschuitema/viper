
[![Build Status](https://travis-ci.org/rschuitema/viper.svg?branch=develop)](https://travis-ci.org/rschuitema/viper)

# viper
Repository for a library that can interact with the viperboard of Nano River Technologies.



## Build

Create a build directory. In this directory execute the following command:

`cmake ..`
`make `



## Run unit tests

Create a build directory. In this directory execute the following command:

`cmake ..`
`make test `

or

`cmake ..`
`make`
``./viperboard/test/test_viperboard`



## Measure coverage

Create a build directory. In this directory execute the following command:

`cmake -DCMAKE_BUILD_TYPE=Debug -DCREATE_COVERAGE_TARGETS=ON ..`
`make test_viperboard_coverage`

This will generate html files in the directory `build/test_viperboard_coverage`

