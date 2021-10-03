
int sensor1 = 2;
int sensor2 = 3;

int valor_sensor1;
int valor_sensor2;
int valor_sensor1_anterior = LOW;

int posicion = 0;
int posicion_anterior = 0;

void setup() {
  
  Serial.begin(9600);
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);

}

void loop() {

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
  Serial.println(posicion);
}
}
