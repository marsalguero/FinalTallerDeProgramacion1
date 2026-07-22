#include <stdio.h>

#define APAGADA 0
#define PRENDIDA 1
#define INTENTOS3 15
#define INTENTOS4 20
#define INTENTOS5 30

/* Configuraciones 3x3 */
int configuraciones3[3][3][3] = {
    { {0,1,0}, {1,0,1}, {0,1,0} }, // A
    { {1,0,1}, {0,0,0}, {1,0,1} }, // B
    { {0,0,1}, {1,0,0}, {0,1,1} }  // C
};

/* Configuraciones 4x4 */
int configuraciones4[3][4][4] = {
    { {0,1,0,1}, {1,0,1,0}, {0,1,0,1}, {1,0,1,0} },
    { {1,0,0,1}, {1,0,1,0}, {0,1,0,1}, {1,0,1,0} },
    { {0,0,0,1}, {1,0,1,0}, {0,1,0,1}, {1,1,0,0} }
};

/* Configuraciones 5x5 */
int configuraciones5[3][5][5] = {
    { {0,1,0,1,0}, {1,0,1,0,1}, {0,1,0,1,0}, {1,0,1,0,1}, {0,1,0,1,0} },
    { {1,1,0,1,1}, {1,0,0,0,1}, {0,0,0,0,0}, {1,0,0,0,1}, {1,1,0,1,1} },
    { {0,0,1,1,0}, {1,0,0,1,1}, {0,1,0,1,0}, {1,1,0,0,1}, {0,1,1,0,0} }
};

/* Variables globales de juego */
int configuracion;
int dimension = 0;
int tablero[5][5];

/* Contadores de estadísticas (índices: 0=3x3, 1=4x4, 2=5x5) */
int ganadas[3] = {0,0,0};
int perdidas[3] = {0,0,0};
int jugadas[3] = {0,0,0};
int interacciones[3] = {0,0,0};
int abandonadas[3] = {0,0,0};

/* Matriz para registrar qué configuraciones ya se jugaron: usadas[dimension][configuracion] */
int usadas[3][3] = { {0,0,0}, {0,0,0}, {0,0,0} };

/* Función que muestra el tablero */
void mostrar_tablero(int dimension) {
    int i, j;
    for(i = 0; i < dimension; i++) {
        for(j = 0; j < dimension; j++) {
            printf("%d ", tablero[i][j]);
        }
        printf("\n");
    }
}

/* Función que muestra los resultados finales */
void mostrar_resultados() {
    printf("\n=== RESULTADOS FINALES ===\n");
    for (int i = 0; i < 3; i++) {
        int dim = i + 3;
        printf("Partidas en %dx%d:\n", dim, dim);
        printf(" - %d Ganadas vs %d Perdidas\n", ganadas[i], perdidas[i]);
        printf(" - %d Jugadas\n", jugadas[i]);
        printf(" - %d Interacciones totales\n", interacciones[i]);

        // Solo se muestran si efectivamente abandonó al menos una partida
        if (abandonadas[i] > 0) {
            printf(" - %d Abandonadas\n", abandonadas[i]);
        }
        printf("---------------------------\n");
    }
}

/* Funciones que copian al tablero las matrices */
void copiar3x3() {
    int i, j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            tablero[i][j] = configuraciones3[configuracion][i][j];
        }
    }
}

void copiar4x4() {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            tablero[i][j] = configuraciones4[configuracion][i][j];
        }
    }
}

void copiar5x5() {
    int i, j;
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 5; j++) {
            tablero[i][j] = configuraciones5[configuracion][i][j];
        }
    }
}

/* Función que cambia las luces en la casilla válida */
void cambiar_casilla(int f, int c, int dimension) {
    if (f >= 0 && f < dimension && c >= 0 && c < dimension) {
        tablero[f][c] = !tablero[f][c];
    }
}

/* Función principal para accionar una coordenada y sus vecinos */
void cambiar_luces(int fila, int columna, int dimension) {
    cambiar_casilla(fila, columna, dimension);
    cambiar_casilla(fila - 1, columna, dimension);
    cambiar_casilla(fila + 1, columna, dimension);
    cambiar_casilla(fila, columna - 1, dimension);
    cambiar_casilla(fila, columna + 1, dimension);
}

/* Retorna 1 si todas las luces están apagadas (victoria), de lo contrario retorna 0 */
int verificar_victoria(int dimension) {
    int i, j;
    for(i = 0; i < dimension; i++) {
        for(j = 0; j < dimension; j++) {
            if (tablero[i][j] == 1) {
                return 0; // Todavía hay luces encendidas
            }
        }
    }
    return 1; // Todo apagado
}

/* Programa principal */
int main() {
    int jugando = 1;
    char opt;

    while (jugando == 1) {
        printf("\nBIENVENIDO A APAGA LAS LUCES\n");
        printf("-----------------------\n");
        printf("Por favor, ingrese la dimensión del tablero que desea ejecutar (3, 4 o 5): \n");
        scanf("%d", &dimension);

        int dim_index;
        int max_intentos;

        if (dimension == 3) { dim_index = 0; max_intentos = INTENTOS3; }
        else if (dimension == 4) { dim_index = 1; max_intentos = INTENTOS4; }
        else if (dimension == 5) { dim_index = 2; max_intentos = INTENTOS5; }
        else {
            printf("Ingresaste una opción incorrecta.\n");
            continue;
        }

        // Validación para no repetir configuración
        int configuracion_valida = 0;
        while (!configuracion_valida) {
            printf("Por favor, ingrese el modelo que elije jugar (0, 1 o 2): \n");
            scanf("%d", &configuracion);

            if (configuracion < 0 || configuracion > 2) {
                printf("Modelo inválido. Intente de nuevo.\n");
            } else if (usadas[dim_index][configuracion] == 1) {
                printf("Esa configuración ya fue utilizada en esta ejecución. Elija otra.\n");
            } else {
                configuracion_valida = 1;
                usadas[dim_index][configuracion] = 1; // Marcar como usada
            }
        }

        // Preparar tablero
        if (dimension == 3) copiar3x3();
        else if (dimension == 4) copiar4x4();
        else if (dimension == 5) copiar5x5();

        jugadas[dim_index]++;
        int contador = 0;
        int partida_activa = 1;

        // Bucle de la partida actual
        while (partida_activa && contador < max_intentos) {
            printf("\nIntentos restantes: %d\n", max_intentos - contador);
            mostrar_tablero(dimension);

            int fila, columna;
            printf("Si desea abandonar la partida, ingrese -1.\n");
            printf("Ingrese el número de fila (0 a %d): \n", dimension - 1);
            scanf("%d", &fila);

            if (fila == -1) {
                printf("Has abandonado la partida.\n");
                abandonadas[dim_index]++;
                partida_activa = 0;
                break;
            }

            printf("Ingrese el número de columna (0 a %d): \n", dimension - 1);
            scanf("%d", &columna);

            cambiar_luces(fila, columna, dimension);
            contador++;
            interacciones[dim_index]++;

            if (verificar_victoria(dimension)) {
                printf("\nFELICIDADES!!!\n");
                printf("Apagaste todas las luces.\n");
                ganadas[dim_index]++;
                partida_activa = 0;
            }
        }

        // Si se quedó sin intentos y la partida seguía activa
        if (partida_activa && contador == max_intentos) {
            printf("\nTe quedaste sin intentos!! Fin de la partida.\n");
            perdidas[dim_index]++;
        }

        // Consultar si desea seguir jugando
        printf("\n¿Deseas seguir jugando en alguna dimensión? (s/n): ");
        scanf(" %c", &opt); // El espacio antes del %c limpia saltos de línea pendientes en el buffer
        if (opt == 'n' || opt == 'N') {
            jugando = 0;
        }
    }

    // Mostrar estadísticas al finalizar la ejecución
    mostrar_resultados();

    return 0;
}
