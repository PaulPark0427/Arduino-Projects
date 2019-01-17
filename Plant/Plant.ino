int requiredtemp;//Desired temperature value
int requiredmoist;//Desired moisture value
int requiredbright;//Desired Brightness value
int temppin = A0;//Declaring what pin the temperature sensor uses
int moistpin = A1;// Delaring what the moisture sensor is going to use
int brightpin = A2;// Declaring what pin the photoresistor is going to use
int temp;// The variable that will store the current temperature
int moist;//Variable that will store the current moisture 
int bright;// Variable that will store the current brightness 
int Red = 11;// Setting what pin the red led will use
int Green = 9;//Setting what pin the green led will use
int Blue = 10;// Setting what pin the Blue led will use
int Sound = 6;//Setting what pin the tone generator
int buttonpin = 12;// What pin the button is going to send it's information
int button;// The variable that will store the value of the button
int beg = 1;// This variable will be used to determine if the code is restarted or reset. 
void setup() {
  // put your setup code here, to run once:
  pinMode (Red, OUTPUT);// Declaring the pin the red led uses to an output
  pinMode (Green, OUTPUT);//Declaring the pin the Green  led uses to an output
  pinMode (Blue, OUTPUT);// Declaring the pin the blue led uses to an output
  pinMode (Sound, OUTPUT);// Declaring the pin used by the tone generator to an OUTPUT
  pinMode (buttonpin, INPUT);// Decalaring the pin used by the button is an input
  Serial.begin (9600);// Setting up the serial monitor
}
int Perfect() { // Run this code when the Perfect integer is used
  analogWrite (Red, 0);// Turn off the red led fully
  analogWrite (Green, 255);// Turn on the green led fully
  analogWrite (Blue, 0);// Turn on the blue led fully
}

int Dry () {  // Run this code when the Dry integer is used
  tone (Sound, 500);// Create a high pitched tone from the tone generator
  analogWrite (Red, 0);// Turn off the red led
  analogWrite (Green, 0);//Turn off green led
  analogWrite (Blue, 255);//Turn on the Blue led fully
  delay (500);// Pause the program for 1/2 a second
  noTone (Sound);// Turn off the tone generator
  analogWrite (Red, 0);// Turn off the red led
  analogWrite (Green, 255);// Turn on the green led fully
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500);// delay the program for 1/2 a second
}

int Wet () { // Run this code when the Wet integer is used
  tone (Sound, 500);//Create a high piched sound
  analogWrite (Red, 0);//Turn off the red led
  analogWrite (Green, 0);// Turn off the green led
  analogWrite (Blue, 255);// Turn on the blue led fully
  delay (500);// Delay the code by 1/2 a second
  noTone (Sound);// Turn off the tone generator
  analogWrite (Red, 255);// Turn on the red led fully 
  analogWrite (Green, 0 );// Turn off the gren led
  analogWrite (Blue, 0);// Turn off the blue led 
  delay (500);// Delay the program by 1/2 second
}

int Frozen () { // Run this code when the Frozen integer is used
  tone (Sound, 500); // Create a high pitched sound
  analogWrite (Red, 255);// turn on the red led fully 
  analogWrite (Green, 75);// Turn on the green led partially to create the color oranged combined with the red
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500);// Delay the program by 1/2 second 
  noTone (Sound);// Turn off the tone generator
  analogWrite (Red, 0);// Turn off the red led
  analogWrite (Green, 0);// Turn off the green led 
  analogWrite (Blue, 255);// Turn on the blue led fully 
  delay (500);// Delay the code by 1/2 a second
}

int Fire () { //Run this code when the Fire integer is used
  tone (Sound, 500); // Create a high picthed tone
  analogWrite (Red, 255);// turn on the red led fully 
  analogWrite (Green, 75);// Turn on the green led partially to create the color oranged combined with the red
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500);// delay the code by 1/2 a second
  noTone (Sound);// Turn off the tone generator
  analogWrite (Red, 255);// Turn on the red led fully
  analogWrite (Green, 0);// Turn off the green led
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500);// Delay the program by 1/2 a second
}

int Dark () {// Run this code when the Dark integer is used
  tone (Sound, 500);// Create a high pitched tone
  analogWrite (Red, 255);// turn on the red led fully 
  analogWrite (Green, 225);// turn on the green led fully 
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500);// Delay the program by 1/2 a second
  noTone (Sound);// Turn off the tone generator
  analogWrite (Red, 0);// turn off the red led  
  analogWrite (Green, 255);// turn on the Green led fully 
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500);// Delay the code by 1/2 a second
}

int Bright () {// Run this code when the Bright integer is used
  tone (Sound, 500);// create a high pitched tone
  analogWrite (Red, 255);// turn on the red led fully 
  analogWrite (Green, 225);// turn on the Green led fully 
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500);// delay the program by 1/2 a second
  noTone (Sound);// Turn off the tone generator
  analogWrite (Red, 255);// turn on the red led fully 
  analogWrite (Green, 0);// Turn off the gren led
  analogWrite (Blue, 0);// Turn off the blue led
  delay (500); // Delay the program by 1/2 a second
}

void loop() {
  // put your main code here, to run repeatedly:
  moist = analogRead (moistpin);// Storing the current value of the moisture sensor 
  temp = analogRead (temppin);// Storing the current temperature sensor
  bright = analogRead (brightpin);// Storing the current value of the phototresistor
  button = digitalRead (buttonpin); // Storing the current value of the button
  if (beg == 1) { // If the code is at the begining or has been reset then run this code
    requiredmoist = analogRead (moistpin);// Storing the current moisture sensor value to be the required moisture 
    requiredtemp = analogRead (temppin);// Storing the current temperature sensor value to be the required temperature
    requiredbright = analogRead (brightpin);//Storing the current value of the photresistor to be the required brightness
    beg = 0;// Letting the program know that the code is no longer in the begning stage
  }
  if (button == 1) {// If button is pressed
     requiredmoist = analogRead (moistpin);// Storing the current moisture sensor value to be the required moisture 
    requiredtemp = analogRead (temppin);// Storing the current temperature sensor value to be the required temperature
    requiredbright = analogRead (brightpin);//Storing the current value of the photresistor to be the required brightness
  }
  if (moist > requiredmoist + 30) { //If the moisture is too high then run this code
    Wet();// Run the wet integer
  }
  else if (moist < requiredmoist - 30) {// If the moisture is too dry then run this code
    Dry();// Run the dry integer
  }
  else if (temp > requiredtemp + 5) { // If the temperature is too high run this code
    Fire(); // Run the integer Fire 
  }
  else if (temp < requiredtemp - 5) { // If the temperature is too low then run this code
    Frozen();// Run the frozen integer
  }
  else if (bright > requiredbright + 150) {// If the brightness is too high then run this code
    Bright ();// Run the bright integer
  }
  else if (bright < requiredbright - 150) {// If the brightness is too low then run this code
    Dark();// Run the dark integer
  }
  else { // If none of the criteria is met run this code
    Perfect(); // Run the Perfect integer
  }
}
