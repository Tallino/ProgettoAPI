#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct transStates{
    struct transInfo *punt;
}transStates;

transStates **tStates = NULL;

typedef struct transInfo{

    struct transInfo *next;
    int stato_part;
    int stato_arriv;
    char carattere_lett;
    char carattere_scritt;
    char mov;

}transInfo;

typedef struct Node {
    transInfo *pointer;
    struct Node *next;
    char *copy;
    int pos;
    int copyDim;
    long int passes;
}Node;

Node *myQueue, *tail = NULL;

int accDim = 0;
long int maxPasses;
int trovato = 0;
int alreadyEnqueued = 0;
int dim=3500;
int acceptationStatus[100];
int i,j=0;
char *input;
int init = 1;
int sonsnum=0;
transInfo *t;
Node *p,*u;

inline static void TransitionsDecode();
inline static void IntermediateStringsDecode();
inline static int dequeueHead();
inline static void copyTape(Node* t);
inline static void enqueue(transInfo* toEnqueue, int flag, int pos, long int passes, int dim);

int main() {

    TransitionsDecode();
    IntermediateStringsDecode();

    while(scanf("%ms",&input) != EOF) {
        i = 0;
        trovato = 0;
        init = 1;
        sonsnum = 0;
        int flagU = 0;
        int found = 0;

        if(tStates[0]!=NULL)
            if(tStates[0][input[0]-65].punt!=NULL){
                for (t = tStates[0][input[0]-65].punt; t != NULL; t = t->next)
                    enqueue(t, 1, 0, 0, (int) (strlen(input) + 1));
                found=1;
            }

        free(input);
        if (found)
            dequeueHead();

        init = 0;

        while (found != 0 && p->passes != maxPasses) {

            alreadyEnqueued = 0;
            sonsnum = 0;

            if(p->pointer->carattere_lett=='_' && p->pointer->stato_part==p->pointer->stato_arriv && (p->pointer->mov=='R' || p->pointer->mov=='L'))
                flagU=1;
            else {
                p->copy[p->pos] = p->pointer->carattere_scritt;
                p->passes++;

                if (p->pointer->mov == 'R') {
                    if (p->copy[p->pos + 1] == '\0') {
                        p->copy = realloc(p->copy, (size_t) (p->copyDim + 256));
                        strcat(p->copy,
                               "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________");
                        p->pos++;
                        p->copyDim = p->copyDim + 256;
                    } else
                        p->pos++;
                }

                if (p->pointer->mov == 'L') {
                     if (p->pos == 0) {
                        char *temp = malloc((size_t) (p->copyDim + 512));
                        memset(temp, '_', 511);
                        temp[511] = '\0';
                        strcat(temp, p->copy);
                        p->copy = temp;
                        p->pos = p->pos + 510;
                        p->copyDim = p->copyDim + 512;
                    } else
                        p->pos--;
                }

                if (p->pointer->mov == 'S') {
                    if (tStates[p->pointer->stato_arriv] != NULL)
                        if (tStates[p->pointer->stato_arriv][p->copy[p->pos] - 65].punt != NULL)
                            for (t = tStates[p->pointer->stato_arriv][p->copy[p->pos] - 65].punt;
                                 t != NULL; t = t->next) {
                                sonsnum++;
                                if (sonsnum == 1)
                                    enqueue(t, 1, p->pos, p->passes, p->copyDim);
                                else
                                    enqueue(t, 0, p->pos, p->passes, p->copyDim);
                                alreadyEnqueued = 1;
                            }
                }

                if (alreadyEnqueued != 1)
                    if (tStates[p->pointer->stato_arriv] != NULL)
                        if (tStates[p->pointer->stato_arriv][p->copy[p->pos] - 65].punt != NULL)
                            for (t = tStates[p->pointer->stato_arriv][p->copy[p->pos] - 65].punt;
                                 t != NULL; t = t->next) {
                                sonsnum++;
                                if (sonsnum == 1)
                                    enqueue(t, 1, p->pos, p->passes, p->copyDim);
                                else
                                    enqueue(t, 0, p->pos, p->passes, p->copyDim);
                            }
            }

            if (myQueue == NULL) {
                if (flagU)
                    break;

                found = 0;
                break;
            } else if (dequeueHead())
                break;

        }

        if (trovato)
            printf("1");
        else if (found && (p->passes == maxPasses || flagU))
            printf("U");
        else if (!found)
            printf("0");

        printf("\n");

        while (myQueue != NULL) {
            u = myQueue;
            myQueue = myQueue->next;
            free(u->copy);
            free(u);
        }

    }
    return 0;
}


inline static void TransitionsDecode() {

    transInfo *nuovo;
    char str[6] = "";

    tStates = (transStates **) malloc(dim * sizeof(transStates*));

    scanf("%s",str);

    if(strcmp(str,"tr")==0) {

        for(i=0;i<dim;i++)
            tStates[i]=NULL;

        while(strcmp(str,"acc")!=0) {

            scanf("%s",str);

            if(strcmp(str,"acc")!=0) {

                nuovo = (transInfo*) malloc(sizeof(transInfo));
                nuovo->stato_part = atoi(str);

                scanf("%s", str);
                nuovo->carattere_lett = str[0];

                scanf("%s", str);
                nuovo->carattere_scritt = str[0];

                scanf("%s", str);
                nuovo->mov = str[0];

                scanf("%s", str);
                nuovo->stato_arriv = atoi(str);

                if(tStates[nuovo->stato_part]==NULL){
                    tStates[nuovo->stato_part] = (transStates *) malloc((58)*sizeof(transStates));

                    for(j=0;j<58;j++)
                        tStates[nuovo->stato_part][j].punt=NULL;

                    tStates[nuovo->stato_part][nuovo->carattere_lett-65].punt = nuovo;
                    nuovo->next=NULL;
                }
                else {
                    nuovo->next= tStates[nuovo->stato_part][nuovo->carattere_lett-65].punt;
                    tStates[nuovo->stato_part][nuovo->carattere_lett-65].punt = nuovo;
                }
            }
        }
    }
}


inline static void IntermediateStringsDecode() {

    char str[6] = "";
    char temp;

    scanf("%s",str);
    acceptationStatus[accDim] = atoi(str);

    scanf("%s",str);

    if(strcmp(str,"max")==0) {
        scanf("%ld",&maxPasses);
        scanf("%s",str);
    }

    else {
        accDim++;
        acceptationStatus[accDim] = atoi(str);
        scanf("%s", str);

        while (strcmp(str, "max") != 0) {
            accDim++;
            acceptationStatus[accDim] = atoi(str);
            scanf("%s", str);

        }
        if (strcmp(str, "max") == 0) {
            scanf("%ld", &maxPasses);
            scanf("%s", str);
        }
    }

    scanf("%c",&temp);
}

inline static void copyTape(Node* t) {

    t->copy = malloc((size_t) p->copyDim);
    memcpy(t->copy, p->copy, (size_t) p->copyDim);

}


inline static void enqueue(transInfo *toEnqueue, int flag, int pos, long int passes, int dim) {

    Node *nuovo;

    if(init==1){
        nuovo = (Node*)malloc(sizeof(Node));
        nuovo->pointer=toEnqueue;
        nuovo->pos=pos;
        nuovo->passes=passes;
        nuovo->copyDim=dim;
        nuovo->copy = malloc((size_t) dim);
        memcpy(nuovo->copy, input, (size_t) dim);
        nuovo->copyDim=dim;
    }
    else if(flag){
        nuovo=p;
        nuovo->pointer=toEnqueue;
        nuovo->pos=pos;
        nuovo->passes=passes;
        nuovo->copyDim=dim;
        nuovo->copy=p->copy;
    }
    else{
        nuovo = (Node*)malloc(sizeof(Node));
        nuovo->pointer=toEnqueue;
        nuovo->pos=pos;
        nuovo->passes=passes;
        nuovo->copyDim=dim;
        copyTape(nuovo);
    }

    if (myQueue == NULL) {
        nuovo->next = myQueue;
        myQueue = nuovo;
        tail=nuovo;
    } else {
        tail->next=nuovo;
        nuovo->next=NULL;
        tail=tail->next;
    }

}

inline static int dequeueHead() {

    if (init==0) {
        if(sonsnum==0){
            free(p->copy);
            free(p);
        }
    }
    p = myQueue;

    for (j = 0; j < accDim+1; j++)
        if (p->pointer->stato_arriv == acceptationStatus[j])
            trovato = 1;

    if (trovato)
        return 1;

    myQueue = myQueue->next;
    return 0;
}

