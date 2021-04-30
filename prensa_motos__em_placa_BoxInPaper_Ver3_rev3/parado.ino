//    modo parado
void parado() {
  bool btDireito; // para zerar contador, bt Star/Stop Automatico
  unsigned long tempoPressionado;

  ligaPrensa = 0;
 ligaAlimentador = 0;
 ligaMotor = 0;
 ligaLuz=0;
 atualizar();

  //      chave seletora de posição no modo 4/4 maquina parada -(todas as portas seletora aberta)
  //Desligado/ zerar contador  A0 = 14 HIGH A1=15 HIGH
  while (TstBit(PINC, 0) && TstBit(PINC, 1)) {

    //        - Desligar motor se estiver ligado
    ClrBit(PORTB, 1); //Desligar motor
    ClrBit(PORTB, 2); //Desligar Pinça
    ClrBit(PORTB, 4); //Desligar Prensa
    ClrBit(PORTB, 0); //Desligar Luz Auto

    //  - reseta motor caso emergencia ou falta de energia
    //        - Botão 'parada' alterar o display para leitura do potenciometro prensa
    //        - Botão 'alimentação' alterar o display para leitura do potenciometro pinça alimentalção
    //          *isso facilita leitura do potenciometro e possibilita setup mais rapida em repetições
   
      while (!TstBit(PIND, 5)) { // soma 1 unidade pot 1
        pot1=pot1+5;
        if (pot1>=maxAlimentador)pot1=maxAlimentador;
        displayP1(pot1);
        delay(100);
      }//end while pot1
      while (!TstBit(PIND, 6)) {
        pot1=pot1-5;
        if(pot1<=minAlimentador)pot1=minAlimentador;
        displayP1(pot1);
        delay(100);
      }//end While pot1
      
       if(!TstBit(PIND,3)) displayN(contador);
    
    
    delay(32);
    //    -como zerar display
    //                    botão bimanual prescionado por mais de 10 segundos

    if (!TstBit(PIND, 3) && !btDireito) { //se o botão é prescionado e a variavel é LOW
      btDireito = HIGH;
      tempoPressionado = millis();
    }
    if (TstBit(PIND, 3) && btDireito)  btDireito = LOW; // se o botão for solto variavel passa a ser LOW
    if (!TstBit(PIND, 3) && btDireito && millis() - tempoPressionado >= 10000) {
      contador = 0; //zerar contador
      if (contador != EEPROMReadLong(marcador))EEPROMWriteLong(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza
      displayN(contador);
      tempoPressionado = millis();
    }



    //      *visor com contagem de 8 dígitos.
    //      contagem no limite de 99.999.999 (noventa e nove milhões novecentos e noventa e nove mil novecentos e noventa e nove peças).
    //      será somado mais uma peça a cada prensagem
    //      memoria será mantida após ser desligado
    //

  }//end while
}//end parado
