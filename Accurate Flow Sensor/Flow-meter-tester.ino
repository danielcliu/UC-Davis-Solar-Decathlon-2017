// Pin reading sensor
#define FLOWSENSORPIN 2

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


void setup()
{
  Serial.begin(9600);
  Serial.print("Flow Sensor Test");
  delay(1000);
}

void loop()
{
  check_state = digitalRead(FLOWSENSORPIN);
  if ((check_state == LOW) && (last_state == HIGH)) {
    pulses++;
  }
  if (last_time + deltaTime < millis()){
      deltaVolume = pulses * (1.0/485.0) * (0.264172);
      flowrate = deltaVolume * 60.0 / (deltaTime/1000.0);
//     Serial.println("pulses = ");
//      Serial.println(pulses);
//       Serial.println("vol = ");
//      Serial.println(deltaVolume);
      Serial.print("flowrate = ");
      Serial.println(flowrate,4);
      pulses = 0;
      last_time = millis();
    }
  last_state  = check_state;
}

