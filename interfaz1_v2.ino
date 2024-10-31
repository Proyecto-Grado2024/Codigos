#include <Nextion.h>
//#include <DHT.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include<OneWire.h>
#include<DallasTemperature.h>

//---------------variables para la tempertura y humedad----------------------------
//#define DHT1 10
//#define DHT1TYPE DHT22
//DHT dht(DHT1,DHT22);
//float t;
//float h;
OneWire psen(10);
DallasTemperature sensor(&psen);
float t;

char temp[10];
char hume[10];
char c_agua[10];
uint32_t tmax;
uint32_t tmin;
uint32_t hmax;
uint32_t hmin;

const int sensorPin=A1;
int sensorValue =0;
int hs;
const int valorair=514;
const int valoragu=329; 
//..................................................................................

//--------------variables para el flujo de agua-----------------------------------
//const int sagua=2;
//volatile double clitros;
//float fagua;
//float pulsosfagua=450;
//float clitros=0.0;

volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion=9.66; //para convertir de frecuencia a caudal
float volumen=0;
long dt=0; //variación de tiempo por cada bucle
long t0=0; //millis() del bucle anterior
//................................................................................

//--------------declarar objetos nextion------------------------------------------
NexText tt             = NexText(1,6,"ttemp");
NexText th             = NexText(1,7,"thume");
NexText tf             = NexText(1,8,"fagua");
//paginas deNexButton
//1ra pagina de nextion
NexButton bConf        = NexButton(1,1,"bConf");
//2da pagina de nextion
NexButton bATemp       = NexButton(2,3,"bATemp");
NexButton bAHum        = NexButton(2,4,"bAHum");
NexButton bAVen        = NexButton(2,5,"bAVen");
NexButton bCerrar      = NexButton(2,1,"bCerrar");
NexButton bAtras       = NexButton(2,2,"bAtras");
//3ra pagina de nextion
NexButton bAtrasp3     = NexButton(3,1,"bAtrasp3");
NexButton bSalirp3     = NexButton(3,2,"bSalirp3");
NexButton bGuardarp3   = NexButton(3,3,"bGuardarp3");
//4ta pagina de nextion
NexButton bAtrasp4     = NexButton(4,1,"bAtrasp4");
NexButton bSalirp4     = NexButton(4,2,"bSalirp4");
NexButton bGuardarp4   = NexButton(4,3,"bGuardarp4");
//5ta pagina de nextion
NexButton bAtrasp5     = NexButton(5,1,"bAtrasp5");
NexButton bSalirp5     = NexButton(5,2,"bSalirp5");
NexDSButton briego     = NexDSButton(5,3,"btRiego");
NexDSButton bvent      = NexDSButton(5,4,"btVent");

//para la temperatura maxima
NexNumber nVerTemMax   = NexNumber(3,8,"nVerTemMax");
NexNumber nSetTemMax   = NexNumber(3,6,"nSetTemMax");
NexProgressBar jTempMax = NexProgressBar(3,4,"jTempMax");
//para la temperatura minima
NexNumber nVerTemMin   = NexNumber(3,9,"nVerTemMin");
NexNumber nSetTemMin   = NexNumber(3,7,"nSetTemMin");
NexProgressBar jTempMin = NexProgressBar(3,5,"jTempMin");
//para la humedad maxima
NexNumber nVerHumeMax   = NexNumber(4,6,"nVerHumeMax");
NexNumber nSetHumeMax   = NexNumber(4,4,"nSetHumeMax");
NexProgressBar jHumeMax = NexProgressBar(4,8,"jHumeMax");
//para la humedad minima
NexNumber nVerHumeMin   = NexNumber(4,7,"nVerHumeMin");
NexNumber nSetHumeMin   = NexNumber(4,5,"nSetHumeMin");
NexProgressBar jHumeMin = NexProgressBar(4,9,"jHumeMin");

int a=0;
int b=0;
int c=0;
int d=0;
int g=0;
int rv=0;
int rv2=0;
int r=0;
int v=0;
int ais=0;
int ttmax;
int ttmin;
int hhmx;
int hhmin;

int tmaxa;
int tmina;
int hmaxa;
int hmina;


unsigned long tiempoInicio;
unsigned long ttmx=0;
unsigned long tiempomx;
unsigned long ttmn=0;
unsigned long tiempomn;
unsigned long ths=0;
unsigned long tiempohs;

NexTouch *next_listen_list[]=
{
 //-----botones 1ra pagina
 &bConf,
 //-----botones 2da pagina
 &bATemp,
 &bAHum,
 &bAVen,
 &bCerrar,
 &bAtras,
 //------botones 3ra pagina
 &bAtrasp3,
 &bSalirp3,
 &bGuardarp3,
 &nVerTemMax,
 &nVerTemMin,
 //-----botones 4ta pagina
 &bAtrasp4,
 &bSalirp4,
 &bGuardarp4,
 &nVerHumeMax,
 &nVerHumeMin,
 //-----botones 5ta pagina
 &bAtrasp5,
 &bSalirp5,
 &briego,
 &bvent,
 NULL 
};

//................................................................................
//-----------------------Ventilacion y Riego--------------------------------------
int ventilacion = 8;
int riego = 7;
int prueba = 13;
//................................................................................
//.......................SMS.................................................
char comando;
String mensaje=" ";
String mensaje2=" ";
String num_e;
String auxi;
String mensaje3=" ";
char nr;
int nrr=0;
int dd;
int ae=0;
String num_r;
//-------------------------funciones asociadas a la pantalla nextion-------------------------------
//---------------------------activar riego----------------------------------------
void onoff_riego()
{
 uint32_t est_riego;
 briego.getValue(&est_riego);

 if(est_riego==1)
 {
  digitalWrite(prueba,HIGH);
  digitalWrite(riego,HIGH);
  r=1;
  rv=1;
  rv2=3;
  ais=1;
  }
 else
 {
  digitalWrite(prueba,LOW);
  digitalWrite(riego,LOW);
  r=0;
  rv=1;
  rv2=4;
  ais=0;
  }
}
//--------------------------activar ventilacion------------------------------------
void onoff_ventilacion()
{
 uint32_t est_venti;
 bvent.getValue(&est_venti);

 if(est_venti==1)
 {
  digitalWrite(prueba,HIGH);
  digitalWrite(ventilacion,HIGH);
  v=1;
  rv=1;
  rv2=1;
  ais=1;
  }
 else
 {
  digitalWrite(prueba,LOW);
  digitalWrite(ventilacion,LOW);
  v=0;
  rv=1;
  rv2=2;
  ais=0;
  }  
}
//---------------boton configuracion-----------------------------------------
void configu()
{
  //Serial.println("configu");
  a=0;  
  while(a==0){
  nexLoop(next_listen_list);   
  
  }
  //Serial.println("Saliendo configu");
  
  }

void atras()
{
  //Serial.println("atras");
  b=1;
  c=1;
  d=1;
  //Serial.println("b"+String(b));
  //Serial.println("c"+String(c));
  //Serial.println("d"+String(d));
  }
  

void atras1()
{
  //Serial.println("atras1");
  a=1;
  //Serial.println("a"+String(a));
  }
void cerrar()
{
  //Serial.println("cerrar");
  a=1;
  b=1;
  c=1;
  d=1;
  //Serial.println("a"+String(a));
  
  //Serial.println("b"+String(b));
  //Serial.println("c"+String(c));
  //Serial.println("d"+String(d));
  }

//--------ajuste temperatura-------------------------------
void ct(){
  b=0;
    //Serial.println("auste temp");
  while(b==0){
  tiempomx=millis();
  tiempomn=millis();
  //---------------Configuracion de la temperatura---------------
//temperatura maxima******************************************
  if (tiempomx-ttmx > 500)
  {
  g=1;
  ttmx=tiempomx;
  nSetTemMax.getValue(&tmax);
  //EEPROM.get(10, ttmax);
  nVerTemMax.setValue(tmaxa);
  jTempMax.setValue(tmaxa);
  }
////temperatura minima******************************************
  if (tiempomn-ttmn > 1000)
  {
   g=2;
  ttmn=tiempomn;
  nSetTemMin.getValue(&tmin);
  //EEPROM.get(20, ttmin);
  nVerTemMin.setValue(tmina);
  jTempMin.setValue(tmina);
  }
//.............................................................. 
  nexLoop(next_listen_list);   
  }  
  }
//-----------------ajuste humedad------------------------------
void ch()
{
  c=0;
  //Serial.println("auste hume");
  while(c==0){
  tiempomx=millis();
  tiempomn=millis();
  //--------------Configuracion de la humedad---------------------
  //humedad maxima************************************************
  if(tiempomx-ttmx > 500){
  g=3;
  ttmx=tiempomx;
  nSetHumeMax.getValue(&hmax);
  //EEPROM.get(30, hhmx);
  nVerHumeMax.setValue(hmaxa);
  jHumeMax.setValue(hmaxa);
  }
 // humedad minima************************************************
  if(tiempomn-ttmn > 800){
  g=4;
  ttmn=tiempomn;
  nSetHumeMin.getValue(&hmin);
  //EEPROM.get(40, hhmin);
  nVerHumeMin.setValue(hmina);
  jHumeMin.setValue(hmina);
  }
  //..............................................................
  nexLoop(next_listen_list);
    }  
  }
//-------------------ventilacion/riego---------------------------
void veri()
{
  d=0;
  //Serial.println("vent/rieg");
  while(d==0)
  {
   nexLoop(next_listen_list);
  }
  }
//--------------------guardar------------------------------------
void guardar()
{
  //Serial.println("guardando");
  switch(g){
            case 1:
                  //Serial.println("Se guardo configuracion temmx");
                  EEPROM.put(10,tmax);
                  tmaxa=tmax;
                  break;
            case 2:
                  //Serial.println("Se guardo configuracion temmn");
                  EEPROM.put(20,tmin);
                  tmina=tmin;
                  break;

            case 3:
                  //Serial.println("Se guardo configuracion humemx");
                  EEPROM.put(30,hmax);
                  hmaxa=hmax;
                  break;
            case 4:
                  //Serial.println("Se guardo configuracion humemn");
                  EEPROM.put(40,hmin);
                  hmina=hmin;
                  break;
            default:
                  g=0;
                  break;
    }
  }

//..................................................................................................

void setup() {
tiempoInicio=millis();

  
pinMode(ventilacion,OUTPUT);
pinMode(riego,OUTPUT);
pinMode(prueba,OUTPUT);
pinMode(PinSensor, INPUT);
//pinMode(sagua,INPUT);

digitalWrite(ventilacion,LOW);
digitalWrite(riego,LOW);
digitalWrite(prueba,LOW);
//digitalWrite(sagua,HIGH);


bConf.attachPop(configu);

bATemp.attachPop(ct);
bAtrasp3.attachPop(atras);
bSalirp3.attachPop(cerrar);
bGuardarp3.attachPop(guardar);

bAHum.attachPop(ch);
bAtrasp4.attachPop(atras);
bSalirp4.attachPop(cerrar);
bGuardarp4.attachPop(guardar);

bAVen.attachPop(veri);
briego.attachPop(onoff_riego);
bvent.attachPop(onoff_ventilacion);
bAtrasp5.attachPop(atras);
bSalirp5.attachPop(cerrar);

bAtras.attachPop(atras1);
bCerrar.attachPop(cerrar);

//pinMode(sagua,INPUT);
//digitalWrite(sagua,HIGH);
//clitros=0;
//attachInterrupt(0,pulse,RISING);
attachInterrupt(0,ContarPulsos,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)

Serial.begin(9600);
Serial2.begin(9600);
Serial3.begin(9600);
//dht.begin();
sensor.begin();
nexInit();
 

configuracion_inicial();
t0=millis();

EEPROM.get(10, tmaxa);
EEPROM.get(20, tmina);
EEPROM.get(30, hmaxa);
EEPROM.get(20, hmina);
}

void loop() {
  
tiempohs=millis();
//h=dht.readHumidity();
//t=dht.readTemperature();  //lectura de la temperatura

sensor.requestTemperatures();
t=sensor.getTempCByIndex(0);

//Lectura humedad del suelo
if(tiempohs-ths>500){
  ths=tiempohs;
  sensorValue = analogRead(sensorPin);
 if(sensorValue >= valoragu && sensorValue <= valorair)
{
  hs = map(sensorValue, valorair,valoragu,0,100);
  }
  else{
    if (sensorValue > valorair)
        hs=0;
        else if (sensorValue < valoragu)
          hs=100;        
    }
}
//lectura consumo de agua en L
  float frecuencia= ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
  dt=millis()-t0; //calculamos la variación de tiempo
  t0=millis();
  volumen=volumen+(caudal_L_m/60)*(dt/1000); // volumen(L)=caudal(L/s)*tiempo(s)

//h=60;
//t=20;
//Serial.println(t);
//Serial.println(h);
//Serial.println(fagua);
 //Serial.println(clitros);
//hs=analogRead(sensorpin);
//hs=map(sensorpin,910,823,0,100);

dtostrf(t,5,2,temp);// 24.09
tt.setText(temp);

dtostrf(hs,5,2,hume);
th.setText(hume);
////----------------------lectura del flujo de agua---------------------------------
dtostrf(volumen,5,2,c_agua);
tf.setText(c_agua);
////................................................................................
Serial.println("TEMP:"+String(t) + ",HUME:"+String(hs)+",FLU:"+String(volumen)+",TX:"+String(tmaxa)+",TN:"+String(tmina)+",HX:"+String(hmaxa)+",HN:"+String(hmina)+",V:"+String(v)+",R:"+String(r));
//Serial.println("TEMP:"+String(t) + ",HUME:"+String(h)+",FLU:"+String(clitros));
////-------Activacion de la ventilacion mediante el sensor de temperatura-------------
if(ais==0){
if(t>= tmaxa)
{
  //rv=1;
  digitalWrite(ventilacion,HIGH);
  v=1;
  //rv2=1;
  Serial2.print("tmVenti.en=1");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  delay(500);
  Serial2.print("nn=1");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  delay(500);
  }
//..........................................................................
//----------Desactivacion de la ventilacion mediante el sensor de temperatura---------
if(t<=tmina)
{
  //rv=1;
  v=0;
  //rv2=2;
  digitalWrite(ventilacion,LOW);
  Serial2.print("tmVenti.en=0");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  delay(500);
  Serial2.print("nn=0");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  delay(500);
  }
//...........................................................................
//delay(500);
//---------Activacion del riego mediante el sensor de humedad del suelo---------------
if(hs>= hmaxa)
{
  rv=1;
  r=0;
  rv2=4;
  digitalWrite(riego,LOW);
  //Serial2.print("tmRieg.en=0");
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);
 // delay(500);
  //Serial2.print("mm=0");
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);
  }
//.....................................................................................
//---------Desactiacion del riego mediante el sensor de humedad del suelo--------------
if(hs<=hmina)
{
  rv=1;
  r=1;
  rv2=3;
  digitalWrite(riego,HIGH);
  //Serial2.print("tmRieg.en=1");
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);
  delay(500);
  //Serial2.print("mm=1");
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);
  //Serial2.write(0xFF);  
  }
}
//......................................................................................
//delay(500);
//--------------------------------
nexLoop(next_listen_list);

while(Serial3.available() > 0)     // se verifica si en el puerto serial entre el arduino y la sim900 hay algun dato 
   {
   comando = Serial3.read();          // los datos que se encuentran en el puerto serial se almacena en la variable comando
   mensaje = mensaje+comando;         // los datos que se almacenan en la variable comando se almacenan en la variable mensaje para formar una cadena de caracteres y no perder los datos que estan en el puerto serial
   ae=1;                              //se pone en 1 la variable ae
   }
while(ae==1){                      //compara si ae es igual a 1
   //Serial.println(mensaje);
 
   mensaje.toUpperCase();             // usando la instruccion toUpperCase convierte la cadena de caracteres que estan en la variable mensaje en mmayusculas                                                                    
   obte_nfh();                        // esta subrutina obtiene la fecha, hora y el numero telefonico que envio un sms a la sim900 
   //Serial.println("num_re"+num_r);
   
   if(mensaje.indexOf("DATOS")>=0)
   {
    //Serial.println("enviando datos");
    auxi="T="+String(t)+" C"+" H="+String(hs)+" %"+"F.Agua= "+String(volumen)+" L";
    enviar_mensaje();
   }
   
   if(mensaje.indexOf("V ON")>=0)
   {
    rv=1;
    v=1;
    ais=1;
    //Serial.println("enviando sms");
    digitalWrite(ventilacion,HIGH);
    digitalWrite(prueba,HIGH);
    rv2=1;
    auxi="Activando Ventilacion";
    enviar_mensaje();
    }

   
   if(mensaje.indexOf("V OFF")>=0)
   {
    rv=1;
    v=0;
    ais=0;
    //Serial.println("enviando sms");
    digitalWrite(ventilacion,LOW);
    rv2=2;
    auxi="Desactivando Ventilacion";
    enviar_mensaje();
    }

if(mensaje.indexOf("R ON")>=0)
   {
    rv=1;
    r=1;
    ais=1;
    //Serial.println("enviando sms");
    digitalWrite(riego,HIGH);
    rv2=3;
    auxi="Activando Riego";
    enviar_mensaje();
    }

   
   if(mensaje.indexOf("R OFF")>=0)
   {
    rv=1;
    r=0;
    ais=0;
    //Serial.println("enviando sms");
    digitalWrite(riego,LOW);
    rv2=4;
    auxi="Desactivando Riego";
    enviar_mensaje();
    }
   ae=0;
}

while(Serial.available()>0)
{
  nr=Serial.read();
  mensaje3=mensaje3+nr;
  nrr=1;
}
while(nrr==1){
  if(mensaje3.indexOf("von")>=0){
    rv=1;
    v=1;
    ais=1;
    digitalWrite(ventilacion,HIGH);
    rv2=1;
  }
  if(mensaje3.indexOf("voff")>=0){
    rv=1;
    v=0;
    ais=0;
    digitalWrite(ventilacion,LOW);
    rv2=2;
  }
  if(mensaje3.indexOf("ron")>=0){
    rv=1;
    r=1;
    ais=1;
    digitalWrite(riego,HIGH);
    rv2=3;
  }
  if(mensaje3.indexOf("roff")>=0){
    rv=1;
    r=0;
    ais=0;
    digitalWrite(riego,LOW);
    rv2=4;
  }
  if(mensaje3.indexOf("tm")>=0){
    int dtm=0;
    String stm=mensaje3.substring(dtm+2,dtm+5);
    int ntm;
    ntm=String(stm).toInt();
    //Serial.println(String(ntm));
    tmaxa=ntm;
    //Serial.println(String(tmaxa));
    //dtm=mensaje3.indexOf("tm");
    
  }

if(mensaje3.indexOf("tn")>=0){
    int dtn=0;
    String stn=mensaje3.substring(dtn+2,dtn+5);
    int ntn;
    ntn=String(stn).toInt();
    //Serial.println(String(ntm));
    tmina=ntn;
    //Serial.println(String(tmaxa));
    //dtm=mensaje3.indexOf("tm");
    
  }

  if(mensaje3.indexOf("hm")>=0){
    int dhm=0;
    String shm=mensaje3.substring(dhm+2,dhm+5);
    int nhm;
    nhm=String(shm).toInt();
    //Serial.println(String(ntm));
    hmaxa=nhm;
    //Serial.println(String(tmaxa));
    //dtm=mensaje3.indexOf("tm");
    
  }
if(mensaje3.indexOf("hn")>=0){
    int dhn=0;
    String shn=mensaje3.substring(dhn+2,dhn+5);
    int nhn;
    nhn=String(shn).toInt();
    //Serial.println(String(ntm));
    hmina=nhn;
    //Serial.println(String(tmaxa));
    //dtm=mensaje3.indexOf("tm");
    
  }
  mensaje3="";
  nrr=0;
}


if(rv==1){

  switch (rv2){
                case 1:   Serial2.print("tmVenti.en=1");
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);   
                          delay(300);
                          Serial2.print("nn=1");
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          break; 
                
                case 2:   Serial2.print("tmVenti.en=0");
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.print("nn=0");
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          break;
                          
                case 3:    
                          Serial2.print("tmRieg.en=1");
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);    
                          delay(300);
                          Serial2.print("mm=1");
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          
                          break;
                          
                case 4:
                          Serial2.print("tmRieg.en=0");
                          //delay(300);
                          Serial2.write(0xFF);
                          //delay(300);
                          Serial2.write(0xFF);
                          //delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.print("mm=0");
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          delay(300);
                          Serial2.write(0xFF);
                          break;
                default:
                          rv2=0;
                          break;
                }
          }

//.................................
//Serial.println(millis());
//if (millis()- tiempoInicio>10000)
//{
//  //asm volatile ("  jmp 0");
//  //asm volatile ("  jmp 0");
////   void (*resetFunc)(void) = 0;
////  resetFunc();
// wdt_enable(WDTO_15MS);  // Activar el temporizador de vigilancia con un tiempo corto
//  while (1);  // Entrar en un bucle infinito para provocar un fallo y reinicio
//  }

}

//-----------------------configuracion inicial para la sim------------------
void configuracion_inicial()
 {
  Serial3.println("AT+IPR=9600");                 //enviamos el comando AT+IPR=9600, para establecer la comunicacion entre el arduino y la sim900
  delay(500);
  Serial3.println("AT+CMGF=1");                   //enviamos el comando AT+CMGF=1, para establecer el modo texto en la sim900
  delay(500);
  Serial3.println("AT+CNMI=2,2,0,0,0\r");         //enviamos el comando AT+CNMI=2,2,0,0,0 el cual al recibir un sms esta se pueda leer en el puerto serial de comunicacion entre el arduino y la sim808
  delay(500);
  Serial.println("configuacion terminada");
 }
//------------------------envio de sms-------------------------------------
void enviar_mensaje()
 {
    Serial3.println("AT+CMGF=1");               // enviamos el comando AT+CMGF=1, para establecer el modo texto en la sim900
    delay(500);
    Serial3.print("AT+CMGS=");                  // enviamos el comando AT+CMGS=, para enviar el sms al usuario
    delay(500);

    num_e="+591"+num_r;
    Serial3.print((char)34);                    // se habre comillas
    Serial3.print(num_e);                      // se establece el numero del usuario 
    Serial3.println((char)34);                  // se cierra comillas
    delay(1000);
 
    Serial3.println(auxi);                      // mediante el puerto serial del la sim808 se toma la variable auxi
    Serial3.print((char)26);                    // se realiza un control z el cual envia el sms al usuario
    delay(5000);
    //Serial.println("Usuario notificado");
    mensaje= " ";                           // inicializamos la variable mensaje 
    comando = 0;                            // inicializamos la variable comando
 }
//....................................................................................................................................

//------------------------------------obteniendo numero, hora y fecha que la sim recibio-------------------------------------------------------------------------------
void obte_nfh(){
 mensaje2= mensaje;                                 // almacenamos la cadena de informacion que recibe la sim900
   //usando la instruccion indexOf buscamos el caracter "+CMT: "  
    dd= mensaje2.indexOf("+CMT: ");                 // a partir de ese carater buscamos el numero telefonico que envio el mensaje,la fecha y hora en la que se envio el sms
    num_r = mensaje2.substring(dd+7,dd+15);         // obtenemos en numero telefonico que envio el comando   
    //fec_r = mensaje2.substring(dd+21,dd+29);        // obtenemos la fecha en la que se envio el sms
    //hor_r = mensaje2.substring(dd+30,dd+38);        // obtenemos la hora en la que se envio el sms
}
//-----------------------------------obteniendo flujo de agua------------------------------
//void pulse()
//{
//  clitros += 1.0/450.0;
//  }
//---Función que se ejecuta en interrupción---------------
void ContarPulsos ()  
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuecia() 
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(1000);   //muestra de 1 segundo
  //noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia=NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}
