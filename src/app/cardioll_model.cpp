# include "cardiollTClient.h"
# include <math.h>

const String CardiollClient::detect_trachidia(int age, int sex, float HR, float PR, float QRS, float QT, float QTC, float RR)
{
    float E = exp(-166.643 + (0.301 * age) + 
    (11.662 * sex) + (16.675 * HR) + 
    (176.638 * PR) + (-231.348 * QRS) + 
    (-1227.175 * QT) + (1132.294 * QTC) + 
    (70.832 * RR));

    float E2 = E / (1 + E);

    return (E2 > 0.5) ? "TRACHYCARDIA: PRESENT" : "TRACHYCARDIA: ABSENT";
}