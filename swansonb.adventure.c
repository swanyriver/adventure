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
void GetMappedRandomRange ( int valuesOut[] , const int numOut,
        int rangeBegining, int rangeEnd);
void GetMappedRandomArr(int valuesOut[], const int numOut,
        int valuesIn[], int numIn );
void CreateRoom(int type, int roomNum, int rooms[]);

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

    //TODO Make call to map range
    //TODO test its still working
    int i;
    for (i=0;i<15;i++){
        int testarr[8];
        GetMappedRandomRange(testarr,6,1,8);
        int v;
        for(v=0;v<6;v++) printf("%d,",testarr[v]);
        printf("\n");
    }

    int room;
   /* for(room=0;room<NUM_ROOMS;room++){
        if(room==start_room) CreateRoom(START);
        else if(room==end_room) CreateRoom(END);
        else CreateRoom(MID);
    }*/

    return 0;
}

/******************************************************************************
 *    purpose:output room information into file
 *
 *    exit: single room file created as specified in assignment
 ******************************************************************************/
//TODO  will need to know its room number to exclude it from the others
//TODO decide on function signature
void CreateRoom(int type, int roomNum, int rooms[]){
    //open file

    //output file name

    //output n number of connections
    int numConnections = GetRandomInRange(MIN_CON,MAX_CON);
    int connections[MAX_CON];

    //todo make call to random arr with exclusion array generated
    //todo returned array will represent indicies of chosen array not room number
    //GetMappedRandomInts(connections,)


    //output roomtype

    //close file
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    entry: max >= min
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
 *    entry: empty array of size numOut, rangeBegining<=rangeEnd,
 *           numGeneratedValues <= rangeEnd-rangeBegining
 *
 *    exit: an array of non repeated random values
 *
 *    must create an array of size rangeEnd-rangeBegining, use for small amounts
 *
 ******************************************************************************/
void GetMappedRandomRange ( int valuesOut[] , const int numOut,
        int rangeBegining, int rangeEnd) {

    //generate values to chose from
    int poolSize = rangeEnd-rangeBegining+1;
    int numPool[poolSize];
    int i;
    int val = rangeBegining;
    for (i=0; i<poolSize; i++){
        numPool[i]=val++;
    }

    GetMappedRandomArr(valuesOut,numOut,numPool,poolSize);

}

void GetMappedRandomArr(int valuesOut[], const int numOut,
        int valuesIn[], int numIn ){
    //chose n number from pool and place them in new array
    int i;
    for(i=0;i<numOut;i++){
        int randindex = GetRandomInRange(0,--numIn);
        valuesOut[i]=valuesIn[randindex];
        //overwrite chosen value replacing it with what was the last selectable value
        valuesIn[randindex]=valuesIn[numIn];
    }
}


