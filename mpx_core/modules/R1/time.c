#include "time.h"
#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "comhand.h"
/**
 * Function: getTime
 * -----------------------
 * Displays the current time
 * in millitary format: hh:mm:ss.
 * Default time zone is UTC.
 *
 * @author Bryce Williams
 */
date_time getTime()
{
    date_time time;
    outb(0x70, 0x00); //seconds
    short int seconds = inb(0x71);

    time.sec = bcdToDec(seconds);
  
    outb(0x70, 0x02); //minutes
    short int minutes = inb(0x71);
    time.min = bcdToDec(minutes);
    outb(0x70, 0x04); //hours
    short int hour = inb(0x71);
    time.hour = bcdToDec(hour);
    return time;
}

/**
 * Function: setTime
 * ----------------------------
 * Prompts the user to enter 
 * time in the format hh:mm:ss
 * Changes the time in the intel
 * clock register to user set
 * time for displaying with 
 * getTime()
 * 
 * @author Bryce Williams
 */
void setTime(int hours_int, int min_int, int sec_int)
{
    cli(); //disables interupt

  //decimal to binary coded decimal conversion
    short int hh = ((hours_int / 10) << 4) + (hours_int % 10); 
    outb(0x70, 0x04); //request to write hours
    outb(0x71, hh); //write the hours
    
    short int mm = ((min_int / 10) << 4) + (min_int % 10); 
    outb(0x70, 0x02); //request to write
    outb(0x71, mm); //write

    short int ss = ((sec_int / 10) << 4) + (sec_int % 10);
    outb(0x71, ss);
    sti(); //enable interupt
           // getTime();
}

/**
 * Function: parseTime
 * ----------------------------------------------------------------------------
 * Parses user entered time
 * by replacing ':' array 
 * location to terminate
 * null string. atoi()
 * parses time until reaching
 * null character.
 * @param time the time as 
 * a character array in the format hh:mm:ss
 * @param hh hours parsed from time are stored in this variable
 * @param mm minutes parsed from time are stored in this variable
 * @param ss seconds parsed from time are stored in this variable
 * @author Bryce Williams
 */

void parseTime(char *time, int *hh, int *mm, int *ss)
{
    time[2] = '\0';   
    *hh = atoi(time); 
    time[5] = '\0';
    *mm = atoi(time + 3);
    time[8] = '\0';
    *ss = atoi(time + 6);
}

void setTimeCall(){
  char timeSet[] = "Please enter the time. hh:mm:ss\n";
  int timeSetSize = sizeof(timeSet) / sizeof(timeSet[0]);
  sys_req(WRITE, DEFAULT_DEVICE, timeSet, &timeSetSize);
  char userTime[10];
  int size = 9;
  memset(userTime, '\0', 10);
  sys_req(READ, DEFAULT_DEVICE, userTime, &size);
  if (strlen(userTime) != 8)
  {

    char errorMessage1[] = "\n\033[91mInvalid time format recieved.\n Make sure time is entered in the following format hh:mm:ss \n Try setTime again";
    int e1size = sizeof(errorMessage1) / sizeof(errorMessage1[0]);
    sys_req(WRITE, DEFAULT_DEVICE, errorMessage1, &e1size);
}
else
{
    int hh = 0;
    int mm = 0;
    int ss = 0;
    parseTime(userTime, &hh, &mm, &ss);
                
    if (hh < 0 || hh > 23)
    {
        char errorHH[] = "\n\033[91mThe input you provided for hh is invalid. hh valid values range between 0-23. \nTry setTime again";
        int errorHHsize = sizeof(errorHH) / sizeof(errorHH[0]);
        sys_req(WRITE, DEFAULT_DEVICE, errorHH, &errorHHsize);
        printGreen();
    }
                
    else if (mm < 0 || mm > 59)
    {
        char errorMM[] = "\n\033[91mThe input you provided for mm is invalid. mm valid values range between 0-59. \nTry setTime again";
        int errorMMsize = sizeof(errorMM) / sizeof(errorMM[0]);
        sys_req(WRITE, DEFAULT_DEVICE, errorMM, &errorMMsize);
        printGreen();
    }
                
    else if (ss < 0 || ss > 59)
    {
        char errorSS[] = "\n\033[91mThe input you provided for ss is invalid. ss valid values range between 0-59. \nTry setTime again";
        int errorSSsize = sizeof(errorSS) / sizeof(errorSS[0]);
        sys_req(WRITE, DEFAULT_DEVICE, errorSS, &errorSSsize);
        printGreen();
    }
    setTime(hh, mm, ss);
}
}
//binary coded decimal means every digit is coded with 4 bits. 
int bcdToDec(int bcd){
    int dec = 0;
    dec = ((bcd & 0xf0) >> 4) * 10;//selects upper 
    dec = dec + (bcd & 0x0f);
    return dec;
}