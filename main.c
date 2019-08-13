#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TRUE  1
#define FALSE 0

#define RED 1
#define BLACK 0

#define STRING_DIMENSION 400
#define ENTITY_HASHTABLE_DIMENSION 30

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
};

struct hashCell{
    struct hashChain * hashChain;
};

struct hashChain{
    struct hashChain * nextHashChain;
    struct entity * entity;
};

struct hashCell entityHashTable[ENTITY_HASHTABLE_DIMENSION];
int numberOfEntityInHashTable = 0;

unsigned int SDBMHash(char* str) {
    unsigned int hash = 0;
    unsigned int i = 0;

    while(true){
        if(str[i] == '\0'){
            break;
        }
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
        str++;
        i++;
    }

    return hash;
}

struct entity * searchEntityHashTable(char entity[STRING_DIMENSION]){
    unsigned int hash = SDBMHash(entity) % ENTITY_HASHTABLE_DIMENSION;

    if(entityHashTable[hash].hashChain == NULL){
        //NOT FOUND
        printf("entity not found\n");
        return NULL;
    }
    else{
        struct hashChain currentHashChainElement = *(entityHashTable[hash].hashChain);
        while(currentHashChainElement.nextHashChain != NULL){
            if(strcmp(currentHashChainElement.entity->name, entity) == 0){
                //FOUND
                printf("entity found\n");
                return currentHashChainElement.entity;
            }
            currentHashChainElement = *currentHashChainElement.nextHashChain;
        }
        //NOT FOUND
        printf("entity not found\n");
        return NULL;
    }
}

void insertEntityHashTable(char entity[STRING_DIMENSION]){
    unsigned int hash = SDBMHash(entity) % ENTITY_HASHTABLE_DIMENSION;

    if(entityHashTable[hash].hashChain == NULL){
        //FIRST TIME ADD ELEMENT IN THAT HASH CELL
        entityHashTable[hash].hashChain = malloc(sizeof(struct hashChain));
        (*entityHashTable[hash].hashChain).nextHashChain = NULL;
        (*entityHashTable[hash].hashChain).entity = malloc(sizeof(struct entity));
        memcpy((*(*entityHashTable[hash].hashChain).entity).name, entity, sizeof(char[STRING_DIMENSION]));
        numberOfEntityInHashTable++;
        printf("entity added as first ever element of the hash chain\n");
        return;
    }
    else{
        struct hashChain * currentHashChainElement = entityHashTable[hash].hashChain;
        while((*currentHashChainElement).nextHashChain != NULL){
            if(strcmp((*currentHashChainElement).entity->name, entity) == 0){
                //ALREADY EXISTS
                printf("entity already exists, not adding\n");
                return;
            }
            currentHashChainElement = currentHashChainElement->nextHashChain;
        }
        if(strcmp(currentHashChainElement->entity->name, entity) == 0){
            //ALREADY EXISTS
            printf("entity already exists, not adding\n");
            return;
        }
        //ADD ELEMENT TO THE HASH CHAIN
        numberOfEntityInHashTable++;
        struct hashChain * new = malloc(sizeof(struct hashChain));
        new->nextHashChain = NULL;
        new->entity = malloc(sizeof(struct entity));
        memcpy(new->entity->name, entity, sizeof(char[STRING_DIMENSION]));
        currentHashChainElement->nextHashChain = new;
        printf("entity added as last element of the hash chain\n");
        return;
    }
}

void deleteEntityHashTable(char entity[STRING_DIMENSION]){
    unsigned int hash = SDBMHash(entity) % ENTITY_HASHTABLE_DIMENSION;

    if(entityHashTable[hash].hashChain == NULL){
        //ELEMENT DOESN'T EXISTS
        printf("entity doesn't exists, the hash chain is empty\n");
        return;
    }
    else{
        struct hashChain * currentHashChainElement = entityHashTable[hash].hashChain;

        //check if it is the first element
        if(strcmp((*currentHashChainElement).entity->name, entity) == 0) {
            //ELEMENT FOUND, DELETE AND FREE MEMORY
            entityHashTable[hash].hashChain = (*currentHashChainElement).nextHashChain;
            free(currentHashChainElement);
            numberOfEntityInHashTable--;
            printf("entity deleted, it was the first element in the hash chain\n");
            return;
        }

        //set previous and current to iterate the chain
        struct hashChain * previousHashChainElement = entityHashTable[hash].hashChain;
        if((*currentHashChainElement).nextHashChain != NULL){
            currentHashChainElement = (*currentHashChainElement).nextHashChain;
        }
        else{
            //ELEMENT DOESN'T EXISTS
            printf("entity doesn't exists\n");
            return;
        }

        //iterate the chain
        while(currentHashChainElement->nextHashChain != NULL){
            if (strcmp((*currentHashChainElement).entity->name, entity) == 0) {
                //ELEMENT FOUND, DELETE AND FREE MEMORY
                (*previousHashChainElement).nextHashChain = (*currentHashChainElement).nextHashChain;
                free(currentHashChainElement);
                numberOfEntityInHashTable--;
                printf("entity deleted from the hash chain\n");
                return;
            }
            previousHashChainElement = currentHashChainElement;
            currentHashChainElement = currentHashChainElement->nextHashChain;
        }
        if (strcmp((*currentHashChainElement).entity->name, entity) == 0) {
            //ELEMENT FOUND, DELETE AND FREE MEMORY
            (*previousHashChainElement).nextHashChain = (*currentHashChainElement).nextHashChain;
            free(currentHashChainElement);
            numberOfEntityInHashTable--;
            printf("entity deleted from the hash chain\n");
            return;
        }

        //ELEMENT DOESN'T EXISTS
        printf("entity doesn't exists\n");
        return;
    }
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

    //initilizing hash table
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION ; ++i) {
        entityHashTable[i].hashChain = NULL;
    }

    char input[STRING_DIMENSION];
    while(true){
        scanf("%s", &input);
        if(strcmp(input, "addent") == 0){
            printf("\ncommand: addent *******************************\n");
            addent();
        }
        else if(strcmp(input, "addrel") == 0){
            printf("\ncommand: addrel *******************************\n");
            addrel();
        }
        else if(strcmp(input, "delent") == 0){
            printf("\ncommand: delent *******************************\n");
            delent();
        }
        else if(strcmp(input, "delrel") == 0){
            printf("\ncommand: delrel *******************************\n");
            delrel();
        }
        else if(strcmp(input, "report") == 0){
            printf("\ncommand: report *******************************\n");
            report();
        }
        else { //end
            printf("\ncommand: END *******************************\n");
            report();
            return 0;
        }
    }
}

void addent(){
    scanf("%s", &idEnt);
    printf("idEnt: %s - hash: %d\n", idEnt, SDBMHash(idEnt)%ENTITY_HASHTABLE_DIMENSION);

    //O(1):  aggiunge entity all'hash table delle entities se non esiste già
    //
    //TOTAL: O(1)

    insertEntityHashTable(idEnt);

    return;
}

void addrel(){

    scanf("%s", &idOrig);
    printf("idOrig: %s\n", idOrig);
    scanf("%s", &idDest);
    printf("idDest: %s\n", idDest);
    scanf("%s", &idRel);
    printf("idRel: %s\n", idRel);


    return;

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
    printf("idEnt: %s - hash: %d\n", idEnt, SDBMHash(idEnt)%ENTITY_HASHTABLE_DIMENSION);

    deleteEntityHashTable(idEnt);

    return;
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
    scanf("%s", &idOrig);
    printf("idOrig: %s\n", idOrig);
    scanf("%s", &idDest);
    printf("idDest: %s\n", idDest);
    scanf("%s", &idRel);
    printf("idRel: %s\n", idRel);

    return;

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

    int numberOfEntityPrinted = 0;
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION ; ++i) {
        if(entityHashTable[i].hashChain != NULL){
            printf("[%d] \t -> %s", i, entityHashTable[i].hashChain->entity->name);
            numberOfEntityPrinted++;
            struct hashChain * temp = (*entityHashTable[i].hashChain).nextHashChain;
            while(temp != NULL){
                printf(" -> %s", temp->entity->name);
                numberOfEntityPrinted++;
                temp = temp->nextHashChain;
            }
            printf("\n");
        }
    }

    printf("hash table dimension: %d\n", ENTITY_HASHTABLE_DIMENSION);
    printf("amount of entity stored in the hash table: %d\n", numberOfEntityInHashTable);
    printf("amount of entity printed: %d\n", numberOfEntityPrinted );

    return;
    //O(k):  itera in ogni relation e stampa il primo blocco
    //
    //TOTAL: O(k)
    // k= numero totale di tipi di relazioni esistenti
}
