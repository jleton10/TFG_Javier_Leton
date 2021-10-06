#include <TimerOne.h>

#include <LiquidCrystal.h>   
LiquidCrystal lcd(9, 8, 7, 6, 5, 4); 

// VARIABLES MEDIDA POSICION
  
int sensor1 = 2;
int sensor2 = 3;

int valor_sensor1; 
int valor_sensor2; 
int valor_sensor1_anterior = LOW; 

volatile int posicion = 0;
volatile int posicion_anterior = 0;

// VARIABLES MEDIDA TIEMPO

int pulsador_inicio = 10;   

int horas = 0;     
int minutos = 0;     
int segundos = 0;    

int inicio = 0;

void setup() {
  pinMode(pulsador_inicio, INPUT_PULLUP);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(Temporizador);
  
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  lcd.begin(16, 2);

  Serial.begin(9600);
}

void loop() {

// POSICION

  lcd.setCursor(0, 0);
  lcd.print("POSICION: ");

  posicion_anterior = posicion;

  valor_sensor1 = digitalRead (sensor1);
  
  if ((valor_sensor1_anterior == HIGH) && (valor_sensor1 == LOW)) { 
    valor_sensor2 = digitalRead (sensor2);
    if(valor_sensor2 == LOW){ 
      posicion--;
    }
    else{ 
      posicion++; 
    }
  }
  
  valor_sensor1_anterior = valor_sensor1;

  if (posicion_anterior != posicion) {
  lcd.setCursor(10, 0);
  lcd.print(posicion);
  
  if (posicion <10 && posicion >=0){
  lcd.setCursor(11, 0);
  lcd.print(" ");
  }
  if (posicion <100 && posicion >=0){
  lcd.setCursor(12, 0);
  lcd.print(" ");
  }
 
  }


// TIEMPO 

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

    void Temporizador() {
  if (inicio == 1) {
    segundos++;
  }
}
