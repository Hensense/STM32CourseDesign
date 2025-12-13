#ifndef PID_H
#define PID_H


/**
 *  PID controller class
 */
class PIDController
{
public:
    /**
     *  
     * @param P - P增益
     * @param I - I增益
     * @param D - D增益 
     * @param ramp - 输出值的最大变化速度
     * @param limit - 最大输出值
     */
    PIDController(float P, float I, float D, float ramp, float limit);
    ~PIDController(){};

    float operator()(float error);

    float P; //!< Proportional gain 
    float I; //!< Integral gain 
    float D; //!< Derivative gain 
    float output_ramp; //!< Maximum speed of change of the output value
    float limit; //!< Maximum output value

protected:
    float error_prev; //!< last tracking error value
    float output_prev;  //!< last pid output value
    float integral_prev; //!< last integral component value
    unsigned long timestamp_prev; //!< Last execution timestamp
};

#endif // PID_H
