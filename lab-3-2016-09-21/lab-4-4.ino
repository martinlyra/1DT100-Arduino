int out_pins[] = {2,3,4,5,6,7,8,9,10,11,12,13};
int max_out_pins = sizeof(out_pins)/sizeof(out_pins[0]);

int high_pins[] = {2,3,4,5,6,7,8,9,10}; // alias column_pins
int max_high_pins = sizeof(high_pins)/sizeof(high_pins[0]);

int low_pins[] = {11,12,13};            // alias layer_pins
int max_low_pins = sizeof(low_pins)/sizeof(low_pins[0]);

#define LAYERS 3
#define COLUMNS 9

// I like to call this the "frame buffer" - it keeps a map of which LEDs should be turned on or not
// 0 = LOW
// 1 = HIGH
// There are 9 columns, starting from 0 to 8, pins 2 to 10 (see high_pins)
int state_map[LAYERS][COLUMNS] = 
{
  {0,0,0,0,0,0,0,0,0}, // first layer
  {0,0,0,0,0,0,0,0,0}, // second layer
  {0,0,0,0,0,0,0,0,0}  // third layer
};

// how many frames do we have got?
#define FRAMES 4

// This is where we hold our beautiful animation frames, an array of pre-defined state_maps to use.
int animation_map[FRAMES][LAYERS][COLUMNS] = 
{
  { // '-'                   first frame
    {0,0,0,0,0,0,0,0,0},  // first layer
    {1,1,1,1,1,1,1,1,1},  // second layer
    {0,0,0,0,0,0,0,0,0}   // third layer
  },
  { // '/'                   second frame
    {0,0,0,0,0,0,1,1,1},
    {0,0,0,1,1,1,0,0,0},
    {1,1,1,0,0,0,0,0,0}
  },
  { // '|'                   third frame
    {0,0,0,1,1,1,0,0,0},
    {0,0,0,1,1,1,0,0,0},
    {0,0,0,1,1,1,0,0,0}
  },
  { // '\'                   fourth frame
    {1,1,1,0,0,0,0,0,0},
    {0,0,0,1,1,1,0,0,0},
    {0,0,0,0,0,0,1,1,1}
  }
};

void setup() {
  // setup our output pins
  for (int i = 0; i < max_out_pins; i++)
  {
    pinMode(out_pins[i],OUTPUT);
  }
  // make sure the layer pins are "turned off" from start
  for (int i = 0; i < max_low_pins; i++)
  { 
    digitalWrite(low_pins[i],HIGH);
  }
}

// analog input limits
const int upper_analog_0 = 407;
const int lower_analog_0 = 270;

const int upper_analog_1 = 340;
const int lower_analog_1 = 260;

const int upper_analog_2 = 398;
const int lower_analog_2 = 337;

int steps;
double accelval;
int analog0, analog1, analog2;
int CM, PM = 0;
int interval = 500;

void loop() {
  // get the current timestamp
  CM = millis(); 
  
  // get analog input
  analog0 = analogRead(0);
  analog1 = analogRead(1);
  analog2 = analogRead(2);

  // calculate the total by mapping each analog input to their own constant limits,
  // then divide by 3000 (due to three inputs and range between 0 and 1000)
  accelval = (double)(map(analog0, lower_analog_0, upper_analog_0, 0, 1000) +
              map(analog1, lower_analog_1, upper_analog_1, 0, 1000) +
              map(analog2, lower_analog_2, upper_analog_2, 0, 1000) ) / 3000.00; 

  // "blink" according to a map of states
  for (int i = 0; i < 3; i++)
  { 
    digitalWrite(low_pins[i], LOW);             // "turn on" the selected layer
    digitalWrite(low_pins[!i ? 2 : i-1 ],HIGH); // "shut off" the previous layer

      for (int c = 0; c < COLUMNS; c++)
      {
      digitalWrite(high_pins[c], state_map[i][c]);  // write according to the state map specified for layer
      digitalWrite(high_pins[c], LOW);              // since we are blinking, turn it off
      }
  }

  // Check if 500 ms (maximium) has elapsed, the interval is smaller as the more displaced the accelerometer becomes
  if (CM - PM >= interval*accelval)
  {
    for (int i = 0; i < LAYERS; i++)                // copy the selected animation state to the state map
      for (int c = 0; c < COLUMNS;c++)
        state_map[i][c] = animation_map[steps][i][c];
    PM = CM;                            // timestamp since last we did this, for when the next interval should occur
    steps = steps > 2 ? 0 : steps + 1;  // check if we already taken four steps (0,1,2,3), reset to 0. Else take another step 
  }
}
