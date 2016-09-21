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
  {0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0}
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

int steps;
int CM, PM = 0;
int interval = 500;

void loop() {

  CM = millis();

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

  // Check if 500 ms has elapsed
  if (CM - PM >= interval)
  {
    if (steps >= 27){ // if we had done 27 steps, fill the entire state_map with 0's (LOWs)
      for (int i = 0; i < 3; i++)
          for (int c = 0; c < COLUMNS; c++)
            state_map[i][c] = 0;
    }
    else // else, just fill the state_map with '1's, one each step. One layer each 9 steps.
    {
      if (steps < 9)        // first layer
        state_map[0][steps] = 1;
      else if (steps < 18)  // second layer
        state_map[1][steps - 9] = 1;
      else                  // third layer
        state_map[2][steps - 18] = 1;
    }
    //state_map[(int)floor(steps/COLUMNS)][(int)floor(steps/LAYERS)] = 1; // we did an experiment, using floor to terminate decimals from steps - but it did not go as expected
    PM = CM;  // timestamp since last we did this, for when the next interval should occur
    steps = steps == 27 ? 0 : steps + 1;  // check if we already taken 27 steps (0,1,2, ... , 26), reset to 0. Else take another step 
  }
}
