//    modo Automatico eletrico
void autoEletric()
{

    attachInterrupt(0, ai0CHANGE_eletrico, CHANGE);



    
  bool btZerarContador; // para zerar contador, bt Star/Stop Automatico

  ligaPrensa = 0;
  ligaAlimentador = 0;
  ligaMotor = 0;
  ligaLuz = 0;
  btAlimentador = 0;
  sensor = HIGH;
  delay(10);
  atualizar();
  delay(100);
  //      chave seletora de posição no modo 4/4 maquina parada -(todas as portas seletora aberta)
  //Desligado/ zerar contador  A0 = 14 HIGH A1=15 HIGH
  while (TstBit(PINC, 0) && TstBit(PINC, 1))
  {

    atualizar();
    if (!TstBit(PIND, 3))
      btAlimentador = HIGH;               // se o botão foi pressionado registra o btAlimentador

    if (TstBit(PIND, 3) && btAlimentador) // se o botão foi solto  execulta função para ligar/deliga prensa
    {
      btAlimentador = LOW;
      ligaLuz = !ligaLuz;
      ligaPrensa = !ligaPrensa;
    }

    //  sensor alimentador
    if (TstBit(PIND, 2))    {
      ligaAlimentador = HIGH;
      atualizar();
      if (!sensor) {
        sensor = HIGH;
        contar();

      }


    }
    if (!TstBit(PIND, 2)) {
      ligaAlimentador = LOW;
      atualizar();
      sensor = LOW;
    }
    // end while sensor e alimentador

    //    -como zerar display
    if (!ligaPrensa) { //                    botão do alimentador prescionado pelo tempo da variavel pré selecionada

      if (!TstBit(PIND, 4) && !btZerarContador)
      { //se o botão é prescionado e a variavel é LOW
        btZerarContador = HIGH;
        tempoPressionado = millis();
      }
      if (TstBit(PIND, 4) && btZerarContador)
        btZerarContador = LOW; // se o botão for solto variavel passa a ser LOW
      if (!TstBit(PIND, 4) && btZerarContador && millis() - tempoPressionado >= tempoZerar)
      {
        contador = 0; //zerar contador
        if (contador != EEPROMReadLong(marcador))
          EEPROMWriteLong(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza
        displayN(contador);
        tempoPressionado = millis();
      }
    }//fim zerar display

    //      *visor com contagem de 8 dígitos.
    //      contagem no limite de 99.999.999 (noventa e nove milhões novecentos e noventa e nove mil novecentos e noventa e nove peças).
    //      será somado mais uma peça a cada prensagem
    //      memoria será mantida após ser desligado
    //

  } //end while
} //end parado
