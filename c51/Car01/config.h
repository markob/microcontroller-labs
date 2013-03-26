#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CRYSTAL_FREQUENCY 14765400

// PWM configuration
#define PWM_OUTPUT_PORT P1

sbit PWM_pin11 = PWM_OUTPUT_PORT^0;
sbit PWM_pin12 = PWM_OUTPUT_PORT^1;

sbit PWM_pin21 = PWM_OUTPUT_PORT^2;
sbit PWM_pin22 = PWM_OUTPUT_PORT^3;

#define USE_PHASE_CORRECTED_PWM 0

#endif /*_CONFIG_H_*/
