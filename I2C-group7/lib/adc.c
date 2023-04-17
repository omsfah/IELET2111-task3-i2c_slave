#include "adc.h"

uint16_t adcVal = 0;

void ADC0_init(PORT_SELECT port, uint8_t pin, ADC_MODE adc_mode) {
	/*What the cases does 
	case n:
	Disable digital input buffer;
	Disable digital input buffer;
	Disable pull-up resistor;
	break;
	
	
	uses memory positions for easier parametric manipulation
	*/
	switch(port) {
		case PORT_D:
			_SFR_MEM8(0x0470+pin) &= ~PORT_ISC_gm;
			_SFR_MEM8(0x0470+pin) |= PORT_ISC_INPUT_DISABLE_gc;
			_SFR_MEM8(0x0470+pin) &= ~PORT_PULLUPEN_bm;
			break;
		case PORT_E:
			_SFR_MEM8(0x0490+pin) &= ~PORT_ISC_gm;
			_SFR_MEM8(0x0490+pin) |= PORT_ISC_INPUT_DISABLE_gc;
			_SFR_MEM8(0x0490+pin) &= ~PORT_PULLUPEN_bm;
			break;
		case PORT_F:
			_SFR_MEM8(0x04B0+pin) &= ~PORT_ISC_gm;
			_SFR_MEM8(0x04B0+pin) |= PORT_ISC_INPUT_DISABLE_gc;
			_SFR_MEM8(0x04B0+pin) &= ~PORT_PULLUPEN_bm;
			break;
	}
	
	ADC0.CTRLC = ADC_PRESC_DIV4_gc;
	ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc;
	VREF.ADC0REF = VREF_REFSEL_VDD_gc;
	
	switch (adc_mode) {
		case SINGLE_CONVERSION_MODE:
			break;
		case FREERUN_MODE:
			ADC0.CTRLA |= ADC_FREERUN_bm;
			break;
	}
}

uint16_t ADC0_readSingle(PORT_SELECT port, uint8_t pin){
	ADC0.MUXPOS = ((port * 8 + pin)<<0);
	ADC0.COMMAND = ADC_STCONV_bm;
	
	while (!(ADC0.INTFLAGS & ADC_RESRDY_bm))
	{
		;
	}
	
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	
	return ADC0.RES;
}