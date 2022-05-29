#include <ArduinoJson.h>   // Lib para usar JSON
#include "DHT.h"           // Lib para usar o DHT11
#include "EspMQTTClient.h" // Lib oara usar o MQTT Client + WIFI

// DHT11 -> Umidade e Temperatura do Ar
#define DHTTYPE DHT11
const int DHTPIN = 12;    // D7(ESP82) D12(ESP32) Sensor de temperatura/umidade ar
DHT dht(DHTPIN, DHTTYPE); // Atribuindo como o DHT irá funcionar

// Válvula solenoide
const int VALVULA = 13; // D6(ESP82) D13(ESP32) Válvula solenoide no RELE
const int LED = 2;      // D22 (ESP32)

// Analógico
const int SOLO = A3; // Sensor de umidade de solo

// Criando json para envio de informação
DynamicJsonDocument doc(256)

void messageHandler(const String &msg);

EspMQTTClient client(
    "Jailson",         // SSID
    "@theo121299jail", // PASSWORD WIFI
    "192.168.3.11",    // MQTT Broker server ip
    "",                // MQTTUsername Can be omitted if not needed
    "",                // MQTTPassword Can be omitted if not needed
    "ESP32",           // Client name that uniquely identify your device
    3000               // The MQTT port
);

void setup()
{
  Serial.begin(115200);

  // Definindo as gpio
  pinMode(VALVULA, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(SOLO, INPUT);

  // Iniciando o DHT11
  dht.begin();

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  // client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA();               // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("ESP32/lastwill", "I am going offline");
  delay(1000)
}

void onConnectionEstablished()
{
  client.subscribe("iot/farm", messageHandler);
  // Publish a message to "iot/farm"
  client.publish("iot/farm", "ESP32 - Conectado!"); // You can activate the retain flag by setting the third parameter to true
}

void messageHandler(const String &msg)
{
  char json[] = msg;
  DeserializationError error = deserializeJson(doc, json);
  if (doc["action"] == "lj")
  {
    digitalWrite(LED, HIGH);
  }
  else if (doc["action"] == "dj")
  {
    digitalWrite(LED, LOW);
  }
  else if (doc["action"] == "releaseWater")
  {
    releaseWater(doc["ms"]);
  }
  else if (doc["action"] == "sendData")
  {
    sendData()
  }
}

void loop()
{
  client.loop();
}

void sendData()
{
  String dataString;
  doc["umi"] = dht.readHumidity();
  doc["temp"] = dht.readTemperature();
  doc["umi_s"] = readSoilHumidity();

  // Print para debug
  serializeJson(doc, Serial);
  serializeJson(doc, dataString);

  if (client.publish("iot/farm", dataString))
  {
    Serial.println("Sucesso ao enviar dados");
  }
  else
  {
    Serial.println("Erro ao enviar dados");
  }
}

// Função para ler o sensor de umidade de solo
int readSoilHumidity()
{
  int vlr = analogRead(SOLO);
  vlr = map(vlr, 100, 1023, 100, 0);
  return vlr;
}

// Vale ressaltar que a válvula solenoide está como normalmente fechada
// Logo ao mandar um sinal low ela abre o circuito para que passe a energia
void releaseWater(int valueMS)
{
  digitalWrite(VALVULA, LOW);
  delay(valueMS);
  digitalWrite(VALVULA, HIGH);
}
