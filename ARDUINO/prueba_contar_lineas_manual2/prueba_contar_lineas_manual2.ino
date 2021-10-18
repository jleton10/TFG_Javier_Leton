#include <TimerOne.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(16, 17, 23, 25, 27, 29);

//Pines de la shield
#define LCD_PINS_RS 16      // LCD control conectado a GADGETS3D  shield LCDRS
#define LCD_PINS_ENABLE 17  // LCD enable pin conectado a GADGETS3D shield LCDE
#define LCD_PINS_D4 23      // LCD signal pin, conectado a GADGETS3D shield LCD4
#define LCD_PINS_D5 25      // LCD signal pin, conectado a GADGETS3D shield LCD5
#define LCD_PINS_D6 27      // LCD signal pin, conectado a GADGETS3D shield LCD6
#define LCD_PINS_D7 29      // LCD signal pin, conectado a GADGETS3D shield LCD7
#define X_MIN_PIN 3         // PIN para el fin de carrera colocado al inicio del recorrido (1: presionado (true))

// VARIABLES MEDIDA POSICION
  
int sensor1 = 20;
int sensor2 = 21;

int valor_sensor1; 
int valor_sensor2;
int valor_sensor1_anterior = LOW; 

volatile int posicion = 0;
int posicion_anterior = 0;

bool fc_inic_X = true;

// VARIABLES MEDIDA TIEMPO

//int pulsador_inicio = 10;   

int horas = 0;     
int minutos = 0;     
volatile int segundos = 0;    

int inicio = 0;

void setup() {
  
  pinMode(X_MIN_PIN, INPUT_PULLUP);   // Fin de carrera inicio X            
  Timer1.initialize(1000000);                
  Timer1.attachInterrupt(Temporizador);
  
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);

  attachInterrupt(digitalPinToInterrupt(sensor1), encoder, RISING);
  
  lcd.begin(20, 4);   // 20 columnas y 4 filas
  
}

void loop() {

  fc_inic_X = digitalRead(X_MIN_PIN);

  if (fc_inic_X == true )
  {
    inicio = 1;
    }

// POSICION

  lcd.setCursor(0, 0);
  lcd.print("LINEAS: ");

  if (posicion_anterior != posicion) {
  lcd.setCursor(7, 0);
  lcd.print(posicion);

  if (posicion <10 && posicion >=0){
  lcd.setCursor(8, 0);
  lcd.print(" ");
  }
  if (posicion <100 && posicion >=0){
  lcd.setCursor(9, 0);
  lcd.print(" ");
  }
  if (posicion <1000 && posicion >=0){
  lcd.setCursor(10, 0);
  lcd.print(" ");
  }
    
  }

// TIEMPO 
  
  if (inicio == 0){
    lcd. setCursor(0, 2);
    lcd.print("00:00:00");
    }
  else{  
      if (segundos == 60)    {
      segundos = 0;
      minutos++;           }
      if (minutos == 60)     {
      minutos = 0;
      horas++;             }
      lcd.setCursor(0, 2);
      if (horas < 10)        {
      lcd.print("0");      }
      lcd.print(horas);
      lcd.print(":");
      lcd.setCursor(3, 2);
      if (minutos < 10)      {
      lcd.print("0");      }
      lcd.print(minutos);
      lcd.print(":");
      lcd.setCursor(6, 2);
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

void encoder() {
  if (inicio == 1) {
    valor_sensor2 = digitalRead (sensor2);
    if(valor_sensor2 == LOW){ 
      posicion++;
    }
    else{ 
      posicion--;
    }
  }
}    
  
