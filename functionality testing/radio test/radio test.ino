//// Settings ////
// baud in bits per second
const int baudrate     = 9600;
// The number of samples per wave
const int resolution   = 100;

//// Constants ////
const float twopi      = 6.28318;
// Length of one bit in seconds
const int period       = 1000000/baudrate;
// time period per value change
const int twitch       = period/resolution;
// angle changle per value change
const float increment  = twopi/resolution;

// Tables of sine values for both waves
int sine1[resolution];
int sine2[resolution];

byte callsign[6] = { 0x4B, 0x4D, 0x34, 0x52, 0x45, 0x45 };

void setup() {
  generate();
  analogWriteResolution(12);
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(24, OUTPUT);
  pinMode(A21, OUTPUT);
  digitalWrite(24, HIGH);
  delay(1000);
  Serial3.write("AT+DMOCONNECT\r\n");
  delay(1000);
  Serial3.write("AT+DMOSETGROUP=0,433.0000,433.0000,0000,0,0012\r\n");
}

void loop() {
//  if(Serial.available()){
//    byte tmp = Serial.read();
//    Serial3.write(tmp);
//    Serial.write(tmp);
//  }
//  if(Serial3.available()){
//    Serial.write(Serial3.read());
//  }
  digitalWrite(24, LOW);
  Serial.write("on");
  for(int i=0; i<9600; i++){
    transmit(callsign, 6);
    delay(1);
  }
  delay(1000);
  digitalWrite(24, HIGH);
  Serial.write("off");
  delay(1000);
}

void transmit(byte* buffer, int len) {
  for (int i=0; i<len; i++){
    for (byte mask = 1; mask>0; mask <<= 1){   //iterate through bit mask
      if (buffer[i] & mask){              // if bitwise AND resolves to true
        bit1();                           // send 1
      }
      else{                               //if bitwise AND resolves to false
        bit0();                           // send 0
      }
    }
  }
}

void bit0() {
  // Write the sampled wave for bit 0 to the DAC
  for(int i=0; i < resolution; i++){
    analogWrite(A21, sine1[i]);
    delayMicroseconds(twitch);
  };
}

void bit1() {
  // Write the sampled wave for bit 1 to the DAC
  for(int i=0; i < resolution; i++){
    analogWrite(A21, sine2[i]);
    delayMicroseconds(twitch);
  };
}

// Generates the table of sine values
void generate(){
  float phase = 0.0;
  for(int i=0; i < resolution; i++){
    float val = sin(phase) * 2000.0 + 2050.0;
    sine1[i] = (int)val;
    phase = phase + increment;
  };
  phase = 0.0;
  for(int i=0; i < resolution; i++){
    float val = sin(1.2*phase) * 2000.0 + 2050.0;
    sine2[i] = (int)val;
    phase = phase + increment;
  };
}
