

// ==============================================
// INCLUDE FILE HEADERS
#include <stdio.h>
#include <stdlib.h>        // Use for exit(0)

#include <string.h>
#include <math.h>

#include <bcm2835.h>

//#define HIGH(1)
//#define LOW(0)

int index_bits[256];
void init_bcm2835();
void close_bcm2835();
void set_pins();



void  drive_random(int valdrive);
time_t t;

// ================================================
int main(int argc, char *argv[]) {
    // ================================================
    printf("Bismillah 1 start Raspberry Pi jogging. \n\n");

    init_bcm2835();
    set_pins();

    //bcm2835_gpio_write(8,HIGH);
    direction_high(8);
    int i;
    for(i=0; i<100000; i++) {

        //bcm2835_gpio_write(7,HIGH);
        //delayMicroseconds (500 );
        //bcm2835_gpio_write(7,LOW);
        step(7);
    }
    //bcm2835_gpio_write(8,LOW);
    direction_low(8);
    for(i=0; i<100000; i++) {

        //bcm2835_gpio_write(7,HIGH);
        //delayMicroseconds (500 );
        //bcm2835_gpio_write(7,LOW);
        step(7);

    }
    close_bcm2835();

    printf("Alhamdulillah. finish Raspberry Pi jogging. \n\n");
    return(0);
}
// ================================================
void init_bcm2835() {
     printf("BISMILLAH. Program started. \n\n");

    if (!bcm2835_init()){
        printf("bcm2835_init()  - Intitionlization failed. \n");
        //return 1;
    } else {
        printf("bcm2835_init()  - Init SUCCESS. Alhamdulillah.  \n");
    }
}
// ================================================
void close_bcm2835() {
    //bcm2835_close();
    if (!bcm2835_close()) {
        printf("bcm2835 _close() - Closing failed. \n");
        // return 1;
    }  else {
        printf("bcm2835_close() - Closing SUCCESS. Alhamdulillah. \n");
    } // end if close()
}
void set_pins() {
    // Set the 8 pins as outputs based on the MAGIC NUMBER
    // printf("Set all selected 8 pins as outputs. Becomes MAGIC NUM. \n");
    bcm2835_gpio_fsel(7,  BCM2835_GPIO_FSEL_OUTP); //bit-1 X-Axis  GPIO (7)    RPi pin (26)
    bcm2835_gpio_fsel(8,  BCM2835_GPIO_FSEL_OUTP); //bit-2 X-Axis  GPIO (8)    RPi pin (24)
    bcm2835_gpio_fsel(9,  BCM2835_GPIO_FSEL_OUTP); //bit-3 Y-Axis  GPIO (9)    RPi pin (21)
    bcm2835_gpio_fsel(10, BCM2835_GPIO_FSEL_OUTP); //bit-4 Y-Axis  GPIO (10)   RPi pin (19)
    bcm2835_gpio_fsel(22, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(23, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(24, BCM2835_GPIO_FSEL_OUTP); //bit-5 Z-Axis  GPIO (24)    RPi pin (18)
    bcm2835_gpio_fsel(25, BCM2835_GPIO_FSEL_OUTP); //bit-6 Z-Axis  GPIO (25)    RPi pin (22)

    bcm2835_gpio_fsel(14, BCM2835_GPIO_FSEL_OUTP);
}

void direction_high(int set_pin){
    bcm2835_gpio_write(set_pin,1);
}

void direction_low(int set_pin){
    bcm2835_gpio_write(set_pin,0);
}


void step(int set_pin){
    bcm2835_gpio_write( set_pin,1);
    delayMicroseconds (3 );
    bcm2835_gpio_write(set_pin,0);
}



