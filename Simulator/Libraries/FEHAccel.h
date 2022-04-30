#ifndef FEHACCEL_H
#define FEHACCEL_H

class FEHAccel
{
public:
    FEHAccel();
    /**
     * @brief Returns the forces acting on the X axis of the Proteus
     *
     * @return double Forces acting on the X axis of the Proteus (in G's)
     */
    double X();
    /**
     * @brief Returns the forces acting on the Y axis of the Proteus
     *
     * @return double Forces acting on the Y axis of the Proteus (in G's)
     */
    double Y();
    /**
     * @brief Returns the forces acting on the Z axis of the Proteus
     *
     * @return double Forces acting on the Z axis of the Proteus (in G's)
     */
    double Z();

    /**
     * @brief Returns the current status of the Proteus accelerometer.
     *
     * If this value is false and you would like to use the accelerometer, talk to a member of your instructional team.
     *
     * @return true if the accelerometer is properly enabled
     */
    bool IsEnabled();
};

/**
 * @brief Global access to the FEHAccel class.
 */
extern FEHAccel Accel;

#endif // FEHACCEL_H
