

void displayN(unsigned long valor) { //exibe o numero na tela
lc.refreshSegments();

  lc.clearMatrix();   //clear the matrix just to be sure there is nothing on it
  byte digito = 0;
  divisor = valor;
  do {
    unidade = divisor % 10;
    lc.setDigit(0, digito, unidade , false);
    divisor = divisor / 10;
    digito++;
  } while (divisor > 0);
}

void displayP1(unsigned  long valor) { // escreve 'P1' e exibe numero na tela
lc.refreshSegments(); // reseta matrix
  lc.clearMatrix();   //clear the matrix just to be sure there is nothing on it
  lc.setChar(0, 7, 'P' , false);
  lc.setDigit(0, 6, 1 , false);
  byte digito = 0;
  divisor = valor;
  do {
    unidade = divisor % 10;
    lc.setDigit(0, digito, unidade , false);
    divisor = divisor / 10;
    digito++;
  } while (divisor > 0);
}

void displayP2(unsigned  long valor) {// escreve 'P2' e exibe numero na tela
lc.refreshSegments(); // reseta matrix
  lc.clearMatrix();   //clear the matrix just to be sure there is nothing on it
  lc.setChar(0, 7, 'P' , false);
  lc.setDigit(0, 6, 2 , false);
  byte digito = 0;
  divisor = valor;
  do {
    unidade = divisor % 10;
    lc.setDigit(0, digito, unidade , false);
    divisor = divisor / 10;
    digito++;
  } while (divisor > 0);
}

void displayOff(){

lc.refreshSegments();

   /*
  
  lc.setIntensity(9);//Set them to the lowest brightness
  lc.shutdownAllSegments();
  lc.resetMatrix();
  lc.resetBuffers(8,8); //não funciona

setScanLimit  KEYWORD2

refreshSegments KEYWORD2
clearSegment  KEYWORD2
activateAllSegments   KEYWORD2

activateSegment KEYWORD2
shutdownSegment KEYWORD2
getSegmentCount     KEYWORD2
clearMatrix KEYWORD2
resetMatrix   KEYWORD2
displayOnSegment  KEYWORD2

*/
}
