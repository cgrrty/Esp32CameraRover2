#ifndef MOTOR_MOTOR_L9110S_H
#define MOTOR_MOTOR_L9110S_H

#include "../gpio/pwm.h"

/**
 * Class to encapsulate the L9110S motor controller
 */
class MotorL9110s {
    private:
        PwmChannel *_forwardPin = nullptr;
        PwmChannel *_reversePin = nullptr;

        // current statte
        bool _attached = false;
        bool _forward = false;
        pwm_type _pwm = 0;
        pwm_type _stall_pwm = 1;    // pwm below which the motor will stall

    public:

    MotorL9110s()
    {
        // no-op
    }

    /**
     * Get bit resolution of motor driver.
     * The PwmChannels must use this value.
     */
    static inline pwm_resolution_type pwmBits()    // RET: bit resolution of pwn signal
    {
        return 8;
    }

    /**
     * Get the maximum pwm value supported by the motor
     */
    static inline pwm_type maxPwm()  // RET: the maximum allows pwm value
    {
        return ((unsigned int)1 << MotorL9110s::pwmBits()) - 1;
    }

    /**
     * Get the motor stall value.
     * This is the pwm below which the motor will stall,
     * and so is the pwm of minimal velocity.
     */
    pwm_type stallPwm() // RET: the pwm below which the motor will stall
    {
        return this->_stall_pwm;
    }

    /**
     * Set the measured motor stall value
     * This is the pwm below which the motor will stall,
     * and so is the pwm of minimal velocity.
     * Logically, this cannot be zero, so this method
     * enforces a minimum of 1.
     */
    MotorL9110s& setStallPwm(pwm_type pwm)  // IN : pwm below which motor will stall
                                            // RET: this motor
    {
        this->_stall_pwm = max<pwm_type>(1, min<pwm_type>(pwm, maxPwm()));
        return *this;
    }

    /**
     * Get motor's current direction
     */
    bool forward();  // RET: true if current pwm value is a forward value

    /**
     * Get motor's current pwm value 
     */
    pwm_type pwm();  // RET: current pwm value (0 to (1 << pwmBits()) - 1)

    /**
     * Determine if dependencies (like pwm channel) are attached.
     */
    bool attached();    // RET: true if dependencies attached 

    /**
     * Attach dependencies to the motor 
     * - set the output pins to pwm write mode
     */
    MotorL9110s& attach(
        PwmChannel &forwardPin, // IN : pin for forward PWM input
                                //      MUST exist until detach is called 
        PwmChannel &reversePin);// IN : pin for reverse PWM input
                                //      MUST exist until detach is called
                                // RET: the attached motor

    /**
     * Detach the motor
     * - stop the motor
     * - if possible, revert output pins to default state
     */
    MotorL9110s& detach();  // RET: the detached motor.

    /**
     * Set the power (pwm duty cycle) for the motor
     */
    void setPower(
        bool forward,   // IN : true for forward PWM, 
                        //      false for reverse PWM
        pwm_type pwm);  // IN : pwm value; zero is stopped
                        //      ((1 << pwmBits()) - 1) is full power
};

#endif // MOTOR_MOTOR_L9110S_H
