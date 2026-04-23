#include <HTTPClient.h>
#include "AdafruitIO_WiFi.h"
#include <DHT.h>
#include <WiFi.h>

// ==== Adafruit io ====

#define IO_USERNAME "KauanLopes02"
#include "config.h"

#define WIFI_SSID "Galaxy A71A624"
#define WIFI_PASS "12345678"

//=== DHT11 ===
#define DHTPIN 4 // pino Data do dht11 ao GPIO4
#define DHTTYPE DHT11

const char* serverName = "https://adafruit.onrender.com/sensor";

DHT dht(DHTPIN, DHTTYPE);

AdafruitIO_WiFi io(IO_USERNAME,IO_KEY, WIFI_SSID, WIFI_PASS);

// === FEEDA ===
AdafruitIO_Feed *temperatura = io.feed("temperatura");
AdafruitIO_Feed *umidade = io.feed("umidade");

// === Variaveis ===

float ultimaTemp = -1000;
float ultimaUmidade = -1000;

void setup(){
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  Serial.print("Iniciando conexão com Adafruit IO...");

  io.connect();
  //Loop
  while(io.status() < AIO_CONNECTED){
    Serial.print("Status: ");
    Serial.println(io.statusText());
    delay(1000);
  }
  Serial.println("Conectado ao Adafruit IO!");
}

void loop(){

  io.run();

  //Verifica conexão continuamente
  if(io.status() != AIO_CONNECTED){
    Serial.print("Desconectado: ");
    Serial.println(io.statusText());
    return;
  }

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  if(isnan(temp) || isnan(hum)){
    Serial.println("Erro ao ler o DHT11");
    delay(2000);
    return;
  }

  if(abs(temp - ultimaTemp) >= 0.2){
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println(" °C");
  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    
    String url = String(serverName) + "?temp=" + String(temp) + "&hum=" + String(hum);

    http.begin(url);

    int httpResponseCode = http.GET();
    Serial.print("Código HTTP");
    Serial.println(httpResponseCode);

    http.end();
  }

    temperatura -> save(temp);
    ultimaTemp = temp;
  }

  if(abs(hum - ultimaUmidade) >=1.0){
    Serial.print("Umidade: ");
    Serial.print(hum);
    Serial.println("%");

    umidade-> save(hum);
    ultimaUmidade = hum;
  }
  delay(3000);

}