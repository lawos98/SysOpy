#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void get_mails(char* mode) {
    printf("Sort by: %s\n", mode);

    FILE* f;
    if (!strcmp(mode,"data")) {
        f=popen("echo | mail -H", "w");
    }
    if (!strcmp(mode, "nadawca")) {
        f = popen("echo | mail -H | sort -k 3", "w");
    }
    if (f == NULL){
        printf("popen error\n.");
        exit(1);
    }
    char line[128];
    while((fgets(line, 128, f)) != NULL) {
        printf("%s\n", line);
    }
}

void send_mail(char* address, char* topic, char* content) {
    char *command = calloc(256,sizeof (char));

    sprintf(command, "echo %s | mail %s -s %s", content, address, topic);
    printf("%s\n", command);

    FILE* f = popen(command, "r");
    if (f == NULL){
        printf("popen error\n.");
        exit(1);
    }
    free(command);
}

int main(int argc, char **argv) {
    if (argc==2 && strcmp(argv[1],"data")==0 && strcmp(argv[1],"nadawca")==0){
        get_mails(argv[1]);
    } else if(argc==4){
        send_mail(argv[1], argv[2], argv[3]);
    }
    return 0;
}