/*
 * 2023 (c) Simtec AG
 * All rights reserved
 * 
 * This simple programme decodes and prints messages received by an ARINC-TO-USB converter box from Simtec AG. 
 * The 32 bits ARINC-429 data word are converted in an proprietary format by the converter box. This programme
 * decodes this format.
 * 
 * Compiled and tested with MinGW (gcc)
 * http://sourceforge.net/projects/mingwbuilds/
 *
 * Example code only. Use at own risk.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * Simtec AG has no obligation to provide maintenance, support,
 * updates, enhancements, or modifications.
 */


#include "arinc_box_translator.h"
#include "serial.h"
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/** Default baudrate at which the serial port is read */
#define DEFAULT_BAUDRATE 230400

static void print_header()
{
    printf("\n");
    printf("A simple program that print messages received from an ARINC-TO-USB converter box. \n");
    printf("(c) 2023, Simtec AG\n");
    printf("\n");
}

static void print_help()
{
    printf("Read messages received through an ARINC-TO-USB converter box from SIMTEC AG \n");
    printf("and print them in the terminal in hexadecimal format. \n");
    printf("\n");
    printf("\n");

    printf("Usage: arinc_box_rx.exe serial-port [baudrate]\n");
    printf("Example: arinc_box_rx.exe COM5\n");
    printf("\n");
    printf("Arguments: \n");
    printf("\tserial-port: Virtual serial port on which the converter box is connected.\n");
    printf("\tbaudrate:    Set the baudrate. By default, 230400 is used. \n");
    printf("\n");
    printf("\n");
    printf("Other usage: arinc_box_rx.exe --help\n");
    printf("\tPrint this message");
    printf("\n");
    printf("\n");

    printf("See https://github.com/Simtec-AG/arinc-box-translator for more information \n");
    printf("\n");
    printf("\n");
}

int main(int argc, char **argv)
{
    int32_t return_code = EXIT_FAILURE;
    serial_port_t arinc_serial =
        {
            .baudrate = DEFAULT_BAUDRATE,
            .com_port = "\\\\.\\",
            .windows_handle = NULL};

    
    print_header();
    if (argc > 2)
    {
        arinc_serial.baudrate = strtol(argv[2], NULL, 10);
    }

    if ((argc > 1) && ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-help") == 0)))
    {
        print_help();
    }
    else if (argc > 1)
    {
        strcat(arinc_serial.com_port, argv[1]);

        if (serial_open(&arinc_serial) == EXIT_SUCCESS)
        {
            printf("Starting on %s @ B%d\n", arinc_serial.com_port, arinc_serial.baudrate);
            printf("Hit any key to exit\n\n");

            arinc_box_msg_t msg_in;
            while(!kbhit())
            {
                char data = 0;
                if(serial_get_byte(&arinc_serial, &data) == EXIT_SUCCESS)
                {
                    msg_in = arinc_box_decode(data);
                    if(msg_in.msg_type == ARINC_RETURNED_DATA)
                    {
                        printf("0x%08X\n", msg_in.data_value);
                    }
                    else if(msg_in.msg_type == ARINC_ERROR)
                    {
                        printf("Error decoding the message!\n");
                    }
                }
            }
            serial_close(&arinc_serial);
        }
        else
        {
            printf("Couldn't open %s", arinc_serial.com_port);
        }
    }
    else
    {
        printf("Error, The serial port needs to be passed as an argument! \n");
        printf("E.g.: COM1, COM2, ... \n\n");
        printf("See 'arinc_box_rx --help ' for more information. \n");
    }

    return return_code;
}
