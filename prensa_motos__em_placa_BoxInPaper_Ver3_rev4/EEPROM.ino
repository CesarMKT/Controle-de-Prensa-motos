//EEPROM  - -
//procedimentos para persistir variaveis na memoria do sistema apos cada configuração escolhida em tempo de usuario

void EEPROMIniciar(){

  //EEPROM - - 
// realizar leitura da EEPROM para alternar enderço de escrita constante evitando fadiga prematura dos registros da EEPROM
marcador=EEPROMReadLong(0);          //Ler marcador que tem posição fixa para atualizar posição do contador variavel
if (marcador>990)marcador =4;       // verifica se a nova posição e maior que o final da memoria - se for zera marcador
if (marcador<4)marcador=4;          // na primeira utilização do programa o marcador deve ser definido no segundo emdereço para iniciar a contagem
contador = EEPROMReadLong(marcador); // memoriza contador na variavel
if (contador != EEPROMReadLong(marcador-4)) marcador = marcador +4;      // se o contador == ao anterior evita reescrever o marcador senão
                                                                       //cria nova posição para gravar contagem do ciclo
if (marcador>990)marcador =4;       // verifica se a nova posição e maior que o final da memoria - se for zera marcador 
if (contador != EEPROMReadLong(marcador))EEPROMWriteLong(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza 
if (marcador!=EEPROMReadLong(0))EEPROMWriteLong(0,marcador);         // por ultimo grava novo endereço na memoria do novo ciclo caso for diferente 
pot1=EEPROMReadInt(996);
  
}// fim do tratamento para evitar fadiga da EEPROM

void EEPROMWriteLong(int address, unsigned long value) {
   byte four = (value & 0xFF);
   byte three = ((value >> 8) & 0xFF);
   byte two = ((value >> 16) & 0xFF);
   byte one = ((value >> 24) & 0xFF);
   EEPROM.write(address, four);
   EEPROM.write(address + 1, three);
   EEPROM.write(address + 2, two);
   EEPROM.write(address + 3, one);
}//end EEPROMWriteLong


//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO


unsigned long EEPROMReadLong(int address) {
   long four = EEPROM.read(address);
   long three = EEPROM.read(address + 1);
   long two = EEPROM.read(address + 2);
   long one = EEPROM.read(address + 3);
   return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}//end EEPROMReadLong




//EEPROM  - -
//procedimentos para persistir variaveis na memoria do sistema apos cada configuração escolhida em tempo de usuario

void EEPROMWriteInt(int address, int value) {
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  EEPROM.write(address, two);
  EEPROM.write(address + 1, one);

}//end EEPROMWriteint

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO


unsigned int EEPROMReadInt(int address) {
  int two = EEPROM.read(address);
  int one = EEPROM.read(address + 1);

  return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF) ;
}//end EEPROMReadint
