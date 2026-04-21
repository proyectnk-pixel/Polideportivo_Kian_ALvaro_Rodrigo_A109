# Requisitos Funcionales — Polideportivo Madrid (A109)

---

## 1. Requisitos Mínimos (Obligatorios)

### Funcionalidades Básicas

| ID   | Requisito Funcional |
|------|---------------------|
| RF01 | El sistema debe leer ficheros de datos CSV y procesarlos en memoria para su análisis. |
| RF02 | El sistema debe mostrar el listado de actividades de un centro deportivo concreto seleccionado por el usuario. |
| RF03 | El sistema debe calcular y mostrar la frecuencia diaria de las actividades ofertadas. |
| RF04 | El sistema debe identificar y mostrar cuál es la actividad más popular (mayor ocupación) en cada centro deportivo. |

### Reglas de Programación en C

| ID   | Requisito |
|------|-----------|
| RNF01 | **Reparto de tareas:** todos los integrantes del grupo deben programar partes del código. |
| RNF02 | **Modularidad:** el código debe organizarse obligatoriamente mediante funciones. |
| RNF03 | **Estructuras de datos:** uso obligatorio de vectores de estructuras para manejar la información. |
| RNF04 | **Memoria dinámica:** el programa debe gestionar los datos con asignación dinámica de memoria. |
| RNF05 | **Prohibición:** no se pueden usar variables globales bajo ningún concepto. |
| RNF06 | **Prohibición:** está prohibido el uso de la instrucción `goto`. |
| RNF07 | **Prohibición:** no se debe usar `break` o `continue` de forma reiterada. |

---

## 2. Requisitos Adicionales (Para subir nota)

### Funcionalidades Avanzadas

| ID   | Requisito Funcional |
|------|---------------------|
| RF05 | **Interfaz Gráfica (Raylib/RayGUI):** crear una interfaz visual interactiva que sustituya a la consola, con ventanas, botones y paneles. |
| RF06 | **Análisis de ocupación:** identificar las franjas horarias y días concretos en que los centros están al 100% de su aforo. |
| RF07 | **Sistema de sugerencias:** si una actividad está al 100% de ocupación, sugerir actividades alternativas disponibles en el mismo centro y franja horaria. |
| RF08 | **Representación gráfica:** mostrar la evolución y distribución de los datos mediante gráficas de barras o líneas implementadas con Raylib. |

### Méritos Extra

| ID   | Mérito | Puntos |
|------|--------|--------|
| ME01 | **Repositorio de código:** uso de GitHub/GitLab de forma colaborativa durante todo el desarrollo. | +1.0 |
| ME02 | **Guía de estilo:** aplicar una normativa de estilo homogénea en todo el código fuente. | +0.5 |
| ME03 | **Originalidad:** claridad y creatividad en la algoritmia empleada. | — |
