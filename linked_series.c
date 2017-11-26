#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#define cls "cls"
#else
#define cls "clear"
#endif

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
void  FreeList(item *lista);

int main()
{/*Função principal*/
  item *netflix = start();

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

      case 6: FreeList(netflix);
      netflix = start();
      break;

      case 7:
      break;

      case 8:
      break;

      case 9:
      break;

      case 10: exit(1);
      break;
    }
  }
  return 0;
}

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

item* archive_insert(item* list)
{/*Insere itens na lista*/
  FILE *archive;
  item *new = start();
  archive = Fopen(arqname, "r");
  char date[sdate], name[sname];
  while ((fscanf(archive, "%[^;];%[^\n]\n", date, name)) == 2)
  {
    new = manual_insert(new, date, name);
  }
  return new;
}

item* insert(void *pointer, int option)
{
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
    } while(scanf("%99[^;]", name) == 0);
    return  manual_insert(pointer, date, name);
    break;

    case archive:
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
  }else return arq;
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

void FreeList(item *lista)
{
  item *atual, *proxima;
  if (lista != NULL) {
    while (lista != NULL){
      atual = lista;
      proxima = lista->next;
      printf("Liberando memoria: %p\n", atual);
      free(atual);
      lista = proxima;
    }
    printf("\nApagado com Sucesso!\n");
    pausa("Continuar...");
  }else{
    printf("\n\nA lista esta vazia!\n");
    pausa("Continuar...");
  }
}

void pausa(char *text)
{/*Efetua uma pausa no programa*/
  printf("%s", text);
  fflush(stdin);
  scanf("%*c");
}
