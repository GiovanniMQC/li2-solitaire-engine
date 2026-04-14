#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "golf.c"
#include "simpleSimon.c"

int main(void)
{
    int gameType = 0;
    while (!(gameType == 1 || gameType == 2))
        scanf("Escolhe o jogo (1- Golf, 2- Simple Simon): %d", &gameType);
    
    if (gameType == 1)
    {
        golfSetup();
    }
    else if (gameType == 2)
    {
        simonSetup();
    }
}