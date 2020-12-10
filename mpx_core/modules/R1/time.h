#ifndef _time_H
#define _time_H
#include <system.h>
date_time getTime();
void setTime(int hours_int, int min_int, int sec_int);
void parseTime(char *time, int *hh, int *mm, int *ss);
void setTimeCall();
int bcdToDec(int bcd);
#endif