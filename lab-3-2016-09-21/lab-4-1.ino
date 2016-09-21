int out_pins[] = {2,3,4,5,6,7,8,9,10,11,12,13};
int max_out_pins = sizeof(out_pins)/sizeof(out_pins[0]);

int high_pins[] = {2,3,4,5,6,7,8,9,10}; // alias column_pins
int max_high_pins = sizeof(high_pins)/sizeof(high_pins[0]);

int low_pins[] = {11,12,13};            // alias layer_pins
int max_low_pins = sizeof(low_pins)/sizeof(low_pins[0]);

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

void loop() {

  for (int i_layer = 0; i_layer < max_low_pins; i_layer++)
  {
    digitalWrite(low_pins[i_layer],LOW); // turn on the selected layer

    for(int i_column = 0; i_column < max_high_pins; i_column++)
    {
      digitalWrite(high_pins[i_column], HIGH);  // turn on selected LED
      delay(500);                               // wait 500 ms (half a second)
      digitalWrite(high_pins[i_column], LOW);   // turn off selected LED as we move on the next LED
    }
    
    digitalWrite(low_pins[i_layer],HIGH); // we are done; turn off the selected layer
  }
