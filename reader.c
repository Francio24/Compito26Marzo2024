#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BLOCK_DIM 1024
#define PROGRAM "writer"

 /*!
 \fn spawn
 \brief crea un nuovo processo figlio eseguito in background
 \param[in] percorso, array_di_stringhe
 */
int spawn(char program[], char* argument[])
{
 int pid;
 pid = fork ();
 if (pid < 0)
   return -1;
 if (pid > 0)
   return 0;
 execv(program, argument);
 abort();
}

int main(int argc, char* argv[])
{
 FILE* file;
 int fifo;
 unsigned char buffer[BLOCK_DIM];
 int n;
 char* arg[3];

 if (argc != 3)
 {
  printf("Uso: %s file-origine file-destinazione\r\n", argv[0]);
  return 0;
 }
 /*!
 \fn malloc
 \brief ritorna un blocco di memoria
 \param[in] grandezza_memoria
 \return size
 */
 arg[0] = (char*)malloc(strlen(PROGRAM) + 1);
 /*!
 \fn strcpy
 \brief copia il contenuto nella stringa arg[0] nel file PROGRAM
 \param[in] Stringa in quale copiare, nome_file di dove si trova la stringa
 */
 strcpy(arg[0], PROGRAM);
 arg[1] = (char*)malloc(strlen(argv[2]) + 1);
 strcpy(arg[1], argv[2]);
 arg[2] = NULL;
 
 
 if (spawn(PROGRAM, arg) < 0)
 {
  printf("Errore creazione processo\r\n");
 /*!
 \fn free
 \brief libera la memoria
 \param[in] variabile_da_liberare
 */
  free(arg[0]);
  free(arg[1]);
  return 0;
 }
  /*!
 \fn open
 \brief apre un file
 \param[in] String nome_file, modalita
 */
 fifo = open("my_fifo", O_WRONLY);
 if (fifo < 0)
 {
  printf("Errore apertura FIFO\r\n");
  free(arg[0]);
  free(arg[1]);
  return 0;
 }
  /*!
 \fn fopen
 \brief ritorna il file in modalita' lettura o scrittura
 \param[in] String nome_file, modalita
 \return FILE
 */
 file = fopen(argv[1], "rb");
 if (file == NULL)
 {
  printf("Errore apertura file \"%s\"\r\n", argv[1]);
  close(fifo);
  free(arg[0]);
  free(arg[1]);
  return 0;
 }
 while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0)
      write(fifo, buffer, n);
 fclose(file);
 close(fifo);
 free(arg[0]);
 free(arg[1]);
 return 1;
}
