// Bluetooth 2.0 receive data
#include <Thread.h>
#include <ThreadController.h>
#define BAUD 9600

typedef struct{
  int pin;
  int state = LOW;
  long intensity = 0;
  unsigned long previousMillis = 0;
  boolean turnOn = false;
  Thread mThread = Thread();
}ledData;

int ledState = LOW;
ledData leds[16];
//ledData ledsActive[16];
ledData ledsActive[5];
char sz[191];
unsigned long previousMillis = 0;

String receivedMessage = "";
ThreadController controll = ThreadController();
float intensities[5] = {100, 200, 300, 400, 500};


void led2_callback(){
  blink_leds(2, leds[0].intensity, leds[0].state, 0);
}

void led3_callback(){
  blink_leds(3, leds[1].intensity, leds[1].state, 1);
}

void led4_callback(){
  blink_leds(4, leds[2].intensity, leds[2].state, 2);
}

void led5_callback(){
  blink_leds(5, leds[3].intensity, leds[3].state, 3);
}

void led6_callback(){
  blink_leds(6, leds[4].intensity, leds[4].state, 4);
}

void led7_callback(){
  blink_leds(7, leds[5].intensity, leds[5].state, 5);
}

void led8_callback(){
  blink_leds(8, leds[6].intensity, leds[6].state, 6);
}

void led9_callback(){
  blink_leds(9, leds[7].intensity, leds[7].state, 7);
}

void led10_callback(){
  blink_leds(10, leds[8].intensity, leds[8].state, 8);
}

void led11_callback(){
  blink_leds(11, leds[9].intensity, leds[9].state, 9);
}

void led12_callback(){
  blink_leds(12, leds[10].intensity, leds[10].state, 10);
}

void led13_callback(){
  blink_leds(13, intensities[11], ledsActive[11].state, 11);
}

void led14_callback(){
  blink_leds(14, leds[12].intensity, leds[12].state, 12);
}

void led15_callback(){
  blink_leds(15, leds[13].intensity, leds[13].state, 13);
}

void led16_callback(){
  blink_leds(16, leds[14].intensity, leds[14].state, 14);
}

void led17_callback(){
  blink_leds(17, leds[15].intensity, leds[15].state, 15);
}


void setup() {
    Serial.begin(BAUD);  
    init_leds();
    //turn_on_all();
      int j = 0;
  /*for(j=0; j < 5; j++){
          ledsActive[j].pin = j;
          ledsActive[j].intensity = intensities[j];
          ledsActive[j].mThread.enabled = true;
          switch(j){
            case 0: 
              ledsActive[j].mThread.onRun(led0_callback);
              ledsActive[j].mThread.setInterval(100);
              break;
            case 1: 
              ledsActive[j].mThread.onRun(led1_callback);
              ledsActive[j].mThread.setInterval(200);
              break;
            case 2: 
              ledsActive[j].mThread.onRun(led2_callback);
              ledsActive[j].mThread.setInterval(300);
              break;
            case 3: 
              ledsActive[j].mThread.onRun(led3_callback);
              ledsActive[j].mThread.setInterval(400);
              break;
            case 4: 
              ledsActive[j].mThread.onRun(led4_callback);
              ledsActive[j].mThread.setInterval(500);
              break;
           
           }
          controll.add(&(ledsActive[j].mThread));
    }*/
} 
// End setup()

void loop() {    
  String input;
  float intensity;
  
  //Check for data
  if(Serial.available() > 0){
    //turn_off_all();
    //read serial port until a \n character is received
    int i = 0;
    int j = 0;
    input = Serial.readStringUntil('\n');
   // Serial.println(input);

    char buf[sizeof(sz)];
    input.toCharArray(buf, sizeof(buf));
    
    char *p = buf;
    char *str;
    controll.clear();
    while ((str = strtok_r(p, ",", &p)) != NULL){
        leds[i].pin = i+2;
        intensity = atof(str);
        leds[i].intensity = (1 - intensity) * 100;
        //Serial.println(intensity);
        if(leds[i].intensity != 100){
          ledsActive[j].pin = leds[i].pin;
          ledsActive[j].intensity = leds[j].intensity;
          switch(ledsActive[j].pin){
            case 2: 
              ledsActive[j].mThread.onRun(led2_callback);
              break;
            case 3: 
              ledsActive[j].mThread.onRun(led3_callback);
              break;
            case 4: 
              ledsActive[j].mThread.onRun(led4_callback);
              break;
           case 5: 
              ledsActive[j].mThread.onRun(led5_callback);
              break;
            case 6: 
              ledsActive[j].mThread.onRun(led6_callback);
              break;
            case 7: 
              ledsActive[j].mThread.onRun(led7_callback);
              break;
            case 8: 
              ledsActive[j].mThread.onRun(led8_callback);
              break;
            case 9: 
              ledsActive[j].mThread.onRun(led9_callback);
              break;
            case 10: 
              ledsActive[j].mThread.onRun(led10_callback);
              break;
            case 11: 
              ledsActive[j].mThread.onRun(led11_callback);
              break;
            case 12: 
              ledsActive[j].mThread.onRun(led12_callback);
              break;
            case 13: 
              ledsActive[j].mThread.onRun(led13_callback);
              break;
            case 14: 
              ledsActive[j].mThread.onRun(led14_callback);
              break;
            case 15: 
              ledsActive[j].mThread.onRun(led15_callback);
              break;
            case 16: 
              ledsActive[j].mThread.onRun(led16_callback);
              break;
            case 17: 
              ledsActive[j].mThread.onRun(led17_callback);
              break;
            
           }
          ledsActive[j].mThread.setInterval(50);
          controll.add(&ledsActive[j].mThread);
          //Serial.println(controll.size());
          j++;
        }
        i++;
         Serial.flush(); //Clean the buffer
    } // delimiter is the semicolon
     
  }

    controll.run();

   
} // End loop()
void blink_leds(int pin, int intensity, int ledState, int index){


           unsigned long currentMillis = millis();
          // Serial.println(currentMillis - leds[index].previousMillis);
           
           if(intensity!=(100)){
            if (currentMillis - leds[index].previousMillis >= intensity) {
            // save the last time you blinked the LED
            leds[index].previousMillis = currentMillis;
        
            // if the LED is off turn it on and vice-versa:
            if (leds[index].state == LOW) {
              leds[index].state = HIGH;
            } else {
              leds[index].state = LOW;
            }
        
            // set the LED with the ledState of the variable:
            if(pin<14){
              digitalWrite(pin,leds[index].state);
            }else if(pin==14){
              digitalWrite(A2,leds[index].state);
            }else if(pin==15){
              digitalWrite(A3,leds[index].state);
            }else if(pin==16){
              digitalWrite(A4,leds[index].state);
            }else if(pin==17){
              digitalWrite(A5,leds[index].state);
            }
           }
           }else{
            if (currentMillis - leds[index].previousMillis >= intensity) {
            // save the last time you blinked the LED
            leds[index].previousMillis = currentMillis;
        
           
           leds[index].state = LOW;
            
        
            // set the LED with the ledState of the variable:
            if(pin<14){
              digitalWrite(pin,leds[index].state);
            }else if(pin==14){
              digitalWrite(A2,leds[index].state);
            }else if(pin==15){
              digitalWrite(A3,leds[index].state);
            }else if(pin==16){
              digitalWrite(A4,leds[index].state);
            }else if(pin==17){
              digitalWrite(A5,leds[index].state);
            }
           }
           }
}
/*
void blink_leds(float inten){
  int i = 0;
  float intensity = inten * 1000;
  long interval = (long) intensity;
  Serial.println(intensity);
  for(i = 0; i < 16; i++){
    if(leds[i].pin <= 13){
      if(leds[i].turnOn)
        digitalWrite(leds[i].pin, HIGH);
    }else{
      if(leds[i].turnOn){
        switch(leds[i].pin){
          case 14:
            digitalWrite(A2, HIGH);
            break;
          case 15:
            digitalWrite(A3, HIGH);
            break;
          case 16:
            digitalWrite(A4, HIGH);
            break;
          case 17:
            digitalWrite(A5, HIGH);
            break;    
          }  
      }
    }
  }
  delay(interval);
    for(i = 0; i < 17; i++){
    if(leds[i].pin <= 13){
      if(leds[i].turnOn)
        digitalWrite(leds[i].pin, LOW);
    }else{
      if(leds[i].turnOn){
        switch(leds[i].pin){
          case 14:
            digitalWrite(A2, LOW);
            break;
          case 15:
            digitalWrite(A3, LOW);
            break;
          case 16:
            digitalWrite(A4, LOW);
            break;
          case 17:
            digitalWrite(A5, LOW);
            break;    
          }  
      }
    }
  }
  delay(interval);
}
*/
void init_leds(){
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT); 
}

void turn_on_all(){
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,HIGH);
  digitalWrite(A4,HIGH);
  digitalWrite(A5,HIGH);
 }

 void turn_off_all(){
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
  digitalWrite(A4,LOW);
  digitalWrite(A5,LOW);
 }

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop1(){
  digitalWrite(leds[0].pin, HIGH);
  delay(leds[0].intensity);
  digitalWrite(leds[0].pin, LOW);
  delay(leds[0].intensity);
}

void loop2(){
  digitalWrite(leds[1].pin, HIGH);
  delay(leds[1].intensity);
  digitalWrite(leds[1].pin, LOW);
  delay(leds[1].intensity);
}
