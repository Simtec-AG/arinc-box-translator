/**
* This module does read a serial port on a windows machine.
*
* © 2023 Simtec AG. All rights reserved.
*
* Compiled and tested with gcc version 8.1.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)
*
* Example code only. Use at own risk.
*
* This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* Simtec AG has no obligation to provide maintenance, support,  updates, enhancements, or modifications.
*/

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <windows.h>

/** Structure of a serial port */
typedef struct
{
    char com_port[32];
    uint32_t baudrate;
    HANDLE windows_handle;
} serial_port_t;

/**
 * Open and initialize a serial interface.
 *
 * @param[in, out]  serial      Serial port that shall be opened.
 *
 * @return EXIT_FAILURE if the stream to the airdata computer couldnt be opened correctly,
 * EXIT_SUCCESS otherwise.
 */
int32_t serial_open(serial_port_t *serial);

/**
 * Close a serial interface.
 *
 * @param[in,out]   serial     Serial port that shall be closed.
 *
 * @return EXIT_FAILURE if the stream to the airdata computer couldnt be closed correctly,
 * EXIT_SUCCESS otherwise.
 */
int32_t serial_close(serial_port_t *serial);

/**
 * Read a byte received through a serial port.
 * @note A call to the function serial_open() shall have been done before calling this function
 *
 * @param[in,out]   serial      Pointer to the serial port returned by the function serial_open()
 * @param[out]      data        Pointer to the variable that will contain the read character.
 * @return EXIT_FAILURE if no new character has been received, EXIT_SUCCESS otherwise.
 */
int32_t serial_get_byte(serial_port_t *serial, char *data);

/**
 * Read multiple bytes received through a serial port.
 * @note A call to the function serial_open() shall have been done before calling this function
 *
 * @param[in,out]   serial      Pointer to the serial port returned by the function serial_open()
 * @param[out]      data        Pointer to the variable that will contain the read characters.
 * @param[in]       size        Number of bytes to read
 * @return EXIT_FAILURE if not enough characters has been received, EXIT_SUCCESS otherwise.
 */
int32_t serial_get_buffer(serial_port_t *serial, char *data, uint32_t size);

/**
 * Write a byte through a serial port.
 * @note A call to the function serial_open() shall have been done before calling this function
 *
 * @param[in,out]   serial      Pointer to the serial port returned by the function serial_open()
 * @param[in]       data        Byte to write to the serial port.
 * @return EXIT_FAILURE if an error occurred during the write operation, EXIT_SUCCESS otherwise.
 */
int32_t serial_send_byte(serial_port_t *serial, char data);

/**
 * Write multiple bytes through a serial port.
 * @note A call to the function serial_open() shall have been done before calling this function
 *
 * @param[in,out]   serial      Pointer to the serial port returned by the function serial_open()
 * @param[in]       data        Pointer to the data to be written to the serial port.
 * @param[in]       size        Number of bytes to write
 * @return EXIT_FAILURE if an error occurred during the write operation, EXIT_SUCCESS otherwise.
 */
int32_t serial_send_buffer(serial_port_t *serial, const char *data, uint32_t size);

#endif