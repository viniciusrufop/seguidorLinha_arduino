//https://www.arduinoecia.com.br/2014/04/controle-de-motor-cc-com-o-l293d-ponte-h.html

#include <avr/io.h>         //Incluindo biblioteca
#include <util/delay.h>       //Incluindo biblioteca
#include <stdbool.h>
#include "utilidades.h"
#include "pwm.h"

#define S1 PD2    // pino digital 2     0 - em cima da linha, 1 - fora da linha
#define S2 PD3    // pino digital 3
#define S3 PD4    // pino digital 4

#define motor1 PB1  // pino digital 9 
#define motor2 PB2  // pino digital 10

#define kp 150
#define kd 100
#define ki 0
#define velIni 600

bool LS1,LS2,LS3; // leitura dos sensores
int proporcional,derivativo,integrador;
int valorErro,erroAnti;
int sinalControle;  // valor do pid calculado

void leituraSensores(){
    LS1 = leituraDigitalD(S1);
    LS2 = leituraDigitalD(S2);
    LS3 = leituraDigitalD(S3);
}

void erro(){
  if(!LS1 && !LS2 && LS3){
    valorErro = 2;
  }
  else if(!LS1 && LS2 && LS3){
    valorErro = 1;
  }
  else if(!LS1 && LS2 && !LS3){
    valorErro = 0;
  }
  else if(LS1 && LS2 && !LS3){
    valorErro = -1;
  }
  else if(LS1 && !LS2 && !LS3){
    valorErro = -2;
  }
}

float PID(){ //função controle PID
	erro();
	proporcional = valorErro;
	derivativo = valorErro - erroAnti;
	integrador = integrador + valorErro;

	erroAnti = valorErro;

	return kp*proporcional + kd*derivativo + ki*integrador;
}

int main(){
	iniciaPWM();// inicia as configurações do pwm nos pinos D9 e D10
	sentidoMotor(1);	// define o sentido dos motores
	while(1){
		leituraSensores();
		sinalControle = PID();		
		escreveAnalogicoD9(velIni + sinalControle);
		escreveAnalogicoD10(velIni - sinalControle);
	}
}

