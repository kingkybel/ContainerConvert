# ContainerConvert

Header-only C++ utilities for converting and filtering standard containers.

## What This Repository Does

This library provides small, focused template helpers in `include/container_convert.h` for:

- Converting between STL container types:
  - `toVector(std::set<...>)`
  - `toVector(std::deque<...>)`
  - `toDeque(std::vector<...>)`
  - `toSet(std::vector<...>)`
  - `toSet(std::unordered_set<...>)`
  - `toMap(std::unordered_map<...>)`
  - `toOrderedKeySet(std::unordered_map<...>)`
- Filtering/removing elements:
  - `eraseRemove(...)` for sequence containers
  - `eraseByKey(...)` / `eraseByValue(...)` for `std::map`
- Moving selected elements from one vector to another:
  - `moveElementsTo(...)`

The conversion helpers preserve element values and ordering semantics of the target container (for example, `std::set` ordering and uniqueness).

## Requirements

- C++23 compiler
- CMake >= 3.26
- Git submodules initialized (the header uses `dkyb/traits.h` from `TypeTraits`)
- GoogleTest (only for tests)

## Quick Start

```bash
git clone git@github.com:kingkybel/ContainerConvert.git
cd ContainerConvert
git submodule update --init --recursive
```

### Build + Install

```bash
mkdir -p build
cd build
cmake -Wno-dev -DCMAKE_INSTALL_PREFIX=/usr ..
cmake --build . --parallel $(nproc)
sudo cmake --install .
```

Headers are installed to `include/dkyb`, so consuming code can use:

```cpp
#include <dkyb/container_convert.h>
```

### Run Tests

```bash
cmake -S . -B build
cmake --build build --parallel $(nproc)
ctest --test-dir build --output-on-failure
```

## Recent Changes

### Committed updates (2024-11-05)

- Added the `TypeTraits` submodule dependency to provide compile-time traits used by the container constraints.
- Added a repository `.clang-tidy` configuration.
- Updated CI workflow (`.github/workflows/cmake-single-platform.yml`).
- Updated top-level `CMakeLists.txt` build configuration.

### Current in-repo changes (staged)

- Added `cmake-common` as a second git submodule in `.gitmodules`.
  - This provides shared CMake settings for dkyb repositories and is now tracked directly in this repo.

## License

GPL-2.0-or-later. See `LICENSE`.
