#include "closest_parallel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
 #include<sys/wait.h>

int curr_depth = 0;

double _closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
   
    static int num_forks = 0;
     *pcount = 0;
    if (n < 4 || pdmax == 0){
         return _closest_serial(P, n);
         exit(0);
    }
    pdmax -= 1;
    int leftsize = floor(n / 2);
    int rightsize = n - leftsize;
    int fd[2][2];
    if (pipe(fd[0])== -1 ) {
        perror("pipe");
        exit(1);
    }
    if ( pipe(fd[1])== -1 ) {
        perror("pipe");
        exit(1);
    }
    pid_t child_a, child_b;
    child_a = fork();
    if (child_a < 0){
        perror("fork");
        exit(1);
    }
    if (child_a == 0) {
        if (close(fd[0][0]) == -1){
            perror("close");
            exit(1);
        }  
        double leftval =  _closest_parallel(P, leftsize, pdmax, pcount);
        if ( write(fd[0][1], &leftval, sizeof(double)) == -1){
            perror("write");
            exit(1);
        }
        if  ( close(fd[0][1]) == -1 ){
            perror("close");
            exit(1);
        }  
        exit(num_forks);
    } else {
        child_b = fork();
        if (child_b < 0){
            perror("fork");
            exit(1);
        }
        if (child_b == 0) {
            if ( close(fd[1][0]) == -1 ){
                perror("close");
                exit(1);
            }
            double rightval =  _closest_parallel(P+leftsize, rightsize, pdmax, pcount);
            if (write(fd[1][1], &rightval, sizeof(double)) == -1){
                perror("write");
                exit(1);
            }
            if ( close(fd[1][1]) == -1 ){
                perror("close");
                exit(1);
            }
            exit(num_forks);
        } else {
            if ( close(fd[0][1]) == -1 ){
                perror("close");
                exit(1);
            } 
            if (close(fd[1][1]) == -1 ){
                perror("close");
                exit(1);
            }
            int status1, status2;
            if (wait(&status1) == -1){
                perror("wait");
                exit(1);
            }
            if (WIFEXITED(status1)){
                if (WEXITSTATUS(status1) == -1){
                    exit(1);
                }else{
                    num_forks += WEXITSTATUS(status1) + 1;
                }
            }
            
            if (wait(&status2) == -1){
                perror("wait");
                exit(1);
            }
            if (WIFEXITED(status2)){
                if (WEXITSTATUS(status2) == -1){
                    exit(1);
                }else{
                    num_forks += WEXITSTATUS(status2) + 1;
                }
            }else{
                exit(1);
            }
            *pcount = num_forks;
             double left_min, right_min;
             if ( read(fd[0][0], &left_min, sizeof(double)) == -1 ){
                 perror("read");
                 exit(1);
             }
             if (read(fd[1][0], &right_min, sizeof(double)) == -1){
                 perror("read");
                 exit(1);
             }
            if (close(fd[0][0]) == -1){
                perror("close");
                exit(1);
            }  
            if (close(fd[1][0]) == -1){
                perror("close");
                exit(1);
            }
             double min = left_min < right_min ? left_min : right_min;
             struct Point mid_point = P[leftsize];
             return combine_lr(P, n, mid_point, min); 
             exit(0);
        }
    }
}



    


double closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    qsort(P, n, sizeof(struct Point), compare_x);
    return _closest_parallel(P, n, pdmax, pcount);
   
}
