#include "Sensors/magnetometer.h"
#include <stdint.h>

/*===============================*/
/* Local functions declarations  */
/*===============================*/

/*===============================*/
/* Global data definition        */
/*===============================*/

BMM150 bmm = BMM150();

/*===============================*/
/* Functions definitions         */
/*===============================*/

void magnetometerInit( void )
{
  if (bmm.initialize() == BMM150_E_ID_NOT_CONFORM) 
  {
    while (1);
  } 
};

void getAzimuth(float * const retValue)
{
    static unsigned long previousMillis = 0U; // Zmienna przechowująca czas ostatniego wywołania
    const unsigned long minInterval = 100U; // Minimalny interwał między wywołaniami w ms

    unsigned long currentMillis = millis(); // Aktualny czas

    // Sprawdzenie, czy minęło 100 ms od ostatniego wywołania
    if (currentMillis - previousMillis < minInterval) 
    {
        unsigned long waitTime = minInterval - (currentMillis - previousMillis);
        delay(waitTime); // Oczekiwanie na resztę czasu
    }

    // Aktualizacja poprzedniego czasu wywołania
    previousMillis = millis();

    // Odczyt danych z magnetometru
    bmm.read_mag_data();

    // Obliczenie azymutu
    *retValue = atan2((bmm.raw_mag_data.raw_datax - OFFSET_X), (bmm.raw_mag_data.raw_datay - OFFSET_Y));

    // Korekta wyniku, aby mieścił się w przedziale 0 do 2*PI
    if (*retValue < 0.0F) 
    {
        *retValue += 2.0F * PI;
    } 
    else if (*retValue > 2.0F * PI) 
    {
        *retValue -= 2.0F * PI;
    }

    // Konwersja azymutu na stopnie
    *retValue = *retValue * 180.0F / M_PI;
}
