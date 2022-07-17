




// horloge


#include <RTClib.h>



#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/

#define ONE_WIRE_BUS A0 // DS18B20 plugged into pin A0 (0) on the Arduino 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
RTC_DS1307 rtc;

/********************************************************************/ 
#define Pump_Relay  2
#define Heat_Relay  3
#define Buzzer      4
#define Button_Sel  5
#define Button_Inc  6
#define Button_Dec  7
#define Button_Prev 8
#define Button_Next 9
int brewStep;
int subStep;

//*** Variable ********************************************************************/
float tSet;
float tMeasured;
DateTime setTimer;
DateTime now;
String dateNow = "12/10/1978";
String timeNow = "12:15";


float myRecipe[2][6];






// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);
// Print on LCD text on x,y position

void LCD_print(int x, int y, String t){
  lcd.setCursor(x,y);
  lcd.print(t);
}

//*** PUMP ***----------------------------------------------------- 
void pump(bool pos){
  String pumpState;
  if(pos){
    digitalWrite(Pump_Relay, HIGH);
    pumpState = "Y";
  }
  else{
    digitalWrite(Pump_Relay, LOW);
    pumpState = "N";
  }
  LCD_print(19, 3, pumpState);
}



//*** HEAT ***-----------------------------------------------------

void heat(bool pos){
  String heatState; // 
  if(pos){
    digitalWrite(Heat_Relay, HIGH);
    heatState = "Y";
  }
  else{
    digitalWrite(Heat_Relay, LOW);
    heatState = "N";
  }
  LCD_print(19, 2, heatState);
}
 
 // *** Nonitor for debugg
void printMonitor(){
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println("Temperature");
  Serial.print(tMeasured);
  Serial.print("°C");
  
}




//** Welcome **
void Welcome()
{
  
  lcd.clear();
  LCD_print(0,0,dateNow);
  LCD_print(15,0,timeNow);
  LCD_print(5,2,"Welcome!");
  LCD_print(11,3,"Next");

  if (digitalRead(Button_Sel == LOW)){
    brewStep = brewStep + 1;
    }
}
//** Select **
void Select(){
  int cursorPos;
  float recipeMono[2][6] = {{ 25, 67, 0, 0, 78, 94}, 
                          {  0, 60, 0, 0, 10, 60}};
  float recipeMulti[2][6] = {{ 25, 55, 67, 72, 78, 94}, 
                           {  0, 15, 30, 10, 10, 60}};
  
  while (digitalRead(Button_Sel == LOW)){
  lcd.clear();
  LCD_print(0,0,dateNow);
  LCD_print(15,0,timeNow);
  LCD_print(1 ,1 ,"Select and Next");
  LCD_print(2 ,2 ,"1.MonoPalier");
  LCD_print(2 ,3 ,"2.MultiPalier");
  lcd.setCursor(2,cursorPos);
  lcd.blink(); // turn on the blinking cursor
  delay(200);
  lcd.noBlink(); // turn on the blinking cursor
  delay(200);
// Select up and down
  if (digitalRead(Button_Inc == LOW)){
    if (cursorPos >= 3){
      cursorPos = 1;}
    else{
      cursorPos = cursorPos + 1;}
    } 

  if (digitalRead(Button_Dec == LOW)){
      if (cursorPos <= 2){
      cursorPos = 3;}
    else{
      cursorPos = cursorPos - 1;}
    }
    
  }
  
   switch (cursorPos){
    case 2: myRecipe[2][6] = recipeMono[2][6];
    case 3: myRecipe[2][6] = recipeMulti[2][6];
   }
  brewStep = brewStep + 1;
  

}
//** Prepare **
void Prepare(){
  switch (subStep){
    case 0: 
      LCD_print(0, 0, "add water");          // add water
      if (digitalRead(Button_Sel == LOW)){
        subStep = subStep + 1;
      }
    case 1: //heat
      tSet = myRecipe[0][0];
      if (tSet <= tMeasured){
        heat(true);
      }
      else {
        heat(false);
        subStep = subStep + 1;
      }
    case 2: ; //add malt
      LCD_print(0, 0, "add malt");          // add malt
      if (digitalRead(Button_Sel == LOW)){
       brewStep = brewStep + 1;
       subStep = 0;  
      } 
    }
}
void Mash(){
   switch (subStep){
    case 0: 
    tSet = myRecipe[0][0];
      if (tSet <= tMeasured){
        heat(true);
      }
   }
}
void Boil(){
  
}
void Cold(){
  
}





void setup()
{
  
  
// Monitor setup (used for test and debugg)
  Serial.begin(9600); 
  
  subStep = 0; 
  brewStep = 0;

   if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
     }
  
// LCD setup  
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");
  lcd.clear();
  // start serial port 
// Monitor setup (used for test and debugg)
  Serial.begin(9600); 


   
 Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Start up the library 
 sensors.begin(); 
}


void loop()
{
  tMeasured = sensors.getTempCByIndex(0); // Call temperature
  DateTime now = rtc.now();              // Call time

  printMonitor();




    // -----------
    Serial.print(" since 2000 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
  
   // 0 refers to the first IC on the wire 
   delay(1000);
   switch (brewStep){
    case 0: Welcome();
    case 1: Select();
   }
   delay(1000); 
}

// LCD screen: https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/
