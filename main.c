#include<stdio.h>

#define TRUE  1
#define FALSE 0

#define RED 1
#define BLACK 0

#define STRING_DIMENSION 100
#define ENTITY_HASHTABLE_DIMENSION 10000

typedef enum { false, true } bool;

char idEnt[STRING_DIMENSION];
char idOrig[STRING_DIMENSION];
char idDest[STRING_DIMENSION];
char idRel[STRING_DIMENSION];

/* entity (quadrato) */
// è in una hash table
// stringa che la rappresenta
// riferimento all'albero delle sue relation tracker
struct entity{
    char name[STRING_DIMENSION];
    struct relationTracker * entityTreeHead;
    struct entity * nextEntityWithSameHash;
};

struct entity * entityHashTable[ENTITY_HASHTABLE_DIMENSION];

unsigned int SDBMHash(char* str) {
    unsigned int hash = 0;
    unsigned int i = 0;

    while(true){
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
        if(str[i] == '\0'){
            break;
        }
        str++;
        i++;
    }

    return hash;
}

void insertEntityHashTable(char entity[STRING_DIMENSION]){
    struct entity * entityToAdd;
    entityToAdd = (struct entity *) malloc(sizeof(struct entity));
    * entityToAdd->name = entity;

    unsigned int hash = SDBMHash(entity);
    if(entityHashTable[hash] == NULL){
        entityHashTable[hash] = entityToAdd;
    }
    else{ //collisione
        //cerca l'elemento nella lista, se lo trova non fa nulla, se non lo trova aggiunge l'elemento in cima alla lista
        struct entity * currentEntity = entityHashTable[hash];
        while( currentEntity != NULL ){
            currentEntity = currentEntity->nextEntityWithSameHash;
            if(strcmp(* currentEntity->name, entity) == 0){
                //l'entity esiste già, non far nulla
                return;
            }
        }
        //l'elemento non esiste, dobbiamo aggiungerlo
        currentEntity = entityHashTable[hash]->nextEntityWithSameHash;
        entityHashTable[hash]->nextEntityWithSameHash = entityToAdd;
        entityToAdd->nextEntityWithSameHash = currentEntity;
    }
}

void deleteEntityHashTable(char entity[STRING_DIMENSION]){

}

struct entity * searchEntityHashTable(char entity[STRING_DIMENSION]){

}

/* relation (stella) */
// è in un albero ordinato alfanumericamente e con possibiltà di essere iterato
// stringa che rappresenta la relazione
// riferimento all'albero 2 di relation tracker
struct relation {
    char name[STRING_DIMENSION];
    struct relation * leftNode;
    struct relation * rightNode;
    unsigned int nodeColor:1;
    struct relationReceivedFrom * graterNumber;
};

/* relation tracker (rettangolo) */
// è in due alberi:
//      albero 1- ordinato alfanumericamente e raccolto in base all'entity origin che rappresenta
//      albero 2- ordinato secondo il numero massimo di relation received e raccolto in base alla relation che rappresenta
// riferimento all'entity che rarppresenta
// riferimento alla relation che rappresenta
// riferimento nodo destro e sinistro per albero 1
// riferimento nodo destro e sinistro per albero 2 + possibile riferimento impilato (stesso numero di received appartengono allo stesso nodo)
// riferimento all'albero delle ralation sent to, cioè delle entities a cui invia la relation
// riferimento all'albero delle relation received from, cioè delle entities da cui riceve la relation
// numero di relation received
struct relationTracker{
    struct entity * entity;
    struct relation * relation;

    struct relationTracker * rightNodeEntityTree; // ALBERO 1
    struct relationTracker * leftNodeEntityTree;
    unsigned int nodeColorEntityTree:1;

    struct relationTracker * rightNodeRelationTree; // ALBERO 2
    struct relationTracker * leftNodeRelationTree;
    struct relationTracker * nextEqualNode;
    unsigned int nodeColorRelationTree:1;

    struct relationSentTo * sentToTreeHead;
    struct relationReceivedFrom * receivedFromTreeHead;

    unsigned int numberOfRelationReceived;
};

/* relation sent to (rombo1) */
// nota: è accessibile dall'entity (che si sta eliminando)
// è in un albero ordinato in base all'indirizzo di memeoria della relation tracker receiver e con posibilità di essere iterato
// riferimento alla relation tracker receiver
struct relationSentTo{
    struct relationSentTo * rightNode;
    struct relationSentTo * leftNode;
    unsigned int nodeColor:1;

    struct relationTracker * receiver;
};

/* relation received from (rombo2) */
// nota: è accessibile dall'entity destinataria
// è in un albero ordinato in base all'indirizzo di memoria della relation tracker origin e con possibilità di essere iterato
// riferimento alla relation tracker origin
struct relationReceivedFrom{
    struct relationReceivedFrom * rightNode;
    struct relationReceivedFrom * leftNode;
    unsigned int nodeColor:1;

    struct relationTracker * origin;
};

/* prototipies */
void addrel();
void addent();
void delent();
void delrel();
void report();
void insertEntityHashTable(char entity[STRING_DIMENSION]);
void deleteEntityHashTable(char entity[STRING_DIMENSION]);
struct entity * searchEntityHashTable(char entity[STRING_DIMENSION]);
unsigned int SDBMHash(char* str);


/* main */
int main(void) {
    char input[10];
    while(true){
        scanf("%s", &input);

        if(input[0] == 'a'){
            if(input[3] == 'e'){
                //addent
                addent();
            }
            else{
                //addrel
                addrel();
            }
        }
        else if(input[0] == 'd'){
            if(input[3] == 'e'){
                //delent
                delent();
            }
            else{
                //delrel
                delrel();
            }
        }
        else if(input[0] == 'r'){
            //report
            report();
        }
        else{
            //end
            return 0;
        }
    }
}

void addent(){
    printf("command: addent\n");

    scanf("%s", &idEnt);
    printf("idEnt: %s\n", idEnt);

    //O(1):  aggiunge entity all'hash table delle entities se non esiste già
    //
    //TOTAL: O(1)

    insertEntityHashTable(idEnt);

}

void addrel(){
    printf("comand: addrel\n");

    scanf("%s", &idOrig);
    printf("idOrig: %s\n", idOrig);
    scanf("%s", &idDest);
    printf("idDest: %s\n", idDest);
    scanf("%s", &idRel);
    printf("idRel: %s\n", idRel);

    //O(1) + O(1):         cerca se le entity origin e receiver esistono nell'hash table delle entities,
    //                     se almeno una non esiste non fa nulla
    //O(log k) + O(log k): una volta trovate accede all'albero 1 delle relation tracker e cerca la relazione interessata per entrambe le entities,
    //                     se non esistono crea il nodo per la relation tracker
    //O(log n):            per l'entity origin accede all'albero delle entity sent to e lo aggiorna aggiungendo la relation tracker receiver
    //O(log n) + O(log j): per l'entity receiver si aumenta di uno il numero di relation received, si sistema l'albero 2 delle relation tracker
    //                     e in seguito si accede all'albero delle entity received from aggiornandolo con la relation tracker origin
    //
    //TOTAL:               O(1) + O(1) + O(log k) + O(log k) + O(log n) + O(log n) + O(log j)
    // k= numero di relation tracker dell'entity considerata
    // n= dimesione dell'albero delle relation sent to o relation received from
    // j= dimensione dell'albero 2 di relation tracker della relation considerata
}

void delent(){
    printf("comand: delent\n");

    scanf("%s", &idEnt);
    printf("idEnt: %s\n", idEnt);

    //O(1):                 cerca se la entity esiste nell'hash table delle entities, se non esiste non fa nulla
    //O(k):                 una volta trovata accede all'albero 1 delle relation tracker e per ognuna di queste:
    //O(n):                          accede all'albero delle entity sent to e per ognuna di queste:
    //O(log n) + O(log j):                       accede alla relation tracker del destinatario, dal relation tracker del destinatario entra nell'albero
    //                                           delle relation received from, cerca quella corretta, cioè con origine l'entity considerata e la elimina;
    //                                           in seguito aggiorna il numero di relation received del relation tracker del destinatario e sistema l'albero 2
    //O(n):                          accede all'albero delle entity received from e per ognuna di queste:
    //O(log n):                                  accede alla relation tracker dell'entity originaria, da qui entra nell'albero delle relation sent to,
    //                                           cerca quella corretta, cioè quella con l'entity considerata e la elimina,
    //
    //TOTAL:                O(1) + O(k) * ( O(n) * ( O(log n) + O(log j) ) + O(n) * ( O(log n) ) )
    // k= numero di relation tracker dell'entity considerata
    // n= dimesione dell'albero delle relation sent to o relation received from
    // j= dimensione dell'albero 2 di relation tracker della relation considerata
}

void delrel(){
    printf("comand: delrel\n");

    scanf("%s", &idOrig);
    printf("idOrig: %s\n", idOrig);
    scanf("%s", &idDest);
    printf("idDest: %s\n", idDest);
    scanf("%s", &idRel);
    printf("idRel: %s\n", idRel);

    //O(1):     cerca se le entities esistono nell'hash table, se non esistono non fa nulla
    //O(log k): dalla entity origin accede all'albero 1 delle relation tracker e cerca la relation da eliminare, se non esiste non fa nulla
    //O(log n): una volta trovata la relation, accede all'albero delle entity sent to e cerca quella corretta
    //O(log n): una volta trovata, accede alla relation tracker del destinatario e cerca nell'albero delle relation received from il nodo da eliminare
    //
    //TOTAL:    O(1) + O(log k) + O(log n) + O(log n)
    // k= numero di relation tracker dell'entity considerata
    // n= dimesione dell'albero delle relation sent to o relation received from
    // j= dimensione dell'albero 2 di relation tracker della relation considerata
}

void report(){
    printf("comand: report\n");
    //O(k):  itera in ogni relation e stampa il primo blocco
    //
    //TOTAL: O(k)
    // k= numero totale di tipi di relazioni esistenti
}
