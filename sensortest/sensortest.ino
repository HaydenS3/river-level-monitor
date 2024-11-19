//https://gitlab.com/devgiants/embedded/arduino/libraries/jsn-sr-04t/-/blob/main/examples/distance_read/distance_read.ino?ref_type=heads

#include <AsyncDelay.h>
#include <jsnsr04t.h>

#define ECHO_PIN 2
#define TRIGGER_PIN 4


// Log levels are described here : https://github.com/thijse/Arduino-Log/#usage
JsnSr04T ultrasonicSensor(ECHO_PIN, TRIGGER_PIN, LOG_LEVEL_VERBOSE);
AsyncDelay measureDelay;

void setup()
{
  Serial.begin(115200);
  ultrasonicSensor.begin(Serial);
  measureDelay.start(500, AsyncDelay::MILLIS);
}

void loop()
{
    if (measureDelay.isExpired()) {
        ultrasonicSensor.readDistance();      
        measureDelay.repeat();
    }        
}
