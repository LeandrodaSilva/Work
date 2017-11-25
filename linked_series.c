#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#define cls "cls"
#else
#define cls "clear"
#endif

#define arqname "MY_SERIES.txt"

enum sizes
{/*Valores dos tamanhos usados nos tipos de dados*/
  sdate = 8,
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

item* start();
int   menu();
FILE* Fopen(char *mode);
void insert(void *pointer, int option);
item* manual_insert(item* list, char *date, char *name);
void archive_insert(item* list);
void  print();

int main()
{/*Função principal*/
  item *netflix = start();

  while (1)
  {
    switch (menu())
    {
      case 1: insert(netflix, manual);
      break;

      case 2: insert(netflix, archive);
      break;

      case 3:
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

void archive_insert(item* list)
{/*Insere itens na lista*/
  FILE *archive;
  archive = Fopen("r");
  char date[sdate], name[sname];
  while ((fscanf(archive, "%[^;];%[^\n]\n", date, name)) == 2) {
  }
}

void insert(void *pointer, int option)
{
  switch (option) {
    case manual:
    break;
    case archive: archive_insert(pointer);
    break;
  }
}

FILE* Fopen(char *mode)
{/*Faz abertura de arquivos com tratamento de erro*/
  FILE *arq;
  if ((arq = fopen(arqname, mode)) == NULL) {
    printf("\nErro ocorrido: %s\n", strerror(errno));
  }else return arq;
}

void print()
{/*Imprime a lista de forma iterativa*/

}
