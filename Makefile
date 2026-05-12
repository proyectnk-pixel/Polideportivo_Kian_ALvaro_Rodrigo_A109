CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

# Nombre del ejecutable
TARGET = polideportivo

# Todos los archivos fuente de la carpeta src
SRCS = src/main.c src/botones.c src/estadisticas.c src/graficas.c src/datos.c src/ventana.c

# Archivos objeto (.o) correspondientes
OBJS = $(SRCS:.c=.o)

# Regla por defecto
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
