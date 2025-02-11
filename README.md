# Análisis de Rendimiento de Aplicaciones Paralelas

## Descripción
Este programa consiste en el desarrollo de una aplicación en C que analiza el rendimiento de una aplicación paralela. Para ello, se leen los resultados de tiempo de ejecución desde un archivo de texto, se calculan las métricas de rendimiento y se generan gráficas de escalabilidad.

## Características
- **Lectura de datos**: Se extraen el número de procesadores y los tiempos de ejecución de diferentes muestras desde un archivo de texto.
- **Cálculo de métricas**:
  - Tiempo de ejecución promedio por número de procesadores.
  - Factor de aceleración.
  - Eficiencia.
- **Generación de gráficas**: Representación visual de los resultados utilizando la librería Gnuplot.

## Formato de Entrada
El archivo de entrada debe contener datos en el siguiente formato:
```
#P run1 run2 run3 run4 run5 run6 run7 run8
1 105.03 102.19 105.55 96.51 98.82 102.02 91.42 98.11
2 50.38 51.04 54.67 51.37 50.86 53.55 54.67 53.06
...
```

## Formato de Salida
El programa genera una tabla con los valores calculados:
```
#procs  runtime         speedup         efficiency
#=====  =======         =======         ==========
     1  99.956245        1.000000       1.000000
     2  52.449997        1.905744       0.952872
...
```

## Instalación y Ejecución
1. Clonar el repositorio:
   ```sh
   git clone https://github.com/dalferr/AnalisisResultadosParalelizacion.git
   ```
2. Compilar el programa:
   ```sh
   gcc -o analisis analisis.c
   ```
3. Ejecutar el programa:
   ```sh
   ./analisis datos.txt
   ```
