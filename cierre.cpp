#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

const int DIAS = 7;

bool esBisiesto(int a) {
    return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
}

int diasDelMes(int m, int a) {
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && esBisiesto(a))
        return 29;
    return dias[m - 1];
}

// pasa al dia siguiente
void siguienteDia(int &d, int &m, int &a) {
    d++;
    if (d > diasDelMes(m, a)) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            a++;
        }
    }
}

// arma el nombre: comandas_dd-mm-aaaa.dat
void armarNombre(char nombre[], int d, int m, int a) {
    strcpy(nombre, "comandas_00-00-0000.dat");
    nombre[9] = '0' + d / 10;
    nombre[10] = '0' + d % 10;
    nombre[12] = '0' + m / 10;
    nombre[13] = '0' + m % 10;
    nombre[15] = '0' + a / 1000;
    nombre[16] = '0' + a / 100 % 10;
    nombre[17] = '0' + a / 10 % 10;
    nombre[18] = '0' + a % 10;
}

// arma el nombre: comandas_semana_sX-mm.dat
void armarNombreSemana(char nombre[], int semana, int mes) {
    strcpy(nombre, "comandas_semana_s");
    int i = 17;
    if (semana > 9) {
        nombre[i] = '0' + semana / 10;
        i++;
    }
    nombre[i] = '0' + semana % 10;
    i++;
    nombre[i] = '-';
    nombre[i + 1] = '0' + mes / 10;
    nombre[i + 2] = '0' + mes % 10;
    strcpy(nombre + i + 3, ".dat");
}

// devuelve la posicion del archivo que tiene el menor idMozo (-1 si no queda ninguno)
int buscarMenor(Comanda actual[], int leido[]) {
    int menor = -1;
    for (int i = 0; i < DIAS; i++) {
        if (leido[i] == 1) {
            if (menor == -1 || actual[i].idMozo < actual[menor].idMozo)
                menor = i;
        }
    }
    return menor;
}

int main() {
    int d, m, a;
    int semana, mesSemana;

    cout << "Cierre semanal" << endl;
    cout << "Primer dia de la semana (dd mm aaaa): ";
    cin >> d >> m >> a;
    if (m < 1 || m > 12 || d < 1 || d > diasDelMes(m, a)) {
        cout << "Fecha invalida" << endl;
        return 1;
    }
    cout << "Numero de semana y mes: ";
    cin >> semana >> mesSemana;
    if (semana < 1 || semana > 99 || mesSemana < 1 || mesSemana > 12) {
        cout << "Datos invalidos" << endl;
        return 1;
    }

    FILE *dias[DIAS];
    Comanda actual[DIAS];
    int leido[DIAS];
    int abiertos = 0;
    char nombre[50];

    // abro los archivos de los 7 dias
    for (int i = 0; i < DIAS; i++) {
        armarNombre(nombre, d, m, a);
        dias[i] = fopen(nombre, "rb");
        leido[i] = 0;
        if (dias[i] == NULL) {
            cout << "No existe " << nombre << endl;
        } else {
            abiertos++;
            leido[i] = fread(&actual[i], sizeof(Comanda), 1, dias[i]);
        }
        siguienteDia(d, m, a);
    }

    if (abiertos == 0) {
        cout << "No hay ningun archivo, no se genera la semanal" << endl;
        return 1;
    }

    char nombreSemana[50];
    armarNombreSemana(nombreSemana, semana, mesSemana);
    FILE *semanal = fopen(nombreSemana, "wb");
    if (semanal == NULL) {
        cout << "No se pudo crear " << nombreSemana << endl;
        return 1;
    }

    // apareo
    int cant = 0;
    int menor = buscarMenor(actual, leido);
    while (menor != -1) {
        fwrite(&actual[menor], sizeof(Comanda), 1, semanal);
        cant++;
        leido[menor] = fread(&actual[menor], sizeof(Comanda), 1, dias[menor]);
        menor = buscarMenor(actual, leido);
    }

    // cierro todo
    fclose(semanal);
    for (int i = 0; i < DIAS; i++) {
        if (dias[i] != NULL)
            fclose(dias[i]);
    }

    cout << "Se guardaron " << cant << " ventas en " << nombreSemana << endl;
    return 0;
}
