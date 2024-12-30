# MCP23008-I2C Library
[![Spell Check](https://github.com/hasenradball/MCP23008-I2C/actions/workflows/spell_checker.yml/badge.svg)](https://github.com/hasenradball/MCP23008-I2C/actions/workflows/spell_checker.yml)
[![Generate Doxygen Documentation](https://github.com/hasenradball/MCP23008-I2C/actions/workflows/doxygen.yml/badge.svg)](https://github.com/hasenradball/MCP23008-I2C/actions/workflows/doxygen.yml)

Arduino Library for MCP23008, a 8-port GPIO exander 


![MCP23008-Adapter Picture](./docs/MCP23008_adapter.png)


## Contents
* [Library Documentation](#library-documentation)
* [Library Usage](#library-usage)
* [License](#license)
* [Helpful Links](#helpful-links)

## Library Documentation
The library documentaition ins mainly placed in the following pdf document [refman.pdf](./docs/latex/refman.pdf).<br>
Additionally in combination with the technical datasheet of microchip [MCP23008-Datasheet](./docs/MCP23008-Datasheet.pdf).

## Library Usage
### Controllers
The library is intended to be used on each microcontroller for Example:<br>
* Arnuino Nano
* Arduino Nano 33 IOT
* ESP8266
* ESP32
* etc ...

### Usage the MCP23008-I2C library in the Code
Include the library in you project via:

`#include <MCP23008-I2C.h>`

Instance an new MCP23008 object by:

`MCP23008_I2C::MCP23008 mcp{0x20};`

Now you ca use the object and his members as normal like:

`mcp.begin();`


Please refer to the examples and the above mentioned documentation files.

# License
This library is licensed under MIT Licence.

[MCP23008-I2C License](https://github.com/hasenradball/MCP23008-I2C/blob/master/LICENSE)


# Helpful Links
* [ESP8266-01-Adapter](https://esp8266-01-adapter.de)