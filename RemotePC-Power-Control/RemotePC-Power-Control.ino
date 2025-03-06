#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

// Configurações de Wi-Fi
const char* ssid = "NOME_DA_REDE_WIFI";
const char* password = "SENHA_DA_REDE";

// Configurações do MQTT
const char* mqtt_server = "SEU_SERVIDOR_MQTT";
const int mqtt_port = 8883;
const char* mqtt_user = "USUARIO_MQTT";
const char* mqtt_password = "SENHA_MQTT";

// Configurações Wake-on-LAN
const char* pc_mac_str = "XX:XX:XX:XX:XX:XX"; 
uint8_t mac_address[6];
const int wol_port = 9;

WiFiClientSecure espClient;
PubSubClient client(espClient);
WiFiUDP udp;

void setup_wifi() {
  Serial.begin(115200);
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado! IP: ");
  Serial.println(WiFi.localIP());

  sscanf(pc_mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
         &mac_address[0], &mac_address[1], &mac_address[2],
         &mac_address[3], &mac_address[4], &mac_address[5]);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado!");
      client.subscribe("comando/wol");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s...");
      delay(5000);
    }
  }
}

void sendWOL() {
  uint8_t magicPacket[102];
  
  memset(magicPacket, 0xFF, 6);
  
  for (int i = 6; i < 102; i += 6) {
    memcpy(&magicPacket[i], mac_address, 6);
  }

  udp.beginPacket("255.255.255.255", wol_port);
  udp.write(magicPacket, sizeof(magicPacket));
  udp.endPacket();
  
  Serial.println("Pacote WoL enviado!");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (message == "ligarPC") {
    sendWOL();
  }
}

void setup() {
  setup_wifi();
  
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}