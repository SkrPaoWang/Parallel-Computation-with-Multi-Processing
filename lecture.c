#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

    int pipes[argc][2];

    for (int i = 1; i < argc; i++) { 
        if ( pipe(pipes[i]) != 0 ){
            perror("pipe");
            exit(1);
        }
    

        int result = fork();

        if (result < 0){
            perror("fork");
            exit(1);
        }
    
        else if (result == 0){
            for (int j = 1; j <= i; j++) { 
                close(pipes[j][0]);
            }

        int len = strlen(argv[i]);
        // exit(0);

        // sleep(5);
        write(pipes[i][1], &len, sizeof(int));
        
        }else{
            close(pipes[i][1]);
        }
    }

    int read_int;
    int sum = 0;

    for(int i = 1; i < argc; i++){
        read(pipes[i][0], &read_int, sizeof(int));
        printf("read from child: %d\n", read_int);
        sum += read_int;
    }

    printf("total: %d\n", sum);
    return 0;


}