/*******************************************************************
 *  Problem1:  
 *  Date:  28/01/2020
 *  Author: Ayush Gupta
 *  ESP32 Assignment
 *                                                       
 *                                                                 
 *******************************************************************/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks

#include <LiquidCrystal.h>

LiquidCrystal lcd(22,23,5,18,19,21);


#define EAP_IDENTITY "jtm192673" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "Ayushjtm@2673" //your Eduroam password
const char* ssid = "IITD_WIFI"; // Eduroam SSID
const char* host = "https://proxy62.iitd.ac.in/cgi-bin/proxy.cgi"; //external server domain for HTTP connection after authentification




// Initialize Wifi connection to the router
//char ssid[] = "jalwa";     // your network SSID (name)
//char password[] = ""; // your network key


const String startpoint = "http://api.openweathermap.org/data/2.5/weather?q=";
const String endpoint = ",in&APPID=";
const String key = "fedd58a6465df7f359fd5586d04e343a";
String city = "";
String weather="";
String weathermain= "";
String weatherdescr= "";
String temp="";
String pressure="";
String humidity= "";
String sts= "";
float tempread;
int touch= 0;
int alert =4;

// Initialize Telegram BOT
#define BOTtoken "985796062:AAEZn41z-Mu64a-g6JgnXEkJDCQTUUF16_0"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
WiFiServer server(80);
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

const int ledPin = 2;
int ledStatus = 0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    city = String(city+text);
    HTTPClient http;
 
    http.begin(startpoint + city + endpoint + key); //Specify the URL
    city = "";
    int httpCode = http.GET();  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        int i = payload.indexOf("temp");
        int f= payload.indexOf("feels");
        int wstart= payload.indexOf("weather");
        int wend= payload.indexOf("]");
        int p= payload.indexOf("pressure");
        int h= payload.indexOf("humidity");
        int s= payload.indexOf("sea_level");


        
        temp= payload.substring(i+6, f-2);
        pressure= payload.substring(p+10, h-2);
        humidity= payload.substring(h+10, s);

        
        weather= payload.substring(wstart,wend);
        int wms= weather.indexOf("main");
        int desc= weather.indexOf("description");
        
        Serial.println(wms);
        Serial.println(temp);
        tempread = temp.toFloat();
        tempread = (tempread-273.15);

        
        weathermain= weather.substring(wms,wms+15);
        weatherdescr= weather.substring(desc, desc+25);
        sts = payload.substring(payload.indexOf("description")+13,payload.indexOf("icon")-2);
       Serial.println(sts);
        
        
        
//        bot.sendMessage(chat_id, weatherdescr, "");
        Serial.println(i);
        Serial.println(httpCode);
        Serial.println(payload);
        Serial.println(weather);
        Serial.println(weathermain);
        Serial.println(weatherdescr);
        Serial.println(tempread);
        Serial.println(pressure);
        Serial.println(humidity);
        Serial.println(sts);
        bot.sendMessage(chat_id,"temperature:" +city+ "is" + tempread +"\n");
        bot.sendMessage(chat_id,"pressure :" +city+ "is"+ pressure + "\n");
        bot.sendMessage(chat_id,"humidity:" +city+ "is"+ humidity + "\n");
        bot.sendMessage(chat_id,"status:" +city+ "is"+ sts + "\n");
        lcd.print("temp is:");
        delay(2000);
        lcd.clear();
        lcd.print(tempread);
        delay(1000);
        lcd.clear();
        lcd.print("pressure is:");
        delay(2000);
        lcd.clear();
        lcd.print(pressure);
        delay(1000);
        lcd.clear();
        lcd.print("humidity is:");
        delay(2000);
        lcd.clear();
        lcd.print(humidity);
        delay(1000);
        lcd.clear();
        lcd.print("status:");
        delay(2000);
        lcd.clear();
        lcd.print(sts);
        delay(1000);
        lcd.clear();
        
        
        
        
        
           
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources

//    lcd.begin(16, 2);
//    lcd.clear();
//    lcd.print(city);
//
//  
//
//    
  //  String from_name = bot.messages[i].from_name;
   // if (from_name == "") from_name = "Guest";
//    
//    if (text == "/ledon") {
//      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
//      ledStatus = 1;
//      bot.sendMessage(chat_id, "Led is ON", "");
//    }
//
//    if (text == "/ledoff") {
//      ledStatus = 0;
//      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
//      bot.sendMessage(chat_id, "Led is OFF", "");
//    }
//
//    if (text == "/status") {
//      if(ledStatus){
//        bot.sendMessage(chat_id, "Led is ON", "");
//      } else {
//        bot.sendMessage(chat_id, "Led is OFF", "");
//      }
//    }
//
//    if (text == "/start") {
//      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
//      welcome += "This is Flash Led Bot example.\n\n";
//      welcome += "/ledon : to switch the Led ON\n";
//      welcome += "/ledoff : to switch the Led OFF\n";
//      welcome += "/status : Returns current status of LED\n";
//      bot.sendMessage(chat_id, welcome, "Markdown");
//    }
//
//    if(text == "weatherinfo"){
////     String weathercondition= "weather in formation of,  ".\n";
//      String weathercondition= "";
//      weathercondition += weathermain ;
//      weathercondition += weatherdescr;
//      bot.sendMessage(chat_id,weathercondition , "Markdown");
//       
//      
//    }
//
   //  if (text == city) {
  //    String welcome = "Welcome, " + from_name+weathermain+weatherdescr ;
      //welcome += weathercondition"This is Flash Led Bot example.\n\n";
////      welcome += weathermain;
////      welcome +=  weatherdescr;
//     // welcome += "/status : Returns current status of LED\n";
   //  bot.sendMessage(chat_id, welcome, "Markdown");
     // Serial.println(welcome);
//      
   }
//
//    
//
//
//
     }
//}


void setup() {
  Serial.begin(115200);
  delay(10);



    // go to row 1 column 0, note that this is indexed at 0
    lcd.setCursor(0,1); 
    lcd.print ("Welcome Ayush ");

  
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.disconnect(true); 
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);


  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, LOW); // initialize pin as off



  
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
    
  }
  
  


 
    

 
  delay(3000);
  
}
