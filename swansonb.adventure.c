/*
 * swansonb.adventure.c
 *
 *  Created on: Apr 29, 2015
 *      Author: Brandon Swanson
 */

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//constants/global
pid_t MYPID;
char DIRNAME[20];
const int NUM_NAMES = 10;
const int NUM_ROOMS = 7;
const int MIN_CON = 3;
const int MAX_CON = 6;


//string constants
const char * const ROOM_NAMES[] = { "Fred's Room","Music Room", "Darkroom", "Observatory", "Attic", "Dungeon", "Kitchen", "Tentacle's Room", "Ed's Room", "Front Porch"};
const char * ROOM = "ROOM NAME: ";
const char * CONNECT = "CONNECTION %d : ";
const char * const ROOM_TYPES[] = {"START_ROOM", "END_ROOM", "MID_ROOM"};
const int START = 0;
const int MID = 2;
const int END = 1;
const char * LOC = "CURRENT LOCATION: ";
const char * POSCONECT = "POSSIBLE CONNECTIONS: ";
const char * PROMPT = "WHERE TO? >";
const char * ERROR = "HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.";
const char * CONGRATS = "YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!";
const char * YOUTOOKNSTEPS = "YOU TOOK %d STEPS.";
const char * PATH = "YOUR PATH TO VICTORY WAS:";

//prototypes
int GetRandomInRange ( int min , int max );
void GetMappedRandomInts ( int valuesOut[] , int rangeBegining ,
      int rangeEnd , const int numGenerateValues, const int exclude );
void CreateRoom(int type);

int main(){

    //initialize random
    srand(time(NULL));

    //set directory name
    MYPID = getpid();
    sprintf(DIRNAME,"swansonb.rooms.%d",MYPID);

    //create working directory and change to it
    mkdir(DIRNAME, 777);
    chdir(DIRNAME);

    //chose start and end rooms
    int start_room = GetRandomInRange(0,NUM_ROOMS-1);
    int end_room = GetRandomInRange(0,NUM_ROOMS-1);
    if(end_room == start_room){
        end_room = end_room + 1 % (NUM_ROOMS-1);
    }

    //create rooms
    int room;
    for(room=0;room<NUM_ROOMS;room++){
        if(room==start_room) CreateRoom(START);
        else if(room==end_room) CreateRoom(END);
        else CreateRoom(MID);
    }

    return 0;
}

/******************************************************************************
 *    purpose:output room information into file
 *
 *    exit: single room file created as specified in assignment
 ******************************************************************************/
void CreateRoom(int type){
    //open file

    //output file name

    //output n number of connections
    int numConnections = GetRandomInRange(MIN_CON,MAX_CON);
    int connections[10];


    //output roomtype

    //close file
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    exit: random int in range [min,max]
 *
 *    Written by Brandon Swanson during Summer Term 2014 @ Oregon State University
 ******************************************************************************/
int GetRandomInRange ( int min , int max ) {

   int random;
   int range = max - min + 1;
   if ( range == 1 )
      return min;
   random = (rand() % range) + min;
   return random;
}
/******************************************************************************
 *    purpose: produce an amount (numGeneratedValues) of numbers across a given
 *             range avoiding duplicate values
 *             excluding 1 value
 *
 *    entry: empty array of size numGenerateValues, rangeBegining<=rangeEnd,
 *           numGeneratedValues <= rangeEnd-rangeBegining
 *
 *    exit: an array of non repeated random values
 *
 *    must create an array of size rangeEnd-rangeBegining, use for small amounts
 *
 ******************************************************************************/
void GetMappedRandomInts ( int valuesOut[] , int rangeBegining ,
      int rangeEnd , const int numGenerateValues, const int exclude) {

    //generate values to chose from
    int poolSize = rangeEnd-rangeBegining;
    int numPool[poolSize];
    int i;
    int val = rangeBegining;
    for (i=0; i<poolSize; i++){
        if (val==exclude)val++;
        numPool[i]=val++;
    }

    //chose n number from pool and place them in new array
    for(i=0;i<numGenerateValues;i++){
        int randindex = GetRandomInRange(0,--poolSize);
        valuesOut[i]=numPool[randindex];
        //overwrite chosen value replacing it with what was the last selectable value
        numPool[randindex]=numPool[poolSize];
    }

}


