// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#undef int
#undef abs
#undef double
#undef float
#undef round
#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);


byte mac[] = {0x2C, 0xF7, 0xF1, 0x08, 0x03, 0x3C };  //Replace with your Ethernet shield MAC
byte ip[] = {192, 168, 10, 129};  // Your Arduino device IP address

//char devid = "v2C8873F7EC054A1";  // THIS IS THE DEVICE ID FROM PUSHINGBOX
char devid = "v067DBD4863902C2";  // THIS IS THE DEVICE ID FROM PUSHINGBOX

int del = 5; // Amount of seconds delay between posting to google docs.


char postmsg[100];
int k = 0;
char server[] = "api.pushingbox.com";
unsigned long testTime = 0;
float h;
float t;
float f;
float hif;
float hic; 
unsigned long last_time = 0;
unsigned long deltaTime = 1000;
static char hum[15];
static char temp_f[15];
static char heat_index[15];
uint32_t delayMS;
EthernetClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHT22 test!");
  Ethernet.begin(mac, ip);
  //client.setTimeout(1000);
  delay(2000);
  Serial.println("connecting...");

}

void loop() {

  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
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

    delay(delayMS);
    if (client.connect(server, 80)== 1)
    {
      //k = 0;
      Serial.println("connected");
      //sprintf(postmsg, "GET /pushingbox?devid=v2C8873F7EC054A1&status=%s HTTP/1.1",temp_f); // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
      sprintf(postmsg, "GET /pushingbox?devid=v067DBD4863902C2&status=%s HTTP/1.1",hum); // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
      client.println(postmsg);
      client.println("Host: api.pushingbox.com");
      client.println("Connection: close");
      client.println();

      Serial.println(postmsg);
      Serial.println("Host: api.pushingbox.com");
      Serial.println("Connection: close");
      Serial.println();

      //delay(100);
      client.stop();
    }
    
    delay(delayMS);
    if (client.connect(server, 80)== 1)
    {
      //k = 0;
      Serial.println("connected");
      //sprintf(postmsg, "GET /pushingbox?devid=v2C8873F7EC054A1&status=%s HTTP/1.1",temp_f); // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
      sprintf(postmsg, "GET /pushingbox?devid=v58A493004D7C72E&status=%s HTTP/1.1",temp_f); // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
      client.println(postmsg);
      client.println("Host: api.pushingbox.com");
      client.println("Connection: close");
      client.println();

      Serial.println(postmsg);
      Serial.println("Host: api.pushingbox.com");
      Serial.println("Connection: close");
      Serial.println();

      //delay(100);
      client.stop();
    }

    delay(delayMS);
    if (client.connect(server, 80)== 1)
    {
      //k = 0;
      Serial.println("connected");
      //sprintf(postmsg, "GET /pushingbox?devid=v2C8873F7EC054A1&status=%s HTTP/1.1",temp_f); // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
      sprintf(postmsg, "GET /pushingbox?devid=vCC13DD95FD9DB19&status=%s HTTP/1.1",heat_index); // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
      client.println(postmsg);
      client.println("Host: api.pushingbox.com");
      client.println("Connection: close");
      client.println();

      Serial.println(postmsg);
      Serial.println("Host: api.pushingbox.com");
      Serial.println("Connection: close");
      Serial.println();

      //delay(100);
      client.stop();
    }
    //delay(100);

    if (!client.connected())
    {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      //k == 1;
      return;
    }
    Serial.println(millis() - testTime);
  //}

}
