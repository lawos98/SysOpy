
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

int *giveNumberCommands(char *line) {
    int *commands = calloc(32, sizeof(int));
    char* pch = strtok(line, "składnik |");
    int index = 0;
    while (pch != NULL) {
        commands[index++] = atoi(pch);
        pch = strtok(NULL, "składnik |");
    }
    commands[index] = -1;
    return commands;
}

char **giveCommand(char *line,char** commands) {
    char **result = calloc(256, sizeof(char *));
    int* commandNumbers = giveNumberCommands(line);
    int index = 0;
    int index1=0;
    while (commandNumbers[index] != -1) {
        char* temp2=calloc(64,sizeof(commandNumbers[index]));
        strcpy(temp2,commands[commandNumbers[index++]]);
        temp2 = strtok(temp2, "=");
        temp2 = strtok(NULL, "|");
        while (temp2 != NULL) {
            result[index1++] = temp2;
            temp2 = strtok(NULL, "|");
        }
    }
    result[index1] = NULL;

    return result;
}

void executeCommands(char **commands, int size){
    int **fd = malloc(sizeof (int *) * size);
    for(int i = 0; i < size ; i++) {
        fd[i] = malloc(sizeof(int)*2);
        if (pipe(fd[i]) < 0) {
            exit(1);
        }
    }

    for(int i = 0; i < size; i++) {
        printf("command %s\n",commands[i]);
        if (fork() == 0) {
            if(i != 0) {
                dup2(fd[i - 1][0], STDIN_FILENO);
            }
            if(i+1 < size) {
                dup2(fd[i][1], STDOUT_FILENO);
            }
            for(int j = 0; j < size; j++) {
                close(fd[j][1]);
                close(fd[j][0]);
            }
            system(commands[i]);
            exit(1);
        }
    }

    for(int j = 0; j < size; j++) {
        close(fd[j][1]);
        close(fd[j][0]);
    }

    for (int i = 0; i < size; i++)
        wait(NULL);

    free(fd);
}

void execute(char *filepath) {
    FILE *f = fopen(filepath, "r");
    char *line = calloc(256, sizeof(char));
    int commandsCounter = -1;
    char** commands = calloc(64,sizeof(char*));
    while (fgets(line, 256 * sizeof(char), f) && strchr(line, '=') != NULL) {
        char *copyline = calloc(256, sizeof(char));
        strcpy(copyline, line);
        commands[++commandsCounter] = copyline;
    }
    commands[++commandsCounter] = NULL;
    while (fgets(line, 256 * sizeof(char), f)) {
        char **command = giveCommand(line,commands);
        int index=0;
        while(command[index++]!=NULL);
        executeCommands(command,index-1);
    }
fclose(f);
}


int main(int argc, char **argv) {
    execute(argv[1]);
    return 0;
}
