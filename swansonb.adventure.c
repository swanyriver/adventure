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
#include <fcntl.h>

//constants/global
pid_t MYPID;
char DIRNAME[20];
const int NUM_NAMES = 10;
const int NUM_ROOMS = 7;
const int MIN_CON = 3;
const int MAX_CON = 6;
const int RMNAMEBUFFSIZE=64;


//string constants
const char * const ROOM_NAMES[] = { "Fred's Room","Music Room", "Darkroom",
        "Observatory", "Attic", "Dungeon", "Kitchen", "Tentacle's Room",
        "Ed's Room", "Front Porch"};
const char * ROOM = "ROOM NAME: %s\n";
const char * CONNECT = "CONNECTION %d : %s\n";
const char * const ROOM_TYPES[] = {"START_ROOM", "END_ROOM", "MID_ROOM"};
const char * ROOM_TYPE = "ROOM TYPE: %s\n";
const int START = 0;
const int MID = 2;
const int END = 1;
const char * LOC = "\nCURRENT LOCATION: %s\n";
const char * POSCONECT = "POSSIBLE CONNECTIONS: ";
const char * WHERE = "WHERE TO? >";
const char * ERROR = "\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n";
const char * CONGRATS = "YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!";
const char * YOUTOOKNSTEPS = "YOU TOOK %d STEPS.";
const char * PATH = "YOUR PATH TO VICTORY WAS:";
const char * STARTFILE ="start";

//prototypes
int GetRandomInRange ( int min , int max );
void GetMappedRandomRange ( int valuesOut[] , const int numOut,
        int rangeBegining, int rangeEnd);
void GetMappedRandomArr(int valuesOut[], const int numOut,
        int valuesIn[], int numIn );
void CreateRoom(int type, int roomNum, int roomsSelected[]);
void displayRoomPrompt(char* roomName);
int isEndRoom(char* roomName);
void GetFirstRoom(char* output);
int isConnection(char* selection, char* connections[], int numConnections);

int main(){

    //initialize random
    srand(time(NULL));

    //set directory name
    MYPID = getpid();
    sprintf(DIRNAME,"swansonb.rooms.%d",MYPID);

    //create working directory and change to it
    mkdir(DIRNAME, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    chdir(DIRNAME);

    //chose start and end rooms
    int start_room = GetRandomInRange(0,NUM_ROOMS-1);
    int end_room = GetRandomInRange(0,NUM_ROOMS-1);
    if(end_room == start_room){
        end_room = end_room + 1 % (NUM_ROOMS-1);
    }

    //create rooms
    int roomsSelected[NUM_ROOMS];
    GetMappedRandomRange(roomsSelected,NUM_ROOMS,0,NUM_NAMES-1);
    int room;
    for(room=0;room<NUM_ROOMS;room++){
        if(room==start_room) CreateRoom(START,room,roomsSelected);
        else if(room==end_room) CreateRoom(END,room,roomsSelected);
        else CreateRoom(MID,room,roomsSelected);
    }

    //find start room to present beginning of adventure
    char nextRoom[RMNAMEBUFFSIZE];
    GetFirstRoom(nextRoom);
    //printf("first: %s\n",nextRoom);

    do{
        //next room is modified to users desired next step
        displayRoomPrompt(nextRoom);
    }while(!isEndRoom(nextRoom));

    //display winning message

    return 0;
}

void GetFirstRoom(char* output){

    //open pointer file
    FILE *stfile = fopen(STARTFILE,"r");

    //read in roomname and null terminate it
    int readcount=fread(output,sizeof(char),RMNAMEBUFFSIZE-1,stfile);
    output[readcount] = '\0';
}

/******************************************************************************
 *    purpose:output room information into file
 *
 *    exit: single room file created as specified in assignment
 ******************************************************************************/
void CreateRoom(int type, int roomNum, int roomsSelected[]){

    int myRoom = roomsSelected[roomNum];

    //open file
    FILE *roomFile = fopen(ROOM_NAMES[myRoom], "w");

    //output room name
    fprintf( roomFile, ROOM,ROOM_NAMES[myRoom]);

    //output n number of connections
    int numConnections = GetRandomInRange(MIN_CON,MAX_CON);
    int connections[MAX_CON];  

    //create array of available connections
    int possibleConnections[MAX_CON];
    int i=0;
    for(;i<NUM_ROOMS-1;i++){
        if(i==roomNum){
            possibleConnections[i]=roomsSelected[NUM_ROOMS-1];
        } else {
            possibleConnections[i]=roomsSelected[i];
        }
    }
    GetMappedRandomArr(connections,numConnections,
            possibleConnections,NUM_ROOMS-1);

    int connection=1;
    for(;connection<=numConnections;connection++){
        fprintf( roomFile, CONNECT,
                connection,ROOM_NAMES[connections[connection-1]]);
    }

    //output roomtype
    fprintf( roomFile, ROOM_TYPE,ROOM_TYPES[type]);

    //close file*/
    fclose(roomFile);

    //make start room pointer
    if(type == START){
        roomFile = fopen(STARTFILE,"w");
        fprintf(roomFile,"%s",ROOM_NAMES[myRoom]);
        fclose(roomFile);
    }
}

void displayRoomPrompt(char* roomName){

    //////////////////////////////////////
    ///GET AND DISPLAY ROOM INFO /////////
    //////////////////////////////////////

    //file read buffer
    const int MAXBUFF = 300;
    char fileBuff[MAXBUFF];

    //output stream buffer
    char prompt[MAXBUFF];
    int psize = 0;

    psize += snprintf(prompt+psize,MAXBUFF-psize-1,LOC,roomName);
    psize += snprintf(prompt+psize,MAXBUFF-psize-1,"%s",POSCONECT);

    //open roomfile for reading
    FILE* roomFile = fopen(roomName,"r");

    int readcount=fread(fileBuff,sizeof(char),MAXBUFF-1,roomFile);
    fileBuff[readcount] = '\0';

    char* lines[MAX_CON+2];
    int numLines =0;
    lines[0]=strtok(fileBuff,"\n");
    while(lines[numLines]){
        lines[++numLines]=strtok(NULL,"\n");
    }

    char* connectNames[MAX_CON];
    int numConnections = numLines - 2;
    int i=0;
    for(;i<numConnections;i++){
        connectNames[i] = strchr(lines[i+1],':') + sizeof(char)*2;
        psize += snprintf(prompt+psize,MAXBUFF-psize-1,"%s",connectNames[i]);
        if (i == numConnections-1) psize += snprintf(prompt+psize,MAXBUFF-psize-1,"%s",".\n");
        else psize += snprintf(prompt+psize,MAXBUFF-psize-1,"%s",", ");
    }

    psize += snprintf(prompt+psize,MAXBUFF-psize-1,"%s",WHERE);
    prompt[psize+1]='\0';

    fclose(roomFile);
    //////////////////////////////////////
    ///GET USERS NEXT STEP ///// /////////
    //////////////////////////////////////
    do{
        printf("%s",prompt);
        fgets(roomName,RMNAMEBUFFSIZE,stdin);
        roomName[strlen(roomName)-1]='\0'; //remove trailing '\n'
    }while(!isConnection(roomName,connectNames,numConnections));

}

int isConnection(char* selection, char* connections[], int numConnections){

    int i=0;
    for(;i<numConnections;i++){
        if(strcmp(selection,connections[i])==0){
            //user inputed string matches one of the connections
            //return true
            return 1;
        }
    }

    //no match found  //return false
    printf("%s",ERROR);
    return 0;
}

int isEndRoom(char* roomName){
    return 0;
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
 *    purpose: produce an amount (numOut) of numbers across a given
 *             range avoiding duplicate values
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

/******************************************************************************
 *    purpose: randomly select numOut number of unique indices to fill
 *             valuesOut from valuesIn
 *
 *    entry: empty array of size numOut,numOut<=NumIn
 *
 *    exit: array of unique copies from valuesIn
 *          (only unique values if valuesIn is all unique)
 *          modifies values in
 *
 ******************************************************************************/
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

