#include "date.h"
#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "comhand.h"
/**
 * Function: setDate
 * ----------------------------
 * Prompts the user for what date
 * they want to set. The function 
 * accesses the day, month, and 
 * year registers. Needs to be in
 * the dd/mm/yyyy format.
 * 
 * @author Farhan Shahbaz
 */
void setDate(int day, int month, int year)
{
    //Disable the interupt
    cli();
    //converting from decimal to binary coded decimal 
    short int day_data = ((day / 10) << 4) + (day % 10); 
    //sending a request to write the day
    outb(0x70, 0x07);
    outb(0x71, day_data);
    int month_data = ((month / 10) << 4) + (month % 10);
    //sending request to write the month
    outb(0x70, 0x08);
    outb(0x71, month_data);
    int year_data = (year / 100); //gets first 2 digits
    year_data = ((year_data / 10) << 4) + (year_data % 10);
    outb(0x70, 0x32); //port for first two digits
    outb(0x71, year_data);
    year_data = (year % 100); //gets last 2 digits of year
    year_data = ((year_data / 10) << 4) + (year_data % 10);
    outb(0x70, 0x09); //port for last two digits
    outb(0x71, year_data);
    sti();
}

/**
 * Function: getDate
 * -----------------------
 * Displays the current date
 * in the format mm/dd/yyy
 * Default time zone is UTC.
 *
 * @author Farhan Shahbaz
 */
void getDate()
{
    int size = 2;
    //Day of the Month
    outb(0x70, 0x07); // Day of the month
    int day = inb(0x71);//reading the day of the month
    char dayArray[3]; 
    dayArray[0] = '0' + ((day & 0xf0) >> 4);
    dayArray[1] = '0' + (day & 0x0f);
    dayArray[2] = '\0';
    //Month
    outb(0x70, 0x08); //same thing but instead of  month
    int mon = inb(0x71);
    char monthArray[3];
    monthArray[0] = '0' + ((mon & 0xf0) >> 4);
    monthArray[1] = '0' + (mon & 0x0f);
    monthArray[2] = '\0';
    //Year
    //gets first two digits
    outb(0x70, 0x32);
    int year = inb(0x71);
    char yearArray[5];
    yearArray[0] = '0' + ((year & 0xf0) >> 4);
    yearArray[1] = '0' + (year & 0x0f);
    outb(0x70, 0x09);
    year = inb(0x71);
    yearArray[2] = '0' + ((year & 0xf0) >> 4);
    yearArray[3] = '0' + (year & 0x0f);
    yearArray[4] = '\0';
    sys_req(WRITE, DEFAULT_DEVICE, monthArray, &size);
    size = 1;
    sys_req(WRITE, DEFAULT_DEVICE, "/", &size);
    size = 2;
    sys_req(WRITE, DEFAULT_DEVICE, dayArray, &size);
    size = 1;
    sys_req(WRITE, DEFAULT_DEVICE, "/", &size);
    size = 2;
    sys_req(WRITE, DEFAULT_DEVICE, yearArray, &size);
}

/**
 * Function: parseDate
 * -------------------------
 * Parser is used to get
 * and seperate the digit
 * values from the '/' in
 * the date. Makes sure it
 * is in porper form.
 * 
 * @author Farhan Shahbaz
 */
void parseDate(char *date, int *m, int *d, int *y)
{
    date[2] = '\0';  
    *m = atoi(date); 
    date[5] = '\0';
    *d = atoi(date + 3); // +3 to skip didgits and terminating null chracter
    date[10] = '\0';
    *y = atoi(date + 6);
}


void setDateCall(){
   char dateSet[] = "Please enter the Date with proper format: mm/dd/yyyy\n";
   int month = 0;
   int day = 0;
   int year = 0;
   int dateSetSize = sizeof(dateSet) / sizeof(dateSet[0]);
   sys_req(WRITE, DEFAULT_DEVICE, dateSet, &dateSetSize);
   char userDate[15];
   int size = 14;
   memset(userDate, '\0', 15);
   sys_req(READ, DEFAULT_DEVICE, userDate, &size);
   if(strlen(userDate) != 10){
    printRed();
    char *error = "\nError: invalid input please try again with correct format: mm/dd/yyyy";
    int size = sizeof(error)/sizeof(error[0]);
    sys_req(WRITE, DEFAULT_DEVICE, error, &size);
    printGreen();
    return;
}
parseDate(userDate, &month, &day, &year);
if(month < 1 || month > 12){
    printRed();
    char *error = "\nError: invalid input for month. Make sure it is between 1 and 12";
    int size = sizeof(error)/sizeof(error[0]);
    sys_req(WRITE, DEFAULT_DEVICE, error, &size);
    printGreen();
}
else if(day < 1 || day > 31){
    printRed();
    char *error = "\nError: invalid input for day. Make sure it is between 1 and 31";
    int size = sizeof(error)/sizeof(error[0]);
    sys_req(WRITE, DEFAULT_DEVICE, error, &size);
    printGreen();
}
setDate(day, month, year);
}