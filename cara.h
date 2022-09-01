#include <LedControl.h>

const int DIN_PIN = 10;
const int CS_PIN = 9;
const int CLK_PIN = 8;

const int matriz_boca = 0;
const int matriz_ojo_der = 1;
const int matriz_ojo_izq = 2;


//==================================================
// MATRIZ 0 : BOCA          (REVES)
// MATRIZ 1 : OJO DERECHO   (REVES)
// MATRIZ 2 : OJO IZQUIERDO (DERECHO)
//==================================================

const uint64_t TEST[] = {
  0xffffffffffffffff,    // LLENO
  0x0000000000000000      // VACIO
};  

// AL REVES
const uint64_t OJO_DERECHA[] = {
  0x3c4281818181423c,   // 0 - ABIERTO
  0x00000000ff000000,   // 1 - CERRADO
  0x040a11214181423c,   // 2 - ASUSTADO / TRISTE
  0x000000ff8181423c,   // 3 - ABURRIDO 1
  0x0000000000ff423c,   // 4 - ABURRIDO 2
  0x3c4281999981423c,   // 5 - FRENTE
  0x3c428d8d8181423c,   // 6 - ARRIBA - DERECHA
  0x3c42b1b18181423c,   // 7 - ARRIBA - IZQUIERDA
  0x3c4281818d8d423c,   // 8 - ABAJO - DERECHA
  0x3c428181b1b1423c    // 9 - ABAJO -IZQUIERDA
};

// AL DERECHO
const uint64_t OJO_IZQUIERDA[] = {
  0x3c4281818181423c,  // 0 - ABIERTO
  0x000000ff00000000,  // 1 - CERRADO
  0x3c42814121110a04,  // 2 - ASUSTADO / TRISTE
  0x3c428181ff000000,  // 3 - ABURRIDO 1
  0x3c42ff0000000000,  // 4 - ABURRIDO 2
  0x3c4281999981423c,  // 5 - FRENTE
  0x3c428181b1b1423c,  // 6 - ARRIBA - DERECHA
  0x3c4281818d8d423c,  // 7 - ARRIBA - IZQUIERDA
  0x3c42b1b18181423c,  // 8 - ABAJO - DERECHA
  0x3c428d8d8181423c   // 9 - ABAJO -IZQUIERDA
};  

// AL REVES
const uint64_t BOCA[] = {
  0x000081ff7e000000,   // 0 - sonrisa 
  0x0000181800000000,   // 1 - dormir 1
  0x00183c3c18000000,   // 2 - dormir 2
  0x81ffffff7e3c0000,   // 3 - boca abierta
  0x000044aa11000000,   // 4 - asustado
  0x00183c7e7e3c1800,   // 5 - sorprendido
  0x0081ffff7e000000    // 6 - risa
};

LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 3);


unsigned int iCara=1;
unsigned int iPaso=0;

const int iCaraDuerme = 1;
const int iCaraSonrie = 2;
const int iCaraHabla = 3;
const int iCaraTest=4;
const int iCaraVacio=5;
const int iCaraAsustado = 6;
const int iCaraNormal=7;
const int iCaraTriste=8;
const int iCaraAburrido=9;
const int iCaraSorpresa=10;
const int iCaraRisa=11;
const int iCaraVistaFrente=12;
const int iCaraVistaArribaDerecha=13;
const int iCaraVistaArribaIzquierda=14;
const int iCaraVistaAbajoDerecha=15;
const int iCaraVistaAbajoIzquierda=16;
const int iCaraGuinaOjo=17;

bool Cara_bTemporizador=false;
int Cara_iTemporizador = 1000;
unsigned long Cara_TiempoInicioTemporizador = 0;

//============================================================================
//============================================================================
void InicializaTemporizador(int iTiempo)
{
  Cara_iTemporizador=iTiempo;
  Cara_bTemporizador=true;
  Cara_TiempoInicioTemporizador=millis();
}

//============================================================================
// Espera no bliqueante del loop
//============================================================================
bool Cara_Esperar()
{
    if (millis() >= Cara_TiempoInicioTemporizador+Cara_iTemporizador)
    {
      // Fin periodo
      Cara_iTemporizador=0;
      Cara_bTemporizador=false;
    }

    return !Cara_bTemporizador;
}

//=================================================================
//=================================================================
void Cara_Establece(int iiCara)
{
  iCara=iiCara;
  iPaso=0;
  Cara_bTemporizador=false;
}

//=================================================================
//=================================================================
void Cara_Inicializa() {
  for (int i = 0; i < 3; i++) {
    display.clearDisplay(i);
    display.shutdown(i, false);
    display.setIntensity(i, 5);
  }
  Cara_Establece(iCaraDuerme);
  Serie_Envia('I',"Cara");
}


//=================================================================
//=================================================================
void PintaIcono(int iPanel, uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed( iPanel, i, j, bitRead(row, j));
    }
  }
}

void normal_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[0]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[0]);
  InicializaTemporizador(2000);
}

void normal_2()
{
  PintaIcono(matriz_ojo_der, OJO_DERECHA[1]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[1]);
  InicializaTemporizador(100);
}

void normal_start()
{
  PintaIcono(matriz_boca, TEST[1]); // sin boca
  Rgb_Aleatorio();
  iPaso=1;
}



//=======================================================================
//=======================================================================
void normal() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) normal_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) normal_1();
  }
}


void duerme_start()
{
  PintaIcono(matriz_ojo_der, OJO_DERECHA[1]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[1]);
  Rgb_BlancoNegro();
 iPaso=1;
}

void duerme_1()
{
  PintaIcono(matriz_boca, BOCA[1]);
  InicializaTemporizador(1000); 
}

void duerme_2()
{
  PintaIcono(matriz_boca, BOCA[2]);
  InicializaTemporizador(1000);
}


//=======================================================================
//=======================================================================
void duerme()
{
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  { 
    if (iPaso==0) duerme_start();
    if (iPaso==2) duerme_2();
    if (iPaso==1) duerme_1();
  }
}


void sonrie_start()
{
  PintaIcono(matriz_boca, BOCA[0]);
  Rgb_Aleatorio();
  iPaso=1;
}


//=======================================================================
//=======================================================================
void sonrie() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) sonrie_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) normal_1();
  }
}

//=======================================================================
//=======================================================================
void guina_ojo_start()
{
  PintaIcono(matriz_ojo_der, OJO_DERECHA[1]); // OJO DERECHO GUIÑADO
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[0]);
  PintaIcono(matriz_boca, BOCA[0]);// BOCA SONRISA
  Rgb_Party();
  InicializaTemporizador(2000);
  //iPaso=1;
}

//=======================================================================
//=======================================================================
void guina_ojo() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) {normal_start(); normal();}} 
  }
  else
  {
    if (iPaso==0) guina_ojo_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) normal_1();
  }
}


//=======================================================================
void test()
{
   iPaso=0;
   PintaIcono(matriz_ojo_der, TEST[0]);
   PintaIcono(matriz_ojo_izq, TEST[0]);
   PintaIcono(matriz_boca, TEST[0]);
}

void vacio()
{
   iPaso=0;
   PintaIcono(matriz_ojo_der, TEST[1]);
   PintaIcono(matriz_ojo_izq, TEST[1]);
   PintaIcono(matriz_boca, TEST[1]);
}


void habla_start()
{
  Rgb_Azul();
  iPaso=1;
}

void habla_1()
{
  PintaIcono(matriz_boca, BOCA[3]);
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[0]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[0]);
  InicializaTemporizador(250);  
}

void habla_2()
{
  PintaIcono(matriz_boca, BOCA[0]);
  InicializaTemporizador(250);  
}

void habla_3()
{
  PintaIcono(matriz_boca, BOCA[3]);
  InicializaTemporizador(250);  
}
  
void habla_4()
{
  PintaIcono(matriz_ojo_der, OJO_DERECHA[1]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[1]);
  InicializaTemporizador(100);
}

//=======================================================================
//=======================================================================
void habla()
{
    if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>11) iPaso=1;}
  }
  else
  { 
    if (iPaso==0) habla_start(); 
    if (iPaso==11) habla_4();
    if (iPaso==10) habla_2();
    if (iPaso==9) habla_3();
    if (iPaso==8) habla_2();
    if (iPaso==7) habla_3();
    if (iPaso==6) habla_2();
    if (iPaso==5) habla_3();
    if (iPaso==4) habla_2();
    if (iPaso==3) habla_3();
    if (iPaso==2) habla_2();
    if (iPaso==1) habla_1();
  }

}





void asustado_start()
{
  PintaIcono(matriz_boca, BOCA[4]);
  Rgb_VioletaVerde();
  iPaso=1;
}

void asustado_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[2]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[2]); 
  InicializaTemporizador(2000);
}



void asustado_2()
{
  PintaIcono(matriz_ojo_der, OJO_DERECHA[1]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[1]);
  InicializaTemporizador(100);
}

//=======================================================================
//=======================================================================
void asustado() {

if (Cara_bTemporizador)
{
  bool bFin=Cara_Esperar();  
  if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
}
else
{
  
 if (iPaso==0) asustado_start();
 if (iPaso==2) asustado_2();
 if (iPaso==1) asustado_1();
}
}

// OJOS DE ASUSTADO (CON PARPADEO) Y SIN BOCA
void triste()
{
  if (Cara_bTemporizador)
{
  bool bFin=Cara_Esperar();  
  if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
}
else
{
  
 if (iPaso==0) normal_start();
 if (iPaso==2) asustado_2();
 if (iPaso==1) asustado_1();
}
}


void aburrido_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[3]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[3]); 
  InicializaTemporizador(2000);
}



void aburrido_2()
{
  PintaIcono(matriz_ojo_der, OJO_DERECHA[4]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[4]);
  InicializaTemporizador(100);
}

// OJOS DE ABURRIDO 1 -2 SIN BOCA
void aburrido()
{
  if (Cara_bTemporizador)
{
  bool bFin=Cara_Esperar();  
  if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
}
else
{
  
 if (iPaso==0) normal_start();
 if (iPaso==2) aburrido_2();
 if (iPaso==1) aburrido_1();
}
}


void sorpresa_start()
{
  PintaIcono(matriz_boca, BOCA[5]);
  Rgb_Heat();
  iPaso=1;
}


// RISA : COMO EL NORMAL PERO CON BOCA DE SORPRESA
void sorpresa()
{
    if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) sorpresa_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) normal_1();
  }
}

void risa_start()
{
  PintaIcono(matriz_boca, BOCA[6]);
  Rgb_Party();
  iPaso=1;
}


// RISA : COMO EL NORMAL PERO CON BOCA DE RISA
void risa()
{
    if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) risa_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) normal_1();
  }
}

void VistaFrente_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[5]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[5]);
  InicializaTemporizador(2000);
}

void VistaArribaDerecha_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[6]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[6]);
  InicializaTemporizador(2000);
}

void VistaArribaIzquierda_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[7]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[7]);
  InicializaTemporizador(2000);
}

void VistaAbajoDerecha_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[8]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[8]);
  InicializaTemporizador(2000);
}

void VistaAbajoIzquierda_1()
{
  // PARPADEO
  PintaIcono(matriz_ojo_der, OJO_DERECHA[9]);
  PintaIcono(matriz_ojo_izq, OJO_IZQUIERDA[9]);
  InicializaTemporizador(2000);
}

//=======================================================================
//=======================================================================
void VistaFrente() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) normal_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) VistaFrente_1();
  }
}

//=======================================================================
//=======================================================================
void VistaArribaDerecha() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) normal_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) VistaArribaDerecha_1();
  }
}
//=======================================================================
//=======================================================================
void VistaArribaIzquierda() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) normal_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) VistaArribaIzquierda_1();
  }
}
//=======================================================================
//=======================================================================
void VistaAbajoDerecha() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) normal_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) VistaAbajoDerecha_1();
  }
}
//=======================================================================
//=======================================================================
void VistaAbajoIzquierda() {
  if (Cara_bTemporizador)
  {
    bool bFin=Cara_Esperar();  
    if (bFin) {iPaso++; if (iPaso>2) iPaso=1;}
  }
  else
  {
    if (iPaso==0) normal_start();
    if (iPaso==2) normal_2();
    if (iPaso==1) VistaAbajoIzquierda_1();
  }
}

//============================================================================
//============================================================================
void PintaCara()
{
    switch (iCara) {
      case iCaraDuerme: duerme(); break; 
      case iCaraSonrie: sonrie(); break; 
      case iCaraHabla: habla(); break;
      case iCaraTest: test(); break;
      case iCaraVacio: vacio(); break;
      case iCaraAsustado: asustado(); break;
      case iCaraNormal: normal(); break;
      case iCaraTriste: triste(); break;
      case iCaraAburrido: aburrido(); break;
      case iCaraSorpresa: sorpresa(); break;
      case iCaraRisa: risa();break;
      case iCaraVistaFrente: VistaFrente(); break;
      case iCaraVistaArribaDerecha: VistaArribaDerecha(); break;
      case iCaraVistaArribaIzquierda: VistaArribaIzquierda(); break;
      case iCaraVistaAbajoDerecha: VistaAbajoDerecha(); break;
      case iCaraVistaAbajoIzquierda: VistaAbajoIzquierda(); break;
      case iCaraGuinaOjo: guina_ojo(); break;
      default: normal(); break;
      break;
    }   
}
