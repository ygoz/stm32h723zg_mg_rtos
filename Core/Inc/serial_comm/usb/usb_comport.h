/**
 * @file usb_comport.h
 * @brief USB CDC (Virtual COM Port) interface control.
 *
 * Provides initialization and data handling functions for USB CDC communication.
 * Controlled by the `USB_COMPORT` macro to enable or disable runtime processing.
 *
 * Typical use case: Serial communication over USB with a host terminal.
 *
 * @version 0.1
 * @date 2025-07-17
 * @author Yam Goz
 */

#ifndef USB_COMPORT_H
#define USB_COMPORT_H

#include <stdint.h>

#define USB_COMPORT_OFF     0
#define USB_COMPORT_ON      1



/**
 * @brief User-implemented initialization for the USB CDC virtual COM port.
 *
 * This function is intended to be implemented by the user to set up any application-specific
 * state or behavior before processing USB CDC data.
 *
 * Called once during system initialization if USB_COMPORT is enabled.
 */
void usb_comport_init(void);



/**
 * @brief Processes received USB CDC data.
 *
 * Called from the USB receive callback when data is received over the virtual COM port.
 *
 * @param buf Pointer to the received data buffer.
 * @param len Length of the received data in bytes.
 */
void usb_comport_process_data(uint8_t *buf, uint32_t len);



#endif // USB_COMPORT_H
