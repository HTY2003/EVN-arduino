/*readTouch.ino

The following program demonstrates some basic EVNTouchArray functionality.
*/

#include <EVN.h>

#define TOUCH_SENS_PORT 1  //set I2C port for touch array here

EVNAlpha board;
EVNTouchArray touch(TOUCH_SENS_PORT);

void setup()
{
    board.begin();  //initialize board at start of void setup()
    Serial.begin(9600);
    touch.begin();
}

void loop()
{
    bool button0 = touch.read(0);
    bool pressed = touch.pressed(false);
    int last_pressed = touch.lastPressed(false);
    int last_released = touch.lastReleased(false);

    if (button0)
        Serial.println("Button 0: Pressed | ");
    else
        Serial.println("Button 0: Not Pressed | ");

    if (pressed)
        Serial.println("No Button Pressed | ");
    else
        Serial.println("One (or more) Buttons Pressed | ");

    Serial.print("Last Pressed: ");
    Serial.print(last_pressed);
    Serial.print(" Last Released: ");
    Serial.println(last_pressed);

    Serial.println();

    delay(1000);
}