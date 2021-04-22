void automatico(){

//  modo automático 1/4:
//if(TstBit(PIND, Emergencia)) return ;// chave de emergência aberta - sai
//      chave seletora de posição no modo 1/4 automático
if(!chaveMotor) return ; // motor deligado - sai

//  -Acionamento para o modo automatico será executado X descidas e alimentação dois reles conforme  ajuste de ponteciometros

  if (!TstBit (PIND, BtAlimentador)){
    ligaPrensa=1; //liga prensa
    if (chaveAuto==0) qtdeAuto = contador;
    chaveAuto = 1;
    if (qtdeAuto-contador>=acionaAuto)  travaAuto=1;
    else travaAuto=0;
  }
   if (TstBit (PIND, BtAlimentador) && chaveAuto == 1) {
    chaveAuto=0;
    if(travaAuto==0)ligaPrensa=0; //desliga prensa
   }
//  -mantendo o botao alimentador pressionado por 3 ou mais prensagens seguidas o auto trava
//        caso um dos botões for prescionado e solto a prensa vai descer 
//        uma ou duas vezes e parar se o botão estiver solto antes do valor determinado. 
     
//   trabalho ativo inicio do cronometro
currentAuto=millis();
if (travaAuto && inicioContador){
  inicioContador=0;
  previusAuto=millis();
  }
if (currentAuto-previusAuto<pot1)ligaPrensa=1;//ligar prensa
if (currentAuto-previusAuto>=pot1)ligaPrensa=0;//desligar prensa
if (currentAuto-previusAuto>=pot2)ligaAlimentador=1;//ligar alimentador


//
//        -O processo automático pode ser interrompido se:
//          -pressionado qualquer botão bimanual
//          -acionado o botão de emergência
//          -chave seletora alterada para os modos 2/3 ou 3/3 
//          -queda de energia

    
}//end automatico
