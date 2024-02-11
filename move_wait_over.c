#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include "readLine.h"
#include "board.h"
#include "sendingMoveFromPipe.h"
#define BUFFER 256


void move_wait_over (int socket_fd,FILE* readFile,struct Piece* pieces,int pipe_fds[0]){

    char spielstand[8][8]= {
        {'*', 'b', '*', 'b', '*', 'b', '*', 'b'},
        {'b', '*', 'b', '*', 'b', '*', 'b', '*'},
        {'*', 'b', '*', 'b', '*', 'b', '*', 'b'},
        {'*', '*', '*', '*', '*', '*', '*', '*'},
        {'*', '*', '*', '*', '*', '*', '*', '*'},
        {'w', '*', 'w', '*', 'w', '*', 'w', '*'},
        {'*', 'w', '*', 'w', '*', 'w', '*', 'w'},
        {'w', '*', 'w', 'w', 'w', '*', 'w', '*'},
        }; 
    int k = 4;

while(1){

    char* buffer_mwo = (char*)malloc(BUFFER* sizeof(char));   
   
    
    readLine(buffer_mwo,readFile);
    
    printf("%s\n",buffer_mwo);

    if(strcmp(buffer_mwo,"+ WAIT\n")== 0){
        
        char okwait[] = "THINKING\n";
        ssize_t sent_byte = send(socket_fd, okwait, strlen(okwait), 0);
        if (sent_byte == -1)
        {
        perror("Error sending THINKING.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
        }
        else
        {
        printf("%zd bytes sent(THINKING).\n", sent_byte);
        }
        usleep(500000);

        continue;

    }else if (strcmp(buffer_mwo,"+ GAMEOVER\n")== 0){

        usleep(50000);
        char* gameover_buffer = (char*)malloc(BUFFER * sizeof(char));               
        ssize_t gameover_received = recv(socket_fd, gameover_buffer, 256 - 1, 0);  
        if (gameover_received > 0) gameover_buffer[gameover_received] = '\0';           
        printf("%s\n", gameover_buffer);     
    
        free(gameover_buffer);

        break;

    }else if((strstr(buffer_mwo,"+ MOVE") != NULL)){
        printf("in move branch\n");
       
        char* buffer_steps = (char*)malloc(256 * sizeof(char));  
        
        readLine(buffer_steps,readFile);
        printf("%s\n",buffer_steps);
        free(buffer_steps);

        char* buffer_board = (char*)malloc(256 * sizeof(char));  
        
        free(buffer_board);

       

         usleep(50000);
        
       


        for (int i = 0; i < 8; i++)
        {
            char* buffer_spielstand = (char*)malloc(256 * sizeof(char));  
           
            readLine(buffer_spielstand,readFile);
            

            for (int j = 0; j < 8; j++)
            {
                
                spielstand[i][j] = buffer_spielstand[k];
                k = k + 2;
                printf("%c ",spielstand[i][j]); 
            } 
            k = 4;
            
            printf("\n");
            free(buffer_spielstand); 
        }


        

        char buffer_end[32];
        
        readLine(buffer_end,readFile);
        printf("%s.\n",buffer_end);

        char thinking[] = "THINKING\n";
        ssize_t sent_byte = send(socket_fd, thinking, strlen(thinking), 0);
        if (sent_byte == -1)
        {
        perror("Error sending THINKING.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
        }
        else
        {
        printf("%zd bytes sent(THINKING).\n", sent_byte);
        }
        usleep(500000);

         // 存储棋子的数组
       

 
        
        char* buffer_okthink = (char*)malloc(256 * sizeof(char));                                   
        readLine(buffer_okthink,readFile);
        printf("after send thinking, recive: %s.\n",buffer_okthink);
        free(buffer_okthink);

        // call save to SHM method;
        CatchPieces(pieces, spielstand); // 填充 pieces 数组
        PrintSavedSD(spielstand); // 打印棋盘
        printPieces(pieces, 24); // 检查棋子信息

        // 发送信号给 Thinker 进程
        printf("start send SIGUSR1 signal to parent.\n");
        kill(getppid(), SIGUSR1);
        printf("SIGUSR1 signal sent to parent.\n");
        printf("Start use sending Move From Pipe\n");
        //SendingMoveFromPipe(pipe_fds[0],socket_fd,readFile);
        printf("Pipe finished\n");


        printf("end of move\n");          
        continue;

    }else{
        perror("not MOVE, WAIT or GAMEOVER\n");
        
        exit(EXIT_FAILURE);
    }

    free(buffer_mwo);
    


};
}

