#include<Adafruit_LiquidCrystal.h>
int trig_pin=9;
int echo_pin=7;
int ldr_pin=A0;
int button_pin=6;
int led_pin=11;
int buzz_pin=13;
int required_duration=5000;
int threshold_start_time_s;
int threshold_start_time_c;
int light_val;
int distance;
int currentState = 0;

Adafruit_LiquidCrystal lcd_1(0);

float readDistance() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  long duration = pulseIn(echo_pin, HIGH);
  return duration * 0.0343 / 2;
}




bool is_above_threshold_s=false;
bool is_above_threshold_c=false;

bool anchorDown = false;

bool lastButtonState = HIGH;


int openSea=0;
int anchored=1;
int storm=2;
int charybdis=3;
int wrecked=4;


void setup() {
  // put your setup code here, to run once:
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(led_pin,OUTPUT);
  pinMode(buzz_pin, OUTPUT);
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  lcd_1.setBacklight(1);
  lcd_1.setCursor(0,1);
  lcd_1.print("OPEN SEA");
}


void loop() {
  // put your main code here, to run repeatedly:
 if (currentState == wrecked) {
    return;
 }
  
  bool buttonReading = digitalRead(button_pin);
if (lastButtonState == HIGH && buttonReading == LOW) { // Button pressed
    anchorDown = !anchorDown; // Flip anchor status
    delay(200);

    if (anchorDown) {
        // Anchor just dropped — reset timers per spec
        is_above_threshold_s = false;
        is_above_threshold_c = false;
        currentState = anchored;
        lcd_1.clear();
        lcd_1.setCursor(0,1);
        lcd_1.print("ANCHOR DROPPED");
    } else {
        // Anchor just raised — go back to open sea and let sensors re-evaluate fresh
        currentState = openSea;
        lcd_1.clear();
        lcd_1.setCursor(0,1);
        lcd_1.print("OPEN SEA");
    }
}
lastButtonState = buttonReading;

lastButtonState = buttonReading;
  
  
  
int light_val=analogRead(ldr_pin);
float distance= readDistance();
bool inStorm = (light_val < 512);
bool inCharybdis = (distance < 100);  
  
  

    

if (currentState!=anchored && currentState!=charybdis){ 
if (inStorm) {
    if (!is_above_threshold_s) {
    
      threshold_start_time_s = millis();
      is_above_threshold_s = true;
      digitalWrite(led_pin,HIGH);
      lcd_1.clear();
      lcd_1.setCursor(0,1);
      lcd_1.print("STORM");
      currentState=storm;
    } 
    else {
      
      if (millis() - threshold_start_time_s >= required_duration) {
        lcd_1.clear();
        lcd_1.setCursor(0,1);
        lcd_1.print("WRECKED");
        digitalWrite(led_pin,LOW);
        currentState=wrecked;
        return; 
      }
    }
  }
 
else {
    is_above_threshold_s = false;
  if(currentState!=openSea){
  	lcd_1.clear();
  	lcd_1.setCursor(0,1);
    lcd_1.print("OPEN SEA");
    digitalWrite(led_pin,LOW);
    currentState=openSea;}
  }
}  
if (currentState!=anchored && currentState!=storm){ 
if (inCharybdis) {
    if (!is_above_threshold_c) {
      
      threshold_start_time_c = millis();
      is_above_threshold_c = true;
      digitalWrite(buzz_pin,HIGH);
      lcd_1.clear();
      lcd_1.setCursor(0,1);
      lcd_1.print("CHARYBDIS");
      currentState=charybdis;
    } 
    else {
      
      if (millis() - threshold_start_time_c >= required_duration) {
        lcd_1.clear();
        lcd_1.setCursor(0,1);
        lcd_1.print("WRECKED");
        digitalWrite(buzz_pin,LOW);
        currentState=wrecked;
        return; 
      }
    }
  } 
  else {
    is_above_threshold_c = false;
    if(currentState!=openSea){
    lcd_1.clear();
    lcd_1.setCursor(0,1);
    lcd_1.print("OPEN SEA");
    digitalWrite(buzz_pin,LOW);
      currentState=openSea;}
  }
}
}