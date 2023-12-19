/*
 * 2023 (c) Simtec AG
 * All rights reserved
 * 
 * This simple programme send 32 bits messages entered by the the user to the ARINC-TO-USB converter 
 * box from Simtec AG. The 32 bits values are converted in a custom format so that they will be
 * properly decoded by the ARINC-TO-USB converter box and correctly transmitted in the ARINC-429
 * protocol.
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
    printf("A simple terminal program to send messages to an ARINC-TO-USB converter box. \n");
    printf("The 32 bits values can be enter as an octal, decimal values or hexadecimal number. \n");
    printf("Enter '0' or any letter character to exit. \n");
    printf("(c) 2023, Simtec AG\n");
    printf("\n");
}

static void print_help()
{
    printf("Send 32 bits values to an ARINC-TO-USB converter box from SIMTEC AG. \n");
    printf("Those 32 bits will be converted and sent in a ARINC-429 data word by the converter box. \n");
    printf("The 32 bits can be entered in octal, decimal or hexadecimal format.\n");
    printf("\t The prefix '0x' or '0X' indicates an hexadecimal number\n");
    printf("\t The prefix '0' indicates an octal number\n");
    printf("\t If no prefix is present, then a decimal number is awaited\n");
    printf("\n");
    printf("Enter '0' or any letter character to exit the program. \n");
    printf("\n");
    printf("\n");

    printf("Usage: arinc_box_tx.exe serial-port [baudrate]\n");
    printf("Example: arinc_box_tx.exe COM5\n");
    printf("\n");
    printf("Arguments: \n");
    printf("\tserial-port: Virtual serial port on which the converter box is connected.\n");
    printf("\tbaudrate:    Set the baudrate. By default, 230400 is used. \n");
    printf("\n");
    printf("\n");
    printf("Other usage: arinc_box_tx.exe --help\n");
    printf("\tPrint this message");
    printf("\n");
    printf("\n");

    printf("See https://github.com/Simtec-AG/arinc-box-translator for more information \n");
    printf("\n");
    printf("\n");
}

int main(int argc, char **argv)
{
    uint8_t medout_buffer[10] = {0};
    char str[20] = {'0'};
    int32_t return_code = EXIT_FAILURE;
    uint32_t arinc_data;
    serial_port_t arinc_serial =
        {
            .baudrate = DEFAULT_BAUDRATE,
            .com_port = "\\\\.\\",
            .windows_handle = NULL
        };


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

            do {
                printf("\n\nEnter a 32 bits value: ");
                scanf("%19s", str);
                arinc_data = (uint32_t) strtoul(str, NULL, 0);

                arinc_box_encode(arinc_data, medout_buffer);
                serial_send_buffer(&arinc_serial, (char *)medout_buffer, 10);
                printf("0x%08X was sent!", arinc_data);

            } while(arinc_data != 0);

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
        printf("See 'arinc_box_tx --help ' for more information. \n");
    }

    return return_code;
}
