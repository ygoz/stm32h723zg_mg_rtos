#include "peripherals/adc/utils.h"
#include "peripherals/adc/hadc3.h"
#include "HaGeneral/HaGeneral_config.h"
#include "mongoose.h"





uint8_t init_adc(ADC_HandleTypeDef *hadc, uint32_t calibration_mode, uint8_t status_bit, const char *label) {
    // de-init and re-init
    if (HAL_ADC_DeInit(hadc) == HAL_OK && HAL_ADC_Init(hadc) == HAL_OK) {
        // calibrate adc
        if (HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, calibration_mode) == HAL_OK) {
            //start adc
            MG_INFO(("%s initialized.", label));
            // add its adc status bit
            return status_bit;
        }
    }
    MG_INFO(("%s initialization failed.", label));
    return 0;
}





uint8_t adc_init_all_handles(void) {
    uint8_t adc_status = 0;

    #if ADC1_HANDLE_STATUS == HANDLE_ON
    adc_status |= init_adc(&hadc1, ADC1_SINGLE_OR_DOUBLE_ENDED, ADC_STATUS_ADC1, "ADC1");
    #endif

    #if ADC2_HANDLE_STATUS == HANDLE_ON
    adc_status |= init_adc(&hadc2, ADC2_SINGLE_OR_DOUBLE_ENDED, ADC_STATUS_ADC2, "ADC2");
    #endif

    #if ADC3_HANDLE_STATUS == HANDLE_ON
    adc_status |= init_adc(&hadc3, ADC3_SINGLE_OR_DOUBLE_ENDED, ADC_STATUS_ADC3, "ADC3");
    #endif

    MG_INFO(("ADC Init Status: 0x%02X\n", adc_status));
    return adc_status;
}
