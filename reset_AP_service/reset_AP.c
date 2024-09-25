#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> // Include WiringPi library!
#include <stdlib.h>

const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11


int main(void)
{
    // Setup stuff:
    wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers

    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); // Enable pull-up resistor on button

    printf("Blinker is running! Press CTRL+C to quit.\n");
    int count=0;
    while(1)
    {
        if (digitalRead(butPin)) // Button is released if this returns 1
        {
            printf("Button is not pressed\n");
            if(count!=0)
            {
                count=0;
            }
        }
        else // If digitalRead returns 0, button is pressed
        {
            printf("Button is pressed\n");
            count++;
            if(count==5){
                //first step here is to verify which file is enabled
                printf("restarting to pi AP mode ...\n");
                printf("changing file names ...\n");
                system("sudo mv /etc/netplan/AP.yaml.txt /etc/netplan/AP.yaml");
                system("sudo mv /etc/netplan/wifi_client.yaml /etc/netplan/wifi_client.yaml.txt");
                system("sudo netplan apply");
            }
        }
        delay(1000);
    }

    return 0;
}