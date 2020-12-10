#include "driver.h"
#include <core/serial.h>
#include <core/io.h>
#include "system.h"
#include "../mpx_supt.h"
#include "../R3/context.h"
#include "../R2/pcb.h"

// The file to house your r6 code.
// You need the have functions similar to the following
dcb_t *DCB; // the device representing the terminal.
void disable_interrupts()
{
    outb(COM1 + REG_IER, 0x00); //disable interrupts
}
//shouldnt this be REG_IER instead? Ask sam
void enable_interrupts()
{
    outb(COM1 + REG_MCR, 0x0B); //enable interrupts, rts/dsr set
}
void pic_mask(char enable)
{
    // If enable, do a logical NOT on the IRQ for COM1.
    // Obtain the mask by inb(the PIC_MASK register).
    // outb (PIC MASK register, (logical AND the mask with the irq from step 1))
    //assume this is correct for now, unsure
    if (enable)
    {
        char data = inb(PIC_MASK); //get PIC IMR
        data &= ~0x10;              //zero bit 4
        outb(PIC_MASK, data);      //write PIC IMR
    }else{
        char data = inb(PIC_MASK); //get PIC IMR
        data |= 0x10;              //disabling the pic interrupt
        outb(PIC_MASK, data);      //write PIC IMR
    }
}
// I think we finished com open come back if needed
int com_open(int *e_flag, int baud_rate)
{
    // Check the event flag is not null, the baud rate valid,and port is open.
    if (e_flag == NULL)
    {
        return -1;
    }
    if (baud_rate < 0 || baud_rate > 115200)
    { //115200 found in serial.c
        return -1;
    }
    // Set the status of the device to open and idle.
    // Set the event flag of the device to the one passed in
    DCB->com_port = COM1;
    DCB->e_flag = e_flag;
    DCB->status = STATUS_IDLE;
    DCB->buffer_loc = NULL;
    DCB->buffer_ptr = NULL;
    DCB->count_ptr = NULL;
    DCB->byte_count = 0;
    DCB->port_open = TRUE;

    // Disable your interrupts.
    disable_interrupts();
    // Set registers. Take a look at init_serial() in serial.c

    outb(DCB->com_port + REG_LCR, 0x80);          //set line control register
    outb(DCB->com_port + REG_LSB, 115200 / 9600); //set bsd least sig bit
    outb(DCB->com_port + REG_MSB, 0x00);          //brd most significant bit
    outb(DCB->com_port + REG_LCR, 0x03);          //lock divisor; 8bits, no parity, one stop
    outb(DCB->com_port + REG_IIR, 0xC7);          //enable fifo, clear, 14byte threshold

    // PIC mask enable
    pic_mask(TRUE);
    // Enable your interrupts.
    enable_interrupts();
    // Read a single byte to reset the port.
    (void)inb(DCB->com_port); //read bit to reset port
    return NO_ERROR;

}
int com_close(void)
{
    // Set the status of the device to closed
    DCB->port_open = FALSE;
    // Disable pic mask
    pic_mask(FALSE);
    // Disable interrupts
    disable_interrupts();
    return NO_ERROR;
}

int com_read(char *buf_ptr, int *count_ptr)
{
    // check port open, check valid pointer, check port is idle, etc.
    if(!DCB->port_open || DCB->status != STATUS_IDLE){
        return -1;
    }
    if(buf_ptr == NULL || count_ptr == NULL){   //Checks the paramters to see if they're null
        return -1;
    }

    // set dcb vars
    DCB->status = STATUS_READING;
    DCB->buffer_loc = buf_ptr;
    DCB->buffer_ptr = buf_ptr;
    DCB->count_ptr = count_ptr;
    DCB->byte_count = 0;

    // disable interrupts
    disable_interrupts();
    if(inb(DCB->com_port + REG_LSR) & 1){   // line status register. checking if lowest bit is one. that means data needs to be read
        DCB->buffer_ptr[DCB->byte_count] = inb(DCB->com_port); //reads store in buffer, increments byte count.
        DCB->byte_count++;  //byte count stores how many is read so far. 
    }    
    // read from ring buffer into the dcb buffer if there is anything
    // TODO: Implement ring buffer
    
    // enable interrupts
    enable_interrupts();
    // enable input ready interrupts
    char value = inb(DCB->com_port + REG_IER);
    value = value | 0x05; //0x05 will enable recieve data available and recive line register change
    outb(DCB->com_port + REG_IER, value); //using interuppt enable register 

    
    return NO_ERROR;
}
int com_write(char *buf_ptr, int *count_ptr)
{
    // check port open, check valid pointer, check port is idle, etc.
    if(!DCB->port_open || DCB->status != STATUS_IDLE){
        return -1;
    }
    if(buf_ptr == NULL || count_ptr == NULL){   //Checks the paramters to see if they're null
        return -1;
    }

    // set dcb vars
    DCB->status = STATUS_WRITING;
    DCB->buffer_loc = buf_ptr;
    DCB->buffer_ptr = buf_ptr;
    DCB->count_ptr = count_ptr;
    DCB->byte_count = 0;

    // disable interrupts
    disable_interrupts();

    // write a single byte to the device.
    outb(DCB->com_port, buf_ptr[DCB->byte_count]);
    DCB->byte_count++;
    DCB->buffer_loc = DCB->buffer_ptr + DCB->byte_count;
    // enable interrupts
    enable_interrupts();

    // enable write interrupts interrupts
    char value = inb(DCB->com_port + REG_IER);
    value = value | 0x02; //0x02 is to enable transmitter holding register empty (signal interrupt when done writing)
    outb(DCB->com_port + REG_IER, value); //using interuppt enable register

    

    return NO_ERROR;
}
//interupt hander for serial port, called when interupt 0x24 is generated. When single character is read/ transmitted
void serial_io()
{
    //klogv("serialio\n");
    // check port open.
    if(!DCB->port_open){
        return;
    }
    // obtain interrupt type. Call appropriate second level handler
    char value = inb(DCB->com_port + REG_IIR); //interuppt identification register
    if((value & 0x0E) == 0x04 && DCB->status == STATUS_READING){ //means we recived data (0x04)
        klogv("data recieved\n");
        //TODO: Implement second level handler to handle received data
        serial_read();
    }  
    if((value & 0x0E) == 0x02 && DCB->status == STATUS_WRITING){// Transmition completed 
        //klogv("data transmitted\n");
        serial_write();  
    }
    outb(PIC_REG, PIC_EOI); //*****Guess is that this line is not working****
    // Check if the event has completed. If so call io scheduler.
        if(*(DCB->e_flag) == 1){
            io_scheduler();
        }
    
}
void serial_write()
{
    // Ensure the dcb status is writing
    // If there are any more characters left in the buffer, print them
    if(DCB->byte_count< *(DCB->count_ptr)){
        disable_interrupts();
        //klogv("writing\n");
        // write a single byte to the device.
        outb(DCB->com_port, DCB->buffer_ptr[DCB->byte_count]);
        DCB->byte_count++;
        DCB->buffer_loc = DCB->buffer_ptr + DCB->byte_count;
        outb(PIC_REG, PIC_EOI);
        // enable interrupts
        enable_interrupts();
        char value = inb(DCB->com_port + REG_IER);
        value = value | 0x02; //0x02 is to enable transmitter holding register empty (signal interrupt when done writing)
        outb(DCB->com_port + REG_IER, value); //using interuppt enable register
     } else{
         DCB->status = STATUS_IDLE;
         *(DCB->e_flag) = 1; //writing  has completed
         char value = inb(DCB->com_port + REG_IER);
        value = value & ~0x02; //0x02 is to disable transmitter holding register empty (signal interrupt when done writing)
        outb(DCB->com_port + REG_IER, value); //using interuppt enable register
     }
     
     outb(PIC_REG, PIC_EOI);
    // Othewise we are done printing
    // Update the dcb status. Disable output interrupts
}
void serial_read()
{
    // Ensure the dcb status is reading. If not, push to the ring buffer.
    // Read a character from the COM port & add it to the buffer.
    if(DCB->byte_count < *(DCB->count_ptr)){
        disable_interrupts();
        //klogv("writing\n");
        // write a single byte to the device.
        if(inb(DCB->com_port + REG_LSR) & 1){   // line status register. checking if lowest bit is one. that means data needs to be read
            DCB->buffer_ptr[DCB->byte_count] = inb(DCB->com_port); //reads store in buffer, increments byte count.
            DCB->byte_count++;  //byte count stores how many is read so far. 

        }
        // enable interrupts
        enable_interrupts();
        char value = inb(DCB->com_port + REG_IER);
        value = value | 0x05; //0x05 is to enable transmitter holding register empty (signal interrupt when done writing)
        outb(DCB->com_port + REG_IER, value); //using interuppt enable register
     } else{
         DCB->status = STATUS_IDLE;
         *(DCB->e_flag) = 1; //reading  has completed
         char value = inb(DCB->com_port + REG_IER);
        value = value & ~0x05; //0x05 is to disable transmitter holding register empty (signal interrupt when done writing)
        outb(DCB->com_port + REG_IER, value); //using interuppt enable register
     }
     if(DCB->buffer_ptr[DCB->byte_count-1]== '\n'){
         DCB->buffer_ptr[DCB->byte_count-1] = '\0';
          DCB->status = STATUS_IDLE;
          *(DCB->e_flag) = 1; //reading  has completed
         char value = inb(DCB->com_port + REG_IER);
        value = value & ~0x05; //0x05 is to disable transmitter holding register empty (signal interrupt when done writing)
        outb(DCB->com_port + REG_IER, value); //using interuppt enable register
     }
    // If we reached a new line or the buffer size, we are done reading
    // Update the dcb status. Disable intput interrupts
}