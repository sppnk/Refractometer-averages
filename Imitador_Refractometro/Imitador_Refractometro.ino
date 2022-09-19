
float intByte = 10.2;
String message = "";
String alcoholvalue = "";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(1200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // prints title with ending line break
  Serial.println("Start...");
}


void loop() {
  message = "Al+";
  alcoholvalue = String (intByte, 1);
    message += alcoholvalue;
    Serial.print (message);
    Serial.write (0x04); //EOT
  //Serial.print(" otro OK ");
  delay(3000);
  intByte += 0.2;
  

}
