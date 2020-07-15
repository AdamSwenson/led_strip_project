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
#define RED_PIN 6
#define BLUE_PIN 5
#define GREEN_PIN 9
//  The digital pin that the signal pin of the sensor is connected to

#define RECV_PIN 13


/**
   LED Settings and variables
*/
//100% duty cycle of pwm
const int MAX_BRIGHTNESS = 255;

//speed of change as variable so can be altered
int fadeSpeed = 10;
int fadeStep = 51;

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
decode_results results;     //  receiver will put the key code into this

/**
   Mapping of remote codes
*/
const int DEFAULT_ON = 16753245; //0xFFA25D;
const int ALL_OFF = 16769565;  //0xFFE21D;
const int RED_UP = 16724175; //0xFF30CF;
const int RED_DOWN = 16716015; //0xFF10EF;
const int GREEN_UP = 16718055; //0xFF18E7;
const int GREEN_DOWN = 16726215; //0xFF38C7;
const int BLUE_UP = 16743045; //0xFF7A85;
const int BLUE_DOWN = 16734885; //0xFF5AA5;

void setup() {
  //set up pins to output.
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);


  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //  Enable receiver so that will start processing infrared signals
  receiver.enableIRIn();

  //initialize objects
  RedLED.address = RED_PIN;
  RedLED.brightness = 0;

  GreenLED.address = GREEN_PIN;
  GreenLED.brightness = 0;

  BlueLED.address = BLUE_PIN;
  BlueLED.brightness = 0;

  Serial.begin(9600);       //  Setup serial port to send key codes to computer
  Serial.println( "Setup ran : \n" );

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
void IncrementUp(struct LED *led) {
    if (led->brightness < MAX_BRIGHTNESS) {
  led->brightness += fadeStep;
  analogWrite(led->address, led->brightness);
    }
}

/**
   Reduces the brightness of the given led by 1 step.

   If brightness already is 0, does nothing
*/
void IncrementDown(struct LED *led) {
  //  if (led_brightness > 0) {
  led->brightness -= fadeStep;
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
//    IncrementDown(&led);
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
//    IncrementDown(&led);
//    //    analogWrite(led_pin, i );
//    delay(fadeSpeed);
//  }
//}



void loop() {

  if (receiver.decode(&results)) {            //  Decode the button code and put it in "results" variable
    //       Serial.println( "recieved : \n" );
    Serial.println( results.value);       //  Print the code as a hexadecimal value
    //       Serial.println(DEFAULT_ON);       //  Print the code as a hexadecimal value
    //   Serial.println(results.value == DEFAULT_ON);


    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LO
    //  delay(3000);                       // wait for a second
    //  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //  delay(3000);                       // wait for a second
    //  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LO
    //
    switch (results.value) {

      case 16753245:
        //      case DEFAULT_ON:
        //case 0xFFA25D:
        Serial.println('default on');       //  Print the code as a hexadecimal value

        FullOn(&RedLED);

        Serial.println('red bright \n' );       //  Print the code as a hexadecimal value

        Serial.println(RedLED.brightness );       //  Print the code as a hexadecimal value
        break;

      //      case ALL_OFF:
      case 16769565:
        FullOff(&RedLED);
        FullOff(&GreenLED);
        FullOff(&BlueLED);
        break;

      //      case RED_UP:
      case 16724175:
        IncrementUp(&RedLED);
        break;

        //      case RED_DOWN:
      case 16716015:
        IncrementDown(&RedLED);
        break;

      //      case GREEN_UP:
      case 16718055:
        IncrementUp(&GreenLED);
        break;

      //      case GREEN_DOWN:
      case 16726215:
        IncrementDown(&GreenLED);
        break;

      //      case BLUE_UP:
      case 16743045:
        IncrementUp(&BlueLED);
        break;

      //      case BLUE_DOWN:
      case 16734885:
        IncrementDown(&BlueLED);
        break;

    }
    receiver.resume();                        //  Continue listening for new signals

  }


}
