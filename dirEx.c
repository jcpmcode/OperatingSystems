#include <curses.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>

//getcwd ?

int leeChar();
char *hazLinea(char *base, int dir);
char *mapFile(char *filePath);
char *upDir(char* str);

//Variables globales
int fd;

int main()
{
   int i;
   int c;
   int cant;
   long size;
   char *buf;
   char *ptr;
   
   initscr();
   raw();
   noecho(); /* No muestres el caracter leido */
   cbreak(); /* Haz que los caracteres se le pasen al usuario */

   //Obtener dir
   struct s_dir {
	   int tipo;
	   char *nombre;
   } res[128];

   //Obtener el tamaño maximo posible del Path SRC: https://pubs.opengroup.org/onlinepubs/009695399/functions/getcwd.html
   size = pathconf(".", _PC_PATH_MAX);

   //Obtener el path actual
   if ((buf = (char *)malloc((size_t)size)) != NULL)
      ptr = getcwd(buf, (size_t)size);

   move(0,0);
   printw("%s MAX: %d", ptr, size);

   //Abrir el path
   DIR *dir = opendir(ptr);
   struct dirent *dp;
   cant=0;
   while((dp=readdir(dir)) != NULL) {
      res[cant].tipo = dp->d_type;
      res[cant].nombre=dp->d_name;
      cant++;
   }

   //Lista de opciones
    do {
        for(int j=0; j<cant; j++) {
            if (j == cant) {
                attron(A_REVERSE);
            }
            if (res[j].tipo == DT_DIR) {
                mvprintw(5+j,1,"D ");
            }
            else {
                mvprintw(5+j,1,"F ");
            }
            mvprintw(5+j,5,res[j].nombre);
            attroff(A_REVERSE);
    }
    move(5+i,5);
    refresh();
    c = leeChar();
      
    switch(c) {

        case 0x1B5B41: // ARRIBA
            i = (i>0) ? i - 1 : cant-1;
            break;
        case 0x1B5B42: // ABAJO
            i = (i<cant-1) ? i + 1 : 0;
            break;
        case 0xA:
            move(0,0);

            //Checar si es DIR
            if (res[i].tipo == DT_DIR) {

                if (strcmp(res[i].nombre, ".") == 0) {
                    //Hacer nada pues
                    
                }else if (strcmp(res[i].nombre, "..") == 0) { //Checar si es atras
                
                    //Obtener el path actual
                    if ((buf = (char *)malloc((size_t)size)) != NULL)

                    ptr = getcwd(buf, (size_t)size);

                    char *newPtr = upDir(ptr);
    
                    //Abrir el path

                    DIR *dir = opendir(newPtr);
                    struct dirent *dp;
                    cant=0;
                    while((dp=readdir(dir)) != NULL) {
                        res[cant].tipo = dp->d_type;
                        res[cant].nombre=dp->d_name;
                        cant++;
                    }
                } else {
                    //Abrir directorio
                    char newDir [255];
                    strcpy(newDir, "/");
                    strcat(newDir, res[i].nombre);
                    move(20,0);
                    printw("Abriendo: %s",newDir);

                    cant=0;
                    opendir(newDir);
                    
                    while((dp=readdir(dir)) != NULL) {
                        res[cant].tipo = dp->d_type;
                        res[cant].nombre=dp->d_name;
                        cant++;
                    }
                }
                //Fin directorio

            } else {  
                //Abrir archivo
                move(20,0);
                printw("Archivo: %s",res[i].nombre);
    
            }

            break; //Fin case ENTER

        default:
            // Nothing 
            break;
      }
      
      move(1,5);
      printw("Estoy en %d: Lei: %x Cant: %d",i,c,cant);
      move(2,5);
      printw("File: %s                 ",res[i].nombre);
      
   } while (c != 'q');
   
   endwin();

   //Cerrar directorio
   closedir(dir);

   return 0;
}

int leeChar() {
  int chars[5];
  int ch,i=0;
  nodelay(stdscr, TRUE);
  while((ch = getch()) == ERR); /* Espera activa */
  ungetch(ch);
  while((ch = getch()) != ERR) {
    chars[i++]=ch;
  }
  /* convierte a numero con todo lo leido */
  int res=0;
  for(int j=0;j<i;j++) {
    res <<=8;
    res |= chars[j];
  }
  return res;
}
 
char *hazLinea(char *base, int dir) {
	char linea[100]; // La linea es mas pequeÃ±a
	int o=0;
	// Muestra 16 caracteres por cada linea
	o += sprintf(linea,"%08x ",dir); // offset en hexadecimal
	for(int i=0; i < 4; i++) {
		unsigned char a,b,c,d;
		a = base[dir+4*i+0];
		b = base[dir+4*i+1];
		c = base[dir+4*i+2];
		d = base[dir+4*i+3];
		o += sprintf(&linea[o],"%02x %02x %02x %02x ", a, b, c, d);
	}
	for(int i=0; i < 16; i++) {
		if (isprint(base[dir+i])) {
			o += sprintf(&linea[o],"%c",base[dir+i]);
		}
		else {
			o += sprintf(&linea[o],".");
		}
	}
	sprintf(&linea[o],"\n");

	return(strdup(linea));
}

char *mapFile(char *filePath) {
    /* Abre archivo */
    fd = open(filePath, O_RDONLY);
    if (fd == -1) {
    	perror("Error abriendo el archivo");
	    return(NULL);
    }

    /* Mapea archivo */
    struct stat st;
    fstat(fd,&st);
    int fs = st.st_size;

    char *map = mmap(0, fs, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
    	close(fd);
	    perror("Error mapeando el archivo");
	    return(NULL);
    }

  return map;
}

//Eliminar de la cadena el directorio actual
char *upDir(char* str){

    int size = strlen(str) - 1;
    char *pch = strrchr(str, '/');
    int until = size - strlen(pch) + 1;

    for(int i = size; i >= until; i--)
        str[i] = '\0';

    return str;
}