
#include <cstdio>

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

const int DIAS_SEMANA = 7;

// ---------- manejo simple de fechas ----------

bool esBisiesto(int a) {
    return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
}

int diasDelMes(int m, int a) {
    int d[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && esBisiesto(a)) return 29;
    return d[m - 1];
}

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

// ---------- programa principal ----------

int main() {
    int d, m, a;
    int semana, mesSemana;

    printf("=== Cierre semanal ===\n");
    printf("Fecha del primer dia de la semana (dd mm aaaa): ");
    if (scanf("%d %d %d", &d, &m, &a) != 3 || m < 1 || m > 12 ||
        d < 1 || d > diasDelMes(m, a)) {
        printf("Fecha invalida.\n");
        return 1;
    }
    printf("Numero de semana (X) y mes (mm) para el nombre del archivo: ");
    if (scanf("%d %d", &semana, &mesSemana) != 2) {
        printf("Datos invalidos.\n");
        return 1;
    }

    // 1) Abrir las planillas de cada dia. Si un dia no tiene planilla
    //    (no se cargaron ventas), simplemente se saltea.
    FILE *dia[DIAS_SEMANA];
    Comanda actual[DIAS_SEMANA];
    bool hayRegistro[DIAS_SEMANA];
    int abiertos = 0;

    for (int i = 0; i < DIAS_SEMANA; i++) {
        char nombre[64];
        snprintf(nombre, sizeof(nombre), "comandas_%02d-%02d-%04d.dat", d, m, a);
        dia[i] = fopen(nombre, "rb");
        hayRegistro[i] = false;
        if (dia[i] != NULL) {
            abiertos++;
            // lectura inicial (la "primera lectura" del apareo)
            hayRegistro[i] = (fread(&actual[i], sizeof(Comanda), 1, dia[i]) == 1);
        } else {
            printf("Aviso: no existe %s, se saltea ese dia.\n", nombre);
        }
        siguienteDia(d, m, a);
    }

    if (abiertos == 0) {
        printf("No se encontro ninguna planilla diaria. No se genera la semanal.\n");
        return 1;
    }

    // 2) Crear la planilla semanal
    char nombreSemana[64];
    snprintf(nombreSemana, sizeof(nombreSemana), "comandas_semana_s%d-%02d.dat",
             semana, mesSemana);
    FILE *sem = fopen(nombreSemana, "wb");
    if (sem == NULL) {
        printf("No se pudo crear %s\n", nombreSemana);
        for (int i = 0; i < DIAS_SEMANA; i++)
            if (dia[i] != NULL) fclose(dia[i]);
        return 1;
    }

    // 3) Apareo: mientras quede algun archivo con registros, escribir el
    //    de menor idMozo. Con "<" estricto, ante un empate gana el dia mas
    //    temprano (queda un orden estable).
    int escritos = 0;
    while (true) {
        int menor = -1;
        for (int i = 0; i < DIAS_SEMANA; i++) {
            if (hayRegistro[i] &&
                (menor == -1 || actual[i].idMozo < actual[menor].idMozo)) {
                menor = i;
            }
        }
        if (menor == -1) break;  // todos los archivos se terminaron

        fwrite(&actual[menor], sizeof(Comanda), 1, sem);
        escritos++;

        // avanzar solo en el archivo del que saque el registro
        hayRegistro[menor] =
            (fread(&actual[menor], sizeof(Comanda), 1, dia[menor]) == 1);
    }

    // 4) Cerrar todo
    fclose(sem);
    for (int i = 0; i < DIAS_SEMANA; i++)
        if (dia[i] != NULL) fclose(dia[i]);

    printf("Listo: %d ventas de %d dia(s) guardadas en %s\n", escritos, abiertos,
           nombreSemana);
    return 0;
}
