#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
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

void pwd(){

    char buff[PATH_MAX];
    if(getcwd(buff, sizeof(buff)) != NULL)
        printf("%s\n", buff);
    else {
        perror("Eroare executand pwd.");
        
    }
    

}

enum {max_size = 256};

int main(){

    /*Informatii despre user*/
    char com[max_size] = "",
        *p = getenv("USER"),
        *host = getenv("HOSTNAME"),
        *s = NULL;
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
        else if (strcmp(s, "pwd") == 0)
            pwd();
        add_history(s);
        free(s);
        nr++;
        sprintf(com, "%d %s@%s> ", nr, p, host);
    }

    

    

    return 0;
}