/**


   Based in part on code from the following:

      - Ian Buckley for an article on makeuseof.com

      - Author of https://www.instructables.com/id/Using-Infrared-Sensor-With-Arduino/
*/

//IR receiver module
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <boarddefs.h>
#include <IRremoteInt.h>
#include <IRremote.h>

/**
   Pin settings
*/
//define pins for the red, green and blue LEDs
#define RED_LED 6
#define BLUE_LED 5
#define GREEN_LED 9
#define RECV_PIN 13          //  The digital pin that the signal pin of the sensor is connected to


/**
 * LED Settings and variables
 */
//100% duty cycle of pwm
const int MAX_BRIGHTNESS = 255;

//speed of change as variable so can be altered
int fadeSpeed = 10;


//individual brightness values for the red, green and blue LEDs
// We keep these as global so that fading tasks start from current value
// rather than starting from 0 or max
//int gBright = 0;
//int rBright = 0;
//int bBright = 0;

struct LED {
  int address;
  int brightness;
};

struct LED RedLED;
struct LED GreenLED;
struct LED BlueLED;

/**
   IR receiver settings
*/

IRrecv receiver(RECV_PIN);  //  Create a new receiver object that would decode signals to key codes
decode_results results;     //  A varuable that would be used by receiver to put the key code into

/**
   Mapping of remote codes
*/
const int DEFAULT_ON = 0xFFA25D;
const int ALL_OFF = 0xFFE21D;
const int RED_UP = 0xFF30CF;
const int RED_DOWN = 0xFF10EF;
const int GREEN_UP = 0xFF18E7;
const int GREEN_DOWN = 0xFF38C7;
const int BLUE_UP = 0xFF7A85;
const int BLUE_DOWN = 0xFF5AA5;

void setup() {
  //set up pins to output.
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  //  Enable receiver so that will start processing infrared signals
  receiver.enableIRIn();

  //initialize objects
  RedLED.address = RED_LED;
  RedLED.brightness = 0;

  GreenLED.address = GREEN_LED;
  GreenLED.brightness = 0;

  BlueLED.address = BLUE_LED;
  BlueLED.brightness = 0;


  //  //Call the TurnOn method, wait, then call TurnOff
  //  TurnOn();
  //  delay(5000);
  //  TurnOff();
}


//void FadeByColor() {
//  //Call the TurnOn method, wait, then call TurnOff
//  AnalogTurnOn();
//  delay(5000);
//  AnalogTurnOff();
//}

/**
   Raises the brightness of the given led by 1 step
   If the led is already at max, does nothing
*/
void IncrementOn(struct LED *led) {
  //  if (led->brightness < MAX_BRIGHTNESS) {
  led->brightness += 1;
  analogWrite(led->address, led->brightness);
  //  }
}

/**
   Reduces the brightness of the given led by 1 step.

   If brightness already is 0, does nothing
*/
void IncrementOff(struct LED *led) {
  //  if (led_brightness > 0) {
  led->brightness -= 1;
  analogWrite(led->address, led->brightness);
  //  }
}

/**
   Turns the given LED fully on
*/
void FullOn(struct LED *led) {
  led->brightness = MAX_BRIGHTNESS;
  analogWrite(led->address, led->brightness);
}

/**
   Turns the given LED fully off
*/
void FullOff(struct LED *led) {
  led->brightness = 0;
  analogWrite(led->address, led->brightness);
}

//
///**
//   Smoothly fades LED on.
//
//   If LED is aready on, will turn off then fade on
//   led_brightness should be one of rBright, bBright, gBright
//*/
//void FadeOn(struct LED *led) {
//
//  for (int i = 0; i < MAX_BRIGHTNESS + 1; i++) {
//    IncrementOn(&led);
//    //    led->brightness += 1;
//    //    analogWrite(led->address, led->brightness);
//    delay(fadeSpeed);
//  }
//
//}

///**
//   Smoothly fades LED off
//   led_brightness should be one of rBright, bBright, gBright
//*/
//void FadeOff(struct LED *led) {
//  for (int i = MAX_BRIGHTNESS; i >= 0; i--) {
//    IncrementOff(&led);
//    //    analogWrite(led_pin, i );
//    delay(fadeSpeed);
//  }
//}



void loop() {

  if (receiver.decode(&results)) {            //  Decode the button code and put it in "results" variable

    switch (results.value) {

      case DEFAULT_ON:
        FullOn(&RedLED);
        break;

      case ALL_OFF:
        FullOff(&RedLED);
        FullOff(&GreenLED);
        FullOff(&BlueLED);
        break;

        //      case RED_UP:
        //        IncrementOn(RED_LED, rBright);
        //        break;
        //
        //      case RED_DOWN:
        //        IncrementOff(RED_LED, rBright);
        //        break;
        //
        //      case GREEN_UP:
        //        IncrementOn(GREEN_LED, gBright);
        //        break;
        //
        //      case GREEN_DOWN:
        //        IncrementOff(GREEN_LED, gBright);
        //        break;
        //
        //      case BLUE_UP:
        //        IncrementOn(BLUE_LED, bBright);
        //        break;
        //
        //      case BLUE_DOWN:
        //        IncrementOff(BLUE_LED, bBright);
        //        break;
        //
    }
  }


}
