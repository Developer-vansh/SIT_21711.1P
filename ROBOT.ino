#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
String pass="";
String otp="";

#define lock 4
#define LM1 A0//(brown) //+ve(out1)
#define LM2 A1//(red)
#define RM1 A2//(orange)  //+ve(out4)
#define RM2 A3//(yellow)

#define leftspeedcontrol 9 //brown
#define rightspeedcontrol 10//(yellow)

#define forwardspeed 200                                                                       


//HC-SR04 Sensor connection
const int trigPin = 2;
const int echoPin = 3;

int sensor1=0;
int sensor2=0;
int sensor3=0;
int sensor4=0;
int sensor5=0;
char mode='0';
int sel=0;
char command;
void setup() 
{
  // put your setup code here, to run once:
  pinMode(LM1,OUTPUT);
  pinMode(LM2,OUTPUT);
  pinMode(RM1,OUTPUT);
  pinMode(RM2,OUTPUT);
pinMode (11, INPUT);
pinMode (12, INPUT);
pinMode (8, INPUT);
pinMode (7, INPUT);
pinMode (6, INPUT);

pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
  
  pinMode(leftspeedcontrol,OUTPUT);
  pinMode(rightspeedcontrol,OUTPUT);
  Serial.begin(9600);
   lcd.init();
  lcd.clear();         
  lcd.backlight();
    lcd.setCursor(0,0);
  lcd.print(" SECURE DELIVERY");
delay(1500);
   lcd.init();
  lcd.clear(); 
  pinMode(lock,OUTPUT);
  digitalWrite(lock,LOW);
  selection();

   analogWrite(leftspeedcontrol,255);
analogWrite(rightspeedcontrol,255);
}
void loop() {
    while(sel==0){
  if(Serial.available()>0)
  {
     mode=Serial.read();
      Serial.println(mode);
  }
  if(mode=='1' ){
     lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MOBILE RC");
  sel=1;
  }
  if(mode=='2'){
     lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("AUTONOMUS");
  sel=1;
  }
  }
    if(mode=='1')rccar();
    else if(mode=='2')  linefollow();

}
void rccar(){
  analogWrite(leftspeedcontrol,255);
analogWrite(rightspeedcontrol,255);
  if (Serial.available() > 0) {
   command = Serial.read();
    Serial.println(command);
    stop(); //Initialize with motors stoped.
    switch (command) {
      case 'F':
        forward();
        break;
      case 'B':
       back();
        break;
      case 'R':
       left();
        break;
      case 'L':
      right(); 
        break;
      case'S':
        stop();
        break;
}
}
}
void selection(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("SELECT");
delay(1500);
 lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-FOR MOBILE RC");
  lcd.setCursor(0,1);
  lcd.print("2-FOR AUTONOMUS");
  
}
void linefollow(){
  analogWrite(leftspeedcontrol,forwardspeed);
analogWrite(rightspeedcontrol,forwardspeed);
 sensor1= digitalRead(11);
sensor2= digitalRead(12);
sensor3= digitalRead(8);
sensor4= digitalRead(7);
sensor5= digitalRead(6);
//objectAvoid();
if(sensor1 == LOW && sensor2 == LOW && sensor3 == HIGH && sensor4 == LOW && sensor5 == LOW)
{
 Serial.println("MID sensor");
  // lcd.clear();
  //   lcd.setCursor(4,0);
  //   lcd.print("F");
forward();
}
//left tyre
else if((sensor1 == HIGH || sensor2 == HIGH) && (sensor3 == LOW && sensor4 == LOW && sensor5 == LOW))
{
    Serial.println("right sensor");
    //     lcd.clear();
    // lcd.setCursor(4,0);
    // lcd.print("R");
left();
 analogWrite(leftspeedcontrol,250);
analogWrite(rightspeedcontrol,200);
delay(800);
}
//sharp left tyre
else if((sensor1 == HIGH && sensor2 == HIGH) && (sensor3 == LOW && sensor4 == LOW && sensor5 == LOW)){
    Serial.println("both right sensor");
    //     lcd.clear();
    // lcd.setCursor(4,0);
    // lcd.print("SR");
left();
 analogWrite(leftspeedcontrol,250);
analogWrite(rightspeedcontrol,200);
while(digitalRead(8)==LOW){}
}
//right tyre
else if((sensor1 == LOW && sensor2 == LOW && sensor3 == LOW) && (sensor4 == HIGH || sensor5 == HIGH))
{
    Serial.println("left sensor");
    //  lcd.clear();
    // lcd.setCursor(4,0);
    // lcd.print("L");
right();
 analogWrite(leftspeedcontrol,200);
analogWrite(rightspeedcontrol,250);
delay(800);
}
//sharp right tyre
else if((sensor1 == LOW && sensor2 == LOW && sensor3 == LOW) && (sensor4 == HIGH && sensor5 == HIGH)){
   Serial.println("both left sensor");
    //    lcd.clear();
    // lcd.setCursor(4,0);
    // lcd.print("SL");
right();
 analogWrite(leftspeedcontrol,200);
analogWrite(rightspeedcontrol,250);
while(digitalRead(8)==LOW){}
}

//stop
else if((sensor1 == LOW && sensor2 == LOW && sensor3 == LOW && sensor4 == LOW && sensor5 == LOW)||(sensor1 == HIGH && sensor2 == HIGH && sensor3 == HIGH && sensor4 == HIGH && sensor5 == HIGH))
{
    //Serial.println("stop");
stop();
security();
}
else forward();
}
void stop(){
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
}
void forward()
{
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
}
void back()
{
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);

}
void left()
{   digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);

}
void right()
{
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
}
void objectAvoid() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  //Serial.print("Distance: ");
  //Serial.println(distance);

  if (distance <= 15) {
    // stop
     Serial.println("S");
    stop();
    objectAvoid();
  
  }
}
  
void security() {
  if(pass==""){
 // Serial.println("Set password");
  lcd.setCursor(0, 0);
  lcd.print("SET ORDER-ID");
  int i=0;
  char input[10];

  if(Serial.available()>0)
  {
     delay(100);
     while( Serial.available() && i< 9) {
        input[i++] = Serial.read();
     }
    input[i++]='\0';
     pass=input;
     //Serial.println("System LOck");
     lcd.clear();
      lcd.setCursor(0, 0);
     lcd.print("PARCEL LOCKED");
     digitalWrite(lock,HIGH);
     delay(2000);
     lcd.clear();
  }
  }
  if(pass!=""){
      //Serial.println("Enter OTP");
  lcd.setCursor(0, 0);
  lcd.print("ENTER ORDER-ID");
      int i=0;
      char input[10];

  if(Serial.available()>0)
  {
     delay(100);
     while( Serial.available() && i< 9) {
        input[i++] = Serial.read();
     }
     input[i++]='\0';
     otp= input;
     if(otp==pass){
    lcd.clear();
     lcd.setCursor(0, 0);
    lcd.print("PARCEL UNLOCKED");
    digitalWrite(lock,LOW);
     pass="";
     
     }
     else if(otp!=pass){//Serial.println("Wrong otp, Try again");
         lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("UNVALID");
    lcd.setCursor(3,1);
    lcd.print("TRY AGAIN:)");
       i=0;
     }
     delay(2000);
     lcd.clear();
  }
  }
}