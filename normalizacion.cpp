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


int buscarOAgregarMozo(Mozo mozos[], int &cantMozos, const char* nombre, float comision) {
    for (int i = 0; i < cantMozos; i++) {
        if (strcmp(mozos[i].nombre, nombre) == 0) {
            mozos[i].totalComision += comision;
            return mozos[i].idMozo;
        }
    }

    // Control de límite para evitar desbordamiento
    if (cantMozos == MAX_MOZOS) {
        return -1;
    }

    cantMozos++;
    int pos = cantMozos - 1;

    mozos[pos].idMozo = cantMozos;
    strcpy(mozos[pos].nombre, nombre);
    mozos[pos].totalComision = comision;

    // Generar clave por defecto (su ID en texto) y cifrarla con K
    char clavePlana[20];
    sprintf(clavePlana, "%d", cantMozos);
    encriptarClave(clavePlana, mozos[pos].password, K);

    return cantMozos;
}
