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

    if (cantMozos == MAX_MOZOS) {
        return -1; // Límite de capacidad
    }

    cantMozos++;
    int pos = cantMozos - 1;

    mozos[pos].idMozo = cantMozos;
    strcpy(mozos[pos].nombre, nombre);
    mozos[pos].totalComision = comision;

    // Generar clave encriptada basada en el ID
    char clavePlana[20];
    sprintf(clavePlana, "%d", cantMozos);
    encriptarClave(clavePlana, mozos[pos].password, K);

    return cantMozos;
}

// Devuelve el idMozo correspondiente al nombre
int buscarIdMozo(Mozo mozos[], int cant, const char* nombre) {
    for (int i = 0; i < cant; i++) {
        if (strcmp(mozos[i].nombre, nombre) == 0) {
            return mozos[i].idMozo;
        }
    }
    return -1;
}

// Verifica si la fecha ya fue registrada en la lista de días
int fechaYaGuardada(char fechas[][11], int cant, const char* fecha) {
    for (int i = 0; i < cant; i++) {
        if (strcmp(fechas[i], fecha) == 0) {
            return 1;
        }
    }
    return 0;
}


void ordenarPorMozo(Comanda v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (v[j].idMozo > v[j + 1].idMozo) {
                Comanda aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}

// Búsqueda binaria directo en disco sobre inventario.dat
int buscarProducto(FILE* f, int cantRegistros, int codigo, Producto &p) {
    int inicio = 0;
    int fin = cantRegistros - 1;

    while (inicio <= fin) {
        int medio = (inicio + fin) / 2;
        fseek(f, medio * sizeof(Producto), SEEK_SET);
        fread(&p, sizeof(Producto), 1, f);

        if (p.codigo == codigo) {
            return medio;
        }
        if (p.codigo < codigo) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    return -1;
}
