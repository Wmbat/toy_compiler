#toy_compiler

C++ executable

## Table of Contents

* [Requirements](#Requirements)
* [Dependencies](#Dependencies)
* [Third-party libraries](#Third-party-libraries)
* [Instructions](#Instructions)

## Requirements

* C++20 (gcc-10/clang-10)
* [Build2](https://build2.org/)
* Linux

## Dependencies

* [spdlog - 1.6.1](https://github.com/gabime/spdlog)

## Third-party Libraries

* [range-v3](https://github.com/ericniebler/range-v3)
* [doctest](https://github.com/onqtam/doctest)
* [libcaramel](https://github.com/Wmbat/libcaramel)
* [monads](https://github.com/Wmbat/monads)
* [doctest (for unit tests)](https://github.com/onqtam/doctest)

## TODO

* [ ] Instead of loading the whole file, load file into chunks
* [X] Implement Scientific notation
* [ ] Properly handle nested block comments
* [X] Implement testing for the Application class
* [X] Document public API
* [o] Implement unit test:
   * [X] grammar/:
      * [X] grammar_type.hpp
      * [X] rule.hpp
      * [X] symbol.hpp
      * [X] symbol_table.hpp
      * [X] token_type.hpp
   * [ ] core/:
      * [ ] parser.hpp
