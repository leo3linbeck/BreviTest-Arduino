/*
 * SoftI2CMaster.h -- Multi-instance software I2C Master library
 * 
 * 2010-2012 Tod E. Kurt, http://todbot.com/blog/
 *
 */

#ifndef SoftI2CMaster_h
#define SoftI2CMaster_h

#include <inttypes.h>

#define _SOFTI2CMASTER_VERSION 12  // software version of this library


class SoftI2CMaster
{

private:
  // per object data
  uint8_t _sdaPin;
  uint8_t _sclPin;
  uint8_t _sdaBitMask;
  uint8_t _sclBitMask;
  volatile uint8_t *_sdaPortReg;
  volatile uint8_t *_sclPortReg;
  volatile uint8_t *_sdaDirReg;
  volatile uint8_t *_sclDirReg;
  volatile uint8_t *_sclInputReg;

  uint8_t usePullups;
  
  // private methods
  void setPins(uint8_t sdaPin, uint8_t sclPin, uint8_t pullups);

  void i2c_writebit( uint8_t c );
  uint8_t i2c_readbit(void);
  void i2c_init(void);
  void i2c_start(void);
  void i2c_stop(void);
  uint8_t i2c_write( uint8_t c );
  uint8_t i2c_read( uint8_t ack );
  
public:
  // public methods
  SoftI2CMaster();
  SoftI2CMaster(uint8_t sdaPin, uint8_t sclPin);
  SoftI2CMaster(uint8_t sdaPin, uint8_t sclPin, uint8_t pullups);

  uint8_t beginWriteTransmission(uint8_t address, uint8_t reg);
  uint8_t beginWriteTransmission(int address, uint8_t reg);
  uint8_t write(uint8_t);
  void write(uint8_t*, uint8_t);
  uint8_t write(int);
  void write(char*);

  uint8_t beginReadTransmission(int address, uint8_t reg);
  uint8_t beginReadTransmission(uint8_t address, uint8_t reg);
  uint8_t read( uint8_t ack );
  uint8_t read();
  uint8_t readLast();

  uint8_t endTransmission(void);
};

#endif