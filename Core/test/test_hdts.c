// #include "mock_stm32h7xx_hal.h"
// #include "mock_stm32h7xx_hal_conf.h"


#include "unity.h"
#include "mock_hdts.h"      // Mock of dts_get_temperature
#include "dts_utils.h"      // Header for system_is_overheating

// Runs before each test
void setUp(void) {}

// Runs after each test
void tearDown(void) {}

// void test_overheating_returns_1_if_temp_above_80(void) {
//     int32_t fake_temp = 90;

//     // Setup mock: dts_get_temperature returns HAL_OK and sets temp to 90
//     dts_get_temperature_ExpectAnyArgsAndReturn(HAL_OK);
//     dts_get_temperature_ReturnThruPtr_temperature(&fake_temp);

//     TEST_ASSERT_EQUAL(1, system_is_overheating());
// }

// void test_overheating_returns_0_if_temp_below_or_equal_80(void) {
//     int32_t fake_temp = 75;

//     dts_get_temperature_ExpectAnyArgsAndReturn(HAL_OK);
//     dts_get_temperature_ReturnThruPtr_temperature(&fake_temp);

//     TEST_ASSERT_EQUAL(0, system_is_overheating());
// }

// void test_overheating_returns_minus1_if_dts_error(void) {
//     int32_t fake_temp = 50; // value doesn’t matter

//     dts_get_temperature_ExpectAnyArgsAndReturn(HAL_ERROR);

//     TEST_ASSERT_EQUAL(-1, system_is_overheating());
// }
