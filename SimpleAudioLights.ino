//#define DEBUGSAMPLE
//#define DEBUGCONSTANTS
//#define DEBUGSIGNALS
//#define DEBUGSCALED

#define SAMPLES 256
#define OUTPUTPIN 8
#define BASEPIN 11
#define TREBPIN 10
#define REVERSEPIN 9
#define ALPHA 8 //This is the filter used in the Low Pass Filter
#define SIGALPHAAGC 64   //Used in the AGC Calculation
#define BASEALPHAAGC 64   //Used in the AGC Calculation
#define TREBALPHAAGC 64   //Used in the AGC Calculation
#define SIGRATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define BASERATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define TREBRATCHETGAP 40 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...


  int smax=0;
  int lpmax=0;
  int hpmax=0;
  int samples[SAMPLES];
  
  float lpfilter,hpfilter;
  float lpfilterLast,hpfilterLast;
  float alpha;
  float baseAlphaAGC,trebAlphaAGC,sigAlphaAGC;
  float sampleF;

  float average=0,lpaverage=0,hpaverage=0;
  float averageLast=0,lpaverageLast=0,hpaverageLast=0;  

  bool sigTrig=false,lpTrig=false,hpTrig=false;


  
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
  alpha=1.0/ALPHA;
  sigAlphaAGC=1.0/SIGALPHAAGC;
  baseAlphaAGC=1.0/BASEALPHAAGC;
  trebAlphaAGC=1.0/TREBALPHAAGC;
}



void loop() {


  smax=0;
  lpmax=0;
  hpmax=0;

  
  for (int count=0;count<SAMPLES;count++)
  {
    samples[count]=analogRead(A0);
    if (samples[count]>smax) smax=samples[count];
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
  lpaverage = (baseAlphaAGC*(float)lpmax) + ( (1.0-baseAlphaAGC)*lpaverageLast );
  hpaverage = (trebAlphaAGC*(float)hpmax) + ( (1.0-trebAlphaAGC)*hpaverageLast );
  
  sigTrig=lpTrig=hpTrig=false;
  
  if(smax>average ) 
  { 
    sigTrig=true;
    average=smax-SIGRATCHETGAP; 
  }
    
  if(lpmax > lpaverage) 
  { 
    lpTrig=true;
    lpaverage=lpmax-BASERATCHETGAP;
   }
   
  if(hpmax>hpaverage ) 
  { 
    hpTrig=true;
    hpaverage=hpmax-TREBRATCHETGAP; 
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
      if(hpTrig==lpTrig==sigTrig==false)
      {
        digitalWrite(REVERSEPIN,HIGH);
      }
      else
      {
        digitalWrite(REVERSEPIN,LOW);
      }

 }

 
  
