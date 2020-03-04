#include <ESP8266WiFi.h>
#include <time.h>
#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN  //4
#define pin_button D8
int button_state;
Adafruit_SSD1306 display(OLED_RESET);
int i;
const char* ssid = "virus";
const char* password = "XFD1048576";
String daystamp;
String timestamp;
String formattedDate;

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

double alpha=0.75;

int ledPin = 13;

//int timezone =19800; //7 * 3600;
int dst = 0;

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
pinMode(A0,INPUT);
pinMode(pin_button, INPUT);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(115200);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.println("Wifi connecting to ");
  display.println( ssid );

  WiFi.begin(ssid,password);
 
  display.println("\nConnecting");

  display.display();

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      display.print("."); 
      display.display();       
  }

  timeClient.begin();
  timeClient.setTimeOffset(19800);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  
  display.println("Wifi Connected!");
  display.println("Initializing Setup");
 // display.println(WiFi.localIP() );

  display.display();

  //configTime(timezone, dst, "time.nplindia.org","in.pool.ntp.org");
  display.println("\nWaiting for NTP...");

  while(!time(nullptr))
  {
     Serial.print("*");
     delay(1000);
  }
  display.println("Time response....OK"); 
  display.println("watch is ready"); 
  display.display();  
  delay(1000);

  display.clearDisplay();
  display.display();
}

void loop() 
{
  button();
  timer();
  Serial.println(button_state);
  while(button_state==HIGH)
  {
    for(i=0;i<70;i++)
    {
    BPM();
    }
    
   button_state = LOW;
    
    }
}




void BPM()
{
   display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.setCursor(0,10);

  display.print("BPM : ");
  
  display.setTextSize(2);
  display.setCursor(60,10);
  
   static double oldValue = 0;
   int sensor=analogRead(A0);
   double value = (alpha * oldValue) + ((1 - alpha) * sensor);
   int x=value/7;
   if(x>=65 && x<=90)
   {
    Serial.print("BPM: ");
    Serial.println(x);
    display.print(x);
   }
   oldValue=value;
   delay(200);
   display.display();
}


int button()
{
  button_state = digitalRead(pin_button);
 // int flag = button_state;
  delay(100);
  return button_state;
}


void timer()
{
  display.clearDisplay();
  timeClient.update();
  timestamp = timeClient.getFormattedTime();
  Serial.println(timeClient.getFormattedTime());

  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  int splitT=formattedDate.indexOf("T");
  daystamp = formattedDate.substring(0,splitT);
  
  // Clear the buffer.
  display.clearDisplay();
 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);

  display.print(timestamp);
  
  display.setTextSize(1);
  display.setCursor(0,25);
  display.print(daystamp);

  display.display();

  delay(500); // update every 1 sec
}
