#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

//char *name = "Please Calibrate.";  //edit name of shield
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

bool button_13 = 0;
bool button_12 = 0;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410

uint16_t version = MCUFRIEND_KBV_H_;


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

  //debug
  //  tft.drawRect(150, 50, 50, 50, RED);
  //tft.setCursor(20, 230);
  //tft.print(AvgAlcValue);// + " tp.y=" + String(tp.y) + "   ");//debug

}


void printTicket() {

  //print ticket throught serial TX
  Serial.println (" " );
  Serial.println (" " );
  Serial.println (" " );
  Serial.println ("XXXXXXX " );
  Serial.write (0x12); // send HEX code to printer to print DATE and TIME
  Serial.print ("Lecturas : " );
  Serial.println (countValues - 1);
  Serial.print ("Valor medio : " );
  Serial.println (AvgAlcValue);
  Serial.println (" " );
  Serial.println (" " );
  Serial.println (" " );


}

void setup() {
  // initialize serial:
  Serial.begin(1200);
  Serial.println ("Start...");

  // initialize digital inputs (if used tftlcd yuo have available pins 10-13 IF NOT USED SD CARD)

  pinMode(13, INPUT_PULLUP); // PRINT BUTTON
  pinMode(12, INPUT_PULLUP); //RESET BUTTON

  //init TFT
  uint16_t ID = tft.readID(); //
  //  Serial.print(F("tft.readID() finds: ID = 0x"));
  //  Serial.println(ID, HEX);
  tft.begin(ID);



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


  //  DEBUG
  //  Serial.print ("Alcohol : " );
  //  Serial.println (inputAlcVal);
  //
  //
  //  Serial.print ("Media : " );
  //  Serial.println (AvgAlcValue);

  tft.setRotation(0);//LANDSCAPE_REV
  int width = tft.width();
  int height = tft.height();
  tft.fillScreen(BLACK);


  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(20, 20);
  tft.print("Refract: ");
  tft.setCursor(20, 60);
  tft.setTextSize(4);
  tft.print(inputAlcVal);

  tft.setTextSize(3);
  tft.setCursor(20, 150);
  tft.print("Valor medio: ");
  tft.setCursor(20, 190);
  tft.setTextSize(4);
  tft.print(AvgAlcValue);

  tft.setTextSize(2);
  tft.setCursor(20, 95);
  tft.print("lecturas: ");
  tft.print(countValues - 1);

  tft.drawRect(15, 250, 100, 50, YELLOW);// posicion, dimensiones, color
  tft.setCursor(25, 265);
  tft.print("INICIO");
  tft.drawRect(130, 250, 100, 50, YELLOW);
  tft.setCursor(140, 265);
  tft.print("IMPRIME");

  //  tft.setCursor(100, 230);
  //  tft.print(AvgAlcValue);// + " tp.y=" + String(tp.y) + "   ");//debug

  button_13 = digitalRead(13);
  tft.setCursor(150, 230);
  tft.print(button_13);//

  button_12 = digitalRead(12);
  tft.setCursor(100, 230);
  tft.print(button_12);//



  // TOUCH BUTTONS
  tp = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if ((tp.z > MINPRESSURE && tp.z < MAXPRESSURE) || !button_12 || !button_13) {

    if ((tp.x > 230 && tp.x < 490  && tp.y > 240 && tp.y < 350) || !button_12) {

      tft.fillRect(18, 252, 94, 46, GRAY);  // posicion, dimensiones, color;
      reset();
    }

    if ((tp.x > 600 && tp.x < 850  && tp.y > 240 && tp.y < 350) || !button_13) {

      tft.fillRect(132, 252, 94, 46, BLUE);// posicion, dimensiones, color;
      printTicket();


    }
    //    tft.setCursor(20, 230);
    //    tft.print("tp.x=" + String(tp.x) + " tp.y=" + String(tp.y) + "   ");//debug
  }


  delay(2000);

}
