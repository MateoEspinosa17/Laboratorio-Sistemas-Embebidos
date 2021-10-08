/*
 * Practica6.c
 *
 * Created: 27/9/2021 12:07:16
 * Author : Mateo Espinosa
 */

#include "sam.h"

#define PINCFG_CONFIG_VALUE 0b00000000

int main(void) {

	/* Initialize the SAM system */
	SystemInit();
	
	//PORT configuration for general-purpose PIN
	PORT->Group[0].PINCFG[14].reg = PINCFG_CONFIG_VALUE;
	PORT->Group[0].DIRSET.reg = PORT_PA14;
	//TIMER configuration
	//Configure the POWER MANAGER to enable the TC3 module
	PM->APBCMASK.reg |= PM_APBCMASK_TC3;
	//Configure the GENERIC CLOCK CONTROL used by the TC3 module
	GCLK->GENDIV.reg = GCLK_GENDIV_RESETVALUE;
	GCLK->GENCTRL.reg = 0x10600;
	GCLK->CLKCTRL.reg = 0x401B;
	//Determine the initial count
	//Time = Toscillator * (2^TIMERMODE - COUNT) * Prescaler
	TC3->COUNT16.COUNT.reg = 65307;
	//Configure the TC3 peripheral
	TC3->COUNT16.CTRLA.reg = TC_CTRLA_RESETVALUE;
	TC3->COUNT16.CTRLBSET.reg = TC_CTRLBSET_OFFSET;
	TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;
	//Enabling the TC3 peripheral
	TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
	//Loop to sync the TC clock with the processor/main clock
	while((TC3->COUNT16.STATUS.bit.SYNCBUSY) == TC_STATUS_SYNCBUSY);
	
	
	while(1){
		
		if((TC3->COUNT16.INTFLAG.reg) != TC_INTFLAG_OVF){		
			PORT->Group[0].OUTTGL.reg = PORT_PA14;
			TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;
			TC3->COUNT16.COUNT.reg = 65307;
		}
	}
	
	return 0;
}