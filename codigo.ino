/*   Controle de prensa para automatização de produção

Premissas;
  
  "entradas"
  *controle de temporizador com potenciometro -tempo de espera entre descida da prensa 
  *controle de temporizador com potenciometro - tempo de pistão de alimentação frente
  *controle de temporizador com potenciometro - tempo de pistão de alimentação traz
  *pedal para acionamento manual com as mãos livres.
  *comando bimanual direito
  *comando bimanual esquerdo
  *chave seletora 1/4 automático
  *chave seletora 2/4 manual com alimentador
  *chave seletora 3/4 manual sem alimentador
  *chave de emergência
  *fim de curso da prensa
  *Botão alterar visor display
 
  "saídas"
  *rele de acionamento prensa
  *rele de acionamento Alimentador
  *rele acionamento do motor
  *rele de indicação emergência/modo de trabalho
  
  "trabalhos a ser executados "
    
    modo parado
      
      chave de emergência fechada
      chave seletora de posição no modo 4/4 maquina parada -(todas as portas seletora aberta)
        - Desligar motor se estiver ligado
	- reseta motor caso emergencia ou falta de energia 
        - Botão alterar o display entre os valores do potenciometro, apos 60 segundos ele volta para contagem
		-isso facilita leitura do potenciometro e possibilita setup mais rapida em repetições
            
      *visor com contagem de 8 dígitos.
      contagem no limite de 99.999.999 (noventa e nove milhões novecentos e noventa e nove mil novecentos e noventa e nove peças).
      será somado mais uma peça a cada prensagem
      memoria será mantida após ser desligado
        -como zerar
                    botão alterar modo display prescionado por mais de 5 segundos
    
    modo manual sem Alimentador 3/4 :
      chave de emergência fechada
      chave seletora de posição no modo 3/4 manual para controle de batida uma por vez
      	-Liga o motor se estiver desligado aguarda X segundo para atingir velocidade de trabalho
        -Acionamento por pedal ou bimanual - bimanual com sistema individual 
		-desce prensa 1 vez e aguarda o tempo do pontenciometro prensa antes da proxima leitura de comando

        rele indicador de modo Trabalho ativo
	      
	      
    modo manual com Alimentador 2/4 :
      chave de emergência fechada
      chave seletora de posição no modo 2/4 manual para controle de batida uma por vez com alimentador
      	-Liga o motor se estiver desligado aguarda X segundo para atingir velocidade de trabalho
        -Acionamento por pedal ou bimanual - bimanual com sistema individual 
		-desce prensa 1 vez e aguarda o tempo do pontenciometro prensa 
		-aciona o alimentador e aguarda o tempo do potenciometro do alimentador antes da proxma leitura

        rele indicador de modo Trabalho ativo
	    
    modo automático 1/4:
      chave de emergência fechada
      chave seletora de posição no modo 1/4 automático
        -Acionamento para o modo automatico será executado descidas e alimentação dois reles conforme  ajuste de ponteciometros
	-mantendo um dos botões do bicomando pressionado por 3 ou mais prensagens seguidas o auto trava
        caso um dos botões for prescionado e solto a prensa vai descer 
        uma ou duas vezes e parar se o botão estiver solto antes do valor determinado. 
        -O acionamento automatico pode ser executado direto pressionado ambos botões bimanual simultaneamente.

        
	      rele indicador trabalho ativo

        -O processo automático pode ser interrompido se:
          -pressionado qualquer botão bimanual
          -acionado o botão de emergência
          -chave seletora alterada para os modos 2/3 ou 3/3 
          -queda de energia

    

	rele indicador emergência modo emergência
          
    modo emergência:
      chave de emergência aberta

      parar motor
      parar prensa em cima
      rele indicador de modo emergência acionado 
      não lê nenhum dos botões 
      registra parada de emergencia ou queda de energia e trava funções aguardando voltar chave seletora de posição no modo 4/4 parado
      
      Toda parte eletrica cortada, mantendo apenas a fonte de 5V dc
     

  Este arquivo foi criado por Cesar Costa e Samuel Oliveira Costa
  São Bernardo do Campo 23/03/21


  DDR que configura o sentido da porta se ela é saída (OUTPUT) ou entrada (INPUT). Substitui a função pinMode().
      se é saída (OUTPUT), usando o 1 ou se é entrada (INPUT), usando o 0.
  PORT que controla se a porta está no estado lógico alto (HIGH) ou baixo (LOW).
  PIN que efetua a leitura das portas configuradas como INPUT pela função pinMode().
  Exemplos:
  Para definir um pino como saída devemos setar seu respectivo bit do
  registrador DDR como 1 e  para defini-lo como entrada seta-lo para 0.
  pinMode(9,OUTPUT);DDRB |= (1 << DDB1);
  pinMode(9,INPUT);DDRB &= ~(1 << DDB1);
*/

#include <EEPROM.h>         //Biblioteca para ler e recuperar dados
#include <TM1637Display.h>  //biblioteca para o display

//pinos para display
#define CLK A5
#define DIO A4
TM1637Display display(CLK, DIO); // inicia display

//controlar registradores
//definições a seguir simplifica escrita direta nos registradores
#define SetBit(RES, BIT)(RES |= (1 << BIT))   // Por BIT em nível alto
#define ClrBit(RES, BIT)(RES &= ~ (1 << BIT)) // Por BIT em nível baixo
#define TstBit(RES, BIT)(RES &  (1 << BIT)) // testar BIT, retorna 0 ou 1
#define CplBit(RES, BIT)(RES ^= (1 << BIT)) // Inverter estado do BIT
// definir portas de Relés
//Legenda Nome    var Pino  Registrador
#define RelePincaA 5 //  Pinça Abrir   D13 PB5
#define RelePincaF 4 //  Pinça fecha  D12  PB4
#define ReleMorcaA 3 //  Morça Abre  D11  PB3
#define ReleMorcaF 2 //  Morça Fecha D10  PB2
#define ReleMotorLiga 1 //  Sinal  D9  PB1

// definir portas de Botões
//Legenda   Nome    var Pino  Registrador
#define BotaoPinca 7 //  Botão Pinça Abrir D7  PD7
#define BotaoMorca 6 //  Botão Morça Abrir D6  PD6
#define BotaoMotorLiga 5 //  Botão Motor Liga D5 PD5
#define BotaoIniciar 4 //  Botão Iniciar ciclo D4  PD4
#define BotaoManual 3 //  Botão Manual/Automatico D3  PD3

#define aguardar 1000// tempo automatico
#define espera 600  // tempo manual

//Variaveis de Tempo
unsigned long tempoPincaA = 180000;
unsigned long tempoPincaF = 180000;
unsigned long tempoMorcaA = 180000;
unsigned long tempoMorcaF = 180000;
unsigned long tempoMotor = 5000;
unsigned long ativar;
unsigned long tempoInicial;
//Variaveis de controle
bool estadoPinca = 0; // alterna entre um rele e outro
bool estadoMorca = 0; // alterna entre um rele e outro
bool estadoMotor = 0; // alterna entre motor ligado e desligado
unsigned int contador; // valor exibido no display
unsigned int marcador; // marca posição do a ser gravado na EEPROM

void setup() {

  // DDRC = 0xFF; //Define todos os pinos PC Saida
  DDRB = 0xFF; //Define todos os pinos PB Saida
  DDRD = 0x0; //Define todos como Entrada
  PORTB = 0x0; //Define todos PB LOW
  PORTD = 0xFF; //Define todos PD PullUp


  //EEPROM - - 
// realizar leitura da EEPROM para alternar enderço de escrita constante evitando fadiga prematura dos registros da EEPROM
marcador=EEPROMReadInt(0);          //Ler marcador que tem posição fixa para atualizar posição do contador variavel
if (marcador>998)marcador =2;       // verifica se a nova posição e maior que o final da memoria - se for zera marcador
if (marcador<2)marcador=2;          // na primeira utilização do programa o marcador deve ser definido no segundo emdereço para iniciar a contagem
contador = EEPROMReadInt(marcador); // memoriza contador na variavel
if (contador != EEPROMReadInt(marcador-2)) marcador = marcador +2;      // se o contador == ao anterior evita reescrever o marcador senão
                                                                       //cria nova posição para gravar contagem do ciclo
if (marcador>998)marcador =2;       // verifica se a nova posição e maior que o final da memoria - se for zera marcador 
if (contador != EEPROMReadInt(marcador))EEPROMWriteInt(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza 
if (marcador!=EEPROMReadInt(0))EEPROMWriteInt(0,marcador);         // por ultimo grava novo endereço na memoria do novo ciclo caso for diferente 
// fim do tratamento para evitar fadiga da EEPROM
  

  // Serial.begin(9600); // open the serial port at 9600 bps:
  // Serial.println(marcador);  // imprime o valor da memoria

  display.setBrightness(0x0f); // ajusta brilho do display
  display.showNumberDec(contador, false); //mostra o valor do contador no display

} // Fim void setup


void loop() {

  if (!TstBit(PIND, BotaoManual)) { // se chave for trocada de posicao
    Manual();
  }
  else {
    Automatico();
  }

}// Fim void loop


//-----Funções----------------------------------------------------------------------
//
void Manual() {

  //Pinca Abre
  if (!TstBit(PIND, BotaoPinca)) { //Verifica se Botão foi pressionado
    estadoPinca = !estadoPinca;    // Troca estado da variavel para identificar Abrir ou Fechar mecanismo
    if (estadoPinca)SetBit(PORTB, RelePincaA); //muda estado da porta diretamente no registrador
    else SetBit(PORTB, RelePincaF);            //conforme condição da variavel
    while (!TstBit(PIND, BotaoPinca)) {        // entra no looping enquanto o botão for pressionado
      delay(espera);                         // aguarda um instante antes de confirmar o estado da porta
    }
  }
  ClrBit(PORTB, RelePincaA);//Desliga rele
  ClrBit(PORTB, RelePincaF);//Desliga rele


  // Morça Abre
  if (!TstBit(PIND, BotaoMorca)) {              // idem Pinca Abre
    estadoMorca = !estadoMorca;
    if (estadoMorca)SetBit(PORTB, ReleMorcaA);
    else SetBit(PORTB, ReleMorcaF);
    while (!TstBit(PIND, BotaoMorca)) {
      delay(espera);
    }
  }
  ClrBit(PORTB, ReleMorcaA);//Desliga rele
  ClrBit(PORTB, ReleMorcaF);//Desliga rele


  // Motor Liga
  if (!TstBit(PIND, BotaoMotorLiga)) {   // verifica o estado do botão para entrar no condicional
    estadoMotor = !estadoMotor;  // altera  o estado da variavel lidado/ desligado
    if (estadoMotor)  SetBit(PORTB, ReleMotorLiga); //Liga rele Motor conforme variavel HIGH
    else   ClrBit(PORTB, ReleMotorLiga); //Desliga rele do motor se LOW

    while (!TstBit(PIND, BotaoMotorLiga)) {
      delay(100);
    }
  }


}

void Automatico() {
  ClrBit(PORTB, ReleMotorLiga);//Desliga rele

  if (!TstBit(PIND, BotaoIniciar)) { // aguarda botão iniciar ser pressionado

    SetBit(PORTB, RelePincaF); //Pinça fecha HIGH
    delay(aguardar); // aguardar
    ClrBit(PORTB, RelePincaF); //Pinça fecha LOW
    delay(150); // aguardar

    //SetBit(PORTB, ReleMotorLiga); //Motor Liga
    delay(150);   // aumentar marca na peça 06/03/21
        
    SetBit(PORTB, ReleMorcaF); //Morça fecha HIGH

    delay(150);
    SetBit(PORTB, ReleMotorLiga); //Motor Liga
    delay(750); // aguardar
    ClrBit(PORTB, ReleMotorLiga); //Motor Desliga
    delay(150);
    ClrBit(PORTB, ReleMorcaF); //Morça fecha LOW
    delay(aguardar); // aguardar
//
    SetBit(PORTB, RelePincaA); //Pinça Abre HIGH
    SetBit(PORTB, ReleMorcaA); //Morça Abre HIGH
    delay(aguardar); // aguardar
    ClrBit(PORTB, RelePincaA); //Pinça Abre LOW
    ClrBit(PORTB, ReleMorcaA); //Morça Abre LOW
    delay(100); // aguardar

    contador++; //soma no contador
    if (contador > 9999) contador = 0;
    if (contador != EEPROMReadInt(marcador))EEPROMWriteInt(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza
    display.showNumberDec(contador, false); //mostra o valor do contador no display
    //Serial.println(contador);   // envia o valor do contador via serial
    while (!TstBit(PIND, BotaoIniciar)) {
      delay(50);

    }
  }
  delay(20);
  if (!TstBit(PIND, BotaoMotorLiga)) {
    tempoInicial    = millis();
    delay(20);
    while (!TstBit(PIND, BotaoMotorLiga)) {
      if (millis() - tempoInicial > 2000) {
        contador = 0;
        if (contador != EEPROMReadInt(marcador))EEPROMWriteInt(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza
        display.showNumberDec(contador, false); //mostra o valor do contador no display
        //Serial.println(contador); // envia o valor do contador via serial

        delay(200);
        break;
      }
    }
  }
  delay(20); // aguardar
}


//EEPROM  - -
//procedimentos para persistir variaveis na memoria do sistema apos cada configuração escolhida em tempo de usuario

void EEPROMWriteInt(int address, int value) {
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  EEPROM.write(address, two);
  EEPROM.write(address + 1, one);

}//end EEPROMWriteLong


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO


unsigned int EEPROMReadInt(int address) {
  int two = EEPROM.read(address);
  int one = EEPROM.read(address + 1);

  return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF) ;
}//end EEPROMReadLong
