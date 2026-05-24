CC = gcc
CFLAGS = -Wall -g
LIBS_TEST = -lcunit

SRCS_COMUNS = logica.c cartas.c

SRC_MAIN = main.c
SRC_TESTE = testes.c

EXEC_JOGO = jogo.out
EXEC_TESTE = testes.out

all: $(EXEC_JOGO) $(EXEC_TESTE)

$(EXEC_JOGO): $(SRC_MAIN) $(SRCS_COMUNS)
	$(CC) $(CFLAGS) -o $@ $^

$(EXEC_TESTE): $(SRC_TESTE) $(SRCS_COMUNS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_TEST)

clean:
	rm -f $(EXEC_JOGO) $(EXEC_TESTE)

test: $(EXEC_TESTE)
	./$(EXEC_TESTE)
