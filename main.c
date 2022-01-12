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

#define BLU   "\x1B[34m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"


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

void help(){
    printf("BOSS Shell v.0.0.1 Pre-Alpha\n");
    printf("Provided by Trandafir Dragos, Lascu Matei, Guleama Dan\n");
    printf("==================================\n");
    printf("istoric : afiseaza istoricul comenzilor;\ncd : schimba directorul de lucru curent.\nmkdir : creeaza un director nou;\nrmdir : sterge un director;\nls : afiseaza continutul folderului curent;\necho : afiseaza in shell;\npwd : afiseaza directory-ul curent\n\n");
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

void rdir(){
	
	char* dir = malloc(2560);
	printf("Numele directorului de sters: ");
	scanf("%s", dir);
	if(!rmdir(dir))
		printf("Directorul %s a fost sters. \n", dir);
	else
		perror("Director inexistent. \n");
}

void echo(char* cuv){
    printf("%s\n",cuv);
    
}

void change_directory(char* path) {

    char *home_dir = "/home";

    
    if (strcmp(path, "") == 0 || (!(strcmp(path, "~") && strcmp(path, "~/"))))
        chdir(home_dir);
    else if(strcmp(path, "..") == 0)
        chdir(path);
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
    //int nr = 1;
    //printf("%s@%s> ", p, host);
    pwd(output);
    sprintf(com, RED "%s@%s" RESET ":" BLU " %s> " RESET, p, host, output);
    using_history();
    
    /*Inputul de comenzi*/
    while ((s = readline(com))){
        //printf("%s@%s> ", p, host);
        if(strcmp(s, "exit") == 0){
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
        else if (strcmp(s, "rmdir") == 0)
            rdir();
        else if(strncmp(s, "echo", 4) == 0)
            echo(s+5);
        else if(strcmp(s, "help") == 0)
            help();
        else
            printf("Nu se recunoaste comanda.\n");
        add_history(s);
        free(s);
        
        pwd(output);
        sprintf(com, RED "%s@%s" RESET ":" BLU " %s> " RESET, p, host, output);
    }

    

    

    return 0;
}

/*
    Aici am incercat ceva cu operatori logici, dar nu ne-a iesit.
    =============================================================

    char** parse_args(char* s){
    char** arg_list = calloc(100, sizeof(char*));
    int i = 0;
    char* arg;
    arg = strtok(s, " ");
    while (arg != NULL){
        arg_list[i] = arg;
        arg = strtok (NULL, " ");
        i++;
    }
        
    return arg_list;
}



int execute_function (char* arg){

    char *output = malloc(4096);
    if(strcmp(arg, "exit") == 0){
            free (arg);
            
        }
        else if (strcmp(arg, "istoric") == 0){
            retrieve_history();
        }
        else if (strcmp(arg, "pwd") == 0){
            pwd(output);
            printf("%s\n", output);
        }
            
        else if (strncmp(arg, "cd", 2) == 0){
            change_directory(arg+3);
        }
        else if (strcmp(arg, "ls") == 0)
            ls(output);
        else if (strcmp(arg, "mkdir") == 0)
            mdir();
        else if (strcmp(arg, "clear") == 0)
            clear();
        else{
            printf("Nu se recunoaste comanda.\n");
            return 0;
        }
    
    return 1;
            
}

while (s = readline(com)){

        
        char** arg_list = parse_args(s);
        int length = sizeof(arg_list)/sizeof(arg_list[0]);
        
        if(length > 1)
            for(int i = 0; i < length; i += 2)
            if(execute_function(arg_list[i])){

                if(strcmp(arg_list[i+1],"&&"))
                    continue;
                else if (strcmp(arg_list[i+1],"||"))
                    break;
                
            } else {
                if(strcmp(arg_list[i+1],"&&"))
                    break;
                else if(strcmp(arg_list[i+1],"||"))
                    continue;
            }
        else
            execute_function(arg_list[0]);
        free(arg_list);

        ---
        restul codului

}

*/