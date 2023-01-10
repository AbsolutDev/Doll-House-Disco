/* Doll House Disco v0.4
 * 15 AUG 2019
 * 
 * Version Control
 * v0.1
 * -Basics set up and initial logic
 * 
 * v0.2
 * -All push buttons configured and actions for short press
 * 
 * v0.3
 * -Created DISCO Modes
 * -Implemented RGB LEDs dimming
 * -Implemented modes 8 and 9
 * -Fixed logic to allow LEDs 0 and 1 to be controlled during DISCO and SMOOTH modes
 * 
 * v0.4
 * 
 * ideas:
 * press sw0 and sw1 to increase/decrease m8delay
 * an additional mode made of 8-12 altogether
 * do we need << and >>??!
 */

//PINs assignment
int sw0=A0;
int sw1=A4;
int sw2=A1;
int swR=A2;
int swG=A3;
int swB=A5;

int led0L=4;
int led0R=7;
int led1L=2;
int led1R=12;

int ledLR=9;
int ledLG=6;
int ledLB=5;

int ledRR=10;
int ledRG=11;
int ledRB=3;

//Push-up buttons environment
int shortPressDur=10;   //min. duration to consider button press
int longPressDur=800;   //min. duration for long button press
int offDur=2500;        //min. duration for OFF button press
int rbStat;           //0=down; 1=up
int gbStat;           //0=down; 1=up
int bbStat;           //0=down; 1=up

//LEDs status
int st0L=0;
int st0R=0;
int st1L=0;
int st1R=0;
int stLR=0;
int stLG=0;
int stLB=0;
int stRR=0;
int stRG=0;
int stRB=0;

//Status mode: 0=OFF; 1=ON/STAND ALONE; 2-13=PROG; 5=HALLOWEEN
int mode=0;
int modes=6;  //temp limitation of modes

//HALLOWEEN MODE CONFIG
int maxfl=4;    //max number of flashes
int sdur=100;   //short flash duration (miliseconds)
int ldur=1000;  //long flash duration (miliseconds)
int minb=5;     //min pause (seconds)
int maxb=10;    //max pause (seconds)
boolean fadedown=false;


unsigned long timer=0;
int change=900;
int fly=70;
int maxCol=70;
int redCount;
int minRed=3;
int maxRed=11;
int count;
int fadeStep=60;

int m89delay=500;
int m8step;
int m9step;
int m10step;

void setup() {
  Serial.begin(9600);
  pinMode(sw0, INPUT_PULLUP);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(swR, INPUT_PULLUP);
  pinMode(swG, INPUT_PULLUP);
  pinMode(swB, INPUT_PULLUP);
  pinMode(led0L,OUTPUT);
  pinMode(led0R,OUTPUT);
  pinMode(led1L,OUTPUT);
  pinMode(led1R,OUTPUT);
  pinMode(ledLR,OUTPUT);
  pinMode(ledLG,OUTPUT);
  pinMode(ledLB,OUTPUT);
  pinMode(ledRR,OUTPUT);
  pinMode(ledRG,OUTPUT);
  pinMode(ledRB,OUTPUT);
}

void loop() {
  if (digitalRead(sw2)==0) {
    //CONTROL BUTTON PRESSED
    long pressTime=millis();
    while (digitalRead(sw2)==0) {
      if (millis()-pressTime>offDur && mode>0) {
        //LONG PRESS
        Serial.println("SWITCH OFF");
        st0L=0;
        st0R=0;
        st1L=0;
        st1R=0;
        stLR=0;
        stLG=0;
        stLB=0;
        stRR=0;
        stRG=0;
        stRB=0;
        digitalWrite(led0L,st0L);
        digitalWrite(led0R,st0R);
        digitalWrite(led1L,st1L);
        digitalWrite(led1R,st1R);
        analogWrite(ledLR,stLR);
        analogWrite(ledLG,stLG);
        analogWrite(ledLB,stLB);
        analogWrite(ledRR,stRR);
        analogWrite(ledRG,stRG);
        analogWrite(ledRB,stRB);
        mode=0;
      }
    }
    if (millis()-pressTime>shortPressDur && millis()-pressTime<offDur) {
      //SHORT PRESS
      if (mode==0) {
        Serial.println("Switch to mode 1: ALL ON");
        st0L=1;
        st0R=1;
        st1L=1;
        st1R=1;
        stLR=255;
        stLG=255;
        stLB=255;
        stRR=255;
        stRG=255;
        stRB=255;
      }
      mode++;
      timer=0;
      switch (mode) {
        case 8:
          m8step=0;
          break;
        case 9:
          m9step=0;
          break;
        case 10:
          m10step=0;
          break;
      }
      
      if (mode==modes) {
        Serial.println("Switch to mode 0: ALL OFF");
        st0L=0;
        st0R=0;
        st1L=0;
        st1R=0;
        stLR=0;
        stLG=0;
        stLB=0;
        stRR=0;
        stRG=0;
        stRB=0;
        mode=0;
      } else {
        Serial.print("Switch to mode ");
        Serial.println(mode);
      }
    }
  }
  
  if (digitalRead(sw0)==0 && mode<5) {
    //GROUND FLOOR BUTTON PRESSED
    long pressTime=millis();
    while (digitalRead(sw0)==0) {
      if (millis()-pressTime>longPressDur) {
        //LONG PRESS
        //Nothing to do
      }
    }
    if (millis()-pressTime>shortPressDur && millis()-pressTime<longPressDur) {
      //SHORT PRESS
      if (st0L==0) {
        st0L=255;
        st0R=255;
      } else {
        st0L=0;
        st0R=0;
      }
    }
  }
  if (digitalRead(sw1)==0 && mode<5) {
    //1st FLOOR BUTTON PRESSED
    long pressTime=millis();
    while (digitalRead(sw1)==0) {
      if (millis()-pressTime>longPressDur) {
        //LONG PRESS
        //Nothing to do
      }
    }
    if (millis()-pressTime>shortPressDur && millis()-pressTime<longPressDur) {
      //SHORT PRESS
      if (st1L==0) {
        st1L=255;
        st1R=255;
      } else {
        st1L=0;
        st1R=0;
      }
    }
  }

  if (digitalRead(swR)==0 && mode<2) {
    //RED BUTTON PRESSED
    long pressTime=millis();
    while (digitalRead(swR)==0) {
      if (millis()-pressTime>longPressDur) {
        //LONG PRESS
        if (stLR==0 && rbStat==0) {               //switch to going up
          rbStat=1;
          delay(100);
        }    
        if (stLR==254 && rbStat==1) {              //switch to going down
          stLR++;
          stRR++;
          analogWrite(ledLR,0);
          analogWrite(ledRR,0);
          delay(50);
          analogWrite(ledLR,stLR);
          analogWrite(ledRR,stRR);
          delay(100);
          rbStat=0;
        } 
        if (rbStat==1) {
          stLR++;
          stRR++;
        } else {
          stLR--;
          stRR--;
        }
        analogWrite(ledLR,stLR);
        analogWrite(ledRR,stRR);
        delay(fadeStep);
      }
    }
    if (millis()-pressTime>shortPressDur && millis()-pressTime<longPressDur) {
      //SHORT PRESS
      if (stLR==0) {
        stLR=255;
        stRR=255;
      } else {
        stLR=0;
        stRR=0;
      }
    }
  }
  if (digitalRead(swG)==0 && mode<2) {
    //GREEN BUTTON PRESSED
    long pressTime=millis();
    while (digitalRead(swG)==0) {
      if (millis()-pressTime>longPressDur) {
        //LONG PRESS
        if (stLG==0 && gbStat==0) {               //switch to going up
          gbStat=1;
          delay(100);
        }    
        if (stLG==254 && gbStat==1) {              //switch to going down
          stLG++;
          stRG++;
          analogWrite(ledLG,0);
          analogWrite(ledRG,0);
          delay(50);
          analogWrite(ledLG,stLG);
          analogWrite(ledRG,stRG);
          delay(100);
          gbStat=0;
        } 
        if (gbStat==1) {
          stLG++;
          stRG++;
        } else {
          stLG--;
          stRG--;
        }
        analogWrite(ledLG,stLG);
        analogWrite(ledRG,stRG);
        delay(fadeStep);
      }
    }
    if (millis()-pressTime>shortPressDur && millis()-pressTime<longPressDur) {
      //SHORT PRESS
      if (stLG==0) {
        stLG=255;
        stRG=255;
      } else {
        stLG=0;
        stRG=0;
      }
    }
  }
  if (digitalRead(swB)==0 && mode<2) {
    //BLUE BUTTON PRESSED
    long pressTime=millis();
    while (digitalRead(swB)==0) {
      if (millis()-pressTime>longPressDur) {
        //LONG PRESS
        if (stLB==0 && bbStat==0) {               //switch to going up
          bbStat=1;
          delay(100);
        }    
        if (stLB==254 && bbStat==1) {              //switch to going down
          stLB++;
          stRB++;
          analogWrite(ledLB,0);
          analogWrite(ledRB,0);
          delay(50);
          analogWrite(ledLB,stLB);
          analogWrite(ledRB,stRB);
          delay(100);
          bbStat=0;
        } 
        if (bbStat==1) {
          stLB++;
          stRB++;
        } else {
          stLB--;
          stRB--;
        }
        analogWrite(ledLB,stLB);
        analogWrite(ledRB,stRB);
        delay(fadeStep);
      }
    }
    if (millis()-pressTime>shortPressDur && millis()-pressTime<longPressDur) {
      //SHORT PRESS
      if (stLB==0) {
        stLB=255;
        stRB=255;
      } else {
        stLB=0;
        stRB=0;
      }
    }
  }
  if (mode==1 && st0L==0 && st1L==0 && stLR==0 && stLG==0 && stLB==0 && stRR==0 && stRG==0 && stRB==0) {
    mode=0;
  }

  switch (mode) {
    case 2: //DISCO RGB SPEED: SLOW
    case 3: //DISCO RGB SPEED: MEDIUM
    case 4: //DISCO RGB SPEED: FAST
      if (timer==0) {
        stLR=maxCol;
        stRR=maxCol;
        stLG=0;
        stRG=0;
        stLB=0;
        stRB=0;
        redCount=1;
        while (redCount<minRed) {
          redCount=random(maxRed);
        }
        count=0;
        timer=millis();
      }
      if (millis()-timer>change/(mode-1)) {
        if (millis()-timer<change/(mode-1)+fly/(mode-1)) {
          stLR=0;
          stRR=0;
          stLG=0;
          stRG=0;
          stLB=0;
          stRB=0;
        } else {
          count++;
          if (count==redCount) {
            stLR=150;
            stRR=150;
            stLG=0;
            stRG=0;
            stLB=0;
            stRB=0;
            redCount=1;
            while (redCount<minRed) {
              redCount=random(maxRed);
            }
            count=0;
          } else {
            stLR=random(maxCol);
            stRR=stLR;
            stLG=random(maxCol);
            stRG=stLG;
            stLB=random(maxCol);//maxCol-stLR;
            stRB=stLB;
          }
          timer=millis();
        }
      }
      break;
    case 5:      //HALLOWEEN
      if (timer==0) {
        st0L=0;
        st0R=0;
        st1L=0;
        st1R=0;
        stLR=255;
        stRR=255;
        stLG=0;
        stLB=0;
        stRG=0;
        stRB=0;
        timer=millis();
        fadedown=true;
      } else {
        Serial.println(stLR);
        if (fadedown) {
          stLR=stLR-5;
          stRR=stLR;
          if (stLR<5) { fadedown=false; };
        } else {
          stLR=stLR+5;
          stRR=stLR;
          if (stLR>250) { fadedown=true; };
        }
      }
      break;
    case 6:
      Serial.println("MODE 6 ON");
      break;
    case 7:
      Serial.println("MODE 7 ON");
      break;
    case 8:
      //UP UP WAVE
      if (m8step==0) {
        m8step++;
        st0L=0;
        st0R=0;
        st1L=0;
        st1R=0;
        stLR=0;
        stLG=0;
        stLB=0;
        stRR=0;
        stRG=0;
        stRB=0;
        timer=millis();
      }
      if (millis()-timer>m89delay) {
        switch (m8step) {
          case 1:
            st0L=1;
            st0R=1;
            m8step++;
            break;
          case 2:
            st1L=1;
            st1R=1;
            m8step++;
            break;
          case 3:
            stLR=255;
            stLG=255;
            stLB=255;
            stRR=255;
            stRG=255;
            stRB=255;
            m8step++;
            break;
          case 4:
            st0L=0;
            st0R=0;
            m8step++;
            break;
          case 5:
            st1L=0;
            st1R=0;
            m8step++;
            break;
          case 6:
            stLR=0;
            stLG=0;
            stLB=0;
            stRR=0;
            stRG=0;
            stRB=0;
            m8step=1;
            break;
        }
        timer=millis();
      }
      break;
    case 9:
      //UP DOWN WAVE
      if (m9step==0) {
        m9step++;
        st0L=0;
        st0R=0;
        st1L=0;
        st1R=0;
        stLR=0;
        stLG=0;
        stLB=0;
        stRR=0;
        stRG=0;
        stRB=0;
        timer=millis();
      }
      if (millis()-timer>m89delay) {
        switch (m9step) {
          case 1:
            st0L=1;
            st0R=1;
            m9step++;
            break;
          case 2:
            st1L=1;
            st1R=1;
            m9step++;
            break;
          case 3:
            stLR=255;
            stLG=255;
            stLB=255;
            stRR=255;
            stRG=255;
            stRB=255;
            m9step++;
            break;
          case 4:
            stLR=0;
            stLG=0;
            stLB=0;
            stRR=0;
            stRG=0;
            stRB=0;
            m9step++;
            break;
          case 5:
            st1L=0;
            st1R=0;
            m9step++;
            break;
          case 6:
            st0L=0;
            st0R=0;
            m9step=1;
            break;
        }
        timer=millis();
      }
      break;
    case 10:
      //RIGHT RIGHT WAVE
      if (m10step==0) {
        m10step++;
        st0L=0;
        st0R=0;
        st1L=0;
        st1R=0;
        stLR=0;
        stLG=0;
        stLB=0;
        stRR=0;
        stRG=0;
        stRB=0;
        timer=millis();
      }
      if (millis()-timer>m89delay) {
        switch (m10step) {
          case 1:
            st0R=1;
            m10step++;
            break;
          case 2:
            st0L=1;
            m10step++;
            break;
          case 3:
            st1L=1;
            m10step++;
            break;
          case 4:
            stLR=255;
            stLG=255;
            stLB=255;
            m10step++;
            break;
          case 5:
            stRR=255;
            stRG=255;
            stRB=255;
            m10step++;
            break;
          case 6:
            st1R=1;
            m10step++;
            break;
          case 7:
            st0R=0;
            m10step++;
            break;
          case 8:
            st0L=0;
            m10step++;
            break;
          case 9:
            st1L=0;
            m10step++;
            break;
          case 10:
            stLR=0;
            stLG=0;
            stLB=0;
            m10step++;
            break;
          case 11:
            stRR=0;
            stRG=0;
            stRB=0;
            m10step++;
            break;
          case 12:
            st1R=0;
            m10step=1;
            break;
        }
        timer=millis();
      }
      break;
    case 11:
      //LEFT LEFT WAVE
      Serial.println("MODE 11 ON");
      break;
    case 12:
      //SPIRAL
      Serial.println("MODE 12 ON");
      break;
    case 13:
      //HAUNTED HOUSE
      Serial.println("MODE 13 ON");
      break;
  }
  digitalWrite(led0L,st0L);
  digitalWrite(led0R,st0R);
  digitalWrite(led1L,st1L);
  digitalWrite(led1R,st1R);
  analogWrite(ledLR,stLR);
  analogWrite(ledLG,stLG);
  analogWrite(ledLB,stLB);
  analogWrite(ledRR,stRR);
  analogWrite(ledRG,stRG);
  analogWrite(ledRB,stRB);
}

/*
 * //RIGHT RIGHT WAVE
      if (m10step==0) {
        m10step++;
        st0L=0;
        st0R=0;
        st1L=0;
        st1R=0;
        stLR=0;
        stLG=0;
        stLB=0;
        stRR=0;
        stRG=0;
        stRB=0;
        timer=millis();
      }
      if (millis()-timer>m89delay) {
        switch (m10step) {
          case 1:
            st0R=1;
            st1R=1;
            stRR=255;
            stRG=255;
            stRB=255;
            m10step++;
            break;
          case 2:
            st0L=1;
            st1L=1;
            stLR=255;
            stLG=255;
            stLB=255;
            m10step++;
            break;
          case 3:
            st0R=0;
            st1R=0;
            stRR=0;
            stRG=0;
            stRB=0;
            m10step++;
            break;
          case 4:
            st0L=0;
            st1L=0;
            stLR=0;
            stLG=0;
            stLB=0;
            m10step=1;
            break;
 */
