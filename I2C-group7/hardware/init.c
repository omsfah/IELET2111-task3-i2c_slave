#include "init.h"


void DIP4_init(void) {
    /* Initialize the 4 bit DIP switch in pull up configuration */
    PORTC.DIRCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
    PORTC.PINCONFIG = PORT_PULLUPEN_bm;
    PORTC.PINCTRLUPD = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
}

