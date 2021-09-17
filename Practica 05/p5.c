#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MODE_READ 0
#define MODE_WRITE 1
#define MAX_LEN 32

typedef enum {
    NO_ACTION,
    I2C_BEGIN,
    I2C_END
} i2c_init;

uint8_t  init = NO_ACTION;
uint16_t clk_div = 2502;
uint8_t slave_rtc = 0x00;
uint8_t slave_tc74 = 0x00;
uint32_t len = 8;

int i;
uint8_t data;
char buf[MAX_LEN];
char temp[MAX_LEN];
char wbuf[MAX_LEN];

int main(){
    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }
      
    // I2C begin if specified    
    if (init == I2C_BEGIN)
    {
      if (!bcm2835_i2c_begin())
      {
        printf("bcm2835_i2c_begin failed. Are you running as root??\n");
	return 1;
      }
    }

	slave_rtc = 104;
	slave_tc74 = 77;
    	
	bcm2835_i2c_setSlaveAddress(slave_rtc);

	
	fprintf(stderr, "len set to: %d\n", len);
	fprintf(stderr, "Slave address set to: %d\n", slave_rtc);

	//ESCRIBIR 01/01/01 12:00
	for (i=0; i<9; i++){
		if( i < 3) wbuf[i] = 0;
		else wbuf[i] = 1;  
	}
	wbuf[3] = 0x12;

	data = bcm2835_i2c_write(wbuf, len);
	
	//LEER RTC
	for (i=0; i<MAX_LEN; i++) buf[i] = 'n';
	data = bcm2835_i2c_write(wbuf, 1);		//para leer desde r0
	data = bcm2835_i2c_read(buf, len - 1);

	for (i=0; i<MAX_LEN; i++) {
    		if(buf[i] != 'n') printf("%x\n",buf[i]);
	}

	//LEER TC74
	bcm2835_i2c_setSlaveAddress(slave_tc74);

	for (i=0; i<MAX_LEN; i++) temp[i] = 'n';
	data = bcm2835_i2c_write(wbuf, 1);		//para leer desde r0
	data = bcm2835_i2c_read(temp, 1);

	for (i=0; i<MAX_LEN; i++) {
    		if(temp[i] != 'n') printf("temp: %x\n",temp[i]);
	}

	char t,dia,mes,year,diaS,hora,min,seg;
	char ampm[][2] = {"AM","PM"};
	char dias[][7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
		
	FILE *f = fopen("datalogs.txt", "w");
	
	while(1){					

		t = temp[0];	
		dia = buf[4];
		mes = buf[5];
		year = buf[6];
		diaS = buf[3];
		hora = buf[2];
		min = buf[1];
		seg = buf[0];
		char old = seg;	
		
		for (i=0; i<MAX_LEN; i++) buf[i] = 'n';		//para leer desde r0
		data = bcm2835_i2c_write(wbuf, 1);	
		data = bcm2835_i2c_read(buf, len - 1);
		char new = buf[0];

		bcm2835_i2c_setSlaveAddress(slave_tc74);

		for (i=0; i<MAX_LEN; i++) temp[i] = 'n';
		data = bcm2835_i2c_write(wbuf, 1);		//para leer desde r0
		data = bcm2835_i2c_read(temp, 1);

		for (i=0; i<MAX_LEN; i++) {
    			if(temp[i] != 'n') printf("temp: %x\n",temp[i]);
		}
		
		if(old == new || old == new + 10){
			printf("%x/%x/%x %s %x:%x:%x %s\n",dia,mes,year,dias[diaS - 1],hora,min,seg,ampm[1]);		
		}

		if(t > 30){
			printf("%x/%x/%x %s %x:%x:%x %s\n",dia,mes,year,dias[diaS - 1],hora,min,seg,ampm[1]);
			fprintf(f,"%x %x/%x/%x %s %x:%x:%x %s\n",t,dia,mes,year,dias[diaS - 1],hora,min,seg,ampm[1]);
		}	
	}
		
	return 0;
}



