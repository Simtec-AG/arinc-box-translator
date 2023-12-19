/*
* © 2023 Simtec AG. All rights reserved.
* Company Confidential
*/

#include "arinc_box_translator.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/** Start of header of a data packet */
#define SOH_1 ((char)0x01u)
#define SOH_2 ((char)0x02u)
#define SOH_3 ((char)0x03u)
#define SOH_5 ((char)0x05u)
#define SOH_6 ((char)0x06u)

/** Carriage return */
#define CR ((char)0x0Du)

/** Maximum size in byte of the buffer needed to decode one message */
#define MAX_BUFFER_LENGTH 10

/** 
 * Decodes a message received from the USB-TO-ARINC converter box to a 32 bits word.
 *
 * @note The message length shall be 7 bytes long.
 * @param[in]   msg     Message received
 * @return RS485_ERROR if there was an error decoding the message, RS485_RETURNED_DATA otherwise.
 */
static uint32_t arinc_box_decode_data(uint8_t msg[])
{
    uint32_t returned_value = 0;

    // Process byte 6
    uint8_t b6 = msg[5];
    if (b6 != 0) {
        // Convert data bytes back to original ACK or CR value
        if ((b6 >> 7) & 1) {
            msg[4] = (msg[4] == 0x00) ? SOH_6 : CR;
        }

        if ((b6 >> 6) & 1) {
            msg[3] = (msg[3] == 0x00) ? SOH_6 : CR;
        }

        if ((b6 >> 5) & 1) {
            msg[2] = (msg[2] == 0x00) ? SOH_6 : CR;
        }

        if ((b6 >> 4) & 1) {
            msg[1] = (msg[1] == 0x00) ? SOH_6 : CR;
        }
    }

    // Append all 4 bytes  to an ARINC word
    returned_value = (uint8_t) msg[1] | ((uint8_t) msg[2] << 8) | ((uint8_t) msg[3] << 16) | ((uint8_t) msg[4] << 24);

    return returned_value;
}

/** 
 * Test if the message received in the proprietary format is an empty message
 * @note The message length shall be 7 bytes long.
 * @param[in]   msg     Message received.
 * @return TRUE if the message received is an empty message.
 */
static bool arinc_box_is_msg_empty(uint8_t msg[])
{
    const uint8_t EMPTY_MSG[7] = {SOH_6, 0x00, 0x00, 0x00, 0x80, 0x00, CR};
    
    return (memcmp(msg, EMPTY_MSG, 7) == 0);
}

/** 
 * Decodes an entire message received by an ARINC-TO-USB converter box.
 * @param[in]   raw_msg         Data received by the air data computer.
 * @param[in]   raw_msg_length  Length of the message received.
 * @param[out]  parsed_msg      Pointer that will contain the decoded air data message.
 * @return void
 */
static void arinc_box_decode_msg(uint8_t raw_msg[], uint8_t raw_msg_length, arinc_box_msg_t *parsed_msg)
{
    parsed_msg->msg_type = ARINC_ERROR;

    if (raw_msg_length == 7)
    {
        if(arinc_box_is_msg_empty(raw_msg))
        {
            parsed_msg->msg_type = ARINC_EMPTY;
            parsed_msg->data_value = 0;
        }
        else
        {
            parsed_msg->msg_type = ARINC_RETURNED_DATA;
            parsed_msg->data_value= arinc_box_decode_data(raw_msg);
        }
    }
}

arinc_box_msg_t arinc_box_decode(char raw_data)
{
    static uint8_t buffer[MAX_BUFFER_LENGTH] = {0};
    static uint8_t pos = 0;

    arinc_box_msg_t returned_message;
    returned_message.msg_type = ARINC_ERROR;

    if ((raw_data == SOH_6))
    {
        // A SOH marks the beggining of a message
        buffer[0] = (uint8_t)raw_data;
        pos = 1;
        returned_message.msg_type = ARINC_PENDING;
    }
    else if ((pos > 0) && (pos < MAX_BUFFER_LENGTH))
    {
        // Store the byte received
        buffer[pos] = (uint8_t)raw_data;

        if (raw_data == CR)
        {
            // A carriage return marks the end of a message, decode
            arinc_box_decode_msg(buffer, pos + 1, &returned_message);
            pos = 0;
        }
        else
        {
            // Current message is not totally received
            pos++;
            returned_message.msg_type = ARINC_PENDING;
        }
    }

    return returned_message;
}

void arinc_box_encode(uint32_t arinc_data, uint8_t encoded_char[10])
{
    encoded_char[0] = SOH_1;
    encoded_char[1] = 'A' + ((arinc_data >> 0) & 0xF);
    encoded_char[2] = 'A' + ((arinc_data >> 4) & 0xF);
    encoded_char[3] = 'A' + ((arinc_data >> 8) & 0xF);
    encoded_char[4] = 'A' + ((arinc_data >> 12) & 0xF);
    encoded_char[5] = 'A' + ((arinc_data >> 16) & 0xF);
    encoded_char[6] = 'A' + ((arinc_data >> 20) & 0xF);
    encoded_char[7] = 'A' + ((arinc_data >> 24) & 0xF);
    encoded_char[8] = 'A' + ((arinc_data >> 28) & 0xF);
    encoded_char[9] = CR;
}