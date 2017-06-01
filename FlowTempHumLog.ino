
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
#define DHTPIN 3
// Pin reading sensor
#define FLOWSENSORPIN 2

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

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
///////////////////////////////
///      EDIT THIS STUFF     //
///////////////////////////////

byte mac[] = {0x2C, 0xF7, 0xF1, 0x08, 0x03, 0x3C };  //Replace with your Ethernet shield MAC
byte ip[] = {192,168,10,129};     // Your Arduino device IP address

char devid = "v2C8873F7EC054A1";  // THIS IS THE DEVICE ID FROM PUSHINGBOX

int del=5;  // Amount of seconds delay between posting to google docs.

///////////////////////////////
//       DONE EDITING        //
///////////////////////////////


 

char postmsg[100];
int k=0;
int temp_av = 0;
char server[] = "api.pushingbox.com";
int flowSensor1 = 0;
bool sensorStatus;
bool currentStatus;
//unsigned long timer = 0;
//unsigned long deltaTime = 0;
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
 
 
void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  dht.begin();
  delay(1000);
  Serial.println("connecting...");
}
 
void loop(){
  
  
  
  
  // average temp reading for 'del' time.........................................
 
//  for(int j=0; j<del;j++)
//  {
//    // Read local temp........................................
//    int chk = dht.read(DHTPIN);
//    int temp = Fahrenheit(dht.readTemperature());
//    temp_av=temp_av+temp;
//    delay(1000);
//  }
//  
//  int avtemp=temp_av/(del);
//  temp_av=6;
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

//  Serial.println();    
//  Serial.println(currentStatus);    
//  Serial.println(sensorStatus);    
//  delay(500);
 // Post to Google Form.............................................
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
   
       //delay(100);
       client.stop();
    }
    //delay(100);
    
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


double Fahrenheit(double celsius) // Function to convert to Fahrenheit
{
	return 1.8 * celsius + 32;
}
