CC = gcc
# -g inclui os símbolos de debug
# -Wall ativa os avisos importantes do compilador
CFLAGS = -Wall -g

golf: main.c cartasGolf.o logicaGolf.o
	$(CC) $(CFLAGS) -o $@ $^ 

cartas.o: cartasGolf.c
	$(CC) $(CFLAGS) -c -o $@ $<
logica.o: logicaGolf.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf golf cartasGolf.o logicaGolf.o