
#define BOOL int
#define TRUE 1
#define FALSE 0

int is_link(const char *file) {
	struct stat info_file;
	int estado;

	estado = lstat (file, &info_file);
    if (S_ISLNK(info_file.st_mode)) {
    	return TRUE;
    } else {
    	return FALSE;
    }

}

int is_dir(const char *path) {
    struct stat s;
    if (lstat(path, &s) == -1) {
        return FALSE;
    }
    return TRUE;
}

int is_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    if ( S_ISREG(path_stat.st_mode) ) {
    	return TRUE;
    } else {
    	return FALSE;
    }
}

char *scat(char *s,char *t) {
    char *p=malloc(strlen(s)+strlen(t)+1);
    int ptr =0, temp = 0;

    while(s[temp]!='\0'){
        p[ptr++] = s[temp++];
    }
    temp=0;
    while(t[temp]!='\0'){
        p[ptr++]=t[temp++];
    }
    p[ptr] = '\0';
    return p;
}

int move ( char *origen, char *destino) {
    char cTemp;
    FILE *ori_file = fopen(origen, "rb");
    FILE *dst_file = fopen(destino, "wb");

    while(fread(&cTemp, 1, 1, ori_file) == 1) {
        fwrite(&cTemp, 1, 1, dst_file);
    }

    fclose(ori_file);
    fclose(dst_file);

    return 1;
}

char *int2string(int x) {
    char *buffer = malloc(sizeof(char) * sizeof(int) * 4 + 1);
    if (buffer) {
         sprintf(buffer, "%d", x);
    }

    return buffer;
}