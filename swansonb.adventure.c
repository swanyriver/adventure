/*
 * swansonb.adventure.c
 *
 *  Created on: Apr 29, 2015
 *      Author: Brandon Swanson
 */

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//constants/global
pid_t MYPID;
char DIRNAME[20];
const int NUMNAMES = 10;
const int NUM_ROOMS = 7;
const int MIN_CON = 3;
const int MAX_CON = 6;


//string constants
const char * const ROOM_NAMES[] = { "Fred's Room","Music Room", "Darkroom", "Observatory", "Attic", "Dungeon", "Kitchen", "Tentacle's Room", "Ed's Room", "Front Porch"};
const char * ROOM = "ROOM NAME: ";
const char * CONNECT = "CONNECTION %d : ";
const char * const ROOM_TYPES[] = {"START_ROOM", "END_ROOM", "MID_ROOM"};
const char * LOC = "CURRENT LOCATION: ";
const char * POSCONECT = "POSSIBLE CONNECTIONS: ";
const char * PROMPT = "WHERE TO? >";
const char * ERROR = "HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.";
const char * CONGRATS = "YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!";
const char * YOUTOOKNSTEPS = "YOU TOOK %d STEPS.";
const char * PATH = "YOUR PATH TO VICTORY WAS:";

//prototypes
int GetRandomInRange ( int min , int max );

int main(){

    //initialize random
    srand(time(NULL));

    //set directory name
    MYPID = getpid();
    sprintf(DIRNAME,"swansonb.rooms.%d",MYPID);


    printf("process:%d directory:%s random:%d",MYPID, DIRNAME, GetRandomInRange(3,6));


    return 0;
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    exit: random int in range [min,max]
 *
 *    Written by Brandon Swanson during Summer Term 2014 @ Oregon State University
 ******************************************************************************/
GetRandomInRange ( int min , int max ) {

   int random;
   int range = max - min + 1;
   if ( range == 1 )
      return min;
   random = (rand() % range) + min;
   return random;
}
