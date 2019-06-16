#include "MorseSender.h"

#define SAMPLES 128
#define OUTPUTPIN 10
#define BASEPIN 11
#define TREBPIN 8
#define REVERSEPIN 9

#define ALPHA 8 //This is the filter used in the Low Pass Filter
#define SIGALPHAAGC 64   //Used in the AGC Calculation
#define BASEALPHAAGC 64   //Used in the AGC Calculation
#define TREBALPHAAGC 64   //Used in the AGC Calculation
#define SIGRATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define BASERATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define TREBRATCHETGAP 20 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define SIGLATCHLENGTH 1 //Once triggered this is the on dwell time
#define HPLATCHLENGTH 2 //Once triggered this is the on dwell time
#define LPLATCHLENGTH 10 //Once triggered this is the on dwell time

MorseSender Morse(LED_BUILTIN);

  int smax=0;
  int lpmax=0;
  int hpmax=0;
  int samples[SAMPLES];
  
  float lpfilter,hpfilter;
  float lpfilterLast,hpfilterLast;
  float alpha;
  float sigAlphaAGC;
  float sampleF;

  float average=0,lpaverage=0,hpaverage=0;
  float averageLast=0,lpaverageLast=0,hpaverageLast=0;  

  bool sigTrig=false,lpTrig=false,hpTrig=false;
  
  unsigned int lplatch;
  unsigned int hplatch;
  unsigned int siglatch;

  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.flush();
  Serial.println("hello");
  pinMode(A0,INPUT);
  pinMode(OUTPUTPIN,OUTPUT);
  pinMode(BASEPIN,OUTPUT);
  pinMode(TREBPIN,OUTPUT);
  pinMode(REVERSEPIN,OUTPUT);
  pinMode(A1,INPUT);
  pinMode(2,INPUT_PULLUP);
  alpha=1.0/ALPHA;
  sigAlphaAGC=1.0/SIGALPHAAGC;
   Morse.tempo=30;
   Morse.Flash(1);
   Morse.SendString("V1.0");
 
}



void loop() {


  smax=0;
  lpmax=0;
  hpmax=0;



  if (digitalRead(2)==LOW)
  {
  for (int count=0;count<SAMPLES;count++)             //Without Anti-Alias Fitler
    {
      samples[count]=analogRead(A0);
      if (samples[count]>smax) smax=samples[count];
    }
  }
  else
  {
    for (int count=0;count<SAMPLES;count++) 
    {
      samples[count]=analogRead(A1);                  //With Aanalogue Anti-Alias Filter
      if (samples[count]>smax) smax=samples[count];
    }
  }

  lpfilterLast=samples[0];
  hpfilterLast=samples[0];
  
  for (int count=1;count<SAMPLES;count++)
  {
    lpfilter = (alpha*(float)samples[count]) + ( (1.0-alpha)*lpfilterLast );
    hpfilter =samples[count]-lpfilterLast;
    if (lpfilter>lpmax) lpmax=lpfilter;  
    if (hpfilter>hpmax) hpmax=hpfilter;
  }

  average = (sigAlphaAGC*(float)smax) + ( (1.0-sigAlphaAGC)*averageLast );
  //lpaverage = (baseAlphaAGC*(float)lpmax) + ( (1.0-baseAlphaAGC)*lpaverageLast );
  //hpaverage = (trebAlphaAGC*(float)hpmax) + ( (1.0-trebAlphaAGC)*hpaverageLast );
  lpaverage = (sigAlphaAGC*(float)lpmax) + ( (1.0-sigAlphaAGC)*lpaverageLast );
  hpaverage = (sigAlphaAGC*(float)hpmax) + ( (1.0-sigAlphaAGC)*hpaverageLast );


  lplatch++;
  hplatch++;
  siglatch++;

  
  if (lplatch>=LPLATCHLENGTH) {lpTrig=false;}
  if (hplatch>=HPLATCHLENGTH) {hpTrig=false;} 
  if (siglatch>=SIGLATCHLENGTH) {sigTrig=false;}

  
  if(smax>average) 
  { 
    sigTrig=true;
    average=smax-SIGRATCHETGAP;
    if (siglatch > SIGLATCHLENGTH) { 
      siglatch=0;
    }
  }
    
  if(lpmax > lpaverage) 
  { 
    lpTrig=true;
    lpaverage=lpmax-BASERATCHETGAP;
    if (lplatch > LPLATCHLENGTH)
    {
         lplatch=0;
    }
   }
   
  if(hpmax>hpaverage ) 
  { 
    hpTrig=true;
    hpaverage=hpmax-TREBRATCHETGAP; 
    if (hplatch > HPLATCHLENGTH)
    {
      hplatch=0;
    }
   }

  averageLast=average;
  lpaverageLast=lpaverage;
  hpaverageLast=hpaverage;

 
     if (sigTrig==false) {
        digitalWrite(OUTPUTPIN,LOW);
      } else {
        digitalWrite(OUTPUTPIN,HIGH);
      } //Flash in time to music

      if (lpTrig==false) {
        digitalWrite(BASEPIN,LOW);
      } else {
        digitalWrite(BASEPIN,HIGH);
      } //Flash in time to music

      if (hpTrig==false) {
        digitalWrite(TREBPIN,LOW);
      } else {
        digitalWrite(TREBPIN,HIGH);
      } //Flash in time to music
      if(
        (hpTrig==false) &&
        (lpTrig==false) &&
        (sigTrig==false)
        )
      {
        digitalWrite(REVERSEPIN,HIGH);
      }
      else
      {
        digitalWrite(REVERSEPIN,LOW);
      }

 }

 
  
