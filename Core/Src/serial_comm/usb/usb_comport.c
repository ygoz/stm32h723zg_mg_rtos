#include "usb_comport.h"
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdio.h>

static char reply_buf[256];


void usb_comport_init(void)
{
    return;
}


void usb_comport_process_data(uint8_t *buf, uint32_t len)
{
    // Null-terminate the received data for safe use as string
    if (len < sizeof(reply_buf) - 28) {
        buf[len] = '\0';
    } else {
        buf[sizeof(reply_buf) - 1] = '\0';
    }

    // Format response
    snprintf(reply_buf, sizeof(reply_buf), "shalom said you wrote: %s\r\n", buf);

    // Send response back to host
    CDC_Transmit_HS((uint8_t *)reply_buf, strlen(reply_buf));
}
