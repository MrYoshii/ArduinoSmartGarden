    #include <Servo.h>
    #include "DHT.h"
    #include <LiquidCrystal.h>

     
    #define DHTPIN 7     // what pin we're connected to
    #define DHTTYPE DHT11   // DHT 22  (AM2302)
    #define fan 8
    #define heat 6
    #define pumpe A2
    #define feucht A3 //10 pinmode vorher (digital)
    #define feucht2 A4
    #define servo 13
    
    LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
     
    int maxHum = 50;
    int maxTemp = 32;
    int minHum = 40;
    int minTemp = 30;
    byte Celsius[8] = {B00100,B01010,B00100,B0000,B00000,B00000,B00000,B00000}; //° Pixel festgelegt!!
    long laenge;
    long cm;
    int z = 0;
    int Pflanze1 = 0; //Deklarierung als Integer für Pfalnze 1
    int Pflanze2 = 0; //Deklarierung als Integer für Pflanze 2
    int Pflanzemax = 700; //Maximaler Feuchtigkeitswert für Berechnung
    int maxfeuchtigkeitslvl = 400;
    int minfeuchtigkeitslvl = 250;
    float sensorPin = A3; //Sensor Pflanze 1 auf Pin Analog 0
    float sensorPin2 = A4; //Sensor Pflanze 2 auf Pin Analog 1
    
    DHT dht(DHTPIN, DHTTYPE);
    Servo myservo;
     
    void setup() {

    
      lcd.begin(16, 2);
      
      
      pinMode(13,OUTPUT);
      pinMode(fan, OUTPUT);
      pinMode(heat, OUTPUT);
      pinMode(feucht,OUTPUT);
      pinMode(feucht2,OUTPUT);
      pinMode(pumpe, OUTPUT);
      myservo.attach(13); //servo
      
      Serial.begin(9600);
      
      delay(500); 
      dht.begin();
     
    }
     
    void loop() {

     String DisplayWords;
     float sensorValue;
     float sensorValue2; // Pflanze 2 
     sensorValue = analogRead(sensorPin); 
     sensorValue2 = analogRead(sensorPin2); //Sensor, als analogwert lesen
     
     Serial.print("Water Level: ");
     Serial.print(String(sensorValue));
     Serial.print("   Prozent:");
     Serial.print(Pflanze1);
     Serial.print("%");
     Serial.print("     Water Level: ");
     Serial.print(String(sensorValue));
     Serial.print("   Prozent:");
     Serial.print(Pflanze2);
     Serial.print("%");
     Serial.print("\n");

     Pflanze1 = 0;

     Pflanze1 = ((sensorValue*100)/Pflanzemax); //Prozent Berechnung
     Pflanze2 = ((sensorValue2*100)/Pflanzemax); //Prozent Berechnung

     pinMode(9,OUTPUT);
     pinMode(A1,INPUT); //10 pinmode vorher

     digitalWrite(9,LOW);
     delayMicroseconds(2);
     digitalWrite(9,HIGH);
     delayMicroseconds(5);
     digitalWrite(9,LOW);

  
     delay(2000);
     
     float h = dht.readHumidity();// Liest die Luftfeuchtigkeit in % aus
      
     float t = dht.readTemperature();// Liest die Temperatur in Celsius aus
      
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
      //Temperaturregelung
      if(t < minTemp) {
          digitalWrite(heat, HIGH);
      }if (t >=28 ) {
         digitalWrite(heat,LOW); 
     
      if(h < minHum) {
          digitalWrite(feucht, HIGH);
      } else {
         digitalWrite(feucht, LOW); 

      }
      }


 


  if (t >= maxTemp or h >= maxHum)
      myservo.write(115);

  else{
      myservo.write(10);
  }
        
    
if ( z <= 2 ){
      lcd.createChar(0, Celsius); 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp:"); // String mit Temperatur+ 9 Leerzeilen und Grad
      lcd.setCursor(14,0);
      
      lcd.write((uint8_t)0);// ° zeichen eingefügt
      lcd.setCursor(15,0);
      lcd.print("C");
      lcd.setCursor(10,0); // 2 Zeilen-Display
      lcd.print(t, 1); // gibt die Temperatur aus

      
      lcd.setCursor(0,1);
      lcd.print("Humidity:");      
      lcd.setCursor(13,1);
      lcd.print(h);
      lcd.setCursor(15,1);
      lcd.print("%");
      z = z + 1;
 }
delay(2000);
if ( z >= 3 ){
    lcd.clear();
    //lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("Pflanze 1:");
    lcd.setCursor(0,1);
    lcd.print("Pflanze 2:");
    lcd.setCursor(12,0);    //Wert wird nach rechts ausgegeben
    lcd.print(Pflanze1);   //Feuchtigkeitssensor 1
    lcd.setCursor(12,1);
    lcd.print(Pflanze2);  //Feuchtigkeitssensor 2
    lcd.setCursor(15,0);
    lcd.print("%");
    lcd.setCursor(15,1);
    lcd.print("%");
    z = z + 1;
}
if ( z >= 6){
    z = 0;
}
      
    }
    
