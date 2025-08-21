#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c1.h"
#include "serial_comm/i2c/hi2c4.h"
#include "serial_comm/i2c/i2c_master_test.h"
#include "serial_comm/i2c/i2c_slave_test.h"






#ifdef HIL_SLAVE_MODE


uint8_t i2c_slave_tx_data[] = I2C_SLAVE_RESPONSE_PASS;  // the data to send


uint8_t i2c_slave_rx_buffer[I2C_BUFF_SIZE];

int slave_rx_i2c_byte_count = 0;



HAL_StatusTypeDef i2c_slave_init(void){
  if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK) {
    return HAL_ERROR;
  }
  if (HAL_I2C_EnableListen_IT(&hi2c4) != HAL_OK) {
    return HAL_ERROR;
  }
    return HAL_OK;
}



/**
 * @brief I2C slave listen complete callback
 *
 * This function is invoked when the slave finishes its listening 
 * sequence for addressing events. After this, the slave must 
 * typically be re-enabled for listening again.
 *
 * @param hi2c Pointer to the I2C handle structure
 */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}


/**
 * @brief I2C address match callback.
 *
 * This callback is executed by HAL when the I2C peripheral, 
 * configured as a slave, detects that its address has been matched 
 * by a master device. Based on the transfer direction, the function 
 * either prepares the slave to receive data from the master or 
 * transmit data back to the master.
 *
 * Behavior:
 * - **Master → Slave (Transmit direction):**
 *   - Prints a debug message.
 *   - Resets the RX byte counter (`slave_rx_i2c_byte_count`).
 *   - Clears the receive buffer (`i2c_slave_rx_buffer`).
 *   - Starts a sequential reception for 1 byte using 
 *     @ref HAL_I2C_Slave_Sequential_Receive_IT.
 *
 * - **Slave → Master (Receive direction):**
 *   - Compares the content of `i2c_slave_rx_buffer` against the 
 *     expected request string (`I2C_MASTER_REQUEST`).
 *   - If the request matches, starts a sequential transmit of 
 *     the prepared response buffer (`i2c_slave_tx_data`) using 
 *     @ref HAL_I2C_Slave_Sequential_Transmit_IT.
 *   - Otherwise, prints a mismatch message with the expected 
 *     request string.
 *
 * @param hi2c Pointer to the I2C handle structure.
 * @param TransferDirection Indicates transfer direction:
 *        - @ref I2C_DIRECTION_TRANSMIT : master is writing to slave.
 *        - @ref I2C_DIRECTION_RECEIVE  : master is reading from slave.
 * @param AddrMatchCode The matched address (useful if multiple slave addresses are configured).
 *
 * @note This function sets up the buffers and starts the appropriate 
 *       sequential transfers. Subsequent data bytes are handled in 
 *       the @ref HAL_I2C_SlaveRxCpltCallback and 
 *       @ref HAL_I2C_SlaveTxCpltCallback functions.
 */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4){
        if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // Master writing to slave
        {
            printf("Master wants to transmit\r\n");
            slave_rx_i2c_byte_count = 0;  // reset slave_rx_i2c_byte_count for new transfer
            memset(i2c_slave_rx_buffer, 0, I2C_BUFF_SIZE); // clean rx buffer for new transfer
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &i2c_slave_rx_buffer[slave_rx_i2c_byte_count], 1, I2C_FIRST_FRAME);
        }
        else if (TransferDirection == I2C_DIRECTION_RECEIVE) // Master reading from slave
        {
            if (strncmp((char*)i2c_slave_rx_buffer, I2C_MASTER_REQUEST, strlen(I2C_MASTER_REQUEST)) == 0) {
                printf("Message matches expected request!\r\n");
                HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, i2c_slave_tx_data, sizeof(i2c_slave_tx_data) - 1, I2C_FIRST_AND_LAST_FRAME);
            } else {
                printf("Message does not match. Expected: \"%s\"\r\n", I2C_MASTER_REQUEST);
            }
        }
    }
}



/**
 * @brief I2C slave transmit complete callback
 *
 * This function is called by the HAL when the I2C slave finishes 
 * sending data back to the master. It can be used for logging, 
 * debugging, or preparing the next transmission.
 *
 * @param hi2c Pointer to the I2C handle structure (I2C1, I2C4, etc.)
 */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4) {
        printf("Slave finished sending: %s\r\n", i2c_slave_tx_data);
    }
}



// Called each time a byte is received
/**
 * @brief I2C slave receive complete callback
 *
 * This function is called by the HAL when the I2C slave finishes 
 * receiving data from the master. The received data is typically 
 * stored in a buffer for processing. From here, the application 
 * can validate the data and prepare a response if necessary.
 *
 * @param hi2c Pointer to the I2C handle structure (I2C1, I2C4, etc.)
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4){
        slave_rx_i2c_byte_count++;  // increment received byte slave_rx_i2c_byte_count
        if (slave_rx_i2c_byte_count < I2C_BUFF_SIZE)
        {
            // Prepare to receive next byte
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &i2c_slave_rx_buffer[slave_rx_i2c_byte_count], 1, I2C_NEXT_FRAME);
        }
        else
        {
            printf("Buffer full, stopping reception\r\n");
        }
    }
}



/**
 * @brief I2C error callback
 *
 * This function is called by the HAL when an I2C error occurs 
 * during slave communication (bus error, arbitration lost, 
 * ACK failure, etc.). It can be used to log errors and 
 * re-initialize the I2C peripheral if necessary.
 *
 * @param hi2c Pointer to the I2C handle structure
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4){
	    HAL_I2C_EnableListen_IT(hi2c);
    }
}




#endif