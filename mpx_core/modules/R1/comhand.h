#ifndef COMHAND_H 
#define COMHAND_H
#include <string.h>
#include <system.h>
//#include "stdlib.h"
//clock stuffss
// #define PROGRAM_STRING "Port70" 
// #define COPYRIGHT_STRING "Copyright 2008, Intel Corporation" 
// #define VERSION_NUMBER_STRING "2.0" 
// #define EXE_NAME "PORT70.EXE" 
// #include <conio.h>      // For clrscr(); 
// #include <stdlib.h>     // Needed for exit(0); 
// #include <stdio.h>      // Needed for printf 
// #include <string.h>     // Needed for strlen 
// #include <ctype.h>      // For toupper() function 
// #include "basedef.h" 
// #include "pci_acc.h"  // Needed for PCI Access Routines 
// #include "mem_acc.h"

/**
 * Function: comhand
 * -----------------------
 * Executes commands based on input gathered
 * from calling polling. Displays an error 
 * message if the entered command does not
 * exist.
 * 
 * @authors Brendan Michael, Selim Demircan,
 * Bryce Williams, Farhan Shahbaz
 */
int comhand();

/**
 * Function: poll
 * ---------------
 * Calls polling from within the
 * command handler.
 *
 * @author Brendan Michael
 */
void poll();

/**
 * Function: noSuchCommand
 * -----------------------
 * Displays an error message when an
 * entered command is invalid.
 *
 * @author Brendan Michael
 */
void deletePCBCall();
void noSuchCommand();
void createPCBC();
void printRed();
void printWhite();
void printGreen();
void resetBuffer();

#endif