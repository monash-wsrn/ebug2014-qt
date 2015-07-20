#include "PIDController.h"

PIDController::PIDController() {

}

void PIDController::Initialise(double Kp, double Ki, double Kd, double error_thresh, double min_integral_error, double step_time)
{
    // Initialize controller parameters
    m_kp = Kp;
    m_ki = Ki;
    m_kd = Kd;
    m_error_thresh = error_thresh;//steady state error
    integral_error_thres = min_integral_error; //min error to activate integral component
    // Controller step time and its inverse
    m_h = step_time;
    m_inv_h = 1 / step_time;

    // Initialize integral and derivative calculations
    m_integral = 0;
    m_started = false;
}

double PIDController::Update(double error)
{
    // Set q to 1 if the error magnitude is below
    // the threshold and 0 otherwise
    double q;


    if (fabs(error) < m_error_thresh)
        q = 1;
    else {
        q = 0;
        m_integral = 0;
    }

    if (fabs(error) < integral_error_thres) m_integral = 0;
    else m_integral += m_h*q*error; // Update the error integral

    // Compute the error derivative
    double deriv;
    if (!m_started)
    {
        m_started = true;
        deriv = 0;
    }
    else
        deriv = (error - m_prev_error) * m_inv_h;

    m_prev_error = error;

    // Return the PID controller actuator command
    return (m_kp*error + m_ki*m_integral + m_kd*deriv);
}

void PIDController::updateParameters(double Kp, double Ki, double Kd, double error_thresh, double min_integral_error, double step_time)
{
    m_kp = Kp;
    m_ki = Ki;
    m_kd = Kd;
    m_error_thresh = error_thresh;//steady state error
    integral_error_thres = min_integral_error; //min error to activate integral component
}

void PIDController::updateIntegralError(double min_integral_error)
{
    integral_error_thres = min_integral_error; //min error to activate integral component
}

void PIDController::reset()
{
    m_integral = 0;
    m_started = false;
} // updateParameters()
