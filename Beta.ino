
    
    #include "DHT.h"
    #include <LiquidCrystal.h>
    #include <SoftwareSerial.h>

     
    #define DHTPIN 7     // what pin we're connected to
    #define DHTTYPE DHT22   // DHT 22  (AM2302)
    #define fan 8
    #define heat 6
    #define pumpe A2
    #define feucht A3 //10 pinmode vorher (digital)

    
    LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
     
    int maxHum = 40;
    int maxTemp = 28;
    int minHum = 35;
    int minTemp = 25;
    byte Celsius[8] = {B00100,B01010,B00100,B0000,B00000,B00000,B00000,B00000}; //° Pixel festgelegt!!
    long laenge;
    long cm;
    int maxfeuchtigkeitslvl = 400;
    int minfeuchtigkeitslvl = 250;
    float sensorPin = A0;
    int a = 0;
    DHT dht(DHTPIN, DHTTYPE);
     
    void setup() {
      
      lcd.createChar(0, Celsius); 
      lcd.begin(16, 2); //2 Zeilen-Display
      lcd.setCursor(0,0);
      lcd.write("Temp:"); // String mit Temperatur+ 9 Leerzeilen und Grad
      lcd.setCursor(14,0);
      lcd.write((uint8_t)0);// ° zeichen eingefügt
      lcd.setCursor(15,0);
      lcd.write("C");
      
      //lcd.begin(16, 2);
      lcd.setCursor(0,1);
      lcd.write("Humidity:      %"); // String mit Luftfeuchtigkeit + 9 Leerzeilen und %

      pinMode(13,OUTPUT);
      
      pinMode(fan, OUTPUT);
      pinMode(heat, OUTPUT);
      pinMode(feucht,OUTPUT);
      pinMode(pumpe, OUTPUT);
      Serial.begin(9600);
      
      delay(500); 
      dht.begin();
     
    }
     
    void loop() {

     String DisplayWords;
     int sensorValue;
     sensorValue = analogRead(sensorPin);
     
     Serial.print("Water Level: ");
     Serial.print(String(sensorValue));


     if (sensorValue <= minfeuchtigkeitslvl){


        DisplayWords = ("\n Dry Water it");
        Serial.print(DisplayWords);
        Serial.print("\n");
     }else if (sensorValue >= maxfeuchtigkeitslvl){

      DisplayWords = ("\n Wet, Leave it!");
      Serial.print(DisplayWords);
      
     }else {

        Serial.print(DisplayWords);
        
     }

     pinMode(9,OUTPUT);
     pinMode(A1,INPUT); //10 pinmode vorher

     digitalWrite(9,LOW);
     delayMicroseconds(2);
     digitalWrite(9,HIGH);
     delayMicroseconds(5);
     digitalWrite(9,LOW);

     laenge = pulseIn(A1,HIGH); // 10 pinmode vorher

     cm = (laenge*34)/2000;
     
     
    if (cm <=45) 
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(50);

  
      // Aufwärmphase für Temperatursensor (2000 millisekunden)
     delay(2000);
     
      float h = dht.readHumidity();
      // Liest die Temperatur in Celsius aus
      float t = dht.readTemperature();
      
      // Prüfen auf Fehler
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
      
      if(h > maxHum || t > maxTemp) {
          digitalWrite(fan, HIGH);
      } else {
         digitalWrite(fan, LOW); 
      }

      if(t > minTemp) {
          digitalWrite(heat, LOW);
      } else {
         digitalWrite(heat, HIGH); 
      }

      if(h < minHum) {
          digitalWrite(feucht, HIGH);
      } else {
         digitalWrite(feucht, LOW); 
      }



      if (sensorValue < minfeuchtigkeitslvl) {
        digitalWrite(pumpe, HIGH);
        delay (120000); 
      } else {
        digitalWrite(pumpe, LOW);
      }

      
        
      
      /*Serial.print("Humidity: "); 
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.println(" *C ");*/

      //Serial.print(cm);
      //Serial.print("cm");

      
      lcd.setCursor(9,0); // 2 Zeilen-Display
      lcd.print(t); // gibt die Temperatur aus
      delay(100);

      
      lcd.setCursor(9,1); // 2 Zeilen-Display
      lcd.print(h); // gibt die Luftfeuchtigkeit aus
      delay(100);
      
    
    }
