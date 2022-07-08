### Description

Simple logging library focused around record implementation with modern syntax
and configuration by template parameters.

Characteristic:

- C++ 20
- Header only
- Syntax like `std::ostream`

Features:

- Supports multiple inputs and outputs at the same time.
- Based on the user implementation of the record, that fulfills type contract.
  enforced by concepts.
- User defined formatting.
- User defined, both static and dynamic filtering.
- Easy extensible by callbacks.
- Ability to provide custom log rotation, retention logic and more.

### Usage

Follow the instructions to use the project:

```shell
git clone https://github.com/milosob/lgg.git lgg
```

```shell
mkdir lgg-build 
```

```shell
cd lgg-build
```

```shell
cmake ../lgg
# Or in case you want to compile examples:
cmake ../lgg -DLGG_EXAMPLES=ON && make
```

```shell
sudo cmake --install .
```

Import the package into the CMake project:

```cmake
find_package(lgg CONFIG REQUIRED)
add_executable(application main.cpp)
target_link_libraries(application PRIVATE lgg)
```

Then, include the following header:

```c++
#include <lgg/logger.h>
```

### Examples

Extensive usage example with commentary, covering most of the features is
available in the `examples` directory.

After initializing the logger, the interaction with it could be similar to:

```c++
lg << "message_1_part_0 " << "message_1_part_1 " << "message_1_part_2" << lgg::push;
lg << "message_2_part_0 " << "message_2_part_1 " << "message_2_part_2" << lgg::push;
lg << "message_3_part_0 " << "message_3_part_1 " << "message_3_part_2";
lg << "message_3_part_3 " << lgg::push;
lg << logger_level::FATAL << "message_4_part_0 " << "message_4_part_1 " << lgg::push;
```

And the possible output, dependent on record formatting implementation could be:

```text
2022-07-07 14:00:30 [id=logger_example] [level=DEBUG]: message_1_part_0 message_1_part_1 message_1_part_2
2022-07-07 14:00:30 [id=logger_example] [level=DEBUG]: message_2_part_0 message_2_part_1 message_2_part_2
2022-07-07 14:00:30 [id=logger_example] [level=DEBUG]: message_3_part_0 message_3_part_1 message_3_part_2message_3_part_3 
2022-07-07 14:00:30 [id=logger_example] [level=FATAL]: message_4_part_0 message_4_part_1 
```
