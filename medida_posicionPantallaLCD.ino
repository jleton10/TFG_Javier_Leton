
#include <LiquidCrystal.h>   
LiquidCrystal lcd(9, 8, 7, 6, 5, 4); 

int sensor1 = 2;
int sensor2 = 3;

int valor_sensor1;
int valor_sensor2; 
int valor_sensor1_anterior = LOW;

int posicion = 0;
int posicion_anterior = 0;

void setup() {
  
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  lcd.begin(16, 2);
}

void loop() {

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

}
