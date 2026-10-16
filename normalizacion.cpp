#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

// Estructura para leer el archivo de la cátedra (comandas_historicas.dat)
struct ComandaHistorica {
    char fecha[11];        // "DD-MM-AAAA"
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

// Estructura para guardar en mozos.dat
struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

const int K = 5;
const int MAX_MOZOS = 100;

// Encriptación por corrimiento de caracteres 
void encriptarClave(const char* origen, char* destino, int k) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i] + k;
        i++;
    }
    destino[i] = '\0';
}
