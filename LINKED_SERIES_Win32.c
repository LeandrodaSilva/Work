/*
* Autor: Leandro da Silva
* Turma: Es21
* Curso: Engenharia de Software
* Objetivo: Coletar as series assistidas na netflix de um usuários
* por meio de inserção manual ou arquivo, alocar memória para a
* estrutura dos itens com séries encadeadas de registros.
*/
#ifdef _WIN32
#include <windows.h>
#define cls "cls"
#else
#define cls "clear"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define arqname "SERIES.txt"

enum sizes
{/*Valores dos tamanhos usados nos tipos de dados*/
  sdate = 11,
  sname = 100
};

enum options
{
  manual,
  archive
};

typedef struct Item
{/*Estrutura usada para armazenar os registros do Banco de dados*/
  char date[sdate];
  char name[sname];
  struct Item* next;
} item;

#ifdef _WIN32
void gotoxy(int linha, int coluna);
#endif
void pausa(char *text);
item* start();
int   menu();
FILE* Fopen(char *name, char *mode);
item* insert(void *pointer, int option);
item* manual_insert(item *list, char *date, char *name);
item* archive_insert(item *list);
void  Print(item *lista);
void  RecurPrint(item *lista);
void  InvertedRecurPrint(item *lista);
int  FreeList(item *lista);
void Save(item *lista, FILE* arq);
void SaveList(item* lista);
void ContList(item *lista);
void DatePrint(char *data, item *lista);

int quantidade = 0;

int main()
{/*Função principal*/
  #ifdef _WIN32
  //system("mode con:cols=90 lines=30");
  system("color F0");
  #endif
  item *netflix = start();
  char data[sdate] = {'\0'};
  while (1)
  {
    switch (menu())
    {
      case 1: netflix = insert(netflix, manual);
      break;

      case 2: netflix = insert(netflix, archive);
      break;

      case 3: Print(netflix);
      break;

      case 4: system(cls);
      if (netflix != NULL) {
        RecurPrint(netflix);
      }else {
        printf("Nada para imprimir\n");
        pausa("\n\nPressione qualquer tecla para continuar. . .");
      }
      break;

      case 5: system(cls);
      if (netflix != NULL) {
        InvertedRecurPrint(netflix);
      }else printf("Nada para imprimir\n");
      pausa("\n\nPressione qualquer tecla para continuar. . .");
      break;

      case 6:
      if (FreeList(netflix)) {
        gotoxy(23, 34);
        printf("Apagado com sucesso!!\n");
        netflix = start();
        gotoxy(25, 21);
        pausa("Pressione qualquer tecla para continuar...");
      }else{
        gotoxy(21, 34);
        printf("Nada para apagar!!");
        gotoxy(25, 21);
        pausa("Pressione qualquer tecla para continuar...");
      }
      break;

      case 7: SaveList(netflix);
      break;

      case 8:
      quantidade = 0;
      ContList(netflix);
      printf("\n\nQuantidade de itens: %d ", quantidade);
      pausa("\n\n");
      quantidade = 0;
      break;

      case 9:
      do {
        if (strlen(data) > 1 && strlen(data) < 10)
        printf("\nErro: Tamanho da data %d\n", strlen(data));
        printf("\nInforme a data para buscar: [dd/mm/aaaa]? ");
        fflush(stdin);
      } while(scanf("%10[0123456789/]s", data) == 0 || strlen(data) < 10);
      printf("\nItens encontrados: \n\n");
      DatePrint(data, netflix);
      strcpy(data, "");
      pausa("\n\nContinuar...");
      break;

      case 10:
      FreeList(netflix);
      netflix = start();
      gotoxy(25, 21);
      exit(1);
      break;
    }
  }
  return 0;
}

#ifdef _WIN32
/********************************************************************************************************/
void gotoxy(int linha, int coluna)
{
  COORD coord;
  coord.Y = linha;
  coord.X = coluna;

  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  if (!SetConsoleCursorPosition(hConsole, coord))
  {
    printf("Erro: %s\n", strerror(errno));
  }
}

int menu()
{/*Tela principal do programa*/
  int key, i;
  int spc, line;
  do {
    spc = 14; line = 7;
    system(cls);
    gotoxy(2, line+23);
    printf("CADASTRO DE SERIES NETFLIX");
    gotoxy(line , spc);
    printf("\xC9");
    for (i = 0; i < 59; i++) {
      printf("\xCD");
    }
    printf("\xBB\n");
    gotoxy(++line, spc);
    printf("\xBA [01] - Inserir um item manualmente.                       \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [02] - Carregar um arquivo com os meus itens.             \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [03] - Listar os itens de forma iterativa.                \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [04] - Listar os itens de forma recursiva.                \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [05] - Listar os itens de forma recursiva invertida.      \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [06] - Limpar a lista em memoria.                         \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [07] - Salvar lista em arquivo de texto.                  \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [08] - Contar itens em memoria.                           \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [09] - Procurar item por data.                            \xBA\n");
    gotoxy(++line, spc);
    printf("\xBA [10] - Sair                                               \xBA\n");
    gotoxy(++line, spc);
    printf("\xC8");
    for (i = 0; i < 59; i++) {
      printf("\xCD");
    }
    printf("\xBC\n");
    gotoxy(++line, spc);
    printf(": ");
    setbuf(stdin, NULL);
    scanf("%d", &key);
  } while(key < 1 || key > 10);
  return key;
}

item* archive_insert(item* list)
{/*Insere itens na lista*/
  FILE *archive;
  item *new = start();
  char name_arq[50];
  char date[sdate], name[sname];
  int linha = 3, coluna = 10, i;
  do {
    linha = 4; coluna = 10;
    system(cls);
    gotoxy(2, coluna+24);
    printf("CARREGAR DE ARQUIVO");
    gotoxy(linha ,coluna);
    printf("\xC9");
    for (i = 0; i < 67; i++) { // largura da moldura
      printf("\xCD");
    }
    printf("\xBB\n");
    gotoxy(++linha, coluna);
    printf("\xBA Informe o nome do arquivo:                                        \xBA\n");
    gotoxy(++linha, coluna);
    printf("\xBA [xxxxxx.txt]?                                                     \xBA\n");
    gotoxy(++linha , coluna);
    printf("\xBA                                                                   \xBA\n");
    gotoxy(++linha, coluna);
    printf("\xC8");
    for (size_t i = 0; i < 67; i++) {
      printf("\xCD");
    }
    printf("\xBC\n");
    linha -= 2;
    gotoxy(linha, coluna+strlen("  [xxxxxx.txt]? "));
    fflush(stdin);
    scanf("%49s", name_arq);
  } while((archive = Fopen(name_arq, "r")) == NULL);
  while ((fscanf(archive, "%[^;];%[^\n]\n", date, name)) == 2)
  {
    new = manual_insert(new, date, name);
  }
  fclose(archive);
  return new;
}

item* insert(void *pointer, int option)
{/*Insere itens na lista de forma manual ou com arquivos*/
  char *date = calloc(sdate, sizeof(char)), name[sname];
  item *new = start();
  int  linha ,coluna, i;
  switch (option)
  {
    case manual:
    do {
      linha = 4; coluna = 10;
      system(cls);
      gotoxy(2, coluna+24);
      printf("CADASTRO DE ITEM");
      gotoxy(linha ,coluna);
      printf("\xC9");
      for (i = 0; i < 67; i++) { // largura da moldura
        printf("\xCD");
      }
      printf("\xBB\n");
      gotoxy(++linha, coluna);
      printf("\xBA Informe a data: [dd/mm/aaaa]?                                     \xBA\n");
      gotoxy(++linha, coluna);
      printf("\xCC\xCD\xCD\xB9                         \xCC");
      for (i = 0; i < 38; i++) {
        printf("\xCD");
      }
      printf("\xB9\n");
      if (strlen(date) > 0  && strlen(date) < 10){
        gotoxy(6, coluna+5);
        printf("Erro: Tamanho da data %d\n", strlen(date));
        gotoxy(linha, coluna);
      }
      gotoxy(linha+1, coluna);
      printf("\xBA                                                                   \xBA\n");
      gotoxy(linha+2, coluna);
      printf("\xBA Informe o nome:                                                   \xBA\n");
      linha += 3;
      gotoxy(linha, coluna);
      printf("\xBA                                                                   \xBA\n");
      gotoxy(++linha, coluna);
      printf("\xC8");
      for (size_t i = 0; i < 67; i++) {
        printf("\xCD");
      }
      printf("\xBC\n");
      linha -= 5;
      gotoxy(linha, coluna+strlen("  Informe a data: [dd/mm/aaaa]? "));
      fflush(stdin);
      scanf("%10[0123456789/]s", date);
    } while(strlen(date) < 10);
    linha += 3;
    do {
      gotoxy(linha, coluna+strlen("  Informe o nome: "));
      fflush(stdin);
    } while(scanf("%99[^;\n]", name) == 0);
    new =  manual_insert(pointer, date, name);
    linha += 1;
    gotoxy(linha, coluna+16);
    printf("Arquivo carregado com sucesso!!");
    gotoxy(25, 21);
    pausa("Pressione qualquer tecla para continuar...");
    return new;
    break;

    case archive:
    FreeList(pointer);
    pointer = start();
    new = archive_insert(pointer);
    gotoxy(7, 28);
    printf("Arquivo carregado com sucesso!\n");
    gotoxy(25, 21);
    pausa("Pressione qualquer tecla para continuar...");
    return new;
    break;
  }
}

FILE* Fopen(char *name, char *mode)
{/*Faz abertura de arquivos com tratamento de erro*/
  FILE *arq;
  if ((arq = fopen(name, mode)) == NULL) {
    gotoxy(7, 22);
    printf("Erro ocorrido: %s\n", strerror(errno));
    gotoxy(25, 21);
    pausa("Pressione qualquer tecla para continuar...");
    return NULL;
  }else return arq;
}
/*-------------------------------------------------------------------------------------------------------------------------------------*/
#else
int menu()
{/*Tela principal do programa*/
  int key;
  do {
    system(cls);
    printf("[01] - Inserir um item manualmente.\n");
    printf("[02] - Carregar um arquivo com os meus itens.\n");
    printf("[03] - Listar os itens de forma iterativa.\n");
    printf("[04] - Listar os itens de forma recursiva.\n");
    printf("[05] - Listar os itens de forma recursiva invertida.\n");
    printf("[06] - Limpar a lista em memoria.\n");
    printf("[07] - Salvar lista em arquivo de texto.\n");
    printf("[08] - Contar itens em memoria.\n");
    printf("[09] - Procurar item por data. \n");
    printf("[10] - Sair\n\n: ");
    setbuf(stdin, NULL);
    scanf("%d", &key);
  } while(key < 1 || key > 10);
  return key;
}

item* archive_insert(item* list)
{/*Insere itens na lista*/
  FILE *archive;
  item *new = start();
  char name_arq[50];
  char date[sdate], name[sname];
  do {
    printf("\nInforme o nome do arquivo: [xxxxxx.txt]? ");
    fflush(stdin);
    scanf("%49s", name_arq);
  } while((archive = Fopen(name_arq, "r")) == NULL);
  while ((fscanf(archive, "%[^;];%[^\n]\n", date, name)) == 2)
  {
    new = manual_insert(new, date, name);
  }
  fclose(archive);
  return new;
}
item* insert(void *pointer, int option)
{/*Insere itens na lista de forma manual ou com arquivos*/
  char *date = calloc(sdate, sizeof(char)), name[sname];
  item *new = start();
  switch (option) {
    case manual:
    do {
      if (strlen(date) > 1 && strlen(date) < 10)
      printf("\nErro: Tamanho da data %d\n", strlen(date));
      printf("\nInforme a data: [dd/mm/aaaa]? ");
      fflush(stdin);
    } while(scanf("%10[0123456789/]s", date) == 0 || strlen(date) < 10);
    do {
      printf("\nInforme o nome: ");
      fflush(stdin);
    } while(scanf("%99[^;\n]", name) == 0);
    new =  manual_insert(pointer, date, name);
    printf("\n\n Arquivo carregado com sucesso!\n");
    pausa("\n\nContinuar...");
    return new;
    break;

    case archive:
    FreeList(pointer);
    pointer = start();
    new = archive_insert(pointer);
    printf("\n\n Arquivo carregado com sucesso!\n");
    pausa("\n\nContinuar...");
    return new;
    break;
  }
}

FILE* Fopen(char *name, char *mode)
{/*Faz abertura de arquivos com tratamento de erro*/
  FILE *arq;
  if ((arq = fopen(name, mode)) == NULL) {
    printf("\nErro ocorrido: %s\n", strerror(errno));
    return NULL;
  }else return arq;
}
#endif



item* start()
{/*Inicia a lista para inserção*/
  return NULL;
}

item* manual_insert(item* list, char *date, char *name)
{/*Insere itens na lista*/
  item *new = (item*) malloc(sizeof(item));
  strcpy(new->date, date);
  strcpy(new->name, name);
  new->next  = list;
  return new;
}


void Print(item *lista)
{/*Imprime a lista de forma iterativa*/
  system(cls);
  if (lista != NULL) {
    while (lista != NULL){
      printf("%s;%s\n", lista->date, lista->name);
      lista = lista->next;
    }
    pausa("\n\nPressione qualquer tecla para continuar. . .");
  }else{
    printf("Nada para imprimir\n");
    pausa("\nPressione qualquer tecla para continuar. . .");
  }
}

void RecurPrint(item *lista)
{/*Imprime a lista de forma iterativa*/
  if (lista == NULL) {
    pausa("\n\nPressione qualquer tecla para continuar. . .");
    return;
  } else {
    printf("%s;%s\n", lista->date, lista->name);
    RecurPrint(lista->next);
  }
}

void InvertedRecurPrint(item *lista)
{/*Imprime a lista de forma iterativa*/
  if (lista == NULL) {
    return;
  } else {
    InvertedRecurPrint(lista->next);
    printf("%s;%s\n", lista->date, lista->name);
  }
}

int FreeList(item *lista)
{/*Apaga todos os itens da memória*/
  item *atual, *proxima;
  int porcentagem = 0, barra = 0, progresso = 0, estado=0;
  ContList(lista);
  gotoxy(20, 30);
  printf("Quantidade de registros: %d\n", quantidade);
  if (lista != NULL) {
    while (lista != NULL){
      atual = lista;
      proxima = lista->next;
      free(atual);
      lista = proxima;
      /******************Calculo da porcentagem****************/
      progresso =  quantidade - (quantidade - estado);
      porcentagem = (progresso*100)/quantidade;
      printf("  %d%%\r", porcentagem);
      fflush(stdout);
      for (barra = 0; barra < (porcentagem/2); barra++){
        if (barra == 0)
        printf ("               ");
        printf ("%c", 0xDB); //simbolo da barra
      }
      estado++;
      /*******************************************************/
    }
    /*O código abaixo é necessário para que o contador de porcentagem chegue a 100% e não a 99%*/
    porcentagem = (++progresso*100)/quantidade;
    printf("  %d%%\r", porcentagem);
    fflush(stdout);
    for (barra = 0; barra < (porcentagem/2); barra++){
      if (barra == 0)
      printf ("               ");
      printf ("%c", 0xDB); //simbolo da barra
    }
    quantidade = 0; //zera a quantidade para uso em outras funções
    return 1;
  }else{
    return 0;
  }
}

void Save(item *lista, FILE* arq)
{/*Percorre a lista e imprime no arquivo*/
  if (lista == NULL) {
    return;
  } else {
    Save(lista->next, arq);
    fprintf(arq, "%s;%s\n", lista->date, lista->name);
  }
}


void SaveList(item* lista)
{/*Salva a lista em arquivo de texto escolhido pelo usuario*/
  FILE *arq;
  char nome_arq[50];

  if (lista != NULL)
  {
    do {
      printf("\nInforme o nome do arquivo para salvar: [xxxxxx.txt]? ");
      fflush(stdin);
    } while((scanf("%49[a-zA-Z.]s", nome_arq)) != 1);

    arq = Fopen(nome_arq, "w");
    Save(lista, arq);
    printf("Arquivo \"%s\" salvo com sucesso!!\n", nome_arq);
    pausa("\n\nPressione qualquer tecla para continuar. . .");
  }else{
    printf("Nada para Salvar\n");
    pausa("\nPressione qualquer tecla para continuar. . .");
  }
}

void ContList(item *lista)
{/*Conta a quantidade de itens e retorna o resultado*/
  if (lista == NULL) {
    return;
  }else{
    quantidade++;
    ContList(lista->next);
  }
}

void DatePrint(char *data, item *lista)
{/*Percorre a lista e imprime no arquivo*/
  if (lista == NULL) {
    return;
  } else {
    DatePrint(data, lista->next);
    if (strcmp(lista->date, data) == 0) {
      printf("%s;%s\n", lista->date, lista->name);
    }
  }
}

void pausa(char *text)
{/*Efetua uma pausa no programa*/
  printf("%s", text);
  fflush(stdin);
  scanf("%*c");
}
