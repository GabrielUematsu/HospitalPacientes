#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <windows.h>
#include "Tad.h"

int main(void){
    TpFila f;
    Inicializar(f);
    FormPrincipal();
    Simular(f);
    getche();
    return 0;
}