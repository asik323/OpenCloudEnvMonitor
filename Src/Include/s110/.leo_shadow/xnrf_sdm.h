//@+leo-ver=4-thin
//@+node:gan0ling.20140624153650.3664:@shadow nrf_sdm.h
//@@language c
//@@tabwidth -4
//@+others
//@-others
/*
 * Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
/**
  @defgroup nrf_sdm_api SoftDevice Manager API
  @{
     
  @brief APIs for SoftDevice management.
 
*/

/* Header guard */
#ifndef NRF_SDM_H__
#define NRF_SDM_H__

#include "nrf_svc.h"
#include "nrf51.h"
#include "nrf_soc.h"
#include "nrf_error_sdm.h"

/** @addtogroup NRF_SDM_DEFINES Defines
 * @{ */

/**@brief SoftDevice Manager SVC Base number. */
#define SDM_SVC_BASE 0x10   

/** @} */

/** @addtogroup NRF_SDM_ENUMS Enumerations
 * @{ */

/**@brief nRF SoftDevice Manager API SVC numbers. */
enum NRF_SD_SVCS
{
  SD_SOFTDEVICE_ENABLE = SDM_SVC_BASE, /**< ::sd_softdevice_enable */
  SD_SOFTDEVICE_DISABLE,               /**< ::sd_softdevice_disable */
  SD_SOFTDEVICE_IS_ENABLED,            /**< ::sd_softdevice_is_enabled */
  SD_SOFTDEVICE_FORWARD_TO_APPLICATION,/**< ::sd_softdevice_forward_to_application */
  SVC_SDM_LAST                         /**< Placeholder for last SDM SVC */
};

/**@brief Possible lfclk oscillator sources. */
enum NRF_CLOCK_LFCLKSRCS
{
  NRF_CLOCK_LFCLKSRC_SYNTH_250_PPM,                       /**< LFCLK Synthesized from HFCLK.                    */
  NRF_CLOCK_LFCLKSRC_XTAL_500_PPM,                        /**< LFCLK crystal oscillator 500 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_250_PPM,                        /**< LFCLK crystal oscillator 250 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_150_PPM,                        /**< LFCLK crystal oscillator 150 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_100_PPM,                        /**< LFCLK crystal oscillator 100 PPM accuracy.       */
  NRF_CLOCK_LFCLKSRC_XTAL_75_PPM,                         /**< LFCLK crystal oscillator 75 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_XTAL_50_PPM,                         /**< LFCLK crystal oscillator 50 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_XTAL_30_PPM,                         /**< LFCLK crystal oscillator 30 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_XTAL_20_PPM,                         /**< LFCLK crystal oscillator 20 PPM accuracy.        */
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_250MS_CALIBRATION,        /**< LFCLK RC oscillator, 250ms  calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_500MS_CALIBRATION,        /**< LFCLK RC oscillator, 500ms  calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_1000MS_CALIBRATION,       /**< LFCLK RC oscillator, 1000ms calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_2000MS_CALIBRATION,       /**< LFCLK RC oscillator, 2000ms calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_4000MS_CALIBRATION,       /**< LFCLK RC oscillator, 4000ms calibration interval.*/
  NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,       /**< LFCLK RC oscillator, 8000ms calibration interval.*/
};

/** @} */

/** @addtogroup NRF_SDM_TYPES Types
 * @{ */

/**@brief Type representing lfclk oscillator source. */
typedef uint32_t nrf_clock_lfclksrc_t;


/**@brief SoftDevice Assertion Handler type.
 *
 * When an unexpected error occurs within the SoftDevice it will call the SoftDevice assertion handler callback.
 * The protocol stack will be in an undefined state when this happens and the only way to recover will be to
 * perform a reset, using e.g. CMSIS NVIC_SystemReset().
 *
 * @note This callback is executed in HardFault context, thus SVC functions cannot be called from the SoftDevice assert callback.
 *       
 * @param[in] pc The program counter of the failed assert.
 * @param[in] line_number Line number where the assert failed.
 * @param[in] file_name File name where the assert failed.
 */
typedef void (*softdevice_assertion_handler_t)(uint32_t pc, uint16_t line_number, const uint8_t * p_file_name);

/** @} */

/** @addtogroup NRF_SDM_FUNCTIONS Functions
 * @{ */

/**@brief Enables the SoftDevice and by extension the protocol stack.
 *
 * Idempotent function to enable the SoftDevice.
 *
 * @note Some care must be taken if a low frequency clock source is already running when calling this function:
 *       If the LF clock has a different source then the one currently running, it will be stopped. Then, the new
 *       clock source will be started.
 *
 * @note This function has no effect when returning with an error.
 *
 * @post If return code is ::NRF_SUCCESS 
 *       - SoC library and protocol stack APIs are made available
 *       - A portion of RAM will be unavailable (see relevant SDS documentation)
 *       - Some peripherals will be unavailable or available only through the SoC API (see relevant SDS documentation)
 *       - Interrupts will not arrive from protected peripherals or interrupts
 *       - nrf_nvic_ functions must be used instead of CMSIS NVIC_ functions for reliable usage of the softdevice.
 *       - Interrupt latency may be affected by the SoftDevice  (see relevant SDS documentation)
 *       - Chosen low frequency clock source will be running
 *
 * @param clock_source Low frequency clock source and accuracy. (Note: In the case of XTAL source, the PPM accuracy of the chosen clock source must be greater than or equal to the actual characteristics of your XTAL clock).
 * @param assertion_handler Callback for SoftDevice assertions.
 *
 * @retval ::NRF_SUCCESS
 * @retval ::NRF_ERROR_SDM_INCORRECT_INTERRUPT_CONFIGURATION SoftDeviceinterrupt is already enabled, or an enabled interrupt has an illegal priority level
 * @retval ::NRF_ERROR_SDM_LFCLK_SOURCE_UNKNOWN Unknown low frequency clock source selected
 */
SVCALL(SD_SOFTDEVICE_ENABLE, uint32_t, sd_softdevice_enable(nrf_clock_lfclksrc_t clock_source, softdevice_assertion_handler_t assertion_handler));

/**@brief Disables the SoftDevice and by extension the protocol stack.
 * 
 * Idempotent function to disable the SoftDevice.
 *
 * @post SoC library and protocol stack APIs are made unavailable.
 * @post All interrupts that was protected by the SoftDevice will be disabled and initialized to priority 0 (highest).
 * @post All peripherals used by the SoftDevice will be reset to default values.
 * @post All of RAM become available.
 * @post All interrupts are forwarded to the application.
 * @post LFCLK source chosen in ::sd_softdevice_enable will be left running.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_SOFTDEVICE_DISABLE, uint32_t, sd_softdevice_disable(void));

/**@brief Check if the SoftDevice is enabled.
 *
 * @param[out]  p_softdevice_enabled If the SoftDevice is enabled: 1 else 0.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_SOFTDEVICE_IS_ENABLED, uint32_t, sd_softdevice_is_enabled(uint8_t * p_softdevice_enabled));

/**@brief Start forwarding interrupts to application.
 * 
 * This function is only intended to be called when a bootloader is enabled is used.
 * The bootloader should call this right before it starts the application. 
 * It is recommended that all interrupt sources are off when this is called, 
 * or you could end up having interrupts in the application being executed before main() of the application.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_SOFTDEVICE_FORWARD_TO_APPLICATION, uint32_t, sd_softdevice_forward_to_application(void)); 

/** @} */

#endif // NRF_SDM_H__

/**
  @}
*/
//@nonl
//@-node:gan0ling.20140624153650.3664:@shadow nrf_sdm.h
//@-leo
