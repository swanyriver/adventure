/*
 * swansonb.adventure.c
 *
 *  Created on: Apr 29, 2015
 *      Author: Brandon Swanson
 *
 *      conforms to ISO C90
 */

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>

/*compile time constants (for setting array sizes)*/
#define NUM_NAMES 10
#define NUM_ROOMS 7
#define MIN_CON 3
#define MAX_CON 6
#define RMNAMEBUFFSIZE 64
#define DIRECTORYNAMEBUFF 32
#define MAXPATH 1200
#define MAXBUFF 300

/*string constants*/
const char * const ROOM_NAMES[] = { "Fred's Room", "Music Room", "Darkroom",
        "Observatory", "Attic", "Dungeon", "Kitchen", "Tentacle's Room",
        "Ed's Room", "Front Porch" };
const char * ROOM = "ROOM NAME: %s\n";
const char * CONNECT = "CONNECTION %d : %s\n";
const char * const ROOM_TYPES[] = { "START_ROOM", "END_ROOM", "MID_ROOM" };
const char * ROOM_TYPE = "ROOM TYPE: %s\n";
const int START = 0;
const int MID = 2;
const int END = 1;
const char * LOC = "\nCURRENT LOCATION: %s\n";
const char * POSCONECT = "POSSIBLE CONNECTIONS: ";
const char * WHERE = "WHERE TO? >";
const char * ERROR = "\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n";
const char * CONGRATS = "\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!";
const char * YOUTOOKNSTEPS = "\nYOU TOOK %d STEPS. ";
const char * PATH = "YOUR PATH TO VICTORY WAS:\n";
const char * STARTFILE = "start";

/*prototypes*/
int GetRandomInRange ( int min , int max );
void GetMappedRandomRange ( int valuesOut[] , const int numOut ,
        int rangeBegining , int rangeEnd );
void GetMappedRandomArr ( int valuesOut[] , const int numOut , int valuesIn[] ,
        int numIn );
void CreateRoom ( int type , int roomNum , int roomsSelected[] );
void displayRoomPrompt ( char* roomName );
int isEndRoom ( char* roomName );
void GetFirstRoom ( char* output );
int isConnection ( char* selection , char* connections[] , int numConnections );

int main () {

    /*variable declarations;*/
    pid_t MYPID;
    char DIRNAME[DIRECTORYNAMEBUFF] , nextRoom[RMNAMEBUFFSIZE] ,
        PathRecord[MAXPATH];
    int start_room , end_room , room , steps, roomsSelected[NUM_ROOMS];
    char* sPos;

    /*initialize random*/
    srand( time( NULL ) );

    /*/////////////////////////////////////////////////////
    //////CREATE ROOM FILE SYSTEM /////////////////////////
    /////////////////////////////////////////////////////*/

    /*set directory name*/
    MYPID = getpid();
    snprintf( DIRNAME , DIRECTORYNAMEBUFF , "swansonb.rooms.%d" , MYPID );

    /*create working directory and change to it*/
    mkdir( DIRNAME , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    chdir( DIRNAME );

    /*randomly chose start and end rooms (index from 0-6)*/
    start_room = GetRandomInRange( 0 , NUM_ROOMS - 1 );
    end_room = GetRandomInRange( 0 , NUM_ROOMS - 1 );
    if ( end_room == start_room ) {
        end_room = end_room + 1 % (NUM_ROOMS - 1);
    }

    /*create rooms, from list of unique randoms for room names, assigning types*/
    GetMappedRandomRange( roomsSelected , NUM_ROOMS , 0 , NUM_NAMES - 1 );
    for ( room = 0; room < NUM_ROOMS ; room++ ) {
        if ( room == start_room )
            CreateRoom( START , roomsSelected[room] , roomsSelected );
        else if ( room == end_room )
            CreateRoom( END , roomsSelected[room] , roomsSelected );
        else
            CreateRoom( MID , roomsSelected[room] , roomsSelected );
    }

    /*find start room to present beginning of adventure*/
    GetFirstRoom( nextRoom );


    /*string pointer set to beginning of path char array, step count init */
    sPos = PathRecord;
    steps = 0;


    /*/////////////////////////////////////////////////////
    // MAIN GAMEPLAY LOOOP ////////////////////////////////
    //////read a file, display prompt, check input,   /////
    //////return selection, break if that is end room//////
    /////////////////////////////////////////////////////*/

    do {
        /*next room is modified to users desired next step*/
        displayRoomPrompt( nextRoom );
        steps++;

        sPos += snprintf( sPos , MAXPATH - (sPos - PathRecord) - 1 , "%s\n" ,
                nextRoom );

    } while ( !isEndRoom( nextRoom ) );


    /*/////////////////////////////////////////////////////
    //////GAME HAS BEEN WON ///////////////////////////////
    /////////////////////////////////////////////////////*/

    /*display winning message*/
    printf( "%s" , CONGRATS );
    printf( YOUTOOKNSTEPS , steps );
    printf( "%s" , PATH );
    *sPos = '\0';  /*null terminating path string*/
    printf( "%s" , PathRecord );

    return 0;
}

/******************************************************************************
 *    purpose:find name of first room
 *
 *    entry: file STARTFILE exists and contains room name
 *
 *    exit: modifies char* output with filename/roomname of first room
 *
 ******************************************************************************/
void GetFirstRoom ( char* output ) {

    FILE *stfile;
    int readcount;

    /*open pointer file*/
    stfile = fopen( STARTFILE , "r" );

    /*read in roomname and null terminate it*/
    readcount = fread( output , sizeof(char) , RMNAMEBUFFSIZE - 1 ,
            stfile );
    output[readcount] = '\0';

    /*close and remove startfile */
    fclose(stfile);
    remove(STARTFILE);
}

/******************************************************************************
 *    purpose:output room information into file
 *
 *    exit: single room file created as specified in assignment
 ******************************************************************************/
void CreateRoom ( int type , int roomNum , int roomsSelected[] ) {

    FILE *roomFile;
    int numConnections , connections[MAX_CON] , i , connection ,
    possibleConnections[MAX_CON];

    /*open file*/
    roomFile = fopen( ROOM_NAMES[roomNum] , "w" );

    /*output room name*/
    fprintf( roomFile , ROOM , ROOM_NAMES[roomNum] );

    /*create array of all available connections from 7 names selected excluding self*/
    numConnections = GetRandomInRange( MIN_CON , MAX_CON );
    for ( i = 0 ; i < NUM_ROOMS - 1 ; i++ ) {
        if ( i == roomNum ) {
            possibleConnections[i] = roomsSelected[NUM_ROOMS - 1];
        } else {
            possibleConnections[i] = roomsSelected[i];
        }
    }
    GetMappedRandomArr( connections , numConnections , possibleConnections ,
            NUM_ROOMS - 1 );

    /*output possible connections*/
    for ( connection = 1 ; connection <= numConnections ; connection++ ) {
        fprintf( roomFile , CONNECT , connection ,
                ROOM_NAMES[connections[connection - 1]] );
    }

    /*output roomtype to file*/
    fprintf( roomFile , ROOM_TYPE , ROOM_TYPES[type] );

    /*close file*/
    fclose( roomFile );

    /*make start room pointer*/
    if ( type == START ) {
        roomFile = fopen( STARTFILE , "w" );
        fprintf( roomFile , "%s" , ROOM_NAMES[roomNum] );
        fclose( roomFile );
    }
}

/******************************************************************************
 *    purpose:read file specified by roomName and display connections and get user input
 *
 *    entry: file roomName exists and contains appropriate room data
 *
 *    exit: modifies char* roomName with users desired step 
 *          (ensured to be one of possible connections)
 *
 ******************************************************************************/
void displayRoomPrompt ( char* roomName ) {

    /*variable declarations*/
    char fileBuff[MAXBUFF];     /*file read buffer */
    char prompt[MAXBUFF];       /*output string buffer*/
    char* lines[MAX_CON + 2];   /*pointer array for strtok divisions*/
    char* connectNames[MAX_CON];/*pointer array for parsed room names*/
    int psize , readcount , numLines , numConnections, i;
    FILE* roomFile;

    /*////////////////////////////////////
    ///GET AND DISPLAY ROOM INFO /////////
    ////////////////////////////////////*/

    /*as the size of the prompt grows each call to snprintf is pointed to the new end of the string
    *  prompt+psize,  and the max allowable writing is the original buffer - psize and minus 1 leaving
    *  room for null terminator 
    *
    *  information read from file is read/parsed/and printed to string for easy re-printing when user
    *   enters invalid input
    */
    psize = 0;
    psize += snprintf( prompt + psize , MAXBUFF - psize - 1 , LOC , roomName );
    psize += snprintf( prompt + psize , MAXBUFF - psize - 1 , "%s" ,
            POSCONECT );

    /*open roomfile for reading*/
    roomFile = fopen( roomName , "r" );

    readcount = fread( fileBuff , sizeof(char) , MAXBUFF - 1 , roomFile );
    fileBuff[readcount] = '\0';

    /*use strtok to create an array of pointers the begining of each line, replacing '/n' with '/0'*/
    numLines = 0;
    lines[0] = strtok( fileBuff , "\n" );
    while ( lines[numLines] ) {
        lines[++numLines] = strtok( NULL , "\n" );
    }

    /*display possible connections*/
    numConnections = numLines - 2;
    for ( i = 0 ; i < numConnections ; i++ ) {
        /*finds location of : char in line and points 2 characters advance*/
        connectNames[i] = strchr( lines[i + 1] , ':' ) + 2;
        psize += snprintf( prompt + psize , MAXBUFF - psize - 1 ,
                "%s" , connectNames[i] );
        /*print punctuation*/
        if ( i == numConnections - 1 )
            psize += snprintf( prompt + psize , MAXBUFF - psize - 1 ,
                    "%s" , ".\n" );
        else
            psize += snprintf( prompt + psize , MAXBUFF - psize - 1 ,
                    "%s" , ", " );
    }

    /*print where to> prompt and terminate string*/
    psize += snprintf( prompt + psize , MAXBUFF - psize - 1 , "%s" , WHERE );
    prompt[psize + 1] = '\0';

    fclose( roomFile );

    /*////////////////////////////////////
    ///GET USERS NEXT STEP ///// /////////
    ////////////////////////////////////*/
    do {
        printf( "%s" , prompt );
        fgets( roomName , RMNAMEBUFFSIZE , stdin );
        roomName[strlen( roomName ) - 1] = '\0'; /*remove trailing '\n' */
    } while ( !isConnection( roomName , connectNames , numConnections ) );

}

/******************************************************************************
 *    purpose:verify users input
 *
 *    entry: char* connections contains numConnection cstrings of possible connections
 *
 *    exit: true if selection is contained in connections[]
 *
 ******************************************************************************/
int isConnection ( char* selection , char* connections[] , int numConnections ) {

    int i = 0;
    for ( ; i < numConnections ; i++ ) {
        if ( strcmp( selection , connections[i] ) == 0 ) {
            /*user inputed string matches one of the connections*/
            /*return true*/
            return 1;
        }
    }

    /*no match found  //return false*/
    printf( "%s" , ERROR );
    return 0;
}

/******************************************************************************
 *    purpose:checks file to see if specified room is end of maze
 *
 *    entry: file roomName exists and contains appropriate room data
 *
 *    exit: true if last line contains string ROOM_TYPES[END]
 *
 ******************************************************************************/
int isEndRoom ( char* roomName ) {

    /*variable declarations*/
    char fileBuff[MAXBUFF]; /*file read buffer*/
    FILE* roomFile;
    int readcount;
    char* end;

    /*open roomfile for reading*/
    roomFile = fopen( roomName , "r" );

    readcount = fread( fileBuff , sizeof(char) , MAXBUFF - 1 , roomFile );
    fileBuff[readcount - 1] = '\0';

    fclose(roomFile);

    /*move pointer to end of file and decrement until pointing at :*/
    end = fileBuff + readcount;
    while ( *end != ':' )
        end--;
    end += 2;

    /*compare last word of file with ROOM_TYPES[END]*/
    if ( strcmp( end , ROOM_TYPES[END] ) == 0 )
        return 1;
    else
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
    /*calculate inclusive range size*/
    int range = max - min + 1;
    if ( range == 1 )
        return min;
    /*map return 0-range onto min through max*/
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
void GetMappedRandomRange ( int valuesOut[] , const int numOut ,
        int rangeBegining , int rangeEnd ) {

    /*generate values to chose from*/
    int poolSize = rangeEnd - rangeBegining + 1;
    int* numPool = malloc( poolSize * sizeof(int) );
    int i;
    int val = rangeBegining;
    for ( i = 0; i < poolSize ; i++ ) {
        numPool[i] = val++;
    }

    /*call with array flled with rangeBeginging to rangeEnd values*/
    GetMappedRandomArr( valuesOut , numOut , numPool , poolSize );

    free( numPool );

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
void GetMappedRandomArr ( int valuesOut[] , const int numOut , int valuesIn[] ,
        int numIn ) {
    /*chose n number from pool and place them in new array*/
    int i;
    for ( i = 0; i < numOut ; i++ ) {
        int randindex = GetRandomInRange( 0 , --numIn );
        valuesOut[i] = valuesIn[randindex];
        /*overwrite chosen value replacing it with what was the last selectable value*/
        valuesIn[randindex] = valuesIn[numIn];
    }
}

