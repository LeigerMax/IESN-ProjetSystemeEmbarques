#include <SPI.h>
#include <RH_RF95.h>
#include <MsTimer2.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3
#define RF95_FREQ 433.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int pinCapteurTemperature = A0;
float volt = 4.67;
float temperatureC;
bool flag =0;
char table[10];

void setup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(9600);

  Serial.println("Arduino LoRa TX ! ");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("L'initialisation radio LoRa a échoué");
  }
  Serial.println("L'initialisation radio LoRa OK !");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency a échoué");
  }

  Serial.print("Freq sur : "); Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);
  MsTimer2::set(1000, TemperatureAndLora);
  MsTimer2::start();
}

void loop()
{
  if(flag){
    float lectureTemperature = analogRead(pinCapteurTemperature);
    float temp = (((lectureTemperature / 1023.0) * volt) * 100.0);
    temperatureC = temp - 273.0;
    itoa(temperatureC,table,10);
    rf95.send((uint8_t *)table, 10);
    rf95.waitPacketSent();
    Serial.println("envoie");
    flag =0;
  }
}

void TemperatureAndLora(void) {
  flag =1;
}
