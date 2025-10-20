#include <ESP8266WiFi.h>

const int pinSensor = A0;
const int pinRelay  = D1;
const int pinButton = D2;

const int limiteUmidade = 400;
bool botaoAnterior = HIGH;

void setup() {
  pinMode(pinRelay, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Sistema de irrigação iniciado...");
}

void loop() {

  int valorUmidade = analogRead(pinSensor);
  bool botaoAtual = digitalRead(pinButton);

  Serial.print("Umidade: ");
  Serial.println(valorUmidade);

  bool soloSeco = valorUmidade > limiteUmidade;

  if (soloSeco) {
    digitalWrite(pinRelay, HIGH);
    Serial.println("Irrigação automática acionada!");
  } else {
    digitalWrite(pinRelay, LOW);
  }

  if ((botaoAnterior == HIGH) && (botaoAtual == LOW)) {
    Serial.println("Irrigação manual acionada!");
    digitalWrite(pinRelay, HIGH);
    delay(5000);
    digitalWrite(pinRelay, LOW);
  }

  botaoAnterior = botaoAtual;

  delay(1000);
}
