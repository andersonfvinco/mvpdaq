/*
 *  Autor: Guilherme Fardin 
 *  Date : 17/09/2024
 *    
 *  ADS1115 ADC read Pressure sensor differential AINp2 - AINn3
 *   
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>             // read/write usleep
#include <stdlib.h>             // exit function
#include <inttypes.h>           // uint8_t, etc
#include <linux/i2c-dev.h>      // I2C bus definitions
#include "ic.h" 		//influx.db client lib

int fd;

int ADS_address = 0x48;
int16_t val;
uint8_t writeBuf[3];
uint8_t readBuf[2];
float Pressure_mVolts;

const float VPS = 1.024 / 32768.0;                            // volts per step

int main() {
	int count;
        int i=10;
        char buf[300+1];
        char myhostname[256+1];

  	ic_influx_database("127.0.0.1", 8086, "pi_DB");
	ic_influx_userpw("pi", "pi@admin");
         /* get the local machine hostname */
        if( gethostname(myhostname, sizeof(myhostname)) == -1) {
                error("gethostname() failed");
        }
        snprintf(buf, 300, "host=%s", myhostname);
        ic_tags(buf);

  
  if ((fd = open("/dev/i2c-1", O_RDWR)) < 0)                  // open device on /dev/i2c-1 the default on Raspberry Pi B
  {
    printf("Error: Couldn't open device! %d\n", fd);
    exit (1);
  }

  
  if (ioctl(fd, I2C_SLAVE, ADS_address) < 0)                    // connect to ADS1115 as i2c slave
  {
    printf("Error: Couldn't find device on address!\n");
    exit (1);
  }

  writeBuf[0] = 1;                                              // Write in config register                   
  writeBuf[1] = 0b10110110;
  // bit 15 flag bit for single shot not used here
  // Bits 14-12 differential input selection AIN2-AIN3
  // Bits 11-9 Amp gain in 1.024V
  // Bit 8 Operational mode in continuous conversion.

  writeBuf[2] = 0b00000111; 
  // Bits 7-5 data rate default to 000 for 8SPS
  // Bits 4-0  comparator function default.

  
  if (write(fd, writeBuf, 3) != 3)                              // begin transmit I2C writing config register
  {
    perror("Write to register 1");
    exit (1);
  }

  sleep(0.2);

  readBuf[0] = 0;                                               // Write in conversion register, set pointer to 0
  if (write(fd, readBuf, 1) != 1) 
  {
    perror("Write register select");
    exit(-1);
  }
  
  
  while (1)   
  {

    if (read(fd, readBuf, 2) != 2)                              // Read conversion register
    {
      perror("Read conversion");
      exit(-1);
    }

    val = readBuf[0] << 8 | readBuf[1];                         // could also multiply by 256 then add readBuf[1]
                                                                // with +- LSB sometimes generates very low neg number.
    if (val < 0)   val = 0;

    Pressure_mVolts =( val * VPS)*1000;                                 // convert to voltage
    ic_measure("pressure_sensor");
    ic_long("raw_int", val); 
    ic_double("Pressure_mV", Pressure_mVolts);
    ic_measureend();
    ic_push();    
    printf("Conversion number HEX 0x%02x DEC %d %4.4f mVolts.\n",
           val, val, Pressure_mVolts);
     
    sleep(1);

  } // end while loop

  close(fd);

  return 0;
}//sudo gcc ads1115_c_code.c -o ads1115_c_code
