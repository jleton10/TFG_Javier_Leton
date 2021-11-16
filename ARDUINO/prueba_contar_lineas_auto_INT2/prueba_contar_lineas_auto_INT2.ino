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

volatile int lineas = 0;
float avance_lineas = 0;
int posicion_anterior = 0;

bool fc_inic_X = true;
bool fc_fin_X = true;

// VARIABLES MEDIDA TIEMPO 

int horas = 0;   
int minutos = 0;
volatile int segundos = 0;

// VARIABLES MOVIMIENTO MOTOR

unsigned long t_mediomicropaso = 0;
unsigned long t_mediopaso = 0;
int v = 0;
float vectort_mediomicropaso[] = {0,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
volatile int nivel = LOW; 
volatile int micropasos = 0;
volatile int n_mediospasos = 0;
float avance_mediospasos = 0;
float vectort_mediopaso[] = {0,9000000,4500000,3000000,2250000,1800000,1500000,1285714.286,1125000,1000000,900000,818181.8182,750000,692307.6923,642857.1429,600000,562500,529411.7647,500000,473684.2105,450000,428571.4286,409090.9091,391304.3478,375000,360000,346153.8462,333333.3333,321428.5714,310344.8276,300000,290322.5806,281250,272727.2727,264705.8824,257142.8571,250000,243243.2432,236842.1053,230769.2308,225000,219512.1951,214285.7143,209302.3256,204545.4545,200000,195652.1739,191489.3617,187500,183673.4694,180000,176470.5882,173076.9231,169811.3208,166666.6667,163636.3636,160714.2857,157894.7368,155172.4138,152542.3729,150000,147540.9836,145161.2903,142857.1429,140625,138461.5385,136363.6364,134328.3582,132352.9412,130434.7826,128571.4286,126760.5634,125000,123287.6712,121621.6216,120000,118421.0526,116883.1169,115384.6154,113924.0506,112500,111111.1111,109756.0976,108433.7349,107142.8571,105882.3529,104651.1628,103448.2759,102272.7273,101123.5955,100000,98901.0989,97826.08696,96774.19355,95744.68085,94736.84211,93750,92783.50515,91836.73469,90909.09091,90000};

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

    if (pulsador == true){
 
    t_mediomicropaso = ((unsigned long)vectort_mediomicropaso[v]);     
    Timer1.attachInterrupt(Micropasos);
    Timer1.initialize(t_mediomicropaso);
    Timer4.attachInterrupt(MedioPaso);
    t_mediopaso = ((unsigned long)(vectort_mediopaso[v]));  
    Timer4.initialize(t_mediopaso);
      
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

    lcd.setCursor(7, 0);
    lcd.print(lineas);

    avance_lineas = 0.226*lineas;
    
    lcd.setCursor(11, 0);
    lcd.print("->"); 
    lcd.setCursor(14, 0);
    lcd.print(avance_lineas);
    lcd.setCursor(18, 0);
    lcd.print("mm"); 
     
      if (lineas <10 && lineas >=0){
      lcd.setCursor(8, 0);
      lcd.print(" ");
      }
      if (lineas <100 && lineas >=0){
      lcd.setCursor(9, 0);
      lcd.print(" ");
      }
      if (lineas <1000 && lineas >=0){
      lcd.setCursor(10, 0);
      lcd.print(" ");
      }
    
    lcd.setCursor(0, 2);
    lcd.print("Mediospasos: ");
    lcd.setCursor(12, 2);    
    lcd.print(n_mediospasos);

    avance_mediospasos = (0.0025*n_mediospasos);

    lcd.setCursor(0, 3);
    lcd.print("Avance: ");
    lcd.setCursor(7, 3);    
    lcd.print(avance_mediospasos);
    lcd.setCursor(11, 3);
    lcd.print("mm");
      
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
      if (micropasos < 16){
        digitalWrite(X_STEP_PIN , nivel);
        if (nivel == LOW){       //                             <- INTERRUPCION MOVIMIENTO MOTOR
           nivel = HIGH;
           micropasos++;}
        else{
           nivel = LOW;
        }
      }
    }
}

void MedioPaso() {
  
  micropasos = 0;
  
    if (inicio == 1 && fin ==0){ 
    n_mediospasos++;
    }
    
}

void encoder() {

    if (inicio == 1 && fin == 0) {
      valor_sensor2 = digitalRead (sensor2);
      if (valor_sensor2 == LOW){ //                             <- INTERRUPCION ENCODER LEER LINEAS
        lineas++;
      }
      else{
        lineas--;
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
      leer_pulso();        //                                   <- MAQUINA DE ESTADOS
      leer_encoder();
      seleccionar_velocidad();
      break;
   
    case 2: 
      experimento();
      break;    
  }
  
}

          
  
