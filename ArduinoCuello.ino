//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%  DOMO5: ARDUINO CONTROL SERVOMOTORES + SENSORES. Incluye:                %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%                                                                          %%
//%%    - Comunicacion puerto Serie         (57600 baudios)                   %%
//%%    - Control de servo X del cuello     (Pin  6)                          %%
//%%    - Control de servo Y del cuello     (Pin  7)                          %%
//%%    - 1 sensor de tacto derecha         (Pin  2)  (interrupcion)          %%
//%%    - 1 sensor de tacto izquierda       (Pin  3)  (interrupcion)          %%
//%%                                                                          %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%% Recibimos paquetes de 2 bytes (=2 caracteres) de manera que en el primer %%
//%% caracter indicamos la orden a realizar y en el segundo informacion adici %%
//%% onal si hace falta: grados,....                                          %%
//%%                                                                          %%
//%%    COMANDOS SERIAL:                                                      %%
//%%                                                                          %%
//%%          BAJO NIVEL:                                                     %%
//%%                                                                          %%
//%%              - 'u ' : Arriba                                             %%
//%%              - 'd ' : Abajo                                              %%
//%%              - 'l ' : Izquierda                                          %%
//%%              - 'r ' : Derecha                                            %%
//%%              - 'c ' : centro                                             %%
//%%              - 's ' : Stop                                               %%
//%%              - 'k ' : Reset                                              %%
//%%              - 't ' : test (arriba/abajo/derecha/izquierda)              %%
//%%              - 'g[grados] ' : grados X: [g] + [grados]                   %%
//%%              - 'h[grados] ' : grados Y: [h] + [grados]                   %%
//%%              - 'b[grados] ' : incrementar grados X: [b] + [grados]       %%
//%%              - 'e[grados] ' : incrementar grados Y: [e] + [grados]       %%
//%%              - 'f[grados] ' : decrementar grados X: [f] + [grados]       %%
//%%              - 'x[grados] ' : decrementar grados Y: [x] + [grados]       %%
//%%              - 'v ' : Posicion actual X (grados)                         %%
//%%              - 'w ' : Posicion actual Y (grados)                         %%
//%%              - '0 ' : Velocidad lenta                                    %%
//%%              - '1 ' : Velocidad normal                                   %%
//%%              - '2 ' : Velocidad rapida                                   %%
//%%              - '8'  : rele on		                     									  %%
//%%              - '9'  : rele off											                      %%
//%%              - '3 ' : cara sonrie                                        %%
//%%              - '4 ' : cara habla                                         %%
//%%              - '5 ' : cara duerme                                        %%
//%%              - '6 ' : cara test                                          %%
//%%              - '7 ' : cara vacio                                         %%
//%%              - '8 ' : cara asustado                                      %%
//%%              - '9 ' : cara normal                                        %%
//%%                                                                          %%
//%%          ALTO NIVEL:                                                     %%
//%%                                                                          %%
//%%              - 'n ' : NO                                                 %%
//%%              - 'm ' : SI                                                 %%
//%%              - 'o ' : Ojear X                                            %%
//%%              - 'p ' : Ojear Y                                            %%
//%%              - 'a ' : Aleatorio                                          %%
//%%              - 'i ' : Triste : centro + abajo lentamente                 %%
//%%              - 'j ' : Susto: centro arriba ligeramente y abajo rapido    %%
//%%              - 'q ' : Sorpresa: centro arriba ligeramente y abajo rapido %%
//%%                                                                          %%
//%%		  Quedan libres : f,z,3,4,5,6,7,8,9								  %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%                                                                          %%
//%%  Para controlar con un Arduino, sugerimos conectar el cable de control   %%
//%%  naranja al pin 9 o 10 y usar la biblioteca Servo incluida con Arduino   %%
//%%  IDE.                                                                    %%
//%%                                                                          %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int IncrementoX = 0;
int IncrementoY = 0;

int iVelocidad = 10;    // NORMAL

//unsigned long tAhora  = 0;

//============================================================================
// Los dos caracteres que recibiremos (1 byte cada caracter)
// En cada byte (caracter) podemos representar un valor entre 0 y 255
//============================================================================
//byte caracter[2];

void Espera(int iMiliSegundos);

//-----------------------------------------------------------------------------
// LIBRERIAS
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Propias
//-----------------------------------------------------------------------------
#include "Serie.h"                  // Libreria propia: Comunicacion puerto serie
#include "rgb.h"                    // Libreria propia: led rgb
#include "cara.h"                   // Libreria propia: Matrices leds 8x8 cara
#include "cuello.h"                 // Libreria propia: Movimiento Cuello
#include "tacto.h"                  // Libreria propia: Sensores tacto
#include "rele.h"                 // Libreria propia: Rele control servos y motores
#include "FuncionesCombinadas.h"    // Funciones de proposito general

//============================================================================
// INICIALIZACION
//============================================================================
void Todo_Reset()
{
  Serie_Inicializa();   // Inicializar Puerto Serie
  Cuello_Inicializa();  // Inicializar Cuello
  Tacto_Inicializa();  // Inicializar Tacto
  Cara_Inicializa();  // Inicializa Cara
  Rgb_Inicializa();   // Inicializa Rgb
}

//============================================================================
// INICIALIZACION
//============================================================================
void setup()
{
  Todo_Reset();
}

//============================================================================
// BUCLE INFINITO
//============================================================================
void loop()
{

  //tAhora = millis();              // Control de tiempo
  Serial_Lee();
  PintaCara();
  Pinta_Rgb();
  
  // Funcion de espera para velocidad en el movimiento
  if (IncrementoX!=0 || IncrementoY !=0)
  {
    Cuello_Mueve_X();
    Cuello_Mueve_Y();
    Espera(iVelocidad);
  }
  else
  {
    // Sensores tacto
   bool bDetectadoDerecha=false; bool bDetectadoIzquierda=false;
   bDetectadoDerecha=Tacto_Derecha_Lee();
    if (!bDetectadoDerecha) bDetectadoIzquierda= Tacto_Izquierda_Lee();
    //CentrarVista5seg();         // Al cabo de 5 seg. si miramos a uno de los lados centramos la vista
    if (!bDetectadoDerecha && !bDetectadoIzquierda)
    {
      VuelveAlCentro();
    }
  }
}

//============================================================================
// Leemos un paquete de 2 bytes
//============================================================================
//bool LeerDosBytes()
//{
//   if (Serial.available() >= 2)
//   {
//      Serial.readBytes((byte*)caracter, 2);
//      return true;
//   }
//   return false;
//}

//============================================================================
// SI leemos algo entendible del serial, mandamos la orden determinada
//============================================================================
bool Serial_Lee()
{
  bool bIdentificado=false;
  
 if (Serial.available() > 0)
  {
    char TeclaApretada = Serial.read();
    bIdentificado=true;

    int iiCara=0;
    
    switch ((char)TeclaApretada) {
        //--------------------------------------------------------
        // CUELLO
        //--------------------------------------------------------
        case 'a': Cuello_Aleatorio(); break;
//        case 'b': IncrementaGradosX((int)caracter[1],iVelocidadNormalX); break;
        case 'c': GiraCuelloCentro(); break;
        case 'd': IncrementoY = -1; break;
//        case 'e': IncrementaGradosY((int)caracter[1],iVelocidadNormalY); break;
//        case 'f': IncrementaGradosX(-(int)caracter[1],iVelocidadNormalX); break;
//        case 'g': GiraCuelloGradosX((int)caracter[1],iVelocidadNormalX); break;
//        case 'h': GiraCuelloGradosY((int)caracter[1],iVelocidadNormalY); break;
        case 'i': Cuello_Triste(); iiCara=iCaraTriste; break;
        case 'j': Cuello_Susto();  iiCara=iCaraAsustado; break;
        case 'k': Todo_Reset(); break;
        case 'l': IncrementoX = 1; break;
        case 'm': Cuello_Si(); break; iiCara=iCaraSonrie; break;
        case 'n': Cuello_No(); break; iiCara=iCaraSonrie; break;
        case 'o': GiraCuelloOjearX(); break;
        case 'p': GiraCuelloOjearY(); break;
        case 'q': Cuello_Sorpresa();iiCara=iCaraSorpresa; break;
        case 'r': IncrementoX = -1; break;
        case 's': IncrementoX = 0; IncrementoY = 0; break;
        case 't': Cuello_Test(); break;
        case 'u': IncrementoY = 1; break;
        case 'v': Serie_Envia('x',(String)iPosicionActualX); break;
        case 'w': Serie_Envia('y',(String)iPosicionActualY); break;
//        case 'x': IncrementaGradosY(-(int)caracter[1],iVelocidadNormalY); break;
          case 'y': break;
        case '2': iVelocidad=iVelocidadRapidoX; break;
        case '?': Serie_Ayuda(); break;
//        case '8': Rele_ON(); break;
//        case '9': Rele_OFF(); break; 
        //--------------------------------------------------------
        // CARA
        //--------------------------------------------------------
        case '3': iiCara=iCaraSonrie; break;
        case '4': iiCara=iCaraHabla; break;
        case '5': iiCara=iCaraDuerme; break;
        case '6': iiCara=iCaraTest; break;
        case '7': iiCara=iCaraVacio; break;
        case '8': iiCara=iCaraAsustado;break;
        case '9': iiCara=iCaraNormal; break;
        case '(': iiCara=iCaraTriste; break;
        case ']': iiCara=iCaraGuinaOjo; break;
        case '+': iiCara=iCaraAburrido; break;
        case '-': iiCara=iCaraSorpresa; break;
        case '/': iiCara=iCaraRisa; break;
        case '>': iiCara=iCaraVistaFrente; break;
        //case 'z': break;
        case '0': iVelocidad=iVelocidadLentoX; break;
        case '1': iVelocidad=iVelocidadNormalX; break;
        case '=': iiCara=iCaraVistaArribaDerecha; break;
        case 'A': iiCara=iCaraVistaArribaIzquierda; break;
        case 'B': iiCara=iCaraVistaAbajoDerecha; break;
        case 'C': iiCara=iCaraVistaAbajoIzquierda; break;
        //--------------------------------------------------------
        // RGB
        //--------------------------------------------------------
        case '!': Rgb_Rainbow(); break;
        case '@': Rgb_Rojo(); break;
        case '#': Rgb_Verde(); break;
        case '~': Rgb_Azul(); break;
        case '%': Rgb_BlancoNegro(); break;
        case '[': Rgb_Aleatorio(); break;
        case '*': Rgb_VioletaVerde(); break;
        case '^': Rgb_Party(); break;
        case '{': Rgb_Cloud(); break;
        case '}': Rgb_Heat(); break;
        //--------------------------------------------------------
        // SENSORES
        //--------------------------------------------------------
        case ';': TactoDerecha_Gira(); break;
        case ',': TactoIzquierda_Gira(); break;
        // POSICIONES LIBRES PARA FUTURAS ACCIONES
        case 'N': break;
// case ....podemos poner todas las mayusculas del abecedario
        default:   bIdentificado=false; break;
      }
      //if (bIdentificado)     Serie_Envia((char)caracter[0], "Cuello: ok"); 
      //else 
      //{
      //}
       //Serial.print("Caracteres: <"); Serial.print((char)caracter[0]);  Serial.print("-"); Serial.print((char)caracter[1]);     Serial.println(">"); 
       //Serial.print("ASCII: <"); Serial.print((int)caracter[0]);  Serial.print("-"); Serial.print((int)caracter[1]);     Serial.println(">"); 

    if (bIdentificado)
    {
      Serie_Envia('K',(String)TeclaApretada);
      
    }

    if (iiCara!=0 && iiCara!=iCara)
      {
        Cara_Establece(iiCara);
      }
  }

  return bIdentificado;
}
   

//============================================================================
// Espera en Milisegundos, leyendo el serial por si recibimos stop o giro.
//============================================================================
void Espera(int iMiliSegundos)
{
  bool bSalir=false;
  
  unsigned long inicio, fin, transcurrido;
  inicio = millis();
  fin = inicio + iMiliSegundos;
  transcurrido = 0;
  while (transcurrido <= fin || bSalir)
  {
    transcurrido = millis();
    bool bSerie=Serial_Lee();
    PintaCara();
    // Sensores tacto
    bool bDetectadoDerecha=false;    bool bDetectadoIzquierda=false;
    ////bDetectadoDerecha=Tacto_Derecha_Lee();
    ////if (!bDetectadoDerecha) bDetectadoIzquierda= Tacto_Izquierda_Lee();

    bSalir= (bSerie || bDetectadoDerecha || bDetectadoIzquierda);
  }
}
