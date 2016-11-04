int pins_out[] = {5,4,3,2};
int pins_out_max = sizeof(pins_out)/sizeof(pins_out[0]);

int pins_in[] = {9,8,7,6};
int pins_in_max = sizeof(pins_in)/sizeof(pins_in[0]);

char buttonMap[4][4] = {
  {	'1',	'2',	'3',	'A'	},
  {	'4',	'5',	'6',	'B'	},
  {	'7',	'8',	'9',	'C'	},
  {	'*',	'0',	'#',	'D'	}
};

int pressed_pin = -1;
int pressed_row, pressed_column;

long previousMillis = 0;
long interval = 10; // 10 ms

void setup() {
	for (int i = 0; i < pins_out_max; i++)
      pinMode(pins_out[i],OUTPUT);
	for (int i = 0; i < pins_in_max; i++)
      pinMode(pins_in[i],INPUT);
  	Serial.begin(9600);
}

bool isPressed()
{
 	return digitalRead(pressed_pin) && pressed_pin != -1; 
}

void parseInput(int row, int column) {
  	if (row >= 0 && column >= 0)
	  Serial.println(buttonMap[row][column]);
}

bool pressed;

void readInput() {
  	if (isPressed())
      return;
    
  	pressed = false;
    pressed_row = pressed_column = -1;
  
 	for (int i = 0; i < pins_out_max; i++)
    {
      digitalWrite(pins_out[i],HIGH);
      for (int I = 0; I < pins_in_max; I++)
      {
        pressed_pin = digitalRead(pins_in[I]) ? pins_in[I] : -1;
        if(pressed_pin >= 0)
        {
          pressed = true;
          pressed_row = I;
          pressed_column = i;
          break;
        }
      }
      digitalWrite(pins_out[i],LOW);
      if (pressed)
        break;
    } 
}

void checkInput()
{
  for (int i = 0; i < pins_in_max; i++)
    Serial.print(digitalRead(pins_in[i]));
  Serial.print('\n');
}

void loop() {
  long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
  	readInput();
    parseInput(pressed_row,pressed_column);
  }
}
