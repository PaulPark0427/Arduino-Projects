//Importing all of the external Libraries that will be used
#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonBuffer<4000> jsonBuffer;  //create a buffer large enough for your data

//Connecting to the internet being used
const char* ssid = "BELL390";
const char* password =  "A976734659C6";

ESP32WebServer server(80); //Webserver Object

const int led1 = 2; //Pin for built in LED

//HTML code for the home screen which contains clickable links which lead to functions
const char * homeMSG = "<!DOCTYPE html>"
                       "<html>"
                       "<body>"
                       "<p>Welcome To Paul's API. Use the help link below to learn how to use this API<p>"
                       "<a href = \"/help\" > Help</a>"
                       "<br>"
                       "<a href = \"/LEDControl\" > Control LED </a>"
                       "<br>"
                       "<a href = \"/pinstate\" > Pin State </a>"
                       "<br>"
                       "<a href = \"/otherESP\"> Control Another ESP32's LED </a>"
                       "<br>"
                       "<a href = \"/TriggerAction\"> Create an Event to Send an SMS to Paul's phone </a>"
                       "</body>"
                       "</html>";
// HTML code for the help screen which displays what the API is going to do
const char * HELP = "<!DOCTYPE html>"
                    "<html>"
                    "<body>"
                    "<p>Welcome to the help page. On this API, you can use the available functions to control a certain aspect of the ESP32<p>"
                    "<p>You must click onto the available extensions which will lead to a certain task the ESP32 will accompolish<p>"
                    "<a href = \"/\" > HOME </a>"
                    "</body>"
                    "</html>";
//HTML code which checks the state of a pin that the user inputted
const char * PINSTATE = "<!DOCTYPE html>"
                        "<html>"
                        "<body>"
                        "<p>This page will allow you to check the state of a pin on the ESP32<p>"
                        "<form action=\"/pinStatus\" method= \"post\">"
                        "Please input the desired pin number below <br>"
                        "<input type= \"text\" name=\"StateOfPin\">"
                        "</form>"
                        "<a href = \"/\" > HOME </a>"
                        "</body>"
                        "</html>";
// HTML code to control the built in LED following the commands of the user
const char * LEDCONTROL = "<!DOCTYPE html>"
                          "<html>"
                          "<body>"
                          "<p>This page will allow you to turn on and off or toggle the built in LED in the ESP32<p>"
                          "<form action=\"/LEDController\" method= \"post\">"
                          "Please input the desired state of LED (ON or OFF) <br>"
                          "<input type= \"text\" name=\"StateOfLED\">"
                          "<br>"
                          "</form>"
                          "<a href = \"/\" > HOME </a>"
                          "</body>"
                          "</html>";
// HTML code which opens a page where the user can control the built in LED of other ESPs
const char * LEDOFESP32 = "<!DOCTYPE html>"
                          "<html>"
                          "<body>"
                          "<p>This page will allow you to turn on and off or toggle the built in LED in the ESP32<p>"
                          "<form action=\"/LEDofESP32\" method= \"post\">"
                          "Please input the desired state of LED (ON or OFF) <br>"
                          "<input type= \"text\" name=\"StateOfLED\">"
                          "<br>"
                          "Please input the desired IP address of the ESP32 <br>"
                          "<input type = \"text\" name = \"IPofESP\" >"
                          "<br>"
                          "<a href=\"Submit\"><button>Submit</button></a>"
                          "</form>"
                          "<a href = \"/\" > HOME </a>"
                          "</body>"
                          "</html>";
// HTML code to send a msg to Paul's phone using a trigger and response app
const char * TRIGGERACTION = "<!DOCTYPE html>"
                             "<html>"
                             "<body>"
                             "<p> This page will create an event which will trigger an action. The action is sending a message to Paul's phone<p>"
                             "<form action=\"/SendMessage\" method=\"post\">"
                             "Please type what you would like the message to say"
                             "<input type='text' name='MSG'>"
                             "<br>"
                             "<a href=\"Submit\"><button>Submit</button></a>"
                             "</form>"
                             "<a href = \"/\" > HOME </a>"
                             "</body>"
                             "</html>";
//HTML code used to display what msg was sent
String MessageSend = "<!DOCTYPE html>"
                     "<html>"
                     "<body>"
                     "<a href = \"/TriggerAction\" > Previous </a>"
                     "<br>"
                     "</body>"
                     "</html>";
//The original HTML so the page can be reset and refreshed
String MessageSendOG = "<!DOCTYPE html>"
                       "<html>"
                       "<body>"
                       "<a href = \"/TriggerAction\" > Previous </a>"
                       "<br>"
                       "</body>"
                       "</html>";
//HTML code which displays the command for and the location of a separate ESP
String ExternalLEDMsg = "<!DOCTYPE html>"
                        "<html>"
                        "<body>"
                        "<a href = \"/otherESP\" > Previous </a>"
                        "<br>"
                        "</body>"
                        "</html>";
//HTML code which displays the command for and the location of a separate ESP. Used to reresh the page
String ExternalLEDMsgOG = "<!DOCTYPE html>"
                          "<html>"
                          "<body>"
                          "<a href = \"/otherESP\" > Previous </a>"
                          "<br>"
                          "</body>"
                          "</html>";


void setup() {
  pinMode (led1, OUTPUT);// Setting up pin
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
  server.on("/help", []() {//When the link is set to /help, display the help page
    server.send(200, "text/html", HELP);
  });
  server.on("/pinstate", []() {//Displays the pinstate page
    server.send(200, "text/html", PINSTATE);
  });
  server.on("/LEDControl" , []() {//Displays the local built in LED control page
    server.send (200, "text/html", LEDCONTROL);
  });
  server.on("/otherESP" , []() {// Displays the controlling remote LED page
    server.send (200, "text/html", LEDOFESP32);
  });
  server.on("/TriggerAction" , []() {// Displays the message send page
    server.send (200, "text/html", TRIGGERACTION);
  });
  server.on("/pinStatus", pinStatus);//Runs the pinstatus function which will get the state of a chosen pin
  server.on("/LEDController", LEDCONTROLLER);//Runs the function where the 
  server.on("/LEDofESP32", LEDofSeparateESP);//Runs the function where the LED on a remote ESP is controlled
  server.on("/SendMessage", SendMsg);//Runs the function where a msg is sent to the phone
  server.on("/changeState", RecieveLED);//Recieves the command from a separate ESP
  server.begin();//Starts the server
}

void loop() {
  server.handleClient(); //Handles incoming requests
}
void pinStatus() {//Checks state of pin
  if (server.arg("StateOfPin") == "") {//When there is no pin inputted, tells the user that the pin doesnt exist

    server.send (200, "text/plain", "pin not found");
  }
  else {
    //Displays what state the user chose and what the current state of pin is
    String State = "The state of pin number ";
    State += server.arg("StateOfPin"); //Gets user input
    State += " Is: ";
    int pinvalue = server.arg("StateOfPin").toInt();
    pinMode (pinvalue, INPUT);//Dclares pin as an OUTPUT
    State += digitalRead (pinvalue);//gets state of pin
    server.send (200, "text/plain", State);//Displays message
  }
}
void LEDCONTROLLER() {//Used to control the built in LED of the ESP
  if (server.arg("StateOfLED") == "") {//If no command is inputted, lets the user know to input a command
    server.send(200, "text/plain", "Please type in a valid command for the LED");
  }
  //The state of the LED is controlled by the user
  else if (server.arg("StateOfLED") == "ON") {
    digitalWrite(led1, HIGH);
    server.send (200, "text/plain", "The Built in LED is on");
  }
  else if (server.arg("StateOfLED") == "OFF") {
    digitalWrite (led1, LOW);
    server.send (200, "text/plain", "The built in LED is OFF");
  }
  else if (server.arg("StateOfLED") == "Toggle") {
    int LEDstate = digitalRead (led1);
    if (LEDstate == 1) {
      digitalWrite (led1, LOW);
      server.send (200, "text/plain", "The built in LED was ON now it is OFF");
    }
    else if (LEDstate == 0) {
      digitalWrite (led1, HIGH);
      server.send(200, "text/plain", "The built in LED was OFF now it is ON");
    }
  }
  else {
    server.send (200, "text/plain", "The inputted command is not valid. Please use one of the three avaialble commands");
  }
}
void LEDofSeparateESP() {//Function used to control a remote LED
  String LEDState = server.arg("StateOfLED");
  String Address = server.arg ("IPofESP");
  if (LEDState == "") {//Makes the user input a command
    ExternalLEDMsg += "Please input a valid state of the LED";
  }
  else if (Address == "") {//Makes the user input an IP address
    ExternalLEDMsg += "Please input a valid IP address of the remote ESP";
  }
  else if (WiFi.status() == WL_CONNECTED) {//Sends the command for the LED to the IP address given by the user
    Serial.println ("Working");
    HTTPClient http;
    String fullAddress = "http://" + Address + "/changeState";//The address where the command will go
    Serial.println (fullAddress);
    http.begin(fullAddress);
    http.addHeader("Content-Type", "application/json");//Sets what type of content it is
    int httpCode = http.POST("{\"LED\":\"" + LEDState + "\"}");//Sends the command paired with a key
    http.end();
    //Displays what kind of command was sent and to where
    ExternalLEDMsg += "The Command for the LED: ";
    ExternalLEDMsg += LEDState;
    ExternalLEDMsg += "<br>";
    ExternalLEDMsg += "The Command was sent to an IP address of: ";
    ExternalLEDMsg += Address;
  }
  server.send (200, "text/html", ExternalLEDMsg);//Display message
  ExternalLEDMsg = ExternalLEDMsgOG;
}
void SendMsg() {//Function used to send a message to the phone through a trigger and response app
  if (server.arg("MSG") == "") {//If no message is present, the message tells the user to add a message and gives suggestions
    MessageSend += ("Please Enter a Message");
    MessageSend += ("<br>");
    MessageSend += ("Suggestions are: Hello, Hi Paul, YEEEET, BYE BYE, I'm Watching You");
    server.send (200, "text/html", MessageSend);
  }
  else {//If a msg is inputted, the message is displayed to show the user what message is being sent
    MessageSend += ("The message will say: ");
    MessageSend += (server.arg("MSG"));
    server.send (200, "text/html", MessageSend);
  }
  String msgText = server.arg ("MSG");//gets the user input
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;//Sends the trigger action to a link where an app is made. The app then recieves the action and sends a response which is to sena a message to Paul's Phone
    http.begin ("https://maker.ifttt.com/trigger/send_message/with/key/dj0baT_yj15rvSo0oRwVMl");
    http.addHeader ("Content-Type", "application/json");//Type of content
    int httpCode = http.POST("{\"value1\":\"" + msgText + "\"}");//The message being sent
    server.send (200);//Send the message

    String payload = http.getString();
    Serial.println("Here5");
    Serial.println(payload);
    Serial.println(httpCode);
    http.end();
  }

  MessageSend = MessageSendOG;//Refresh the page
}
void RecieveLED () {//used to recieve command for the LED
  if (server.hasArg("plain") == false) { //Check if body received
    server.send(200, "text/plain", "Body not received"); //if it isn't tell the user
    return;
  }
  String payload = server.arg("plain"); //Storing the body of the POST
  JsonObject& root = jsonBuffer.parseObject(payload); //Parsing the payload
  int LEDstate = digitalRead (led1);//Stores the state of LED
  server.send(200, "text/plain", "Body Recieved");//Shows that body has been recieved
  if (!root.success()) {
    return;
  }
  //Depending on the command, the LED follows it
  String LEDCommand = root["LED"];
  if (LEDCommand == "ON") {
    digitalWrite (led1, HIGH);
  }
  else if (LEDCommand == "OFF") {
    digitalWrite (led1, LOW);
  }
  else if (LEDCommand == "TOGGLE") {
    if (LEDstate == 1) {
      digitalWrite (led1, LOW);
    }
    else if (LEDstate == 0) {
      digitalWrite (led1, HIGH);
    }
  }
  jsonBuffer.clear();//Clears the buffer
}


