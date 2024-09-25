#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> // Include WiringPi library!

const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11


int main(void)
{
    // Setup stuff:
    wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers

    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); // Enable pull-up resistor on button

    printf("Blinker is running! Press CTRL+C to quit.\n");

    while(1)
    {
        if (digitalRead(butPin)) // Button is released if this returns 1
        {
            printf("Button is not pressed\n");
        }
        else // If digitalRead returns 0, button is pressed
        {
            printf("Button is pressed\n");
        }
    }

    return 0;
}