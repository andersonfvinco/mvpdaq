#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> // Include WiringPi library!
#include <stdlib.h>
#include <stdbool.h>
const int butPin = 17; // Active-low button - Broadcom pin 17, P1 pin 11
char buf[1024];

bool isAPDisabled(){
	FILE *in = popen("ls /etc/netplan | grep AP.yaml.txt", "r");

   if(in == NULL)
	{
		perror("popen:");
		exit(EXIT_FAILURE);
	}

   // Write the output of the command to the console.
	while(fgets(buf, sizeof(buf), in) != NULL)
    //	printf("result: %s", buf);
      if(buf[0]==0){
	      printf("AP is enabled\n");
          return false;
      }
      else{
	      printf("AP is disabled\n");
          return true;
      }
}


int main(void)
{
    // Setup stuff:
    wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers

    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); // Enable pull-up resistor on button

    int count=0;
    while(1)
    {
        if (digitalRead(butPin))
        {
            printf("Button is not pressed\n");
            if(count!=0)
            {
                count=0;
            }
        }
        else 
        {
            printf("Button is pressed\n");
            count++;
            if(count==5 && isAPDisabled()){
                printf("restarting to pi AP mode ...\n");
                printf("changing file names ...\n");
                system("mv /etc/netplan/AP.yaml.txt /etc/netplan/AP.yaml");
                system("mv /etc/netplan/wifi_client.yaml /etc/netplan/wifi_client.yaml.txt");
                system("netplan apply");
            }
        }
        delay(1000);
    }

    return 0;
}
