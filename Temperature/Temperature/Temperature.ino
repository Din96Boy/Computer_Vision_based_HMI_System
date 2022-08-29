/*
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//----------------------------for display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);


float a=0;
int counter_detect = 0;
float Tempc=0;

//--------------- Create a WiFiClientSecure object.
WiFiClientSecure client; 
//https://script.google.com/macros/s/AKfycbxP9BYN557wjLdFfmpThf0hAPTUekgSlmdrjMbKN3XFpZaoB1fBcqZeWkjcw5qr_ARy/exec
//--------------- spreadsheet script ID
String GAS_ID = "AKfycbxP9BYN557wjLdFfmpThf0hAPTUekgSlmdrjMbKN3XFpZaoB1fBcqZeWkjcw5qr_ARy";                      


 //--------------ssid and password

//const char *ssid     = "Redmi Note 10 Pro"; 
//const char *password = "Dinoj@960512";

const char *ssid     = "Redmi 10"; 
const char *password = "21356378";


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


void sendData(float tem) {
//Serial.println("====================================================");
Serial.print("connecting to ");
Serial.println(host);


//----------------------------------------Connect to Google host
if (!client.connect(host, httpsPort)) {
  Serial.println("connection failed");
  return;
}

   //----------------------------------------Processing data and sending data
String string_temperature =  String(tem);

String url = "/macros/s/" + GAS_ID + "/exec?Tem=" + string_temperature;
Serial.print("requesting URL: ");
Serial.println(url); 

 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
   "Host: " + host + "\r\n" +
   "User-Agent: BuildFailureDetectorESP8266\r\n" +
   "Connection: close\r\n\r\n");

Serial.println("request sent");

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

//----------------------------------------Checking whether the data was sent successfully or not
while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
    Serial.println("headers received");
    break;
  }
}
String line = client.readStringUntil('\n');
if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
} else {
  Serial.println("esp8266/Arduino CI has failed");
}
Serial.print("reply was : ");
Serial.println(line);
Serial.println("closing connection");
Serial.println("=========================================================");
Serial.println();
//----------------------------------------
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Thank You!");
  display.setTextSize(1);
  display.setCursor(0, 27);
  display.print("**************************");
  display.display();
delay(5000);
exit(0);
 
}



  
void setup() {

  Serial.begin(115200);
if(!display.begin(SSD1306_SWITCHCAPVCC))
{
Serial.println(F("SSD1306 allocation failed"));
for(;;);
}

display.clearDisplay();
display.display();
delay(100);

display.clearDisplay();
display.setTextSize(2);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,10);
display.println("WELL COME");
 display.setTextSize(1);
display.setCursor(0,27);
display.print("***************************");
display.display();
display.display();
delay(2000); 

    Serial.begin(115200);
  mlx.begin(); 
 
  WiFi.begin(ssid,password);
  Serial.println();
  Serial.println("*******************************");
  
  
  
  while(WiFi.status()!=WL_CONNECTED)
  {
    
    delay (2000);
    Serial.print ("*");
display.clearDisplay();
display.setTextSize(1.3);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,15);
display.println("WIFI Conecting...");
display.setTextSize(1);
display.setCursor(0,27);
display.print("**************************");
display.display();
delay (500);

}

Serial.print("WIFI Connected");
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,10);
display.println("WIFI CONNECTED");

display.setTextSize(1);
display.setCursor(0,27);
display.print("**************************");
display.display();
delay (3000);
                                                           

 
  while (!Serial);

  Serial.println("Adafruit MLX90614 test");

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  Serial.print("Emissivity = "); 
  Serial.println(mlx.readEmissivity());
  Serial.println("================================================");

  display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(10,5);
display.println("Sensor redy for");
display.setTextSize(1);
display.setCursor(20,15);
display.print("Detecting.....");
display.setTextSize(1);
display.setCursor(0,27);
display.print("**************************");
display.display();
delay (3000);



client.setInsecure();

}

void loop() {

   
 if (mlx.readObjectTempC()< 30 && a == 0) {
  Serial.println("Put your hand");
  delay(1000);
  }
  



else if (mlx.readObjectTempC() > 30 && mlx.readObjectTempC()<37) {

  
  Serial.print(mlx.readObjectTempC());

  
   for (int i = 1; i <= 10; i++) {
    a = a + mlx.readObjectTempC();
 }

 
  


 Serial.println( a);

  
Serial.print("Your Body Temperature= ");
Serial.print( a/10);
Serial.println("*C");

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,0);
display.println("Body Temperature(C):");
display.setTextSize(2);
display.setCursor(30,10);
display.print(a/10);
display.setTextSize(1);
display.setCursor(0,27);
display.print("**************************");
display.display();
delay (2000);
sendData(a/10);


display.clearDisplay();
display.setTextSize(2);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,10);
display.print("Wating.......");
display.setTextSize(1);
display.setCursor(0,27);
display.print("**************************");
display.display();
delay(1000);
  }





 else if(mlx.readObjectTempC()> 38){
  Serial.print("Plese ry Again!");

  display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,0);
display.println("Please try Again!");
display.setTextSize(1);
display.setCursor(0,27);
display.print("**************************");
display.display();
delay (2000);
  }

}
  

*/



  
  
   
  
