// Library functions added- Wire.h is used for connection to the LCD. LiquidCrystal- Driver for the LCD screen. Servo H, controls the hobby motor by PWM. 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Creates a working display with 16 x 2 instances. 
LiquidCrystal_I2C lcd(0x27,16,2);

// Define the pins for the hardware inclduing the LED's, ultrasonic sensor, Potentiometer pins, then the servo. 
const int buttonPin = 2;
const int trigPin = 7;
const int echoPin = 6;
const int potPin = A0;
Servo gateServo;
const int servoPin = 9;

// Sets the servo motor degree of rotation- Gate closed/ gate opened
int gateClosed = 0; 
int gateOpen = 90;  


long getDistance()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
// Sets pulses for the trigger to get an exact measurement. 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); 
  long distance = duration * 0.034 / 2; 

  return distance;

  // Converts time - distnace 
}

void lcdMessage(String line1, String line2)
{
  lcd.clear(); // Clear display 
  lcd.setCursor(0,0); // Set Cursor 
  lcd.print(line1); // use string 'Line 1' 
  lcd.setCursor(0,1); // Drop down a line' Print string 2'
  lcd.print(line2);
}

void setup()
{
  Serial.begin(9600); // Number bits per second. 

  pinMode(buttonPin, INPUT_PULLUP);// When push to make swithc is pressed =LOW then released = HIGH.

// Inputs + Outputs defined for logix 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  gateServo.attach(servoPin); // Servo attached. 
  gateServo.write(gateClosed); // Automatically closed to start loop. 

  lcd.init();
  lcd.backlight(); // LCD backlight turned on and initialised .

  lcdMessage("Barrier Ready", "Waiting Car"); // Initial LCD print. 
}

// LOOP
void loop()
{
  long distance = getDistance(); // Code for measuirng in CM 

  int potValue = analogRead(potPin); // Pot read 
  int threshold = map(potValue, 0, 1023, 5, 30); // Potentiometer between 0-1023- Read 5-30cm on the ultrasonic sensor. 


  Serial.print("Distance: "); // Print Values to the LCD
  Serial.print(distance);
  Serial.print(" cm | Threshold: "); // Print the threshold to LCD 
  Serial.print(threshold);
  Serial.println(" cm");


  
  if(distance < threshold)
  {
    lcdMessage("Car Detected","Press Button"); // LCD message 

    while(digitalRead(buttonPin) == LOW) // Opens gate as HIGH is close 
    {
      delay(50); 
    }
    lcdMessage("Opening Gate",""); // Display on screen 

   
    gateServo.write(gateOpen); // Gate open 
    delay(1500);

    lcdMessage("Car Passing","");

    while(getDistance() < threshold) // Car is close then the threshold set by the potentiometer, wait for the car to go. 
    {
      delay(100);
    }
    
    delay(3000);

    lcdMessage("Closing Gate",""); // Display on LCD

    
    gateServo.write(gateClosed); // Gate clossed 
    delay(2000);

    lcdMessage("Barrier Ready","Waiting Car"); // Resets Message 
  }

  delay(100);
}
 