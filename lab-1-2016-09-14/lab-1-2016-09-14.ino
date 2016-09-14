// Source code used for task 2 and 3 in Laboration 1, 1DT100, Datateknik - introduktion med projektarbete, HT 2016
//
// Name: Advanced Toggleable Solid/Blink
// Author: Martin Lyrå
// Date (of creation): 2016-09-14


// For easy setup using 'for' loop(s)
int out_pins[] = {0,1,2,3,4,5,6,7,8,9,10,11};
int max_out_pins = sizeof(out_pins)/sizeof(out_pins[0]); 

// Comment for previous line:
// sizeof() on an array will return its size in memory,
// while sizeof on the first element returns the size of THAT element.
// sizeof({0,1,2}) returns the same value as 3*sizeof(int). 
// Because there are three 'int's in the array.
// Now apply the assignment as an equation: 12*sizeof(int)/sizeof(int)
// The answer? 12. - Because a/a = 1 and in this case the sizeof(int) is the 'a'

int in_pins[] = {13};
int max_in_pins = sizeof(in_pins)/sizeof(in_pins[0]);
// end easy pin setup

int button_input_pin = 13;
int button_pressed = LOW; // HIGH (true) the entire time it is being held
int button_state = LOW;

unsigned long previousMillis, currentMillis = 0; // for our beautiful intevals
#define INTERVAL(x) (currentMillis - previousMillis >= x)

// 1 sec = 1000 ms
#define ONE_SECOND 1000

const int USE_PULLUP = 1;

// Since this is a language derived from C/C++, forward declaraions (declaration before use) is a must.
void buttonReadToggle(int pin, int* pressed_state, int* state);

void setup() {
  // Setup all output pins defined in out_pins
  for (int i = 0; i < max_out_pins; i++)
  {
    pinMode(out_pins[i],OUTPUT);
  }

  // Setup all input pins defined in in_pins
  for (int i = 0; i < max_in_pins; i++)
  {
    pinMode(in_pins[i], (USE_PULLUP ? INPUT_PULLUP : INPUT)); // if USE_PULLUP is 1, use INPUT_PULLUP, else use INPUT ( statement ? true : fale )
  }
}

void loop() {
  
  currentMillis = millis();
  
  buttonReadToggle(button_input_pin, &button_pressed, &button_state);

  if (currentMillis - previousMillis >= ONE_SECOND)
  {
    // lit up the LEDs connected to pins as defined in out_pins. Given that they are shunted (as in parallel connection) to a 10k ohm resistor, connected to one of the GND (GrouNDing) pin outlets.
    // daisy-connecting the LEDs (chain/serial connection) will just lit the LED and shut off the LED in the previous iteration.

    for (int i = 0; i < max_out_pins; i++)
    {
      digitalWrite(out_pins[i],HIGH);
      delay(10);
    }
    
    // in the first mode, the previous lit lamps will turned off by one and one after a barely noticeable delay (0.1 seconds), else they still stay lit
    if (!button_state && INTERVAL(100))
      for (int i = 0; i < max_out_pins; i++)
      {
        digitalWrite(out_pins[i],LOW);  
        delay(10);
      }
  }
}
  //delay(ONE_SECOND); // <-- Inpractical for responsive setups, but fast and cheap. Application of millis() is more efficient and responsive, but complex.


// Function for controlling the button. If the button is being held, this function still considers it as a single press. Press again to change again.
// It uses pointers to variables (pressed_state and state, passed by reference) so it can modify them here rather than adapting to a more complex [spaghetti] code.
void buttonReadToggle(int pin, int* pressed_state, int* state)
{
  if (digitalRead(pin))
  {
     if (!*pressed_state) // is it the first time we do this? If yes, do this. Otherwise, don't.
      *state = !*state;   // this is a TOGGLE function, assignment using the ! operator "toggles" the state.
     *pressed_state = 1;  // Go on, there is nothing to see anymore.
  } else
    *pressed_state = 0;   // Since we are not using the button anymore. Take down the barrier so we can use the button again.
}

