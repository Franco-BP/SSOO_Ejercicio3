#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "genqueue.h"

// ============ Colored Text ===========

#define BLACK_TEXT		   "\e[30;1m"
#define RED_TEXT		   "\e[31;1m"
#define GREEN_TEXT 		   "\e[32;1m"
#define YELLOW_TEXT 	   "\e[33;1m"
#define BLUE_TEXT 		   "\e[34;1m"
#define MAGENTA_TEXT 	   "\e[35;1m"
#define CYAN_TEXT 	   	   "\e[36;1m"
#define WHITE_TEXT 		   "\e[37m"

// ============ Colored Text End ===========

#define  ONE_MS    	1000	   // One mili sec in micro Seconds
#define TRUE    1
#define FALSE   0

#define MAX_ELEMENTS_IN_QUEUE   4

#define PRINTERS_AMOUNT     2

// Ubicacion del file donde se va a escribir. cmd: 'tty' para ver la terminal.
#define FILE_LOCATION   "/dev/pts/2"

#define FILE_LOCATION2  "/dev/pts/3"


FILE *file1;
FILE *file2;
int filesCounter = 0;

pthread_mutex_t accessPrintQueue = PTHREAD_MUTEX_INITIALIZER;

Queue queueForPrint;
int elementsInQueue = 0;
int queueFull = FALSE;
int printingOngoing = FALSE;

int isQueueFull() {
    return elementsInQueue + 1 > MAX_ELEMENTS_IN_QUEUE;
}

void * computer_1() {
    while(1) {

        // TryLock returns 0 if successfull.
        // We should evaluate queueFull first to avoid locking without entering the condition.
        if (!isQueueFull() && !pthread_mutex_lock(&accessPrintQueue)) {
            printf(MAGENTA_TEXT "Computadora 1: enviando archivo...\n");

            // Logic to send the file.
            char textToPrint[] = MAGENTA_TEXT "Texto proveniente de Computadora_1.";
            enqueue(&queueForPrint, textToPrint);
            elementsInQueue += 1;

            pthread_mutex_unlock(&accessPrintQueue);
        }

        // Computer does something else
        sleep(1);
    }
}

void * computer_2() {
    while(1) {

        // TryLock returns 0 if successfull.
        // We should evaluate queueFull first to avoid locking without entering the condition.
        if (!isQueueFull() && !pthread_mutex_lock(&accessPrintQueue)) {
            printf(YELLOW_TEXT "Computadora 2: enviando archivo...\n");

            // Logic to send the file.
            char textToPrint[] =  YELLOW_TEXT "Texto proveniente de Computadora_2.";
            enqueue(&queueForPrint, textToPrint);
            elementsInQueue += 1;

            pthread_mutex_unlock(&accessPrintQueue);
        }

        // Computer does something else
        sleep(1);
    }
}

void * computer_3() {
    while(1) {

        // TryLock returns 0 if successfull.
        // We should evaluate queueFull first to avoid locking without entering the condition.
        if (!isQueueFull() && !pthread_mutex_lock(&accessPrintQueue)) {
            printf(GREEN_TEXT "Computadora 3: enviando archivo...\n");

            // Logic to send the file.
            char textToPrint[] = GREEN_TEXT "Texto proveniente de Computadora_3.";
            enqueue(&queueForPrint, textToPrint);
            elementsInQueue += 1;

            pthread_mutex_unlock(&accessPrintQueue);
        }

        // Computer does something else
        sleep(1);
    }
}


void * printer1() {
    while (1) {
        if( (!isQueueEmpty(&queueForPrint) && elementsInQueue >= PRINTERS_AMOUNT) |
            (!isQueueEmpty(&queueForPrint) && printingOngoing == FALSE)) {
            
            printingOngoing = TRUE;

            printf(RED_TEXT "Printer1: recibiendo archivo...\n");

            // "w" para abrir el archivo para escribir.
            file1 = fopen(FILE_LOCATION, "w");

            if (file1 != NULL) {
                pthread_mutex_lock(&accessPrintQueue);
                char* currentPrint = (char*)dequeue(&queueForPrint);
                elementsInQueue -= 1;
                pthread_mutex_unlock(&accessPrintQueue);

                printf(WHITE_TEXT "Printing1 ongoing.\n");
                for (int i = 0; currentPrint[i] != '\0'; i++) {
                    fprintf(file1, "%c", currentPrint[i]);
                    fflush(file1);
                    usleep(200 * ONE_MS);
                }
            }
            fclose(file1);
            printingOngoing = FALSE;
        }
    }
}

void * printer2() {
    while (1) {
        if( (!isQueueEmpty(&queueForPrint) && elementsInQueue >= PRINTERS_AMOUNT) |
            (!isQueueEmpty(&queueForPrint) && printingOngoing == FALSE)) {

            printingOngoing = TRUE;

            printf(CYAN_TEXT "Printer2: recibiendo archivo...\n");

            // "w" para abrir el archivo para escribir.
            file2 = fopen(FILE_LOCATION2, "w");

            if (file2 != NULL) {
                pthread_mutex_lock(&accessPrintQueue);
                char* currentPrint = (char*)dequeue(&queueForPrint);
                elementsInQueue -= 1;
                pthread_mutex_unlock(&accessPrintQueue);

                printf(WHITE_TEXT "Printing2 ongoing.\n");
                for (int i = 0; currentPrint[i] != '\0'; i++) {
                    fprintf(file2, "%c", currentPrint[i]);
                    fflush(file2);
                    usleep(200 * ONE_MS);
                }
            }
            fclose(file2);
            printingOngoing = FALSE;
        }
    }
}


int main () {
    int status;
    pthread_t computer_1Id, computer_2Id, computer_3Id, printer1Id, printer2Id;
    initializeQueue(&queueForPrint);

    pthread_create(&computer_1Id, NULL, computer_1, NULL);
    printf(BLUE_TEXT "Created thread computer_1.\n");

    pthread_create(&computer_2Id, NULL, computer_2, NULL);
    printf(BLUE_TEXT "Created thread computer_2.\n");

    pthread_create(&computer_3Id, NULL, computer_3, NULL);
    printf(BLUE_TEXT "Created thread computer_3.\n");

    pthread_create(&printer1Id, NULL, printer1, NULL);
    printf(BLUE_TEXT "Created thread printer1.\n" WHITE_TEXT);

    pthread_create(&printer2Id, NULL, printer2, NULL);
    printf(BLUE_TEXT "Created thread printer2.\n" WHITE_TEXT);

    pthread_join(computer_1Id, NULL);
    pthread_join(computer_2Id, NULL);
    pthread_join(computer_3Id, NULL);
    pthread_join(printer1Id, NULL);
    pthread_join(printer2Id, NULL);
    return 0;
}