#include <TimerOne.h>
#include <TimerThree.h>
#include <TimerFour.h>

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
#define X_MAX_PIN 2         // PIN para el fin de carrera colocado al inicio del recorrido (1: presionado (true))

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

#define BTN_EN1 31          // Encoder, conectado a GADGETS3D shield S_E1
#define BTN_EN2 33          // Encoder, cconectado a GADGETS3D shield S_E2
#define BTN_ENC 35          // Encoder Click, connected to Gadgets3D shield S_EC

// VARIABLES MEDIDA POSICION
  
int sensor1 = 20;
int sensor2 = 21;

int valor_sensor1; 
int valor_sensor2;
int valor_sensor1_anterior = LOW; 

volatile int posicion = 0;
int posicion_anterior = 0;

bool fc_inic_X = true;
bool fc_fin_X = true;

// VARIABLES MEDIDA TIEMPO 

int horas = 0;   
int minutos = 0;
volatile int segundos = 0;

// VARIABLES MOVIMIENTO MOTOR

int t_mediomicropaso = 0;
int t_mediopaso = 0;
int v = 1;
float vectort_mediomicropaso[] = {0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,973.18,919.12,870.74,827.21,787.82,752.01,719.31,689.34,661.76,636.31,612.75,590.86,570.49,551.47,533.68,517.00,501.34,486.59,472.69,459.56,447.14,435.37,424.21,413.60,403.52,393.91,384.75,376.00,367.65,359.65,352.00,344.67,337.64,330.88,324.39,318.16,312.15,306.37,300.80,295.43,290.25,285.24,280.41,275.74,271.22,266.84,262.61,258.50,254.52,250.67,246.93,243.30,239.77,236.34,233.02,229.78,226.63,223.57,220.59,217.69,214.86,212.10,209.42,206.80,204.25,201.76,199.33,196.95,194.64,192.37,190.16,188.00,185.89,183.82,181.80,179.83,177.89,176.00,174.15,172.33,170.56,168.82,167.11,165.44};
volatile int nivel = LOW; 
volatile int pasos = 0;
float vectort_mediopaso[] = {0,529411.765,264705.882,176470.588,132352.941,105882.353,88235.294,75630.252,66176.471,58823.529,52941.176,48128.342,44117.647,40723.982,37815.126,35294.118,33088.235};

// VARIABLES MAQUINA DE ESTADOS

int inicio = 0;
int fin = 0;
int volatile estado = 0;                               // Variables del estado (valores de 0 a 2)

// VARIABLES ENCODER LCD

bool btn_en1, btn_en2, btn_enc, btn_en1_prev, btn_en2_prev ;       //variables de lectura directa del encoder
bool direccion = false;                                            //false es hacia 0 y true hacia 400
bool derecha, izquierda, pulsador = false;                         // variables de lectura del encoder interpretadas
int i = 0;                                                         // contador de pulsos

void setup() {

  pinMode(BTN_EN1, INPUT_PULLUP);     // Encoder 1
  pinMode(BTN_EN2, INPUT_PULLUP);     // Encoder 2
  pinMode(BTN_ENC, INPUT_PULLUP);     // Encoder Swith
  
  pinMode(X_MIN_PIN, INPUT_PULLUP);   // Fin de carrera inicio X   
  pinMode(X_MAX_PIN, INPUT_PULLUP);   // Fin de carrera final X             

  Timer3.initialize(1000000);              
  Timer3.attachInterrupt(Temporizador);
 
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);

  attachInterrupt(digitalPinToInterrupt(sensor1), encoder, RISING);
  
  lcd.begin(20, 4);   // 20 columnas y 4 filas

  pinMode(X_STEP_PIN , OUTPUT);
  pinMode(X_DIR_PIN , OUTPUT);
  pinMode(X_ENABLE_PIN , OUTPUT);

  btn_en1 , btn_en1_prev = digitalRead(BTN_EN1);
  btn_en2 , btn_en2_prev = digitalRead(BTN_EN2);
  btn_enc = digitalRead(BTN_ENC);
 
  digitalWrite(X_ENABLE_PIN , LOW);

}

////////////////// PROCESOS ENCODER /////////////////////

void leer_encoder()
{
  btn_en1 = digitalRead(BTN_EN1);
  btn_en2 = digitalRead(BTN_EN2);
  digitalWrite(X_DIR_PIN, direccion);

  if (btn_en1 != btn_en1_prev || btn_en2 != btn_en2_prev)
  {
    if (btn_en2 == false & btn_en1 == false & btn_en2_prev == true & btn_en1_prev == false)
    {
      derecha = true;
      izquierda = false;
    }
    else if ( btn_en2 == false & btn_en1 == false & btn_en2_prev == false & btn_en1_prev == true )
    {
      derecha = false;
      izquierda = true;
    }
    else
    {
      derecha = false;
      izquierda = false;
    }
  }
  else
  {
    derecha = false;
    izquierda = false;
  }
  btn_en1_prev = btn_en1;
  btn_en2_prev = btn_en2;
}

void leer_pulso()
{
  btn_enc = digitalRead(BTN_ENC);

  if (btn_enc == false) //Detector de flanco del pulsador
  {
    i++;
  }
  if (i >= 80)
  {
    pulsador = true;
    i = 0;
    delay(200);
  }
  else
  {
    pulsador = false;
  }
}

//////////////////////// ESTADO 0 /////////////////////////

void pantalla_inicio()
{
  lcd.setCursor(2, 0);    // posiciona el cursor en la columna 1 fila 0
  lcd.print("ENSAYO DE MODELO");
  lcd.setCursor(3, 1);    // posiciona el cursor en la columna 1 fila 1
  lcd.print("ANALOGO SIMPLE");
    
  lcd.setCursor(0, 2);
  lcd.print("Velocidad = ");
  lcd.setCursor(12, 2);    
  lcd.print(v);
  lcd.setCursor(15, 2);    
  lcd.print("mm/h");
  lcd.setCursor(0, 3);
  lcd.print("Iniciar experimento");

}

//////////////// ESTADO 1 ////////////////

void seleccionar_velocidad() {

      if (derecha == true and v < 100)
      {
        v = v + 1;
        lcd.setCursor(12,2);
        lcd.print(v);
      }
      else if (izquierda == true and v - 1 >= 0)
      {
        v = v - 1;
        lcd.setCursor(12,2);
        lcd.print(v);
      }

        if (v <10 && v >=0){
        lcd.setCursor(13, 2);
        lcd.print(" ");
        }
        if (v <100 && v >=0){
        lcd.setCursor(14, 2);
        lcd.print(" ");
        }

  t_mediomicropaso = vectort_mediomicropaso[v];     
  Timer1.attachInterrupt(Micropasos);
  Timer1.initialize(t_mediomicropaso);

      if (v <= 17){ 
      Timer4.attachInterrupt(MedioPaso);
      }

  t_mediopaso = vectort_mediopaso[v];  
  Timer4.initialize(t_mediopaso);
            
    if (pulsador == true)
      {
        lcd.clear();
        estado = 2;
      }
}

//////////////// ESTADO 2 ////////////////

void experimento() {

  fc_inic_X = digitalRead(X_MIN_PIN);
  fc_fin_X = digitalRead(X_MAX_PIN);
  
  if (fc_inic_X == true )
  {
    inicio = 1;
    }

  if (fc_fin_X == true )
  {
    fin = 1;
    }

// MOVIMIENTO MOTOR

  if (inicio == 1 && fin == 0) {  
       digitalWrite(X_DIR_PIN , HIGH);
  }
  else {   
       digitalWrite(X_DIR_PIN , LOW);
  }   

// POSICION

  if (inicio == 1) {
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
  }
  
// TIEMPO 
  
  if (inicio == 0 && fin ==0) {
    lcd.setCursor(0, 0);
    lcd.print("MOVIENDO");
    lcd.setCursor(0, 1);
    lcd.print("A CERO");
    }
  else if (inicio == 1 && fin ==0){  
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
  if (inicio == 1 && fin ==0) {//                             <- INTERRUPCION CRONOMETRO
    segundos++;
  }
}

void Micropasos() {
    if (estado == 2){
      if (pasos < 16 || v >= 17){
        digitalWrite(X_STEP_PIN , nivel);
        if (nivel == LOW){       //                             <- INTERRUPCION MOVIMIENTO MOTOR
           nivel = HIGH;
           pasos++;}
        else{
           nivel = LOW;
        }
      }
    }
}

void MedioPaso() {
pasos = 0;
}

void encoder() {

    if (inicio == 1 && fin == 0) {
      valor_sensor2 = digitalRead (sensor2);
      if(valor_sensor2 == LOW){ //                             <- INTERRUPCION ENCODER LEER LINEAS
        posicion++;
      }
      else{
        posicion--;
      }
    }
}

void loop() {
  
  switch (estado){
    case 0:
      pantalla_inicio();
      estado = 1;
      break;
            
    case 1:
      leer_pulso();
      leer_encoder();
      seleccionar_velocidad();
      break;
   
    case 2: 
      experimento();
      break;
        
  }
}

          
  
