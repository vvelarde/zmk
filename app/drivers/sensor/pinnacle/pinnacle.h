#pragma once

#include <device.h>
#include <drivers/spi.h>
#include <drivers/i2c.h>

#define PINNACLE_READ       0xA0
#define PINNACLE_WRITE      0x80

#define PINNACLE_AUTOINC    0xFC
#define PINNACLE_DUMMY      0xFB

// Registers
#define PINNACLE_FW_ID               0x00 // ASIC ID.
#define PINNACLE_FW_VER              0x01 // Firmware Version Firmware revision number.
#define PINNACLE_STATUS1             0x02 // Contains status flags about the state of Pinnacle.
#define PINNACLE_STATUS1_SW_DR       BIT(2)
#define PINNACLE_STATUS1_SW_CC       BIT(3)
#define PINNACLE_SYS_CFG             0x03 // Contains system operation and configuration bits.
#define PINNACLE_SYS_CFG_EN_SLEEP    BIT(2)
#define PINNACLE_SYS_CFG_SHUTDOWN    BIT(1)
#define PINNACLE_SYS_CFG_RESET       BIT(0)

#define PINNACLE_FEED_CFG1           0x04 // Contains feed operation and configuration bits.
#define PINNACLE_FEED_CFG1_EN_FEED   BIT(0)
#define PINNACLE_FEED_CFG1_ABS_MODE  BIT(1)
#define PINNACLE_FEED_CFG1_DIS_FILT  BIT(2)
#define PINNACLE_FEED_CFG1_DIS_X     BIT(3)
#define PINNACLE_FEED_CFG1_DIS_Y     BIT(4)
#define PINNACLE_FEED_CFG1_INV_X     BIT(6)
#define PINNACLE_FEED_CFG1_INV_Y     BIT(7)
#define PINNACLE_FEED_CFG2           0x05 // Contains feed operation and configuration bits.
#define PINNACLE_FEED_CFG2_EN_IM     BIT(0)  // Intellimouse
#define PINNACLE_FEED_CFG2_DIS_TAP   BIT(1)  // Disable all taps
#define PINNACLE_FEED_CFG2_DIS_SEC   BIT(2)  // Disable secondary tap
#define PINNACLE_FEED_CFG2_DIS_SCRL  BIT(3)  // Disable scroll
#define PINNACLE_FEED_CFG2_DIS_GE    BIT(4)  // Disable GlideExtend
#define PINNACLE_FEED_CFG2_ROTATE_90 BIT(7)  // Swap X & Y
#define PINNACLE_CAL_CFG             0x07 // Contains calibration configuration bits.
#define PINNACLE_PS2_AUX             0x08 // Contains Data register for PS/2 Aux Control.
#define PINNACLE_SAMPLE              0x09 // Sample Rate Number of samples generated per second.
#define PINNACLE_Z_IDLE              0x0A // Number of Z=0 packets sent when Z goes from >0 to 0.
#define PINNACLE_Z_SCALER            0x0B // Contains the pen Z_On threshold.
#define PINNACLE_SLEEP_INTERVAL      0x0C // Sleep Interval
#define PINNACLE_SLEEP_TIMER         0x0D // Sleep Timer
#define PINNACLE_AG_PACKET0          0x10 // trackpad Data (Pinnacle AG)
#define PINNACLE_2_2_PACKET0         0x12 // trackpad Data
#define PINNACLE_REG_COUNT           0x18

#define PINNACLE_PACKET0_BTN_PRIM    BIT(0)  // Primary button
#define PINNACLE_PACKET0_BTN_SEC     BIT(1)  // Secondary button
#define PINNACLE_PACKET0_BTN_AUX     BIT(2)  // Auxiliary (middle?) button
#define PINNACLE_PACKET0_X_SIGN      BIT(4)  // X delta sign
#define PINNACLE_PACKET0_Y_SIGN      BIT(5)  // Y delta sign

struct pinnacle_data {
    int16_t dx, dy;
    uint8_t btn;
    bool in_int;
#ifdef CONFIG_PINNACLE_TRIGGER
    const struct device *dev;
    const struct sensor_trigger *data_ready_trigger;
	struct gpio_callback gpio_cb;
    sensor_trigger_handler_t data_ready_handler;
#if defined(CONFIG_PINNACLE_TRIGGER_OWN_THREAD)
    K_THREAD_STACK_MEMBER(thread_stack, CONFIG_PINNACLE_THREAD_STACK_SIZE);
    struct k_sem gpio_sem;
    struct k_thread thread;
#elif defined(CONFIG_PINNACLE_TRIGGER_GLOBAL_THREAD)
    struct k_work work;
#endif
#endif
};

struct pinnacle_config {
#if DT_INST_ON_BUS(0, i2c)
	const struct i2c_dt_spec bus;
#elif DT_INST_ON_BUS(0, spi)
	const struct spi_dt_spec bus;
#endif
    bool rotate_90, sleep_en, no_taps;
#ifdef CONFIG_PINNACLE_TRIGGER
    const struct gpio_dt_spec dr;
#endif
};
