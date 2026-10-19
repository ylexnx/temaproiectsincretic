#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int sensorPin = A0; 

volatile int ore = 0;
volatile int minute = 0;
volatile int secunde = 0;


volatile bool updateFlag = false; 

void setup() {
 
  lcd.begin(16, 2);
  Serial.begin(9600);

  ore = 12;
  minute = 23;
  secunde = 0;


  
  noInterrupts(); 

  TCCR1A = 0;    
  TCCR1B = 0;
  TCNT1  = 0;    

  
  OCR1A = 15625;            

  TCCR1B |= (1 << WGM12);  
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);  

  interrupts(); 
}


ISR(TIMER1_COMPA_vect) {
  secunde++;
  if (secunde >= 60) {
    secunde = 0;
    minute++;
    if (minute >= 60) {
      minute = 0;
      ore++;
      if (ore >= 24) {
        ore = 0;
      }
    }
  }
  updateFlag = true; 
}
void loop() {
 
  int sensorValue = analogRead(sensorPin);
  float milivolti = (sensorValue * 5000.0) / 1023.0;
  float temperatura = milivolti / 10.0;

  
  if (updateFlag || true) { 
    
   lcd.setCursor(0, 0);
    lcd.print("Temp = ");
    lcd.print((int)temperatura); 
    lcd.print(" oC");          
    
    lcd.setCursor(0, 1);
    lcd.print("Ora ");
    if (ore < 10) lcd.print("0");
    lcd.print(ore);
    lcd.print(":");
    if (minute < 10) lcd.print("0");
    lcd.print(minute);
    lcd.print(":");
    if (secunde < 10) lcd.print("0");
    lcd.print(secunde);

    
    Serial.print("Temp: ");
    Serial.print(temperatura);
    Serial.print(" C | Ora: ");
    if (ore < 10) Serial.print("0");
    Serial.print(ore);
    Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.print(minute);
    Serial.print(":");
    if (secunde < 10) Serial.print("0");
    Serial.println(secunde);

    updateFlag = false;
  }
  
  delay(500);
}
