#include <stdio.h>
#include <windows.h>



int main(int argc, char const *argv[]) {
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); //adquire o o console em uso na aplicacao
  CONSOLE_CURSOR_INFO info; 
  info.dwSize = 10; //define o tamanho do cursor
  info.bVisible = FALSE; //Desabilita o cursor do console
  char nome[50];
  SetConsoleCursorInfo(consoleHandle, &info); // envia as configuracoes feitas
  printf("nome: ");
  gets(nome);
  printf("%s\n", nome);
  return 0;
}
