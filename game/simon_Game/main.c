#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartasSimon.h"
#include "logicaSimon.h"
#include "cartasSimon.c"
#include "logicaSimon.c"

struct carta baralho[52];

int main(void)
{
    print_carta(baralho[10]);
    print_naipe(baralho[10]);
    return 0;
}