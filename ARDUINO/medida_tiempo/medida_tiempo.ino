#include <TimerOne.h>

#include <LiquidCrystal.h>   
LiquidCrystal lcd(9, 8, 7, 6, 5, 4); 


int pulsador_inicio = 10;   

int horas = 0;     
int minutos = 0;     
int segundos = 0;    

int inicio = 0;

void setup() 
{
  pinMode(pulsador_inicio, INPUT_PULLUP);             // Pulsador para iniciar el cronometro
  Timer1.initialize(1000000);                //Configura el TIMER en 1 Segundo
  Timer1.attachInterrupt(Temporizador) ;
  lcd.begin(16, 2);                          // Dimensiones de la pantalla LCD
}

void loop() 
{
    lcd. setCursor(0, 0);
    lcd.print("CRONOMETRO");
  
  if (digitalRead(pulsador_inicio) == false )
  {
    inicio = 1;
    }
  
  if (inicio == 0){
    lcd. setCursor(0, 1);
    lcd.print("00:00:00");
    }
  else{  
    if (segundos == 60)    {
    segundos = 0;
    minutos++;           }
    if (minutos == 60)     {
    minutos = 0;
    horas++;             }
    lcd.setCursor(0, 1);
    if (horas < 10)        {
    lcd.print("0");      }
    lcd.print(horas);
    lcd.print(":");
    lcd.setCursor(3, 1);
    if (minutos < 10)      {
    lcd.print("0");      }
    lcd.print(minutos);
    lcd.print(":");
    lcd.setCursor(6, 1);
    if (segundos < 10)     {
    lcd.print("0");        }
    lcd.print(segundos);
  }

}


void Temporizador()
{
  if (inicio == 1) {
    segundos++;
  }
}
  
