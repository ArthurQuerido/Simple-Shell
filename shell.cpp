#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>


#define maxLetters 1000
#define maxCommands 10

#define clear() printf("\033[H\033[J")

int takeInput(char* commandLine)
{
    char* buff;

    buff = readline("\n ~$ ");
    if(strlen(buff) !=0)
    {
        add_history(buff);
        strcpy(commandLine, buff);
        return 0;
    }
    else return 1;
}


void printCwd()
{
    char buffer[2048];
    getcwd(buffer, 2048);
    printf("Current Directory: %s", buffer);
}

void parseWords(char* commandLine, char** parsed)
{
    int i;

    for(i=0 ; i<maxCommands ; i++)
    {
        parsed[i] = strsep(&commandLine, " ");

        if(parsed[i] == NULL)
            break;

        if(strlen(parsed[i]) == 0)
            i--;
    }
}

void execCommand(char** parsed)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == -1)
    {
        printf("Forking just failed!");
        return;
    }

    else if(pid == 0)
    {
        if(execvp(parsed[0], parsed) <0) printf("Could not execute your command!");
        exit(0);
    }

    else
    {
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return;
    }
}

int changeDir (char** parsed)
{

    if(strcmp(parsed[0], "cd") == 0)
    {
        chdir(parsed[1]);
        return 1;
    }
    return 0;
}

int processStr(char* commandLine, char** parsed)
{
    parseWords(commandLine, parsed);

    if(changeDir(parsed))
        return 0;
    else
        return 1;
}

int main()
{
    char input[maxLetters], *parsedArgs[maxCommands];
    int execFlag;

    while(1)
    {
        printCwd();

        if(takeInput(input)) continue;

        execFlag = processStr(input, parsedArgs);

        if(execFlag == 1)
            execCommand(parsedArgs);

    }

    return 0;
}












