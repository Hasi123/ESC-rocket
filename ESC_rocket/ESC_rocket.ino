/*ESC-rocket
This code basically turns your ESC into a switch
The output is PWM controlled to limit the current*/

//define your pins:
#define AL 5 //AnFET
#define BL A4 //BnFET
#define CL 8 //CnFET
#define AH 4 //ApFET
#define BH A5 //BpFET
#define CH A3 //CpFET
#define RCIN 2 //rcp_in

//configure these too:
#define nFET 1 //high side nFet?
byte pwm_cycle = 45; //the duty cycle: between 0 (always off) and 255 (always on).

unsigned int duration = 0;
byte pin[] = {AL, BL, CL, AH, BH, CH};
byte pulse = B00000000;

void setup() {
  //init timer
  TCCR2 = 0;// set entire TCCR2 register to 0
  TCNT2  = 0;//initialize counter value to 0
  OCR2 = pwm_cycle;
  // turn on CTC mode
  TCCR2 |= (1 << WGM21);
  // Set bits for 32 prescaler
  TCCR2 |= (1 << CS21) | (1 << CS20);
  // enable timer compare interrupt
  TIMSK |= (1 << OCIE2);
  sei();

  //init pins
  for (byte i = 0; i < sizeof(pin); i++) {
    pinMode(pin[i], OUTPUT);
  }
  digitalWrite(AL, LOW);
  digitalWrite(BL, LOW);
  digitalWrite(CL, LOW);
  digitalWrite(AH, nFET); //because of nFet this is off
  digitalWrite(BH, nFET);
  digitalWrite(CH, nFET);

  pinMode(RCIN, INPUT);

  //wait for first valid pulse
  while(1){
    duration = pulseInLong(RCIN, HIGH, 60000);
    if(duration >= 950 && duration < 1550){ //valid and not active?
      break;
    }
  }
}

void loop() {
  duration = pulseInLong(RCIN, HIGH, 60000);


  if (duration >= 1600 && duration < 1800) { //turn on one side
    //digitalWrite(AL, HIGH);
    pulse = B00000001;
    digitalWrite(BL, LOW);
    digitalWrite(CL, LOW);
    digitalWrite(AH, nFET);
    digitalWrite(BH, !nFET);
    digitalWrite(CH, nFET);
  }
  else if (duration >= 1800 && duration <= 2200) { //other side
    digitalWrite(AL, LOW);
    digitalWrite(BL, LOW);
    //digitalWrite(CL, HIGH);
    pulse = B00000100;
    digitalWrite(AH, nFET);
    digitalWrite(BH, !nFET);
    digitalWrite(CH, nFET);
  }
  else { //off
    pulse = B00000000;
    digitalWrite(AL, LOW);
    digitalWrite(BL, LOW);
    digitalWrite(CL, LOW);
    digitalWrite(AH, nFET);
    digitalWrite(BH, nFET);
    digitalWrite(CH, nFET);
  }
}

ISR(TIMER2_COMP_vect) {
  static byte state = 0;

  TCNT2  = 0;
  if (state) {
    state = 0;
    OCR2 = 255 - pwm_cycle;
  }
  else {
    state = 1;
    OCR2 = pwm_cycle;
  }

  for (byte i = 0; i < sizeof(pin); i++) {
    if (bitRead(pulse, i)) {
      digitalWrite(pin[i], state);
    }
  }
}

