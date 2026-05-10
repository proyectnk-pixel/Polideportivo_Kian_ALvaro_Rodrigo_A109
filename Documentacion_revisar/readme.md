# Polideportivo Madrid - Análisis Estadístico (Grupo 18)

Aplicación desarrollada en C para la carga, análisis y visualización de datos estadísticos sobre las actividades en los polideportivos del Ayuntamiento de Madrid.

Este proyecto corresponde a la Tarea de Programación del grupo A109, implementando los requerimientos funcionales y requisitos adicionales exigidos, incluyendo una interfaz gráfica interactiva utilizando **GTK3**.

##  Funcionalidades

- **Carga de Datos:** Lectura y procesamiento de ficheros CSV (`dataset.csv`).
- **Listado y Filtrado:** Visualización de actividades por centro deportivo con un buscador interactivo en tiempo real.
- **Estadísticas (Ranking):** Cálculo y ordenación de centros deportivos según su ocupación media utilizando algoritmos de ordenación.
- **Representación Gráfica:** Generación de gráficas de barras integradas para visualizar los porcentajes de ocupación por actividad.
- **Gestión Dinámica:** Uso intensivo de vectores de estructuras y memoria dinámica, garantizando la no utilización de variables globales (cumpliendo con la rúbrica).

##  Requisitos Previos

Para compilar y ejecutar el proyecto, necesitas tener instalado un compilador de C (`gcc`), `make` y las librerías de desarrollo de **GTK3**.

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev pkg-config
```

### Windows (Recomendado MSYS2)
1. Descarga e instala MSYS2.
2. Abre la terminal **"MSYS2 MinGW 64-bit"**.
3. Instala las herramientas necesarias:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkgconf make
   ```

## Compilación y Ejecución

1. Abre una terminal en la raíz del proyecto.
2. Compila el código fuente usando el `Makefile` incluido:
   ```bash
   make
   ```
3. Ejecuta la aplicación:
   - En **Linux**: `./polideportivo`
   - En **Windows** (MSYS2): `./polideportivo.exe`

## Limpieza

Para eliminar los archivos compilados temporalmente (`.o`) y el ejecutable, ejecuta:
```bash
make clean
```

## 👥 Autores

- Kian
- Álvaro
- Rodrigo
