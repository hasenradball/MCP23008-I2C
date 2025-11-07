/**
 * @file    MCP23008-I2c.cpp
 * @author  Frank Häfele
 * @date    27.12.2024
 * @version 1.1.0
 * @brief   MCP23008 Function and Class Definitions
 * @see     https://github.com/hasenradball/MCP23008-I2C
 * 
 */

#include "MCP23008-I2C.h"
#include "MCP23008-Constants.h"

using namespace MCP23008_I2C;
using namespace MCP23008_Constants;

MCP23008::MCP23008(uint8_t address, TwoWire *wire)
: _address{address}, _wire{wire}
{}

int8_t MCP23008::begin(bool inputPullUp) const {
  if (!isConnected()) {
    return MCP23008_ERROR_I2C;
  }
  if (inputPullUp) {
    // Force INPUT_PULLUP for all pins => write 0xFF
    return writeReg(MCP23008_GPPU_REG, 0xFF);
  }
  return MCP23008_STATE_OK;
}


int8_t MCP23008::isConnected() const {
  _wire->beginTransmission(_address);
  if (_wire->endTransmission() != 0) {
    return MCP23008_ERROR_I2C;
  }
  return 1;
}

/* #################################### */
/* ### --- single pin interface --- ### */
/* #################################### */
int MCP23008::setPinMode1(uint8_t pin, uint8_t mode) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  if ((mode != INPUT) && (mode != INPUT_PULLUP) && (mode != OUTPUT)) {
    return MCP23008_ERROR_VALUE;
  }

  int iodir = readReg(MCP23008_IODIR_REG);
  if (iodir < 0) {
    return iodir;
  }
  uint8_t mask = 1 << pin;
  // only work with valid
  if ((mode == INPUT) || (mode == INPUT_PULLUP)) {
    iodir |= mask;
  }
  else if (mode == OUTPUT) {
    iodir &= ~mask;
  }
  // other values won't change iodir
  return writeReg(MCP23008_IODIR_REG, iodir);
}


int MCP23008::write1(uint8_t pin, uint8_t value) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  int olat = readReg(MCP23008_OLAT_REG);
  if (olat < 0 ) {
    return olat;
  }
  int pre = olat;

  uint8_t mask = 1 << pin;
  if (value) {
    olat |= mask;
  }
  else {
    olat &= ~mask;
  }
  // only write when changed.
  if (pre != olat) {
    return writeReg(MCP23008_OLAT_REG, olat);
  }
  return MCP23008_STATE_OK;
}


int MCP23008::read1(uint8_t pin) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  int gpio = readReg(MCP23008_GPIO_REG);
  if (gpio < 0) {
    return gpio;
  }
  uint8_t mask = 1 << pin;
  if (gpio & mask) {
    return 1;
  }
  else {
    return 0;
  }
}


int MCP23008::setPolarity(uint8_t pin,  bool reversed) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  int ipol = readReg(MCP23008_IPOL_REG);
  if (ipol < 0) {
    return ipol;
  }
  uint8_t mask = 1 << pin;
  if (reversed) {
    ipol |= mask;
  }
  else {
    ipol &= ~mask;
  }
  return writeReg(MCP23008_IPOL_REG, ipol);;
}


int MCP23008::getPolarity(uint8_t pin) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  int ipol = readReg(MCP23008_IPOL_REG);
  if (ipol < 0) {
    return ipol;
  }
  uint8_t mask = 1 << pin;
  return (ipol & mask) > 0;
}

int MCP23008::setPullup(uint8_t pin, bool pullup) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  int gppu = readReg(MCP23008_GPPU_REG);
  if (gppu < 0) {
    return gppu;
  }
  uint8_t mask = 1 << pin;
  if (pullup) {
    gppu |= mask;
  }
  else {
    gppu &= ~mask;
  }
  return writeReg(MCP23008_GPPU_REG, gppu);
}


int MCP23008::getPullup(uint8_t pin) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  int gppu = readReg(MCP23008_GPPU_REG);
  if (gppu < 0) {
    return gppu;
  }
  uint8_t mask = 1 << pin;
  return (gppu & mask) > 0;
}

/* #################################### */
/* ### ---  8 - pin interface   --- ### */
/* #################################### */

int8_t MCP23008::setPinMode8(uint8_t mask) const {
  return writeReg(MCP23008_IODIR_REG, mask);
}

int MCP23008::getPinMode8() const {
  return readReg(MCP23008_IODIR_REG);
}

int8_t MCP23008::write8(uint8_t value) const {
  return writeReg(MCP23008_OLAT_REG, value);
}

int MCP23008::read8() const {
  return readReg(MCP23008_GPIO_REG);
}

int8_t MCP23008::setPolarity8(uint8_t mask) const {
  return writeReg(MCP23008_IPOL_REG, mask);
}

int MCP23008::getPolarity8() const {
  return readReg(MCP23008_IPOL_REG);
}

int8_t MCP23008::setPullup8(uint8_t mask) const {
  return writeReg(MCP23008_GPPU_REG, mask);
}

int MCP23008::getPullup8() const {
  return readReg(MCP23008_GPPU_REG);
}

int MCP23008::setInterrupt(uint8_t pin, uint8_t mode) const {
  int8_t state;
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  int8_t intcon = readReg(MCP23008_INTCON_REG);
  if (intcon < 0) {
    return intcon;
  }
  if (mode == CHANGE) {
    // compare to previous value.
    intcon &= ~(1 << pin);
  }
  else {
    int8_t defval = readReg(MCP23008_DEFVAL_REG);
    if (defval < 0) {
      return defval;
    }
    if (mode == RISING) {
      intcon |= (1 << pin);
      defval &= ~(1 << pin);  // RISING == compare to 0
    }
    else if (mode == FALLING) {
      intcon |= (1 << pin);
      defval |= ~(1 << pin);  // FALLING == compare to 1
    }
    if ((state = writeReg(MCP23008_DEFVAL_REG, defval)) < 0) {
      return state;
    }
  }
  if ((state = writeReg(MCP23008_INTCON_REG, intcon)) < 0) {
    return state;
  }

  // enable interrupt
  int8_t value = readReg(MCP23008_GPINTEN_REG);
  if (value < 0) {
    return value;
  }
  value |= (1 << pin);
  return writeReg(MCP23008_GPINTEN_REG, value);
}


int MCP23008::disableInterrupt(uint8_t pin) const {
  if (pin > 7) {
    return MCP23008_ERROR_PIN;
  }
  // disable interrupt
  int8_t reg = readReg(MCP23008_GPINTEN_REG);
  if (reg < 0) {
    return reg;
  }
  reg &= ~(1 << pin);
  return writeReg(MCP23008_GPINTEN_REG, reg);
}

int MCP23008::readInterruptFlagRegister() const {
  return readReg(MCP23008_INTF_REG);
}

int MCP23008::readInterruptCaptureRegister() const {
  return readReg(MCP23008_INTCAP_REG);
}

int MCP23008::setInterruptPolarity(uint8_t polarity) const {
  if (polarity > 2) {
    return MCP23008_ERROR_VALUE;
  }
  int reg = readReg(MCP23008_IOCON_REG);
  if (reg < 0) {
    return reg;
  }
  reg &= ~(MCP23008_IOCON_ODR | MCP23008_IOCON_INTPOL);
  //  LOW is default set
  if (polarity == 2) reg |= MCP23008_IOCON_ODR;
  if (polarity == 1) reg |= MCP23008_IOCON_INTPOL;
  return writeReg(MCP23008_IOCON_REG, reg);
}

int MCP23008::getInterruptPolarity() const {
  int reg = readReg(MCP23008_IOCON_REG);
  if (reg < 0) return reg;
  if (reg & MCP23008_IOCON_ODR) return 2;
  if (reg & MCP23008_IOCON_INTPOL) return 1;
  return 0;
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

int8_t MCP23008::writeReg(uint8_t regAddress, uint8_t value) const {
  _wire->beginTransmission(_address);
  _wire->write(regAddress);
  _wire->write(value);
  if (_wire->endTransmission() != 0) {
    return MCP23008_ERROR_I2C;
  }
  return MCP23008_STATE_OK;
}

int MCP23008::readReg(uint8_t regAddress) const {
  _wire->beginTransmission(_address);
  _wire->write(regAddress);
  if (_wire->endTransmission() != 0) {
    return MCP23008_ERROR_I2C;
  }
  uint8_t n = _wire->requestFrom(_address, (uint8_t)1);
  if (n != 1) {
    return MCP23008_ERROR_I2C;
  }
  return _wire->read();
}
