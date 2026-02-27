#define MOTOR1_PIN 8
#define MOTOR2_PIN 27
#define MOTOR3_PIN 2
#define MOTOR4_PIN 1

bool pin_r_last_value = 0, pin_p_last_value = 0, pin_t_last_value = 0, pin_y_last_value = 0;
int rc_roll_pwm, rc_pitch_pwm, rc_throttle_pwm, rc_yaw_pwm;
unsigned long r_c_time, p_c_time, t_c_time, y_c_time;


int esc_timer[] = {1000, 1000, 1000, 1000};
unsigned long c_time, loop_time;

void setup() {

  //Configure pins 2-5 as input
  NRF_P1->PIN_CNF[11] = 0x0000000C;
  NRF_P1->PIN_CNF[12] = 0x0000000C;
  NRF_P1->PIN_CNF[15] = 0x0000000C;
  NRF_P1->PIN_CNF[13] = 0x0000000C;

  //Configure pins 8-11 as output
  NRF_P1->PIN_CNF[MOTOR1_PIN] = 0x00000001;
  NRF_P0->PIN_CNF[MOTOR2_PIN] = 0x00000001;
  NRF_P1->PIN_CNF[MOTOR3_PIN] = 0x00000001;
  NRF_P1->PIN_CNF[MOTOR4_PIN] = 0x00000001;

  attachInterrupt(digitalPinToInterrupt(2), rc_roll, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), rc_pitch, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), rc_thr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(5), rc_yaw, CHANGE);

  c_time = micros();
  while(micros() - c_time < 5000000){
    
    NRF_P1->OUT |= (1 << MOTOR1_PIN);
    NRF_P0->OUT |= (1 << MOTOR2_PIN);
    NRF_P1->OUT |= (1 << MOTOR3_PIN);
    NRF_P1->OUT |= (1 << MOTOR4_PIN);
    
    delayMicroseconds(1000);
    
    NRF_P1->OUT &= ~(1 << MOTOR1_PIN);
    NRF_P0->OUT &= ~(1 << MOTOR2_PIN);
    NRF_P1->OUT &= ~(1 << MOTOR3_PIN);
    NRF_P1->OUT &= ~(1 << MOTOR4_PIN);

  }

  loop_time = micros();

}

void loop() {
  /*
  NRF_P1->OUT |= (1 << MOTOR1_PIN);
  NRF_P0->OUT |= (1 << MOTOR2_PIN);
  NRF_P1->OUT |= (1 << MOTOR3_PIN);
  NRF_P1->OUT |= (1 << MOTOR4_PIN);
  
  delayMicroseconds(rc_throttle_pwm);
  
  NRF_P1->OUT &= ~(1 << MOTOR1_PIN);
  NRF_P0->OUT &= ~(1 << MOTOR2_PIN);
  NRF_P1->OUT &= ~(1 << MOTOR3_PIN);
  NRF_P1->OUT &= ~(1 << MOTOR4_PIN);
  */

  c_time = micros();
  NRF_P1->OUT |= (1 << MOTOR1_PIN);
  NRF_P0->OUT |= (1 << MOTOR2_PIN);
  NRF_P1->OUT |= (1 << MOTOR3_PIN);
  NRF_P1->OUT |= (1 << MOTOR4_PIN);

  esc_timer[0] = c_time + rc_roll_pwm;
  esc_timer[1] = c_time + rc_pitch_pwm;
  esc_timer[2] = c_time + rc_throttle_pwm;
  esc_timer[3] = c_time + rc_yaw_pwm;

  while (NRF_P1->OUTSET & (1 << MOTOR1_PIN) | NRF_P0->OUTSET & (1 << MOTOR2_PIN) | NRF_P1->OUTSET & (1 << MOTOR3_PIN) | NRF_P1->OUTSET & (1 << MOTOR4_PIN)) {
    c_time = micros();
    if (esc_timer[0] <= c_time) NRF_P1->OUT &= ~(1 << MOTOR1_PIN);
    if (esc_timer[1] <= c_time) NRF_P0->OUT &= ~(1 << MOTOR2_PIN);
    if (esc_timer[2] <= c_time) NRF_P1->OUT &= ~(1 << MOTOR3_PIN);
    if (esc_timer[3] <= c_time) NRF_P1->OUT &= ~(1 << MOTOR4_PIN);
  }
  
  while(micros() - loop_time < 4000);
  loop_time = micros();

}

void rc_roll(){
  if(pin_r_last_value == 0 && (NRF_P1->IN & 0x00000800)){
    pin_r_last_value = 1;
    r_c_time = micros();
  }
  else if(pin_r_last_value == 1 && !(NRF_P1->IN & 0x00000800)){
    pin_r_last_value = 0;
    rc_roll_pwm = micros() - r_c_time;
  }
 
}

void rc_pitch(){
  if(pin_p_last_value == 0 && (NRF_P1->IN & 0x00001000)){
    pin_p_last_value = 1;
    p_c_time = micros();
  }
  else if(pin_p_last_value == 1 && !(NRF_P1->IN & 0x00001000)){
    pin_p_last_value = 0;
    rc_pitch_pwm = micros() - p_c_time;
  }
 
}

void rc_thr(){
  if(pin_t_last_value == 0 && (NRF_P1->IN & 0xFFFF8000)){
    pin_t_last_value = 1;
    t_c_time = micros();
  }
  else if(pin_t_last_value == 1 && !(NRF_P1->IN & 0xFFFF8000)){
    pin_t_last_value = 0;
    rc_throttle_pwm = micros() - t_c_time;
  }
 
}

void rc_yaw(){
  if(pin_y_last_value == 0 && (NRF_P1->IN & 0x00002000)){
    pin_y_last_value = 1;
    y_c_time = micros();
  }
  else if(pin_y_last_value == 1 && !(NRF_P1->IN & 0x00002000)){
    pin_y_last_value = 0;
    rc_yaw_pwm = micros() - y_c_time;
  }
 
}
