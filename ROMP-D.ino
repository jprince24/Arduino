#include <LiquidCrystal.h>

int state;

#define INIT 1
#define MAIN_MENU 2
#define STATE_ONE 3
#define STATE_TWO 4
#define STATE_THREE 5
#define STATE_FOUR 6
#define LOWEST_LIMIT 0 //for joystick (between 0 && 20 == joystick down)
#define LOWER_LIMIT 20 //for joystick
#define UPPER_LIMIT 1002 //for joystick (between 1002 && 1024 == joystick up) 
#define HIGHEST_LIMIT 1024 //for joystick 
#define SCROLL_DOWN Y >= 0 && Y <= 20
#define SCROLL_UP Y >= 1002 && Y <=1024

String message = "Hello";
int degrees, sensor;
int X, Y, select;
int button_press = HIGH;
int prev_button_press = HIGH;
int joystick_state = 0;
String selStatus;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int prev_state;

// setup
void setup() 
{
  state = INIT;  
//setup pins and serial monitor 
  Serial.begin(9600); //communication with serial monitor 
  pinMode (7,INPUT); //pin7 = input
  pinMode (11,OUTPUT); //pin 11 = output
  digitalWrite (7,HIGH); //SEL pin initialized to HIGH
  String selStatus = "ON"; //default off pushbutton
  lcd.begin (16,2); //number of rows and columns in LCD 
  lcd.clear();
}

// loop
void loop()
{
  lcd.clear();
  Serial.println(selStatus);
  delay(100);
  degrees = map(sensor, 676, 814, 0, 90); //flex sensor values 
  sensor = analogRead(5);
  Serial.print("analog input: "); 
  Serial.print(sensor,DEC); 
  Serial.print("degrees: "); 
  Serial.println(degrees,DEC);
  delay (1000); //delay before next reading
  X = analogRead(A0); 
  Y = analogRead (A1); 
  Serial.print("X: ");
  Serial.print(X); //print out x-axis value from joystick 
  Serial.print(", Y: ");
  Serial.print(Y); //print out y-axis value from joystick
  Serial.print(", SEL: ");
  select = digitalRead(13);  //read in digital value from pin 13 - HIGH if not pressed, LOW if pressed
  
  
  switch(state) 
  {
         case INIT: //initializing 
            Serial.println("I'm in state INIT... main menu");
            degrees = 0;
            lcd.setCursor(0,0);
            lcd.print("Initializing...");
            delay(200);
            state = MAIN_MENU;
            break;
            
         case MAIN_MENU://main menu, just displays options
            Serial.println("I'm in MAIN_MENU");
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("New Baseline+"); //currently highlighted option
            lcd.setCursor(0,1);
            lcd.print("New Reading");
            if (select == LOW && prev_button_press == HIGH) //highlighted option selected
            {
              Serial.println("Button was pushed");
              state = STATE_ONE;
            } else if (SCROLL_DOWN) { //scroll down to highlight new option 
              state = STATE_THREE;
            } else if (SCROLL_UP) {         // state = MAIN_MENU;
                if (prev_state == MAIN_MENU) {
                  state = STATE_FOUR;
                } else {
                  state = MAIN_MENU;
                }
            } else {
              state = MAIN_MENU;
              prev_state = MAIN_MENU; 
            }
            
            if (select == HIGH) {
             prev_button_press = HIGH;
             Serial.println("Button press high");
            }
            
            delay(200);
            break;
         
         case STATE_ONE:
            if (select == HIGH) //highlighted option selected
            {
              Serial.println("I'm in STATE_ONE");
              degrees = analogRead(5); 
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("New Baseline+");
              lcd.setCursor(0,1);
              lcd.print(degrees, DEC);
              state = STATE_ONE;
              prev_button_press = HIGH;
            } else if (select == LOW && prev_button_press == HIGH) {
              state = STATE_TWO;
              prev_button_press == LOW;
            }           
       
            delay(200);
            break;

         case STATE_TWO:
           if (select == HIGH) {
             Serial.println("I'm in STATE_TWO");    
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("Reading Stored");
             state = STATE_TWO;
             prev_button_press = HIGH;
           } else if (select == LOW && prev_button_press == HIGH) {
             state = MAIN_MENU;
             prev_button_press = LOW;
           }
           delay(200);
           break;
   
     case STATE_THREE:           
            
            Serial.println("I'm in STATE_THREE");
            degrees = 0;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("New Baseline");
            lcd.setCursor(0,1);
            lcd.print("New Reading+");
            delay(200);
            if (select == LOW) {
             state = STATE_FOUR; 
             prev_button_press = LOW;
            }
            break;

   case STATE_FOUR:           
            Serial.println("I'm in STATE_FOUR");
            degrees = analogRead(5); 
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("New Reading+");
            lcd.setCursor(0,1);
            lcd.print(degrees, DEC);
            delay(200);
            if (select == LOW) {
               state = MAIN_MENU;
               prev_button_press = LOW;
            } else { 
               state = STATE_FOUR;
            }
            break;

         default:
            break;
  }
}
