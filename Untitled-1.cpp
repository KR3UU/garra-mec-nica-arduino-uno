#include <SoftwareSerial.h> //INCLUSÃO DE BIBLIOTECA PARA USO DE MÚLTIPLAS PORTAS SERIAIS 
  #include <Servo.h> // BIBLIOTECA QUE FACILITA O USO DOS SERVOMOTORES

// DEFINIÇÃO DE ENTRADAS DOS SERVOMOTORES
  
#define servoBase        3  // Porta Digital do Servo da Base
#define servoAlturaBraco 5  // Porta Digital do Servo da Altura do Braço
#define servoAnguloBraco 9  // Porta Digital do Servo do Ângulo do Braço
#define servoGarra       11  // Porta Digital do Servo da Garra

// DEFINIÇAO DAS ENTRADAS DOS POTENCIÔMETROS

#define potBase         A3  // Porta Analógica do Potenciometro para Controle da Garra
#define potAlturaBraco  A2  // Porta Analógica do Potenciometro para Controle da Ângulo do Braço 
#define potAnguloBraco  A1  // Porta Analógica do Potenciometro para Controle do Altura do Braço
#define potGarra        A0  // Porta Analógica do Potenciometro para Controle da base

// DEFINIÇAO DAS ENTRADA DOS BOTÕES

#define botaoCongela     8  // Porta Digital do Botao Congela
#define botaoDescongela  10  // Porta Digital do Botao Descongela

// Instanciação dos Objetos de Controle dos Servos

Servo garra;
Servo angulo;
Servo altura;
Servo base; 

// Variaveis para cálculo dos angulos

int valorPotBase = 0;
int valorAngBase = 70;
int valorPotAltura;
int valorAngAltura = 90;
int valorPotAngulo = 0;
int valorAngAngulo = 150; 
int valorPotGarra;
int valorAngGarra = 65;

// Variaveis para controle dos botões

int estadoBotaoC, estadoBotaoD;
int estadoAntBotaoC = HIGH;
int estadoAntBotaoD = HIGH;
boolean congelado = false;
int bt = 2; // BOTÃO QUE ALTERNA ENTRE OS MODOS DE CONTROLE


// DEFINIÇÃO DE VARIÁVEIS EXTRAS
 
const int pinoRX = 12; //PINO DIGITAL 12 (RX)
const int pinoTX = 7; //PINO DIGITAL 7 (TX)
int dadoBluetooth = 0; //VARIÁVEL QUE ARMAZENA O VALOR ENVIADO PELO BLUETOOTH
bool y = false; // VARIÁVEL USADA PARA ALTERNAR ENTRE OS MODOS DE CONTROLE
 
SoftwareSerial bluetooth(pinoRX, pinoTX); //PINOS QUE EMULAM A SERIAL, ONDE
//O PINO 12 É O RX E O PINO 7 É O TX




void setup() {

// AQUI DEFINIMOS ALGUMAS ENTRADAS DO ARDUINO COMO BOTÕES
  
  pinMode(botaoCongela, INPUT_PULLUP); 
  pinMode(botaoDescongela, INPUT_PULLUP);
  pinMode(bt, INPUT_PULLUP);
    
  //Configuração das Portas dos Servos
  
  base.attach(servoBase);
  altura.attach(servoAlturaBraco);
  angulo.attach(servoAnguloBraco);
  garra.attach(servoGarra); 


  
  Serial.begin(9600); //INICIALIZA A SERIAL
  bluetooth.begin(9600); //INICIALIZA A SERIAL DO BLUETOOTH
  bluetooth.print("$"); //IMPRIME O CARACTERE
  bluetooth.print("$"); //IMPRIME O CARACTERE
  bluetooth.print("$"); //IMPRIME O CARACTERE
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS

// ANGULOS INICIAIS DOS SERVOMOTORES
  
  base.write(70);
  altura.write(90);
  angulo.write(150);
  garra.write(80);
  
}

void loop() {

// UTILIZANDO A ESTRUTURA CONDICIONAL PARA ALTERNAR ENTRE OS MODOS DE CONTROLE

 if (digitalRead(bt)==HIGH) y = true;
 if (digitalRead(bt)==LOW) y = false;

   Serial.println(digitalRead(bt)); // EXIBIMOS NA TELA O ESTADO ATUAL DA GARRA PARA VERIFICAÇÃO
   // UTILIZAMOS A ESTRUTURA CNDICIONAL PARA CHAMAR AS FUNÇÕES QUE CONTROLAM A GARRA CONT (MANUAL) E BLUE (BLUETOOTH)
  if(y==true) cont();
  if(y==false) blue(); 
  
}


// FUNÇÃO CRIADA PARA CONTROLAR A GARRA VIA BLUETOOTH

void blue()
{
   if(bluetooth.available()){ //SE O BLUETOOTH ESTIVER HABILITADO, FAZ
       dadoBluetooth = bluetooth.read(); //VARIÁVEL RECEBE O VALOR ENVIADO PELO BLUETOOTH

// UTILIZAMOS A ESTRUTURA CONDICIONAL PARA CONTROLAR A GARRA UTILIZANDO VALORES RECEBIDOS VIA BLUETOOTH
 
    if(dadoBluetooth == 'w')
    { //SE O VALOR RECEBIDO FOR IGUAL A W, FAZ
      Serial.println("sobe"); //IMPRIME O TEXTO NA SERIAL
            if(valorAngAltura+20<190)valorAngAltura = valorAngAltura+20; // ADICIONA 20 GRAUS AO ÂNGULO DO SERVOMOTOR
             altura.write(valorAngAltura); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR

    }
    if(dadoBluetooth == 's')
    { 
      Serial.println("desce"); //IMPRIME O TEXTO NA SERIAL
           if(valorAngAltura-20>40)valorAngAltura = valorAngAltura-20; // REMOVE 20 GRAUS AO ÂNGULO DO SERVOMOTOR
            altura.write(valorAngAltura); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
            
    }
    if(dadoBluetooth == 'a')
    {
       Serial.println("esquerda"); //IMPRIME O TEXTO NA SERIAL
      if(valorAngBase+20<140)valorAngBase = valorAngBase+20; // ADICIONA 20 GRAUS AO ÂNGULO DO SERVOMOTOR
      base.write(valorAngBase); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
    }
    if(dadoBluetooth == 'd')
    { 
      Serial.println("direita"); //IMPRIME O TEXTO NA SERIAL
      if(valorAngBase-20>1)valorAngBase = valorAngBase-20; // REMOVE 20 GRAUS AO ÂNGULO DO SERVOMOTOR
      base.write(valorAngBase); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR 
    }
     if(dadoBluetooth == '1')
     { 
      Serial.println("abre"); //IMPRIME O TEXTO NA SERIAL
      if(valorAngGarra+20<146)valorAngGarra = valorAngGarra+20; // ADICIONA 20 GRAUS AO ÂNGULO DO SERVOMOTOR
        garra.write(valorAngGarra); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR

      
    }
    if(dadoBluetooth == '2')
    { 
      Serial.println("fecha"); //IMPRIME O TEXTO NA SERIAL
      if(valorAngGarra-20>75)valorAngGarra = valorAngGarra-20; // REMOVE 20 GRAUS AO ÂNGULO DO SERVOMOTOR
      garra.write(valorAngGarra); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
    }
    if(dadoBluetooth == '3')
    {
       Serial.println("sobe"); //IMPRIME O TEXTO NA SERIAL
             if(valorAngAngulo+20<180)valorAngAngulo = valorAngAngulo+20; // ADICIONA 20 GRAUS AO ÂNGULO DO SERVOMOTOR
             angulo.write(valorAngAngulo); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR 
    }
     if(dadoBluetooth == '4')
    {
       Serial.println("desce"); //IMPRIME O TEXTO NA SERIAL
             if(valorAngAngulo-20>0)valorAngAngulo = valorAngAngulo-20; // REMOVE 20 GRAUS AO ÂNGULO DO SERVOMOTOR
             angulo.write(valorAngAngulo); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
    } 
  }
}

// FUNÇÃO CRIADA PARA CONTROLAR A GARRA VIA JOYSTICK

void cont() 
{
  if (!congelado) {
    if((analogRead(potBase)>valorPotBase+20) or (analogRead(potBase)<valorPotBase-20)) // VERIFICA OS LIMITES DOS SERVOMOTORES
    {
       valorPotBase = analogRead(potBase);  // LÊ O VALOR DO JOYSTICK
       valorAngBase = map(valorPotBase, 0, 1024, 1, 140); //Conversão do valor do potenciometro (de 0 até 1024) para o angulo (de 0 até 180)
       base.write(valorAngBase); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
    }
    if((analogRead(potAlturaBraco)>valorPotAltura+30) or (analogRead(potAlturaBraco)<valorPotAltura-30)) // VERIFICA OS LIMITES DOS SERVOMOTORES
    {
       valorPotAltura = analogRead(potAlturaBraco);   // LÊ O VALOR DO JOYSTICK
       valorAngAltura = map(valorPotAltura, 0, 1024, 180, 0); //Conversão do valor do potenciometro (de 0 até 1024) para o angulo (de 0 até 180)
       altura.write(valorAngAltura); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
    }

    if((analogRead(potAnguloBraco)>valorPotAngulo+30) or (analogRead(potAnguloBraco)<valorPotAngulo-30)) // VERIFICA OS LIMITES DOS SERVOMOTORES
    {
       valorPotAngulo = analogRead(potAnguloBraco);  // LÊ O VALOR DO JOYSTICK 
       valorAngAngulo = map(valorPotAngulo, 0, 1024, 180, 0); //Conversão do valor do potenciometro (de 0 até 1024) para o angulo (de 0 até 180)
       angulo.write(valorAngAngulo); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
    }
        if((analogRead(potGarra)>valorPotGarra+150) or (analogRead(potGarra)<valorPotGarra-150)) // VERIFICA OS LIMITES DOS SERVOMOTORES
    {
       valorPotGarra = analogRead(potGarra);   // LÊ O VALOR DO JOYSTICK
       valorPotGarra = map(valorPotGarra, 0, 1024, 175, 70); //Conversão do valor do potenciometro (de 0 até 1024) para o angulo (de 0 até 180)
       garra.write(valorPotGarra); // ALTERA O ÂNGULO ATUAL DO SERVOMOTOR
    }
  }

// BLOCO RESPONSÁVEL POR CONGELAR E DESCONGELAR A GARRA

  estadoBotaoC = digitalRead(botaoCongela); // VERIFICA O ESTADO DO BOTÃO DE CONGELAMENTO DA GARRA 
  if (estadoBotaoC != estadoAntBotaoC) { // VERIFICA SE HOUVE UMA ALTERAÇÃO NO ESTADO DO BOTÃO
     congelado = true; // CONGELA A GARRA
  }
  estadoAntBotaoC = estadoBotaoC; // SALVA O ESTADO ATUAL PARA COMPARAÇÕES FUTURAS

  estadoBotaoD = digitalRead(botaoDescongela); // VERIFICA O ESTADO DO BOTÃO DE DESCONGELAMENTO DA GARRA 
  if (estadoBotaoD != estadoAntBotaoD) { // VERIFICA SE HOUVE UMA ALTERAÇÃO NO ESTADO DO BOTÃO
     congelado = false; // DESCONGELA A GARRA
  }
  estadoAntBotaoD = estadoBotaoD; // SALVA O ESTADO ATUAL PARA COMPARAÇÕES FUTURAS
}