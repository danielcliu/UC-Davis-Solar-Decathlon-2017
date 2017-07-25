
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

// distributio flow sensor pins
#define FLOWSENSORPIN_1 2 // Small water sensor pin reading
#define FLOWSENSORPIN_2 2 // Small water sensor pin reading
#define FLOWSENSORPIN_3 2 // Small water sensor pin reading
#define FLOWSENSORPIN_4 2 // Small water sensor pin reading
#define FLOWSENSORPIN_5 2 // Small water sensor pin reading
#define FLOWSENSORPIN_6 2 // Small water sensor pin reading
#define FLOWSENSORPIN_7 2 // Small water sensor pin reading
#define FLOWSENSORPIN_8 2 // Small water sensor pin reading
#define FLOWSENSORPIN_9 2 // Small water sensor pin reading
#define FLOWSENSORPIN_10 2 // Small water sensor pin reading
#define FLOWSENSORPIN_11 2// Small water sensor pin reading
#define FLOWSENSORPIN_12 2 // Small water sensor pin reading
#define FLOWSENSORPIN_13 2 // Small water sensor pin reading
#define FLOWSENSORPIN_14 2 // Small water sensor pin reading

// main water flow sensor pin
#define WMNLC 3         // Large water sensor pin reading

// Temp & Humidity
#define DHTTYPE DHT22   // We are using the DHT22 for all sensors

#define DHTPIN_1 4        // DHT sensor pin reading
#define DHTPIN_2 4
#define DHTPIN_3 4
#define DHTPIN_4 4
#define DHTPIN_5 4
#define DHTPIN_6 4
#define DHTPIN_7 4
#define DHTPIN_8 4

DHT dht1(DHTPIN_1, DHTTYPE);
DHT dht2(DHTPIN_2, DHTTYPE);
DHT dht3(DHTPIN_3, DHTTYPE);
DHT dht4(DHTPIN_4, DHTTYPE);
DHT dht5(DHTPIN_5, DHTTYPE);
DHT dht6(DHTPIN_6, DHTTYPE);
DHT dht7(DHTPIN_7, DHTTYPE);
DHT dht8(DHTPIN_8, DHTTYPE);

byte mac[] = {0x2C, 0xF7, 0xF1, 0x08, 0x03, 0x3C };  //Replace with your Ethernet shield MAC
byte ip[] = {192, 168, 10, 129};  // Your Arduino device IP address

/////////////////////////////////////////////////
//              Decalred variable              //
////////////////////////////////////////////////
char server[] = "ucd-sd.net";

// Temp /Humidity sensore variables
float h1; // humidity sensor 1
float t1; // temp in celcius sensor 1
float f1; // temp in farenheit sensor 1
float h2; // humidity sensor 2
float t2; // temp in celcius sensor 2
float f2; // temp in farenheit sensor 2
float h3; // humidity sensor 3
float t3; // temp in celcius sensor 3
float f3; // temp in farenheit sensor 3
float h4; // humidity sensor 4
float t4; // temp in celcius sensor 4
float f4; // temp in farenheit sensor 4
float h5; // humidity sensor 5
float t5; // temp in celcius sensor 5
float f5; // temp in farenheit sensor 5
float h6; // humidity sensor 6
float t6; // temp in celcius sensor 6
float f6; // temp in farenheit sensor 6
float h7; // humidity sensor 7
float t7; // temp in celcius sensor 7
float f7; // temp in farenheit sensor 7
float h8; // humidity sensor 8
float t8; // temp in celcius sensor 8
float f8; // temp in farenheit sensor 8

float current_f1;
float current_h1;

// Number of pulses
int pulses_1 = 0;
int pulses_2 = 0;
int pulses_3 = 0;
int pulses_4 = 0;
int pulses_5 = 0;
int pulses_6 = 0;
int pulses_7 = 0;
int pulses_8 = 0;
int pulses_9 = 0;
int pulses_10 = 0;
int pulses_11 = 0;
int pulses_12 = 0;
int pulses_13 = 0;
int pulses_14 = 0;
int pulses_WM = 0;
// last state of sensor
bool last_state_1;
bool last_state_2;
bool last_state_3;
bool last_state_4;
bool last_state_5;
bool last_state_6;
bool last_state_7;
bool last_state_8;
bool last_state_9;
bool last_state_10;
bool last_state_11;
bool last_state_12;
bool last_state_13;
bool last_state_14;

bool last_state_WM;

// check current state of sensor
bool check_state_1;
bool check_state_2;
bool check_state_3;
bool check_state_4;
bool check_state_5;
bool check_state_6;
bool check_state_7;
bool check_state_8;
bool check_state_9;
bool check_state_10;
bool check_state_11;
bool check_state_12;
bool check_state_13;
bool check_state_14;

bool  check_state_WM;

// calculate flow rate
float flowrate;
int ID[15]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
// time
unsigned long last_time = 0;
unsigned long deltaTime = 1000;
unsigned long last_print_time = 0;
unsigned long last_switch_time = 0;
float deltaVolume;
uint32_t delayMS;
int temp[15] = {};
unsigned long time_since_last_reset = 0;
/*
  unsigned long last_pulse_1 = 0;
  unsigned long last_pulse_2 = 0;
  unsigned long last_pulse_3 = 0;
  unsigned long last_pulse_4 = 0;
  unsigned long last_pulse_5 = 0;
  unsigned long last_pulse_6 = 0;
  unsigned long last_pulse_7 = 0;
  unsigned long last_pulse_8 = 0;
  unsigned long last_pulse_9 = 0;
  unsigned long last_pulse_10 = 0;
  unsigned long last_pulse_11 = 0;
  unsigned long last_pulse_12 = 0;
  unsigned long last_pulse_13 = 0;
  unsigned long last_pulse_14 = 0;
*/
//int water_flow_interval = 1;
//float interval = 1; // 1s

EthernetClient client;

/////////// Available SRAM //////////////
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
/////////////////////////////////////////


void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  pinMode(WMNLC, INPUT_PULLUP);
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
  dht6.begin();
  dht7.begin();
  dht8.begin();
  delay(1000);
}

void loop() {


  // mains water sensor
  check_state_WM = digitalRead(WMNLC);
  if ((check_state_WM != last_state_WM) && (check_state_WM == LOW) && (last_switch_time + 10 < millis())) {
    pulses_WM++;
    last_switch_time = millis();
  }

  //time_since_last_reset = millis();
  //while ((millis() - time_since_last_reset) < interval) {

  check_state_1 = digitalRead(FLOWSENSORPIN_1);
  if ((check_state_1 == LOW) && (last_state_1 == HIGH)) {
    //if ((check_state_1 == LOW) && (last_state_1 == HIGH) && (millis() - last_pulse_1 < water_flow_interval)) {
    pulses_1++;
    //last_pulse_1 = millis();
    Serial.println(pulses_1);
  }

  check_state_2 = digitalRead(FLOWSENSORPIN_2);
  if ((check_state_2 == LOW) && (last_state_2 == HIGH)) {
    //if ((check_state_2 == LOW) && (last_state_2 == HIGH) && (millis() - last_pulse_2 < water_flow_interval)) {
    pulses_2++;
    //last_pulse_2 = millis();
  }

  check_state_3 = digitalRead(FLOWSENSORPIN_3);
  if ((check_state_3 == LOW) && (last_state_3 == HIGH)) {
    //if ((check_state_3 == LOW) && (last_state_3 == HIGH) && (millis() - last_pulse_3 < water_flow_interval)) {
    pulses_3++;
    //last_pulse_3 = millis();
  }

  check_state_4 = digitalRead(FLOWSENSORPIN_4);
  if ((check_state_4 == LOW) && (last_state_4 == HIGH)) {
    //if ((check_state_4 == LOW) && (last_state_4 == HIGH) && (millis() - last_pulse_4 < water_flow_interval)) {
    pulses_4++;
    // last_pulse_4 = millis();
  }

  check_state_5 = digitalRead(FLOWSENSORPIN_5);
  if ((check_state_5 == LOW) && (last_state_5 == HIGH)) {
    //if ((check_state_5 == LOW) && (last_state_5 == HIGH) && (millis() - last_pulse_5 < water_flow_interval)) {
    pulses_5++;
    //last_pulse_5 = millis();
  }

  check_state_6 = digitalRead(FLOWSENSORPIN_6);
  if ((check_state_6 == LOW) && (last_state_6 == HIGH)) {
    //if ((check_state_6 == LOW) && (last_state_6 == HIGH) && (millis() - last_pulse_6 < water_flow_interval)) {
    pulses_6++;
    //last_pulse_6 = millis();
  }

  check_state_7 = digitalRead(FLOWSENSORPIN_7);
  if ((check_state_7 == LOW) && (last_state_7 == HIGH)) {
    //if ((check_state_7 == LOW) && (last_state_7 == HIGH) && (millis() - last_pulse_7 < water_flow_interval)) {
    pulses_7++;
    // last_pulse_7 = millis();
  }

  check_state_8 = digitalRead(FLOWSENSORPIN_8);
  if ((check_state_8 == LOW) && (last_state_8 == HIGH)) {
    //if ((check_state_8 == LOW) && (last_state_8 == HIGH) && (millis() - last_pulse_8 < water_flow_interval)) {
    pulses_8++;
    // last_pulse_8 = millis();
  }

  check_state_9 = digitalRead(FLOWSENSORPIN_9);
  if ((check_state_9 == LOW) && (last_state_9 == HIGH)) {
    //if ((check_state_9 == LOW) && (last_state_9 == HIGH) && (millis() - last_pulse_9 < water_flow_interval)) {
    pulses_9++;
    // last_pulse_9 = millis();
  }

  check_state_10 = digitalRead(FLOWSENSORPIN_10);
  if ((check_state_10 == LOW) && (last_state_10 == HIGH)) {
    //if ((check_state_10 == LOW) && (last_state_10 == HIGH) && (millis() - last_pulse_10 < water_flow_interval)) {
    pulses_10++;
    //last_pulse_10 = millis();
  }

  check_state_11 = digitalRead(FLOWSENSORPIN_11);
  if ((check_state_11 == LOW) && (last_state_11 == HIGH)) {
    //if ((check_state_11 == LOW) && (last_state_11 == HIGH) && (millis() - last_pulse_11 < water_flow_interval)) {
    pulses_11++;
    // last_pulse_11 = millis();
  }

  check_state_12 = digitalRead(FLOWSENSORPIN_12);
  if ((check_state_12 == LOW) && (last_state_12 == HIGH)) {
    //if ((check_state_12 == LOW) && (last_state_12 == HIGH) && (millis() - last_pulse_12 < water_flow_interval)) {
    pulses_12++;
    //last_pulse_12 = millis();
  }

  check_state_13 = digitalRead(FLOWSENSORPIN_13);
  if ((check_state_13 == LOW) && (last_state_13 == HIGH)) {
    //if ((check_state_13 == LOW) && (last_state_13 == HIGH) && (millis() - last_pulse_13< water_flow_interval)) {
    pulses_13++;
    // last_pulse_13 = millis();
  }

  check_state_14 = digitalRead(FLOWSENSORPIN_14);
  if ((check_state_14 == LOW) && (last_state_14 == HIGH)) {
    //if ((check_state_14 == LOW) && (last_state_14 == HIGH) && (millis() - last_pulse_14 < water_flow_interval)) {
    pulses_14++;
    //last_pulse_14 = millis();
  }

  //}


  temp [0] = pulses_1;
  temp [1] = pulses_2;
  temp [2] = pulses_3;
  temp [3] = pulses_4;
  temp [4] = pulses_5;
  temp [5] = pulses_6;
  temp [6] = pulses_7;
  temp [7] = pulses_8;
  temp [8] = pulses_9;
  temp [9] = pulses_10;
  temp [10] = pulses_11;
  temp [11] = pulses_12;
  temp [12] = pulses_13;
  temp [13] = pulses_14;
  temp [14] = pulses_WM;

  //pulses_1 = 10;
  //if ((last_time + deltaTime < millis()) && ((pulses_WM > 0) || (pulses_1 > 0))) {

  //deltaVolume = temp[i] * (1.0/485.0) * (0.264172);
  //flowrate = deltaVolume * 60.0 / (deltaTime/1000.0);
  //Serial.print("flowrate = ");
  //Serial.println(flowrate,4);
  //Serial.println(pulse_temp,4);
  //dtostrf(flowrate,6,4,flow);
  if ((millis() % 500 == 0) && ((pulses_WM > 0) || (pulses_1 > 0) || (pulses_2 > 0) || (pulses_3 > 0) || (pulses_4 > 0) || (pulses_5 > 0) || (pulses_6 > 0) || (pulses_7 > 0) || (pulses_8 > 0) || (pulses_9 > 0) || (pulses_10 > 0) || (pulses_11 > 0) || (pulses_12 > 0) || (pulses_13 > 0) || (pulses_14 > 0))) {

    if (client.connect(server, 80)) {
      Serial.print("Connected to ");
      Serial.println(server);
      client.print("GET /Small_water_sensor.php?");
      //for (i = 0; i < 14; i++) {
      //ID[i] = i + 1;
      //Serial.print("ID:");
      //Serial.println(ID[i]);
      //client.print("ID=");
      //client.print(ID[i]);
      //client.print("&pulses=");
      //client.print(temp[i]);
      //Serial.print("Pulses:");
      //Serial.println(temp[i]);
      //client.print("&notes=");
      //client.print("Small");
      //Serial.print("ID:");
      //Serial.println(3);
      // client.print("ID=");
      //client.print(1);
      /*
        client.print("ID[]=");
        client.print(ID[0]);
        client.print("&ID[]=");
        client.print(ID[1]);
        client.print("&ID[]=");
        client.print(ID[2]);
        client.print("&ID[]=");
        client.print(ID[3]);
        client.print("&ID[]=");
        client.print(ID[4]);
        client.print("&ID[]=");
        client.print(ID[5]);
        client.print("&ID[]=");
        client.print(ID[6]);
        client.print("&ID[]=");
        client.print(ID[7]);
        client.print("&ID[]=");
        client.print(ID[8]);
        client.print("&ID[]=");
        client.print(ID[9]);
        client.print("&ID[]=");
        client.print(ID[10]);
        client.print("&ID[]=");
        client.print(ID[11]);
        client.print("&ID[]=");
        client.print(ID[12]);
        client.print("&ID[]=");
        client.print(ID[13]);
        client.print("&ID[]=");
        client.print(ID[14]);
      */
      client.print("&pulses[]=");
      client.print(temp[0]);
      client.print("&pulses[]=");
      client.print(temp[1]);
      client.print("&pulses[]=");
      client.print(temp[2]);
      client.print("&pulses[]=");
      client.print(temp[3]);
      client.print("&pulses[]=");
      client.print(temp[4]);
      client.print("&pulses[]=");
      client.print(temp[5]);
      client.print("&pulses[]=");
      client.print(temp[6]);
      client.print("&pulses[]=");
      client.print(temp[7]);
      client.print("&pulses[]=");
      client.print(temp[8]);
      client.print("&pulses[]=");
      client.print(temp[9]);
      client.print("&pulses[]=");
      client.print(temp[10]);
      client.print("&pulses[]=");
      client.print(temp[11]);
      client.print("&pulses[]=");
      client.print(temp[12]);
      client.print("&pulses[]=");
      client.print(temp[13]);
      client.print("&pulses[]=");
      client.print(temp[14]);

      Serial.print("ID: ");
      Serial.println(ID[0]);
      Serial.print("Pulses: ");
      Serial.println(temp[0]);
      Serial.print("ID: ");
      Serial.println(ID[1]);
      Serial.print("Pulses: ");
      Serial.println(temp[1]);
      Serial.print("ID: ");
      Serial.println(ID[2]);
      Serial.print("Pulses: ");
      Serial.println(temp[2]);
      Serial.print("ID: ");
      Serial.println(ID[3]);
      Serial.print("Pulses: ");
      Serial.println(temp[3]);
      Serial.print("ID: ");
      Serial.println(ID[4]);
      Serial.print("Pulses: ");
      Serial.println(temp[4]);
      Serial.print("ID: ");
      Serial.println(ID[5]);
      Serial.print("Pulses: ");
      Serial.println(temp[5]);
      Serial.print("ID: ");
      Serial.println(ID[6]);
      Serial.print("Pulses: ");
      Serial.println(temp[6]);
      Serial.print("ID: ");
      Serial.println(ID[7]);
      Serial.print("Pulses: ");
      Serial.println(temp[7]);
      Serial.print("ID: ");
      Serial.println(ID[8]);
      Serial.print("Pulses: ");
      Serial.println(temp[8]);
      Serial.print("ID: ");
      Serial.println(ID[9]);
      Serial.print("Pulses: ");
      Serial.println(temp[9]);
      Serial.print("ID: ");
      Serial.println(ID[10]);
      Serial.print("Pulses: ");
      Serial.println(temp[10]);
      Serial.print("ID: ");
      Serial.println(ID[11]);
      Serial.print("Pulses: ");
      Serial.println(temp[11]);
      Serial.print("ID: ");
      Serial.println(ID[12]);
      Serial.print("Pulses: ");
      Serial.println(temp[12]);
      Serial.print("ID: ");
      Serial.println(ID[13]);
      Serial.print("Pulses: ");
      Serial.println(temp[13]);
      Serial.print("ID: ");
      Serial.println(ID[14]);
      Serial.print("Pulses: ");
      Serial.println(temp[14]);


      client.println(" HTTP/1.1");  // Part of GET request
      client.print("Host:");
      client.println(server);
      client.println("Connection: close");  // Part of the GET request telling the server that we are over transmitting the message
      client.println(); // Empty Line
      client.println(); // Empty Line
      client.stop();    // Closing connection to server
      Serial.println("Disonnected");
      pulses_1 = 0;
      pulses_2 = 0;
      pulses_3 = 0;
      pulses_4 = 0;
      pulses_5 = 0;
      pulses_6 = 0;
      pulses_7 = 0;
      pulses_8 = 0;
      pulses_9 = 0;
      pulses_10 = 0;
      pulses_11 = 0;
      pulses_12 = 0;
      pulses_13 = 0;
      pulses_14 = 0;
      pulses_WM = 0;
      //last_time = millis();

    }
    else {
      Serial.println("--> connection failed\n");
    }
    //delay(500);
  }


  last_state_1 = check_state_1;
  last_state_2 = check_state_2;
  last_state_3 = check_state_3;
  last_state_4 = check_state_4;
  last_state_5 = check_state_5;
  last_state_6 = check_state_6;
  last_state_7 = check_state_7;
  last_state_8 = check_state_8;
  last_state_9 = check_state_9;
  last_state_10 = check_state_10;
  last_state_11 = check_state_11;
  last_state_12 = check_state_12;
  last_state_13 = check_state_13;
  last_state_14 = check_state_14;
  last_state_WM = check_state_WM;

  //if ((pulses_WM == 0) && (pulses_1 == 0) && (pulses_2 == 0) && (pulses_2 == 0) && (pulses_3 == 0) && (pulses_4 == 0) && (pulses_5 == 0) && (pulses_6 == 0) && (pulses_7 == 0) && (pulses_8 == 0) && (pulses_9 == 0) && (pulses_10 == 0) && (pulses_11 == 0) && (pulses_12 == 0) && (pulses_13 == 0)) {
    //delay(2000);

    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h1 = dht1.readHumidity();
    //delay(1000);
   // h2 = dht2.readHumidity();
    //delay(1000);
  //  h3 = dht3.readHumidity();
   // delay(1000);
   // h4 = dht4.readHumidity();
    //delay(1000);
   // h5 = dht5.readHumidity();
    //delay(1000);
  //  h6 = dht6.readHumidity();
    //delay(1000);
  //  h7 = dht7.readHumidity();
   //delay(1000);
 //   h8 = dht8.readHumidity();
/*
    // Read temperature as Celsius (the default)
    t1 = dht1.readTemperature();
    delay(1000);
    t2 = dht2.readTemperature();
    delay(1000);
    t3 = dht3.readTemperature();
    delay(1000);
    t4 = dht4.readTemperature();
    delay(2000);
    t5 = dht5.readTemperature();
    delay(2000);
    t6 = dht6.readTemperature();
    delay(2000);
    t7 = dht7.readTemperature();
    delay(2000);
    t8 = dht8.readTemperature();
*/
    // Read temperature as Fahrenheit (isFahrenheit = true)
   // delay(1000);
    f1 = dht1.readTemperature(true);
    //delay(1000);
    //f2 = dht2.readTemperature(true);
   // delay(1000);
    //f3 = dht3.readTemperature(true);
    //delay(1000);
  //  f4 = dht4.readTemperature(true);
    //delay(1000);
   // f5 = dht5.readTemperature(true);
    //delay(1000);
  //  f6 = dht6.readTemperature(true);
   // delay(1000);
  //  f7 = dht7.readTemperature(true);
  // delay(1000);
  //  f8 = dht8.readTemperature(true);
  
        // Check if any reads failed and exit early (to try again).
        if (isnan(h1) || isnan(t1) || isnan(f1)) {
          Serial.println("Failed to read from DHT sensor1!");
          //return;
        }
       /* if (isnan(h2) || isnan(t2) || isnan(f2)) {
          Serial.println("Failed to read from DHT sensor2!");
          //return;
        }
        if (isnan(h3) || isnan(t3) || isnan(f3)) {
          Serial.println("Failed to read from DHT sensor3!");
         // return;
        }
        if (isnan(h4) || isnan(t4) || isnan(f4)) {
          Serial.println("Failed to read from DHT sensor4!");
          //return;
        }
        if (isnan(h5) || isnan(t5) || isnan(f5)) {
          Serial.println("Failed to read from DHT sensor5!");
          //return;
        }
        if (isnan(h6) || isnan(t6) || isnan(f6)) {
          Serial.println("Failed to read from DHT sensor6!");
         // return;
        }
        if (isnan(h7) || isnan(t7) || isnan(f7)) {
          Serial.println("Failed to read from DHT sensor7!");
         // return;
        }
        if (isnan(h8) || isnan(t8) || isnan(f8)) {
          Serial.println("Failed to read from DHT sensor8!");
         // return;
        }*/
        
    if (abs(f1 - current_f1) > 0.01 || abs(h1 - current_h1) > 0.01) {
      if ((pulses_WM == 0) && (pulses_1 == 0) && (pulses_2 == 0) && (pulses_2 == 0) && (pulses_3 == 0) && (pulses_4 == 0) && (pulses_5 == 0) && (pulses_6 == 0) && (pulses_7 == 0) && (pulses_8 == 0) && (pulses_9 == 0) && (pulses_10 == 0) && (pulses_11 == 0) && (pulses_12 == 0) && (pulses_13 == 0)) {
        delay(delayMS);
        if (client.connect(server, 80)) {
          Serial.print("Connected to ");
          Serial.println(server);
          client.print("GET /dht.php?");
          //client.print("ID[]=");
          // client.print(ID[0]);
          client.print("temp[]=");
          client.print(f1);
          client.print("&temp[]=");
          client.print(f2);
          client.print("&temp[]=");
          client.print(f3);
          client.print("&temp[]=");
          client.print(f4);
          client.print("&temp[]=");
          client.print(f5);
          client.print("&temp[]=");
          client.print(f6);
          client.print("&temp[]=");
          client.print(f7);
          client.print("&temp[]=");
          client.print(f6);
          client.print("&hum[]=");
          client.print(h1);
          client.print("&hum[]=");
          client.print(h2);
          client.print("&hum[]=");
          client.print(h3);
          client.print("&hum[]=");
          client.print(h4);
          client.print("&hum[]=");
          client.print(h5);
          client.print("&hum[]=");
          client.print(h6);
          client.print("&hum[]=");
          client.print(h7);
          client.print("&hum[]=");
          client.print(h8);
          //client.print("&notes=");
         // client.print("test");
          client.println(" HTTP/1.1");  // Part of GET request
          client.print("Host:");
          client.println(server);
          client.println("Connection: close");  // Part of the GET request telling the server that we are over transmitting the message
          client.println(); // Empty Line
          client.println(); // Empty Line
          client.stop();    // Closing connection to server
          Serial.print("temp:");
          Serial.println(f1);
          Serial.print("hum:");
          Serial.println(h1);
          Serial.print("temp:");
          Serial.println(f2);
          Serial.print("hum:");
          Serial.println(h2);
          Serial.println("Disonnected");
          //current_t1 = t1;
          current_h1 = h1;
          current_f1 = f1;
        }
      }
      else {
        Serial.println("--> connection failed\n");
      }
    //}

  }

}

