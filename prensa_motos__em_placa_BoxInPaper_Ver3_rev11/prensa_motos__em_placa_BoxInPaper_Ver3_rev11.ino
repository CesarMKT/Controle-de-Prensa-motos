

/*   Controle de prensa para automatização de produção

  Premissas;

  "entradas"
   Contador 8 digitos  pinos 13-11-18
   controle de temporizador com potenciometro -tempo de espera entre descida da prensa  A2
   controle de temporizador com potenciometro - tempo de pinça alimentação  A3
   Sensor do pistão PD2 = 2
   BT Parada  PD3 =3
   BT Alimentação PD4 = 4
   comando bimanual esquerdo PD6 = 6
   comando bimanual direito  PD5 = 5
   pedal para acionamento manual com as mãos livres.  PD7 = 7

   chave seletora 1/4 automático A0 = 14  e A1 = 15  LOW
   chave seletora 2/4 manual com alimentador A0 = 14 LOW A1=15 HIGH semiAuto
   chave seletora 3/4 manual sem alimentador A0 = 14 HIGH A1=15 LOW
   Desligado/ zerar contador  A0 = 14 HIGH A1=15 HIGH



  "saídas"
   rele de acionamento prensa  PB4=12
   rele de acionamento Alimentador PB2=10
   rele acionamento do motor PB1 = 9
   rele de indicação modo de trabalho PB0 = 8

  "trabalhos a ser executados "

  Seletor na posição “Manual” – já implantado
  Aciona o pistão do alimentador e aguardo o pressionar bi manual para acionar o martelo.
  Aguardo pressionar Alimentador acionando o mesmo e desacionado quando solta o botão
  Conta as peças a cada saída do sensor

  Seletor na posição “Semi-alto” – já implantado
  Aguardo o pressionar bi manual para acionar o alimentador e o martelo simultâneos.
  Faz uso do tempo programado no AUTO-TEMPO para regular o tempo do alimentador
  Conta as peças a cada saída do sensor


  Seletor na posição “AUTO-TEMPO” – já implantado
  Aguardo pressionar o INICIO para acionar o alimentador e o martelo simultâneos.
  Conta as peças a cada saída do sensor e para o martelo X tempo e retorna alimentador na metade do X tempo
  Após dez peças contadas trava no modo automático indicada uma luz
  Quando parada é possível reduzir e aumentar tempo nos botões bi manual.
  Após pressionar ALIMENTADOR 10´s  zerar o contador

  Seletor na posição “AUTO-SENSOR” – * A IMPLANTAR
  Aguardo  pressionar o INICIO para acionar o martelo e acender a luz do modo automático. Sempre que o sensor entrar o pistão do alimentador é acionado e quando sair é desacionado e conta uma peça. (Igual ao quadro Elétrico)
  pressionar ALIMENTADOR 10´s zerar o contador

  //

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

  //    modo automático 1/4:
  //      chave de emergência fechada
  //      chave seletora de posição no modo 1/4 automático
  //        -Acionamento para o modo automatico será executado descidas e alimentação dois reles conforme  ajuste de ponteciometros
  //  -mantendo um dos botões do bicomando pressionado por 3 ou mais prensagens seguidas o auto trava
  //        caso um dos botões for prescionado e solto a prensa vai descer
  //        uma ou duas vezes e parar se o botão estiver solto antes do valor determinado.
  //        -O acionamento automatico pode ser executado direto pressionado ambos botões bimanual simultaneamente.
  //
  //
  //        rele indicador trabalho ativo
  //
  //        -O processo automático pode ser interrompido se:
  //          -pressionado qualquer botão bimanual
  //          -acionado o botão de emergência
  //          -chave seletora alterada para os modos 2/3 ou 3/3
  //          -queda de energia
  //
  //


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

#include <Arduino.h>
#include <EEPROM.h>          //Biblioteca para ler e recuperar dados
#include <LedController.hpp> //incluir a biblioteca do dsiplay de 8 digitos

LedController<1, 1> lc = LedController<1, 1>(); //declara lc para

//controlar registradores
//definições a seguir simplifica escrita direta nos registradores
#define SetBit(RES, BIT) (RES |= (1 << BIT))  // Por BIT em nível alto
#define ClrBit(RES, BIT) (RES &= ~(1 << BIT)) // Por BIT em nível baixo
#define TstBit(RES, BIT) (RES & (1 << BIT))   // testar BIT, retorna 0 ou 1
#define CplBit(RES, BIT) (RES ^= (1 << BIT))  // Inverter estado do BIT
// definir portas de Relés

//Legenda Nome    var Pino  Registrador
#define RelePrensa 4      //  Rele Prensa D10 PB2
#define ReleAlimentador 1 //  rele Alimentador D7  PD7
#define ReleMotor 2       //  Motor  D8  PB0
#define ReleSinaliza 0    //  sinaliza D9  PB1

// definir portas de Botões
//Legenda   Nome    var Pino  Registrador
#define BtSensor 2           //  D2 PD2
#define BtParada 3           //   D3  PD3
#define BtAlimentador 4      //   D4  PD4
#define BtBimanualDireito 5  //  D5  PD5
#define BtBimanualEsquerdo 6 //  D6  PD6
#define BtPedal 7            //   D6  PD6
#define seletor1 0           //  A0 PC0
#define seletor2 1           //  A4 PC4

#define aguardar 1000        // tempo automatico
#define espera 600           // tempo manual
#define limiteContadorAuto 3 // quantas peças bater antes de travar automatico

#define maxAlimentador 2000
#define minAlimentador 130

unsigned long contador; // valor exibido no display
unsigned long marcador; // marca posição do a ser gravado na EEPROM
unsigned long pot1;
unsigned long timerPrensa;
unsigned long timerAlimentador = 0;
unsigned int contadorAuto;
unsigned long tempoZerar = 10000;
unsigned long tempoPressionado;


unsigned long currentAuto;
unsigned long previusAuto;
unsigned long qtdeAuto;
unsigned long divisor;
byte digito;
byte unidade;
byte exibirDisplay;
bool voltarDisplay;
bool registrarContador;
bool chaveAuto;
bool travaAuto;
byte chave;
bool chaveMotor = 0;
bool inicioContador;
bool ligaPrensa = 0;
bool ligaAlimentador = 0;
bool ligaMotor = 0;
bool ligaLuz = 0;
bool btDireito;
bool btEsquerdo;
bool btPedal;
bool btAlimentador;
bool sensor =HIGH;
bool sair;
bool displayTempo = LOW;


volatile byte intSensor, offSensor = 0; //variavel para escrita pela interupção

void setup()
{
  delay(1000); //delay para o diplay ligar

  // DDRC = 0xFF; //Define todos os pinos PC Saida
  //  DDRB = 0xFF; //Define todos os pinos PB Saida
  //  DDRD = 0x0; //Define todos como Entrada
  PORTB = 0x0; //Define todos PB LOW
  //  PORTD = 0xFF; //Define todos PD PullUp
  pinMode(A0, INPUT_PULLUP); // seletor1 - PC0
  //pinMode(A5, INPUT_PULLUP);  //vazia
  pinMode(A1, INPUT_PULLUP); // seletor2 PC1
  pinMode(A2, INPUT);        // potenciomentro prensa - PC2
  pinMode(A3, INPUT);        // potenciomentro pinça alimentador - PC3
  pinMode(0, INPUT_PULLUP);  // RX
  pinMode(1, INPUT_PULLUP);  // TX
  pinMode(2, INPUT_PULLUP);  // Sensor Prensa PD2
  pinMode(3, INPUT_PULLUP);  // Parada  PD3
  pinMode(4, INPUT_PULLUP);  // alimentação semi-automatico PD4
  pinMode(5, INPUT_PULLUP);  // bimanualDireito PD5
  pinMode(6, INPUT_PULLUP);  // bimanualEsquerdo PD6
  pinMode(7, INPUT_PULLUP);  // Pedal - PD7
  pinMode(8, OUTPUT);        // rele LUZ - PB0
  pinMode(9, OUTPUT);        // rele pinça alimentador- PB1
  pinMode(12, OUTPUT);       // rele prensa - PB4
  pinMode(13, OUTPUT);       // CLOCK Display - PB5
  pinMode(11, OUTPUT);       // DIN Display - PB3
  pinMode(A4, OUTPUT);       // CS Display - PC4 18 = A4

  ClrBit(PORTB, RelePrensa);
  ClrBit(PORTB, ReleAlimentador);
  ClrBit(PORTB, ReleMotor);
  ClrBit(PORTB, ReleSinaliza);

  EEPROMIniciar(); // começar tratativa para recuperar dados do contador

  //   Serial.begin(9600); // open the serial port at 9600 bps:
  //   Serial.println(marcador);  // imprime o valor da memoria

  // Display / Contador
  delay(100); // delay para reles desacionar

  lc.init(18);        //O pin clock = 13 e o DIN = 11  CS=12  (CS pode ser alterado...) //initilize a ledcontroller with a hardware spi and one row
  // Display / Contador
  delay(100);         // delay para reles desacionar
  displayN(contador); // exibe o valor da memoria no display

  // encoder-inicio
  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2
  pinMode(3, INPUT_PULLUP); // internalเป็น pullup input pin 3
  //Setting up interrupt
  // AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0CHANGE, CHANGE);

  while (1)
  {
    delay(10);
    if (!TstBit(PINC, 0) && !TstBit(PINC, 1))
      automatico(); //chave seletora 1/4 automático A0 = 14  e A1 = 15  LOW
    if (!TstBit(PINC, 0) && TstBit(PINC, 1))
      semiAuto(); //chave seletora 2/4 manual com alimentador A0 = 14 LOW A1=15 HIGH semiAuto
    if (TstBit(PINC, 0) && !TstBit(PINC, 1))
      manual(); //chave seletora 3/4 manual sem alimentador A0 = 14 HIGH A1=15 LOW
    if (TstBit(PINC, 0) && TstBit(PINC, 1))
      autoEletric(); //Automatico imitando painel elétrico 4/4  A0 = 14 HIGH A1=15 HIGH
  }

} // Fim void setup

void loop()
{

} // Fim void loop
