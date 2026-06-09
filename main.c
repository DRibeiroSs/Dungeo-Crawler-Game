#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =========================================================
   DUNGEON ESCAPE - main.c (Buffer Tratado com getchar)
   ========================================================= */

/* ---------- VARIÁVEIS GLOBAIS ---------- */
int vidas = 3;
int arma = 0;
char direcao = '>';
int fx, fy; 

int xmon_x[10], xmon_y[10]; 
int ymon_x[2],  ymon_y[2];  
int ny = 2;                 

/* ---------- FUNÇÕES DE TRATAMENTO DE BUFFER (getchar) ---------- */

/* Limpa completamente qualquer resíduo deixado no buffer de entrada (stdin) */
void limpar_buffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Lê apenas o primeiro caractere válido e descarta qualquer excesso digitado */
char ler_caractere() 
{
    char c = getchar();
    if (c != '\n' && c != EOF) 
    {
        limpar_buffer(); // Descarta o resto da linha (ex: se o usuário digitou "sssss")
    }
    return c;
}

/* ---------- HELPERS ---------- */
void frente(char dir, int x, int y)
{
    fx = x; fy = y;
    if(dir=='^') fy--;
    if(dir=='v') fy++;
    if(dir=='<') fx--;
    if(dir=='>') fx++;
}

void exibir_arma()
{
    if(arma==1) printf("ARMA: ESPADA\n");
    if(arma==2) printf("ARMA: ARCO E FLECHA\n");
    if(arma==3) printf("ARMA: CAJADO\n");
    if(arma==0) printf("ARMA: nenhuma\n");
}

int atacar_celula(char mapa[][25], int ay, int ax)
{
    if(mapa[ay][ax]=='k'){ mapa[ay][ax]=' '; return 0; }
    if(mapa[ay][ax]=='X'){ mapa[ay][ax]=' '; return 0; }
    if(mapa[ay][ax]=='Y'){ mapa[ay][ax]=' '; return 0; }
    if(mapa[ay][ax]=='Z'){ mapa[ay][ax]=' '; return 1; } 
    return 0;
}

int aplicar_ataque(char mapa[][25], int linhas, int colunas, int x, int y)
{
    int ax, ay, t;
    int boss_morreu = 0;

    if(arma==1) 
    {
        if(direcao=='^')
        {
            for(ay=y-2;ay<=y-1;ay++)
                for(ax=x-1;ax<=x+1;ax++)
                    if(ay>=0&&ay<linhas&&ax>=0&&ax<colunas)
                        if(atacar_celula(mapa, ay, ax)) boss_morreu=1;
        }
        else if(direcao=='v')
        {
            for(ay=y+1;ay<=y+2;ay++)
                for(ax=x-1;ax<=x+1;ax++)
                    if(ay>=0&&ay<linhas&&ax>=0&&ax<colunas)
                        if(atacar_celula(mapa, ay, ax)) boss_morreu=1;
        }
        else if(direcao=='<')
        {
            for(ax=x-2;ax<=x-1;ax++)
                for(ay=y-1;ay<=y+1;ay++)
                    if(ay>=0&&ay<linhas&&ax>=0&&ax<colunas)
                        if(atacar_celula(mapa, ay, ax)) boss_morreu=1;
        }
        else if(direcao=='>')
        {
            for(ax=x+1;ax<=x+2;ax++)
                for(ay=y-1;ay<=y+1;ay++)
                    if(ay>=0&&ay<linhas&&ax>=0&&ax<colunas)
                        if(atacar_celula(mapa, ay, ax)) boss_morreu=1;
        }
    }
    else if(arma==2) 
    {
        for(t=1;t<=4;t++)
        {
            ax=x; ay=y;
            if(direcao=='^') ay-=t;
            if(direcao=='v') ay+=t;
            if(direcao=='<') ax-=t;
            if(direcao=='>') ax+=t;
            if(ay>=0&&ay<linhas&&ax>=0&&ax<colunas)
                if(atacar_celula(mapa, ay, ax)) boss_morreu=1;
        }
    }
    else if(arma==3) 
    {
        int dx, dy;
        for(dy=-1;dy<=1;dy++)
            for(dx=-1;dx<=1;dx++)
                if(!(dx==0&&dy==0))
                {
                    ay=y+dy; ax=x+dx;
                    if(ay>=0&&ay<linhas&&ax>=0&&ax<colunas)
                        if(atacar_celula(mapa, ay, ax)) boss_morreu=1;
                }
    }
    return boss_morreu;
}

void mover_x(char mapa[][25], int lines, int columns, int idx)
{
    int dir = rand()%4;
    int nx = xmon_x[idx], ny = xmon_y[idx];
    if(dir==0) ny--;
    if(dir==1) ny++;
    if(dir==2) nx--;
    if(dir==3) nx++;
    if(ny>=0&&ny<lines&&nx>=0&&nx<columns&&mapa[ny][nx]==' ')
    {
        mapa[xmon_y[idx]][xmon_x[idx]]=' ';
        xmon_y[idx]=ny; xmon_x[idx]=nx;
        mapa[xmon_y[idx]][xmon_x[idx]]='X';
    }
}

void mover_y(char mapa[][25], int lines, int columns, int idx, int px, int py)
{
    int nx = ymon_x[idx], ny = ymon_y[idx];
    int dx = px - ymon_x[idx], dy = py - ymon_y[idx];
    if(abs(dx)>=abs(dy))
        nx += (dx>0)?1:-1;
    else
        ny += (dy>0)?1:-1;
    if(ny>=0&&ny<lines&&nx>=0&&nx<columns&&mapa[ny][nx]==' ')
    {
        mapa[ymon_y[idx]][ymon_x[idx]]=' ';
        ymon_y[idx]=ny; ymon_x[idx]=nx;
        mapa[ymon_y[idx]][ymon_x[idx]]='Y';
    }
}

/* =========================================================
   ANDAR 1
   ========================================================= */
int jogar_andar1()
{
    char mapa_orig[25][25] =
    {
        {'*','*','*','*','*','*','*','*','*','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ','@','*'},
        {'*',' ','k','k','k',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ','k','D',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ','L','*'},
        {'*','*','*','*','*','*','*','*','*','*'}
    };

    char mapa[25][25];
    int i, j;

reiniciar_a1:
    for(i=0;i<10;i++) for(j=0;j<10;j++) mapa[i][j]=mapa_orig[i][j];

    int x=1, y=1, chaves=0;
    direcao='>';
    char cmd;

    while(1)
    {
        system("cls");
        printf("=== ANDAR 1 ===   VIDAS: %d   CHAVES: %d   ", vidas, chaves);
        exibir_arma();
        printf("\n");

        for(i=0;i<10;i++)
        {
            for(j=0;j<10;j++)
            {
                if(i==y&&j==x) printf("%c ", direcao);
                else printf("%c ",mapa[i][j]);
            }
            printf("\n");
        }

        printf("\nW/A/S/D - MOVER   I - INTERAGIR   O - ATACAR\n");
        cmd = ler_caractere(); // Substituído scanf

        if(cmd=='w'){ direcao='^'; if(mapa[y-1][x]!='*'&&mapa[y-1][x]!='D'&&mapa[y-1][x]!='k') y--; }
        if(cmd=='s'){ direcao='v'; if(mapa[y+1][x]!='*'&&mapa[y+1][x]!='D'&&mapa[y+1][x]!='k') y++; }
        if(cmd=='a'){ direcao='<'; if(mapa[y][x-1]!='*'&&mapa[y][x-1]!='D'&&mapa[y][x-1]!='k') x--; }
        if(cmd=='d'){ direcao='>'; if(mapa[y][x+1]!='*'&&mapa[y][x+1]!='D'&&mapa[y][x+1]!='k') x++; }

        if(mapa[y][x]=='@'){ chaves++; mapa[y][x]=' '; printf("\nVOCE PEGOU UMA CHAVE!\n"); system("pause"); }

        if(cmd=='i')
        {
            frente(direcao, x, y); 
            if(mapa[fy][fx]=='D')
            {
                if(chaves>0){ chaves--; mapa[fy][fx]='='; printf("\nPORTA ABERTA!\n"); system("pause"); }
                else { printf("\nVOCE PRECISA DE UMA CHAVE!\n"); system("pause"); }
            }
        }

        if(cmd=='o') aplicar_ataque(mapa, 10, 10, x, y);

        if(mapa[y][x]=='L')
        {
            if(mapa[5][4] == 'D')
            {
                printf("\n[BARREIRA MAGICA] A escada esta selada! Voce precisa abrir a porta 'D' primeiro!\n");
                system("pause");
                continue;
            }

            system("cls");
            printf("=================================\n");
            printf("      ANDAR 1 CONCLUIDO!\n");
            printf("=================================\n\n");
            printf("Voce encontrou a escada.\nPrepare-se para o Andar 2.\n\n");
            system("pause");
            return 1;
        }

        if(mapa[y][x]=='#')
        {
            vidas--;
            printf("\nVOCE CAIU EM UM ESPINHO! Vidas: %d\n", vidas);
            system("pause");
            if(vidas<=0) return 0;
            goto reiniciar_a1;
        }
    }
}

/* =========================================================
   ANDAR 2
   ========================================================= */
int jogar_andar2()
{
    char mapa_orig[25][25] =
    {
        {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},
        {'*',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ','@',' ','*'},
        {'*',' ','k',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ','#',' ','X',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ','D',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#',' ','*'},
        {'*',' ',' ',' ',' ',' ','X',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*','*','*','*','*',' ',' ',' ',' ',' ',' ',' ','@',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ','O',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ','D',' ',' ',' ',' ','#',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','L','*'},
        {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'}
    };

    char mapa[25][25];
    int i, j;
    int nm = 2; 

reiniciar_a2:
    for(i=0;i<15;i++) for(j=0;j<15;j++) mapa[i][j]=mapa_orig[i][j];
    
    xmon_x[0]=6; xmon_y[0]=3;
    xmon_x[1]=6; xmon_y[1]=6;
    for(i=0;i<nm;i++) mapa[xmon_y[i]][xmon_x[i]]='X';

    int x=1, y=1, chaves=0;
    direcao='>';
    char cmd;
    int turno=0;

    while(1)
    {
        system("cls");
        printf("=== ANDAR 2 ===   VIDAS: %d   CHAVES: %d   ", vidas, chaves);
        exibir_arma();
        printf("\n");

        for(i=0;i<15;i++)
        {
            for(j=0;j<15;j++)
            {
                if(i==y&&j==x) printf("%c ", direcao);
                else printf("%c ",mapa[i][j]);
            }
            printf("\n");
        }

        printf("\nW/A/S/D - MOVER   I - INTERAGIR   O - ATACAR\n");
        printf("Dica: O botao (O) abre uma passagem secreta!\n");
        cmd = ler_caractere(); // Substituído scanf

        if(cmd=='w'){ direcao='^'; if(mapa[y-1][x]!='*'&&mapa[y-1][x]!='D'&&mapa[y-1][x]!='k') y--; }
        if(cmd=='s'){ direcao='v'; if(mapa[y+1][x]!='*'&&mapa[y+1][x]!='D'&&mapa[y+1][x]!='k') y++; }
        if(cmd=='a'){ direcao='<'; if(mapa[y][x-1]!='*'&&mapa[y][x-1]!='D'&&mapa[y][x-1]!='k') x--; }
        if(cmd=='d'){ direcao='>'; if(mapa[y][x+1]!='*'&&mapa[y][x+1]!='D'&&mapa[y][x+1]!='k') x++; }

        if(mapa[y][x]=='@'){ chaves++; mapa[y][x]=' '; printf("\nVOCE PEGOU UMA CHAVE!\n"); system("pause"); }

        if(cmd=='i')
        {
            frente(direcao, x, y);
            if(mapa[fy][fx]=='D')
            {
                if(chaves>0){ chaves--; mapa[fy][fx]='='; printf("\nPORTA ABERTA!\n"); system("pause"); }
                else { printf("\nVOCE PRECISA DE UMA CHAVE!\n"); system("pause"); }
            }
            if(mapa[fy][fx]=='O')
            {
                mapa[7][4]=' '; mapa[7][3]=' '; mapa[7][2]=' '; mapa[7][1]=' ';
                printf("\n*CLIQUE* Uma passagem se abriu!\n");
                system("pause");
            }
        }

        if(cmd=='o')
        {
            aplicar_ataque(mapa, 15, 15, x, y);
            for(i=0;i<nm;i++)
                if(mapa[xmon_y[i]][xmon_x[i]]!='X')
                    xmon_x[i]=-1; 
        }

        turno++;
        for(i=0;i<nm;i++)
        {
            if(xmon_x[i]<0) continue;
            mover_x(mapa, 15, 15, i); 
        }

        for(i=0;i<nm;i++)
        {
            if(xmon_x[i]<0) continue;
            if(xmon_x[i]==x && xmon_y[i]==y)
            {
                vidas--;
                printf("\nVOCE FOI ATINGIDO POR UM MONSTRO! Vidas: %d\n", vidas);
                system("pause");
                if(vidas<=0) return 0;
                goto reiniciar_a2;
            }
        }

        if(mapa[y][x]=='#')
        {
            vidas--;
            printf("\nVOCE CAIU EM UM ESPINHO! Vidas: %d\n", vidas);
            system("pause");
            if(vidas<=0) return 0;
            goto reiniciar_a2;
        }

        if(mapa[y][x]=='L')
        {
            if(xmon_x[0] != -1 || xmon_x[1] != -1)
            {
                printf("\n[BARREIRA MAGICA] A escada esta selada! Derrote os monstros do andar primeiro!\n");
                system("pause");
                continue;
            }

            system("cls");
            printf("=================================\n");
            printf("      ANDAR 2 CONCLUIDO!\n");
            printf("=================================\n\n");
            printf("Voce superou os monstros e os espinhos.\nPrepare-se para o desafio final!\n\n");
            system("pause");
            return 1;
        }
    }
}

/* =========================================================
   ANDAR 3 — Boss Final
   ========================================================= */
int jogar_andar3()
{
    char mapa_orig[25][25] =
    {
        {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ','k','k','k',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ','k',' ','k',' ',' ',' ','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ','k','k','k',' ',' ',' ','#',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ','D',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*','*','*','*','*','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','Y',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','D',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','D',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','Z',' ','*'},
        {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
        {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'}
    };

    char mapa[25][25];
    int i, j;
    int nx_vivos = 0;
    int boss_vivo = 1;

reiniciar_a3:
    for(i=0;i<25;i++) for(j=0;j<25;j++) mapa[i][j]=mapa_orig[i][j];
    
    ymon_x[0]=8;  ymon_y[0]=12;
    ymon_x[1]=14; ymon_y[1]=15;
    for(i=0;i<ny;i++) if(ymon_x[i]>=0) mapa[ymon_y[i]][ymon_x[i]]='Y';
    
    mapa[22][22]='Z'; 
    for(i=0;i<10;i++) xmon_x[i]=xmon_y[i]=-1;
    nx_vivos=0;
    boss_vivo=1;

    int x=1, y=1, chaves=0;
    direcao='>';
    char cmd;
    int turno=0;

    while(1)
    {
        system("cls");
        printf("=== ANDAR 3 - BOSS FINAL ===   VIDAS: %d   CHAVES: %d   ", vidas, chaves);
        exibir_arma();
        if(boss_vivo) printf("BOSS: VIVO\n"); else printf("BOSS: DERROTADO\n");
        printf("\n");

        for(i=0;i<25;i++)
        {
            for(j=0;j<25;j++)
            {
                if(i==y&&j==x) printf("%c ", direcao);
                else printf("%c ",mapa[i][j]);
            }
            printf("\n");
        }

        printf("\nW/A/S/D - MOVER   I - INTERAGIR   O - ATACAR\n");
        printf("Aviso: O boss invoca monstros a cada 3 turnos!\n");
        cmd = ler_caractere(); // Substituído scanf

        if(cmd=='w'){ direcao='^'; if(mapa[y-1][x]!='*'&&mapa[y-1][x]!='D'&&mapa[y-1][x]!='k') y--; }
        if(cmd=='s'){ direcao='v'; if(mapa[y+1][x]!='*'&&mapa[y+1][x]!='D'&&mapa[y+1][x]!='k') y++; }
        if(cmd=='a'){ direcao='<'; if(mapa[y][x-1]!='*'&&mapa[y][x-1]!='D'&&mapa[y-1][x]!='k') x--; } // Corrigido indexador do 'k'
        if(cmd=='d'){ direcao='>'; if(mapa[y][x+1]!='*'&&mapa[y][x+1]!='D'&&mapa[y][x+1]!='k') x++; }

        if(mapa[y][x]=='@'){ chaves++; mapa[y][x]=' '; printf("\nVOCE PEGOU UMA CHAVE!\n"); system("pause"); }

        if(cmd=='i')
        {
            frente(direcao, x, y);
            if(mapa[fy][fx]=='D')
            {
                if(chaves>0){ chaves--; mapa[fy][fx]='='; printf("\nPORTA ABERTA!\n"); system("pause"); }
                else { printf("\nVOCE PRECISA DE UMA CHAVE!\n"); system("pause"); }
            }
        }

        if(cmd=='o')
        {
            int boss_hit = aplicar_ataque(mapa, 25, 25, x, y);
            if(boss_hit)
            {
                boss_vivo=0;
                printf("\n*** VOCE DERROTOU O BOSS! ***\n");
                system("pause");
            }

            for(i=0;i<ny;i++)
                if(ymon_x[i]>=0 && mapa[ymon_y[i]][ymon_x[i]]!='Y') ymon_x[i]=-1;

            for(i=0;i<10;i++)
                if(xmon_x[i]>=0 && mapa[xmon_y[i]][xmon_x[i]]!='X') xmon_x[i]=-1;
        }

        turno++;

        if(boss_vivo && turno%3==0)
        {
            int slot=-1;
            for(i=0;i<10;i++) if(xmon_x[i]<0){ slot=i; break; }
            if(slot>=0)
            {
                int tentativas=20;
                while(tentativas-->0)
                {
                    int rx = 1 + rand()%10;
                    int ry = 1 + rand()%8;
                    if(mapa[ry][rx]==' ')
                    {
                        xmon_x[slot]=rx;
                        xmon_y[slot]=ry;
                        mapa[ry][rx]='X';
                        nx_vivos++;
                        printf("\n*** O BOSS INVOCOU UM MONSTRO! ***\n");
                        system("pause");
                        break;
                    }
                }
            }
        }

        for(i=0;i<ny;i++)
        {
            if(ymon_x[i]<0) continue;
            mover_y(mapa, 25, 25, i, x, y);
        }

        for(i=0;i<10;i++)
        {
            if(xmon_x[i]<0) continue;
            mover_x(mapa, 25, 25, i);
        }

        for(i=0;i<ny;i++)
        {
            if(ymon_x[i]<0) continue;
            if(ymon_x[i]==x && ymon_y[i]==y)
            {
                vidas--;
                printf("\nVOCE FOI ATINGIDO POR UM MONSTRO! Vidas: %d\n", vidas);
                system("pause");
                if(vidas<=0) return 0;
                goto reiniciar_a3;
            }
        }

        for(i=0;i<10;i++)
        {
            if(xmon_x[i]<0) continue;
            if(xmon_x[i]==x && xmon_y[i]==y)
            {
                vidas--;
                printf("\nVOCE FOI ATINGIDO POR UM MONSTRO! Vidas: %d\n", vidas);
                system("pause");
                if(vidas<=0) return 0;
                goto reiniciar_a3;
            }
        }

        if(mapa[y][x]=='#')
        {
            vidas--;
            printf("\nVOCE CAIU EM UM ESPINHO! Vidas: %d\n", vidas);
            system("pause");
            if(vidas<=0) return 0;
            goto reiniciar_a3;
        }

        if(!boss_vivo)
        {
            system("cls");
            printf("*******************************************\n");
            printf("* *\n");
            printf("* VOCE VENCEU!!!                 *\n");
            printf("* *\n");
            printf("*******************************************\n\n");
            printf("O Necromante das Profundezas foi derrotado.\n");
            printf("Suas invocacoes se dissiparam no ar.\n");
            printf("A vila esta salva.\n");
            printf("O heroi retorna coberto de gloria!\n\n");
            printf("Obrigado por jogar DUNGEON ESCAPE!\n\n");
            system("pause");
            return 2;
        }
    }
}

/* =========================================================
   MAIN
   ========================================================= */
int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));
    int opcao;

    while(1)
    {
        system("cls");
        printf("=================================\n");
        printf("       DUNGEON ESCAPE\n");
        printf("=================================\n\n");
        printf("1 - Jogar\n");
        printf("2 - Tutorial\n");
        printf("3 - Creditos\n");
        printf("4 - Sair\n");
        printf("\nUse Tela Cheia para uma Melhor Experiencia.\n\n");
        printf("Escolha: ");
        
        // Tratamento com getchar caso digitem letras no menu numérico
        if (scanf("%d", &opcao) != 1) 
        {
            opcao = -1; // Força cair no loop de opção inválida
        }
        limpar_buffer(); // Garante o consumo do '\n' ou de lixos textuais digitados

        if(opcao == 1)
        {
            vidas = 3;
            arma  = 0;

            char vila[10][10] =
            {
                {'*','*','*','*','*','*','*','*','*','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ','N','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
                {'*',' ',' ',' ',' ',' ',' ',' ','E','*'},
                {'*','*','*','*','*','*','*','*','*','*'}
            };

            int x=1, y=1;
            direcao='>';
            char cmd;
            int i, j;

            while(1)
            {
                system("cls");
                printf("=================================\n");
                printf("            VILA\n");
                printf("=================================\n");
                printf("VIDAS: %d   ", vidas);
                exibir_arma();
                printf("\n");

                for(i=0;i<10;i++)
                {
                    for(j=0;j<10;j++)
                    {
                        if(i==y&&j==x) printf("%c ", direcao);
                        else printf("%c ",vila[i][j]);
                    }
                    printf("\n");
                }

                printf("\nW/A/S/D - MOVER   I - INTERAGIR\n");
                printf("Dica: Fale com o NPC (N) para escolher sua arma!\n");
                printf("Dica: Interaja com a entrada (E) para entrar na masmorra.\n");
                cmd = ler_caractere(); // Substituído scanf

                if(cmd=='w'){ direcao='^'; if(vila[y-1][x]!='*') y--; }
                if(cmd=='s'){ direcao='v'; if(vila[y+1][x]!='*') y++; }
                if(cmd=='a'){ direcao='<'; if(vila[y][x-1]!='*') x--; }
                if(cmd=='d'){ direcao='>'; if(vila[y][x+1]!='*') x++; }

                if(cmd=='i')
                {
                    frente(direcao, x, y);

                    if(vila[fy][fx]=='N')
                    {
                        system("cls");
                        printf("NPC: \"Escolha sua arma, aventureiro!\"\n\n");
                        printf("1 - Espada  (ataca 3x2 a frente)\n");
                        printf("2 - Arco e Flecha  (linha reta, 4 celulas)\n");
                        printf("3 - Cajado  (8 celulas adjacentes)\n\n");
                        printf("Opcao: ");
                        
                        if(scanf("%d", &arma) != 1) 
                        {
                            arma = 1;
                        }
                        limpar_buffer(); // Consome o '\n' gerado aqui

                        if(arma<1||arma>3) arma=1;
                        printf("\nArma escolhida! Boa sorte!\n");
                        system("pause");
                    }

                    if(vila[fy][fx]=='E')
                    {
                        if(arma==0)
                        {
                            printf("\nVoce precisa escolher uma arma antes de entrar!\n");
                            system("pause");
                        }
                        else
                        {
                            system("cls");
                            printf("Voce entra na masmorra...\n\n");
                            system("pause");

                            if(jogar_andar1() == 0) goto game_over;
                            if(jogar_andar2() == 0) goto game_over;
                            if(jogar_andar3() == 0) goto game_over;
                            break;

                            game_over:
                            system("cls");
                            printf("=================================\n");
                            printf("           GAME OVER\n");
                            printf("=================================\n\n");
                            printf("Voce perdeu todas as suas vidas.\n");
                            printf("A masmorra permanece sem ser conquistada...\n\n");
                            system("pause");
                            break;
                        }
                    }
                }
            }
        }
        else if(opcao == 2)
        {
            system("cls");
            printf("=================================\n");
            printf("          TUTORIAL\n");
            printf("=================================\n\n");
            printf("CONTROLES:\nW/A/S/D - Mover\nI - Interagir\nO - Atacar\n\n");
            system("pause");
        }
        else if(opcao == 4)
        {
            break;
        }
        else if(opcao == 3)
        {
            system("cls");
            printf("=================================\n\n");
            printf("Creditos - Victor Hugo & Davi Ribeiro\n\n");
            printf("=================================\n");
            system("pause");
        }
    }
    return 0;
}
