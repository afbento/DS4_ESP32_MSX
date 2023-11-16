#include <dummy.h>

#include <PS4Controller.h>

#include "SPIFFS.h"

#define PIN8_CONTROL FALSE   //if you use the pin-8 schematic version, set TRUE

#define UP_JOY1    12
#define DOWN_JOY1  13
#define RIGHT_JOY1 14
#define LEFT_JOY1  25
#define TRIG1_JOY1   26
#define TRIG2_JOY1   27

#define UP_JOY2    4
#define DOWN_JOY2  5
#define RIGHT_JOY2 15
#define LEFT_JOY2  18
#define TRIG1_JOY2  19
#define TRIG2_JOY2  21

int deadzone=64,deadzone2=90,i,j,x,y,x1;

String text;

int keymap[12],keymap1[30],keymap2[30],pinout[12],mode=0,kmode=0,kmode1,kmode2,pindex,wlight=0;

void save_config() {
  File file=SPIFFS.open("/config.txt",FILE_WRITE);
  file.printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d",keymap[0],keymap[1],keymap[2],keymap[3],keymap[4],keymap[5],
                                                          keymap[6],keymap[7],keymap[8],keymap[9],keymap[10],keymap[11],
                                                          deadzone,deadzone2);
  file.close();
}

void load_config() {
  File file=SPIFFS.open("/config.txt",FILE_READ);
  while(file.available()) text = file.readString();//text+=file.read();//Serial.write(file.read());
  sscanf(text.c_str(),"%d %d %d %d %d %d %d %d %d %d %d %d %d %d",&keymap[0],&keymap[1],&keymap[2],&keymap[3],&keymap[4],&keymap[5],
                                                                  &keymap[6],&keymap[7],&keymap[8],&keymap[9],&keymap[10],&keymap[11],
                                                                  &deadzone,&deadzone2);
  file.close();
}

void setup() {
	
  Serial.begin(115200);
  
	//pinos do lado do botao EN 
	pinout[0]=UP_JOY1; pinout[1]=DOWN_JOY1; pinout[2]=RIGHT_JOY1; pinout[3]=LEFT_JOY1; pinout[4]=TRIG1_JOY1; pinout[5]=TRIG2_JOY1;
  
  //pinos do lado do botão BOOT
  pinout[6]=UP_JOY2; pinout[7]=DOWN_JOY2; pinout[8]=RIGHT_JOY2; pinout[9]=LEFT_JOY2; pinout[10]=TRIG1_JOY2; pinout[11]=TRIG2_JOY2;
  
  for(i=0;i<12;i++) pinMode(pinout[i],OUTPUT);

  for(i=0;i<12;i++) digitalWrite(pinout[i],HIGH);

  #ifdef PIN8_CONTROL   
  pinMode(33,INPUT); //PIN8_JOY1
  pinMode(32,INPUT); //PIN8_JOY2
  pinMode(35,OUTPUT); //Controls U2
  pinMode(34,OUTPUT); //Controls U3
  #endif
  
  // Right=0, Down=1, Up=2, Left=3
  // Square=4, Cross=5, Circle=6, Triangle=7
  // UpRight=8, DownRight=9, UpLeft=10, DownLeft=11
  
  // Share=12, Options=13, 
  // PSButton=14, TouchPad=15
  
  // LStickX = 16(L), 17(R)
  // LStickY = 18(U), 19(D)
  // RStickX = 20(L), 21(R)
  // RStickY = 22(U), 23(D)

  // L1=24, R1=25
  // L2=26, R2=27
  // L3=28, R3=29
  
  // joystick 1 - UP, DOWN, RIGHT, LEFT, BUTTON 1, BUTTON 2
  keymap[0]=22;keymap[1]=23;keymap[2]=21;keymap[3]=20;
  keymap[4]=26;keymap[5]=24;
  
  
  // joystick 2 - UP, DOWN, RIGHT, LEFT, BUTTON 1, BUTTON 2
  keymap[6]=18;keymap[7]=19;keymap[8]=17;keymap[9]=16;
  keymap[10]=27;keymap[11]=25;


  SPIFFS.begin(true);
  load_config();

  PS4.begin();
}

void scan_joy() {
	for(i=0;i<30;i++) keymap2[i]=0;

	if(PS4.Right()) keymap2[0]=1;
  if(PS4.Down()) keymap2[1]=1;
  if(PS4.Up()) keymap2[2]=1;
  if(PS4.Left()) keymap2[3]=1;

  if(PS4.Square()) keymap2[4]=1;
  if(PS4.Cross()) keymap2[5]=1;
  if(PS4.Circle()) keymap2[6]=1;
  if(PS4.Triangle()) keymap2[7]=1;

  if(PS4.UpRight()) {keymap2[2]=1;keymap2[0]=1;}
  if(PS4.DownRight()) {keymap2[0]=1;keymap2[1]=1;}
  if(PS4.UpLeft()) {keymap2[2]=1;keymap2[3]=1;}
  if(PS4.DownLeft()) {keymap2[3]=1;keymap2[1]=1;}
  
  if(PS4.Share()) keymap2[12]=1;
  if(PS4.Options()) keymap2[13]=1;
   
  if(PS4.PSButton()) keymap2[14]=1;
  if(PS4.Touchpad()) keymap2[15]=1;
    
  x=PS4.LStickX();
  if(x) {if(x>deadzone) keymap2[17]=1; else if(x<-deadzone) keymap2[16]=1; }
	
  y=PS4.LStickY();
  if(y) {if(y>deadzone) keymap2[18]=1; else if(y<-deadzone) keymap2[19]=1; }
	
  x=PS4.RStickX();
  if(x) {if(x>deadzone) keymap2[21]=1; else if(x<-deadzone) keymap2[20]=1; }
	
  y=PS4.RStickY();
  if(y) {if(y>deadzone) keymap2[22]=1; else if(y<-deadzone) keymap2[23]=1; }

  if(PS4.L1()) keymap2[24]=1;
  if(PS4.R1()) keymap2[25]=1;

  if(PS4.L2()) if(abs(PS4.L2Value())>deadzone2) keymap2[26]=1;
	if(PS4.R2()) if(abs(PS4.R2Value())>deadzone2) keymap2[27]=1;

  if(PS4.L3()) keymap2[28]=1;
  if(PS4.R3()) keymap2[29]=1;
 
}

void loop() {
  
  #ifdef PIN8_CONTROL
  if(digitalRead(33)) digitalWrite(35,HIGH); else digitalWrite(35,LOW); //if PIN8_JOY1 equals HIGH then disables U2, setting its output signals to Hi-Z
  if(digitalRead(32)) digitalWrite(34,HIGH); else digitalWrite(34,LOW); //if PIN8_JOY2 equals HIGH then disables U3, setting its output signals to Hi-Z
  #endif

  if (PS4.isConnected()) {
    
  	scan_joy();

    if((keymap2[13])&&(kmode==0)) kmode=1;
    if((kmode==1)&&(keymap2[13]==0)) {
      kmode=0;mode++;save_config();if(mode==3) mode=0;
      pindex=0;kmode1=-1;wlight=0;
      for(i=0;i<30;i++) keymap1[i]=0;
      keymap1[13]=1;
    }

    if(mode==0)	{   //operational mode
            
      PS4.setLed(84,85,237);

      x=0;
      i=keymap2[keymap[0]];
      i+=keymap2[keymap[2]]*2;
      i+=keymap2[keymap[1]]*4;
      i+=keymap2[keymap[3]]*8;
      
      if(i==1) x=32; else if(i==3) x=64; else if(i==2) x=96;
      if(i==6) x=128; else if(i==4) x=160; else if(i==12) x=192;
      if(i==8) x=224; else if(i==9) x=255;

      y=0;
      i=keymap2[keymap[6]];
      i+=keymap2[keymap[8]]*2;
      i+=keymap2[keymap[7]]*4;
      i+=keymap2[keymap[9]]*8;
      
      if(i==1) y=32; else if(i==3) y=64; else if(i==2) y=96;
      if(i==6) y=128; else if(i==4) y=160; else if(i==12) y=192;
      if(i==8) y=224; else if(i==9) y=255;

      if((x>0)||(y>0)) PS4.setLed(0,y,x);
         
      for(i=0;i<12;i++) 
        if(keymap2[keymap[i]]) {
          y=1;
          digitalWrite(pinout[i],LOW);
           
          if((i==4)||(i==10)) PS4.setLed(255,0,0); else
          if((i==5)||(i==11)) PS4.setLed(64,0,0); //else PS4.setLed(0,0,128); 

        } else digitalWrite(pinout[i],HIGH);
      
      PS4.sendToController();
    }
    
    if(mode==1) {    //Remapping mode
      PS4.setLed(0,255,0);PS4.sendToController();
      if(kmode1==-1) {
        for(i=0;i<30;i++) if((keymap2[i])&&(!keymap1[i])) {kmode1=i;keymap1[i]=1;}
      } else {
        if(!keymap2[kmode1]) {
          keymap[pindex]=kmode1;pindex++;kmode1=-1;
          wlight=20;
        }
      }
      if(pindex==12) {mode=0;save_config();}
    } 
    
    if(mode==2) {  //Adjusting "deadzones" mode
      x=abs(PS4.LStickX());
      y=abs(PS4.L2Value());
      x1=x*2;if(x1>255) x1=255;
      PS4.setLed(32+y/1.15,32,x1);
      if((keymap2[5]==1)&&(x>32)) {deadzone=x;PS4.setLed(255,255,255);}
      if((keymap2[6]==1)&&(x>32)) {deadzone2=x;PS4.setLed(255,255,255);}
      PS4.sendToController();
    }

    if(wlight>0) {PS4.setLed(255,255,255);PS4.sendToController();wlight--;}
   
  } else {
    mode=0;kmode=0;
    for(i=0;i<12;i++) digitalWrite(pinout[i],HIGH);
	
  }
  
  for(i=0;i<12;i++) Serial.printf("km[%d]=%d ",i,keymap[i]);
  Serial.printf("dz=%d dz2=%d",deadzone,deadzone2);
  Serial.println();
  
}
