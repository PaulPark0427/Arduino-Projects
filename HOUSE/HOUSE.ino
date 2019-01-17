//Importing all of the external Libraries that will be used
#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonBuffer<4000> jsonBuffer;  //create a buffer large enough for your data

//Connecting to the internet being used
const char* ssid = "MYHDSB";
const char* password =  "";

ESP32WebServer server(80); //Webserver Object
//Setting up the variable that stores the pin numbers of the buttons and leds
const int kitchenlights = 25;
const int livingroomlights = 26;
const int normalbutton = 27;
const int deliverybutton = 13;
const int friendsbutton = 5;

//Used to create the home page. Contains the title of the API and clickable links which directs the user to a specific 
//Job the API will accompolish
const char * homeMSG = "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                       "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                       "<h1>Paul's House Control </h1>"
                       "</head>"
                       "<body>"
                       "<br>"
                       "<a href = \"/KitchenLights\"> Kitchen Light Control</a>"
                       "<br>"
                       "<a href = \"/LivingRoomLights\"> Living Room Lights Control</a>"
                       "<br>"
                       "<a href = \"/DoorBell\"> Notify the house owner who's at the door</a>"
                       "<br>"
                       "<a href = \"/GroceryReminders\"> Set Grocery Reminders</a>"
                       "<br>"
                       "<a href = \"/PayBills\"> Bill Notification</a>"
                       "<br>"
                       "</body>"
                       "</html>";

//The code used to display the kitchen LED control page
//Contains titles, instruction and user input box for the desired state of lights
String KITCHEN = "<!DOCTYPE html>"
                 "<html>"
                 "<head>"
                 "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                 "<h1>Controlling the Kitchen Lights </h1>"
                 "</head>"
                 "<body>"
                 "<p> This page will allow you to control the state of the kitchen lights(ON or OFF or Toggle) <p>"
                 "<a href = \"/\"> Go Home</a>"
                 "<form>"
                 "Please enter the desired state of the kitchen lights<br>"
                 "<input type='text' name='KitchenState'>"
                 "<br>"
                 "<input type='submit' value='Submit'>"
                 "<br>"
                 "</form>"
                 "</body>"
                 "</html>";
//The code used to reresh the kitchen light control page
String KITCHENmaster = "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                       "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                       "<h1>Controlling the Kitchen Lights </h1>"
                       "</head>"
                       "<body>"
                       "<p> This page will allow you to control the state of the kitchen lights(ON or OFF or Toggle) <p>"
                       "<a href = \"/\"> Go Home</a>"
                       "<form>"
                       "Please enter the desired state of the kitchen lights<br>"
                       "<input type='text' name='KitchenState'>"
                       "<br>"
                       "<input type='submit' value='Submit'>"
                       "<br>"
                       "</form>"
                       "</body>"
                       "</html>";
//The code used to create the living room light control page
//Contains a title, instructions, and a input box where the user can input the desired state of the light
String LIVINGROOM = "<!DOCTYPE html>"
                    "<html>"
                    "<head>"
                    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                    "<h1>Controlling the Living Room Lights </h1>"
                    "</head>"
                    "<body>"
                    "<p> This page will allow you to control the state of the living room lights(ON or OFF or Toggle) <p>"
                    "<a href = \"/\"> Go Home</a>"
                    "<form>"
                    "Please enter the desired state of the living room lights<br>"
                    "<input type='text' name='LivingRoomState'>"
                    "<br>"
                    "<input type='submit' value='Submit'>"
                    "</form>"
                    "</body>"
                    "</html>";
//The code used to refresh the living room light control page
String LIVINGROOMmaster =  "<!DOCTYPE html>"
                           "<html>"
                           "<head>"
                           "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                           "<h1>Controlling the Living Room Lights </h1>"
                           "</head>"
                           "<body>"
                           "<p> This page will allow you to control the state of the living room lights(ON or OFF or Toggle) <p>"
                           "<a href = \"/\"> Go Home</a>"
                           "<form>"
                           "Please enter the desired state of the living room lights<br>"
                           "<input type='text' name='LivingRoomState'>"
                           "<br>"
                           "<input type='submit' value='Submit'>"
                           "</form>"
                           "</body>"
                           "</html>";

//The section of code is used to display a page where the user sends a message to the house owner on who is at the door and why
//The page contains a title, instructions, and 2 input boxes for the name of the person at the door and their purpose
String DOORBELL = "<!DOCTYPE html>"
                  "<html>"
                  "<head>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<h1>The Door Bell Of Paul's House</h1>"
                  "</head>"
                  "<body>"
                  "<p> This page will notify the house owner when a door bell is rung<p>"
                  "<p> The user can input their purpose for ringing the doorbell using the API <p>"
                  "<a href = \"/\"> Go Home</a>"
                  "<form>"
                  "Who are you? <br>"
                  "<input type='text' name='FrontDoorPerson'>"
                  "<br>"
                  "Why are you ringing the door bell for? <br>"
                  "<input type='text' name='FrontDoorMessage'>"
                  "<br>"
                  "<input type='submit' value='RING'>"
                  "</form>"
                  "</body>"
                  "</html>";
//Code used to refresh the DOORBELL page
String DOORBELLmaster = "<!DOCTYPE html>"
                        "<html>"
                        "<head>"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                        "<h1>The Door Bell Of Paul's House</h1>"
                        "</head>"
                        "<body>"
                        "<p> This page will notify the house owner when a door bell is rung<p>"
                        "<p> The user can input their purpose for ringing the doorbell using the API <p>"
                        "<a href = \"/\"> Go Home</a>"
                        "<form>"
                        "Who are you? <br>"
                        "<input type='text' name='FrontDoorPerson'>"
                        "<br>"
                        "Why are you ringing the door bell for? <br>"
                        "<input type='text' name='FrontDoorMessage'>"
                        "<br>"
                        "<input type='submit' value='RING'>"
                        "</form>"
                        "</body>"
                        "</html>";

//This page is used to create a grocery list onto the house owner's reminder
//The page contains a title, instruction and 2 input boxes for the item needed and the reminder date
String GROCERY = "<!DOCTYPE html>"
                 "<html>"
                 "<head>"
                 "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                 "<h1> Set Reminders For Groceries</h1>"
                 "</head>"
                 "<body>"
                 "<p> This page will allow you to set a reminder to get certains items for your next grocery shopping trip<p>"
                 "<a href = \"/\"> Go Home</a>"
                 "<form>"
                 "Please enter what items needs to be bought at the grocery store<br>"
                 "<input type='text' name = 'GroceryItem'>"
                 "<br>"
                 "Please enter a date for the reminder (dd/mm/yyy)"
                 "<br>"
                 "<input type = 'text' name = 'GroceryDate'>"
                 "<br>"
                 "<input type='submit' value='Submit'>"
                 "</form>"
                 "</body>"
                 "</html>";
//Used to refresh the grocery list creating page
String GROCERYmaster = "<!DOCTYPE html>"
                       "<html>"
                       "<head>"
                       "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                       "<h1> Set Reminders For Groceries</h1>"
                       "</head>"
                       "<body>"
                       "<p> This page will allow you to set a reminder to get certains items for your next grocery shopping trip<p>"
                       "<a href = \"/\"> Go Home</a>"
                       "<form>"
                       "Please enter what items needs to be bought at the grocery store<br>"
                       "<input type='text' name = 'GroceryItem'>"
                       "<br>"
                       "Please enter a date for the reminder (dd/mm/yyy)"
                       "<br>"
                       "<input type = 'text' name = 'GroceryDate'>"
                       "<br>"
                       "<input type='submit' value='Submit'>"
                       "</form>"
                       "</body>"
                       "</html>";

//This code is used to display the billing information page. The page sends a bill to the house owner via email
//The page contains a title, instructions and 2 input boxes for the name of the bill provider and how much it is. 
String BILL = "<!DOCTYPE html>"
               "<html>"
               "<head>"
               "<meta name =\"viewport\" content=\"width=device - width, inital-scale+1\">"
               "<h1> Send a bill reminder to the house owner</h1>"
               "</head>"
               "<body>"
               "<p> This page is used to send a bill notification to the house owner<p>"
               "<a href = \"/\"> Go Home</a>"
               "<form>"
               "Who sent the bill?   "
               "<input type ='text' name = 'BillTitle'>"
               "<br>"
               "How much is the bill?   "
               "<input type='text' name = 'BillPrice'>"
               "<br>"
               "<input type='submit' value='Send'>"
               "</form>"
               "</body>"
               "</html>";
String BILLmaster = "<!DOCTYPE html>"
               "<html>"
               "<head>"
               "<meta name =\"viewport\" content=\"width=device - width, inital-scale+1\">"
               "<h1> Send a bill reminder to the house owner</h1>"
               "</head>"
               "<body>"
               "<p> This page is used to send a bill notification to the house owner<p>"
               "<a href = \"/\"> Go Home</a>"
               "<form>"
               "Who sent the bill?   "
               "<input type ='text' name = 'BillTitle'>"
               "<br>"
               "How much is the bill?   "
               "<input type='text' name = 'BillPrice'>"
               "<br>"
               "<input type='submit' value='Send'>"
               "</form>"
               "</body>"
               "</html>";

void setup() {
  // put your setup code here, to run once:
  // Setting up LED pins
  pinMode (kitchenlights, OUTPUT);
  pinMode (livingroomlights, OUTPUT);
  //Setting up input pins for the buttons
  pinMode (normalbutton, INPUT);
  pinMode (deliverybutton, INPUT);
  pinMode (friendsbutton, INPUT);

  Serial.begin(115200);//Starting the serial monitor where the IP address is displayed

  WiFi.begin(ssid, password);// Connect to WIFI

  while (WiFi.status() != WL_CONNECTED) { //While WIFI is not connected, the serial monitor displays that the ESP is trying to connect
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());//prints the IP address of the local ESP

  server.on("/", []() {//When the link is at its base, display the home page
    server.send(200, "text/html", homeMSG);
  });
  server.on("/KitchenLights", kitchencontrol);//On this link, run the function that controls the kitchen light
  server.on("/LivingRoomLights", livingroomcontrol);// On this link, run the function that controls the living room lights
  server.on("/DoorBell", DoorRing);//This page will use the DoorRing function to deal with events occuring on the page

  server.on("/GroceryReminders", groceryreminders); // This page will use the groceryreminders function to deal with events occuring on the page
  server.on("/PayBills", billreminder);// This page will use the billreminder function to deal with events occuring on the page
  server.begin();//Starts the server
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient(); //Handles incoming requests
  //Serial.println (digitalRead(normalbutton));
  Button();
}

void kitchencontrol () {//The function is used to control the state of the kitchen lights
  String KitState = server.arg("KitchenState");//Getting user input for what state of the lights they desire
  int LightState = digitalRead (kitchenlights);//Checks what state the LED is at the moment
  if (KitState == "") {//If user input is empty, don't do anything

  }
  else if (KitState == "ON") {//If the user input is ON, turn the light on and display on the API that the lights have been turned on
    digitalWrite (kitchenlights, HIGH);
    KITCHEN += "The kitchen lights are on";
    KITCHEN += "<br>";
  }
  else if (KitState == "OFF") {//If the user input is PFF, turn the light off  and display on the API that the lights have been turned off
    digitalWrite (kitchenlights, LOW);
    KITCHEN += "The kitchen lights are off";
    KITCHEN += "<br>";
  }
  else if (KitState == "Toggle") {//When user input is Toggle, check the state of the light and change it to be the opposite of what it currently is
    if (LightState == 1) {//If the lights are already on, turn it off and print to the API that the light is now off
      digitalWrite (kitchenlights, LOW);
      KITCHEN += "The kitchen light used to be on but now it is off";
      KITCHEN += "<br>";
    }
    if (LightState == 0) {//If the lights are already off, turn it on and print to the API that the light is now on
      digitalWrite (kitchenlights, HIGH);
      KITCHEN += "The kitchen light used to be on but now it is on";
      KITCHEN += "<br>";
    }
  }
  else {//if an invalid command is used, print to the API telling the user to enter a correct command
    KITCHEN += "Please enter a valid command that was provided previously on the page";
    KITCHEN += "<br>";
  }
  LightState = digitalRead (kitchenlights);//checks state of lights
  if (LightState == 1) {//If the lights are on, display to the API that the lights are on
    KITCHEN += "State of Kitchen lights: ";
    KITCHEN += "ON";
    KITCHEN += "<br>";
  }
  else if (LightState == 0) {//If the lights are off, display to the API that the lights are off
    KITCHEN += "State of Kitchen lights: ";
    KITCHEN += "OFF";
    KITCHEN += "<br>";
  }
  server.send (200, "text/html", KITCHEN);//Update the page
  KITCHEN = KITCHENmaster;//Refresh the page 
}

void livingroomcontrol() {//The function is used to control the state of the living room lights
  String LivState = server.arg("LivingRoomState");//Sees what the user input is
  int LightState = digitalRead (livingroomlights);//Checks the state of the lights
  if (LivState == "") {//If there are no input, don't do anything

  }
  else if (LivState == "ON") {//If the command is ON, turn the lights on and print to the API showing that the lights are now on
    digitalWrite (livingroomlights, HIGH);
    LIVINGROOM += "The living room lights are on";
    LIVINGROOM += "<br>";
  }
  else if (LivState == "OFF") {//If the command is OFF, turn the lights off and print to the API showing that the lights are now off
    digitalWrite (livingroomlights, LOW);
    LIVINGROOM += "The living room lights are off";
    LIVINGROOM += "<br>";
  }
  else if (LivState == "Toggle") {//If the command was Toggle, check the current state of the lights and change it to be the opposite
    if (LightState == 1) {//If the lights are already on, turn the lights off
      digitalWrite (livingroomlights, LOW);
      LIVINGROOM += "The living room light used to be on but now it is off";
      LIVINGROOM += "<br>";
    }
    if (LightState == 0) {//If the lights are already off, turn the lights on
      digitalWrite (livingroomlights, HIGH);
      LIVINGROOM += "The living room light used to be on but now it is on";
      LIVINGROOM += "<br>";
    }
  }
  else {//If an invalid command is entered, the API will tell the user to input a valid command
    LIVINGROOM += "Please enter a valid command that was provided previously on the page";
    LIVINGROOM += "<br>";
  }
  LightState = digitalRead (livingroomlights);//Checks state of lights
  if (LightState == 1) {//If the lights are on, display on the API that the lights are now on
    LIVINGROOM += "State of lving room lights: ";
    LIVINGROOM += "ON";
    LIVINGROOM += "<br>";
  }
  else if (LightState == 0) {//If the lights are off, display on the API that the lights are off
    LIVINGROOM += "State of living room lights: ";
    LIVINGROOM += "OFF";
    LIVINGROOM += "<br>";
  }
  server.send (200, "text/html", LIVINGROOM);//Updates the page
  LIVINGROOM = LIVINGROOMmaster;//Refreshes the page so it can be used again
}

void groceryreminders() {//This function is used to create a grocery list on a phone
  String Item = server.arg("GroceryItem");//Stores the item the user wants to buy
  String Date = server.arg("GroceryDate");//Stores the date the item needs to be purchased by
  boolean input = false;
  if (Item == "") {//If the item input is empty, don't do anything

  }
  if (Date == "") {//If the date input is empty, don't do anything

  }
  else {//If a command is inputed, display on the API what the item they added was and what date it needs to be purchased by
    GROCERY += "The grocery item set up for a reminder: ";
    GROCERY += Item;
    GROCERY += "<br>";
    GROCERY += "The date for the reminder is: ";
    GROCERY += Date;
    input = true;
  }
  server.send (200, "text/html", GROCERY);//Update the page
  GROCERY = GROCERYmaster;//Refreh the page
  if (WiFi.status() == WL_CONNECTED && input == true) {//If ther is an input and the wifi is connected, send a POST request containing the date and item for the grocery list. 
    //This will reach webhook where the action is matched to a response and the POST request is used for the response action. In this case, create a grocery list
    HTTPClient http;//Sends the trigger action to a link where an app is made. The app then recieves the action and sends a response which is to sena a message to Paul's Phone
    http.begin ("https://maker.ifttt.com/trigger/Grocery_Reminders/with/key/oJ63Ss1ViLUnUGIIzePT69WfNAK1ZnBNqZ9kHV2QuYt");
    http.addHeader ("Content-Type", "application/json");//Type of content
    int httpCode = http.POST("{\"value1\":\"" + Item + "\" , \"value2\":\"" + Date + "\"}");//The message being sent
    server.send (200);//Send the message
    String payload = http.getString();
    Serial.println(payload);
    Serial.println ("{\"value1\":\"" + Item + "\" , \"value2\":\"" + Date + "\"}");
    http.end();
    input = false;
  }
}

void DoorRing () {//The function is used to handle user input for who is at the door
  String DoorMSG = server.arg("FrontDoorMessage");//Stores user input on why someone is at the door
  String DoorPerson = server.arg("FrontDoorPerson");//Stores user input on who is at the door
  boolean input = false;
  if (DoorMSG == "") {//If the user does not input anything don't do anything

  }
  else if (DoorPerson == "") {//If user does not input anything, don't do anything

  }
  else {//If there are inputs, display on the API who is at the door and why
    DOORBELL += "Person Name: ";
    DOORBELL += DoorPerson;
    DOORBELL += "<br>";
    DOORBELL += "Purpose of being here: ";
    DOORBELL += DoorMSG;
    input = true;
  }
  server.send (200, "text/html", DOORBELL);//Update the page
  DOORBELL = DOORBELLmaster;//Refresh the page
  if (WiFi.status() == WL_CONNECTED && input == true) {//If ther is an input and the wifi is connected, send a POST request containing the date and item for the grocery list. 
    //This will reach webhook where the action is matched to a response and the POST request is used for the response action. In this case, send a message to house owner who is at the door and why
    HTTPClient http;//Sends the trigger action to a link where an app is made. The app then recieves the action and sends a response which is to sena a message to Paul's Phone
    http.begin ("https://maker.ifttt.com/trigger/DoorRing/with/key/oJ63Ss1ViLUnUGIIzePT69WfNAK1ZnBNqZ9kHV2QuYt");
    http.addHeader ("Content-Type", "application/json");//Type of content
    int httpCode = http.POST("{\"value1\":\"" + DoorPerson + "\" , \"value2\":\"" + DoorMSG + "\"}");//The message being sent
    server.send (200);//Send the message
    String payload = http.getString();
    Serial.println(payload);
    Serial.println ("{\"value1\":\"" + DoorPerson + "\" , \"value2\":\"" + DoorMSG + "\"}");
    http.end();
    input = false;
  }
}
void Button() {//Function is used to handle what happens when the doorbells are pressed
  int normalbell = digitalRead (normalbutton);//Stores the state of the generic doorbell
  int deliverybell = digitalRead (deliverybutton);//Stores the state of the delivery doorbell
  int friendsbell = digitalRead (friendsbutton);//Stores the state of the friends doorbell
  String BellText = "";//String used to contain message fot the owner of the house
  if (normalbell == 1) {//when the generic doorbell is pressed send a POST request to webhook
    if (WiFi.status() == WL_CONNECTED) {//If ther is an input and the wifi is connected, send a POST request containing the date and item for the grocery list. 
    //This will reach webhook where the action is matched to a response and the POST request is used for the response action. In this case, send a message saying that the doorbell has been pressed
      BellText = "The generic door bell has been rung. The person at the door is unknown";//The message the house owner will recieve
      HTTPClient http;//Sends the trigger action to a link where an app is made. The app then recieves the action and sends a response which is to sena a message to Paul's Phone
      http.begin ("https://maker.ifttt.com/trigger/Door_Bell/with/key/oJ63Ss1ViLUnUGIIzePT69WfNAK1ZnBNqZ9kHV2QuYt");
      http.addHeader ("Content-Type", "application/json");//Type of content
      int httpCode = http.POST("{\"value1\":\"" + BellText + "\"}");//The message being sent
      server.send (200);//Send the message
      String payload = http.getString();
      Serial.println(payload);
      Serial.println ("{\"value1\":\"" + BellText + "\"}");
      http.end();
    }
  }
  else if (deliverybell == 1) {//When the delivery button is pressed send a POST request to Webhook
    if (WiFi.status() == WL_CONNECTED) {//If ther is an input and the wifi is connected, send a POST request containing the date and item for the grocery list. 
    //This will reach webhook where the action is matched to a response and the POST request is used for the response action. In this case, send a message saying that delivery is at the house
      BellText = "The delivery door bell has been rung. A delivery has arrived at the front door";//The txt message the house owner will recieve
      HTTPClient http;//Sends the trigger action to a link where an app is made. The app then recieves the action and sends a response which is to sena a message to Paul's Phone
      http.begin ("https://maker.ifttt.com/trigger/Door_Bell/with/key/oJ63Ss1ViLUnUGIIzePT69WfNAK1ZnBNqZ9kHV2QuYt");
      http.addHeader ("Content-Type", "application/json");//Type of content
      int httpCode = http.POST("{\"value1\":\"" + BellText + "\"}");//The message being sent
      server.send (200);//Send the message
      String payload = http.getString();
      Serial.println(payload);
      Serial.println ("{\"value1\":\"" + BellText + "\"}");
      http.end();
    }
  }
  else if (friendsbell == 1){//When the friends doorbell is pressed send a POST request to Webhook
    if (WiFi.status() == WL_CONNECTED) {//If ther is an input and the wifi is connected, send a POST request containing the date and item for the grocery list. 
    //This will reach webhook where the action is matched to a response and the POST request is used for the response action. In this case, send a message saying that friends are at the house
      BellText = "The friend door bell has been rung. A friend has arrived at the front door";//The message the house owner will recieve
      HTTPClient http;//Sends the trigger action to a link where an app is made. The app then recieves the action and sends a response which is to sena a message to Paul's Phone
      http.begin ("https://maker.ifttt.com/trigger/Door_Bell/with/key/oJ63Ss1ViLUnUGIIzePT69WfNAK1ZnBNqZ9kHV2QuYt");
      http.addHeader ("Content-Type", "application/json");//Type of content
      int httpCode = http.POST("{\"value1\":\"" + BellText + "\"}");//The message being sent
      server.send (200);//Send the message
      String payload = http.getString();
      Serial.println(payload);
      Serial.println ("{\"value1\":\"" + BellText + "\"}");
      http.end();
    }
  }
}

void billreminder(){//This fucntion is used to send billing information to the house owner
  String reciever = server.arg("BillTitle");//Stores user input on who send the bill
  String money = server.arg ("BillPrice");//Stores user input on how much the bill is
  boolean input = false; 
  if (reciever == "" or money == ""){//If no input is there, don't do anything
    
  }
  else {//If there is an input, display on the API who send the bill and how much it is
    BILL += reciever; 
    BILL += " sent a bill for the house owner";
    BILL += "<br>";
    BILL += "The amount he needs to pay is ";
    BILL += money; 
    BILL += " dollars";  
    input = true; 
  }
  server.send (200, "text/html", BILL);//Updates the page
  BILL = BILLmaster; 
   if (WiFi.status() == WL_CONNECTED && input == true) {
    //If ther is an input and the wifi is connected, send a POST request containing the date and item for the grocery list. 
    //This will reach webhook where the action is matched to a response and the POST request is used for the response action. In this case, sends an email containing billing information
      HTTPClient http;//Sends the trigger action to a link where an app is made. The app then recieves the action and sends a response which is to sena a message to Paul's Phone
      http.begin ("https://maker.ifttt.com/trigger/Bill_Send/with/key/oJ63Ss1ViLUnUGIIzePT69WfNAK1ZnBNqZ9kHV2QuYt");
      http.addHeader ("Content-Type", "application/json");//Type of content
      int httpCode = http.POST("{\"value1\":\"" + reciever + "\" , \"value2\":\"" + money + "\"}");//The message being sent
      server.send (200);//Send the message
      String payload = http.getString();
      Serial.println(payload);
      Serial.println ("{\"value1\":\"" + reciever + "\", \"value2\":\"" + money + "\"}");
      http.end();
    }
  }

