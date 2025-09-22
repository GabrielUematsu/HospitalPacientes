#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <windows.h>

#define MAXFILA 100

struct TpPaciente
{
    char nome[50];
    int cod,prio,tempo,tempochegada;
};

struct TpFila
{
    int fim,inicio,qtd;
    TpPaciente fila[MAXFILA];
};

void Inicializar(TpFila &f){
    f.fim = -1;
    f.qtd = 0;
    f.inicio = 0;
}

char FilaCheia(int qtd){
    return qtd == MAXFILA;
}

char FilaVazia(int qtd){
    return qtd == 0;
}

void Inserir(TpFila &f, TpPaciente elem){
    int pos, ant;
    TpPaciente aux;

    if(f.fim == MAXFILA - 1)
        f.fim = -1; 

    f.fila[++f.fim] = elem;  

 
    pos = f.fim;
    while(f.qtd > 0 && pos != f.inicio){
        if(pos == 0)
            ant = MAXFILA - 1; 
        else
            ant = pos - 1;

        if(f.fila[pos].prio < f.fila[ant].prio){
            aux = f.fila[ant];
            f.fila[ant] = f.fila[pos];
            f.fila[pos] = aux;
        } 

        pos = ant;
    }

    f.qtd++;
}


TpPaciente Retirar(TpFila &f ){
	TpPaciente elem;
    elem = f.fila[f.inicio]; 

    if(f.inicio == MAXFILA - 1)  
        f.inicio = 0;
    else
        f.inicio++;

    f.qtd--; 
    return elem;

}

TpPaciente Inicio(TpFila f){
    return f.fila[f.inicio];
}

TpPaciente Fim(TpFila f){
    return f.fila[f.fim];
}

void Moldura(int CI, int LI,int CF,int LF, int frente,int fundo){
	int a = 0;
	textcolor(frente);
	textbackground(fundo);
	gotoxy(CI,LI); printf("%c",201);
	gotoxy(CF,LI); printf("%c",187);
	gotoxy(CI,LF); printf("%c",200);
	gotoxy(CF,LF); printf("%c",188);
	
	for (a = CI+1 ; a < CF; a++){
		gotoxy(a,LI); printf("%c", 205);
		gotoxy(a, LF); printf("%c",205);
	}
	
	for (a = LI+1 ; a < LF; a++){
		gotoxy(CI,a); printf("%c", 186);
		gotoxy(CF,a); printf("%c",186);
	}
	
	textcolor(7);
	textbackground(0);
	printf("\n\n\n\n");
	
	
}

void FormPrincipal(void){
	
	system("cls");
	Moldura(10, 5, 110, 29 ,11 ,15); // moldura externa
	Moldura(11, 4, 109, 8 ,15 ,4); // moldura do titulo
	gotoxy(50, 16);
	Moldura(11, 9, 109, 26, 7,3); // moldura do jogo
}

int GerarTempo(){
	return 3 + rand() % 4;
}

void MostrarFila(TpFila f, int pagina_atual, int itens_por_pagina) {
    int i, pos;
    int linha_inicial = 14; 
    int coluna_inicial = 58;
        int total_paginas = f.qtd / itens_por_pagina;
    if (f.qtd % itens_por_pagina > 0) {
        total_paginas++;
    }
    if (f.qtd == 0) {
        total_paginas = 1;
    }
    int indice_inicio = (pagina_atual - 1) * itens_por_pagina;
    gotoxy(coluna_inicial, linha_inicial); 
    textcolor(11); 
    printf("Fila de Espera (%d/%d) - Pagina %d de %d", f.qtd, MAXFILA, pagina_atual, total_paginas);

    for(i = 0; i < itens_por_pagina + 1; i++) { 
        gotoxy(coluna_inicial, linha_inicial + 2 + i);
        printf("%-45s", " "); 
    }
    
    if (!FilaVazia(f.qtd)) {
        for(i = 0; i < itens_por_pagina && (indice_inicio + i) < f.qtd; i++) {
            int item_atual = indice_inicio + i;
            pos = (f.inicio + item_atual) % MAXFILA;
            
            gotoxy(coluna_inicial, linha_inicial + 2 + i);
            
            if (f.fila[pos].prio == 1)      
				textcolor(12);
            else if (f.fila[pos].prio == 2) 
					textcolor(14);
            	else if (f.fila[pos].prio == 3) 
						textcolor(10);
            			else                            
							textcolor(7);
            
            printf(" %d. %s (Prio: %d)", item_atual + 1, f.fila[pos].nome, f.fila[pos].prio);
        }
    }
    textcolor(7);
}

void Simular(TpFila f){
    TpPaciente pac, atual;
    atual.tempo = 0;
    int ut = 0;
    int somaatend=0, somaespera=0, somatotal=0;
	int itens = 8;
    FILE *ptr = fopen("Pacientes.txt","r");
    
    int pagina_atual = 1;
    char tecla;
    int flag = 0;

    if(ptr == NULL){
        gotoxy(12,12); textcolor(12);
        printf("Erro ao abrir arquivo!  \n");

    }
    int linhaatend = 11;
    int linhachamada = 6;
	int linha = 15;

    gotoxy(12, 25); textcolor(15);
    printf("Use 'A' e 'D' para navegar pelas paginas ou pressione outra tecla para sair.");

    do {
        gotoxy(12, 6); textcolor(6);
        printf("Tempo: %d ut", ut);

        if(atual.tempo > 0) {
            gotoxy(33, linhaatend); textcolor(14); 
            printf("Atendimento de %s: %d ut restantes      ", atual.nome, atual.tempo);
            atual.tempo--;
            if(atual.tempo == 0) {
                gotoxy(33, linhaatend + 1); textcolor(10); 
                printf("Paciente %s finalizou o atendimento.    ", atual.nome);
                somaatend++;
            }
        } 
        else if(atual.tempo == 0 && !FilaVazia(f.qtd)) {
            gotoxy(33, linhaatend + 1);
            printf("                                                    ");
            
            pac = Retirar(f);
            int tempofila = ut - pac.tempochegada;
            atual = pac;
            atual.tempo = GerarTempo();
            
            somaespera += tempofila;
            somatotal += tempofila + atual.tempo;
            
            gotoxy(33, linhachamada); textcolor(15);
            printf("ALERTA: %-15s (esperou %d ut) -> Atendimento: %d ut       ", atual.nome, tempofila, atual.tempo);
        }

		if(ut != 0 && ut % 3 == 0){
			pac.tempo  = 0;
				if(fscanf(ptr,"%[^;];%d;%d\n",&pac.nome,&pac.cod,&pac.prio) == 3){
					pac.tempochegada = ut;
					if(!FilaCheia(f.qtd)){
						Inserir(f,pac);					
						gotoxy(12, linha);textcolor(6);
						printf("Paciente %s entrou na fila    ", pac.nome);
						Sleep(1000);
						gotoxy(12,linha);
						printf("                                            ");
						if(linha > 24)
						linha = 14;
					}
				}	
		}

        if (kbhit()) {
            tecla = getch();
            tecla = tolower(tecla);
            
            int total_paginas;
            if (f.qtd == 0) {
                total_paginas = 1;
            } else {
                total_paginas = (f.qtd - 1) / itens + 1;
            }

            if (tecla == 'd') {
                if (pagina_atual < total_paginas) pagina_atual++;
            } else if (tecla == 'a') {
                if (pagina_atual > 1) pagina_atual--;
            } else {
                flag = 1;
            }
        }

        MostrarFila(f, pagina_atual,itens);

        ut++;
        Sleep(400);

    } while (!flag && (atual.tempo > 0 || !FilaVazia(f.qtd) || !feof(ptr)));
    
    fclose(ptr);

    FormPrincipal();
    float tempomedioespera, tempomedio;
    tempomedio =  (float)somatotal / somaatend;
    tempomedioespera = (float)somaespera / somaatend;

    gotoxy(42,6); textcolor(13); printf("HOSPITAL DO LEANDRAO");
    gotoxy(40, 11); textcolor(13); printf("=-=-=-= FIM DA SIMULACAO =-=-=-=");
    gotoxy(40, 13); textcolor(6); printf("Pacientes atendidos: %d", somaatend);
    if(somaatend > 0) {
        gotoxy(40, 15); textcolor(6); printf("Tempo medio total (fila+atend): %.2f ut", tempomedio);
        gotoxy(40, 17); textcolor(6); printf("Tempo medio de espera na fila: %.2f ut", tempomedioespera);
    }
    gotoxy(40,23); textcolor(10); printf("GABRIEL KENJI UEMATSU  262320290");
    
    getch();
}


// NOME NA HORA DO ATENDIMENTO TEM Q SAIR ASSIM SE TEMPO DE ATE = 10;
//JOAO -> MARIA -> LUCAS -> PATRICIA E ASSIM POR DIANTE;