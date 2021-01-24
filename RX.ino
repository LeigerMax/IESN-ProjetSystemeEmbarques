#include <SPI.h>
#include <RH_RF95.h>
#include <MsTimer2.h>
#include<stdio.h>
#include <stdlib.h>
#include <LiquidCrystal.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3
#define RF95_FREQ 431.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int pinVentilateur =  6;
int temperatureC;
bool flag = 0;
LiquidCrystal lcd(8,7,5,2,A0,A1);

void setup()
{
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  pinMode(pinVentilateur,OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(9600);

  Serial.println("Arduino LoRa RX ! Allemeersch Maxime");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("L'initialisation radio LoRa a échoué");
    while (1);
  }
  Serial.println("L'initialisation radio LoRa OK !");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency a échoué ");
    while (1);
  }
  Serial.print("Freq sur : "); Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);
  MsTimer2::set(1000, lcdFonction);
  MsTimer2::start();
}

void loop()
{
  if (rf95.available())
  {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      temperatureC = atoi((char*)buf);
      Serial.print("Temperature : ");
      Serial.println(temperatureC);

      if (temperatureC > 15) {
        digitalWrite(pinVentilateur,HIGH);
      }
      else {
        digitalWrite(pinVentilateur,0);
      }
    }
    else
    {
      Serial.println("La réception a échoué !");
    }
  }

  if(flag){
    lcd.clear();
    lcd.print("Temperature ");
    lcd.print(temperatureC);
    lcd.print(" C");
    flag=0;
  }
}

void lcdFonction(void) {
  flag=1;
}
