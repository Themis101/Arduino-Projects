#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#define WHITE 1
#define BLACK 0
LiquidCrystal_I2C lcd(0x27,16,2); 

  unsigned int time[2]; //in seconds
  bool isSet=false,changedSides=false;
  char playing_side; // 1 is white and 0 is black 
  int set=0,blackButton=0,whiteButton=0;



void setup() { 
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0,0); 
  lcd.print(" BLACK || WHITE"); 
  lcd.setCursor(7, 1);
  lcd.print("||");
  lcd.setCursor(3, 1);
  lcd.print(":");
  lcd.setCursor(12, 1);
  lcd.print(":");
  pinMode(4,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
}



void print_time(char isWhite,unsigned int time)//possible error due to time being a pointer and a global variable
{
  if(!isWhite) // Black
  {
    lcd.setCursor(1,1);
    (time/60)/10!=0?lcd.print((time/60)/10):lcd.print(" ");//if the second digit to the minute part of the timer is 0 then remove so for example 05:34 would become 5:34
    lcd.setCursor(2,1);
    lcd.print((time/60)%10);
    lcd.setCursor(4,1);
    lcd.print((time%60)/10);
    lcd.setCursor(5,1);
    lcd.print((time%60)%10);
  }
  else //White
  {
    lcd.setCursor(10,1);
    (time/60)/10!=0?lcd.print((time/60)/10):lcd.print(" ");//same as above
    lcd.setCursor(11,1);
    lcd.print((time/60)%10);
    lcd.setCursor(13,1);
    lcd.print((time%60)/10);
    lcd.setCursor(14,1);
    lcd.print((time%60)%10);
  }
}


void loop() { 
  set = digitalRead(2);
  if(set==HIGH || !isSet)
  {
    playing_side=WHITE;
    time[BLACK]=600;//initializing time variables 
    time[WHITE]=600;
    isSet=true;
  }

  print_time(BLACK,time[BLACK]);
  print_time(WHITE,time[WHITE]);

  blackButton = digitalRead(4);
  whiteButton = digitalRead(3);
  if(blackButton == HIGH) playing_side=WHITE;
  if(whiteButton == HIGH) playing_side=BLACK;
  
  //Receives input during the 1 second delay
  for (int i=0;i<1000 && !changedSides;i++) //Stops after 1000ms=1s or if someone pressed their button
  {
    
    if(digitalRead(4) == HIGH && playing_side==BLACK) 
      //Black's button starts white's timer
    {
      changedSides = true;
      //time[playing_side]+=2;
      playing_side=WHITE;
    }

    if(digitalRead(3) == HIGH && playing_side==WHITE) 
      //White's button starts white's timer
    {
      changedSides = true;
      //time[playing_side]+=2;
      playing_side=BLACK;
      

    }
    delay(1);
  }
  changedSides?changedSides=false:time[playing_side]--;
  print_time(playing_side,time[playing_side]);
  while((time[BLACK]==0 || time[WHITE]==0))
  {
    lcd.setCursor(0,1);
    time[BLACK]==0?lcd.print("-->WHITE WINS<--"):lcd.print("-->BLACK WINS<--"); //If black's time runs out display White wins, else display black wins
    
    set = digitalRead(2);
    if(set==HIGH)
    {
    playing_side=WHITE;
    time[BLACK]=600;
    time[WHITE]=600;

    lcd.clear(); //clears the display
    lcd.setCursor(0,0); 
    lcd.print(" BLACK || WHITE"); 
    lcd.setCursor(7, 1);
    lcd.print("||");
    lcd.setCursor(3, 1);
    lcd.print(":");
    lcd.setCursor(12, 1);
    lcd.print(":");
    break;

    }
  }
  


}
