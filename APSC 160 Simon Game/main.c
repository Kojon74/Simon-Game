/*
 * Simon Game
 * Author: Ken Johnson
 * Student Number: 49613193
 * Lab Section: L2X
 * Date: Firday March 22, 2019
 
 * Purpose: To help me get into CPEN.
 * How to Play: Run program and follow instructions. Objective of the game is to memorize the pattern after it has flashed on the DAQ LED's, then enter the pattern by pressing the button that corresponds to each LED. Number of LED flashes increases by one each time you enter the correct sequence until you win (after a certain number of successful rounds) or lose (enter the wrong pattern).
 
 get rid of global variables?
 */

#include <DAQlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define OFF 0
#define ON 1

#define LED0 0
#define NUM_LED 4

#define BUTTON0 0
#define NUM_BUTTON 4

#define MAX_LENGTH 10

void runSimon(void);
void generateSequence(int[]);
int checkIfRight(int, int[]);
void WinOrLose(int);

/*
 * Parameters: None, Return Values: -1 if error setting up DAQ, 0 otherwise
 * Checks whether DAQ has been set up correctly & asks user to enter setupDAQ number corrisponding to HW Module 6
 * Calls function runSimon()
 */
int main(void)
{
    int setupNum;
    
    printf("Enter 6 to play the Simon Game.\n");
    scanf("%d", &setupNum);
    if(!setupDAQ(setupNum)){
        return -1;
    }
    runSimon();
    return 0;
}

/*
 * Parameters: None, Return Values: None
 * Creates array of length MAX_LENGTH which will contain the LED pattern
 * Calls generateSequence(LEDnum) then uses a for loop to flash first b LED's in the array LEDnum
 * Calls checkIfRight() and then uses a simple if statement to check whether user has lost or not to break out of the current for loop to start a new game
 * Checks if the user has gotten the entire sequence with length MAX_LENGTH right, if true, calls WinOrLose(0)
 */
void runSimon(void)
{
    int LEDnum[MAX_LENGTH];
    int b;
    int lose;
    while(continueSuperLoop()){
        delay(2000);
        generateSequence(LEDnum);
        for(b = 0; b < MAX_LENGTH; b++){
            for(int c = 0; c <= b; c++){
                delay(500);
                if(!continueSuperLoop()){
                    return;
                }
                digitalWrite(LEDnum[c], ON);
                delay(500);
                digitalWrite(LEDnum[c], OFF);
            }
            lose = checkIfRight(b, LEDnum);
            if(lose > 0){
                break;
            }
        }
        if(b == MAX_LENGTH){
            WinOrLose(0);
        }
    }
}

/*
 * Parameters: LEDnum[], Return Values: None
 * Uses srand(), for loop, and % to set random numbers to each index in the array between 0 and 3 inclusive
 */
void generateSequence(int LEDnum[])
{
    srand((unsigned) time(NULL));
    for(int a = 0; a < MAX_LENGTH; a++){
        LEDnum[a] = rand() % 4;
    }
}

/*
 * Parameters: b (indicates up to which index in the LEDnum array to flash), LEDnum[] (which contains the pattern to flash), Return Values: None
 * Checks to see whether the pattern player has input matches the pattern that was flashed. If it does not match, increments _lose by one, which indicates the player has lost and calls WinOrLose(1) when the player has finished entering their pattern
 * Does not start reading for the next button press until the user releases the last button
 */
int checkIfRight(int b, int LEDnum[])
{
    int count = 0;
    int lose = 0;
    while(count <= b){
        for(int a = 0; a < NUM_BUTTON; a++){
            if(!continueSuperLoop()){
                return 0;
            }
            if(digitalRead(BUTTON0 + a)){
                if(!(LEDnum[count] == BUTTON0 + a)){
                    lose++;
                }
                count++;
                while(digitalRead(BUTTON0 + a)){
                }
            }
        }
    }
    if(lose > 0){
        WinOrLose(1);
        printf("%d\n", b);
        return lose;
    }
    return 0;
}

/*
 * Parameters: a (0 corresponds to the player winning, 1 corresponds to the player losing), Return Values: None
 * Flashes green or red light 3 times, depending on the value of a to indicate that the player has won or lost (respectfully)
 * Flashes slower than pattern so player doesn't think it is a pattern
 */
void WinOrLose(int a)
{
    for(int d = 0; d < 3; d++){
        delay(500);
        digitalWrite(LED0 + a, ON);
        delay(1000);
        digitalWrite(LED0 + a, OFF);
        delay(1000);
    }
}
