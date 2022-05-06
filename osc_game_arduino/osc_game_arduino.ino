#include "Keyboard.h"
#include <ArduinoOSCWiFi.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

// WiFi stuff
const char* ssid = SECRET_SSID; //YOUR WifiSSD name
const char* pwd = SECRET_PASS; //YOUR PASSWORD WATCH OUT
const IPAddress ip(192, 168, 1, 201); //The local ip of the Arduino is whatever you say it is
const IPAddress gateway(10, 23, 8, 1);
const IPAddress subnet(255, 255, 252, 0);


const char* host = "192.168.1.2";
/*
 * 
 */

// set pin numbers for the five buttons:
const int upButton = 2  ;
const int downButton = 3;
const int leftButton = 4;
const int rightButton = 5;
const int startButton = 6;
const int jumpButton = 7;
bool isStart = false;
int buttonState = false;
int lastButtonState = false;
const int publish_port = 8000;


//The variables we will send
int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;
//button2State, button3State, button4State, button5State, button6State, button7State;



void setup() { // initialize the buttons' inputs:
  
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLDOWN);
  pinMode(jumpButton, INPUT_PULLDOWN);

  Serial.begin(9600);
  // initialize mouse control
  Keyboard.begin();
   connectToNetwork();

    OscWiFi.publish(host, publish_port, "/up", button1State)
        ->setFrameRate(60.f);

        OscWiFi.publish(host, publish_port, "/right", button4State)
        ->setFrameRate(60.f);

        OscWiFi.publish(host, publish_port, "/left", button3State)
        ->setFrameRate(60.f);

         OscWiFi.publish(host, publish_port, "/jump", button2State)
        ->setFrameRate(60.f);
}

void loop() {
  
  // use serial input to control the mouse:
  buttonState = digitalRead(startButton);
  if (buttonState == LOW && lastButtonState == HIGH) {
    isStart = !isStart;
  }
  lastButtonState = buttonState;
  //Serial.print("buttonstate ");
  //Serial.println(buttonState);
  //Serial.print("last ");
  //Serial.println(lastButtonState);
  //delay(100);
  
  if (isStart) {
    Serial.println("Started!");
    // use the pushbuttons to control the keyboard:
    if (digitalRead(upButton) == LOW) {     
      button1State = 1;
    }
    else if (digitalRead(downButton) == LOW) {
      Keyboard.press('s');
    }
    else if (digitalRead(leftButton) == LOW) {
      button3State = 1;
      Keyboard.press('a');
    }
    else if (digitalRead(rightButton) == LOW) {
      button4State = 1;
      Keyboard.press('d');
    }
    else if (digitalRead(jumpButton) == HIGH){
      button2State = 1;
      Keyboard.press(' ');
    }
    else {
      button1State = 0;
      button3State = 0;
      button2State = 0;
      button4State = 0;
      Keyboard.releaseAll();
    }
  }

   
OscWiFi.update();
}

void connectToNetwork() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to: " + String(SECRET_SSID));
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    WiFi.config(ip, gateway, subnet);
    delay(2000);
    
  }
  Serial.println("Connected to: " + String(SECRET_SSID));
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}
