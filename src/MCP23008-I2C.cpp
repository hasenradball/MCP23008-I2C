/**
 * @file    MCP23008
 * @author  Frank Häfele
 * @date    27.12.2024
 * @version 1.0.0
 * @brief   Arduino class for 8-channel port expander MCP23008
 * @see     tbd
 * 
 */

#include "MCP23008-I2C.h"
#include "MCP23008_registers.h"

using namespace MCP23008_I2C;

MCP23008::MCP23008(uint8_t address, TwoWire *wire)
: _address{address}, _wire{wire}
{}

bool MCP23008::begin(bool inputPullUp) {
  //  check connected
  if (! isConnected()) return false;

  //  disable address increment (datasheet P20
  //    SEQOP: Sequential Operation mode bit
  //    1 = Sequential operation disabled, address pointer does not increment.
  //    0 = Sequential operation enabled, address pointer increments.
  //  if (! writeReg(MCP23x08_IOCR, MCP23008_IOCON_SEQOP)) return false;

  if (inputPullUp)
  {
    //  Force INPUT_PULLUP
    if (! writeReg(MCP23008_GPPU_REG, 0xFF)) return false;   //  0xFF == all UP
  }
  return true;
}


bool MCP23008::isConnected() {
  _wire->beginTransmission(_address);
  if (_wire->endTransmission() != 0)
  {
    _error = MCP23008_I2C_ERROR;
    return false;
  }
  _error = MCP23008_OK;
  return true;
}

/* #################################### */
/* ### --- single pin interface --- ### */
/* #################################### */
bool MCP23008::pinMode1(uint8_t pin, uint8_t mode) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }
  if ((mode != INPUT) && (mode != INPUT_PULLUP) && (mode != OUTPUT))
  {
    _error = MCP23008_VALUE_ERROR;
    return false;
  }

  uint8_t val = readReg(MCP23008_IODIR_REG);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  uint8_t mask = 1 << pin;
  //  only work with valid
  if ((mode == INPUT) || (mode == INPUT_PULLUP))
  {
    val |= mask;
  }
  else if (mode == OUTPUT)
  {
    val &= ~mask;
  }
  // other values won't change val ....
  writeReg(MCP23008_IODIR_REG, val);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  return true;
}


bool MCP23008::write1(uint8_t pin, uint8_t value) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }
  uint8_t val = readReg(MCP23008_OLAT_REG);
  uint8_t pre = val;
  if (_error != MCP23008_OK)
  {
    return false;
  }

  uint8_t mask = 1 << pin;
  if (value)
  {
    val |= mask;
  }
  else
  {
    val &= ~mask;
  }
  //  only write when changed.
  if (pre != val)
  {
    writeReg(MCP23008_OLAT_REG, val);
    if (_error != MCP23008_OK)
    {
      return false;
    }
  }
  return true;
}


uint8_t MCP23008::read1(uint8_t pin) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return MCP23008_INVALID_READ;
  }
  uint8_t val = readReg(MCP23008_GPIO_REG);
  if (_error != MCP23008_OK)
  {
    return MCP23008_INVALID_READ;
  }
  uint8_t mask = 1 << pin;
  if (val & mask) return HIGH;
  return LOW;
}


bool MCP23008::setPolarity(uint8_t pin,  bool reversed) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }
  uint8_t val = readReg(MCP23008_IPOL_REG);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  uint8_t mask = 1 << pin;
  if (reversed)
  {
    val |= mask;
  }
  else
  {
    val &= ~mask;
  }
  writeReg(MCP23008_IPOL_REG, val);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  return true;
}


bool MCP23008::getPolarity(uint8_t pin, bool &reversed) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }
  uint8_t val = readReg(MCP23008_IPOL_REG);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  uint8_t mask = 1 << pin;
  reversed = (val & mask) > 0;
  return true;
}

bool MCP23008::setPullup(uint8_t pin,  bool pullup) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }
  uint8_t val = readReg(MCP23008_GPPU_REG);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  uint8_t mask = 1 << pin;
  if (pullup)
  {
    val |= mask;
  }
  else
  {
    val &= ~mask;
  }
  writeReg(MCP23008_GPPU_REG, val);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  return true;
}


bool MCP23008::getPullup(uint8_t pin, bool &pullup) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }
  uint8_t val = readReg(MCP23008_GPPU_REG);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  uint8_t mask = 1 << pin;
  pullup = (val & mask) > 0;
  return true;
}

/* #################################### */
/* ### ---  8 - pin interface   --- ### */
/* #################################### */

bool MCP23008::pinMode8(uint8_t mask) {
  writeReg(MCP23008_IODIR_REG, mask);
  _error = MCP23008_OK;
  return true;
}

bool MCP23008::write8(uint8_t value) {
  writeReg(MCP23008_OLAT_REG, value);
  _error = MCP23008_OK;
  return true;
}

int MCP23008::read8() {
  _error = MCP23008_OK;
  return readReg(MCP23008_GPIO_REG);
}

bool MCP23008::setPolarity8(uint8_t mask) {
  writeReg(MCP23008_IPOL_REG, mask);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  return true;
}

bool MCP23008::getPolarity8(uint8_t &mask) {
  mask = readReg(MCP23008_IPOL_REG);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  return true;
}

bool MCP23008::setPullup8(uint8_t mask) {
  writeReg(MCP23008_GPPU_REG, mask);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  return true;
}


bool MCP23008::getPullup8(uint8_t &mask) {
  mask = readReg(MCP23008_GPPU_REG);
  if (_error != MCP23008_OK)
  {
    return false;
  }
  return true;
}

bool MCP23008::setInterrupt(uint8_t pin, uint8_t mode) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }

  // right mode
  uint8_t intcon = readReg(MCP23008_INTCON_REG);
  if (mode == CHANGE)
  {
    // compare to previous value.
    intcon &= ~(1 << pin);
  }
  else
  {
    uint8_t defval = readReg(MCP23008_DEFVAL_REG);
    if (mode == RISING)
    {
      intcon |= (1 << pin);
      defval &= ~(1 << pin);  // RISING == compare to 0
    }
    else if (mode == FALLING)
    {
      intcon |= (1 << pin);
      defval |= ~(1 << pin);  // FALLING == compare to 1
    }
    writeReg(MCP23008_DEFVAL_REG, defval);
  }
  writeReg(MCP23008_INTCON_REG, intcon);

  // enable interrupt
  uint16_t value = readReg(MCP23008_GPINTEN_REG);
  value |= (1 << pin);
  return writeReg(MCP23008_GPINTEN_REG, value);
}


bool MCP23008::disableInterrupt(uint8_t pin) {
  if (pin > 7)
  {
    _error = MCP23008_PIN_ERROR;
    return false;
  }
  //  disable interrupt
  uint16_t value = readReg(MCP23008_GPINTEN_REG);
  value &= ~(1 << pin);
  return writeReg(MCP23008_GPINTEN_REG, value);
}

uint8_t MCP23008::readInterruptFlagRegister() {
  return readReg(MCP23008_INTF_REG);
}


uint8_t MCP23008::readInterruptCaptureRegister() {
  return readReg(MCP23008_INTCAP_REG);
}

bool MCP23008::setInterruptPolarity(uint8_t polarity) {
  if (polarity > 2) return false;
  uint8_t reg = readReg(MCP23008_IOCON_REG);
  reg &= ~(MCP23008_IOCON_ODR | MCP23008_IOCON_INTPOL);
  //  LOW is default set.
  if (polarity == 2) reg |= MCP23008_IOCON_ODR;
  if (polarity == 1) reg |= MCP23008_IOCON_INTPOL;
  return writeReg(MCP23008_IOCON_REG, reg);
}

uint8_t MCP23008::getInterruptPolarity() {
  uint8_t reg = readReg(MCP23008_IOCON_REG);
  if (reg & MCP23008_IOCON_ODR) return 2;
  if (reg & MCP23008_IOCON_INTPOL) return 1;
  return 0;
}

int MCP23008::lastError() {
  int e = _error;
  _error = MCP23008_OK;  //  reset error after read.
  return e;
}

/* rename member functions
bool MCP23008::MCP23008::enableControlRegister(uint8_t mask) {
  uint8_t reg = readReg(MCP23008_IOCON_REG);
  reg |= mask;
  return writeReg(MCP23008_IOCON_REG, reg);
}

bool MCP23008::MCP23008::disableControlRegister(uint8_t mask) {
  uint8_t reg = readReg(MCP23008_IOCON_REG);
  reg &= ~mask;
  return writeReg(MCP23008_IOCON_REG, reg);
}
*/

uint8_t MCP23008::getPinMode8() {
  return readReg(MCP23008_IODIR_REG);
}

bool MCP23008::writeReg(uint8_t regAddress, uint8_t value) {
  _wire->beginTransmission(_address);
  _wire->write(regAddress);
  _wire->write(value);
  if (_wire->endTransmission() != 0)
  {
    _error = MCP23008_I2C_ERROR;
    return false;
  }
  _error = MCP23008_OK;
  return true;
}

uint8_t MCP23008::readReg(uint8_t regAddress) {
  _wire->beginTransmission(_address);
  _wire->write(regAddress);
  if (_wire->endTransmission() != 0)
  {
    _error = MCP23008_I2C_ERROR;
    return 0;
  }
  uint8_t n = _wire->requestFrom(_address, (uint8_t)1);
  if (n != 1)
  {
    _error = MCP23008_I2C_ERROR;
    return 0;
  }
  _error = MCP23008_OK;
  return _wire->read();
}
