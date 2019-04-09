#include <PID_v1.h>


#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);





const int photores = A0;   // LDR input pin
const int pot = A1;    // Potentiometer input pin
const int potP = A3; 
const int potI = A4;
const int potD = A5;
const int led = 9;         // LED output pin
double lightLevel;         // Indirectly store the light level
// Tuning parameters
float Kp = 0;         // Proportional gain
float Ki = 0;             // Integral gain
float Kd = 0;              // Differential gain
// Record the set point as well as the controller input and output
double Setpoint, Input, Output;
// Create a controller that is linked to the specified Input, Ouput and Setpoint
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
const int sampleRate = 1;        // Time interval of the PID control
const long serialPing = 500;     // How often data is recieved from the Arduino
unsigned long now = 0;           // Store the time that has elapsed
unsigned long lastMessage = 0;   // The last time that data was recieved
int SetpointI, InputI, OutputI;


void setup()
 {
 lightLevel = analogRead(photores);                 // Read the set point
 // Arduino has an analogueRead() resolution of 0-1023 and an analogueWrite() resolution of 0-255
 Input = map(lightLevel, 0, 1023, 0, 255);        // Scale the input
 Setpoint = map(analogRead(pot), 0, 1023, 0, 255);     // Scale the set point
Kp = map(analogRead(potP), 0, 1023, 0, 100); 
Ki = map(analogRead(potI), 0, 1023, 0, 100); 
Kd = map(analogRead(potD), 0, 1023, 0, 100); 
 Serial.begin(9600);                                // Initialise serial communications at 9600 bps
 myPID.SetMode(AUTOMATIC);                          // Turn on the PID control
 myPID.SetSampleTime(sampleRate);                   // Assign the sample rate of the control
 Serial.println("Begin");                           // Let the user know that the set up s complete
 lastMessage = millis();   
 myPID.SetTunings(Kp, Ki, Kd);
  lcd.begin(16, 4);
  lcd.setCursor(4, 0);
 lcd.print("PID  HMI");
// Serial data will be recieved relative to this first point
 }
  
void loop()
 {
 Setpoint = map(analogRead(pot), 0, 1023, 0, 255);    // Continue to read and scale the set point
Kp = map(analogRead(potP), 0, 1023, 0, 100);
Ki = map(analogRead(potI), 0, 1023, 0, 100); 
Kd = map(analogRead(potD), 0, 1023, 0, 100); 
 lightLevel = analogRead(photores);                 // Read the light level
 Input = map(lightLevel, 0, 1023, 0, 255);          // Scale the input to the PID
 myPID.Compute();                                   // Calculates the PID output at a specified sample time
 analogWrite(led, Output);                          // Power the LED
 now = millis();                                    // Keep track of the elapsed time
 if(now - lastMessage > serialPing)                 // If enough time has passed send data
 {
 Serial.print("Setpoint = ");
 Serial.print(Setpoint);

  lcd.setCursor(0, 1);
 lcd.print("Set");
 lcd.setCursor(4, 1);
 lcd.print(SetpointI);

lcd.setCursor(9, 1);
 lcd.print("P");
 lcd.setCursor(11, 1);
 lcd.print(Kp);
 
 Serial.print(" Input = ");
 Serial.print(Input);

 lcd.setCursor(0, 2);
 lcd.print("In ");
  lcd.setCursor(4, 2);
 lcd.print(InputI);

 lcd.setCursor(9, 2);
 lcd.print("I");
  lcd.setCursor(11, 2);
 lcd.print(Ki);
 
 Serial.print(" Output = ");
 Serial.print(Output);

 lcd.setCursor(0, 3);
 lcd.print("Out");
  lcd.setCursor(4, 3);
 lcd.print(OutputI);

lcd.setCursor(9, 3);
 lcd.print("D");
  lcd.setCursor(11, 3);
 lcd.print(Kd);
 
 
 Serial.print("\n");
 // The tuning parameters can be retrieved by the Arduino from the serial monitor: 0,0.5,0 set Ki to 0.5.
 // Commas are ignored by the Serial.parseFloat() command
 if (Serial.available() > 0)
 {
 for (int x = 0; x < 4; x++)
 {
 switch(x)
 {
 case 0:
 Kp = Serial.parseFloat();
 break;
 case 1:
 Ki = Serial.parseFloat();
 break;
 case 2:
 Kd = Serial.parseFloat();
 break;
 case 3:
 for (int y = Serial.available(); y == 0; y--)
 {
 Serial.read();
 }
 break;
 }
 }
 Serial.print(" Kp,Ki,Kd = ");  // Display the new parameters
 Serial.print(Kp);
 Serial.print(",");
 Serial.print(Ki);
 Serial.print(",");
 Serial.print(Kd);
 myPID.SetTunings(Kp, Ki, Kd);  // Set the tuning of the PID loop
 }
 lastMessage = now;               // Reference the next serial communication to this point
 }



 SetpointI = Setpoint;
 InputI =  Input ;
 OutputI =  Output ;



 
 }
