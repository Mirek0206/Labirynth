#ifndef CALMANFILTER_H
#define CALMANFILTER_H

/*===============================*/
/* Includes                      */
/*===============================*/

/*===============================*/
/* Types definitions             */
/*===============================*/

typedef struct calmanFilter_tag{
    float q_f32; // Szum procesu
    float r_f32; // Szum pomiaru
    float x_f32; // Wartość estymowana
    float p_f32; // Błąd estymowany
    float k_f32; // Wzmocnienie Kalmana
} calmanFilter_t;

/*===============================*/
/* Global data                   */
/*===============================*/

/*===============================*/
/* Global functions declarations */
/*===============================*/

void calmanFilterInit(calmanFilter_t * const kf, const float q_f32, const float r_f32, const float initial_value);

float calmanFilterUpdate(calmanFilter_t * const kf, const float measurement_f32);

#endif