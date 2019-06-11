//#define DEBUGSAMPLE
//#define DEBUGCONSTANTS
//#define DEBUGSIGNALS
//#define DEBUGSCALED

#define SAMPLES 64
#define OUTPUTPIN 8
#define BASEPIN 9
#define TREBPIN 10
#define ALPHA 10 //This is the filter used in the Low Pass Filter
#define SIGALPHAAGC 256   //Used in the AGC Calculation
#define BASEALPHAAGC 400   //Used in the AGC Calculation
#define TREBALPHAAGC 128   //Used in the AGC Calculation
#define SIGRATCHETGAP 20 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define BASERATCHETGAP 1 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
                         //The higher the value of Alpha the tighter the filter and less base is available
#define TREBRATCHETGAP 20 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define FULLSCALE 256
#define HEADROOM 1


  int smin=1024;
  int smax=0;
  int lpmin=1024;
  int lpmax=0;
  int hpmin=1024;
  int hpmax=0;
  int samples[SAMPLES];
  float lpfilter,hpfilter;
  float lpfilterLast,hpfilterLast;
  float alpha;
  float baseAlphaAGC,trebAlphaAGC,sigAlphaAGC;
  float sampleF;

  float average=0,lpaverage=0,hpaverage=0;
  float averageLast=0,lpaverageLast=0,hpaverageLast=0;  
  int p2p,lpp2p,hpp2p;
  int scaleSig,scaleLp,scaleHp;
  bool sigTrig=false,lpTrig=false,hpTrig=false;


  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.flush();
  Serial.println("hello");
  delay(1000);
  pinMode(A0,INPUT);
  pinMode(OUTPUTPIN,OUTPUT);
  pinMode(BASEPIN,OUTPUT);
  pinMode(TREBPIN,OUTPUT);
  alpha=1.0/ALPHA;
  sigAlphaAGC=1.0/SIGALPHAAGC;
  baseAlphaAGC=1.0/BASEALPHAAGC;
  trebAlphaAGC=1.0/TREBALPHAAGC;
  
#ifdef DEBUGCONSTANTS
Serial.print("Constants sigAlphaAGC : ");
Serial.print(sigAlphaAGC,8);
Serial.print(" | baseAlphaAGC : ");
Serial.print(baseAlphaAGC,8);
Serial.print(" | trebAlphaAGC : ");
Serial.println(trebAlphaAGC,8);
while(1);
#endif


/*Set prescaler for analogue read to 16 - speeds up the sampling process*/

/*
#ifndef cbi
#define cbi(sfr,bit)(_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr,bit)(_SFR_BYTE(sfr) &= _BV(bit))
#endif
sbi(ADCSRA,ADPS2);
sbi(ADCSRA,ADPS1);
cbi(ADCSRA,ADPS0);

*/

}



void loop() {
  // put your main code here, to run repeatedly:
  smin=1024;
  smax=0;
  lpmin=1024;
  lpmax=0;
  hpmin=1024;
  hpmax=0;

  
  int start=millis();
  for (int count=0;count<SAMPLES;count++)
  {
    samples[count]=analogRead(A0);
    if (samples[count]<smin) smin=samples[count];
    if (samples[count]>smax) smax=samples[count];
  }
  int endtime=millis();
  endtime=endtime-start;
  sampleF=(float)endtime/SAMPLES;
  sampleF=(1/sampleF)*1000;  
  p2p=smax-smin;
  
  lpfilterLast=samples[0];
  hpfilterLast=samples[0];
  
  for (int count=1;count<SAMPLES;count++)
  {
    lpfilter = (alpha*(float)samples[count]) + ( (1.0-alpha)*lpfilterLast );
    hpfilter =samples[count]-lpfilterLast;
    if (lpfilter<lpmin) lpmin=lpfilter;
    if (lpfilter>lpmax) lpmax=lpfilter;  
    if (hpfilter<hpmin) hpmin=hpfilter;
    if (hpfilter>hpmax) hpmax=hpfilter;

#ifdef DEBUGSAMPLE
    Serial.print("Sample : ");
    Serial.print(count);
    Serial.print(" Sampled at ");
    Serial.print(samples[count]);
    Serial.print(" lpFilter ");
    Serial.print(lpfilter);
    Serial.print(" hpFilter ");
    Serial.println(hpfilter);
#endif

  }

#ifdef DEBUGSAMPLE
    while(1);
#endif

  lpp2p = lpmax-lpmin;
  hpp2p = hpmax-hpmin;


  average = (sigAlphaAGC*(float)p2p) + ( (1.0-sigAlphaAGC)*averageLast );
  lpaverage = (baseAlphaAGC*(float)lpp2p) + ( (1.0-baseAlphaAGC)*lpaverageLast );
  hpaverage = (trebAlphaAGC*(float)hpp2p) + ( (1.0-trebAlphaAGC)*hpaverageLast );
  
  sigTrig=lpTrig=hpTrig=false;
  
  if(p2p>average ) 
  { 
    sigTrig=true;
    average=p2p-SIGRATCHETGAP; 
  }
    
  if(lpp2p > lpaverage) 
  { 
    lpTrig=true;
    /*
    Serial.print(lpp2p);
    Serial.print(" Greater than ");
    Serial.print(lpaverage);
    Serial.print(" lpTrigger Set "); 
    */
    lpaverage=lpp2p-BASERATCHETGAP;
   }
   
  if(hpp2p>hpaverage ) 
  { 
    hpTrig=true;
    hpaverage=hpp2p-TREBRATCHETGAP; 
   }

  averageLast=average;
  lpaverageLast=lpaverage;
  hpaverageLast=hpaverage;

  scaleSig= (int) ( (float) FULLSCALE * (float) p2p / (average*HEADROOM) );
  scaleLp = (int) ( (float) FULLSCALE * (float) lpp2p / (lpaverage*HEADROOM) );
  scaleHp = (int) ( (float) FULLSCALE * (float) hpp2p / (hpaverage*HEADROOM) );


#ifdef DEBUGSCALED
    Serial.print("Sig ");
    Serial.print(scaleSig);
    Serial.print(" LP ");
    Serial.print(scaleLp);
    Serial.print(" HP ");
    Serial.println(scaleHp);
#endif
  
  
  #ifdef DEBUGSIGNALS
  
    Serial.print(endtime);
    Serial.print(" : ");
    Serial.print(sampleF);
    Serial.print(" : ");
    Serial.print(p2p);
    Serial.print(" : AV :");
    Serial.print(average);
    Serial.print (" TRIG ");
    Serial.print (sigTrig);
    Serial.print(" : LPF = ");
    Serial.print(lpp2p);
    Serial.print(" : AV :");
    Serial.print(lpaverage);
    Serial.print (" TRIG ");
    Serial.print (lpTrig);
    Serial.print(" : HPF = ");
    Serial.print(hpp2p);
    Serial.print(" : AV :");
    Serial.print(hpaverage);
    Serial.print (" TRIG ");
    Serial.print (hpTrig);
    Serial.println (".");
#endif
    
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

 }

 
  
