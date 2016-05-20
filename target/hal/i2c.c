#include "i2c.h"
#include "io.h"
#include <msp430.h>

//PCONF(9, 7, OUL); /* MAG SCL */
//PCONF(9, 6, IN); /* MAG SDA */

#define SCL_BIT BIT7
#define SDA_BIT BIT6

#define SDA_OUT P9OUT
#define SDA_IN  P9IN
#define SDA_DIR P9DIR

#define SCL_OUT P9OUT
#define SCL_IN  P9IN
#define SCL_DIR P9DIR

// based on https://en.wikipedia.org/wiki/I%C2%B2C#Example_of_bit-banging_the_I.C2.B2C_Master_protocol
// Hardware-specific support functions that MUST be customized:
#define I2CSPEED 100
static void I2C_delay( void );
static void set_SCL( void ) { // Actively drive SCL signal high
	SCL_DIR &= ~SCL_BIT;
	SCL_OUT |= SCL_BIT;
}
static void clear_SCL( void ) { // Actively drive SCL signal low
	SCL_OUT &= ~SCL_BIT;
	SCL_DIR |= SCL_BIT;
}
static void set_SDA( void ) { // Actively drive SDA signal high
	SDA_DIR &= ~SDA_BIT;
	SDA_OUT |= SDA_BIT;
}
static void clear_SDA( void ) { // Actively drive SDA signal low
	SDA_OUT &= ~SDA_BIT;
	SDA_DIR |= SDA_BIT;
}
static uint8_t read_SCL( void ) {// Set SCL as input and return current level of line, 0 or 1
	set_SCL();
	return !!(SCL_IN & SCL_BIT);
} 
static uint8_t read_SDA( void ) { // Set SDA as input and return current level of line, 0 or 1
	set_SDA();
	return !!(SDA_IN & SDA_BIT);
}

static void arbitration_lost( void ) {
	
}

void hal_i2c_init(void) {
	PCONF(9, 7, INPU); /* MAG SCL */
	PCONF(9, 6, INPU); /* MAG SDA */
	set_SDA();
	set_SCL();
}

void hal_i2c_deinit(void) {
	clear_SDA();
	clear_SCL();
}


static uint8_t started = 0; // global data

static void i2c_start_cond( void ) 
{
  if( started ) 
  { 
    // if started, do a restart cond
    // set SDA to 1
    set_SDA();
    I2C_delay();

    while( read_SCL() == 0 ) 
    {  // Clock stretching
      // You should add timeout to this loop
    }

    // Repeated start setup time, minimum 4.7us
    I2C_delay();

  }

  if( read_SDA() == 0 ) 
  {
    arbitration_lost();
  }

  // SCL is high, set SDA from 1 to 0.
  clear_SDA();
  I2C_delay();
  clear_SCL();
  started = 1;

}

static void i2c_stop_cond( void )
{
  // set SDA to 0
  clear_SDA();
  I2C_delay();

  // Clock stretching
  while( read_SCL() == 0 ) 
  {
    // add timeout to this loop.
  }

  // Stop bit setup time, minimum 4us
  I2C_delay();

  // SCL is high, set SDA from 0 to 1
  set_SDA();
  I2C_delay();

  if( read_SDA() == 0 ) 
  {
    arbitration_lost();
  }

  I2C_delay();
  started = 0;

}

// Write a bit to I2C bus
static void i2c_write_bit( uint8_t bit ) 
{
  if( bit ) 
  {
    set_SDA();
  } 
  else 
  {
    clear_SDA();
  }

  // SDA change propagation delay
  I2C_delay();  

  // Set SCL high to indicate a new valid SDA value is available
  set_SCL();

  // Wait for SDA value to be read by slave, minimum of 4us for standard mode
  I2C_delay();

  while( read_SCL() == 0 ) 
  { // Clock stretching
    // You should add timeout to this loop
  }

  // SCL is high, now data is valid
  // If SDA is high, check that nobody else is driving SDA
  if( bit && ( read_SDA() == 0 ) )
  {
    arbitration_lost();
  }

  // Clear the SCL to low in preparation for next change
  clear_SCL();
}

// Read a bit from I2C bus
static uint8_t i2c_read_bit( void ) 
{
  uint8_t bit;

  // Let the slave drive data
  set_SDA();

  // Wait for SDA value to be written by slave, minimum of 4us for standard mode
  I2C_delay();

  // Set SCL high to indicate a new valid SDA value is available
  set_SCL();

  while( read_SCL() == 0 ) 
  { // Clock stretching
    // You should add timeout to this loop
  }

  // Wait for SDA value to be read by slave, minimum of 4us for standard mode
  I2C_delay();

  // SCL is high, read out bit
  bit = read_SDA();

  // Set SCL low in preparation for next operation
  clear_SCL();

  return bit;

}

// Write a byte to I2C bus. Return 0 if ack by the slave.
uint8_t hal_i2c_write_byte( uint8_t          send_start ,
                     uint8_t          send_stop  ,
                     uint8_t byte         ) 
{
  uint8_t     nack;

  if( send_start ) 
  {
    i2c_start_cond();
  }

  for(uint8_t bit = 0; bit < 8; bit++ ) 
  {
    i2c_write_bit( ( byte & 0x80 ) != 0 );
    byte <<= 1;
  }

  nack = i2c_read_bit();

  if (send_stop) 
  {
    i2c_stop_cond();
  }

  return nack;

}

// Read a byte from I2C bus
uint8_t hal_i2c_read_byte( uint8_t nack , uint8_t send_stop ) 
{
  uint8_t byte = 0;
  uint8_t bit;

  for( bit = 0; bit < 8; bit++ ) 
  {
    byte = ( byte << 1 ) | i2c_read_bit();
  }

  i2c_write_bit( nack );

  if( send_stop ) 
  {
    i2c_stop_cond();
  }

  return byte;

}

void I2C_delay( void ) 
{ 
  for(uint16_t i = 0; i < I2CSPEED / 2; i++ )
  {
    __no_operation();
  }

}
