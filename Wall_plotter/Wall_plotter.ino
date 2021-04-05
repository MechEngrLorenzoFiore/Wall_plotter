#include <Stepper.h>
#include "Image.h"

// parametri degli stepper motors
#define SPR 2038    // 32 STEPS per revolution per 64:1 GEAR RATIO
#define SPD 14      // SPEED OF ROTATION in rpm
#define SPM 31     // STEP per mm

// parametri geometrici fondamentali del v plotter
float B = 250;       // baseline 180 mm
float LL0 = 310;     // coordinate di home 
float LR0 = 310;     // coordinate di home 

// inizializzazione variabili
float LL1 = LL0;
float LR1 = LR0;

//LEFT MOTOR invert IN2 and IN3 for correct verse of rotation
Stepper LM(SPR, 2, 3, 4, 5); 

//RIGHT MOTOR 
Stepper RM(SPR, 8, 9, 10, 11); 

int dir = 1; // comando di direzione 

void setup() {
  // set the speed at 60 rpm:
  LM.setSpeed(SPD);
  RM.setSpeed(SPD);
  // initialize the serial port:
  Serial.begin(9600);
  Serial.println("-------------------- NEW UPLOAD -----------------------");
  Serial.print("Step per mm = ");
  Serial.println(SPM);
  Serial.print("rpm = ");
  Serial.println(SPD);
  Serial.print("LL0 = ");
  Serial.println(LL0);
  Serial.print("LR0 = ");
  Serial.println(LR0);
}

void loop() {
  if (dir == 0) {
    while (!Serial.available()) {}  //Waits for an input on the serial device
    dir = Serial.parseInt();       //Takes the Serial input and looks for an integer
    Serial.flush();
  }else{
  switch(dir){
    case 8:
      Serial.println("--------------- going up ---------------");
      movexy(0,-10);
      dir = 0; // reset della variabile di comando
      break;
    case 2:  
      Serial.println("--------------- going down ---------------");
      movexy(0,10);
      dir = 0; // reset della variabile di comando
      break;
    case 4:
      Serial.println("--------------- going left ---------------");
      movexy(-10,0);
      dir = 0; // reset della variabile di comando
      break;
    case 6:
      Serial.println("--------------- going right ---------------");
      movexy(10,0);
      dir = 0; // reset della variabile di comando
      break;  
    case 5:
      Serial.println("--------------- coming home ---------------");  
      homing();
      dir = 0; // reset della variabile di comando
      break; 
    case 9:
      Serial.println("--------------- Draw image ---------------");  
      for (byte i = 0; i < (sizeof(Image) / sizeof(Image[0])); i=i+2) {
          movexy(Image[i],Image[i+1]);
      }
      dir = 0; // reset della variabile di comando
      break;    
    default:
      Serial.print(dir);
      Serial.println(" is an invalid command");
      dir = 0; // reset della variabile di comando
      break;
  }
 }
}

// funzione per gli spostamenti relativi
void movexy(int x, int y){
  Serial.print("LL1 = ");
  Serial.println(LL1);
  Serial.print("LR1 = ");
  Serial.println(LR1);
  
  float cosa1 = (B*B+LL1*LL1-LR1*LR1)/(2*B*LL1);
  float x1 = LL1*cosa1;
  float y1 = LL1* sqrt( 1-cosa1*cosa1 );
 
  float LL2 = sqrt( (x+x1)*(x+x1) + (y+y1)*(y+y1) );
  float LR2 = sqrt( (B-(x+x1))*(B-(x+x1)) + (y+y1)*(y+y1) );

  Serial.print("LL2 = ");
  Serial.println(LL2);
  Serial.print("LR2 = ");
  Serial.println(LR2);

  //spostamento alle lunghezze calcolate
  int SLM = (int)(LL2-LL1)*SPM;
  int SRM = (int)(LR2-LR1)*SPM;
  
  Serial.print("STEP LM = ");
  Serial.println(SLM);
  Serial.print("STEP RM = ");
  Serial.println(SRM);
  
  LM.step(SLM);
  RM.step(SRM);
  
  //ridefinizione delle variabili
  LL1 = LL2;
  LR1 = LR2;
}

void homing(){
  float cosa0 = (B*B+LL0*LL0-LR0*LR0)/(2*B*LL0);
  float x0 = LL0*cosa0;
  float y0 = LL0* sqrt( 1-cosa0*cosa0 );
  Serial.print("x0 = ");
  Serial.print(x0);
  Serial.print("  ");
  Serial.print("y0 = ");
  Serial.println(y0);

  float cosa1 = (B*B+LL1*LL1-LR1*LR1)/(2*B*LL1);
  float x1 = LL1*cosa1;
  float y1 = LL1* sqrt( 1-cosa1*cosa1 );
  Serial.print("x1 = ");
  Serial.print(x1);
  Serial.print("  ");
  Serial.print("y1 = ");
  Serial.println(y1);
  
  movexy(x0-x1,y0-y1);  
}
