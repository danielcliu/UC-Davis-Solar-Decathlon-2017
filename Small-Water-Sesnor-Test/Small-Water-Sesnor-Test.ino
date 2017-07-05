
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <SPI.h>
#include <DHT.h> 
#include <Adafruit_Sensor.h>
#undef int
#undef abs
#undef double
#undef float
#undef round
// Pin reading sensor
#define FLOWSENSORPIN 2

///////////////////////////////
///      EDIT THIS STUFF     //
///////////////////////////////

byte mac[] = {0x2C, 0xF7, 0xF1, 0x08, 0x03, 0x3C };  //Replace with your Ethernet shield MAC
byte ip[] = {192,168,10,129};     // Your Arduino device IP address

///////////////////////////////
//       DONE EDITING        //
///////////////////////////////

char postmsg[100];
int k=0;
int temp_av = 0;
char server[] = "www.db.ucd-sd.net/";
int flowSensor1 = 0;
bool sensorStatus;
bool currentStatus;
unsigned long testTime = 0;
// count number of pulses
int pulses = 0;
// last state of sensor
bool last_state;
// check current state of sensor
bool check_state;
// calculate flow rate
float flowrate;
// time
unsigned long last_time = 0;
unsigned long deltaTime = 1000;
float deltaVolume;
static char flow[15];

EthernetClient client;
//bool connected = false; 

 
void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  //server.begin();
  //Serial.print("Arduino server IP address: ");
  //Serial.println(Ethernet.localIP());
  delay(1000);
}
 
void loop(){
  
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

// Connect to database.............................................................
if (client.connect(server,80)){
  Serial.print("Connected to ");
  Serial.println(server);
  client.print("GET /write_data.php?");
  client.print("value=");
  client.print(flow);
  Serial.println(flow);
  client.print(" HTTP/1.1");  // Part of GET request
  client.print("Host: db.ucd-sd.net");
  client.println("Connection: close");  // Part of the GET request telling the server that we are over transmitting the message
  client.println(); // Empty Line
  client.println(); // Empty Line
  client.stop();    // Closing connection to server
  Serial.println("Disonnected");
}
else {
  Serial.println("--> connection failed\n");
}
delay(1000);
}
