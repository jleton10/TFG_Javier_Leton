#include <TimerOne.h>

#include <LiquidCrystal.h>   
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 

int inicio = 8;   

int horas = 0;     
int minutos = 0;     
int segundos = 0;    
int decimas = 0;           
int milisegundos = 0;

int a = 0;

void setup() 
{
  pinMode(inicio, INPUT_PULLUP);             // Pulsador para iniciar el cronometro
  Timer1.initialize(1000000);                //Configura el TIMER en 1 Segundo
  Timer1.attachInterrupt(Temporizador) ;
  lcd.begin(16, 2);                          // Dimensiones de la pantalla LCD
}

void loop() 
{

  lcd. setCursor(0, 0);
  lcd.print("CRONOMETRO");
  lcd. setCursor(0, 1);
  lcd.print("00:00:00");  
  
  if (digitalRead(inicio) == false )
  {
    a = 1;
    while (a == 1)
    {
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
  }

void Temporizador()
{
  if (a == 1) {
    segundos++;
  }
}
  
