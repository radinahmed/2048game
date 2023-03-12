        /* UART0 is on USB/Debug */
        /* Use TeraTerm to see the message "YES" on a PC */
        #include <stdint.h>
        #include <string.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <stdbool.h>
        #include "tm4c123gh6pm.h"
        #define atoi(x) #x
        void timer_init(void);
        void Timer_Handler(void);
        void UART0Tx(char c);
        void UART0_Handler(void);
        char UART0Rx(void);
        void delayMs(int n);
        void outputstring(char *c1);
        void inttostring(int i3);
        int count;
        int grid1;
        int score;
        int grid2;
        int u;
        int u3;
        int direc=3;
        int gamestarted=0;
        int nomoves=0;
        int initial = 0;
        char pressanykey[]= "\033c\033[48;5;008m\033[38;5;7mPress any key to start\n\r"
                            "Game controls: W,A,S,D\n\r"
                            "Combine Numbers to 2048";
        char scorestring[]="";
        bool boolArray[4][4] = {{false , false , false , false} , {false, false , false , false} , {false , false, false , false} , {false , false , false, false}};
        int gamearray[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
        char scoreboard[]= "\033c\033[48;5;008m\033[38;5;7m\r                   Score:        \n\r";
        char gameboard[] ="\033[38;5;017m\033[1;46m\r|-------|-------|-------|-------|\n\r";
        char gameboard2[] ="\033[1;46m|       |       |       |       |\n\r\033[1;46m|       |       |       |       |\n\r|       |       |       |       |\n\r|       |       |       |       |\n\r|-------|-------|-------|-------|\n\r";
        char gameboard3[] ="\033[1;46m|       |       |       |       |\n\r\033[1;46m|       |       |       |       |\n\r|       |       |       |       |\n\r|       |       |       |       |\n\r|-------|-------|-------|-------|\n\r";
        char gameboard4[] ="\033[1;46m|       |       |       |       |\n\r\033[1;46m|       |       |       |       |\n\r|       |       |       |       |\n\r|       |       |       |       |\n\r|-------|-------|-------|-------|\n\r";
        char gameboard5[] ="\033[1;46m|       |       |       |       |\n\r\033[1;46m|       |       |       |       |\n\r|       |       |       |       |\n\r|       |       |       |       |\n\r|-------|-------|-------|-------|\n\r";
        char numlst[] = {'2','4','8','16','32','64','128','256','512','1024','2048'};
        int main(void)
        {

            int i;
            SYSCTL_RCGCUART_R |= 1;
            SYSCTL_RCGCGPIO_R |= 1;
            SYSCTL_RCGCGPIO_R |= 0x20;
            UART0_CTL_R = 0;
            UART0_IBRD_R = 8;
            UART0_FBRD_R = 54;
            UART0_CC_R  = 0; /* use system clock */
            UART0_LCRH_R = 0x60; /* 8-bit, no parity, 1-stop bit, no FIFO */
            UART0_CTL_R  = 0x301; /* enable UART0, TXE, RXE */
            GPIO_PORTA_DEN_R = 0x03; /* Make PA0 and PA1 as digital */
            GPIO_PORTA_AFSEL_R = 0x03; /* Use PA0,PA1 alternate function */
            GPIO_PORTA_PCTL_R = 0x11; /* configure PA0 and PA1 for UART */
            UART0_IM_R=0x10;
            UART0_ICR_R &= ~(0x010);
            NVIC_PRI30_R = 3<<5;
            NVIC_EN0_R |= 0x20;

            NVIC_ST_RELOAD_R = 8000000 -1;
            NVIC_ST_CTRL_R =5;
            timer_init();
            delayMs(1); /* wait for output line to stabilize */

            if(gamestarted){
                if(initial==0){
                clearblock();
                generateblock();
                generatescore();
                outputstring(scoreboard);
                outputstring(gameboard);
                outputstring(gameboard2);
                outputstring(gameboard3);
                outputstring(gameboard4);
                outputstring(gameboard5);
                int i,j;
                for(i = 0; i < 4; i++){
                    for(j = 0; j < 4; j++){
                        int x=(gamearray[i][j]);
                        inttostring(x);

                    }
                }

            }


            }
            else{
                outputstring(pressanykey);
            }





        }  /* UART0 Transmit */
        /* This function waits until the transmit buffer is available then */
        /* writes the character in the transmit buffer. It does not wait */
        /* for transmission to complete. */
        void timer_init(void){
            SYSCTL_RCGCTIMER_R = SYSCTL_RCGCTIMER_R | 1;
            TIMER0_CTL_R = 0;
            TIMER0_CFG_R = 0;
            TIMER0_TAMR_R =0;
            TIMER0_TAMR_R = TIMER0_TAMR_R | 2;
            TIMER0_TAPR_R = 250;
            TIMER0_TAILR_R = 64000;
            TIMER0_IMR_R = TIMER0_IMR_R | 1;
            NVIC_EN0_R = 524288;
            TIMER0_CTL_R = TIMER0_CTL_R |1;

        }
        void outputstring(char *c1)  {
            int j;
            for (j=0; j<strlen(c1); j++){
                UART0Tx(c1[j]);
            }

        }
        void inttostring(int i3){
            if(i3==0){
               UART0Tx('0');

            }
            else{
                int reverse=0;
                int remainder = 0;
                int divisiblebyten =0;
                if(i3%10==0){
                    divisiblebyten=1;
                }
                while(i3 != 0){
                   remainder = i3 % 10;
                   reverse = reverse * 10 + remainder;
                   i3 /= 10;
                }
                int divider= 10;
                while(reverse !=0){
                    int remainder = reverse % 10;
                    reverse /=10;
                    UART0Tx(remainder+48);
                    divider = divider*10;
                    if (reverse == 0){
                        break;
                    }
                }
                if(divisiblebyten==1){
                    UART0Tx('0');
                }

            }

            return;
        }

        void inttostring2(int i3, int b){
            if(i3==0){
               scoreboard[b]='0';
               b++;

            }
            else{
                int reverse=0;
                int remainder = 0;
                int divisiblebyten =0;
                if(i3%10==0){
                    divisiblebyten=1;
                }
                while(i3 != 0){
                   remainder = i3 % 10;
                   reverse = reverse * 10 + remainder;
                   i3 /= 10;
                }
                int divider= 10;
                while(reverse !=0){
                    int remainder = reverse % 10;
                    reverse /=10;
                    scoreboard[b]=remainder+48;
                    b++;
                    divider = divider*10;
                    if (reverse == 0){
                        break;
                    }
                }
                if(divisiblebyten==1){
                    scoreboard[b]='0';
                    b++;
                }

            }

            return;
        }
        void UART0Tx(char c)  {
            while((UART0_FR_R & 0x20) != 0);
            /* wait until Tx buffer not full */
            UART0_DR_R = c; /* before giving it another byte */
        }
        char UART0Rx(void)  {
            char c;
            while((UART0_FR_R & 0x10) != 0);
            /* wait until Tx buffer not full */
            c = UART0_DR_R ; /* before giving it another byte */
            return c;
        }
        void Timer_Handler(void)
        {
            TIMER0_ICR_R = TIMER0_ICR_R | 1;
            count = count + 1;
            count = count % 100;
            if(initial==0){
                if(count>16){
                    count = count-16;
                }
                else{
                    count= count;
                }
                if(count>7){
                    grid1= 2;
                    grid2= 4;
                }
                else{
                    grid1 =2;
                    grid2=2;
                }
                if(count>1){
                    u= count/2;
                }
                else{
                    u= u +5;
                }

            }
            else{
                if(count>16){
                    count = count-16;
                }
                else{
                    count= count;
                }
                if(count>7){
                    grid1= 2;
                    grid2= 0;
                }
                else{
                    grid1 =2;
                    grid2=0;
                }
                if(count>1){
                    u= count/2;
                }
                else{
                    u= u +5;
                }

            }











        }

        /* Append delay functions and SystemInit() here */
        void UART0_Handler(void)
        {

            UART0_ICR_R =  UART0_ICR_R | 0x10;
            char c=UART0Rx();
            UART0Tx(c);

            inttostring(count);
            int i, j;
            gamestarted=1;
            int nomoves=0;

            if(initial==1){
                int arcu=0;
                int y=0;
                int l=0;
                for(y=0; y<4; y++){
                    for(l=0;l<4;l++){
                        if(gamearray[y][l]!=0){
                            arcu++;

                        }
                    }
                }
                if(arcu==16){
                    int l=0;
                    int e=0;
                    for(l=0; l<3; l++){
                        for(e=0; e<4; e++){
                            if(e==3){
                                if(gamearray[l][e]!=gamearray[l+1][e]){
                                    nomoves++;

                                }
                                else{
                                    nomoves=nomoves;
                                }
                            }
                            else{
                                if((gamearray[l][e]!=gamearray[l][e+1]) &&((gamearray[l][e])!=gamearray[l+1][e])){
                                    nomoves++;
                                }
                                else{
                                    nomoves=nomoves;
                                }
                            }

                        }
                    }
                    l=3;
                    e=0;
                    for(e=0; e<4; e++){
                        if(e==3){
                            gamearray[1][1]=gamearray[1][1];
                        }
                        else if((gamearray[l][e]!=gamearray[l][e+1])&&((gamearray[l][e])!=gamearray[l+1][e])){
                            nomoves++;
                        }
                        else{
                            nomoves=nomoves;
                        }
                    }




                }

                if(c=='d'&&nomoves!=16){
                    int p,l,p1;
                    for(p1=0; p1<4; p1++){
                        for(p = 0; p < 5; p++){
                        if(gamearray[p1][3]==0|gamearray[p1][2]==0|gamearray[p1][3]==0){
                          if(gamearray[p1][3]==0){
                              gamearray[p1][3]=gamearray[p1][2];
                              gamearray[p1][2]=gamearray[p1][1];
                              gamearray[p1][1]=gamearray[p1][0];
                              gamearray[p1][0]=0;

                          }
                          else if(gamearray[p1][2]==0){
                              gamearray[p1][2]=gamearray[p1][1];
                              gamearray[p1][1]=gamearray[p1][0];
                              gamearray[p1][0]=0;


                          }
                          else if(gamearray[p1][1]==0){
                              gamearray[p1][1]=gamearray[p1][0];
                              gamearray[p1][0]=0;

                          }

                      }

                      }

                    }

                    for(l=0;l<4;l++){
                        if(gamearray[l][3]!=0){
                        if(gamearray[l][2]!=0){
                            if(gamearray[l][3]==gamearray[l][2]){
                                gamearray[l][3]=gamearray[l][3]*2;
                                gamearray[l][2]=gamearray[l][1];
                                gamearray[l][1]=gamearray[l][0];
                                gamearray[l][0]=0;
                            }
                            else if(gamearray[l][2]==gamearray[l][1]){
                                gamearray[l][2]=gamearray[l][2]*2;
                                gamearray[l][1]=gamearray[l][0];
                                gamearray[l][0]=0;
                            }
                            else if(gamearray[l][1]==gamearray[l][0]){
                                gamearray[l][1]=gamearray[l][1]*2;
                                gamearray[l][0]=0;
                            }
                        }
                    }
                    else if(gamearray[l][2]!=0){
                        if(gamearray[l][1]!=0){
                            if(gamearray[l][2]==gamearray[l][1]){
                                gamearray[l][2]=gamearray[l][2]*2;
                                gamearray[l][2]=gamearray[l][1];
                                gamearray[l][1]=gamearray[l][0];
                                gamearray[l][0]=0;
                            }
                            else if(gamearray[l][2]==gamearray[l][1]){
                                gamearray[l][2]=gamearray[l][2]*2;
                                gamearray[l][1]=gamearray[l][0];
                                gamearray[l][0]=0;
                            }
                            else if(gamearray[l][1]==gamearray[l][0]){
                                gamearray[l][1]=gamearray[l][1]*2;
                                gamearray[l][0]=0;
                            }
                        }

                    }
                    else{
                        gamearray[1][1]=gamearray[1][1];
                    }

                    }
                    islastmove();
                    if(u3==16){
                        gamearray[1][1]=gamearray[1][1];
                        u3=0;
                    }
                    else{
                        u3=0;
                        while(randomdisplay2(c,direc)!=0){
                            if(randomdisplay2(c,direc)==3){
                                count++;
                            }
                            else{
                                count--;
                            }
                        }
                        direc=3;
                        inttostring(count);

                        randomdisplay3();
                    }






                }
                if(c=='a'&&nomoves!=16){

                    int p,l,p1;
                    for(p1=0; p1<4; p1++){
                        for(p = 0; p < 5; p++){
                        if(gamearray[p1][0]==0|gamearray[p1][1]==0|gamearray[p1][2]==0){
                          if(gamearray[p1][0]==0){
                              gamearray[p1][0]=gamearray[p1][1];
                              gamearray[p1][1]=gamearray[p1][2];
                              gamearray[p1][2]=gamearray[p1][3];
                              gamearray[p1][3]=0;

                          }
                          else if(gamearray[p1][1]==0){
                              gamearray[p1][1]=gamearray[p1][2];
                              gamearray[p1][2]=gamearray[p1][3];
                              gamearray[p1][3]=0;


                          }
                          else if(gamearray[p1][2]==0){
                              gamearray[p1][2]=gamearray[p1][3];
                              gamearray[p1][3]=0;

                          }

                      }

                      }

                    }

                    for(l=0;l<4;l++){
                        if(gamearray[l][0]!=0){
                        if(gamearray[l][1]!=0){
                            if(gamearray[l][0]==gamearray[l][1]){
                                gamearray[l][0]=gamearray[l][0]*2;
                                gamearray[l][1]=gamearray[l][2];
                                gamearray[l][2]=gamearray[l][3];
                                gamearray[l][3]=0;
                            }
                            else if(gamearray[l][1]==gamearray[l][2]){
                                gamearray[l][1]=gamearray[l][1]*2;
                                gamearray[l][2]=gamearray[l][3];
                                gamearray[l][3]=0;
                            }
                            else if(gamearray[l][2]==gamearray[l][3]){
                                gamearray[l][2]=gamearray[l][2]*2;
                                gamearray[l][3]=0;
                            }
                        }
                    }
                    else if(gamearray[l][1]!=0){
                        if(gamearray[l][2]!=0){
                            if(gamearray[l][1]==gamearray[l][2]){
                                gamearray[l][1]=gamearray[l][1]*2;
                                gamearray[l][1]=gamearray[l][2];
                                gamearray[l][2]=gamearray[l][3];
                                gamearray[l][3]=0;
                            }
                            else if(gamearray[l][1]==gamearray[l][2]){
                                gamearray[l][1]=gamearray[l][1]*2;
                                gamearray[l][2]=gamearray[l][3];
                                gamearray[l][3]=0;
                            }
                            else if(gamearray[l][2]==gamearray[l][3]){
                                gamearray[l][2]=gamearray[l][2]*2;
                                gamearray[l][3]=0;
                            }
                        }

                    }
                    else{
                        gamearray[1][1]=gamearray[1][1];
                    }

                    }
                    islastmove();
                    if(u3==16){
                        gamearray[1][1]=gamearray[1][1];
                        u3=0;

                    }
                    else{
                        u3=0;
                        while(randomdisplay2(c,direc)!=0){
                            if(randomdisplay2(c,direc)==3){

                                count++;
                            }
                            else{
                                count--;


                            }
                        }
                        direc=3;

                        randomdisplay3();

                    }




                }
                if(c=='w'&&nomoves!=16){
                    int u,o;

                    int p,l,p1;
                    for(p1=0; p1<4; p1++){
                        for(p = 0; p < 5; p++){
                        if(gamearray[0][p1]==0|gamearray[1][p1]==0|gamearray[2][p1]==0){

                          if(gamearray[0][p1]==0){
                              gamearray[0][p1]=gamearray[1][p1];
                              gamearray[1][p1]=gamearray[2][p1];
                              gamearray[2][p1]=gamearray[3][p1];
                              gamearray[3][p1]=0;

                          }
                          else if(gamearray[1][p1]==0){
                              gamearray[1][p1]=gamearray[2][p1];
                              gamearray[2][p1]=gamearray[3][p1];
                              gamearray[3][p1]=0;


                          }
                          else if(gamearray[2][p1]==0){
                              gamearray[2][p1]=gamearray[3][p1];
                              gamearray[3][p1]=0;

                          }

                      }

                      }

                    }

                    for(l=0;l<4;l++){
                        if(gamearray[0][l]!=0){

                        if(gamearray[1][l]!=0){
                            if(gamearray[0][l]==gamearray[1][l]){
                                gamearray[0][l]=gamearray[0][l]*2;
                                gamearray[1][l]=gamearray[2][l];
                                gamearray[2][l]=gamearray[3][l];
                                gamearray[3][l]=0;
                            }
                            else if(gamearray[1][l]==gamearray[2][l]){
                                gamearray[1][l]=gamearray[1][l]*2;
                                gamearray[2][l]=gamearray[3][l];
                                gamearray[3][l]=0;
                            }
                            else if(gamearray[2][l]==gamearray[3][l]){
                                gamearray[2][l]=gamearray[2][l]*2;
                                gamearray[3][l]=0;
                            }
                        }
                    }
                    else if(gamearray[1][l]!=0){

                        if(gamearray[2][l]!=0){
                            if(gamearray[1][l]==gamearray[2][l]){
                                gamearray[1][l]=gamearray[1][l]*2;
                                gamearray[1][l]=gamearray[2][l];
                                gamearray[2][l]=gamearray[3][l];
                                gamearray[3][l]=0;
                            }
                            else if(gamearray[1][l]==gamearray[2][l]){
                                gamearray[1][l]=gamearray[1][l]*2;
                                gamearray[2][l]=gamearray[3][l];
                                gamearray[3][l]=0;
                            }
                            else if(gamearray[2][l]==gamearray[3][l]){
                                gamearray[2][l]=gamearray[2][l]*2;
                                gamearray[3][l]=0;
                            }
                        }

                    }
                    else{

                        gamearray[1][1]=gamearray[1][1];
                    }

                    }
                    inttostring(count);
                    islastmove();

                    if(u3==16){
                        gamearray[1][1]=gamearray[1][1];
                        u3=0;

                    }
                    else{
                        u3=0;
                        inttostring(u3);
                        while(randomdisplay2(c,direc)!=0){
                            if(randomdisplay2(c,direc)==3){

                                count++;
                            }
                            else{

                                count--;
                            }
                        }
                        direc=3;
                        inttostring(count);

                        randomdisplay3();

                    }







                }
                else if(c=='s'&&nomoves!=16){

                    int p,l,p1;
                    for(p1=0; p1<4; p1++){
                        for(p = 0; p < 5; p++){
                        if(gamearray[3][p1]==0|gamearray[2][p1]==0|gamearray[1][p1]==0){
                          if(gamearray[3][p1]==0){

                              gamearray[3][p1]=gamearray[2][p1];
                              gamearray[2][p1]=gamearray[1][p1];
                              gamearray[1][p1]=gamearray[0][p1];
                              gamearray[0][p1]=0;

                          }
                          if(gamearray[2][p1]==0){

                              gamearray[2][p1]=gamearray[1][p1];
                              gamearray[1][p1]=gamearray[0][p1];
                              gamearray[0][p1]=0;


                          }
                          if(gamearray[1][p1]==0){

                              gamearray[1][p1]=gamearray[1][p1];
                              gamearray[0][p1]=0;

                          }

                      }

                      }

                    }

                    for(l=0;l<4;l++){
                        if(gamearray[3][l]!=0){
                        if(gamearray[2][l]!=0){
                            if(gamearray[3][l]==gamearray[2][l]){
                                gamearray[3][l]=gamearray[3][l]*2;
                                gamearray[2][l]=gamearray[1][l];
                                gamearray[1][l]=gamearray[0][l];
                                gamearray[0][l]=0;
                            }
                            else if(gamearray[2][l]==gamearray[1][l]){
                                gamearray[2][l]=gamearray[2][l]*2;
                                gamearray[1][l]=gamearray[0][l];
                                gamearray[0][l]=0;
                            }
                            else if(gamearray[1][l]==gamearray[0][l]){
                                gamearray[1][l]=gamearray[1][l]*2;
                                gamearray[0][l]=0;
                            }
                        }
                    }
                    else if(gamearray[2][l]!=0){
                        if(gamearray[1][l]!=0){
                            if(gamearray[2][l]==gamearray[1][l]){
                                gamearray[2][l]=gamearray[2][l]*2;
                                gamearray[2][l]=gamearray[1][l];
                                gamearray[1][l]=gamearray[0][l];
                                gamearray[0][l]=0;
                            }
                            else if(gamearray[2][l]==gamearray[1][l]){
                                gamearray[2][l]=gamearray[2][l]*2;
                                gamearray[1][l]=gamearray[0][l];
                                gamearray[0][l]=0;
                            }
                            else if(gamearray[1][l]==gamearray[0][l]){
                                gamearray[1][l]=gamearray[1][l]*2;
                                gamearray[0][l]=0;
                            }
                        }

                    }
                    else{
                        gamearray[1][1]=gamearray[1][1];
                    }


                    }
                    islastmove();
                    if(u3==16){
                        gamearray[1][1]=gamearray[1][1];
                        u3=0;

                    }
                    else{
                        u3=0;
                        while(randomdisplay2(c,direc)!=0){
                            if(randomdisplay2(c,direc)==3){
                                count++;
                            }
                            else{
                                count--;
                            }
                        }
                        direc=3;
                        inttostring(count);

                        randomdisplay3();
                    }




                }
            }
            else if(initial==0){
                randomdisplay(c);
                initial = 1;


            }

            if(nomoves==15){
                outputstring("\033c\e[44m\e[0;32m    Game over, done by Radin Ahmed");
            }
            else{
                clearblock();
                generateblock();
                generatescore();
                outputstring(scoreboard);
                outputstring(gameboard);
                outputstring(gameboard2);
                outputstring(gameboard3);
                outputstring(gameboard4);
                outputstring(gameboard5);










                score=0;

            }
















            /* int x=NVIC_ST_CURRENT_R; */

        }
        int islastmove(void){
            int i,k;
            int s;

            for(i=0; i<4; i++){
                for(k=0;k<4;k++){
                    if(gamearray[i][k]!=0){
                        s++;
                        u3++;
                    }
                }
            }
            inttostring(s);

            return s;
        }
        void clearblock(voild){
            int i,j;

            for(i = 0; i < 4; i++){
                for(j = 0; j < 4; j++){
                    gameboard2[53+(8*j)-1] = ' ';
                    gameboard2[53+(8*j)+1-1] = ' ';
                    gameboard2[53+(8*j)+2-1] = ' ';
                    gameboard2[53+(8*j)+3-1] = ' ';
                    gameboard3[53+(8*j)-1] = ' ';
                    gameboard3[53+(8*j)+1-1] = ' ';
                    gameboard3[53+(8*j)+2-1] = ' ';
                    gameboard3[53+(8*j)+3-1] = ' ';

                    gameboard4[53+(8*j)-1] = ' ';
                    gameboard4[53+(8*j)+1-1] = ' ';
                    gameboard4[53+(8*j)+2-1] = ' ';
                    gameboard4[53+(8*j)+3-1] = ' ';

                    gameboard5[53+(8*j)-1] = ' ';
                    gameboard5[53+(8*j)+1-1] = ' ';
                    gameboard5[53+(8*j)+2-1] = ' ';
                    gameboard5[53+(8*j)+3-1] = ' ';

                }
            }



        }
        void randomdisplay(char c){
            if(c=='w'|c=='a'|c=='s'|c=='d'){
            if(count>0 && count<5){
                gamearray[0][count-1]= grid1;
                if(grid1==2){
                   gameboard2[53+(8*(count-1))] = '2';
                }
                else{
                    gameboard2[53+(8*(count-1))] = '4';
                }

            }
            else if(count>4 && count <9){
                gamearray[1][count-5]= grid1;
                if(grid1==2){
                    gameboard3[53+(8*(count-5))] = '2';
                 }
                 else{
                     gameboard3[53+(8*(count-5))] = '4';
                 }
            }
            else if(count>8 && count<13){
                gamearray[2][count-9]= grid1;
                if(grid1==2){
                    gameboard4[53+(8*(count-9))] = '2';
                 }
                 else{
                     gameboard4[53+(8*(count-9))] = '4';
                 }
            }
            else {
                gamearray[3][count-13]= grid1;
                if(grid1==2){
                    gameboard5[53+(8*(count-13))] = '2';
                 }
                 else{
                     gameboard5[53+(8*(count-13))] = '4';
                 }
            }
            if(u>0 && u<5){
                gamearray[0][u-1]= grid2;
                if(grid1==2){
                    gameboard2[53+(8*(u-1))] = '2';
                 }
                 else{
                     gameboard2[53+(8*(u-1))] = '4';
                 }
            }
            else if(u>4 && u <9){
                gamearray[1][u-5]= grid2;
                if(grid2==2){
                    gameboard3[53+(8*(u-5))] = '2';
                 }
                 else{
                     gameboard3[53+(8*(u-5))] = '4';
                 }
            }
            else if(u>8 && u<13){
                gamearray[2][u-9]= grid2;
                if(grid2==2){
                    gameboard4[53+(8*(u-9))] = '2';
                 }
                 else{
                     gameboard4[53+(8*(u-9))] = '4';
                 }
            }
            else {
                gamearray[3][u-13]= grid2;
                if(grid2==2){
                    gameboard5[53+(8*(u-13))] = '2';
                 }
                 else{
                     gameboard5[53+(8*(u-13))] = '4';
                 }
            }
        }

        }
        void randomdisplay2(char c, int dir){
            if(dir==3){
                if(count>0 && count<5){
                    if((gamearray[0][count-1])!=0){
                        direc=3;
                        return 3;
                    }
                    else{
                        return 0;
                    }

                }
                else if(count>4 && count <9){
                    if((gamearray[1][count-5])!=0){
                        direc=3;
                        return 3;

                    }
                    else{
                        return 0;
                    }
                }
                else if(count>8 && count<13){
                    if((gamearray[2][count-9])!=0){
                        direc=3;
                        return 3;
                    }
                    else{
                        return 0;
                    }

                }
                else {
                    if((gamearray[3][count-13])!=0){
                        if(count==16){
                            direc=6;
                            return 6;
                        }
                        else{
                            direc=3;
                            return 3;
                        }
                    }
                    else{
                        return 0;
                    }


                }

            }
            else{

                if(count>0 && count<5){
                    if((gamearray[0][count-1])!=0){
                        if(count==0){
                            direc=3;
                            return 3;
                        }
                        else{
                            direc=6;
                            return 6;
                        }
                    }
                    else{
                        return 0;
                    }

                }
                else if(count>4 && count <9){
                    if((gamearray[1][count-5])!=0){
                        direc=6;
                        return 6;
                    }
                    else{
                        return 0;
                    }
                }
                else if(count>8 && count<13){
                    if((gamearray[2][count-9])!=0){
                        direc=6;
                        return 6;
                    }
                    else{
                        return 0;
                    }

                }
                else {
                    if((gamearray[3][count-13])!=0){
                        direc=6;
                        return 6;
                    }
                    else{
                        return 0;
                    }


                }
            }

        }

        void randomdisplay3(char c){
            if(count>0 && count<5){
                gamearray[0][count-1]= grid1;

            }
            else if(count>4 && count <9){
                gamearray[1][count-5]= grid1;
            }
            else if(count>8 && count<13){
                gamearray[2][count-9]= grid1;

            }
            else {
                gamearray[3][count-13]= grid1;
            }
        }




        void generatescore(void){
            int i, j;
            for(i=0; i<4; i++){
                for(j=0; j<4; j++){
                    score=score+gamearray[i][j];
                }
            }
            inttostring2(score,50);

        }
        void generateblock(void){
            int i,j;
            for(i = 0; i < 4; i++){
                for(j = 0; j < 4; j++){
                    int x=(gamearray[i][j]);

                    switch(x){
                        case 0:
                            if(i==0){
                                gameboard2[53+(8*j)] = ' ';


                            }
                            else if(i==1){
                                gameboard3[53+(8*j)] = ' ';


                            }
                            else if(i==2){
                                gameboard4[53+(8*j)] = ' ';


                            }
                            else if(i==3){
                                gameboard5[53+(8*j)] = ' ';


                            }
                            break;

                        case 2:
                            if(i==0){
                                gameboard2[53+(8*j)] = '\033[0;31m2';
                            }
                            else if(i==1){
                                gameboard3[53+(8*j)] = '2';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)] = '2';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)] = '2';

                            }
                            break;

                        case 4:
                            if(i==0){
                                gameboard2[53+(8*j)] = '4';
                            }
                            else if(i==1){
                                gameboard3[53+(8*j)] = '4';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)] = '4';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)] = '4';

                            }
                            break;

                        case 8:
                            if(i==0){
                                gameboard2[53+(8*j)] = '8';
                            }
                            else if(i==1){
                                gameboard3[53+(8*j)] = '8';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)] = '8';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)] = '8';

                            }
                            break;


                        case 16:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '1';
                                gameboard2[53+(8*j)+1-1] = '6';

                            }
                            else if(i==1){
                                gameboard3[53+(8*j-1)] = '1';
                                gameboard3[53+(8*j)+1-1] = '6';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '1';
                                gameboard4[53+(8*j)+1-1] = '6';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '1';
                                gameboard5[53+(8*j)+1-1] = '6';

                            }
                            break;


                        case 32:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '3';
                                gameboard2[53+(8*j)+1-1] = '2';

                            }
                            else if(i==1){
                                gameboard3[53+(8*j)-1] = '3';
                                gameboard3[53+(8*j)+1-1] = '2';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '3';
                                gameboard4[53+(8*j)+1-1] = '2';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '3';
                                gameboard5[53+(8*j)+1-1] = '2';

                            }
                            break;


                        case 64:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '6';
                                gameboard2[53+(8*j)+1-1] = '4';

                            }
                            else if(i==1){
                                gameboard3[53+(8*j)-1] = '6';
                                gameboard3[53+(8*j)+1-1] = '4';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '6';
                                gameboard4[53+(8*j)+1-1] = '4';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '6';
                                gameboard5[53+(8*j)+1-1] = '4';

                            }
                            break;

                        case 128:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '1';
                                gameboard2[53+(8*j)+1-1] = '2';
                                gameboard2[53+(8*j)+2-1] = '8';


                            }
                            else if(i==1){
                                gameboard3[53+(8*j)-1] = '1';
                                gameboard3[53+(8*j)+1-1] = '2';
                                gameboard3[53+(8*j)+2-1] = '8';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '1';
                                gameboard4[53+(8*j)+1-1] = '2';
                                gameboard4[53+(8*j)+2-1] = '8';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '1';
                                gameboard5[53+(8*j)+1-1] = '2';
                                gameboard5[53+(8*j)+2-1] = '8';

                            }
                            break;


                        case 256:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '2';
                                gameboard2[53+(8*j)+1-1] = '5';
                                gameboard2[53+(8*j)+2-1] = '6';


                            }
                            else if(i==1){
                                gameboard3[53+(8*j)-1] = '2';
                                gameboard3[53+(8*j)+1-1] = '5';
                                gameboard3[53+(8*j)+2-1] = '6';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '2';
                                gameboard4[53+(8*j)+1-1] = '5';
                                gameboard4[53+(8*j)+2-1] = '6';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '2';
                                gameboard5[53+(8*j)+1-1] = '5';
                                gameboard5[53+(8*j)+2-1] = '6';

                            }
                            break;

                        case 512:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '5';
                                gameboard2[53+(8*j)+1-1] = '1';
                                gameboard2[53+(8*j)+2-1] = '2';


                            }
                            else if(i==1){
                                gameboard3[53+(8*j)-1] = '5';
                                gameboard3[53+(8*j)+1-1] = '1';
                                gameboard3[53+(8*j)+2-1] = '2';

                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '5';
                                gameboard4[53+(8*j)+1-1] = '1';
                                gameboard4[53+(8*j)+2-1] = '2';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '5';
                                gameboard5[53+(8*j)+1-1] = '1';
                                gameboard[53+(8*j)+2-1] = '2';
                            }
                            break;

                        case 1024:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '1';
                                gameboard2[53+(8*j)+1-1] = '0';
                                gameboard2[53+(8*j)+2-1] = '2';
                                gameboard2[53+(8*j)+3-1] = '4';



                            }
                            else if(i==1){
                                gameboard3[53+(8*j)-1] = '1';
                                gameboard3[53+(8*j)+1-1] = '0';
                                gameboard3[53+(8*j)+2-1] = '2';
                                gameboard3[53+(8*j)+3-1] = '4';


                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '1';
                                gameboard4[53+(8*j)+1-1] = '0';
                                gameboard4[53+(8*j)+2-1] = '2';
                                gameboard4[53+(8*j)+3-1] = '4';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '1';
                                gameboard5[53+(8*j)+1-1] = '0';
                                gameboard5[53+(8*j)+2-1] = '2';
                                gameboard5[53+(8*j)+3-1] = '4';
                            }
                            break;

                        case 2048:
                            if(i==0){
                                gameboard2[53+(8*j)-1] = '2';
                                gameboard2[53+(8*j)+1-1] = '0';
                                gameboard2[53+(8*j)+2-1] = '4';
                                gameboard2[53+(8*j)+3-1] = '8';



                            }
                            else if(i==1){
                                gameboard3[53+(8*j)-1] = '2';
                                gameboard3[53+(8*j)+1-1] = '0';
                                gameboard3[53+(8*j)+2-1] = '4';
                                gameboard3[53+(8*j)+3-1] = '8';


                            }
                            else if(i==2){
                                gameboard4[53+(8*j)-1] = '2';
                                gameboard4[53+(8*j)+1-1] = '0';
                                gameboard4[53+(8*j)+2-1] = '4';
                                gameboard4[53+(8*j)+3-1] = '8';

                            }
                            else if(i==3){
                                gameboard5[53+(8*j)-1] = '2';
                                gameboard5[53+(8*j)+1-1] = '0';
                                gameboard5[53+(8*j)+2-1] = '4';
                                gameboard5[53+(8*j)+3-1] = '8';
                            }
                            break;

                    }

                }
            }

        }
        void delayMs(int n)
        {
            int i, j;
            for(i = 0; i < n; i++)
                for(j = 0; j < 3180; j++)
                    {} /* do nothing for 1 ms */
        }  /* This function is called by the startup assembly code to perform system specific  initialization tasks. */
        void SystemInit(void)  {  /* Grant coprocessor access*/  /* This is required since TM4C123G has a floating point coprocessor */
            NVIC_CPAC_R |= 0x00F00000;
        }
