#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <SPI.h>
#include "DHT.h"
#include <DHT.h> 
#include <Adafruit_Sensor.h>
#undef int
#undef abs
#undef double
#undef float
#undef round
// Pin reading sensor
#define FLOWSENSORPIN 2
#define FLOW_WM_NLC 3
#define DHTPIN 4  
// DHT 22  (AM2302), AM2321
#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);


byte mac[] = {0x2C, 0xF7, 0xF1, 0x08, 0x03, 0x3C };
byte ip[] = {192,168,10,129};  


char devid = "v2C8873F7EC054A1"; 

////////////// Variables //////////////////////
// Amount of seconds delay between posting to google docs.
int del=5;
char postmsg[100];
int k=0;
char server[] = "api.pushingbox.com";
unsigned long testTime = 0;
int pulses = 0;
int pulse = 0;  // WM_NLC
bool last_state;
bool check_state;
bool last_state1; // WM_NLC
bool check_state1; // WM_NLC
float flowrate;
float flowrate1; // WM_NLC
unsigned long last_time = 0;
unsigned long deltaTime = 1000;
unsigned long last_print_time = 0;
unsigned long last_switch_time = 0;
float deltaVolume;
static char flow[15];
static char flow1[15]; // WM_NLC
float h;
float t;
float f;
float hif;
float hic; 
static char hum[15];
static char temp_f[15];
static char heat_index[15];
uint32_t delayMS;

EthernetClient client;
////////////// Variables //////////////////////


void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  dht.begin();
  delay(1000);
  Serial.println("connecting...");
  pinMode(FLOW_WM_NLC,INPUT_PULLUP);

}

void loop(){
  
///////////// Small Water Sensor /////////////
check_state = digitalRead(FLOWSENSORPIN);
  if ((check_state == LOW) && (last_state == HIGH)) {
    pulses++;
  }
  if (last_time + deltaTime < millis()){
      deltaVolume = pulses * (1.0/485.0) * (0.264172);
      flowrate = deltaVolume * 60.0 / (deltaTime/1000.0);
      Serial.print("flowrate = ");
      Serial.println(flowrate,4);
      dtostrf(flowrate,6,4,flow);
      pulses = 0;
      last_time = millis();
    }
  last_state  = check_state;

///////////// Small Water Sensor /////////////



//////////// WM_NLC Water Sensor ////////////
check_state = digitalRead(FLOW_WM_NLC);
if ((check_state1 != last_state1) && (check_state1 == LOW) && (last_switch_time + 10 < millis())) {
    pulse++;
    last_switch_time = millis();
  }

if (last_print_time + deltaTime < millis()){
      flowrate1 = pulse /(deltaTime/1000.00);
      Serial.print("pulses = ");
      Serial.println(pulse);
      dtostrf(flowrate1,6,4,flow);
      Serial.print("flowrate = ");
      Serial.println(flowrate1,4);
      dtostrf(flowrate1,6,4,flow1);
      pulse = 0;
      last_print_time = millis();
    }
  last_state1  = check_state1;
//////////// WM_NLC Water Sensor ////////////



//////////////// DHT Sensor ////////////////
delay(2000);
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
h = dht.readHumidity();
// Read temperature as Celsius (the default)
t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit = true)
f = dht.readTemperature(true);
// Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht.computeHeatIndex(t, h, false);


  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  dtostrf(h,6,4,hum);
  dtostrf(f,6,4,temp_f);
  dtostrf(hif,6,4,heat_index);
//////////////// DHT Sensor ////////////////

  
if(millis() % 500 == 0){
  testTime = millis();
    if (client.connect(server, 80)) 
    {
      k=0;
      Serial.println("connected");
      sprintf(postmsg,"GET /pushingbox?devid=v2C8873F7EC054A1&status=%s HTTP/1.1",flow);  // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
      client.println(postmsg);
      client.println("Host: api.pushingbox.com");
      client.println("Connection: close");
      client.println();
  
      Serial.println(postmsg);
      Serial.println("Host: api.pushingbox.com");
      Serial.println("Connection: close");
      Serial.println();
   
       client.stop();
    }
    
    if (!client.connected()) 
    {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      k==1;
      return;
    }
  Serial.println(millis()-testTime);
 }
 
}
