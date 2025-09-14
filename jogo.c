#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#define LARGURA 50
#define ALTURA 14

//Aqui é a matriz onde tudo será desenhado antes de ser printado na tela
char telaJogo[ALTURA][LARGURA + 1];

char labirinto[ALTURA][LARGURA + 1] = 
{
    "##################################################",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "                                                  ",
    "                                                  ",
    "                                                  ",
    "                                                  ",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "#                                                #",
    "##################################################"
};

//cobrinha => lista simplesmente encadeada
struct sPOS
{
    int lin, col;
};

struct sNODE {
    struct sPOS p;
    struct sNODE *prox;
};

struct sNODE *cobrinha = NULL;

struct sPOS maca;

int pos_igual(struct sPOS a, struct sPOS b);
int verifica_ocupa(int lin, int col);
void cobra_push(struct sPOS p);
void cobra_pop();
void cobra_free();
void copiar_labirinto();

void desenhar_maca();
void desenhar_cobra();
void printar_tela();

void escolher_maca();
int movimento(char ent, int *dlin, int *dcol);
int bateu_parede(struct sPOS p);
int livre_p_maca(int lin, int col);

int main() {
    //preparando números randômicos
    srand(time(NULL));
    
     //antes de começar, precisamos preparar a tela
    //1 - copiar labirinto em telaJogo
    //2 - colocar maça (@) em telaJogo
    //3 - colocar cobrinha
    //4 - printa na tela (terminal) a matriz telaJogo
    
struct sPOS inicio = {ALTURA/2, LARGURA/2}; 
cobra_push(inicio);

escolher_maca();

copiar_labirinto();
desenhar_maca();
desenhar_cobra();
printar_tela();
    
    
while(1) // loop game
{
    char ent=_getch();

    if (ent=='q' || ent=='Q') { // se digitar q ou Q sai do jogo
        printf("Jogo encerrado! A cobrinha foi descansar 🐍💤 \n");
        break; // sai do while
    }

    int dlin, dcol;
    // se a tecla não for válida, volta para o início do while
    if (!movimento(ent, &dlin, &dcol))
        continue;

    struct sPOS p_cabeca = cobrinha->p; // aqui eu vou pegar a posição atual da cabeça da cobra

    struct sPOS nova; // a nova posição que essa cabeça vai ter
    nova.lin = p_cabeca.lin + dlin; // aqui eu estou calculando os novos valores (cima e baixo)
    nova.col = p_cabeca.col + dcol; // direita e esquerda, o ponto vai pegar o valor específico do ponto que vou pedir dentro da struct 

    if (bateu_parede(nova)) { // verifica se bateu na parede
        printf("🔚 GAME OVER 🔚 \n");
        printf("Bateu em uma parede 🧱 \n");
        break;
    }
    
    int comeu = pos_igual(nova, maca); // checando se a nova posição da cobra é a mesma da maçã
                                       // se for, significa que comeu e guardamos na variável "comeu"
    
   struct sNODE *n = cobrinha; // ponteiro para percorrer o corpo da cobra
    while (n) { // percorre o corpo da cobra
        if (pos_igual(n->p, nova)) { // se a nova posição for igual a alguma parte do corpo
            printf("🐍💥🐍\n");
            printf("GAME OVER\n");
            printf("Você mordeu o próprio rabo!\n");
            cobra_free();
            return 0;
        }
        n = n->prox; // avança para o próximo nó
    }

    cobra_push(nova); // adiciona a nova cabeça
    if (comeu)        // se comeu, sorteia uma nova maçã
        escolher_maca();
    else              // se não comeu, remove a cauda
        cobra_pop();
    
    
    copiar_labirinto();
    desenhar_maca();
    desenhar_cobra();
    printar_tela();
}

cobra_free();
return 0;
}

        //2 - processar entrada do jogador
        
        //2.1 pegou maça?
        //2.2 bateu em parede?
        //2.3 bateu no rabo
        //3 - redesenha tudo (labirinto, maça e cobrinha)
        //4 - printa na tela


// funçoes abaixo da main 

int pos_igual(struct sPOS a, struct sPOS b) {
    if (a.lin == b.lin && a.col == b.col){
        return 1;
    } else {
        return 0;
    }
    } // para saber se a cabeça da cobra vai estar na mesma posição que a maça e realizar a pega, ou para saber se bateu no próprio rabo 
int movimento(char ent, int *dlin, int *dcol) { //para ver qual das teclas vai entrar e qual será o movimento 
    *dlin=*dcol=0; // vou inicialzar com 0
    if (ent == 'w' || ent=='W') 
    *dlin = -1; //cima, então tem que diminuir um na linha, por isso -1 e assim sucessivamente 
    else if (ent=='s' || ent=='S')
    *dlin=1;
    else if (ent=='a' || ent=='A')
    *dcol=-1;
    else if (ent=='d'||ent=='D')
    *dcol=1;
    else 
    return 0; // se foi inválida
    return 1; // se foi válida
    
} 

int verifica_ocupa(int lin, int col){ //aqui vou verificar se alguma parte da cobra ocupa a posiçaun 
     struct sNODE *n=cobrinha;
     while (n!=NULL){
         if (n->p.lin == lin && n->p.col == col) {
             return 1;
         }
         n=n->prox;
     }
     return 0;
}

// acaba pelo o amor de deueeeueues
void cobra_push(struct sPOS p){ // vai criar uma nova cabeça para a cobra, vai receber o p que é a posição que deve colocar 
struct sNODE *n = (struct sNODE*)malloc(sizeof *n); 
if (n==NULL) {
    printf("ERRO DE ALOCAÇÃO\n");
    exit(1);
}
n->p=p;
n->prox=cobrinha; // apontando para inde a cobra estava antes 
cobrinha=n; // atualizando 
}

void cobra_pop(){// aqui eu vou remover o rabo da cobra 
    if (cobrinha==NULL) return; // se não tiver o que remover faz isso
    if (cobrinha->prox==NULL){ //caso tenha só um nó - liberar
        free(cobrinha);
        cobrinha=NULL;
        return;
    }
    struct sNODE *ant = NULL, *atual = cobrinha;
    while (atual->prox){
        ant=atual;
        atual=atual->prox; // avança
    }
    ant->prox=NULL;
    free(atual);
}
    
void cobra_free(){ //liberar a bixinha toda
    while (cobrinha)
    cobra_pop();
}

void copiar_labirinto(){
    for (int i=0; i<ALTURA; i++){ //pecorre linha
        for (int j=0; j<LARGURA; j++)//coluna
            telaJogo[i][j]=labirinto[i][j]; //VAI COPIAR O CARACTER DA POSIÇÃO - ISSO TA LÁ EM CIMAA
        telaJogo[i][LARGURA]='\0'; //colocar o nulo
    }
}

void desenhar_maca(){
    telaJogo[maca.lin][maca.col]='@';
}

void desenhar_cobra(){
    if (cobrinha==NULL) return; // não tem o que desenhar
    telaJogo[cobrinha->p.lin][cobrinha->p.col]='O';
    struct sNODE *n=cobrinha->prox;  // começar no segunda no 
    while (n!=NULL){
    telaJogo[n->p.lin][n->p.col]='o';
    n=n->prox; 
    }
}
void printar_tela(){ // desenhar linha por linha
    for (int i=0; i<ALTURA; i++){
        printf("%s\n", telaJogo[i]);
    }
    printf("Controles: W A S D | Clique em Q para sair 👋\n");
}

void escolher_maca(){ // sortear a linha e coluna 
    int lin, col;
    do {
        lin=rand() % ALTURA;
        col=rand() % LARGURA;
        
    }while (labirinto [lin][col]=='#' || verifica_ocupa(lin, col));
    maca.lin = lin;
    maca.col=col;
}
int bateu_parede(struct sPOS p){ 
   if (p.lin<0 || p.lin >= ALTURA || p.col<0 || p.col>= LARGURA)
       return 1;
    return labirinto[p.lin][p.col]=='#';
}
int livre_p_maca(int lin, int col){ // se posso colocR a maca 
    if (lin<0 || lin >= ALTURA || col < 0 || col>=LARGURA) // fora do desenho do tabuleiro
       return 0;
    if (labirinto[lin][col]=='#') // ver na parede
       return 0;
    if (verifica_ocupa(lin, col))
       return 0; // vee se alguma parte da cobra ocupa esse lugar
    return 1; // caso passou por tudo
    
}
