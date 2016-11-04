// reverse order due to connections
int pins_out[] = {5,4,3,2};
int pins_out_max = sizeof(pins_out)/sizeof(pins_out[0]);

// reverse order due to connections
int pins_in[] = {9,8,7,6};
int pins_in_max = sizeof(pins_in)/sizeof(pins_in[0]);

// two-dimensional array acting as the matrix of characters mapped to the buttons of a 4x4 keypad
char buttonMap[4][4] = {
  {	'1',	'2',	'3',	'A'	},
  {	'4',	'5',	'6',	'B'	},
  {	'7',	'8',	'9',	'C'	},
  {	'*',	'0',	'#',	'D'	}
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
	for (int i = 0; i < pins_in_max; i++)
      pinMode(pins_in[i],INPUT);
  	Serial.begin(9600);
}

// is the button (still) pressed down? (by reading the state from previously known pin and assuring that it is not -1)
bool isPressed()
{
 	return digitalRead(pressed_pin) && pressed_pin != -1; 
}

// get the character the specified key (row and column) is mapped to
// then print it to your serial monitor, does this every 10ms
void parseInput(int row, int column) {
  	if (row >= 0 && column >= 0)
	  Serial.println(buttonMap[row][column]);
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
    parseInput(pressed_row,pressed_column);
  }
}
