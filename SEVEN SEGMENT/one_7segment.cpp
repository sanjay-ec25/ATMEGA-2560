#include "one_7segment.h"

/* Segment codes for common-cathode display
 * bit 0 = a, bit 1 = b, ... bit 6 = g, bit 7 = dp
 */
static const uint8_t sevensegment_code[10] =
{
    0x3F,   /* 0 */
    0x06,   /* 1 */
    0x5B,   /* 2 */
    0x4F,   /* 3 */
    0x66,   /* 4 */
    0x6D,   /* 5 */
    0x7D,   /* 6 */
    0x07,   /* 7 */
    0x7F,   /* 8 */
    0x6F    /* 9 */
};

static uint8_t seg1_port;

/* Configure the 8 segment lines as outputs */
void sevensegment1_init(uint8_t segment_port)
{
    seg1_port = segment_port;
    port_config(seg1_port, 0xFF);
    port_write(seg1_port, 0x00);
}

/* Display one decimal digit: 0 to 9 */
void sevensegment1_display(uint8_t value)
{
    if (value <= 9)
    {
        port_write(seg1_port, sevensegment_code[value]);
    }
    else    
    {
        port_write(seg1_port, 0x00);
    }
}

/* Turn OFF all segments */
void sevensegment1_clear(void)
{
    port_write(seg1_port, 0x00);
}
