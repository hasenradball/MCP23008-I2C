#pragma once
/**
 * @file    MCP23008.h
 * @author  Frank Häfele
 * @date    27.12.2024
 * @version 1.0.0
 * @brief   Arduino class for 8-channel port expander MCP23008
 * @see     tbd
 * 
 */

#include "Arduino.h"
#include "Wire.h"

/**
 * @brief namespace of MCP23008
 * 
 */
namespace MCP23008 {
  #define MCP23008_LIB_VERSION              (F("1.0.0"))

  constexpr uint8_t MCP23008_OK             {0x00};
  constexpr uint8_t MCP23008_PIN_ERROR      {0x81};
  constexpr uint8_t MCP23008_I2C_ERROR      {0x82};
  constexpr uint8_t MCP23008_VALUE_ERROR    {0x83};
  constexpr uint8_t MCP23008_PORT_ERROR     {0x84};
  constexpr uint8_t MCP23008_REGISTER_ERROR {0xFF};
  constexpr uint8_t MCP23008_INVALID_READ   {0xFF};


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
       * @param inputPullUp optional force all inputs with pullup; default = true;
       * @return true 
       * @return false 
       */
      bool begin(bool inputPullUp = true);

      /**
       * @brief check connection status
       * 
       * @return true 
       * @return false 
       */
      bool isConnected();

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
       * @return true 
       * @return false 
       */
      bool pinMode1(uint8_t pin, uint8_t mode);

      /**
       * @brief write value for a single pin to OLAT register (OLAT)
       * 
       * The OLAT register provides access to the output
       * latches. A write to this register
       * modifies the output latches that modify the pins
       * configured as outputs.
       * @param pin pin number of pin 0...7
       * @param value to write 0/1
       * @return true 
       * @return false 
       */
      bool write1(uint8_t pin, uint8_t value);

      /**
       * @brief read value for a single pin from GPIO register (GPIO)
       * 
       * The GPIO register reflects the value on the port.
       * Reading from this register reads the port.
       * @param pin pin number of pin 0...7
       * @return uint8_t value of pin 0/1
       */
      uint8_t read1(uint8_t pin);

      /**
       * @brief Set the polarity of a single pin in the Input polarity register (IPOL)
       * 
       * If a bit is set, the corresponding GPIO register bit 
       * will reflect the inverted value on the pin.
       * 
       * @param pin pin number of pin 0...7
       * @param reversed true or false
       * @return true 
       * @return false 
       */
      bool setPolarity(uint8_t pin, bool reversed);

      /**
       * @brief Get the polarity of a single pin of Input polarity register (IPOL)
       * 
       * If a bit is set, the corresponding GPIO register bit 
       * will reflect the inverted value on the pin.
       * 
       * @param pin pin number of pin 0...7
       * @param reversed reference of data memory to hold the value
       * @return true 
       * @return false 
       */
      bool getPolarity(uint8_t pin, bool &reversed);

      /**
       * @brief Set the Pullup register for on pin (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * 
       * @param pin pin number of pin 0...7
       * @param pullup set pullup true/false
       * @return true 
       * @return false 
       */
      bool setPullup(uint8_t pin, bool pullup);

      /**
       * @brief Get the Pullup register for one pin (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * 
       * @param pin pin number of pin 0...7
       * @param pullup reference of memory to hold the value
       * @return true 
       * @return false 
       */
      bool getPullup(uint8_t pin, bool &pullup);


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
       * outputMode = 0
       * 
       * inputMode = 1
       * 
       * Bit pattern to set
       * in hex:     0x10
       * 
       * in binary:  0b00010000
       * 
       * in decimal: 16
       * @param mask bit mask to set
       * @return true 
       * @return false 
       */
      bool pinMode8(uint8_t mask);

      /**
       * @brief write 8-bit value at once to Output Latch register (OLAT)
       * 
       * The OLAT register provides access to the output
       * latches. A read from this register results in a read of the
       * OLAT and not the port itself. A write to this register
       * modifies the output latches that modify the pins
       * configured as outputs.
       * @param value value to write in hex, bin or decimal
       * @return true 
       * @return false 
       */
      bool write8(uint8_t value);

      /**
       * @brief read 8 bit at once from GPIO register (GPIO)
       * 
       * The GPIO register reflects the value on the port.
       * Reading from this register reads the port.
       * @return int value of output latch register
       */
      int read8();

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
       * @return true 
       * @return false 
       */
      bool setPolarity8(uint8_t mask);

      /**
       * @brief Get the polarity in 8-bit at once in Input polarity register (IPOL)
       * If a bit is set, the corresponding GPIO register bit 
       * will reflect the inverted value on the pin.
       * @param mask memory to store the value
       * @return true 
       * @return false 
       */
      bool getPolarity8(uint8_t &mask);

      /**
       * @brief Set pullUp for all 8 pins at once (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * @param mask mask for pullUp to set
       * @return true 
       * @return false 
       */
      bool setPullup8(uint8_t mask);

      /**
       * @brief Get pullUp for all 8 pins at once (GPPU)
       * 
       * If a bit is set and the corresponding pin is
       * configured as an input, the corresponding PORT pin is
       * internally pulled up with a 100 kOhm resistor.
       * @param mask 
       * @return true 
       * @return false 
       */
      bool getPullup8(uint8_t &mask);

      /**
       * @brief Set the Interrupt Control Register for specified pin (INTCON)
       * 
       * If a bit is set, the corresponding I/O pin is compared
       * against the associated bit in the DEFVAL register. If a
       * bit value is clear, the corresponding I/O pin is compared
a      * gainst the previous value.
       * @param pin number of pin to set the interrupt (0...7)
       * @param mode mode of interrupt (RISING, FALLING, CHANGE)
       * @return true 
       * @return false 
       */
      bool setInterrupt(uint8_t pin, uint8_t mode);
      
      /**
       * @brief Disable interrupt on specified pin (INTCON)
       * 
       * @param pin 
       * @return true number of pin to clear the interrupt (0...7)
       * @return false 
       */
      bool disableInterrupt(uint8_t pin);

      /**
       * @brief Read the Interrupt Flag Register (INTF)
       * 
       * The INTF register reflects the interrupt condition on the
       * PORT pins of any pin that is enabled for interrupts via
       * the GPINTEN register. A ‘set’ bit indicates that the
       * associated pin caused the interrupt.
       * @return uint8_t value of register
       */
      uint8_t readInterruptFlagRegister();

      /**
       * @brief Read the interrupt capture register (INTCAP)
       * 
       * @return uint8_t value of interrupt capture register
       */
      uint8_t readInterruptCaptureRegister();

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
       * @return true 
       * @return false 
       */
      bool setInterruptPolarity(uint8_t polarity);


      /**
       * @brief Read the Interrupt Polarity
       * 
       * @return uint8_t 2 = ODR; 1 = active-high; 0 = active-low
       */
      uint8_t getInterruptPolarity();

      /**
       * @brief Get last error for debugging
       * 
       * @return int error value
       */
      int lastError();

      /* rename member functions set/clear IOCR bit fields
      bool     enableControlRegister(uint8_t mask);
      bool     disableControlRegister(uint8_t mask);
      */

      /**
       * @brief Read I/O Direction register (IODIR)
       * 
       * @return value of register
       */
      uint8_t  getPinMode8();


    protected:
      /**
       * @brief I2C write value to MCP23008 register
       * 
       * @param regAddress address of specific register
       * @param value value to write
       * @return true 
       * @return false 
       */
      bool writeReg(uint8_t regAddress, uint8_t value);

      /**
       * @brief I2c Read value of MCP23008 register
       * 
       * @param regAddress address of specific register
       * @return uint8_t value of register read
       */
      uint8_t readReg(uint8_t regAddress);

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

      /**
       * @brief memory for last error
       * 
       */
      uint8_t _error {MCP23008_OK};
  };
}
