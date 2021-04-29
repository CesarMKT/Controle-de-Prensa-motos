//    modo parado
void parado() {
  bool btDireito;
  unsigned long tempoPressionado;

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
    if (!TstBit(PIND, 5) || !TstBit(PIND, 6)) {
      while (!TstBit(PIND, 5)) { // exibe o pot1
        pot1 = map(analogRead(A2), 0, 1023, minPrensa, maxPrensa); // le e memoriza pot1
        displayP1(pot1);
        delay(200);
      }//end while pot1
      while (!TstBit(PIND, 6)) {
        pot2 = map(analogRead(A3), 0, 1023, minAlimentador, maxAlimentador); // le e memoriza pot1
        displayP2(pot2);
        delay(200);
      }//end While pot2
    }//end if display pot 1 e 2
    displayN(contador);
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
