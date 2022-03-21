#include <LiquidCrystal.h>

#define BUTTON_ONE 10
#define BUTTON_TWO 9
#define BUTTON_THREE 8

#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

bool clock_mode=false;
bool button_one,button_two,button_three;
bool lbutton_one=false,lbutton_two=false,lbutton_three=false;
float player_one=0,player_two=0;
unsigned long clock_end=0,last_time=0;
unsigned long start_time=0;
bool turn=true; 
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600); 
  pinMode(BUTTON_ONE, INPUT);
  pinMode(BUTTON_TWO, INPUT);
  pinMode(BUTTON_THREE, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  start_time=millis()-last_time;
  last_time=millis();
  button_one=button(BUTTON_ONE,&lbutton_one);
  button_two=button(BUTTON_TWO,&lbutton_two);
  button_three=button(BUTTON_THREE,&lbutton_three);
   
  if(clock_mode){
    if(button_one){
      change_player(true);
    } 
    if(button_two){
      change_player(false);
    }
  }else{
    if(button_one){
      clock_end_time(false);
    } 
    if(button_two){
      clock_end_time(true);
    }
  }
  
  if(button_three){
  	clock_mode=!clock_mode;
    player_two=clock_end;
    player_one=clock_end;
    clock_end=0;
    lcd.clear();
  }
  clock(clock_mode);
  delay(10);
}

void clock(bool status)
{
  if(status){   
    if(turn){
      player_two=player_two-start_time;
    }else{
      player_one=player_one-start_time;
    }
    if(player_one<0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("#02 Win!");
    }else if(player_two<0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("#01 Win!");
    }else{
      lcd.setCursor(0,0);
      lcd.print("#1: "+human_time(player_one)+"         ");
      lcd.setCursor(0,1);
      lcd.print("#2: "+human_time(player_two)+"         ");
    }
     //Serial.println(String(player_one));
  } else{
    lcd.setCursor(0,0);
    lcd.print("Minutos: "+String(clock_end/60000));
  }
}

void clock_end_time(bool status)
{
  if(status){
    clock_end+=60000;
  } else if(clock_end>0){
    clock_end-=60000;
  }
}

bool button(byte button,bool *lastButtonState)
{
  bool state;
  bool buttonState = digitalRead(button);
  state=buttonState;
  if (buttonState != *lastButtonState) {
    state=buttonState;
  } else {
    state=false;
  }
  *lastButtonState = buttonState;
  //Serial.println(String(state));
  return state;
}

void change_player(bool status)
{
  turn=status;
}

String human_time(unsigned long tiempo)
{
  String time;
  unsigned long tp;
  int m,s,h;
  tp = tiempo % 3600000;
  m = int(tp / 60000);
  tp = tp % 60000;
  s = int(tp / 1000);
  tp = tp %  3600;
  h = int(tp / 3600000);
  if (h > 0) {
   	time=String(h)+"h ";
  }
  if (m > 0) {
    time+= String(m)+ "m ";
  }
  if (s > 0) {
    time+= String(s)+"s";
  }
  
  return time;
}
