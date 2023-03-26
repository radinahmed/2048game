# 2048 game
This is a game I've developed using C programming language. The game consists of blocks where you have to connect each block that has the the same value, eventually reaching a maximum of 2048.
A key component of this game was to use a random number generator to output new numbers either 2, or 4 and to calculate the placement of newly generated blocks. I used the GPIO Timer which is built in to the Tiva C series processor. I've also utilized the UART to display my output. 

![2048_game_c_project 99940c6168f080e6326c](https://user-images.githubusercontent.com/54868742/227751836-7cabc65a-68bb-49a5-853a-e1fef800b6e1.gif)

If you have a Tiva C series Tm4c123gh6pm on hand you can sample this code on your machine using Code composer studio and Putty.
