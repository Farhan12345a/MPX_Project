#ifndef _SERIAL_H
#define _SERIAL_H

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8


/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device);

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg);

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg);

/*
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device);

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device);

/*
   Procedure:  Polling
   Description:   Gathers keyboard input via the serial port using
   		   the technique of polling
*/   		   

/**
 * Function: polling 
 * --------------------
 * Fills the buffer with keyboard input and 
 * returns to command handler when enter key
 * is pressed. Also handles special characters
 *
 * @param *buffer: String buffer to be filled
 * @param *count: size of the buffer
 * @return the buffer size
 * @author Brendan Michael
 */
int *polling(char *buffer, int *count);

/**
 * Function: resetLine 
 * --------------------
 * Using escape sequences, moves
 * the cursor to the beginning 
 * of the line, clears it, and changes
 * the color to green.
 * @author Brendan Michael
 */
void resetLine();

/**
 * Function: resetCursor 
 * --------------------
 * Using escape sequences, moves
 * the cursor from the start of the
 * line up to the current location
 * in the buffer
 * @author Brendan Michael
 */
void resetCursor();

/**
 * Function: backspaceChar 
 * --------------------
 * Handles backspace characters.
 * Copies the current buffers content
 * up to the cursor location into a new
 * buffer and concatenates that with everything
 * past the cursor.
 *
 * @param *buffer: the buffer to be changed
 * @author Brendan Michael
 */
void backspaceChar();

/**
 * Function: letterChar 
 * --------------------
 * Adds a non-special character
 * to the buffer and adjusts buffer size
 * and index.
 * @author Brendan Michael
 */
void letterChar();

/**
 * Function: deleteChar
 * --------------------
 * Handles delete character.
 * Copies the current buffers content
 * up to the cursor location into a new
 * buffer and concatenates that with everything
 * past the cursor not including the final 
 * character.
 *
 * @param *buffer: the buffer to be changed
 * @author Brendan Michael
 */
void deleteChar();

/**
 * Function: righArrowChar
 * -----------------------
 * Moves the cursor one space to
 * the right and increments the buffer
 * index.
 *
 * @param *buffer: the buffer to be changed
 * @author Brendan Michael
 */
void rightArrowChar();

/**
 * Function: leftArrowChar
 * -----------------------
 * Moves the cursor one space to
 * the left and decrements the buffer
 * index.
 *
 * @param *buffer: the buffer to be changed
 * @author Brendan Michael
 */
void leftArrowChar();

int isEscape(char *letter);
int isBackspace(char *letter);
int isEnter(char *letter);
int isLetter(char *letter);
int isRightArrow(char *letter);
int isLeftArrow(char *letter);
int isDownArrow(char *letter);
int isUpArrow(char *letter);
void initializeHistory();
int addToHistory(char *command);
int historyFull();
char* getCommand();
int comEmpty();
void getHistory(char* buffer,char *letter);
char* getCommandUp();
char* getCommandDown();

#endif
