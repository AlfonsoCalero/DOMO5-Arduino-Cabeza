//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%																												                                                        %%
//%%  LIBRERIA PROPIA: MOVIMIENTO DE CUELLO									%%
//%%                                                                                                                %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%                                                                                                                %%
//%%    - Control de servo X del cuello     (Pin   6)																%%
//%%    - Control de servo Y del cuello     (Pin   7)																%%
//%%                                                                                                                %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//-----------------------------------------------------------------------------
// Cuello
//-----------------------------------------------------------------------------
#include <Servo.h>                        // Libreria Estandar: ServoMotores

//=====================================================================================================================================
// SERVO X : MOVEMOS 180º -> 0º A 180º
//=====================================================================================================================================
Servo servoX;

#define  iPinCuelloX 7

#define iVelocidadLentoX  20
#define  iVelocidadNormalX 10         // Velocidad en milisegundos
#define  iVelocidadRapidoX 5          // Velocidad en milisegundos

// 000 010 020 030 040 050 060 070 080 090 100 110 120 130 140 150 160 170 180
// ---             ---                 ---                 ---             ---

// 000 010 020 030 040 050 060 070 080 090 100 110 120 130 140 150 160
// ---             ---             ---                 ---     ---

#define  iAnguloIzquierda 0         // Limite izquierda
#define  iAnguloDerecha 120          // Limite derecha

int iAnguloCentroX = 65;           // (((iAnguloDerecha-iAnguloIzquierda)/2)+ iAnguloIzquierda)


#define  iAnguloMedioIzquierda 30
#define  iAnguloMedioDerecha 90

int iPosicionActualX =65;


unsigned long TiempoAnterior=0;


//=====================================================================================================================================
// SERVO Y : MOVEMOS 60º -> 20º A 80º
//=====================================================================================================================================
Servo servoY;

#define  iPinCuelloY 6

#define  iVelocidadLentoY 30
#define  iVelocidadNormalY 20         // Velocidad en milisegundos
#define  iVelocidadRapidoY 10         // Velocidad en milisegundos


// 0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90
//           --       --       --       --       --

#define  iAnguloArriba 70            // Limite arriba
#define  iAnguloAbajo 20             // Limite abajo
int iAnguloCentroY = 20;            // (((iAnguloAbajo-iAnguloArriba)/2)+ iAnguloArriba)


#define  iAnguloMedioArriba 50
#define  iAnguloMedioAbajo 20

int iPosicionActualY = 20;

boolean bParo = true;

#define iEsperaRapido 100     // 500 ms. es la espera normal entre movimientos (Si, No, etc...)
#define iEsperaNormal 500     // 500 ms. es la espera normal entre movimientos (test...)
#define  iEsperaLarga 1000      // 1 segundo es la espera larga (ojear x, ojear y, etc...)

// Conteo 5 segundos antes de centrar
bool bSemaforo=false;               // Empezamos el conteo
unsigned long tAntesCentro=0;       // Conteo de tiempo
      
//=====================================================================================================================================
// INICIALIZA
//=====================================================================================================================================
void Cuello_Inicializa()
{
  //iAnguloCentroX = (int)(((iAnguloDerecha - iAnguloIzquierda) / 2) + iAnguloIzquierda);
  //iAnguloCentroY = (int)(((iAnguloAbajo - iAnguloArriba) / 2) + iAnguloArriba);
  // Inicializar: Cuello
  // servo .attach (pin, min, max)
// donde: 
//      pin: el número del pin al que está conectado el servo
//      min (opcional): el ancho del pulso, en microsegundos, correspondiente al ángulo mínimo (0 grados) en el servo (por defecto es 544)
//      max (opcional): el ancho del pulso, en microsegundos, que corresponde al ángulo máximo (180 grados) en el servo (por defecto es 2400)

  servoX.attach(iPinCuelloX, 1000, 2000);                         // Pin Cuello X : Pin 6
  servoY.attach(iPinCuelloY, 1000, 2000);                         // Pin Cuello Y : Pin 7

  servoX.write(iAnguloCentroX);                       // Mueve cuello centro X
  servoY.write(iAnguloCentroY);                       // Mueve cuello centro Y
  
  iPosicionActualX = iAnguloCentroX;                  // Establecemos posicionactual X=0
  iPosicionActualY = iAnguloCentroY;                  // Establecemos posicionactual Y=0
  IncrementoX = 0;
  IncrementoY = 0;
  bParo=true;
  iVelocidad = iVelocidadNormalX;
  TiempoAnterior=0; // Reset a temporizador que lo lleva al centro
  
  Serie_Envia('I', "Cuello");

  //IncrementoY = 1;  // Lo movemos para arriba para qe luego pueda acer el si y el no.
}

//=====================================================================================================================================
//=====================================================================================================================================
void Cuello_Mueve_X()
{
  if (IncrementoX != 0)
  {
    TiempoAnterior=0; // Reset a temporizador que lo lleva al centro
    iPosicionActualX = iPosicionActualX + IncrementoX;
    if (iPosicionActualX >= iAnguloDerecha)
    {
      iPosicionActualX = iAnguloDerecha;   IncrementoX = 0; 
    }
    else
    {
      if (iPosicionActualX <= iAnguloIzquierda)
      {
        iPosicionActualX = iAnguloIzquierda;    IncrementoX = 0;
      }
      else
      {
        servoX.write(iPosicionActualX);
      }
    }
  }
}

//=====================================================================================================================================
//=====================================================================================================================================
void Cuello_Mueve_Y()
{

   
  if (IncrementoY != 0)
  {
    TiempoAnterior=0; // Reset a temporizador que lo lleva al centro  
    iPosicionActualY = iPosicionActualY + IncrementoY;
    if (iPosicionActualY <= iAnguloAbajo)
    {
      iPosicionActualY = iAnguloAbajo;
      IncrementoY = 0;
    }
    else
    {
      if (iPosicionActualY >= iAnguloArriba)
      {
        iPosicionActualY = iAnguloArriba;
        IncrementoY = 0;
      }
      else
      {
        servoY.write(iPosicionActualY);
      }
    }
  }
}

//=====================================================================================================================================
// Segun donde este el cuello gira a la derecha o a la izquierda a la posicion deseada
//=====================================================================================================================================
void GiraCuelloGradosX(int iGrados, int iVelocidad)
{
  //Serie_Envia('X', (String)iGrados);

  if (iPosicionActualX != iGrados)
  {
    TiempoAnterior=0; // Reset a temporizador que lo lleva al centro  
    int inc = 0;
    if (iPosicionActualX > iGrados) inc = -1;
    if (iPosicionActualX < iGrados) inc = 1;
  
  
    for (int i = iPosicionActualX; i != iGrados && !bParo; i = i + inc)
    {
      if (i >= iAnguloIzquierda && i <= iAnguloDerecha)
      {
        servoX.write(i);    // Movemos 1 paso
        iPosicionActualX = i; // Establecemos posicion actual
        Espera(iVelocidad); // Esperamos /leemos el serial/Leemos tacto)
      }
      else bParo=true;
    }
  }
  else bParo=true;
}

//=====================================================================================================================================
// Segun donde este el cuello gira a la derecha o a la izquierda a la posicion deseada
//=====================================================================================================================================
void GiraCuelloGradosY(int iGrados, int iVelocidad)
{
  //Serie_Envia('Y', (String)iGrados);
  if (iPosicionActualY != iGrados)
  {
    TiempoAnterior=0; // Reset a temporizador que lo lleva al centro  
  int inc = 0;
  if (iPosicionActualY > iGrados) inc = -1;
  if (iPosicionActualY < iGrados) inc = 1;

  for (int i = iPosicionActualY; i != iGrados && !bParo; i = i + inc)
  {
    // Controlamos los limites por arriba y abajo
    if (i >= iAnguloAbajo && i <= iAnguloArriba)
    {
      servoY.write(i);    // Movemos 1 paso
      iPosicionActualY = i; // Establecemos posicion actual
      Espera(iVelocidad); // Esperamos /leemos el serial)
    }
    else bParo=true;
  }
    }
  else bParo=true;

}

//=====================================================================================================================================
// DESDE DONDE ESTOY ME MUEVO A LA DERECHA (+) O IZQUIERDA (-) TANTOS GRADOS COMO SE LE PASE
// POR PARAMETRO. SI ES MAS DE UN GRADO, TENEMOS EN CUENTA LA VELOCIDAD
//=====================================================================================================================================
void IncrementaGradosX(int iGrados, int iVelocidad)
{
  //Serie_Envia('X', (String)iGrados);

  int iContador = 0;
  int inc = 0;
  int ii = 0;
  bool bFin = false;

  if (iGrados < 0) inc = -1;
  else inc = 1;


  while (!bFin)
  {
    ii = iPosicionActualX + inc;
    if (inc > 0) if (ii > iAnguloDerecha) bFin = true;
    if (inc < 0) if (ii < iAnguloIzquierda) bFin = true;
    if (!bFin)
    {
      iContador = iContador + inc;
      servoX.write(ii);
      iPosicionActualX = ii;
    }
    if (iContador == iGrados) bFin = true;
    if (!bFin) Espera(iVelocidad);
  }
}


//=====================================================================================================================================
// DESDE DONDE ESTOY ME MUEVO A LA DERECHA (+) O IZQUIERDA (-) TANTOS GRADOS COMO SE LE PASE
// POR PARAMETRO. SI ES MAS DE UN GRADO, TENEMOS EN CUENTA LA VELOCIDAD
//=====================================================================================================================================
void IncrementaGradosY(int iGrados, int iVelocidad)
{
  int iContador = 0;
  int inc = 0;
  int ii = 0;
  bool bFin = false;

  if (iGrados < 0) inc = -1;
  else inc = 1;


  while (!bFin)
  {
    ii = iPosicionActualY + inc;
    if (inc > 0) if (ii > iAnguloAbajo) bFin = true;
    if (inc < 0) if (ii < iAnguloArriba) bFin = true;
    if (!bFin)
    {
      iContador = iContador + inc;
      servoY.write(ii);
      //Serie_Envia('Y', (String)ii);
      iPosicionActualY = ii;
    }
    if (iContador == iGrados) bFin = true;
    if (!bFin) Espera(iVelocidad);
  }
}

//=====================================================================================================================================
//=====================================================================================================================================
void GiraCuelloDerecha(int iVelocidad)
{
  bParo = false;        // La variable indica que se quiere mover
  GiraCuelloGradosX(iAnguloDerecha, iVelocidad);   // ordena ir a la posicion 120ï¿½ con un delay de 25
}

//=====================================================================================================================================
//=====================================================================================================================================
void GiraCuelloIzquierda(int iVelocidad)
{
  bParo = false;        // La variable indica que se quiere mover
  GiraCuelloGradosX(iAnguloIzquierda, iVelocidad);   // ordena ir a la posicion 20ï¿½ con un delay de 50
}

//=====================================================================================================================================
//=====================================================================================================================================
void GiraCuelloCentroX(int iVelocidad)
{
  bParo = false;        // La variable indica que se quiere mover
  GiraCuelloGradosX(iAnguloCentroX, iVelocidad);   // ordena ir a la 70ï¿½ con un delay de 50
}

//=====================================================================================================================================
//=====================================================================================================================================
void GiraCuelloArriba(int iVelocidad)
{
  bParo = false;        // La variable indica que se quiere mover
  GiraCuelloGradosY(iAnguloArriba, iVelocidad);
}

//=====================================================================================================================================
//=====================================================================================================================================
void GiraCuelloAbajo(int iVelocidad)
{
  bParo = false;        // La variable indica que se quiere mover
  GiraCuelloGradosY(iAnguloAbajo, iVelocidad);
}

//=====================================================================================================================================
//=====================================================================================================================================
void GiraCuelloCentroY(int iVelocidad)
{
  bParo = false;        // La variable indica que se quiere mover
  GiraCuelloGradosY(iAnguloCentroY, iVelocidad);
}

//=====================================================================================================================================
//=====================================================================================================================================
void GiraCuelloCentro()
{
  GiraCuelloCentroX(iVelocidadNormalX);
  GiraCuelloCentroY(iVelocidadNormalY);
}

//=====================================================================================================================================
// Giraa la izquierda - centro - derecha - centro
// a velocidad normal
//=====================================================================================================================================
void GiraCuelloOjearX()
{
  if (!bParo) GiraCuelloIzquierda(iVelocidadNormalX);
  if (!bParo) Espera(iEsperaLarga);             // Espera 1 segundo
  if (!bParo) GiraCuelloCentroX(iVelocidadNormalX);
  if (!bParo) Espera(iEsperaLarga);             // Espera 1 segundo
  if (!bParo) GiraCuelloDerecha(iVelocidadNormalX);
  if (!bParo) Espera(iEsperaLarga);             // Espera 1 segundo
  if (!bParo) GiraCuelloCentroX(iVelocidadNormalX);
}

//=====================================================================================================================================
// Giraa la arriba - centro - abajo - centro
// a velocidad normal
//=====================================================================================================================================
void GiraCuelloOjearY()
{
  if (!bParo) GiraCuelloArriba(iVelocidadNormalY);
  if (!bParo) Espera(iEsperaLarga);             // Espera 1 segundo
  if (!bParo) GiraCuelloCentroY(iVelocidadNormalY);
  if (!bParo) Espera(iEsperaLarga);             // Espera 1 segundo
  if (!bParo) GiraCuelloAbajo(iVelocidadNormalY);
  if (!bParo) Espera(iEsperaLarga);             // Espera 1 segundo
  if (!bParo) GiraCuelloCentroY(iVelocidadNormalY);
}

//=====================================================================================================================================
// Al cabo de 5 seg. si miramos a uno de los lados centramos la vista
//=====================================================================================================================================
void CentrarVista5seg()
{

  
  // Si no nos movemos, estamos sin centtrar y han pasado mas de 5 segundos 
  if (iPosicionActualX!=iAnguloCentroX  || iPosicionActualY!=iAnguloCentroY )
  {
  Serial.print("bsemaforo=");
  Serial.print(bSemaforo);
  Serial.print("  |  iPosicionActualY=");
  Serial.print(iPosicionActualY);
  Serial.print("    iAnguloCentroY=");
  Serial.print(iAnguloCentroY);    if (bSemaforo)
  Serial.print("  |  iPosicionActualX=");
  Serial.print(iPosicionActualX);
  Serial.print("    iAnguloCentroX=");
  Serial.println(iAnguloCentroX);
  if (bSemaforo)
    {
      // Si han pasado 8 segundos centramos
      if ((millis() - tAntesCentro) > 8000)
      {
        bSemaforo=false;  // Fin del conteo
        //tAntesCentro=millis();
        GiraCuelloCentro();
      }
    }
    else
    {
      bSemaforo=true; // Empezamos el conteo
      tAntesCentro=millis();
    }
  }
}

//=====================================================================================================================================
// DICE NO
//=====================================================================================================================================
void Cuello_No()
{
  // Paro el resto de movimientos
  IncrementoX = 0; IncrementoY = 0;
  GiraCuelloCentro();
  // Hago el movimiento completo
  GiraCuelloGradosX(iAnguloMedioIzquierda, iVelocidadRapidoX);  Espera(iEsperaRapido);// IZQUIERDA
  GiraCuelloGradosX(iAnguloMedioDerecha, iVelocidadRapidoX);  Espera(iEsperaRapido);  // DERECHA
  GiraCuelloGradosX(iAnguloMedioIzquierda, iVelocidadRapidoX);  Espera(iEsperaRapido);// IZQUIERDA
  GiraCuelloGradosX(iAnguloMedioDerecha, iVelocidadRapidoX);  Espera(iEsperaRapido);  // DERECHA
  GiraCuelloCentroX(iVelocidadRapidoX);                                               // CENTRO
}

//=====================================================================================================================================
// DICE SI
//=====================================================================================================================================
void Cuello_Si()
{
  // Paro el resto de movimientos
  IncrementoX = 0; IncrementoY = 0;
  GiraCuelloCentro();
  // Hago el movimiento completo
  GiraCuelloGradosY(iAnguloMedioArriba, iVelocidadRapidoY);  Espera(iEsperaRapido);   // ARRIBA
  GiraCuelloGradosY(iAnguloMedioAbajo, iVelocidadRapidoY);  Espera(iEsperaRapido);    //ABAJO
  GiraCuelloGradosY(iAnguloMedioArriba, iVelocidadRapidoY);  Espera(iEsperaRapido);   // ARRIBA
  GiraCuelloGradosY(iAnguloMedioAbajo, iVelocidadRapidoY);  Espera(iEsperaRapido);    //ABAJO
  GiraCuelloCentroY(iVelocidadRapidoY);                                               // CENTRO
}

//=====================================================================================================================================
// TEST: (arriba/abajo/derecha/izquierda)
//=====================================================================================================================================
void Cuello_Test()
{
  // Paro el resto de movimientos
  IncrementoX = 0; IncrementoY = 0;
  // Hago el movimiento completo
  GiraCuelloIzquierda(iVelocidadNormalX);  Espera(iEsperaNormal);
  GiraCuelloDerecha(iVelocidadNormalX);  Espera(iEsperaNormal);
  GiraCuelloCentroX(iVelocidadNormalX);  Espera(iEsperaNormal);
  GiraCuelloArriba(iVelocidadNormalY);  Espera(iEsperaNormal);
  GiraCuelloAbajo(iVelocidadNormalY);  Espera(iEsperaNormal);
  GiraCuelloCentroY(iVelocidadNormalY);                                               // CENTRO
}

//=====================================================================================================================================
// ALEATORIO
//=====================================================================================================================================
void Cuello_Aleatorio()
{
}

//=====================================================================================================================================
// TRISTE
// movimiento cuello centro + abajo lentamente.
//=====================================================================================================================================
void Cuello_Triste()
{
}

//=====================================================================================================================================
// SORPRESA
// movimiento cuello centro hacia arriba ligeramente y hacia abajo rapido.
//=====================================================================================================================================
void Cuello_Sorpresa()
{
}

//=====================================================================================================================================
// SUSTO
// movimiento cuello centro hacia arriba ligeramente y hacia abajo rapido. (??)
//=====================================================================================================================================
void Cuello_Susto()
{
}




//=====================================================================================================================================
//=====================================================================================================================================
void VuelveAlCentro()
{
  if (iCara!= iCaraDuerme)
  {
    if ((iPosicionActualX!= iAnguloCentroX || iPosicionActualY!= iAnguloCentroY ) && IncrementoX ==0 && IncrementoY ==0)
    {
        if (TiempoAnterior==0)
        {
          TiempoAnterior=millis();  // Inicia temporizador
        }
        else
         {
            unsigned long TiempoActual=millis();
            if (TiempoActual - TiempoAnterior >= 5000)
            {
              //Cara_Establece(iCaraNormal);
              GiraCuelloCentro();
            }
            else
            {
              // Esperar pasar el tiempo
            }
         }
    }
    else
    {
      TiempoAnterior=0;
    }
  }
}
