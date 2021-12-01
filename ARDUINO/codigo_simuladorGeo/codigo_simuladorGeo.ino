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

int L = 400;                                                       // l es la longitud del husillo, por lo que se trata de la distancia máxima que se puede llevar a cabo

// VARIABLES MEDIDA TIEMPO 

int horas = 0;   
int minutos = 0;
volatile int segundos = 0;

// VARIABLES MOVIMIENTO MOTOR

unsigned long t_mediomicropaso = 0;
unsigned long t_mediopaso = 0;
int v = 100;
int vectort_mediomicropaso[] = {0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
volatile int nivel = LOW; 
volatile int micropasos = 0;
volatile int n_mediospasos = 0;
float avance_mediospasos = 0;
float vectort_mediopaso[] = {0,529411.76,264705.88,176470.59,132352.94,105882.35,88235.29,75630.25,66176.47,58823.53,52941.18,48128.34,44117.65,40723.98,37815.13,35294.12,33088.24,31141.87,29411.76,27863.78,26470.59,25210.08,24064.17,23017.90,22058.82,21176.47,20361.99,19607.84,18907.56,18255.58,17647.06,17077.80,16544.12,16042.78,15570.93,15126.05,14705.88,14308.43,13931.89,13574.66,13235.29,12912.48,12605.04,12311.90,12032.09,11764.71,11508.95,11264.08,11029.41,10804.32,10588.24,10380.62,10181.00,9988.90,9803.92,9625.67,9453.78,9287.93,9127.79,8973.08,8823.53,8678.88,8538.90,8403.36,8272.06,8144.80,8021.39,7901.67,7785.47,7672.63,7563.03,7456.50,7352.94,7252.22,7154.21,7058.82,6965.94,6875.48,6787.33,6701.41,6617.65,6535.95,6456.24,6378.45,6302.52,6228.37,6155.95,6085.19,6016.04,5948.45,5882.35,5817.71,5754.48,5692.60,5632.04,5572.76,5514.71,5457.85,5402.16,5347.59,5294.12};
int di_X, df_X = 0;

// VARIABLES MAQUINA DE ESTADOS

int inicio = 0;
int inicio_experimento = 0;
int fin = 0;
int volatile estado = 0;                               // Variables del estado (valores de 0 a 2)

int fila, columna = 0;                                             // Variable de fila en la pantalla lcd

// VARIABLES ENCODER LCD

bool btn_en1, btn_en2, btn_enc, btn_en1_prev, btn_en2_prev ;       //variables de lectura directa del encoder
bool direccion = false;                                            //false es hacia 0 y true hacia 400
bool derecha, izquierda, pulsador = false;                         // variables de lectura del encoder interpretadas
int i = 0;                                                         // contador de pulsos

// SIGNOS CREADOS

byte empty[8] =
{
  B00000,
  B00100,
  B01010,
  B10001,
  B10001,
  B01010,
  B00100,
};
byte full[8] =
{
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
  B00100,
};
byte micro[8] =
{
  B00000,
  B10001,
  B10001,
  B11011,
  B10101,
  B10000,
  B10000,
};
byte arrow[8] =
{
  B00000,
  B00001,
  B00001,
  B00001,
  B01001,
  B11111,
  B01000,
};

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

  lcd.createChar(0, empty);   // 0: numero de carácter; empty: matriz que contiene los pixeles del carácter
  lcd.createChar(1, full);    // 1: numero de carácter; full: matriz que contiene los pixeles del carácter
  lcd.createChar(2, micro);   // 2: numero de carácter; micro: matriz que contiene los pixeles del carácter
  lcd.createChar(3, arrow);   // 3: número de carácter; arrow: matriz que contiene los pixeles del carácter

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

  lcd.setCursor(0, 3);    // posiciona el cursor en la columna 1 fila 3
  lcd.print("Iniciar Experimento");

  if (pulsador == true ) { 
    lcd.clear();
    estado = 1;
  }
  
}


//////////////// ESTADO 1 ////////////////

void menu()
{
  fila, columna = 0;
  di_X = 0;
  df_X = 0;
  v = 1;

  lcd.setCursor(1, 0);
  lcd.print("dist.iniX");
  lcd.setCursor(12, 0);
  lcd.print(di_X);
  lcd.setCursor(18, 0);
  lcd.print("mm");
  lcd.setCursor(1, 1);
  lcd.print("dist.finX");
  lcd.setCursor(12, 1);
  lcd.print(df_X);
  lcd.setCursor(18, 1);
  lcd.print("mm");
  lcd.setCursor(1, 2);
  lcd.print("velocidad");
  lcd.setCursor(12, 2);
  lcd.print(v);
  lcd.setCursor(16, 2);
  lcd.print("mm/h");
  lcd.setCursor(1, 3);
  lcd.print("Iniciar experimento");
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(0));
  lcd.setCursor(0, 3);
  lcd.write(byte(0));
  lcd.setCursor(0, fila);
  lcd.write(byte(1));
}

//////////////// ESTADO 2 ////////////////

void DefinicionDeVariables()
{
  switch (fila)//fila es la variable vertical de la pantalla, indica que variable se esta manipulando
  {
    case 0: //modificar la distancia inicial
      if (pulsador == true  and columna < 3) //columna es una variable que avanza en horizontal por la pantalla, cuando vale 0 podemos cambia en vertical con derecha o izquierda, cuando no aumentamos la variable con la que estemos trabajando
      {
        columna++;
      }
      else if (pulsador == true and columna == 3) //hay 4 opciones
      {
        columna = 0;
      }
      switch (columna)
      {
        case 0: //opción 1: seleccionar variable
          if (derecha == true )
          {
            fila = 1;
            lcd.setCursor(0, 0);
            lcd.write(byte(0));
            lcd.setCursor(0, 1);
            lcd.write(byte(1));
            lcd.setCursor(0, 2);
            lcd.write(byte(0));
            lcd.setCursor(0, 3);
            lcd.write(byte(0));
          }
          else if (izquierda == true)
          {
            fila = 3;
            lcd.setCursor(0, 0);
            lcd.write(byte(0));
            lcd.setCursor(0, 1);
            lcd.write(byte(0));
            lcd.setCursor(0, 2);
            lcd.write(byte(0));
            lcd.setCursor(0, 3);
            lcd.write(byte(1));
          }
          break;
        case 1: //opción 2: ir sumando y restando de 1 en 1
          if (derecha == true and di_X + 1 < L)
          {
            di_X = di_X + 1;
            lcd.setCursor(12, 0);
            lcd.print("   ");
            lcd.setCursor(12, 0);
            lcd.print(di_X);
          }
          else if (izquierda == true and di_X - 1 >= 0)
          {
            di_X = di_X - 1;
            lcd.setCursor(12, 0);
            lcd.print("   ");
            lcd.setCursor(12, 0);
            lcd.print(di_X);
          }
          break;
        case 2: //opción 3: ir sumando y restando de 10 en 10
          if (derecha == true and di_X + 10 < L)
          {
            di_X = di_X + 10;
            lcd.setCursor(12, 0);
            lcd.print("   ");
            lcd.setCursor(12, 0);
            lcd.print(di_X);
          }
          else if (izquierda == true and di_X - 10 >= 0)
          {
            di_X = di_X - 10;
            lcd.setCursor(12, 0);
            lcd.print("   ");
            lcd.setCursor(12, 0);
            lcd.print(di_X);
          }
          break;
        default: //opción 4: ir sumando y restando de 100 en 100
          if (derecha == true and di_X + 100 < L)
          {
            di_X = di_X + 100;
            lcd.setCursor(12, 0);
            lcd.print("   ");
            lcd.setCursor(12, 0);
            lcd.print(di_X);
          }
          else if (izquierda == true and di_X - 100 >= 0)
          {
            di_X = di_X - 100;
            lcd.setCursor(12, 0);
            lcd.print("   ");
            lcd.setCursor(12, 0);
            lcd.print(di_X);
          }
          break;
      }
      break;
    case 1: //modificar la distancia final
      if (pulsador == true  and columna < 3) //columna es una variable que avanza en horizontal por la pantalla, cuando vale 0 podemos cambia en vertical con derecha o izquierda, cuando no aumentamos la variable con la que estemos trabajando
      {
        columna++;
      }
      else if (pulsador == true and columna == 3)
      {
        columna = 0;
      }
      switch (columna)
      {
        case 0: //opción 1: seleccionar variable
          if (derecha == true )
          {
            fila = 2;
            lcd.setCursor(0, 0);
            lcd.write(byte(0));
            lcd.setCursor(0, 1);
            lcd.write(byte(0));
            lcd.setCursor(0, 2);
            lcd.write(byte(1));
            lcd.setCursor(0, 3);
            lcd.write(byte(0));
          }
          else if (izquierda == true)
          {
            fila = 0;
            lcd.setCursor(0, 0);
            lcd.write(byte(1));
            lcd.setCursor(0, 1);
            lcd.write(byte(0));
            lcd.setCursor(0, 2);
            lcd.write(byte(0));
            lcd.setCursor(0, 3);
            lcd.write(byte(0));
          }
          break;
        case 1: //opción 2: ir sumando y restando de 1 en 1
          if (derecha == true and df_X + 1 < L)
          {
            df_X = df_X + 1;
            lcd.setCursor(12, 1);
            lcd.print("   ");
            lcd.setCursor(12, 1);
            lcd.print(df_X);
          }
          else if (izquierda == true and df_X - 1 >= 0)
          {
            df_X = df_X - 1;
            lcd.setCursor(12, 1);
            lcd.print("   ");
            lcd.setCursor(12, 1);
            lcd.print(df_X);
          }
          break;
        case 2: //opción 3: ir sumando y restando de 10 en 10
          if (derecha == true and df_X + 10 < L)
          {
            df_X = df_X + 10;
            lcd.setCursor(12, 1);
            lcd.print("   ");
            lcd.setCursor(12, 1);
            lcd.print(df_X);
          }
          else if (izquierda == true and df_X - 10 >= 0)
          {
            df_X = df_X - 10;
            lcd.setCursor(12, 1);
            lcd.print("   ");
            lcd.setCursor(12, 1);
            lcd.print(df_X);
          }
          break;
        default://opción 4: ir sumando y restando de 100 en 100
          if (derecha == true and df_X + 100 < L)
          {
            df_X = df_X + 100;
            lcd.setCursor(12, 1);
            lcd.print("   ");
            lcd.setCursor(12, 1);
            lcd.print(df_X);
          }
          else if (izquierda == true and df_X - 100 >= 0)
          {
            df_X = df_X - 100;
            lcd.setCursor(12, 1);
            lcd.print("   ");
            lcd.setCursor(12, 1);
            lcd.print(df_X);
          }
          break;
      }
      break;
    case 2: //modificar la velocidad
      if (pulsador == true  and columna < 1) //columna es una variable que avanza en horizontal por la pantalla, cuando vale 0 podemos cambia en vertical con derecha o izquierda, cuando no aumentamos la variable con la que estemos trabajando
      {
        columna++;
      }
      else if (pulsador == true and columna == 1) //hay 2 opciones
      {
        columna = 0;
      }
      switch (columna)
      {
        case 0: //opción 1: seleccionar variable
          if (derecha == true )
          {
            fila = 3;
            lcd.setCursor(0, 0);
            lcd.write(byte(0));
            lcd.setCursor(0, 1);
            lcd.write(byte(0));
            lcd.setCursor(0, 2);
            lcd.write(byte(0));
            lcd.setCursor(0, 3);
            lcd.write(byte(1));
          }
          else if (izquierda == true)
          {
            fila = 1;
            lcd.setCursor(0, 0);
            lcd.write(byte(0));
            lcd.setCursor(0, 1);
            lcd.write(byte(1));
            lcd.setCursor(0, 2);
            lcd.write(byte(0));
            lcd.setCursor(0, 3);
            lcd.write(byte(0));
          }
          break;
        default: //opción 2: modificar la velocidad de 1 en 1 o de 10 en 10 en función de la velocidad
          if (izquierda == true and v - 1 >= 0)
          {
              v = v - 1;
              lcd.setCursor(12, 2);
              lcd.print(v);
          }
          else if (derecha == true and v < 100)
          {
              v = v + 1;
              lcd.setCursor(12, 2);
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
          
          break;
      }
      break;
      
    default: //iniciar experimento
    
      if (pulsador == true)
      {
          t_mediomicropaso = ((unsigned long)vectort_mediomicropaso[v]);     
          Timer1.attachInterrupt(Micropasos);
          Timer1.initialize(t_mediomicropaso);
          Timer4.attachInterrupt(MedioPaso);
          t_mediopaso = ((unsigned long)(vectort_mediopaso[v]));  
          Timer4.initialize(t_mediopaso);
      
          lcd.clear();
          estado = 3;
        
      }
      else if (derecha == true )
      {
        fila = 0;
        columna = 0;
        lcd.setCursor(0, 0);
        lcd.write(byte(1));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(0, 2);
        lcd.write(byte(0));
        lcd.setCursor(0, 3);
        lcd.write(byte(0));
      }
      else if (izquierda == true)
      {
        fila = 2;
        columna = 0;
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(0, 2);
        lcd.write(byte(1));
        lcd.setCursor(0, 3);
        lcd.write(byte(0));
      }
      break;
  }
}

//////////////// ESTADO 3 ////////////////

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

  if (inicio == 1) {
    if (di_X == 0) {
      inicio_experimento = 1;
    }
    else if (di_X == (0.0025*n_mediospasos)) {
      inicio_experimento = 1;
    }
  }  

// MOVIMIENTO MOTOR

  if (inicio == 1 && fin == 0) {  
       digitalWrite(X_DIR_PIN , HIGH);
  }
  else { 
       digitalWrite(X_DIR_PIN , LOW);
  }   

// POSICION

  if (inicio_experimento == 1) {
    
    lcd.setCursor(0, 0);
    lcd.print("LINEAS: ");

    lcd.setCursor(7, 0);
    lcd.print(lineas);

    avance_lineas = 0.169*lineas;
    
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
  else if (inicio_experimento == 1 && fin ==0 && (avance_mediospasos < df_X)){  
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
  
  if (inicio_experimento == 1 && fin ==0) {//                             <- INTERRUPCION CRONOMETRO
    segundos++;
  }
  
}

void Micropasos() {
    if (estado == 3 && (avance_mediospasos < df_X) && fin == 0){
      if (micropasos < 16){
        digitalWrite(X_STEP_PIN , nivel);
        if (nivel == LOW){       //                             <- INTERRUPCION MOVIMIENTO MOTOR TIEMPO MEDIO MICROPASO
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
  
    if (inicio == 1 && fin ==0 && (avance_mediospasos < df_X)){ //                             <- INTERRUPCION MOVIMIENTO MOTOR TIEMPO TOTAL MEDIO PASO
    n_mediospasos++;
    }
    
}

void encoder() {

    if (inicio_experimento == 1 && fin == 0) {
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
      leer_pulso();
      leer_encoder();
      pantalla_inicio();
      break;
            
    case 1:
      menu();
      estado = 2;
      break;

    case 2:
      leer_pulso();
      leer_encoder();
      DefinicionDeVariables();
      break;
   
    case 3: 
      experimento();
      break;    
  }
  
}

          
  
