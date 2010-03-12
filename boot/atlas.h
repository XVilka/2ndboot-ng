#ifndef __HW_ATLAS_H__
#define __HW_ATLAS_H__

#include "types.h"

/* Thanks power_ic */
typedef enum
{
    /* Beginning of Atlas Registers */
    ATLAS_REG_FIRST_REG,

    ATLAS_REG_INT_STAT_0 = ATLAS_REG_FIRST_REG,  /*!< Interrupt Status 0 */
    ATLAS_REG_INT_MASK_0,            /*!< Interrupt Mask 0 */
    ATLAS_REG_INT_SENSE_0,           /*!< Interrupt Sense 0 */
    ATLAS_REG_INT_STAT_1,            /*!< Interrupt Status 1 */
    ATLAS_REG_INT_MASK_1,            /*!< Interrupt Mask 1 */
    ATLAS_REG_INT_SENSE_1,           /*!< Interrupt Sense 1 */
    ATLAS_REG_PWRUP_SENSE,           /*!< Power Up Mode Sense */
    ATLAS_REG_REVISION,              /*!< Revision */
    ATLAS_REG_SEMAPHORE,             /*!< Semaphore */
    ATLAS_REG_ARB_PERIPH_AUDIO,      /*!< Arbitration Peripheral Audio */
    ATLAS_REG_ARB_SWITCHERS,         /*!< Arbitration Switchers */
    ATLAS_REG_ARB_REG_0,             /*!< Arbitration Regulators 0 */
    ATLAS_REG_ARB_REG_1,             /*!< Arbitration Regulators 1 */
    ATLAS_REG_PWR_CONTROL_0,         /*!< Power Control 0 */
    ATLAS_REG_PWR_CONTROL_1,         /*!< Power Control 1 */
    ATLAS_REG_PWR_CONTROL_2,         /*!< Power Control 2 */
    ATLAS_REG_REGEN_ASSIGN,          /*!< Regen Assignment */
    ATLAS_REG_CONTROL_SPARE,         /*!< Control Spare */
    ATLAS_REG_MEMORY_A,              /*!< Memory A */
    ATLAS_REG_MEMORY_B,              /*!< Memory B */
    ATLAS_REG_RTC_TIME,              /*!< RTC Time */
    ATLAS_REG_RTC_ALARM,             /*!< RTC Alarm */
    ATLAS_REG_RTC_DAY,               /*!< RTC Day */
    ATLAS_REG_RTC_DAY_ALARM,         /*!< RTC Day Alarm */
    ATLAS_REG_SWITCHERS_0,           /*!< Switchers 0 */
    ATLAS_REG_SWITCHERS_1,           /*!< Switchers 1 */
    ATLAS_REG_SWITCHERS_2,           /*!< Switchers 2 */
    ATLAS_REG_SWITCHERS_3,           /*!< Switchers 3 */
    ATLAS_REG_SWITCHERS_4,           /*!< Switchers 4 */
    ATLAS_REG_SWITCHERS_5,           /*!< Switchers 5 */
    ATLAS_REG_REG_SET_0,             /*!< Regulator Setting 0 */
    ATLAS_REG_REG_SET_1,             /*!< Regulator Setting 1 */
    ATLAS_REG_REG_MODE_0,            /*!< Regulator Mode 0 */
    ATLAS_REG_REG_MODE_1,            /*!< Regulator Mode 1 */
    ATLAS_REG_PWR_MISC,              /*!< Power Miscellaneous */
    ATLAS_REG_PWR_SPARE,             /*!< Power Spare */
    ATLAS_REG_AUDIO_RX_0,            /*!< Audio Rx 0 */
    ATLAS_REG_AUDIO_RX_1,            /*!< Audio Rx 1 */
    ATLAS_REG_AUDIO_TX,              /*!< Audio Tx */
    ATLAS_REG_SSI_NETWORK,           /*!< SSI Network */
    ATLAS_REG_AUDIO_CODEC,           /*!< Audio Codec */
    ATLAS_REG_AUDIO_STEREO_DAC,      /*!< Audio Stereo DAC */
    ATLAS_REG_AUDIO_SPARE,           /*!< Audio Spare */
    ATLAS_REG_ADC_0,                 /*!< ADC 0 */
    ATLAS_REG_ADC_1,                 /*!< ADC 1 */
    ATLAS_REG_ADC_2,                 /*!< ADC 2 */
    ATLAS_REG_ADC_3,                 /*!< ADC 3 */
    ATLAS_REG_ADC_4,                 /*!< ADC 4 */
    ATLAS_REG_CHARGER_0,             /*!< Charger 0 */
    ATLAS_REG_USB_0,                 /*!< USB 0 */
    ATLAS_REG_CHARGE_USB_1,          /*!< USB 1 */
    ATLAS_REG_LED_CONTROL_0,         /*!< LED Control 0 */
    ATLAS_REG_LED_CONTROL_1,         /*!< LED Control 1 */
    ATLAS_REG_LED_CONTROL_2,         /*!< LED Control 2 */
    ATLAS_REG_LED_CONTROL_3,         /*!< LED Control 3 */
    ATLAS_REG_LED_CONTROL_4,         /*!< LED Control 4 */
    ATLAS_REG_LED_CONTROL_5,         /*!< LED Control 5 */
    ATLAS_REG_SPARE,                 /*!< Spare */
    ATLAS_REG_TRIM_0,                /*!< Trim 0 */
    ATLAS_REG_TRIM_1,                /*!< Trim 1 */
    ATLAS_REG_TEST_0,                /*!< Test 0 */
    ATLAS_REG_TEST_1,                /*!< Test 1*/
    ATLAS_REG_TEST_2,                /*!< Test 2 */
    ATLAS_REG_TEST_3,                /*!< Test 3 */

    ATLAS_REG_LAST_REG = ATLAS_REG_TEST_3,
} atlas_register;

int atlas_init();
void atlas_set_timeout(unsigned int timeout);
int atlas_reg_io(atlas_register reg, uint32_t *p, int write);
int atlas_reg_read(atlas_register reg, uint32_t *v);
int atlas_reg_modify(atlas_register reg, uint32_t mask, uint32_t v);
int atlas_reg_write(atlas_register reg, uint32_t v);
int atlas_reg_partial(atlas_register reg, uint32_t mask, uint32_t data);

#endif // __HW_ATLAS_H__
