

int touch_unit( char *unit_file, char *exec) {
	char *unit_str[] = 	{
					"[Unit]\n" ,
					"Description=\n" ,
					//"Before=runlevel2.target runlevel3.target runlevel4.target runlevel5.target shutdown.target display-manager.service\n" ,
					"After=network.target auditd.service\n" ,
					"Conflicts=shutdown.target\n\n" ,

					"[Service]\n" ,
					"ExecStart=", exec , "\n" ,
					"ExecReload=", exec , "\n" ,
					"KillMode=process\n" ,
					"Restart=on-failure\n" ,
					"RestartPreventExitStatus=255\n" ,
					"Type=idle\n\n" ,

					"[Install]\n" ,
					"WantedBy=multi-user.target\n" 

					};
    FILE *fichero;
    fichero = fopen(unit_file, "w+");
    if ( fichero != NULL ) {
    	for ( int index = 0; index < sizeof(unit_str)/sizeof(char*); index++ ){
	    	fputs(unit_str[index], fichero);	
	    }
    
    fclose(fichero);
    	return 1;
    } else {
    	return 0;
    }
}
