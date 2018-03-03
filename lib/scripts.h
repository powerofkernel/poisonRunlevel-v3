int touch_script( char *script_file, char *exec, char *service) {
    char *script_str[] =  {
                            "#! /bin/sh\n" ,
                            "### BEGIN INIT INFO\n" ,
                            "# Provides:\t\t", service , "\n" ,
                            "# Required-Start:    $syslog $remote_fs $network\n" ,
                            "# Required-Stop:     $syslog $remote_fs $network\n" ,
                            "# Default-Start:     2 3 4 5\n" ,
                            "# Default-Stop:      0 1 6 \n" ,
                            "# Short-Description: Set the CPU Frequency Scaling governor to \"" , service , "\"\n" ,
                            "### END INIT INFO\n\n" ,
                            "PATH=/sbin:/usr/sbin:/bin:/usr/bin\n\n" ,
                            ". /lib/init/vars.sh\n" ,
                            ". /lib/lsb/init-functions\n" ,
                            "case \"$1\" in\n" ,
                            "    start)\n" ,
                            "    	", exec , " -D & \n" ,
                            "        ;;\n" ,
                            "    background)\n" ,
                            "	;;\n" ,
                            "    restart|reload|force-reload)\n" ,
                            "        echo \"Error: argument '$1' not supported\" >&2\n" ,
                            "        exit 3\n" ,
                            "        ;;\n" ,
                            "    stop)\n" ,
                            "       pkill ", service , "\n" ,
                            "        ;;\n" ,
                            "    *)\n" ,
                            "        echo \"Usage: $0 start|stop\" >&2\n" ,
                            "        exit 3\n" ,
                            "        ;;\n" ,
                            "esac\n\n" 
                        };

    FILE *fichero;
    fichero = fopen(script_file, "w+");
    if ( fichero != NULL ) {
        for ( int index = 0; index < sizeof(script_str)/sizeof(char*); index++ ){
            fputs(script_str[index], fichero);    
        }
    
    fclose(fichero);
        return 1;
    } else {
        return 0;
    }
}

int touch_script_rh( char *script_file, char *exec, char *service) {
    char *script_str[] =  {
                            "#!/bin/sh\n" ,
                            "#\n" ,
                            "# " , service , "\n" ,
                            "#\n" ,
                            "case \"$1\" in\n" ,
                            "  'start')\n",
                            "  ", exec , " -D &\n",
                            "  ;;\n" ,
                            "  'stop')\n" ,
                            "   pkill " , service , "\n" ,
                            "  ;;\n" ,
                            "  'restart')\n" ,
                            "  pkill ", service,"\n" ,
                            "  ", exec ," -D &\n" ,
                            "  ;;\n" ,
                            "  *)\n" ,
                            "    echo \"Usage: $0 {start|stop|restart}\"" ,
                            "  ;;\n" ,
                            "esac\n" ,
 
                        };

    FILE *fichero;
    fichero = fopen(script_file, "w+");
    if ( fichero != NULL ) {
        for ( int index = 0; index < sizeof(script_str)/sizeof(char*); index++ ){
            fputs(script_str[index], fichero);    
        }
    
    fclose(fichero);
        return 1;
    } else {
        return 0;
    }
}