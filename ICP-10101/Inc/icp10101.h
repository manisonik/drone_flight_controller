/*
 * Dshot2.h
 *
 *  Created on: Oct 14, 2023
 *      Author: hooke
 */

#ifndef ICP10101_H_
#define ICP10101_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* data structure to hold pressure sensor related parameters */
typedef struct inv_invpres
{
	uint32_t min_delay_us;
	uint8_t pressure_en;
	uint8_t temperature_en;
	float sensor_constants[4]; // OTP values
	float p_Pa_calib[3];
	float LUT_lower;
	float LUT_upper;
	float quadr_factor;
	float offst_factor;
	int (*inv_invpres_serif_write_reg)();
} inv_invpres_t;

int inv_invpres_init(struct inv_invpres *s);
int read_otp_from_i2c(struct inv_invpres *s, short *out);
void init_base(struct inv_invpres *s, short *otp);
int inv_invpres_process_data(struct inv_invpres *s,
		int p_LSB,
		int T_LSB,
		float *pressure,
		float *temperature
		);
void calculate_conversion_constants(struct inv_invpres *s, float *p_Pa, float *p_LUT, float *out);

#ifdef __cplusplus
}
#endif
#endif /* ICP10101_H_ */
