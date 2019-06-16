#ifndef MORSESENDER
#define MORSESENDER

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Morse Sender Class implements a way to send a single letter using an LED Pin
//  Assignment.
//////////////////////////////////////////////////////////////////////////////////////////////////

class MorseSender {
  public:
   int tempo;
    MorseSender(void);
    MorseSender(int ledpin);
    void SendString(String);
    void SendLetter(byte letter);
    void StartTX(void);
    void Flash(void);
    void Flash(int);

  private:
    int LEDPIN;
    void dot (void);
    void dash (void);
    void charGap(void);
    void wordGap(void);
    
};

#endif
