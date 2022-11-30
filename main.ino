//connect the HT R phase in analog A0
//connect the HT Y phase in analog A1
//connect the HT B phase in analog A2
//connect the LT R phase in alalog A3
//connect the LT Y phase in analog A4
//conntct the LT B phase in analog A5
//connect the HT Test phase in analog A6
//connect the ralay pin 13
//connect the trigger pin in 14
//connect the echo pin in 15
//connect the gas sensor D0 to A7
//connect the temperature sensor in A15
//TX to 4 & RX to 3

//temparature sensor value
const int tempsensor=A15; // Assigning analog pin A1 to variable 'sensor'
float tempc;  //variable to store temperature in degree Celsius
float vout;  //temporary variable to hold sensor reading

//gas sensor value initiation
  float sensorVoltage =0; 
  float sensorValue =0 ;

//values for sensor HT R

int sensorPin1 = A0;      
int ct1[30]={};  
float sum1=0;
float avr1=0;

//value for sensor HT Y
int sensorPin2 = A1;   
int ct2[30]={};  
float sum2=0;
float avr2=0;

//values for sensor HT B

int sensorPin3 = A2;   
int ct3[30]={};  
float sum3=0;
float avr3=0;

//values for sensor LT R

int sensorPin4 = A3;   
int ct4[30]={};  
float sum4=0;
float avr4=0;

//values for sensor LT Y

int sensorPin5 = A4;   
int ct5[30]={};  
float sum5=0;
float avr5=0;

//values for sensor HT B

int sensorPin6 = A5;   
int ct6[30]={};  
float sum6=0;
float avr6=0;

//values for sensor HT TEST

int sensorPin7 = A6;   
int ct7[30]={};  
float sum7=0;
float avr7=0;


//common value for all
int iter1 = 0;
int sensorValue1,sensorValue2,sensorValue3,sensorValue4,sensorValue5,sensorValue6,sensorValue7 = 0; 
int iter2=0;

//ralay pin
const int relaypin =  13;     //connect the ralay in pin 13

//initiating PIN for ultrsonic senfor
const int trigPin = 14;
const int echoPin = 15;
long duration=0;
int distance=0;


// initilaisation of GSM 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4,3);//TX to 4 & RX to 3



void setup() {
  pinMode(tempsensor,INPUT); // Configuring pin A15 as input
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(relaypin, OUTPUT);
  Serial.begin(19200);
}

void loop() {
//reading temparature value
vout=analogRead(tempsensor);
vout=(vout*500)/1023;

  
  digitalWrite(relaypin, HIGH);// inisiating the relay high 
//reading the analog values from sensors block
  
  sensorValue1 = analogRead(sensorPin1);  //reading the value for ct1
  sensorValue2 = analogRead(sensorPin2);  //reading the value for ct2
  sensorValue3 = analogRead(sensorPin3);  //reading the value for ct3
  sensorValue4 = analogRead(sensorPin4);  //reading the value for ct4 
  sensorValue5 = analogRead(sensorPin5);  //reading the value for ct5 
  sensorValue6 = analogRead(sensorPin6);  //reading the value for ct6
  sensorValue7 = analogRead(sensorPin7);  //reading the value for ct7
  if(iter1==30){
    iter1=0;
    for (iter2 = 0; iter2 <30; iter2 += 1) {
      sum1=ct1[iter2]+sum1  ;
      sum2=ct2[iter2]+sum2  ; 
      sum3=ct3[iter2]+sum3  ;
      sum4=ct4[iter2]+sum4  ;
      sum5=ct5[iter2]+sum5  ;
      sum6=ct6[iter2]+sum6  ;
      sum7=ct7[iter2]+sum7  ;                 
    }

  //temperature sensor value printing
  Serial.print("in DegreeC=");
Serial.print(vout);
Serial.print("\n");

  //oil lebel sensing value reading
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
 
  Serial.print("The length is:");
  Serial.print(distance);
  Serial.println("cm");

//gas sensing
  sensorValue = analogRead(A7);
  sensorVoltage = sensorValue/1024*5.0;
 
  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");


  //making ct average
  avr1=sum1/30;
  avr2=sum2/30;
  avr3=sum3/30;
  avr4=sum4/30;
  avr5=sum5/30;
  avr6=sum6/30;
  avr7=sum7/30;
  Serial.print("average of HT R is:");
  Serial.println(avr1);
  Serial.print("average of HT Y is:");
  Serial.println(avr2);
  Serial.print("average of HT B is:");    
  Serial.println(avr3);
  Serial.print("average of LT R is:");
  Serial.println(avr4);
  Serial.print("average of LT Y is:");
  Serial.println(avr5);
  Serial.print("average of LT B is:");
  Serial.println(avr6);
  Serial.print("average of HT test value is:");
  Serial.println(avr7);
  sum1=0;
  sum2=0;
  sum3=0;
  sum4=0;
  sum5=0;
  sum6=0;
  sum7=0;

/* ----------------------logic session for all programs--------------------------------- */


//logic  te check all HT 3-phases

//checking the three phases
bool htphase=true;
if (avr1<=5 && avr2<=5 && avr3<=5){
  htphase=false; 
  }
if (htphase==false && avr7>5){
  //sending massage 3-HT phase is gone
  SendMessage();
  }
//checking single line phases
if (avr1>5 && avr2>5 && avr3<5){
  //sendging massage B phase is gone
 SendMessage();
  }

if (avr1>5 && avr2<5 && avr3>5){
  //sendging massage y phase is gone
 SendMessage();
  }


if (avr1<5 && avr2>5 && avr3>5){
  //sendging massage R phase is gone
 SendMessage();
  }
if (avr1<5 && avr2<5 && avr3>5){
  //sendging massage R and Y phase is gone
 SendMessage();
  }
if (avr1<5 && avr2>5 && avr3<5){
  //sending R and B phase is gone
 SendMessage();
  }
if (avr1>5  &&  avr2<5    && avr3<5  ){
  //sending Y and B phase fault
 SendMessage();
}


//............................................ this is to check Lt fuses faults.............................................................
//.............................................lt fuse 3-phase fault................. 
bool ltphase = true;
if (avr4<=5 && avr5<=5 && avr6<=5){
 ltphase=false; 

}
if ( ltphase == false && htphase==true){
  //send the message 3lt fuses are gone 
SendMessage();
}

//checking single line phases
if (avr4>5 && avr5>5 && avr6<5){
  //sendging massage B lt  phase is gone
  SendMessage();
  }

if (avr4>5 && avr5<5 && avr6>5){
  //sendging massage y lt phase is gone
  SendMessage();
  }


if (avr4<5 && avr5>5 && avr6>5){
  //sendging massage R lt  phase is gone
  SendMessage();
  }
if (avr4<5 && avr5<5 && avr6>5){
  //sendging massage R and Y lt  phase is gone
  SendMessage();
  }
if (avr4<5 && avr5>5 && avr6<5){
  //sending R and B lt phase is gone
   SendMessage();

  }
if (avr4>5  &&  avr5<5    && avr6<5  ){
  //sending Y and B lt phase fault
  SendMessage();
}


/*...............code for relay control.........................*/

if(vout>70 || sensorVoltage>20  ){//vout for temperature and sensorVoltage is for GAS
 digitalWrite(relaypin, LOW);
 if (vout>70){
 SendMessage();
  }

  if (sensorVoltage>20){
 SendMessage();
 Serial.print("GAS is hugher and T/F(1234) is shut down");
  }
}




  }

  delay(30);
  ct1[iter1]=sensorValue1;
  ct2[iter1]=sensorValue2;
  ct3[iter1]=sensorValue3;
  ct4[iter1]=sensorValue4;
  ct5[iter1]=sensorValue5;
  ct6[iter1]=sensorValue6;
  ct7[iter1]=sensorValue7;
  iter1++;

    
}

 void SendMessage()
{ Serial.println("Hey this is calling for massage");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918790658857\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("hi there i am from GSM");// The SMS text you want to send
  delay(1000);
   mySerial.println((char)100);// ASCII code of CTRL+Z
  delay(3000);
  
}

