





































































#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//----------------------------for display
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

int buzzer=D7;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

//--------------- Create a WiFiClientSecure object.
WiFiClientSecure client;

//--------------- spreadsheet script ID

//https://script.google.com/macros/s/AKfycbxM-fXL9uR2BAaz-peXnBI5_1D1I5SvqiNIx1on5k4GA3DHaCpwnP7fuO2veDCSrCot/exec

String GAS_ID = "AKfycbxM-fXL9uR2BAaz-peXnBI5_1D1I5SvqiNIx1on5k4GA3DHaCpwnP7fuO2veDCSrCot";
                     


//--------------ssid and password

const char *ssid     = "Redmi Note 10 Pro";
const char *password = "Dinoj@960512";

//const char *ssid     = "Redmi 10";
//const char *password = "21356378";


//const char *ssid     = "AndroidAP";
//const char *password = "21356378";

//const char *ssid     = "Keshan-Wi-Fi";
//const char *password = "dissanayaka6767";





//Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;


// Pins on wemos D1 mini
#define APDS9960_SDA    4  //GPIO4 (D2)
#define APDS9960_SCL    5  //GPIO5 (D1)
// Constants
const byte APDS9960_INT  = 12; //GPIO12 (D6)


int a = 0;
int counter_detect = 0;
float Heart = 0;
int x = 0;
int Detected = 0;
float SPO2 = 0;

#define REPORTING_PERIOD_MS     1000

//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
  //Serial.println("Beat!");
  Detected = 1;
}

String LED1_State ="off";
String LED2_State ="off";
String LED3_State ="off";

#define LED_1    16  //GPI16 (D0)(WiFi Connect)
#define LED_2    00  //GPI00 (D3)(Detect)
#define LED_3    02  //GPI00 (D3)(Data Sent)
                                                        

void setup()
{

  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
  delay(100);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("WELL COME");
  display.setTextSize(1);
  display.setCursor(0, 27);
  display.print("***************************");
  display.display();
  delay(2000);


  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println("*******************************");



  while (WiFi.status() != WL_CONNECTED)
  {

    
    Serial.print ("*");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 15);
    display.println("WIFI Conecting...");
    display.setTextSize(1);
    display.setCursor(0, 27);
    display.print("**************************");
    display.display();
    delay (500);

  }

  Serial.print("WIFI Connected");
                              pinMode(LED_1,OUTPUT);//Confirm WiFi connected pin
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("WIFI CONNECTED");

  display.setTextSize(1);
  display.setCursor(0, 27);
  display.print("**************************");
  display.display();
 
   
  
  

  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance

  if (!pox.begin()) {
    Serial.println("FAILED");

    for (;;);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 10);
    display.println("FAILED");
    display.setTextSize(1);
    display.setCursor(0, 27);
    display.print("***************************");
    display.display();
    delay(6000);

  }
  else {
    Serial.println("SUCCESS");
    

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 10);
    display.println("SUCCESS");

    display.setTextSize(1);
    display.setCursor(0, 27);
    display.print("**************************");
    display.display();
    //delay(2000);
  }


  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);



  client.setInsecure();
  pinMode(LED_2,OUTPUT);//Data detected pin
   pinMode(LED_3,OUTPUT);//Data sent pin
  
 

}


void loop()
{
  digitalWrite(LED_2,LOW); 
  digitalWrite(LED_3,LOW); //Google sheet update
  digitalWrite(LED_1,HIGH); //WiFi connect
 
                                            
                                            
  

  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"


  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    a == 0;

    if (pox.getHeartRate() == 0 && a == 0 ) {

      Serial.println("Put your hand please");
      //delay(100);


      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 10);
      display.println("Put Your Hand");
      display.setTextSize(1);
      display.setCursor(0, 27);
      display.print("**************************");
      display.display();
      a == 0;
     

    }

    else if (pox.getHeartRate() > 0 && pox.getHeartRate() < 60 && a == 0) {

      Serial.println("Wait");
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 10);
      display.println("Wait.....");
      display.setTextSize(1);
      display.setCursor(0, 27);
      display.print("**************************");
      display.display();
      a == 0;
      

    }

    else if (pox.getHeartRate() >= 60 && pox.getHeartRate() <= 100 && a == 0 && pox.getSpO2()>=92 ) {

      for (int i = 0; i <= 10; i++) {

                                                                digitalWrite(LED_2,HIGH);
                                                              
                                                               
        a = a + pox.getHeartRate();
      }
tone(buzzer,6000,2000);


      if ((counter_detect == 0)) {
                                                 

        Serial.print("Heart rate:");
        Serial.print(a / 10);
        Heart = a / 10;
       
        
        Serial.print("SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        SPO2=pox.getSpO2();

        counter_detect = 5;

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println("Heart rate:");
        display.setTextSize(1);
        display.setCursor(70, 0);
        display.print(Heart);

        display.setTextSize(1);
        display.setCursor(90, 0);
        display.print("bpm");

        display.setTextSize(1);
        display.setCursor(0, 15);
        display.print("SpO2:");
        display.setTextSize(1);
        display.setCursor(70, 15);
        display.print(pox.getSpO2());

        display.setTextSize(1);
        display.setCursor(90, 15);
        display.print("%");

        display.setTextSize(1);
        display.setCursor(0, 27);
        display.print("**************************");
        display.display();

        delay(2000);

         sendData(Heart,SPO2);

       
         



      }


      counter_detect = counter_detect - 1;
      a = 0;



    }
    tsLastReport = millis();


  }


}

// -------------------------Subroutine for sending data to Google Sheets
void sendData(float heart,float spo2) {

  //Serial.println("====================================================");
  Serial.print("connecting to ");
  Serial.println(host);


  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  //----------------------------------------Processing data and sending data

 float string_heart =  heart;
 float string_spo2 =  spo2;
 
  String url = "/macros/s/" + GAS_ID + "/exec?SPO2=" + string_spo2 + "&Pulse_Rate=" + string_heart;


  //String url = "/macros/s/" + GAS_ID + "/exec?Pulse Rate=" + string_heart;

  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
   
  
  digitalWrite(LED_3,HIGH); //Google sheet update
 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Sent to Google Sheett");
  display.setTextSize(1);
  display.setCursor(0, 27);
  display.print("**************************");
  display.display();
  delay(2000);
                                                                     
  Serial.println("closing connection");
  Serial.println("=========================================================");
  Serial.println();

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Thank You!");
  display.setTextSize(1);
  display.setCursor(0, 27);
  display.print("**************************");
  display.display();



  //----------------------------------------


  delay(5000);
  exit (0);
}
