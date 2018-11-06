//----------BIBLIOTECAS----------
//#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
//-------------------------------

//----------PINOS----------
#define luzcozinha
#define luzsuite
#define luzbanheiro 
#define luzquarto 
#define luzsala
#define luzareadeservico
#define luzcopa
#define interruptorsala 
#define interruptorcopa
#define interruptorquarto
#define interruptorbanheiro
#define interruptorcozinha
#define interruptorsuite
#define interruptorareadeservico    
//-------------------------

//DHT dht(14, DHT11);
//float temperature = 0;

//----------CONFIGURAÇÕES MQTT----------
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[20];
const char* ssid = //NOME DA REDE
const char* password = //SENHA DA REDE
const char* mqttServer = "m14.cloudmqtt.com";
const int mqttPort =  13546;
const char* mqttUser = //USUÁRIO MQTT CLOUD
const char* mqttPassword = //SENHA MQTT CLOUD
//---------------------------------------

//----------VARIÁVEIS ESTADO LED----------
boolean luzsalaestado =false;
boolean luzcozinhaestado = false;
boolean luzcopaestado = false;
boolean luzsalaestado = false;
boolean luzsuiteestado =false;
boolean luzareadeservicoestado =false;
boolean luzquartoestado =false;
//----------------------------------------

//---------VARIÁVEIS BOTÕES----------
boolean pressionarsala = false; 
boolean pressionarcozinha = false;
boolean pressionarquarto = false;
boolean pressionarcopa = false;
boolean pressionarbanheiro = false;
boolean pressionarareadeservico = false;
boolean pressionarsuite = false;
//-----------------------------------

//----------FUNÇÃO MQTT---------- 
void callback(char* topic, byte* payload, unsigned int length) 
{
  String comand;
  char c; 
  //Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    c = (char)payload[i];
    comand += c;
  }  
  //QUARTO----------------
  if(comand.equals("LQL"))
  {
    luzquartoestado=true;
  }
  if(comand.equals("LQD"))
  {
    luzquartoestado=false;
  } 
  //----------------------
  
  //COZINHA---------------
  if(comand.equals("LCZL"))
  {
    luzcozinhaestado=true;
  }
  if(comand.equals("LCZD"))
  {
    luzcozinhaestado=false;
  }
  //----------------------
  
  //SALA------------------
  if(comand.equals("LSL"))
  {
    luzsalaestado=true;
  }
  if(comand.equals("LSD"))
  {
    luzsalaestado=false;
  } 
  //----------------------
  
  //BANHEIRO--------------
  if(comand.equals("LBL"))
  {
    luzbanheiroestado=true;
  }
  if(comand.equals("LBD"))
  {
    luzbanheiroestado=false;
  } 
  //----------------------

  //ÁREA DE SERVIÇO--------------
  if(comand.equals("LASL"))
  {
    luzareadeservicoestado=true;
  }
  if(comand.equals("LASD"))
  {
    luzareadeservicoestado=false;
  }
  //-----------------------------

  //COPA-------------------------
  if(comand.equals("LCPL"))
  {
    luzcopaestado=true;
  }
  if(comand.equals("LCPD"))
  {
    luzcopaestado=false;
  }
  //-----------------------------
  
  //SUITE------------------------
  if(comand.equals("LSTL"))
  {
    luzsuiteestado=true;
  }
  if(comand.equals("LSTD"))
  {
    luzsuiteestado=false;
  }   
 //------------------------------
}
//---------------------------------------------

 //--------------------SETUP--------------------
void setup() {
  //----------INICIAR SERIAL----------
  Serial.begin(9600);
  //----------------------------------
  //---------CONECTAR WiFi------------
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando no WiFi..");
  }
  Serial.println("Conectado na rede WiFi :)");
  //----------------------------------
  //----------CONECTAR MQTT SERVER----
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Conectando servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("Conectado ao Servidor :)");  
    } else {
      Serial.print("Deu merda :(");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("smarthouse");
  //--------------------------------
  //----------DECLARAÇÃO PINOS------
  pinMode(luzcozinha, OUTPUT);
  pinMode(luzsala, OUTPUT);
  pinMode(luzcopa, OUTPUT);
  pinMode(luzquarto, OUTPUT);
  pinMode(luzsuite, OUTPUT);
  pinMode(luzbanheiro, OUTPUT);
  pinMode(luzareadeservico, OUTPUT);
  pinMode(interruptorsala, INPUT);
  pinMode(interruptorcopa, INPUT);
  pinMode(interruptorcozinha, INPUT);
  pinMode(interruptorquarto, INPUT);
  pinMode(interruptorsuite, INPUT);
  //--------------------------------
}

//-------------------LOOP--------------------
void loop() {
  client.loop();
  long now = millis();

//-------------------------------------------------------IDENTIFICANDO INTERRUPTORES-------------------------------------------------------
//--------------------INTERRUPTOR SALA--------------------  
  if ((digitalRead(interruptorsala))==1){
    pressionarsala=true;
  }
  if ((pressionarsala==true) and (digitalRead(interruptorsala)==0)) {
    pressionarsala=false;
    luzsalaestado=!luzsalaeestado;
    if (luzsalaestado==true){
      client.publish("smarthouse", "LSL");
    }else{
      client.publish("smarthouse", "LSD");
    }
  }
//-------------------------------------------------------
//--------------------INTERRUPTOR COZINHA--------------------  
  if ((digitalRead(interruptorcozinha))==1){
    pressionarcozinha=true;
  }
  if ((pressionarcozinha==true) and (digitalRead(interruptorcozinha)==0)) {
    pressionarcozinha=false;
    luzcozinhaestado=!luzcozinhaeestado;
    if (luzcozinhaestado==true){
      client.publish("smarthouse", "LCZL");
    }else{
      client.publish("smarthouse", "LCZD");
    }
  }
//-------------------------------------------------------

//--------------------INTERRUPTOR COPA--------------------  
    if ((digitalRead(interruptorcopa))==1){
    pressionarcopa=true;
  }
  if ((pressionarcopa==true) and (digitalRead(interruptorcopa)==0)) {
    pressionarcopa=false;
    luzcopaestado=!luzcopaeestado;
    if (luzcopaestado==true){
      client.publish("smarthouse", "LCPL");
    }else{
      client.publish("smarthouse", "LCPD");
    }
  }
//-------------------------------------------------------

//--------------------INTERRUPTOR QUARTO--------------------  
  if ((digitalRead(interruptorquarto))==1){
    pressionarquarto=true;
  }
  if ((pressionarquarto==true) and (digitalRead(interruptorquarto)==0)) {
    pressionarquarto=false;
    luzquartoestado=!luzquartoestado;
    if (luzquartoestado==true){
      client.publish("smarthouse", "LQL");
    }else{
      client.publish("smarthouse", "LQD");
    }
  }
//-------------------------------------------------------

//--------------------INTERRUPTOR SUÍTE--------------------  
  if ((digitalRead(interruptorsuite))==1){
    pressionarsuite=true;
  }
  if ((pressionarsuite==true) and (digitalRead(interruptorsuite)==0)) {
    pressionarsuite=false;
    luzsuiteestado=!luzsuiteestado;
    if (luzsuiteestado==true){
      client.publish("smarthouse", "LSTL");
    }else{
      client.publish("smarthouse", "LSTD");
    }
  }
//-------------------------------------------------------




//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------APLICANDO MODIFICAÇÕES LUZES-------------------------------------------------------

//--------------------SALA--------------------
if ((luzsalaestado==true)) {
  digitalWrite(luzsala, HIGH);
}
if ((luzsalaestado==false)) {
  digitalWrite(luzsala, LOW);
}
//-------------------------------------------------

//--------------------QUARTO--------------------
if ((luzquartoestado==true)) {
  digitalWrite(luzquarto, HIGH);
}
if ((luzquartoestado==false)) {
  digitalWrite(luzquarto, LOW);
}
//----------------------------------------------------

//--------------------COZINHA--------------------
if ((luzcozinhaestado==true)) {
  digitalWrite(luzcozinha, HIGH);
}
if ((luzcozinhaestado==false)) {
  digitalWrite(luzcozinha, LOW);
}
//---------------------------------------------------

//--------------------COPA--------------------
if ((luzcopaestado==true)) {
  digitalWrite(luzcopa, HIGH);
}
if ((luzcopaestado==false)) {
  digitalWrite(luzcopa, LOW);
}
//------------------------------------------------

//--------------------WC--------------------
if ((luzbanheiroestado==true)) {
  digitalWrite(luzbanheiro, HIGH);
}
if ((luzbanheiroestado==false)) {
  digitalWrite(luzbanheiro, LOW);
}
//------------------------------------------------

//--------------------ÁREA DE SERVIÇO--------------------
if ((luzareadeservicoestado==true)) {
  digitalWrite(luzareadeservico, HIGH);
}
if ((luzareadeservicoestado==false)) {
  digitalWrite(luzsareadeservico, LOW);
}
//------------------------------------------------

//--------------------SUÍTE--------------------
if ((luzsuiteestado==true)) {
  digitalWrite(luzsuite, HIGH);
}
if ((luzsuiteestado==false)) {
  digitalWrite(luzsuite, LOW);
}
//------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------
  }
/*  if (now - lastMsg > 3000) {
    lastMsg = now;
     //read DHT11/DHT22 sensor and convert to string 
    temperature = dht.readTemperature();
    if (!isnan(temperature)) {
      snprintf (msg, 20, "%lf", temperature);
      // publish the message 
      Serial.println(msg);
      client.publish("TEMP_TOPIC", msg);
    }
  }*/


