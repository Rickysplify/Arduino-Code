#include <Servo.h>

unsigned long pulse_duration3;
unsigned long pulse_duration2;
unsigned long pulse_duration0;
unsigned long pulse_duration4;

volatile unsigned long pulseInTimeBegin2 = micros();
volatile unsigned long pulseInTimeEnd2 = micros();
volatile bool newPulseDurationAvailable2 = false;

volatile unsigned long pulseInTimeBegin3 = micros();
volatile unsigned long pulseInTimeEnd3 = micros();
volatile bool newPulseDurationAvailable3 = false;

int motorspeed1=0;
int motorspeed2=0;

#define enA 6
#define enB 5
#define in1 9
#define in2 10
#define in3 7
#define in4 8

bool isAttached=true;

Servo myservo1;
Servo myservo2;

void buttonPinInterrupt2()
{
  if (digitalRead(2) == HIGH) {
    // start measuring
    pulseInTimeBegin2 = micros();
  }
  else {
    // stop measuring
    pulseInTimeEnd2 = micros();
    newPulseDurationAvailable2 = true;
  }
}

void buttonPinInterrupt3()
{
  if (digitalRead(3) == HIGH) {
    // start measuring
    pulseInTimeBegin3 = micros();
  }
  else {
    // stop measuring
    pulseInTimeEnd3 = micros();
    newPulseDurationAvailable3 = true;
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT); //ch1
  pinMode(0, INPUT); //ch5
  pinMode(3, INPUT); //ch3
  pinMode(4, INPUT); //ch7
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  myservo2.attach(11,1000,2000); //servo du kicker
  myservo1.attach(12,500,2400); //servo pour ramassage de balle
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2),buttonPinInterrupt2,CHANGE);

  attachInterrupt(digitalPinToInterrupt(3), buttonPinInterrupt3,CHANGE);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  pulse_duration0=pulseIn(0,HIGH);
  pulse_duration4=pulseIn(4,HIGH);

  if (newPulseDurationAvailable2) {
    newPulseDurationAvailable2 = false;
    pulse_duration2 = pulseInTimeEnd2 - pulseInTimeBegin2;
  }
  if (pulse_duration2 == 2000) {
    pulse_duration2 = 1996;
  }
  if (pulse_duration2==1008){
    pulse_duration2=1012;
  }


  if (newPulseDurationAvailable3) {
    newPulseDurationAvailable3 = false;
    pulse_duration3 = pulseInTimeEnd3 - pulseInTimeBegin3;
  }
  if (pulse_duration3 == 1008){
    pulse_duration3 = 1004;
  }
  if (pulse_duration3 == 2000){
    pulse_duration3 = 1996;
  }


  //motor forward
  if (pulse_duration3>1510) {
    //moteur 1 sens horaire
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    //moteur 2 sens horaire
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    motorspeed1=map(pulse_duration3,1510,1996,0,255);
    motorspeed2=map(pulse_duration3,1510,1996,0,255);
  }
  //motor backwards
  else if (pulse_duration3<1490){
    //moteur 1 sens anti-horaire
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    //moteur 2 sens anti-horaire
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    motorspeed1=map(pulse_duration3,1490,1004,0,255);
    motorspeed2=map(pulse_duration3,1490,1004,0,255);
  }
  //turning right
  else if (pulse_duration2>1520){
    //moteur 1 sens anti-horaire
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    //moteur 2 sens horaire
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    motorspeed1=map(pulse_duration2,1515,1996,0,255);
    motorspeed2=map(pulse_duration2,1515,1996,0,255);
  }
  //turning left
  else if (pulse_duration2<1500){
    //moteur 1 sens horaire
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    //moteur 2 sens anti-horaire
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    motorspeed1=map(pulse_duration2,1500,1012,0,255);
    motorspeed2=map(pulse_duration2,1500,1012,0,255);
  }
  else if ((pulse_duration2>1500 && pulse_duration2<1520) && (pulse_duration3>1490 && pulse_duration3<1510)){
    motorspeed1=0;
    motorspeed2=0;
  }

  analogWrite(enA,motorspeed1);
  analogWrite(enB,motorspeed2);

  
  if (pulse_duration0>1700 && isAttached==true){
    myservo2.write(95); //trouver angle optimal pour circuit obligatoire
    delay(20);
    myservo2.detach();    
    isAttached=false;
  }
  else if (pulse_duration0>1450 && pulse_duration0<1550){
    myservo2.attach(11,400,2500);
    myservo2.write(30);
    isAttached=true;
  }
  else if (pulse_duration0<1200){
    myservo2.attach(11,400,2500);
    myservo2.write(180); //trouver l'angle optimal pour le circuit bonus
    isAttached=true;
  }
  

  if (pulse_duration4>1600){
    myservo1.write(25); //trouver angle pour ouvrir
  }
  else if (pulse_duration4>1450 && pulse_duration4<1500){
    myservo1.write(50);
  }
  else if (pulse_duration4<1200){
    myservo1.write(65); //trouver l'angle pour fermer
  }

}
