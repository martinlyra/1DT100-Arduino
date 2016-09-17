// Source code used for task 4 in Laboration 2, 1DT100, Datateknik - introduktion med projektarbete, HT 2016
//
// Name: Arduino-Based Calculator
// Author: Martin Lyrå
// Date (of creation): 2016-09-17
// Libraries: 
//	* Keypad

// Here, we are no longer considering original code. Fullfilling our goal of making a simple calculator is now important.
#include <Keypad.h> //vital for our keypad

// #define FLIP_PINS

// easy pin setup
byte out_pins[] = {5,4,3,2};	// alias column_pins
int max_out_pins = sizeof(out_pins)/sizeof(out_pins[0]); 

byte in_pins[] = {9,8,7,6};		// alias row_pins;
int max_in_pins = sizeof(in_pins)/sizeof(in_pins[0]);

// for when you realize you put the pins in proper order. When it has to be reversed
#ifdef FLIP_PINS
out_pins = {2,3,4,5};
in_pins = {6,7,8,9};
#endif

// NOTE: Not all keypads have the same order of pins for I/O, they can have a different number of pins.
// Specially when the matrix (row x columns) are different.

#define ROWS 	4
#define COLUMNS 4

char keys[ROWS][COLUMNS] =
{
  '1','2','3','A',
  '4','5','6','B',
  '7','8','9','C',
  '*','0','#','D'
};

Keypad kpd = Keypad( makeKeymap(keys), in_pins, out_pins, ROWS, COLUMNS );

// Calculator things

double result			= 0;	// For previous calculations included in the new calculation
String calc_buffer		= ""; 	// we need somewhere to keep the data
byte operation_mode		= 0;
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

#define ADD_CHAR 'A'
#define SUB_CHAR 'B'
#define DIV_CHAR 'C'
#define REM_CHAR 'D'
#define MUL_CHAR '*'
#define EQU_CHAR '#'

void calculate();

void setup() {
  Serial.begin(9600);
  while (!Serial) 
  {
    // wait for the establishment and confirmation of the Serial connection, for native USB connections.
  }
  Serial.println("To reset the calculations, press on the onboard reset button.");
}

void loop() {
  char key = kpd.getKey();  // self-explainatory? it gets the currently pressed key
  
  if (key != NO_KEY) {
    //Serial.println(key); // for debugging the input
    switch (key){
      case EQU_CHAR : { // equal, when pressing #
        calculate();
        operation_mode = 0;
        break;
      }
      case ADD_CHAR : { // addition, when pressing A
        calculate();
        operation_mode = ADD;
        break;
      }
      case SUB_CHAR : { // subtraction, when pressing B
        calculate();
        operation_mode = SUB;
        break;
      }
      case DIV_CHAR : { // division, when pressing C
        calculate();
      	operation_mode = DIV;
        break;
      }
      case MUL_CHAR : { // multiplication, when pressing *
        calculate();
       	operation_mode = MUL;
        break;
      }
      case REM_CHAR : { // undo the last step, when pressing D
       	calc_buffer.remove(calc_buffer.length()-1,1); //removes the last char
        break;
      }
      default : { // all the numbers!
       	if (calc_buffer == "0") // replace 0 with other digits, because [integer] numbers do not start with zero
          calc_buffer = "";
        calc_buffer += key;
        break;
      }
    }
    //Serial.println(calc_buffer); // for debugging the output
    
    Serial.print(result); 
  	switch (operation_mode) {
   		case ADD : Serial.print("\t + \t"); break;
    	case SUB : Serial.print("\t - \t"); break;
    	case DIV : Serial.print("\t / \t"); break;
    	case MUL : Serial.print("\t * \t"); break;
  	}
  	Serial.println(calc_buffer);
  }
  delay(1);
}

void calculate() // Calculate the result depending on the operation mode. Print result. Clear buffer. Move on.
{
  Serial.print(result); 
  switch (operation_mode) {
   	case ADD : result += calc_buffer.toInt(); Serial.print("\t + \t"); break;
    case SUB : result -= calc_buffer.toInt(); Serial.print("\t - \t"); break;
    case DIV : result /= calc_buffer.toInt(); Serial.print("\t / \t"); break;
    case MUL : result *= calc_buffer.toInt(); Serial.print("\t * \t"); break;
  }
  Serial.println(calc_buffer);
  Serial.print("\t= "); Serial.println(result); // print the result
  calc_buffer = ""; // clear the buffer
}
