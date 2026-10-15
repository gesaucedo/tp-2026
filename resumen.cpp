#include <iostream>
#include <cstdio>
#include <iomanip>

using namespace std;

// Estructura de comanda para el reporte semanal
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

int main() {
    char nombreArchivo[50];

    cout << "========================================" << endl;
    cout << "    RESUMEN DE CIERRE SEMANAL           " << endl;
    cout << "========================================" << endl;
    cout << "Ingrese el nombre del archivo semanal (ej: comandas_semana_s1-06.dat): ";
    cin >> nombreArchivo;

    FILE* pArch = fopen(nombreArchivo, "rb");

    if (pArch == NULL) {
        cout << "\nError: No se pudo abrir el archivo '" << nombreArchivo << "'." << endl;
        cout << "Verifique que el archivo exista y vuelva a intentarlo." << endl;
        return 1;
    }
Comanda reg;
    int totalProductosBuffet = 0;

    cout << "\n--------------------------------------------------" << endl;
    cout << left << setw(10) << "ID MOZO" 
         << setw(20) << "CANT. PRODUCTOS" 
         << setw(15) << "TOTAL COMISION" << endl;
    cout << "--------------------------------------------------" << endl;

    if (fread(&reg, sizeof(Comanda), 1, pArch) == 1) {
        while (!feof(pArch)) {
            int mozoActual = reg.idMozo;
            int cantProductosMozo = 0;
            float totalComisionMozo = 0.0f;

            while (!feof(pArch) && reg.idMozo == mozoActual) {
                cantProductosMozo += reg.cantidad;
                totalComisionMozo += reg.comision;
                fread(&reg, sizeof(Comanda), 1, pArch);
            }

            cout << left << setw(10) << mozoActual 
                 << setw(20) << cantProductosMozo 
                 << "$ " << fixed << setprecision(2) << totalComisionMozo << endl;

            totalProductosBuffet += cantProductosMozo;
        }
    } else {
        cout << "\nEl archivo de la semana está vacío o no contiene comandas." << endl;
    }
    
    fclose(pArch);
    
    return 0;
}
