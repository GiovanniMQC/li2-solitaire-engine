CC = gcc
# -g inclui os símbolos de debug
# -Wall ativa os avisos importantes do compilador
CFLAGS = -Wall -g

golf: main.c cartas.o logica.o
	$(CC) $(CFLAGS) -o $@ $^ 

cartas.o: cartas.c
	$(CC) $(CFLAGS) -c -o $@ $<
logica.o: logica.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf golf cartas.o logica.o