#include "msp.h"
#include "LCDMENU.txt"
#include "LCD_INIT.txt"
#include "stdio.h"
void reset();
#include "MENU_KEYPAD.txt"
void LCD_init();                            //gets LCD cursor blinking
void PinDeclaration();                      //declare pins
void main_menu();                                //main menu
int door_menu();
void door_open();
void door_closed();
int door_flag;
int door_variable;
int Door_Menu_Keypad();                                    //Reads the key pad values
enum states{
    mainmenu,
    door,
    lights,
    motor
};
/**
 * This Program
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    PinDeclaration();                                //set up my pins
    LCD_init();
enum states state= mainmenu;
int main_menu_keypad;
int door_check;                                     //prevents door menu from continuously prining, might have to figure out how to update later
int door_menu_keypad;
main_menu();
while(1){

main_menu_keypad=Menu_Keypad();
//door_menu_keypad=Door_Menu_Keypad();
switch (state){
case mainmenu:
    //Menu_Keypad();                              used for printing what value we entered
    if (main_menu_keypad==1){
        state=door;
    }
    if(main_menu_keypad==2){
        state=motor;
    }
    if(main_menu_keypad==3){
        state=lights;
    }
    break;
case door:
   door_menu_keypad=Door_Menu_Keypad();
    if(door_check!=1){
    LCD_init();
    door_check=door_menu();
    }
    if (door_check==1)
    {

        door_check=1;
        if(door_menu_keypad==1)
        {
//            delay_ms(10);
            door_open();
        }
        if(door_menu_keypad==2)
        {
//            delay_ms(10);
            door_closed();
        }

    }
    state=door;
    break;
case motor:
    LCD_init();
    delay_ms(10);
    dataWrite('M');
    state=motor;
    break;
case lights:
    LCD_init();
    delay_ms(10);
    dataWrite('L');
    state=lights;
    break;
}
}

}
void PinDeclaration()
{
               SysTick -> CTRL = 0;                    //Systic Timer
               SysTick -> LOAD = 0x00FFFFFF;
               SysTick -> VAL = 0;
               SysTick -> CTRL = 0x00000005;
//Following for LCD************************************************************************************************

P6->SEL0 &= BIT7;                         //DB pins for the LCD screen
P6->SEL1 &= BIT7;
P6->DIR  |= BIT7;
P6->OUT &= ~BIT7;

P6->SEL0 &= BIT6;
P6->SEL1 &= BIT6;
P6->DIR  |= BIT6;
P6->OUT &= ~BIT6;

P6->SEL0 &= BIT5;
P6->SEL1 &= BIT5;
P6->DIR  |= BIT5;
P6->OUT &= ~BIT5;

P6->SEL0 &= BIT4;
P6->SEL1 &= BIT4;
P6->DIR  |= BIT4;
P6->OUT &= ~BIT4;

P1->SEL0 &= BIT5;                                //Enable Pin
P1->SEL1 &= BIT5;
P1->DIR  |= BIT5;
P1->OUT &= ~BIT5;

    P1->SEL0 &= BIT6;                          //Rs
    P1->SEL1 &= BIT6;
    P1->DIR  |= BIT6;
    P1->OUT &=~BIT6;
//LCD done ******************************************************************************************

//Keypad Initialization**************************************************************************************************************
       P4->SEL0 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);                              //Set up button pin to IN
        P4->SEL1 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
        P4->DIR &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
        P4->REN |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);                           //default state is off of a button
        P4->OUT |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
//Keypad initialization*****************************************************************************************************************

//Door LED initialization****************************************************************************
           P3->SEL0 &= BIT6;                          //Green
           P3->SEL1 &= BIT6;
           P3->DIR  |= BIT6;
           P3->OUT &=~BIT6;

                     P3->SEL0 &= BIT7;                          //Red
                     P3->SEL1 &= BIT7;
                     P3->DIR  |= BIT7;
                     P3->OUT &=~BIT7;
//***************************************************************************************************
}

void reset()
{
    P4->DIR &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P4->OUT |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
}
int door_menu()
{
    int j=1;
    char doormenu[]="   Door Menu        ";
         char open[]="1. Open         ";
          char close[]="2. Close                ";
          char blank[]="         ";
          int i=0;
          LCD_init();   //lcd cursor blink
          while(!(doormenu[i]=='\0')){                            //print my name until null
          dataWrite(doormenu[i]);
          i++;
          }
          i=0;
          while (!(close[i]=='\0'))                                 //print EGR until null
          {
              dataWrite(close[i]);
              i++;
          }
          i=0;
          while(!(open[i]=='\0'))                             //print partner's name until null
          {
              dataWrite(open[i]);
              i++;
          }
          i=0;
          while(!(blank[i]=='\0'))                                   //print 226 until null
                {
                    dataWrite(blank[i]);
                    i++;
                }
          return j;
}
void door_open()
{
    P3->OUT |= BIT6;
    P3->OUT &=~BIT7;
    //servo open
    printf("Open\n");
}
void door_closed()
{
       P3->OUT |= BIT7;
       P3->OUT &=~BIT6;
       //servo closed
       printf("Close\n");
}
int Door_Menu_Keypad()                                    //Reads the key pad values
{
//    int door_variable;
    P4->DIR |= BIT1;                              //set bit 1 to output
    P4->OUT &= ~BIT1;                             //send out a low


   if(!( P4->IN & BIT4)){
       while(!(P4->IN & BIT4)){}
       door_variable= 11;                                 //returns a flag for #
       door_flag=1;
       return door_variable;
   }

   if(!( P4->IN & BIT5)){
       while(!(P4->IN & BIT5)){}
       door_variable= 9;
       door_flag=1;
       return door_variable;
   }
   if(!( P4->IN & BIT6)){
       while(!(P4->IN & BIT6)){}
       door_variable= 6;
       door_flag=1;
       return door_variable;
   }
   if(!( P4->IN & BIT7)){
       while(!(P4->IN & BIT7)){}
       door_variable= 3;
       door_flag=1;
       return door_variable;
   }
           reset();                                //reset columns

     P4->DIR |= BIT2;                              //set bit 2 to output
     P4->OUT &= ~BIT2;                             //send out a low

     if(!( P4->IN & BIT4)){
         while(!(P4->IN & BIT4)){}
          door_variable= 0;
          door_flag=1;
          return door_variable;
     }
      if(!( P4->IN & BIT5)){
          while(!(P4->IN & BIT5)){}
          door_variable= 8;
          door_flag=1;
          return door_variable;
      }
      if(!( P4->IN & BIT6)){
          while(!(P4->IN & BIT6)){}
          door_variable= 5;
          door_flag=1;
          return door_variable;
      }
      if(!( P4->IN & BIT7)){
          while(!(P4->IN & BIT7)){}
          door_variable= 2;
          door_flag=1;
          return door_variable;
      }

              reset();                              //reset columns


       P4->DIR |= BIT3;                              //set bit 3 to output
       P4->OUT &= ~BIT3;                             //send out a low

       if(!( P4->IN & BIT4)){
           while(!(P4->IN & BIT4)){}
              door_variable= 10;                            //returns a flag for *
              door_flag=1;
              return door_variable;
       }
          if(!( P4->IN & BIT5)){
              while(!(P4->IN & BIT5)){}
              door_variable= 7;
              door_flag=1;
              return door_variable;
          }
          if(!( P4->IN & BIT6)){
              while(!(P4->IN & BIT6)){}
              door_variable= 4;
              door_flag=1;
              return door_variable;
          }
          if(!( P4->IN & BIT7)){
              while(!(P4->IN & BIT7)){}
              door_variable= 1;
              door_flag=1;
              return door_variable;
          }

                  reset();                              //reset columns

                  if ((door_flag!=1))    {                      //if none are pressed return -1
                      door_variable=-1;
                      return door_variable;
                  }
    return door_variable;
 }
