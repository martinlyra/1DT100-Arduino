// Source code used for task 4 in Laboration 2, 1DT100, Datateknik - introduktion med projektarbete, HT 2016
//
// Name: Arduino-Based Calculator
// Author: Martin Lyrå
// Date (of creation): 2016-09-17
// Libraries: 
//	Keypad

// Here, we are no longer considering original code. Fullfilling our goal of making a simple calculator is now important.
#include <Keypad.h> //vital for our keypad

// easy pin setup
byte out_pins[] = {5,4,3,2};	// alias column_pins
int max_out_pins = sizeof(out_pins)/sizeof(out_pins[0]); 

byte in_pins[] = {9,8,7,6};	// alias row_pins;
int max_in_pins = sizeof(in_pins)/sizeof(in_pins[0]);


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

int result				= 0;
String calc_buffer		= ""; // we need somewhere to keep the data
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
}

void loop() {
  char key = kpd.getKey();
  
  if (key != NO_KEY) {
    Serial.println(key);
    switch (key){
      case EQU_CHAR : {
        calculate();
        break;
      }
      case ADD_CHAR : {
        operation_mode = ADD;
        break;
      }
      case SUB_CHAR : {
        operation_mode = SUB;
        break;
      }
      case DIV_CHAR : {
      	operation_mode = DIV;
        break;
      }
      case MUL_CHAR : {
       	operation_mode = MUL;
        break;
      }
      default : {
       	calc_buffer += key;
        break;
      }
    }
    Serial.println(calc_buffer);
  }
  delay(1);
}

void calculate()
{
  int result = 0;
  
  //clean up the buffer
  calc_buffer = "";
}
