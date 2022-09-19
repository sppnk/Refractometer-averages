
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display


bool printButton = 0;
bool resetButton = 0;



String inputMessage = "";
String alcString = "";
float inputAlcVal = 0;
float sumInputAlcVal = 0;
float AvgAlcValue = 0;
int countValues = 1;

void reset() {

  //RESET VALUES
  inputMessage = "";
  alcString = "";
  inputAlcVal = 0;
  sumInputAlcVal = 0;
  AvgAlcValue = 0;
  countValues = 1;

  lcd.clear();
  displayData();
  delay(4000);

}


void printTicket() {

  //print ticket throught serial TX
  Serial.println (" " );
  Serial.println (" " );
  Serial.println (" " );
  Serial.println ("XXXXXXXXX " );
  Serial.write (0x12); // send HEX code to printer to print DATE and TIME
  Serial.print ("Lecturas : " );
  Serial.println (countValues - 1);
  Serial.print ("Valor medio : " );
  Serial.println (AvgAlcValue);
  Serial.println (" " );
  Serial.println (" " );
  Serial.println (" " );
  delay(4000);

}


void displayData() {

  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Refractometro: ");
  lcd.setCursor(15, 0);
  lcd.print(inputAlcVal);

  lcd.setCursor(0, 1);
  lcd.print("Valor medio:        ");
  lcd.setCursor(15, 1);
  lcd.print(AvgAlcValue);


  lcd.setCursor(0, 2);
  lcd.print("                   "); // clean reset - print indications
  lcd.setCursor(15, 1);
  lcd.print(AvgAlcValue);


  lcd.setCursor(0, 3);
  lcd.print("Lecturas:        ");
  lcd.setCursor(10, 3);
  lcd.print(countValues - 1);


}

void setup() {
  // initialize serial:
  Serial.begin(1200);
  Serial.println ("Start...");

  // initialize LCD 12c
  lcd.init();                      // initialize the lcd

  // initialize digital inputs


  pinMode(5, INPUT_PULLUP); //RESET BUTTON
  pinMode(6, INPUT_PULLUP); // PRINT BUTTON

  //DEBUG
  //displayData();

}

void loop() {

  // if there's any serial available, read it: RX PIN

  while (Serial.available() > 0) {

    inputMessage = Serial.readStringUntil (0x04);
    alcString = inputMessage.substring (3, 8); //parse data
    inputAlcVal = alcString.toFloat();
    sumInputAlcVal += inputAlcVal;
    AvgAlcValue = sumInputAlcVal / countValues++;

  }

  displayData();

  // read buttons
  printButton = digitalRead(6);
  resetButton = digitalRead(5);

  if (!resetButton || !printButton) {

    if ( !resetButton) {
      //debug
      lcd.setCursor(0, 2);
      lcd.print("RESET ");
      delay (4000);
      reset();
    }

    if ( !printButton) {
      //debug
      lcd.setCursor(10, 2);
      lcd.print("PRINT");
      
      printTicket();

    }
  }


  ///delay(2000);

}
