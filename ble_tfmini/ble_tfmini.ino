byte tfmini_data[7];
int16_t checksum;
int height_cm, raw_height_cm;
int a1, a2;

unsigned long loop_time;

void setup() {

  Serial.begin(9600);
  Serial1.begin(115200);

  loop_time = micros();
}

void loop() {

  if(Serial1.available() && Serial1.read() == 0x59 && Serial1.read() == 0x59){
    
    //Serial1.readBytes(tfmini_data, 7);
    a1 = Serial1.read();
    a2 = Serial1.read();
    //checksum = tfmini_data[0] + tfmini_data[1] + tfmini_data[2] + tfmini_data[3] + tfmini_data[4] + tfmini_data[5] + tfmini_data[6];
    //if(tfmini_data[7] == (checksum & 0xff)) height_cm = (int)tfmini_data[1] * 256 + (int)tfmini_data[0]; 
    raw_height_cm = (int)a2 * 256 + (int)a1;
    if(raw_height_cm > 0 && raw_height_cm < 1000) height_cm = raw_height_cm;
    
  }

  Serial.print(height_cm);Serial.print("\t");
  Serial.println(micros() - loop_time);
  loop_time = micros();

  
  //if(height_cm > 10) digitalWrite(13, HIGH);
  //else digitalWrite(13, LOW);

}
