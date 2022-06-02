#ifndef GESTURE_H
#define GESTURE_H
#include "Adafruit_APDS9960.h"

Adafruit_APDS9960 apds;

void initializeGestureSensor()
{
    if (!apds.begin())
    {
        Serial.println("Failed to initialize gesture sensor\n");
    }
    else Serial.println("Gesture sensor initialized");
    apds.enableProximity(true);
    apds.enableGesture(true);
}

/*
void testGestureSensor()
{
    uint8_t gesture = apds.readGesture();
    if(gesture == APDS9960_DOWN) Serial.println("DOWN\n");
    if(gesture == APDS9960_UP) Serial.println("UP\n");
    if(gesture == APDS9960_LEFT) Serial.println("LEFT\n");
    if(gesture == APDS9960_RIGHT) Serial.println("RIGHT\n");
}
*/
#endif