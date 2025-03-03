# base64safe

base64safe is a C++ library for encoding and decoding base64 strings with URL and filename-safe characters (`-` and `_` instead of `+` and `/`)

## Requirements

- C++17 or later

## Usage

Include the [base64safe.h](./include/base64safe.h) header in your project and use the `base64safe::encode` and `base64safe::decode` functions.

```cpp
#include "base64safe.h"
#include <iostream>

int main() {
    std::string original = "hello";
    std::string encoded = base64safe::encode(original);
    std::string decoded = base64safe::decode(encoded);

    std::cout << "Original: " << original << std::endl;
    std::cout << "Encoded: " << encoded << std::endl;
    std::cout << "Decoded: " << decoded << std::endl;

    return 0;
}
```

## Tests

### Requirements

- [Meson](https://mesonbuild.com/) build system

### Running Tests

To run the unit tests, enable the tests option and build the project:
```
meson setup build -Denable_tests=true
meson test -C build
```

## License

This project is licensed under the 0BSD License. See the LICENSE file for details.

