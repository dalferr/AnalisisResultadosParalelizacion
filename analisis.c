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


void mostrarResultados(int *procs, double **matriz, int numFilas, int numColumnas, double **runtimes, double **speedups, double **efficiencies) {

  // Reservamos memoria para los resultados
  *runtimes = (double *)malloc(numFilas * sizeof(double));
  *speedups = (double *)malloc(numFilas * sizeof(double));
  *efficiencies = (double *)malloc(numFilas * sizeof(double));

  double tserie;

  // Encabezados
  printf("#procs runtime   speedup  efficiency\n");
  printf("#===== ======= ======= ==========\n");

  // Obtenemos los datos necesarios
  for (int i = 0; i < numFilas; i++) {
    double sumaRuntime = 0.0; 
    for (int u = 0; u < numColumnas; u++){
      sumaRuntime = sumaRuntime + matriz[i][u];
    }
    double runtime = sumaRuntime / numColumnas;
    if (i == 0){
      tserie = runtime;
    }
    double speedup = tserie / runtime;
    double efficiency = speedup / procs[i];

    // Guardamos los valores en memoria
    (*runtimes)[i] = runtime;
    (*speedups)[i] = speedup;
    (*efficiencies)[i] = efficiency;

    // Imprimimos los resultados
    printf("%5d %9.6f %8.6f %9.6f\n", procs[i], runtime, speedup, efficiency);
  }
}



void generarGraficos(int *procs, double *runtimes, double *speedups, double *efficiencies, int numFilas) {
  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
  if (!gnuplotPipe) {
    perror("Error al abrir GNUplot");
    exit(1);
  }

  fprintf(gnuplotPipe, "set terminal png size 800,300\n");

  // Gráfico de Runtime
  fprintf(gnuplotPipe, "set output 'runtime.png'\n");
  fprintf(gnuplotPipe, "set title 'Exec. time'\n");
  fprintf(gnuplotPipe, "set xlabel 'Processors'\n");
  fprintf(gnuplotPipe, "set ylabel 'Runtime (ms)'\n");
  fprintf(gnuplotPipe, "plot '-' using 1:2 with linespoints title 'Runtime'\n");

  for (int i = 0; i < numFilas; i++) {
    fprintf(gnuplotPipe, "%d %lf\n", procs[i], runtimes[i]);
  }
  fprintf(gnuplotPipe, "e\n");

  // Gráfico de Speedup y Eficiencia
  fprintf(gnuplotPipe, "set output 'speedup_efficiency.png'\n");
  fprintf(gnuplotPipe, "set title 'Speed Up and Efficiency'\n");
  fprintf(gnuplotPipe, "set xlabel 'Processors'\n");
  fprintf(gnuplotPipe, "set ylabel 'Speed Up'\n");
  fprintf(gnuplotPipe, "set y2label 'Efficiency'\n");
  fprintf(gnuplotPipe, "set y2tics\n");
  fprintf(gnuplotPipe, "set logscale x\n");
  fprintf(gnuplotPipe, "plot '-' using 1:2 with linespoints title 'Speed Up' axis x1y1, '-' using 1:2 with linespoints title 'Efficiency' axis x1y2\n");

  for (int i = 0; i < numFilas; i++) {
    fprintf(gnuplotPipe, "%d %lf\n", procs[i], speedups[i]);
  }
  fprintf(gnuplotPipe, "e\n");

  for (int i = 0; i < numFilas; i++) {
    fprintf(gnuplotPipe, "%d %lf\n", procs[i], efficiencies[i]);
  }
  fprintf(gnuplotPipe, "e\n");

  pclose(gnuplotPipe);
}


int main(int argc, char* argv[]) {

  if (argc != 2){
    printf("\nUso: %s <fichero_de_datos>\n", argv[0]);
    exit(1);
  }

  int *primerosEnteros;
  double **matriz;
  int numFilas, numColumnas, numEnteros;
  double *runtimes, *speedups, *efficiencies;

  procesarArchivo(argv[1], &primerosEnteros, &numEnteros, &matriz, &numFilas, &numColumnas);

  mostrarResultados(primerosEnteros, matriz, numFilas, numColumnas, &runtimes, &speedups, &efficiencies);

  generarGraficos(primerosEnteros, runtimes, speedups, efficiencies, numFilas);

 // Liberamos la memoria dinamica
  free(primerosEnteros);
  free(matriz[0]);
  free(matriz);
  free(runtimes);
  free(speedups);
  free(efficiencies);

  return 0;
}
