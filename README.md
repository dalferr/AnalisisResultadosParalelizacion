**Trabajo Optativo Tema 0**   (+0.3 puntos extra) 

El objetivo es desarrollar una aplicación en C que sea capaz de leer desde un fichero de texto los resultados, en tiempo de ejecución, de una aplicación paralela. Los datos de entrada son, simplemente, el número de procesadores y el tiempo de ejecución de cada muestra. 



|#P |run1  run2  run3  run4 |run5 |run6  run7 |run8 |
| - | - | - | - | - |
|1 |105\.03  102.19  105.55  96.51 |98\.82 |102\.02  91.42 |98\.11 |
|2 |50\.38  51.04  54.67  51.37 |50\.86 |53\.55  54.67 |53\.06 |
|4 |24\.58  25.09  24.38  26.64 |26\.29 |27\.59  24.16 |23\.71 |
|8 |14\.55  12.57  14.68  14.78 |13\.28 |14\.21  12.45 |12\.41 |
|16 |6\.96  7.02  7.42  7.04 |6\.87 |6\.84  7.83 |7\.20 |
|32 |4\.19  3.94  4.23  4.22 |4\.36 |4\.42  3.72 |4\.16 |
|64 |2\.63  2.46  2.55  2.41 |2\.35 |2\.57  2.40 |2\.44 |

A partir de estos datos el programa debe de calcular la media de los tiempos de ejecución en cada número de procesadores y con estos calcular el factor de aceleración y la eficiencia obtenidos. 



|#procs  runtime         speedup         efficiency |
| - |
|#=====  =======         =======         ========== |
|1  99.956245        1.000000       1.000000 |
|2  52.449997        1.905744       0.952872 |
|4  25.305000        3.950059       0.987515 |
|8  13.616250        7.340952       0.917619 |
|16   7.147501       13.984783       0.874049 |
|32   4.155000       24.056856       0.751777 |
|64   2.476250       40.365971       0.630718 |

Finalmente, el programa dibujará una o varias gráficas de escalabilidad mostrando como varían el tiempo de ejecución, el factor de aceleración y la eficiencia con el número de procesadores. Las gráficas se pueden dibujar utilizando cualquier librería de plotting (e.g. Gnuplot[^1], R[^2], MathGL[^3], pbPlots[^4], PLplot[^5], koolplot[^6], ...) 

Exec. time

100 75 50 25 0![](images/image001.png)

||
| :- |
||
||
||
Speed Up and Efficiency

100

100% 75% 50% 25% 0%![](images/image002.png)

||
| :- |
|Speed Up Efficiency|

10

1

0 10 20 30 40 50 60 70 1 10 100

Processors Processors

La evaluación estará dividida en 3 partes, todas ellas valoradas en 0.1 puntos: 

1. Lectura de fichero y procesamiento de datos 
1. Dibujo de las gráficas 
1. Calidad del código 

[^1]: http://www.gnuplot.info/ 
[^2]: https://www.r-project.org/ 
[^3]: https://mathgl.sourceforge.net/ 
[^4]: https://github.com/InductiveComputerScience/pbPlots 
[^5]: https://plplot.sourceforge.net/index.php 
[^6]: http://www.koolplot.codecutter.org/ 
