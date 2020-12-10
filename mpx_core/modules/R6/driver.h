#ifndef DRIVER_H
#define DRIVER_H
// The file to house your r6 header.
// You need the have functions similar to the following
#define PIC_REG 0x20
#define PIC_EOI 0x20
#define PIC_MASK 0x21
#define IRQ_COM1 0x10

#define REG_RHR         0// recieve hold register
#define REG_THR         0// transmitter holding register
#define REG_LSB         0//This is LSB for baud rate
#define REG_MSB         1//This is MSB for baud rate
#define REG_IER         1//Interupt enable register
#define REG_IIR         2// Interupt identification register
#define REG_LCR         3//line control register
#define REG_MCR         4//modem control register
#define REG_LSR         5//line status register
#define REG_MSR         6//modem status register
#define REG_SCRATCH     7// scratch registers
/*!
+* enum for the possible dcb states.
*/
typedef enum status_t {
STATUS_IDLE, /*!< Port is idle */
STATUS_READING, /*!< Port is reading */
STATUS_WRITING /*!< Port is writing */
} status_t;
/*!
+* struct dcb represents a Device Control Block.
+* A dcb should exist for each COM port, but you can just use COM1
+* @param com_port the COM port. (You can omit this and just always use COM1)
+* @param port_open whether the COM is open.
+* @param e_flag whether the operation has completed (0 or 1).
+* @param status the different operations (IDLE, READ, WRITE).
+* @param buffer_ptr the buffer array to read into/write from.
+* @param count_ptr how many characters to read/write.
+* @param buffer_loc the current location we are reading/writing at.
+* @param byte_count the number of bytes that have been read/written so far.
*/
typedef struct dcb_s {
    int com_port;
    int port_open;
    int* e_flag;
    status_t status;
    char* buffer_ptr;
    int* count_ptr;
    char* buffer_loc;
    int byte_count;
    
} dcb_t;



/*!
+* pic_mask() masks so only the desired PIC interrupt is enabled or disabled.
+* @param enable 1 to enable or 0 to disable.
*/
void pic_mask(char enable);
/*!
+* disable_interrupts() disables all interrupts to device.
*/
void disable_interrupts();
/*!
+* enable_interrupts() enables interrupts to device.
*/
void enable_interrupts();
/*!
+* com_open() Opens the communication port.
+* @param e_flag event flag will be set to 1 if read/write
+* @param baud_rate the desired baud rate
+* @return Returns three possible error codes, or a 0 for successful
operation.
*/
int com_open(int* e_flag, int baud_rate);

/*!
+* com_close() Closes the communication port.
+* @return error code if port was not open, or a 0 for successful operation
*/
int com_close(void);
/*!
+* com_read() Reads the buffer from the port. Non-blocking.
+* @param buf_ptr buffer in which the read characters will be stored.
+* @param count_ptr the maximum number of bytes to read. After completion,
+* this will contain the number of characters read.
+* @return Returns four possible error codes, or a 0 for successful operation.
*/
int com_read(char* buf_ptr, int* count_ptr);
/*!
+* com_write() Writes the buffer to the port. Non-blocking.
+* @param buf_ptr buffer in which the characters to write are stored.
+* @param count_ptr the number of characters from the buffer to write.
+* @return Returns four possible error codes, or a 0 for successful operation.
*/
int com_write(char* buf_ptr, int* count_ptr);
/*!
+* serial_io() is the interrupt C routine for serial IO.
*/
void serial_io();
/*!
+* serial_write() provides interrupt routine for writing IO.
*/
void serial_write();
/*!
+* serial_read() provides interrupt routine for reading IO.
*/
void serial_read();
#endif