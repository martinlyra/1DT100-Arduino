// reverse order due to connections
int pins_out[] = {5,4,3,2};
int pins_out_max = sizeof(pins_out)/sizeof(pins_out[0]);

// reverse order due to connections
int pins_in[] = {9,8,7,6};
int pins_in_max = sizeof(pins_in)/sizeof(pins_in[0]);

// pins where there are leds connected, keep in mind that they should had been reversed
int pins_led[] = {10,11,12,13};
int pins_led_max = sizeof(pins_led)/sizeof(pins_led[0]);

// put in reversed where columns is x and rows y ( [columns][rows] ), this is due to the nature of the code
// see led() for more clues
int leds[4][4] = {
  {	13,	11,	0,	0	},
  {	12,	10,	0,	0	},
  {	0,	0,	0,	0	},
  {	0,	0,	0,	0	}
};

// for the keypad matrix, -1 = nothing of interest
int pressed_pin = -1;
int pressed_row, pressed_column;

// intervals
long previousMillis = 0;
long interval = 10; // 10 ms

// setup our pins by iterating through them in for-loops
// and a serial connection for monitoring and debugging
void setup() {
	for (int i = 0; i < pins_out_max; i++)
      pinMode(pins_out[i],OUTPUT);
  	for (int i = 0; i < pins_led_max; i++)
      pinMode(pins_led[i],OUTPUT);
	for (int i = 0; i < pins_in_max; i++)
      pinMode(pins_in[i],INPUT);
  	Serial.begin(9600);
}

// is the button (still) pressed down? (by reading the state from previously known pin and assuring that it is not -1)
bool isPressed()
{
 	return digitalRead(pressed_pin) && pressed_pin != -1; 
}

// blink through the leds according to what button is being held (by row and column)
// also prints information to a monitor via a serial connection
void led(int row, int column) {
  	for (int x = 0; x < 4; x++)
      for (int y = 0; y < 4; y++)
      	digitalWrite(leds[x][y],LOW);
    Serial.print(row);
  	Serial.println(column);
  	if (row >= 0 && column >= 0)
	  digitalWrite(leds[column][row],HIGH);
}

// check for when the button is being pressed - and held down, which will omit as "true", else "false" when not pressed
bool pressed;

// let's -really- read the input!
void readInput() {
  	if (isPressed()) // has anything been changed since last we checked? if no, don't bother
      return;
    
  	pressed = false;	// reset the information so they aren't carried over and cause errorenous behaviour
    pressed_row = pressed_column = -1;
  
 	for (int i = 0; i < pins_out_max; i++)
    {
      digitalWrite(pins_out[i],HIGH);	// pulse through the output pins to make the matrix useable
      for (int I = 0; I < pins_in_max; I++)
      {
        pressed_pin = digitalRead(pins_in[I]) ? pins_in[I] : -1; // find the first pin with a HIGH signal
        if(pressed_pin >= 0)					// if the line above found a pin (when not -1), do this
        {
          pressed = true;
          pressed_row = I;
          pressed_column = i;
          break;
        }
      }
      digitalWrite(pins_out[i],LOW);	// we turn it off as we had finished checking for any input..
      if (pressed)			// ..proceed to next pin if there was no input
        break;
    } 
}

// an artifcat from development -- it prints out the input status of the pins specified in pins_in
void checkInput()
{
  for (int i = 0; i < pins_in_max; i++)
    Serial.print(digitalRead(pins_in[i]));
  Serial.print('\n');
}

// this is where we do the job
void loop() {
  long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) // just interval things; check if 10 ms has elapsed 
  {
    readInput();
    led(pressed_row,pressed_column);
  }
}
