#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

void retrieve_history(){
    HISTORY_STATE *com_hist = history_get_history_state();
    HIST_ENTRY **com_list = history_list();

    printf("\n Istoricul de comenzi: \n");
    for(int i = 0; i < com_hist->length; i++){
        printf(" %8s  %s\n", com_list[i]->line, com_list[i]->timestamp);
        //free_history_entry(com_list[i]);
    }

    printf("\n");

    //free (com_hist);
    //free (com_list);
}

void pwd(char* output){

    char buff[PATH_MAX];
    if(getcwd(buff, sizeof(buff)) != NULL){
        strcpy(output, buff);
    }
        //printf("%s\n",buff);
    else {
        perror("Eroare executand pwd");
        
    }
}

void mdir(){
	
	char* dir = malloc(2560);
	printf("Numele directorului de creat: ");
	scanf("%s", dir);
	if(mkdir(dir, 0777) == 0)
		printf("Director creat\n");
	else{
		perror("Directorul nu a fost creat\n");
		
	}

    free(dir);
}

void change_directory(char* path) {

    char *home_dir = "/home";

    if (strcmp(path, "") == 0 || (!(strcmp(path, "~") && strcmp(path, "~/"))))
        chdir(home_dir);
    else if (chdir(path) < 0)
        perror("///No such file or directory: ");
    else
        chdir(path);
    
 
}

void ls(char* output){
	char* dir;
	struct dirent *d;
    pwd(output);
	dir = output;
	DIR *myd = opendir(dir);
	if(!myd){
	     perror ("Directorul nu exista");
	}
	while(d = readdir(myd)){
	  	printf("%s\n ", d -> d_name);
	 }
}

void clear(){
    system("clear");
}

enum {max_size = 256};

int main(){

    /*Informatii despre user*/
    char com[max_size] = "",
        *p = getenv("USER"),
        *host = getenv("HOSTNAME"),
        *s = NULL,
        *output = malloc(4096);
    int nr = 1;
    //printf("%s@%s> ", p, host);
    sprintf(com, "%d %s@%s> ", nr, p, host);
    using_history();
    
    /*Inputul de comenzi*/
    while ((s = readline(com))){
        //printf("%s@%s> ", p, host);
        if(strcmp(s, "") == 0){
            free (s);
            break;
        }
        else if (strcmp(s, "istoric") == 0){
            retrieve_history();
        }
        else if (strcmp(s, "pwd") == 0){
            pwd(output);
            printf("%s\n", output);
        }
            
        else if (strncmp(s, "cd", 2) == 0){
            change_directory(s+3);
        }
        else if (strcmp(s, "ls") == 0)
            ls(output);
        else if (strcmp(s, "mkdir") == 0)
            mdir();
        else if (strcmp(s, "clear") == 0)
            clear();
        else
            printf("Nu se recunoaste comanda.\n");
        add_history(s);
        free(s);
        nr++;
        sprintf(com, "%d %s@%s> ", nr, p, host);
    }

    

    

    return 0;
}