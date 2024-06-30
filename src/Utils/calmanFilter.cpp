#include "Utils/calmanFilter.h"

/*===============================*/
/* Local functions declarations  */
/*===============================*/

/*===============================*/
/* Global data definition        */
/*===============================*/

/*===============================*/
/* Functions definitions         */
/*===============================*/

// Inicjalizacja filtru Kalmana
void calmanFilterInit(calmanFilter_t * const kf, const float q_f32, const float r_f32, const float initial_value) {
    kf->q_f32 = q_f32;
    kf->r_f32 = r_f32;
    kf->x_f32 = initial_value;
    kf->p_f32 = 1.0; // Początkowy błąd estymacji
}

// Aktualizacja filtru Kalmana na podstawie nowego pomiaru
float calmanFilterUpdate(calmanFilter_t * const kf, const float measurement_f32) {
    // Predykcja
    kf->p_f32 = kf->p_f32 + kf->q_f32;

    // Obliczenie wzmocnienia Kalmana
    kf->k_f32 = kf->p_f32 / (kf->p_f32 + kf->r_f32);

    // Korekcja
    kf->x_f32 = kf->x_f32 + kf->k_f32 * (measurement_f32 - kf->x_f32);
    kf->p_f32 = (1 - kf->k_f32) * kf->p_f32;

    return kf->x_f32;
}