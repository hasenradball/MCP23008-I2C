#pragma once

/**
 * @file    MCP23008-Constants.h
 * @author  Frank Häfele
 * @date    27.12.2024
 * @version 1.0.0
 * @brief   MCP23008 Constants and Register short Descriptions
 * @see     https://github.com/hasenradball/MCP23008-I2C
 * 
 */

/**
 * @brief Namespace of MCP23008 Constants.
 * Contains mainly the register description
 */
namespace MCP23008_Constants {
   
  /**
   * @brief I/O Direction Register Address (IODIR)
   * 
   */
  constexpr uint8_t MCP23008_IODIR_REG {0x00};

  /**
   * @brief Input Polarity Register (IPOL)
   * 
   */
  constexpr uint8_t MCP23008_IPOL_REG {0x01};

  /**
   * @brief Interrupt-On-Change Control Register (GPINTEN)
   * 
   */
  constexpr uint8_t MCP23008_GPINTEN_REG {0x02};

  /**
   * @brief Default Compare Register for Interrupt-On-Change 
   * 
   */
  constexpr uint8_t MCP23008_DEFVAL_REG {0x03};

  /**
   * @brief Interrupt Control Register (INTCON)
   * 
   */
  constexpr uint8_t MCP23008_INTCON_REG {0x04};

  /**
   * @brief Configuration Register (IOCON)
   * 
   */
  constexpr uint8_t MCP23008_IOCON_REG {0x05};

  /**
   * @brief Pull-Up Resistor Configuration Register (GPPU)
   * 
   */
  constexpr uint8_t MCP23008_GPPU_REG {0x06};

  /**
   * @brief Interrupt Flag Register (INTF)
   * 
   */
  constexpr uint8_t MCP23008_INTF_REG {0x07};

  /**
   * @brief Interrupt Capture Register (INTCAP)
   * 
   */
  constexpr uint8_t MCP23008_INTCAP_REG {0x08};

  /**
   * @brief Port Register (GPIO)
   * 
   */
  constexpr uint8_t MCP23008_GPIO_REG {0x09};

  /**
   * @brief Output Latch Register (OLAT)
   * 
   */
  constexpr uint8_t MCP23008_OLAT_REG {0x0A};


  /**
   * @brief The Sequential Operation (SEQOP) bit
   * 
   * The Sequential Operation (SEQOP) controls the
   * incrementing function of the Address Pointer. If
   * the Address Pointer is disabled, the Address
   * Pointer does not automatically increment after
   * each byte is clocked during a serial transfer. This
   * feature is useful when it is desired to continuously
   * poll (read) or modify (write) a register.
   */
  constexpr uint8_t MCP23008_IOCON_SEQOP {0x20};

  /**
   * @brief Slew Rate control bit for SDA output.
   * 
   * The Slew Rate (DISSLW) bit controls the slew
   * rate function on the SDA pin. If enabled, the SDA
   * slew rate will be controlled when driving from a
   * high to a low
   */
  constexpr uint8_t MCP23008_IOCON_DISSLW {0x10};

  /**
   * @brief The Open-Drain control bit (ODR)
   * 
   * The Open-Drain (ODR) control bit
   * enables/disables the INT pin for open-drain
   * configuration
   */
  constexpr uint8_t MCP23008_IOCON_ODR {0x04};

  /**
   * @brief The Input Polarity Control bit (INTPOL)
   * 
   * The Interrupt Polarity (INTPOL) control bit sets
   * the polarity of the INT pin. This bit is functional
   * only when the ODR bit is cleared, configuring the
   * INT pin as active push-pull.
   */
  constexpr uint8_t MCP23008_IOCON_INTPOL {0x02};

}