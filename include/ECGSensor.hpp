#ifndef ECGSampling_h
#define ECGSampling_h
#include <Arduino.h>

class ECGSensor{
    private:
        // The hardware timer pointer
        hw_timer_t * timer = NULL;
        uint16_t analog_pin;
        uint16_t sample_per_second;

    public:
        ECGSensor(uint16_t analog_pin, uint16_t sample_per_second){

        }

}

#endif