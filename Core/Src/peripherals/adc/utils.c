#include "peripherals/adc/utils.h"
#include "peripherals/adc/hadc3.h"
#include "peripherals/adc/hadc2.h"
#include "peripherals/adc/hadc1.h"
#include "HaGeneral/HaGeneral_config.h"
#include "mongoose.h"





uint8_t init_adc(ADC_HandleTypeDef *hadc, uint32_t ADC_SINGLE_OR_DOUBLE_ENDED, uint8_t status_bit, const char *label) {
        // calibrate adc
        if (HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, ADC_SINGLE_OR_DOUBLE_ENDED) == HAL_OK) {
            MG_INFO(("%s initialized.", label));
            // return its adc status bit
            return status_bit;
        }
    MG_INFO(("%s initialization failed.", label));
    return 0;
}




// add dma to init + analog wdg
uint8_t adc_init_all_handles(void) {
    uint8_t adc_status = 0;

    #if ADC1_HANDLE_STATUS == HANDLE_ON
    adc_status |= init_adc(&hadc1, ADC1_SINGLE_OR_DOUBLE_ENDED, ADC_STATUS_ADC1, "ADC1");
        #if ADC1_ANALOG_WATCHDOG == HANDLE_ON
        HAL_ADC_Start_IT(&hadc1);
        #endif
    #endif

    #if ADC2_HANDLE_STATUS == HANDLE_ON
    adc_status |= init_adc(&hadc2, ADC2_SINGLE_OR_DOUBLE_ENDED, ADC_STATUS_ADC2, "ADC2");
        #if ADC2_ANALOG_WATCHDOG == HANDLE_ON
        HAL_ADC_Start_IT(&hadc2);
        #endif
    #endif

    #if ADC3_HANDLE_STATUS == HANDLE_ON
    adc_status |= init_adc(&hadc3, ADC3_SINGLE_OR_DOUBLE_ENDED, ADC_STATUS_ADC3, "ADC3");
        #if ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE
        HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc3_dma_buffer, ADC3_DMA_BUFFER_SIZE);
        #endif
        #if ADC3_ANALOG_WATCHDOG == HANDLE_ON
        HAL_ADC_Start_IT(&hadc3);
        #endif
    #endif

    MG_INFO(("ADC Init Status: 0x%02X\n", adc_status));
    return adc_status;
}



// Process interrupt vals here
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc == &hadc3){
    // Process interrupt vals here
    // printf("ADC value [%d]: %d\r\n", 0, adc3_dma_buffer[0]);

  }
}


// Anomally callback
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc3){
        adc3_wdg_process_anomaly();
      }
    else if (hadc == &hadc2){
        adc2_wdg_process_anomaly();
      }
    else if (hadc == &hadc1){
        adc1_wdg_process_anomaly();
      }
}


uint16_t * adc_get_dma_buffer(ADC_HandleTypeDef *hadc){
    if (hadc == &hadc3){
        return adc3_dma_buffer;
      }
    else{
        MG_INFO(("only ADCs 1, 2, and 3 are available!!"));
        return 0;
    }
}


uint16_t adc_get_http_response(
    uint16_t *adc_value,
    char *response,
    size_t response_size,
    ADC_HandleTypeDef *hadc,
    uint8_t ADC_POLLING_OR_DMA_MODE,
    uint8_t ADC_HANDLE_STATUS
) {

    uint16_t http_status_code = 0;
  
    if (ADC_HANDLE_STATUS == HANDLE_OFF){
        snprintf(response, response_size, "adc handle is off");
        http_status_code = 400;
        return http_status_code;
    }
    
    else if (ADC_HANDLE_STATUS == HANDLE_ON){

        if (ADC_POLLING_OR_DMA_MODE == ADC_DMA_MODE){

            uint16_t *dma_buffer = adc_get_dma_buffer(hadc);
            
            if (dma_buffer) {
                snprintf(response, response_size, "adc value : %u, %u\n", dma_buffer[0], dma_buffer[1]);
                http_status_code = 200;
            } else {
                snprintf(response, response_size, "adc dma buffer error");
                http_status_code = 500;
            }
            return http_status_code;
        }
    
        else if (ADC_POLLING_OR_DMA_MODE == ADC_POLLING_MODE){
            if (adc_polling_get_value(hadc, adc_value, 100) == HAL_OK) {
                snprintf(response, response_size, "adc value : %u\n", *adc_value);
                http_status_code = 200;
                return http_status_code;
            } else {
                snprintf(response, response_size, "adc read failed\n");
                http_status_code = 500;
                return http_status_code;
              }
        }
    }
    else{
        snprintf(response, response_size, "adc read failed\n");
        http_status_code = 500;
        return http_status_code;
    }
}




// POLLING !!!
HAL_StatusTypeDef adc_polling_get_value(ADC_HandleTypeDef *hadc, uint16_t *adc_value, uint16_t polling_timeout) {
    HAL_StatusTypeDef status = HAL_OK;

    // Attempt to start ADC conversion
    status = HAL_ADC_Start(hadc);
    
    if (status == HAL_BUSY) {
        // ADC is likely in continuous mode; read the latest value
        *adc_value = (uint16_t)HAL_ADC_GetValue(hadc);
        return HAL_OK;
    }
    else if (status != HAL_OK) {
        // Failed to start ADC conversion (other than busy)
        MG_INFO(("ADC start failed with status %d", status));
        return status;
    }

    // Wait for conversion to complete
    status = HAL_ADC_PollForConversion(hadc, polling_timeout);
    if (status == HAL_OK) {
        *adc_value = (uint16_t)HAL_ADC_GetValue(hadc);
    } else {
        MG_INFO(("ADC conversion timeout or failed with status %d", status));
    }

    return status;
}