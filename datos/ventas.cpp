
#include <iostream>
#include <cstdio>     // FILE, fopen, fread, fwrite, fseek, ftell, fclose
#include <cstring>    // strcpy, strcmp, strlen
#include <cstdlib>    // atoi

using namespace std;

const float TASA_COMISION = 0.10f;
const int   K = 5;                   // corrimiento de la clave: DEBE ser el mismo en normalizacion.cpp

struct Producto {
    int   codigo;
    char  descripcion[50];
    float precio;
    int   stockActual;
};

struct Mozo {
    int   idMozo;
    char  nombre[50];
    char  password[20];      // guardada cifrada (corrimiento de K)
    float totalComision;
};

struct Comanda {
    int   idMozo;
    int   codigoProducto;
    int   cantidad;
    float comision;
};

//Declaracion de funciones
void cifrar(char pssw[]);
int login(FILE* fMozos);
long buscarProducto(FILE* f, int codigo, Producto& p);
void cargarVenta(int idMozo, FILE* f, FILE* fInv);


int main() {

    char fecha [11];

    cout <<"Ingrese la fecha del dia de hoy (dd-mm-aaaa)" << endl;      //Pido que el usuario ingrese la fecha de hoy.
    cin >> fecha;
    
    //habria que validar primero que haya ingrado datos y que el formato sea el correcto
    char planilla[30];
    sprintf(planilla, "comandas_%s.dat", fecha);
    
//Abre o crea el archivo comanda dd-mm-aaa.dat(planilla con la fecha ingresada)
    
    FILE* f = fopen(planilla, "ab");
    if (f == NULL) {
    cout << "No se pudo abrir la planilla." << endl;
    return 1;
    }
    
//Cargar datos 
    FILE* fMozos = fopen("mozos.dat", "rb");
    if (fMozos == NULL) {
    cout << "No se pudo abrir mozos.dat" << endl;
    return 1;
    }
    
    FILE* fInv = fopen("inventario.dat", "rb+");
    if (fInv == NULL) {
    cout << "No se pudo abrir inventario.dat" << endl;
    return 1;
    }


//Pido que se logue el mozo, id de mozo y contraseña. Valida los datos con la información que hay en mozo.dat
//Por cada venta, busca precio calcula comisión y agtrega la venta  ala planilla del dia.
    int opcion = -1;
    while (opcion != 0) {
        cout << endl;
        cout << "===== VENTAS =====" << endl;
        cout << "1. Cargar una venta" << endl;
        cout << "0. Terminar" << endl;
        cout << "Opcion: ";

        if (!(cin >> opcion)) {            // si escribio letras en vez de un numero
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
        }

        if (opcion == 1) {
            int idMozo = login(fMozos);
            if (idMozo != 0) {
                cargarVenta(idMozo, f, fInv);
                // calcular la comision y guardar la venta con fwrite en f
            }
        } else if (opcion != 0) {
            cout << "Opcion invalida." << endl;
        }
    }

    fclose(fMozos);
    fclose(f);
    fclose(fInv);

//Planilla ordenada por mozo.
     
     
    return 0;
}
   

//Definicion de funciones
void cifrar(char pssw[]) {
    for (int i = 0; pssw[i] != '\0'; i++) {
        pssw[i] = pssw[i] + K;
    }
}

int login(FILE* fMozos) {
    int id;
    cout << "Numero de mozo: ";
    cin >> id;

    // cuantos mozos hay
    fseek(fMozos, 0, SEEK_END);
    long cantidad = ftell(fMozos) / sizeof(Mozo);

    // caso raro: el mozo no existe
    if (id < 1 || id > cantidad) {
        cout << "El mozo no existe." << endl;
        return 0;
    }

    // PUP: ir directo a la posicion id - 1 y leer
    Mozo m;
    fseek(fMozos, (long)(id - 1) * sizeof(Mozo), SEEK_SET);
    fread(&m, sizeof(Mozo), 1, fMozos);

    // pedir la clave, cifrarla y compararla con la guardada
    char clave[20];
    cout << "Clave: ";
    cin >> clave;      // necesita #include <iomanip>
    cifrar(clave);

    // caso raro: la clave no coincide
    if (strcmp(clave, m.password) != 0) {
        cout << "Clave incorrecta." << endl;
        return 0;
    }

    cout << "Hola, " << m.nombre << "." << endl;
    return id;
}

long buscarProducto(FILE* f, int codigo, Producto& p) {
    fseek(f, 0, SEEK_END);
    long cantidad = ftell(f) / sizeof(Producto);

    long pri = 0, ult = cantidad - 1, pos = -1;
    while (pri <= ult && pos == -1) {
        long med = (pri + ult) / 2;
        fseek(f, med * sizeof(Producto), SEEK_SET);
        fread(&p, sizeof(Producto), 1, f);

        if (p.codigo == codigo)      pos = med;
        else if (codigo > p.codigo)  pri = med + 1;
        else                         ult = med - 1;
    }
    return pos;
}

void cargarVenta(int idMozo, FILE* f, FILE* fInv) {
    // Pedir el producto y buscarlo
    int codigo;
    cout << "Codigo de producto: ";
    cin >> codigo;
    if (!cin) {                        // escribio letras en vez de un numero
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Codigo invalido. Venta cancelada." << endl;
        return;
    }

    Producto p;
    long pos = buscarProducto(fInv, codigo, p);
    if (pos == -1) {
        cout << "El producto no existe." << endl;
        return;
    }
    cout << p.descripcion << " - precio " << p.precio
         << " - stock " << p.stockActual << endl;

    // Pedir la cantidad y controlar el stock
    int cantidad;
    cout << "Cantidad: ";
    cin >> cantidad;
    if (!cin) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Cantidad invalida. Venta cancelada." << endl;
        return;
    }
    if (cantidad < 1) {
        cout << "La cantidad debe ser mayor a 0." << endl;
        return;
    }
    if (p.stockActual == 0) {
        cout << "Sin stock. Venta cancelada." << endl;
        return;
    }
    if (cantidad > p.stockActual) {
        cout << "Stock insuficiente: quedan " << p.stockActual << endl;
        return;
    }

    // Armar la comanda y calcular la comision segun el precio
    Comanda c;
    c.idMozo         = idMozo;
    c.codigoProducto = codigo;
    c.cantidad       = cantidad;
    c.comision       = p.precio * cantidad * TASA_COMISION;

    // Guardar la venta al final de la planilla del dia
    fwrite(&c, sizeof(Comanda), 1, f);

    // Descontar el stock: volver a ese registro y reescribirlo
    p.stockActual -= cantidad;
    fseek(fInv, pos * sizeof(Producto), SEEK_SET);
    fwrite(&p, sizeof(Producto), 1, fInv);

    cout << "Venta cargada. Comision: " << c.comision << endl;
}
