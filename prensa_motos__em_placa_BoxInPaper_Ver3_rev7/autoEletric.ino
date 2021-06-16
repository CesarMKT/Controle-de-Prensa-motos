//    modo Automatico eletrico
void autoEletric()
{
  bool btDireito; // para zerar contador, bt Star/Stop Automatico
  unsigned long tempoPressionado;

  ligaPrensa = 0;
  ligaAlimentador = 0;
  ligaMotor = 0;
  ligaLuz = 0;
  btAlimentador = 0;

  atualizar();

  //      chave seletora de posição no modo 4/4 maquina parada -(todas as portas seletora aberta)
  //Desligado/ zerar contador  A0 = 14 HIGH A1=15 HIGH
  while (TstBit(PINC, 0) && TstBit(PINC, 1))
  {

    if (!TstBit(PIND, 3))
      btAlimentador = HIGH;               // se o botão foi pressionado registra o btAlimentador
    if (TstBit(PIND, 3) && btAlimentador) // se o botão foi solto  execulta função para ligar/deliga prensa
    {
      btAlimentador = LOW;
      ligaPrensa = !ligaPrensa;
    }

    unsigned long timer = millis() + 10;
    while (!TstBit(PIND, 2)) //botão pressionado
    {
      if (timer <= millis())
      {
        ligaAlimentador = HIGH; //ativa o o botão depois de 10 milisegundos
        break;
      }
    }                       //end while botão pressionado
    while (TstBit(PIND, 2)) //botão solto
    {
      if (timer <= millis())
      {
        ligaAlimentador = LOW; //ativa o o botão depois de 10 milisegundos
        contar();
        break;
      }
    } // end while botão solto

    //    -como zerar display
    //                    botão do alimentador prescionado pelo tempo da variavel pré selecionada

    if (!TstBit(PIND, 4) && !btDireito)
    { //se o botão é prescionado e a variavel é LOW
      btDireito = HIGH;
      tempoPressionado = millis();
    }
    if (TstBit(PIND, 4) && btDireito)
      btDireito = LOW; // se o botão for solto variavel passa a ser LOW
    if (!TstBit(PIND, 4) && btDireito && millis() - tempoPressionado >= tempoZerar)
    {
      contador = 0; //zerar contador
      if (contador != EEPROMReadLong(marcador))
        EEPROMWriteLong(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza
      displayN(contador);
      tempoPressionado = millis();
    }
    //fim zerar display

    //      *visor com contagem de 8 dígitos.
    //      contagem no limite de 99.999.999 (noventa e nove milhões novecentos e noventa e nove mil novecentos e noventa e nove peças).
    //      será somado mais uma peça a cada prensagem
    //      memoria será mantida após ser desligado
    //

  } //end while
} //end parado
