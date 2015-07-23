#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <cmath>

class PIDController
{
public:
    PIDController();
    void Initialise(double Kp, double Ki, double Kd, double error_thresh, double min_integral_error, double step_time);
    double Update(double error);
    void updateParameters(double Kp, double Ki, double Kd, double error_thresh, double min_integral_error, double step_time);
    void updateIntegralError(double min_integral_error);
    void reset();
private:
    bool m_started;
    double m_kp, m_ki, m_kd, m_h, m_inv_h, m_prev_error, m_error_thresh, m_integral, integral_error_thres;
};

#endif // PIDCONTROLLER_H
