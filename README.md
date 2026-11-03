# TP Integrador — Buffet Albert Einstein

## Integrantes del Grupo:

  -Saucedo Gerónimo José (gesaucedo) [ventas.cpp]

  -Farina Lautaro ( flautaro-ops)[ ]

  -Yamil Poma (yamilpomayampasi-lgtm)[ ]

 -        ()[ ]




## Como se compila:

Todos los comandos se ejecutan desde la raíz del repositorio. El sistema son 4 programas independientes, cada uno con su propio main, así que se compilan por separado:

 1. Compilar normalizacion.cpp
 2. Compilar ventas.cpp
 3. Compilar cierre.cpp
 4. Compilar resumen.cpp


## Cómo se corre y en qué orden:

Los programas leen y escriben los .dat en la carpeta desde donde se ejecutan. Los archivos de datos/ son los originales de la cátedra y no se tocan.

Paso 0: preparar la carpeta de trabajo. Se copian los .dat originales a la raíz y se trabaja sobre las copias

Paso 1: ./normalizacion
Lee comandas_historicas.dat e inventario.dat. Genera mozos.dat (con las claves cifradas), una planilla comandas_dd-mm-aaaa.dat por cada día y deja inventario.dat con el stock al día.

Paso 2: ./ventas
Lee mozos.dat e inventario.dat. Al empezar pide la fecha del día y el mozo inicia sesión con su clave. Se cargan las ventas, que se agregan a comandas_dd-mm-aaaa.dat (si no existe, se crea) y se actualiza el stock en inventario.dat. Se corre una vez por cada día que se quiera cargar. Si se repite la fecha, las ventas se suman a la planilla existente.

Paso 3: ./cierre
Lee las planillas diarias comandas_dd-mm-aaaa.dat de la semana y genera la planilla semanal comandas_semana_sX-mm.dat.

Paso 4: ./resumen
Lee comandas_semana_sX-mm.dat y muestra por pantalla los productos vendidos y la comisión de cada mozo, más el total del buffet.

Cada programa deja los archivos que necesita el siguiente, por eso hay que respetar el orden(1;2;3;4)