/*
 * Copyright (c) 2020-2021 Aspeed Technology Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PWM_ASPEED_H_
#define _PWM_ASPEED_H_
/**********************************************************
 * PWM register fields
 *********************************************************/
union pwm_general_register_s {
	volatile uint32_t value;
	struct {
		volatile uint32_t pwm_clock_division_l: 8;            /*[0-7]*/
		volatile uint32_t pwm_clock_division_h: 4;            /*[8-11]*/
		volatile uint32_t enable_pwm_pin: 1;                  /*[12-12]*/
		volatile uint32_t enable_open_drain: 1;               /*[13-13]*/
		volatile uint32_t inverse_pwm_pin: 1;                 /*[14-14]*/
		volatile uint32_t output_pwm_level: 1;                /*[15-15]*/
		volatile uint32_t enable_pwm_clock: 1;                /*[16-16]*/
		volatile uint32_t disable_pwm_duty_instant_change: 1; /*[17-17]*/
		volatile uint32_t enable_pwm_duty_load_as_wdt: 1;     /*[18-18]*/
		volatile uint32_t load_selection_of_duty_as_wdt: 1;   /*[19-19]*/
		volatile uint32_t reserved: 12;                       /*[20-31]*/

	} fields;
}; /* 00000000 */

union pwm_duty_cycle_register_s {
	volatile uint32_t value;
	struct {
		volatile uint32_t pwm_rising_point: 8;                /*[0-7]*/
		volatile uint32_t pwm_falling_point: 8;               /*[8-15]*/
		volatile uint32_t pwm_rising_falling_point_as_wdt: 8; /*[16-23]*/
		volatile uint32_t pwm_period: 8;                      /*[24-31]*/

	} fields;
}; /* 00000004 */

struct pwm_gather_register_s {
	union pwm_general_register_s pwm_general;
	union pwm_duty_cycle_register_s pwm_duty_cycle;
	uint32_t reserved[2];
};

union pwm_g100_register_s {
	volatile uint32_t value;
	struct {
		volatile uint32_t pwm_clock_gating: 16; /*[0-15]*/
		volatile uint32_t reserved: 16;         /*[16-31]*/

	} fields;
}; /* 00000100 */

union pwm_g104_register_s {
	volatile uint32_t value;
	struct {
		volatile uint32_t pwm_duty_gating: 16; /*[0-15]*/
		volatile uint32_t reserved: 16;        /*[16-31]*/

	} fields;
}; /* 00000104 */

union pwm_g108_register_s {
	volatile uint32_t value;
	struct {
		volatile uint32_t reserved: 32; /*[0-31]*/

	} fields;
}; /* 00000108 */

union pwm_g10c_register_s {
	volatile uint32_t value;
	struct {
		volatile uint32_t interrupt_status: 16; /*[0-15]*/
		volatile uint32_t reserved: 16;         /*[16-31]*/

	} fields;
}; /* 0000010c */

struct pwm_register_s {
	struct pwm_gather_register_s pwm_gather[16];
	union pwm_g100_register_s pwm_g100; /* 00000100 */
	union pwm_g104_register_s pwm_g104; /* 00000104 */
	union pwm_g108_register_s pwm_g108; /* 00000108 */
	union pwm_g10c_register_s pwm_g10c; /* 0000010c */
};

#endif /* end of "#ifndef _PWM_ASPEED_H_" */
