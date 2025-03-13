#include "../include/Trajectory.h"

void Trajectory::computeParameters(const float qi[4], const float qf[4]){
    xi = qi[0], yi = qi[1], thetai = qi[2]; ti = qi[3];
    xf = qf[0], yf = qf[1], thetaf = qf[2]; tf = qf[3];

    b0 = xi;
    b1 = 0;
    b2 = 3*(xf - xi);
    b3 = -2*(xf - xi);
    Serial.print("xi = "); Serial.print(xi); Serial.print("; xf = "); Serial.println(xf);
    Serial.print("yi = "); Serial.print(yi); Serial.print("; yf = "); Serial.println(yf);
    Serial.print("thetai = "); Serial.print(thetai); Serial.print("; thetaf = "); Serial.println(thetaf);

    double alpha = tan(thetai)*(xf - xi);
    double beta = tan(thetaf)*(xf - xi);

    a0 = yi;
    a1 = alpha;
    a2 = 3*(yf - yi) - 2*alpha - beta;
    a3 = -2*(yf - yi) + alpha + beta;

    Serial.print("a0 = "); Serial.print(a0); Serial.print("; a1 = "); Serial.print(a1); Serial.print("; a2 = "); Serial.print(a2); Serial.print("; a3 = "); Serial.println(a3);
    Serial.print("b0 = "); Serial.print(b0); Serial.print("; b1 = "); Serial.print(b1); Serial.print("; b2 = "); Serial.print(b2); Serial.print("; b3 = "); Serial.println(b3);
    Serial.print("alpha = "); Serial.print(alpha); Serial.print("; beta= "); Serial.println(beta);

}

Point Trajectory::computeTraj(float x, float t){
    //Serial.print("ti = "); Serial.print(ti); Serial.print("; tf = "); Serial.println(tf);
    //Serial.print("xi = "); Serial.print(xi); Serial.print("; xf = "); Serial.println(xf);
    //Serial.print("x = "); Serial.println(x);
    const float tau = (t - ti)/(tf - ti);
    const float d = (x - xi)/(xf - xi);

    float x_traj, y_traj;

    if (xf != xi){
        x_traj = b0 + b1*tau + b2 * pow(tau, 2) + b3*pow(tau, 3);
        y_traj = a0 + a1*d + a2*pow(d, 2) + a3*pow(d, 3);
    } else {
        x_traj = xi;
        y_traj = yf;
    }
    //Serial.print("X_traj live = "); Serial.print(x_traj);
    //Serial.print("; Y_traj live = "); Serial.println(y_traj);

    //Serial.print("tau = "); Serial.print(tau); Serial.print("; d = "); Serial.println(d);
    return Point(x_traj, y_traj);
}

Trajectory::Trajectory(): ti(0), tf(0), xi(0), yi(0), thetai(0), xf(0), yf(0), thetaf(0), a0(0), a1(0), a2(0), a3(0), b0(0), b1(0), b2(0), b3(0) {}