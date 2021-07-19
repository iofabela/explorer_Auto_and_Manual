//-------------- LIBRARIES -----------------

#include<Ultrasonic.h>
#include<Servo.h>

Servo servo ;

//----------------- DEFINES ----------------------
#define TRIGGER_PIN  12
#define ECHO_PIN     13

//---------------- SETUP VARIABLES -----------------

//Estado
  int estado = 0;

//Servo
  int servo_izq = 135 ;
  int servo_cen = 90 ;
  int servo_der = 45 ;

//-------------------------------------------------

//Motor Tracción
  int mot_tracc_adel = 8 ;
  int mot_tracc_atras = 9 ;
  int mot_tracc_ena = 3 ;

//Declara motor dirección

  int mot_derecha = 6;
  int mot_izquierda = 7;

//bt int pwm
  int pwm = 0;

  int lim_max =3000 ;

  int lim_sup_colision = 3000;
  int lim_inf_colision = 2000;

  int lim_sup_300 = 2000;
  int lim_inf_300 = 300;

  int lim_sup_80 = 300; //80% Ciclo trabajo motor
  int lim_inf_80 = 200;

  int lim_sup_60 = 200;
  int lim_inf_60 = 100;

  int lim_sup_40 = 100;
  int lim_inf_40 = 50;

  int lim_sup_ultra = 50;
  int lim_inf_ultra = 30;

  int lim_sup_reversa = 30;
  int lim_inf_reversa = 10;

  int lim_critico = 10 ;


  Ultrasonic ultrasonic(TRIGGER_PIN,ECHO_PIN);


//--------------- VOID SETUP ----------------

void setup()
  {
  Serial.begin(9600);
  servo.attach(4); //Pin digital 4
  servo.write(servo_cen);
  
  pinMode(mot_tracc_adel,OUTPUT);
  pinMode(mot_tracc_atras,OUTPUT);
  pinMode(mot_tracc_ena,OUTPUT);
  
  pinMode(mot_derecha,OUTPUT);
  pinMode(mot_izquierda,OUTPUT);
  
  //Declarar los de dirección
  
}


//-------------- VOID LOOP --------------
void loop()
{
  //Estado  
  int estado= digitalRead(A0);    
    
  servo.write(servo_cen);
  delay(1000);
  
  float cmMsec_izq;
  float cmMsec_cen;
  float cmMsec_der;
  
  long microsec_izq;
  long microsec_der;
  long microsec_cen = ultrasonic.timing();
  
  cmMsec_izq ;
  cmMsec_der ;
  cmMsec_cen = ultrasonic.convert(microsec_cen, Ultrasonic::CM);

  if (estado == HIGH)  
  { 
    //Programa explorer
    Serial.println("mode manual");

    //Max
    if (cmMsec_cen>lim_max)
    {
    //Activar motor al 90%
    adelante(90);
    Serial.println("Sin obstaculos");
    }
  
    //Colisión
    else if (cmMsec_cen>=lim_inf_colision && cmMsec_cen<lim_sup_colision)
    {
      Serial.println("Colisión !");
      atras(100);
      delay(2000);
      detener();
    }

    //300
    else if (cmMsec_cen>=lim_inf_300 && cmMsec_cen< lim_sup_300)
    {
      //Igual a 300
      //Mandar a 90%
      adelante(90);
      Serial.println("Motor al 90%");
    }
    //80
    else if(cmMsec_cen>=lim_inf_80 && cmMsec_cen<lim_sup_80)
    {
      //Poner motor a 80%
      adelante(80);
      Serial.println("Motor al 80%");
    } 
    //60
    else if(cmMsec_cen>=lim_inf_60 && cmMsec_cen<lim_sup_60)
    {
      //Motor en 60%
      adelante(60);
      Serial.println("Motor al 60%");
    }
    //40
    else if(cmMsec_cen>=lim_inf_40 && cmMsec_cen<lim_sup_40)
    {
      //Motor en 40%
      adelante(40);
      Serial.println("Motor al 40%");
    }
    //ultras
    else if (cmMsec_cen<lim_sup_ultra && cmMsec_cen>lim_inf_ultra)
    {
      detener();
      Serial.println("Decidiendo");
      Serial.println(" ");
      delay(1000);
  
      //Lectura hacia la izquierda
      servo.write(servo_izq);
      delay(1000);
      microsec_izq = ultrasonic.timing();
      cmMsec_izq = ultrasonic.convert(microsec_izq, Ultrasonic::CM);
      Serial.print(" Izquierda = ");
      Serial.print(cmMsec_izq);
      Serial.print("   "); 

      //Lectura hacia el centro
      servo.write(servo_cen);
      delay(1000);
      microsec_cen = ultrasonic.timing();
      cmMsec_cen = ultrasonic.convert(microsec_cen, Ultrasonic::CM);
      Serial.print(" Centro = ");
      Serial.print(cmMsec_cen);
      Serial.print("   "); 
 
      //Lectura hacia la derecha
      servo.write(servo_der);
      delay(1000);
      microsec_der = ultrasonic.timing();
      cmMsec_der = ultrasonic.convert(microsec_der, Ultrasonic::CM);
      Serial.print(" Derecha = ");
      Serial.print(cmMsec_der);
      Serial.print("");
 
      if ( cmMsec_izq > cmMsec_cen && cmMsec_izq > cmMsec_der)
      {
        //principio de if para cmMsec_izq
        Serial.print ("  Vete a la izquierda ! ");
        Serial.println("");
        atras(70);
        delay(1000);
        izquierda();
        delay(1000);
        }//fin principio de if para cmMsec_izq
      
      else if (cmMsec_cen > cmMsec_izq && cmMsec_cen > cmMsec_der)
      {
        //principio de if para cmMsec_cen
        Serial.print("  Vete al centro ! ");
        Serial.println("");
      }//fin de if para cmMsec_izq

      else if(cmMsec_der > cmMsec_cen && cmMsec_der > cmMsec_izq )
      {
        Serial.print("  Vete a la derecha");
        Serial.println("");      
        atras(70);
        delay(1000);
        derecha();
        delay(1000);
      }
        
    }//Fin else ultra

    //Reversa
    else if(cmMsec_cen>=lim_inf_reversa && cmMsec_cen<lim_sup_reversa)
    {
      //Motor en reversa suave
      Serial.println("Reversa Suave"); 
      atras(50);
      delay(2000); 
    }
    
    else if (cmMsec_cen<lim_critico)
    {
      //Motor en Reversa Máxima
      Serial.println("Reversa Máxima");
      atras(100);
      delay(2000); 
    }

    else
    {
      //Imprime Peligro
      //Detener Motor
      //Paro de emergencia
      detener();
      Serial.println("Fallo de ultrasonico");
    }//Fin else
    
  } //Fin Estado == HIGH

  else if(estado == LOW)
  {
    //Programa bt
    Serial.println("mode automatico");
    while(Serial.available())
    {//llave de while
      char dato = Serial.read();
    
      switch(dato)
      {//switch
       
       case 'w':
        {//w
          digitalWrite(mot_tracc_adel,HIGH);
          digitalWrite(mot_tracc_atras,LOW);
          pwm = 0 ;
          analogWrite(mot_tracc_ena,pwm);
          Serial.println("Adelante :D");
          break;
        }//w
        
        case 'e':
        {//Sumador
          pwm = pwm + 10 ;
          if( pwm >= 250 ){pwm = 250 ;}  
          analogWrite(mot_tracc_ena,pwm);
          Serial.println (pwm);
          break;       
        }
        
        case 'x':
        {//Restador
          pwm = pwm - 10 ;
          if( pwm <= 0 ){pwm = 0;}  
          analogWrite(mot_tracc_ena,pwm);
          Serial.println (pwm);
          break;       
        }
        
        case 's':
        {//s
          digitalWrite(mot_tracc_adel,LOW);
          digitalWrite(mot_tracc_atras,HIGH);
          pwm=0;
          analogWrite(mot_tracc_ena,pwm);
          Serial.println("Atras :C");
          break;
        }//s
      
        case 'd':
        {//d
          digitalWrite(mot_derecha,HIGH);
          digitalWrite(mot_izquierda,LOW);
          pwm = 0 ;
          Serial.println("Derecha ->");
          break;
        }//d

        case 'a':
        {//a
          digitalWrite(mot_derecha,LOW);
          digitalWrite(mot_izquierda,HIGH);
          pwm = 0 ;
          Serial.println("Izquierda <-");
          break;
        }//a      
       default:
        {//default
          digitalWrite(mot_tracc_ena,LOW);
          digitalWrite(mot_tracc_adel,LOW);
          digitalWrite(mot_tracc_atras,LOW);
          digitalWrite(mot_derecha,LOW);
          digitalWrite(mot_izquierda,LOW);
          Serial.println("Detener !"); 
          break;   
        }//default
       
      }//Fin de switch
    
    }//Fin llave de while
  
  }//Fin else if estado == LOW


}//Fin de void loop  


/* FUNCTIONS */
void detener()
{
  digitalWrite(mot_tracc_adel ,LOW);
  digitalWrite(mot_tracc_atras ,LOW);
  digitalWrite(mot_tracc_ena ,LOW);
}

void adelante(int pwm)
{
  digitalWrite(mot_tracc_adel ,HIGH);
  digitalWrite(mot_tracc_atras,LOW);
  pwm = map (pwm,0,100,0,255);
  analogWrite(mot_tracc_ena,pwm);
  
  Serial.println("Adelante : ");
  Serial.print(pwm);
  Serial.print("%");
 
}

void atras(int pwm)
{
  digitalWrite(mot_tracc_adel ,LOW);
  digitalWrite(mot_tracc_atras,HIGH);
  pwm = map (pwm,0,100,0,255);
  analogWrite(mot_tracc_ena,pwm);
  
  Serial.println("Atras : ");
  Serial.print(pwm);
  Serial.print("%");
 
}

void derecha()
{
  digitalWrite(mot_derecha,HIGH);
  digitalWrite(mot_izquierda,LOW);
  
  adelante(70);
  
  delay(1000);
  
  detener();
  
  digitalWrite(mot_derecha,LOW);
  digitalWrite(mot_izquierda,LOW);
}

void izquierda()
{
  digitalWrite(mot_derecha,LOW);
  digitalWrite(mot_izquierda,HIGH);
  adelante(70);
  delay(1000);
  detener();
  digitalWrite(mot_derecha,LOW);
  digitalWrite(mot_izquierda,LOW);
}
