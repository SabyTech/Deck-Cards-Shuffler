#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>

/* getmousedeltas  : fucntion to get the mouse deltas
 * uint8_t *buffer : incoming mouse data
 * bool dXdelta    : flag to check if dX or not i.e dY delta
 */
static uint16_t getmousedeltas(uint8_t *buffer, bool dXdelta)
{
    uint8_t B2 = *(buffer+2);
    uint8_t B3 = *(buffer+3);
    uint8_t B4 = *(buffer+4);
    uint16_t delta = 0;

    if (dXdelta)
        delta = (((B4 & 0xC) << 6) | B2); 
    else 
        delta = (((B4 & 0x30) << 4) | B3); 

    return delta;
}

/* getbuttonstate  : function to get the mouse buttons state
 * uint8_t *buffer : incoming mouse data
 * bool leftClick  : flag to check if left or not i.e right button 
 */
static bool getbuttonstate(uint8_t *buffer, bool leftbutton)
{
    uint8_t B4 = *(buffer+4);
    bool status = false;

    // considering left click is the primary control,
    // and left click is secondary. B4[1:0] represents
    // set bits combinations as 00, 01, 10, 11. Using
    // mask 1 to check for left button status to catch
    // both left only(01) and left+right(11) as left is
    // primary control. Using mask 2 for right button
    // status (10).

    if (leftbutton)
        status = !!(B4 & 0x1);
    else 
        status = !!((B4 & 0x2) >> 1);

    return status;
}

/* gettimestamp    : function to get the device timestamp
 * uint8_t *buffer : incoming mouse data
 */
static uint32_t gettimestamp(uint8_t *buffer)
{
    uint8_t B4 = *(buffer+4);
    uint8_t B5 = *(buffer+5);
    uint8_t B6 = *(buffer+6);
    uint32_t counter = 0;
    uint32_t timestamp = 0;

    counter  = (B6 << 10) | (B5 << 2) | ((B4 & 0xC0) >> 6);

    if (counter >= 0x3FFFF || counter < timestamp)
        timestamp+=counter;
    else
        timestamp = counter;

    return timestamp;
}

/* handleReport : function to handle incoming mouse data
 * uint8 *buffer: incoming mouse data
 * uint16 length: length of incoming mouse data
 */
static void handleReport(uint8_t *buffer, uint16_t length)
{
    uint16_t dX = 0;
    uint16_t dY = 0;
    uint32_t timestamp = 0;
    bool leftbutton = false;
    bool rightbutton = false;

    //Taking care of Endianess 
    
    uint64_t data = (uint64_t)buffer[7] << 56 | (uint64_t)buffer[6] << 48 | \
                    (uint64_t)buffer[5] << 40 | (uint64_t)buffer[4] << 32 | \
                    (uint64_t)buffer[3] << 24 | (uint64_t)buffer[2] << 16 | \
                    (uint64_t)buffer[1] << 8 | (uint64_t)buffer[0];
    
    buffer = (uint8_t*)(&data);
                        
    uint8_t *tmpbuff = (uint8_t*)malloc(length*sizeof(uint8_t)) ;
    memcpy(tmpbuff, buffer, length);

    dX = getmousedeltas(tmpbuff, true);
    dY = getmousedeltas(tmpbuff, false);

    leftbutton = getbuttonstate(tmpbuff, true);
    rightbutton = getbuttonstate(tmpbuff, false);

    timestamp = gettimestamp(tmpbuff);

    printf("\n\tMouse deltas (dX:%u, dY:%u), button state (left:%d,"
           " right:%d), timestamp: %u ms\n\n", dX, dY, leftbutton,
            rightbutton, timestamp);
}

/* driver test function */
int main() {
    uint64_t packet = 0xDFF1B19B441011;
    uint8_t *buffer = (uint8_t*)(&packet); 
    uint16_t length = 7;

    //call handleReport to process incoming data
    handleReport(buffer, length);

    return 0;
}
