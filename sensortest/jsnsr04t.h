/*
  jsnsr04t.h - Library for using ultrasonic sensor driver JSN-SR-04T.
  Created by Overflaw, August 28, 2023
  Released into the public domain.
*/
#ifndef JSNSR04T_H
    #define JSNSR04T_H

    #define JSNSR04T_PULSE_LOW_US           5
    #define JSNSR04T_PULSE_HIGH_US          30
    #define METER_CONVERSION_COEFFICIENT    58

    #include <ArduinoLog.h>
    #include <AsyncDelay.h>
    #include "Arduino.h"

    class JsnSr04T
    {
        public:
            JsnSr04T(int echoPin, int triggerPin, int logLevel = LOG_LEVEL_VERBOSE);
            void begin(Stream &serial);
            int readDistance();
        private:
            int _triggerPin;
            int _echoPin;
            int _logLevel;
            Stream* _serial;
            AsyncDelay _pulseLow;
            AsyncDelay _pulseHigh;            

            void initiatesPulse();
            int calculateDistance(long echoDurationUs);
    };
#endif