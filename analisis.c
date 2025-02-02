#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void procesarArchivo(const char *filename, int **arrayEnteros, int *numEnteros, double ***matriz, int *numFilas, int *numColumnas) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Error al abrir el fichero");
    exit(1);
  }

  char buffer[1024];

  // Ignoramos la primera línea
  if (!fgets(buffer, sizeof(buffer), fp)) {
    fprintf(stderr, "Error o fichero vacío.\n");
    fclose(fp);
    exit(1);
  }

  int filas = 0, cols = 0;
  while (fgets(buffer, sizeof(buffer), fp)) {
    // Si la línea está vacía se omite
    if (buffer[0] == '\n' || buffer[0] == '\0')
      continue;
    filas++;

    // En la primera fila leída, contamos el número de columnas (después de la primera)
    if (filas == 1) {
      char *temp = strdup(buffer);
      if (!temp) {
        perror("Error en strdup");
        fclose(fp);
        exit(1);
      }
      // Ignoramos el primer token (que se guarda en el array de enteros)
      char *token = strtok(temp, " \t");
      int c = 0;
      token = strtok(NULL, " \t");
      while (token) {
        c++;
        token = strtok(NULL, " \t");
      }
      cols = c;
      free(temp);
    }
  }


  // Asignamos memoria para el array de enteros y para la matriz.
  // La matriz se reserva de forma contigua y se crea un array de punteros para acceder como matriz[][].
  int *ints = malloc(filas * sizeof(int));
  double *matContiguo = malloc(filas * cols * sizeof(double));
  double **mat = malloc(filas * sizeof(double*));
  for (int i = 0; i < filas; i++) {
    mat[i] = matContiguo + i * cols;
  }

  // Retrocedemos al inicio y leemos de nuevo
  rewind(fp);
  // Volvemos a leer y descartamos la cabecera
  fgets(buffer, sizeof(buffer), fp);

  int filaActual = 0;
  while (fgets(buffer, sizeof(buffer), fp)) {
    // Omitir líneas vacías
    if (buffer[0] == '\n' || buffer[0] == '\0')
      continue;
    // Eliminar el salto de línea si existe
    char *newline = strchr(buffer, '\n');
    if (newline)
      *newline = '\0';

    // Tokenizamos la línea
    char *token = strtok(buffer, " \t");
    if (!token) continue;

    // El primer token se guarda en el array de enteros
    ints[filaActual] = atoi(token);

    // Se leen los siguientes tokens y se asignan directamente a la matriz
    for (int j = 0; j < cols; j++) {
      token = strtok(NULL, " \t");
      if (token) {
        mat[filaActual][j] = atof(token);
      }
    }
    filaActual++;
  }
  fclose(fp);

  // Asignamos los parámetros de salida.
  *arrayEnteros = ints;
  *numEnteros = filas;
  *matriz = mat;
  *numFilas = filas;
  *numColumnas = cols;
}

/* Función main de ejemplo para demostrar el uso de procesarArchivo */
int main(int argc, char* argv[]) {

  if (argc != 2){
    printf("\nUso: %s <fichero_de_datos>\n", argv[0]);
    exit(1);
  }

  int *primerosEnteros;
  double **matriz;
  int numFilas, numColumnas, numEnteros;

  procesarArchivo(argv[1], &primerosEnteros, &numEnteros, &matriz, &numFilas, &numColumnas);

  // Muestra el array de enteros (primer número de cada fila).
  printf("Array de enteros (primer número de cada fila):\n");
  for (int i = 0; i < numEnteros; i++) {
    printf("%d ", primerosEnteros[i]);
  }
  printf("\n\n");

  // Muestra la matriz de doubles (resto de los datos) usando notación mat[fila][columna].
  printf("Matriz de doubles:\n");
  for (int i = 0; i < numFilas; i++) {
    for (int j = 0; j < numColumnas; j++) {
      printf("%.2f ", matriz[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  //Prueba sitio especifico
  printf("Posición 0,0: %.2f\n", matriz[0][0]);
  printf("Posición 1,1: %.2f\n", matriz[1][1]);
  printf("Posición 2,2: %.2f\n", matriz[2][2]);
  printf("Posición 3,3: %.2f\n", matriz[3][3]);

  // Liberamos la memoria asignada.
  free(primerosEnteros);
  // Primero liberamos el bloque contiguo de datos.
  free(matriz[0]);
  // Luego liberamos el array de punteros de filas.
  free(matriz);

  return 0;
}
