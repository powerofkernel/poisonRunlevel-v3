#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <assert.h>

int is_number (char *str) {
  int i;
  for (i=0; str[i] != '\0'; i++)
  {
    if (!isdigit (str[i]))
    {
      return 0;
    }
  }
  return 1;
}
 
#define MAX_BUF 1024
#define PID_LIST_BLOCK 32
 
char *pidof (char *chktipoinit, int pid_init) {
  DIR *openproc;
  FILE *openfile;

  struct dirent *entry;
  char *pidnombre;
  char path[MAX_BUF], read_buf[MAX_BUF];
 
  openproc = opendir ("/proc/");
  if (openproc == NULL)
  {
    perror ("Fail");
    return NULL;
  }
 
  pidnombre = malloc (sizeof (int) * PID_LIST_BLOCK);
  if (pidnombre == NULL)
  {
    return NULL;
  }
  
  int count_primera_linea = 0;
  while ((entry = readdir (openproc)) != NULL) {
    if ( is_number (entry->d_name) ) { // si es numerico el path(pid)
      
      // concatena path con info pid
      strcpy (path, "/proc/");
      strcat (path, entry->d_name);
      strcat (path, "/status");
      //printf("%s\n", path);

      openfile = fopen (path, "r"); // abre archivo

      if (openfile != NULL ) {
        fgets (read_buf, 100, openfile);
        if (strcmp (read_buf, chktipoinit) == 0) { // compara nombre de servicio con argv pidof
          pidnombre = (char *)read_buf;

          if ( atoi(entry->d_name) == pid_init) {
          //printf("pid %s => nombre %s\n", entry->d_name, read_buf);
          fclose (openfile);
          closedir (openproc);
          return pidnombre;
          break;
          }
        }
        
      }
    }

  } // fin while read fopen
}
