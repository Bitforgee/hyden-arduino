#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "NOME_DA_SUA_REDE";      // Substitua pelo nome da sua rede Wi-Fi
const char* password = "SENHA_DA_SUA_REDE"; // Substitua pela senha da sua rede

// Configuração dos pinos
const int pinSensor = A0;      // Sensor de umidade do solo no pino A0
const int pinRelay = D1;       // Relé no pino D1 (GPIO5)
const int pinButton = D2;      // Botão manual no pino D2 (GPIO4)
const int limiteUmidade = 400; // Ajuste para o solo seco (faixa 0-1023)

// Variável para armazenar o estado anterior do botão
bool botaoAnterior = HIGH;

void setup() {
  pinMode(pinRelay, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP); // Botão usando resistor de pull-up interno
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  
  Serial.println("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conectado ao Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}
}

void loop() {
  int valorUmidade = analogRead(pinSensor);   // Lê o sensor (0 = solo molhado, 1023 = solo seco)
  bool botaoAtual = digitalRead(pinButton);

  Serial.print("Umidade: ");
  Serial.println(valorUmidade);

  bool soloSeco = valorUmidade > limiteUmidade;

  // Irrigação automática se o solo estiver seco
  if (soloSeco) {
    digitalWrite(pinRelay, LOW); // Liga o relé/bomba
    Serial.println("Irrigação automática acionada!");
  } else {
    digitalWrite(pinRelay, HIGH);  // Desliga o relé/bomba
  }

  // Irrigação manual: ativa ao pressionar o botão (bordo de descida)
  if ((botaoAnterior == HIGH) && (botaoAtual == LOW)) {
    Serial.println("Irrigação manual acionada!");
    digitalWrite(pinRelay, HIGH);
    delay(5000);                  // Irriga por 5 segundos (ajuste conforme necessário)
    digitalWrite(pinRelay, LOW);
  }
  botaoAnterior = botaoAtual;

  delay(1000); // Aguarda 1 segundo antes da próxima leitura
}