CC = gcc
# -g inclui os símbolos de debug
# -Wall ativa os avisos importantes do compilador
CFLAGS = -Wall -g

simpleSimon: main.c cartasSimon.o logicaSimon.o
	$(CC) $(CFLAGS) -o $@ $^ 

cartasSimon.o: cartasSimon.c
	$(CC) $(CFLAGS) -c -o $@ $<
logicaSimon.o: logicaSimon.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf simpleSimon cartasSimon.o logicaSimon.o