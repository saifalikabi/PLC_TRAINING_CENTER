#include <SPI.h>
#include <Ethernet.h>

#include <LiquidCrystal.h>

const int rs = 22, en = 24, d4 = 26, d5 = 28, d6 = 30, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "Mudbus.h"

Mudbus Mb;
//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in Mudbus.h) MB_PORT

void setup()
{
   lcd.begin(16,2);
   
    
  uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06 };
  uint8_t ip[]      = { 192, 168, 1, 8 };
  uint8_t gateway[] = { 192, 168, 1, 1 };
  uint8_t subnet[]  = { 255, 255, 255, 0 };
  Ethernet.begin(mac, ip, gateway, subnet);
  //Avoid pins 4,10,11,12,13 when using ethernet shield

  delay(5000);
  Serial.begin(9600);
  
  pinMode(23, INPUT);
  pinMode(25, INPUT);
  pinMode(27, INPUT);
  pinMode(29, INPUT);
  pinMode(31, INPUT);
  pinMode(33, INPUT);
  
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT) ;
}

void loop()
{
  Mb.Run();
  
  //Analog inputs 0-1023
  Mb.R[0] = analogRead(A8); //pin A0 to Mb.R[0]
  Mb.R[1] = analogRead(A9);
  Mb.R[2] = analogRead(A10);
  
  //Analog outputs 0-255
 // analogWrite(6, Mb.R[6]); //pin ~6 from Mb.R[6]
lcd.setCursor(0, 0);
lcd.print("R6: ");
lcd.setCursor(3, 0);
    lcd.print(Mb.R[6]);


lcd.setCursor(0, 1);
lcd.print("R7: ");
lcd.setCursor(3, 1);
    lcd.print(Mb.R[7]);

lcd.setCursor(8, 0);
lcd.print("R8: ");
lcd.setCursor(11, 0);
    lcd.print(Mb.R[8]);

lcd.setCursor(8, 1);
lcd.print("R9: ");
lcd.setCursor(11, 1);
    lcd.print(Mb.R[9]);





    
  //Digital inputs
  Mb.C[7] = digitalRead(23); //pin 7 to Mb.C[7]
Mb.C[8] = digitalRead(25);
Mb.C[9] = digitalRead(27);
Mb.C[10] = digitalRead(29);
Mb.C[11] = digitalRead(31);
Mb.C[12] = digitalRead(33);

  //Digital outputs
  digitalWrite(35, Mb.C[14]);
  digitalWrite(37, Mb.C[15]);
  digitalWrite(39, Mb.C[16]);
  digitalWrite(41, Mb.C[17]);//pin 8 from Mb.C[8]
}
