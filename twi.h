#ifndef TWI_HEADER
#define TWI_HEADER


#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
typedef uint8_t byte;
typedef uint32_t word;

#define TW_SCL_PIN			PORTC0
#define TW_SDA_PIN			PORTC1
#define TW_READ 1
#define TW_WRITE 0

void twi_init();
void twi_start();
void twi_repeat_start();
void twi_stop();
byte twi_MT_SLA_W(byte addr);
byte twi_MR_SLA_R(byte addr);
byte twi_MT_write(byte data);
byte twi_MR_read_ACK();
byte twi_MR_read_NACK();
byte twi_MT_SLA_W_check(byte addr);

#endif 