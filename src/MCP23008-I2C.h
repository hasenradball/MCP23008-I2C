#pragma once

/**
 * @file    MCP23008-I2C.h
 * @author  Frank Häfele
 * @date    27.12.2024
 * @version 1.0.0
 * @brief   MCP23008 Declarations
 * @see     https://github.com/hasenradball/MCP23008-I2C
 * 
 */

#include "Arduino.h"
#include "Wire.h"

/**
 * @brief namespace of MCP23008.
 * includes class declaration, errors, and states 
 * 
 */
namespace MCP23008_I2C {

  constexpr const char *MCP23008_LIB_VERSION   {"1.0.0"};

  /**
   * @brief constant which states all ok, no error
   * 
   */
  constexpr int8_t MCP23008_STATE_OK           {0x00};
  
  /**
   * @brief constant which states a wrong pin number was used
   * 
   */
  constexpr int8_t MCP23008_ERROR_PIN          {-1};

  /**
   * @brief constant which states an error during I2C communication
   * 
   */
  constexpr int8_t MCP23008_ERROR_I2C          {-2};

  /**
   * @brief constant which states that there was an error regarding a parameter value
   * 
   */
  constexpr int8_t MCP23008_ERROR_VALUE        {-3};

  /**
   * @brief Class MCP23008
   * 
   */
  class MCP23008 {
    public:
      /**
       * @brief Construct a new MCP23008 object
       * 
       * @param address optional address of I2C device; default = 0x20;
       * @param wire optional address of Wire instance; default = \&Wire;
       */
      MCP23008(uint8_t address = 0x20, TwoWire *wire = &Wire);

      /**
       * @brief init MCP23008 instance
       * 
       * Check connection status and set Pull-up resistors if needed (by default).
       * 
       * @param inputPullUp optional force all inputs with Pull-up; default = true;
       * @return status of begin
       *
       * @retval  0: state OK
       * @retval <0: error code
       */
      int8_t begin(bool inputPullUp = true) const;

      /**
       * @brief check connection status
       * 
       * @return int8_t status of connection
       * 
       * @retval   1: connection OK
       * @retval  <0: error code
       */
      int8_t isConnected() const;

      /**
       * @brief Get the address of device
       * 
       * @return uint8_t address
       */
      uint8_t getAddress() const {return _address;}

      /* #################################### */
      /* ### --- single pin interface --- ### */
      /* #################################### */
      /* mode = INPUT, OUTPUT, INPUT_PULLUP (= same as in Arduino for INPUT)
       * DO NOT USE 0 or 1 for mode!
       */

      /**
       * @brief set pinMode of a single pin (IODIR)
       * 
       * @param pin pin number of pin 0...7
       * @param mode mode of pin (INPUT, INPUT_PULLUP, OUTPUT)
       * @return int status of write in IODIR register
       * 
       * @retval   0: state OK
       * @retval  <0: error code
       */
      int setPinMode1(uint8_t pin, uint8_t mode) const;

      /**
       * @brief write value for a single pin to OLAT register (OLAT)
       * 
       * The OLAT register provides access to the output
       * latches. A write to this register
       * modifies the output latches that modify the pins
       * configured as outputs.
       * @param pin pin number of pin 0...7
       * @param value to write 0/1
       * @return int statusof write in olat register
       * 
       * @retval   0: state OK
       * @retval  <0: error code
       */
      int write1(uint8_t pin, uint8_t value) const;

      /**
       * @brief read value for a single pin from GPIO register (GPIO)
       * 
       * The GPIO register reflects the value on the port.
       * Reading from this register reads the port.
       * @param pin pin number of pin 0...7
       * @return int status of gpio register for pin
       * 
       * @retval    0: pin is in LOW state
       * @retval    1: pin is in HIGH
       * @retval   <0: error code
       */
      int read1(uint8_t pin) const;

      /**
       * @brief Set the polarity of a single pin in the Input polarity register (IPOL)
       * 
       * If a bit is set, the corresponding GPIO register bit 
       * will reflect the inverted value on the pin.
       * 
       * @param pin pin number of pin 0...7
       * @param reversed true or false
       * @return int status
       * 
       * @retval   0: state OK
       * @retval  <0: error code
       */
      int setPolarity(uint8_t pin, bool reversed) const;

      /**
       * @brief Get the polarity of a single pin of Input polarity register (IPOL)
       * 
       * If a bit is set, the corresponding GPIO register bit 
       * will reflect the inverted value on the pin.
       * 
       * @param pin pin number of pin (0...7)
       * @return int status of polatity for pin
       * 
       * @retval  0: noninverted => GPIO pin will reflect the same logic state on input pin
       * @retval  1: inverted => GPIO pin will reflect the opposite logic state on input pin
       * @retval <0: error code
       */
      int getPolarity(uint8_t pin) const;

      /**
       * @brief Set the Pull-up register for on pin (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * 
       * @param pin pin number of pin 0...7
       * @param pullup set Pull-up true/false
       * @return int status
       * 
       * @retval  0: state OK
       * @retval <0: error code
       */
      int setPullup(uint8_t pin, bool pullup) const;

      /**
       * @brief Get the Pull-up register for one pin (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * 
       * @param pin pin number of pin (0...7)
       * @return int status of Pull-up for pin
       * 
       * @retval  0: Pull-up disabled
       * @retval  1: Pull-up enabled
       * @retval <0: error code
       */
      int getPullup(uint8_t pin) const;


      /* #################################### */
      /* ### ---  8 - pin interface   --- ### */
      /* #################################### */
      /* mask  = 0x00..0xFF bit pattern, 
       * 0 = output mode, 1 = input mode
       * value = bit pattern.
      */
      
      /**
       * @brief set mask for pinMode in I/O register for all pins at once (INTCON)
       * 
       * 1 = Pin is configured as an input
       * 
       * 0 = Pin is configured as an output
       * 
       * Bit pattern to set
       * in hex:     0x10
       * 
       * in binary:  0b00010000
       * 
       * in decimal: 16
       * @param mask bit mask to set
       * @return int status of write to I/O Register
       * 
       * @retval  0: state OK
       * @retval <0: error code
       */
      int8_t setPinMode8(uint8_t mask) const;

      /**
       * @brief Read I/O Direction register (IODIR)
       * 
       * @return state of I/O direction register
       * 
       * @retval >=0: register value
       * @retval  <0: error code
       */
      int getPinMode8() const;
      
      /**
       * @brief write 8-bit value at once to Output Latch register (OLAT)
       * 
       * The OLAT register provides access to the output
       * latches. A read from this register results in a read of the
       * OLAT and not the port itself. A write to this register
       * modifies the output latches that modify the pins
       * configured as outputs.
       * @param value value to write in hex, bin or decimal
       * @return int status of write to Output Latch register
       * 
       * @retval  0: state OK
       * @retval <0: error code
       */
      int8_t write8(uint8_t value) const;

      /**
       * @brief read 8 bit at once from GPIO register (GPIO)
       * 
       * The GPIO register reflects the value on the port.
       * Reading from this register reads the port.
       * @return int status value of GPIO register
       * 
       * @retval >=0: register value
       * @retval  <0: error code
       */
      int read8() const;

      /**
       * @brief Set the polarity in 8-bit at once in Input polarity register (IPOL)
       * If a bit is set, the corresponding GPIO register bit 
       * will reflect the inverted value on the pin.
       * @param mask to write
       * 
       * Bit pattern to set
       * in hex:     0x10
       * 
       * in binary:  0b00010000
       * 
       * in decimal: 16
       * @return int status
       * 
       * @retval  0: state OK
       * @retval <0: error code
       */
      int8_t setPolarity8(uint8_t mask) const;

      /**
       * @brief Get the polarity in 8-bit at once in Input polarity register (IPOL)
       * If a bit is set, the corresponding GPIO register bit 
       * will reflect the inverted value on the pin.
       * @return int status
       * 
       * @retval >=0: register value
       * @retval  <0: error code
       */
      int getPolarity8() const;

      /**
       * @brief Set Pull-up for all 8 pins at once (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * @param mask mask for Pull-up to set
       * @return int status
       * 
       * @retval  0: state OK
       * @retval <0: error code
       */
      int8_t setPullup8(uint8_t mask) const;

      /**
       * @brief Get Pull-up for all 8 pins at once (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * @return int status
       * 
       * @retval >=0: register value
       * @retval  <0: error code
       */
      int getPullup8() const;

      /**
       * @brief Set the Interrupt Control Register for specified pin (INTCON)
       * 
       * If a bit is set, the corresponding I/O pin is compared
       * against the associated bit in the DEFVAL register. If a
       * bit value is clear, the corresponding I/O pin is compared
       * against the previous value.
       * @param pin number of pin to set the interrupt (0...7)
       * @param mode mode of interrupt (RISING, FALLING, CHANGE)
       * @return int status
       * 
       * @retval   0: state OK
       * @retval  <0: error code
       */
      int setInterrupt(uint8_t pin, uint8_t mode) const;
      
      /**
       * @brief Disable interrupt on specified pin (INTCON)
       * 
       * @param pin number of pin to clear the interrupt (0...7)
       * @return int status
       * 
       * @retval  0: state OK
       * @retval <0: error code
       */
      int disableInterrupt(uint8_t pin) const;

      /**
       * @brief Read the Interrupt Flag Register (INTF)
       * 
       * The INTF register reflects the interrupt condition on the
       * PORT pins of any pin that is enabled for interrupts via
       * the GPINTEN register. A ‘set’ bit indicates that the
       * associated pin caused the interrupt.
       * @return int read state of interrupt flag register
       *
       * @retval >=0: register value
       * @retval  <0: error code
       */
      int readInterruptFlagRegister() const;

      /**
       * @brief Read the interrupt capture register (INTCAP)
       * 
       * @return int read state of interrupt capture register
       *
       * @retval >=0: register value
       * @retval  <0: error code
       */
      int readInterruptCaptureRegister() const;

      /**
       * @brief Set the Interrupt Polarity in IOCON Register
       * 
       * The Interrupt Polarity (INTPOL) control bit sets
       * the polarity of the INT pin. This bit is functional
       * only when the ODR bit is cleared, configuring the
       * INT pin as active push-pull.
       * 
       * 2 = Open-drain Output (ODR)
       * 1 = active-high
       * 0 = active-low
       * 
       * @param polarity value (2...0)
       * @return int status
       * 
       * @retval  0: state OK
       * @retval <0: error code
       */
      int setInterruptPolarity(uint8_t polarity) const;


      /**
       * @brief Read the Interrupt Polarity
       * 
       * @return int status
       * 
       * @retval  2: Opden-drain (ODR)
       * @retval  1: active-high
       * @retval  0: active-low
       * @retval <0: error code
       */
      int getInterruptPolarity() const;

      /* rename member functions set/clear IOCR bit fields
      bool     enableControlRegister(uint8_t mask);
      bool     disableControlRegister(uint8_t mask);
      */

    protected:
      /**
       * @brief I2C write value to MCP23008 register
       * 
       * @param regAddress address of specific register
       * @param value value to write
       * @return int8_t write status
       *
       * @retval =0: write OK
       * @retval <0: error code
       */
      int8_t writeReg(uint8_t regAddress, uint8_t value) const;

      /**
       * @brief I2c Read value of MCP23008 register
       * 
       * @param regAddress address of specific register
       * @return int read status
       *
       * @retval >=0: register value
       * @retval  <0: error code
       */
      int readReg(uint8_t regAddress) const;

      /**
       * @brief address of MCP23008 device
       * 
       */
      uint8_t _address;

      /**
       * @brief pointer of wire instance
       * 
       */
      TwoWire* _wire;
  };
}