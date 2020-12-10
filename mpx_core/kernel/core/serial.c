/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>
#include <core/io.h>
#include <core/serial.h>
#include <../modules/mpx_supt.h>

#define NO_ERROR 0
#define NEWLINE 10
#define ENTER 13
#define DELETE 8
#define DOWN_ARROW 66
#define ESCAPE 27
#define RIGHT_ARROW 67
#define LEFT_ARROW 68
#define LEFT_BRACKET 91
#define UP_ARROW 65
#define BACKSPACE 127
#define MV_CURSOR_START "\033[1000D"
#define MV_CURSOR_RIGHT "\033[C"
#define MV_CURSOR_LEFT "\033[D"
#define CLEAR_LINE "\033[2K"
#define PRINT_GREEN "\033[92m"

#define MAX_SIZE 20
#define UP "\033[A"
#define DOWN "\033[B"
#define max 99




int bufferSize;
int bufferIndex;
int serial_port_out = 0;
int serial_port_in = 0;
char commands[21][31];
int size;
int index;

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device)
{
  outb(device + 1, 0x00);          //disable interrupts
  outb(device + 3, 0x80);          //set line control register
  outb(device + 0, 115200 / 9600); //set bsd least sig bit
  outb(device + 1, 0x00);          //brd most significant bit
  outb(device + 3, 0x03);          //lock divisor; 8bits, no parity, one stop
  outb(device + 2, 0xC7);          //enable fifo, clear, 14byte threshold
  outb(device + 4, 0x0B);          //enable interrupts, rts/dsr set
  (void)inb(device);               //read bit to reset port
  return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg)
{
  int i;
  for (i = 0; *(i + msg) != '\0'; i++)
  {
    outb(serial_port_out, *(i + msg));
  }
  outb(serial_port_out, '\r');
  outb(serial_port_out, '\n');
  return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg)
{
  int i;
  for (i = 0; *(i + msg) != '\0'; i++)
  {
    outb(serial_port_out, *(i + msg));
  }
  if (*msg == '\r')
    outb(serial_port_out, '\n');
  return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device)
{
  serial_port_out = device;
  return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device)
{
  serial_port_in = device;
  return NO_ERROR;
}

//fix backspace and delete

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
int *polling(char *buffer, int *count)
{
  bufferSize = 0;
  bufferIndex = 0;
  while (1)
  {
    char letter[2];
    letter[1] = '\0';
  //read line status register and check the first bit, which check if the data is available
    if (inb(COM1 + 5) & 1) // if that bit is set, data is available
    {
      //if data is avaiable read it from the serial port
      letter[0] = inb(COM1);
      if (isLetter(letter))
      {
        //storing char into buffer
        letterChar(buffer, letter);
      }
      //set count pointer to buffer size, the number of character that were read
      else if (isEnter(letter))
      {
        *count = bufferSize;
        return 0;
      }
      else if (isBackspace(letter))
      {
        backspaceChar(buffer);
      }
      else if (isEscape(letter))
      {
        letter[0] = inb(COM1);
        if (isRightArrow(letter))
        {
          rightArrowChar();
        }
        else if (isLeftArrow(letter))
        {
          leftArrowChar();
        }
        else if (isUpArrow(letter))
        {
          getHistory(buffer, letter);
        }
        else if (isDownArrow(letter))
        {
          getHistory(buffer, letter);
        }
        else
        {
          deleteChar(buffer);
        }
      }
      resetLine();
      serial_print(buffer);
      resetCursor();
    }
  }
}

/**
 * Function: resetLine 
 * --------------------
 * Using escape sequences, moves
 * the cursor to the beginning 
 * of the line, clears it, and changes
 * the color to green.
 * @author Brendan Michael
 */
void resetLine()
{
  serial_print(MV_CURSOR_START);
  serial_print(CLEAR_LINE);
  serial_print(PRINT_GREEN);
}

/**
 * Function: resetCursor 
 * --------------------
 * Using escape sequences, moves
 * the cursor from the start of the
 * line up to the current location
 * in the buffer
 * @author Brendan Michael
 */
void resetCursor()
{
  serial_print(MV_CURSOR_START);
  int i;
  for (i = 0; i < bufferIndex; i++)
  {
    serial_print(MV_CURSOR_RIGHT);
  }
}

void letterChar(char *buffer, char *letter)
{
  buffer[bufferIndex] = letter[0];
  bufferIndex++;
  bufferSize++;
}

void deleteChar(char *buffer)
{
  if (bufferIndex >= 1)
  {
    char newbuffer[bufferSize];
    int newbufferIndex = 0;
    int i, k;
    memset(newbuffer, '\0', sizeof(bufferSize));
    for (i = 0; i <= bufferIndex; i++)
    {
      newbuffer[i] = buffer[i];
      newbufferIndex++;
    }

    for (k = bufferIndex + 2; buffer[k] != '\0'; k++)
    {
      newbuffer[newbufferIndex] = buffer[k];
      newbufferIndex++;
    }

    newbuffer[bufferSize] = '\0';
    bufferIndex--;
    bufferSize--;

    memset(buffer, '\0', sizeof(bufferSize));

    strcpy(buffer, newbuffer);
  }
}

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
void backspaceChar(char *buffer)
{
  if (bufferIndex >= 1)
  {
    char newbuffer[bufferSize];
    int newbufferIndex = 0;
    int i, k;
    memset(newbuffer, '\0', sizeof(bufferSize));
    for (i = 0; i < bufferIndex - 1; i++)
    {
      newbuffer[i] = buffer[i];
      newbufferIndex++;
    }
    for (k = bufferIndex; buffer[k] != '\0'; k++)
    {
      newbuffer[newbufferIndex] = buffer[k];
      newbufferIndex++;
    }

    newbuffer[bufferSize] = '\0';
    bufferIndex--;
    bufferSize--;

    memset(buffer, '\0', sizeof(bufferSize));
    strcpy(buffer, newbuffer);
  }
}

void rightArrowChar()
{
  if (bufferIndex != bufferSize)
  {
    serial_print(MV_CURSOR_RIGHT);
    bufferIndex++;
  }
}

void leftArrowChar()
{
  if (bufferIndex > 0)
  {
    serial_print(MV_CURSOR_LEFT);
    bufferIndex--;
  }
}

int isUpArrow(char *letter)
{
  if (letter[0] == UP_ARROW)
  {
    return 1;
  }
  else
    return 0;
}

int isDownArrow(char *letter)
{
  if (letter[0] == DOWN_ARROW)
  {
    return 1;
  }
  else
    return 0;
}

int isRightArrow(char *letter)
{
  if (letter[0] == RIGHT_ARROW)
  {
    return 1;
  }
  else
    return 0;
}
int isLeftArrow(char *letter)
{
  if (letter[0] == LEFT_ARROW)
  {
    return 1;
  }
  else
    return 0;
}
int isEscape(char *letter)
{
  if (letter[0] == LEFT_BRACKET)
  {
    return 1;
  }
  else
    return 0;
}
int isBackspace(char *letter)
{
  if (letter[0] == BACKSPACE)
  {
    return 1;
  }
  else
    return 0;
}

int isEnter(char *letter)
{

  if (letter[0] == ENTER && bufferSize != 0)
  {
    return 1;
  }
  return 0;
}

int isLetter(char *letter)
{
  if (letter[0] >= 32 && letter[0] <= 122 && letter[0] != LEFT_BRACKET)
  {
    return 1;
  }
  else
    return 0;
}

void initializeHistory()
{
  //memset(commands, '\0', 21);
  size = 0;
  index = 0;
}

int addToHistory(char *command)
{
  if (comEmpty())
  {
    initializeHistory();
  }
  if (!historyFull())
  {
    if (strlen(command) >= 35)
    {
      return 0;
    }
    else
    {
      //   char * added = " added to hist";
      //     serial_print(added);
      strcpy(commands[index], command);
      //    char* insert = commands[index];
      //      serial_print(insert);
      index++;
      size++;
      return 1;
    }
  }
  return 0;
}

int comEmpty()
{
  return size == 0;
}

void getHistory(char *buffer, char *letter)
{
  if (letter[0] == 65)
  {
    char *response = getCommandUp();
    bufferIndex = strlen(response);
    memset(buffer, '\0', max);
    strcpy(buffer, response);
  }
  else
  {
    //    char *response = getCommandDown();
    //   serial_print(response);
  }
  //}
}

char *getCommandUp()
{
  //  if(!comEmpty()){
  if (index != 0)
  {
    char *response = commands[--index];
    return response;
  }
  else
  {
    char *r = commands[index];
    return r;
  }
  //  }
  return "none";
}

char *getCommandDown()
{
  if (!comEmpty())
  {
    if (commands[index + 1] == '\0')
    {
      return commands[++index];
    }
    else
    {
      return "none";
    }
  }
  return "none";
}

int historyFull()
{
  return size >= MAX_SIZE;
}
