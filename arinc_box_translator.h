/**
* This module decodes and encodes messages in the proprietary format used to communicated with the
* ARINC-429-TO-USB Converter Box from Simtec AG.
*
* © 2023 Simtec AG. All rights reserved.
* Company Confidential
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

#ifndef ARINC_BOX_TRANSLATOR_H
#define ARINC_BOX_TRANSLATOR_H

#include <stdint.h>
#include <stdbool.h>

/** Type of message that could be returned by the USB to ARINC converter */
typedef enum
{
    ARINC_PENDING = 0,             /**< No complete message has been decoded yet */
    ARINC_RETURNED_DATA = 1,       /**< One complete data message has been decoded */
    ARINC_EMPTY = 2,               /**< Empty message received (time-out) */
    ARINC_ERROR = 3                /**< An error happened during the decoding of the message */
} arinc_box_msg_type_t;

/** Decoded ARINC message sent by a swiss air-data computer*/
typedef struct
{
    arinc_box_msg_type_t msg_type;
    uint32_t data_value;
} arinc_box_msg_t;

/**
 * Decodes a message transmitted by an ARINC-429-TO-USB converter box.
 * 
 * This function shall be called everytime a new byte has been received from the serial port.
 * If the byte is the last of a message and this message was decoded successfully, 
 * the 32 bits arinc data will be returned.
 * If the message is not fully decoded yet, the rs485 message type will be ARINC_PENDING.
 * If there was an error decoding the message, the rs485 message type will be ARINC_ERROR.
 *
 * @param[in]   raw_data    Raw 8 bits data received by an air data computer.
 *
 * @return Decoded arinc message.
 */
arinc_box_msg_t arinc_box_decode(char raw_data);

/**
 * Encodes a 32 bits message to be transmitted to an ARINC-429-TO-USB converter box.
 *
 * @param[in]   arinc_word      32 bits arinc word to be encoded.
 * @param[out]  encoded_char    Buffer of at least 10 bytes that will be filled with the encoded message.
 *
 * @return Decoded arinc message.
 */
void arinc_box_encode(uint32_t arinc_data, uint8_t encoded_char[10]);

#endif
