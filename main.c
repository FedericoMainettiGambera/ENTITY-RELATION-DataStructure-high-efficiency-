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
    struct relationTracker * relationTrackerEntityTreeHead;
};

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
    struct relationTracker * relationTreeHead;
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

    struct relationTracker * fatherEntityTree;
    struct relationTracker * rightNodeEntityTree; // ALBERO 1
    struct relationTracker * leftNodeEntityTree;
    unsigned int nodeColorEntityTree:1;

    struct relationTracker * fatherRelationTree;
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


/*FUNCTIONS*/

struct hashCell{
    struct hashChain * hashChain;
};

struct hashChain{
    struct hashChain * nextHashChain;
    struct entity * entity;
};

struct hashCell entityHashTable[ENTITY_HASHTABLE_DIMENSION];
int numberOfEntityInHashTable = 0;

struct relationTracker nullRelationTracker;


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
struct relationTracker *  searchRelationTracker_1(char * idRelation, struct entity * entity);
void leftRotateRelationTracker_1(struct relationTracker * originNode, struct entity * entity);
void rightRotateRelationTracker_1(struct relationTracker * originNode, struct entity * entity);
void insertRelationTracker_1(struct entity * entity, struct relationTracker * newNode);
void insertRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * newNode);
struct relationTracker * minimumRelationTracker_1(struct relationTracker * node);
struct relationTracker * maximumRelationTracker_1(struct relationTracker * node);
struct relationTracker * successorRelationTracker_1(struct relationTracker * node);
struct relationTracker * predecessorRelationTracker_1(struct relationTracker * node);
void deleteRelationTracker_1(struct entity * entity, struct relationTracker * node);
void deleteRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * node);


/* entity (quadrato) */
// è in una hash table
// stringa che la rappresenta
// riferimento all'albero delle sue relation tracker
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

    struct hashChain * currentElement = entityHashTable[hash].hashChain;
    while(currentElement != NULL){
        if(strcmp(currentElement->entity->name, entity) == 0){
            printf("entity %s found in the hash table\n", entity);
            return currentElement->entity;
        }
        currentElement = currentElement->nextHashChain;
    }
    printf("entity %s not found in the hash table\n", entity);
    return NULL;
}

void insertEntityHashTable(char entity[STRING_DIMENSION]){
    unsigned int hash = SDBMHash(entity) % ENTITY_HASHTABLE_DIMENSION;

    if(entityHashTable[hash].hashChain == NULL){
        //FIRST TIME ADD ELEMENT IN THAT HASH CELL
        entityHashTable[hash].hashChain = malloc(sizeof(struct hashChain));
        (*entityHashTable[hash].hashChain).nextHashChain = NULL;
        (*entityHashTable[hash].hashChain).entity = malloc(sizeof(struct entity));
        (*entityHashTable[hash].hashChain).entity->relationTrackerEntityTreeHead = &nullRelationTracker;
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
        //ADD ELEMENT TO THE END OF THE HASH CHAIN
        numberOfEntityInHashTable++;
        struct hashChain * new = malloc(sizeof(struct hashChain));
        new->nextHashChain = NULL;
        new->entity = malloc(sizeof(struct entity));
        memcpy(new->entity->name, entity, sizeof(char[STRING_DIMENSION]));
        new->entity->relationTrackerEntityTreeHead = &nullRelationTracker;
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

struct relation * searchRelation(char * idRel){

}

void addRelation(){

}

void deleteRelation(){

}

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


// ALBERO 1

struct relationTracker *  searchRelationTracker_1(char * idRelation, struct entity * entity){
    struct relationTracker * result = entity->relationTrackerEntityTreeHead;

    while (result != &nullRelationTracker) {
        int cmp = strcmp(result->entity->name, idRelation);

        if (cmp == 0) {
            printf("relation %s found\n", idRelation);
            return result;
        } else if (cmp < 0) {
            //go left
            result = result->leftNodeEntityTree;
        } else {
            //go right
            result = result->rightNodeEntityTree;
        }
    }
    printf("could'n find the relation %s\n", idRelation);
    return result;
}

void leftRotateRelationTracker_1(struct relationTracker * originNode, struct entity * entity){
    struct relationTracker * temp = originNode->rightNodeEntityTree;
    originNode->rightNodeEntityTree = temp->leftNodeEntityTree;

    if(temp->leftNodeEntityTree != &nullRelationTracker){
        temp->leftNodeEntityTree->fatherEntityTree = originNode;
    }
    temp->fatherEntityTree = originNode->fatherEntityTree;

    if(originNode->fatherEntityTree == &nullRelationTracker){
        entity->relationTrackerEntityTreeHead = temp;
    }
    else if(originNode == originNode->fatherEntityTree->leftNodeEntityTree){
        originNode->fatherEntityTree->leftNodeEntityTree = temp;
    }
    else{
        originNode->fatherEntityTree->rightNodeEntityTree = temp;
    }
    temp->leftNodeEntityTree = originNode;
    originNode->fatherEntityTree = temp;
}

void rightRotateRelationTracker_1(struct relationTracker * originNode, struct entity * entity){
    struct relationTracker * temp = originNode->leftNodeEntityTree;
    originNode->leftNodeEntityTree = temp->rightNodeEntityTree;

    if(temp->rightNodeEntityTree != &nullRelationTracker){
        temp->rightNodeEntityTree->fatherEntityTree = originNode;
    }
    temp->fatherEntityTree = originNode->fatherEntityTree;

    if(originNode->fatherEntityTree == &nullRelationTracker){
        entity->relationTrackerEntityTreeHead = temp;
    }
    else if(originNode == originNode->fatherEntityTree->rightNodeEntityTree){
        originNode->fatherEntityTree->rightNodeEntityTree = temp;
    }
    else{
        originNode->fatherEntityTree->leftNodeEntityTree = temp;
    }
    temp->rightNodeEntityTree = originNode;
    originNode->fatherEntityTree = temp;
}

void insertRelationTracker_1(struct entity * entity, struct relationTracker * newNode) {
    struct relationTracker * tempFather = &nullRelationTracker;
    struct relationTracker * tempNewNode = entity->relationTrackerEntityTreeHead;
    while(tempNewNode != &nullRelationTracker){
        tempFather = tempNewNode;
        if(strcmp(newNode->relation->name, tempNewNode) < 0){
            tempNewNode = tempNewNode->leftNodeEntityTree;
        }
        else{
            tempNewNode = tempNewNode->rightNodeEntityTree;
        }
    }
    newNode->fatherEntityTree = tempFather;
    if(tempFather == &nullRelationTracker){
        entity->relationTrackerEntityTreeHead = newNode;
    }
    else if(strcmp(newNode->relation->name, tempFather->relation->name) < 0){
        tempFather->leftNodeEntityTree = newNode;
    }
    else{
        tempFather->rightNodeEntityTree = newNode;
    }
    newNode->leftNodeEntityTree = &nullRelationTracker;
    newNode->rightNodeEntityTree = &nullRelationTracker;
    newNode->nodeColorEntityTree = RED;
    insertRelationTracker_1_FIXUP(entity, newNode);
    return;
}


void insertRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * newNode){
    struct relationTracker * tempFather = &nullRelationTracker;
    struct relationTracker * tempNewNode = entity->relationTrackerEntityTreeHead;
    if(newNode = entity->relationTrackerEntityTreeHead){
        newNode->nodeColorEntityTree = BLACK;
    }
    else{
        tempNewNode = newNode->fatherEntityTree;
        if(newNode->nodeColorEntityTree == RED){
            if(newNode = newNode->fatherEntityTree->leftNodeEntityTree){
                tempFather = newNode->fatherEntityTree->rightNodeEntityTree;
                if(tempFather->nodeColorEntityTree == RED){
                    tempNewNode->nodeColorEntityTree = BLACK;
                    tempFather->nodeColorEntityTree = BLACK;
                    tempNewNode->fatherEntityTree->nodeColorEntityTree = RED;
                    insertRelationTracker_1_FIXUP(entity, tempNewNode->fatherEntityTree);
                }
                else{
                    if(newNode == tempNewNode->rightNodeEntityTree) {
                        newNode = tempNewNode;
                        leftRotateRelationTracker_1(entity, newNode);
                        tempNewNode = newNode->fatherEntityTree;
                    }
                    tempNewNode->nodeColorEntityTree = BLACK;
                    tempNewNode->fatherEntityTree->nodeColorEntityTree = RED;
                    rightRotateRelationTracker_1(entity, tempNewNode->fatherEntityTree);
                }
            }
            else{
                tempFather = newNode->fatherEntityTree->leftNodeEntityTree;
                if(tempFather->nodeColorEntityTree == RED){
                    tempNewNode->nodeColorEntityTree = BLACK;
                    tempFather->nodeColorEntityTree = BLACK;
                    tempNewNode->fatherEntityTree->nodeColorEntityTree = RED;
                    insertRelationTracker_1_FIXUP(entity, tempNewNode->fatherEntityTree);
                }
                else{
                    if(newNode == tempNewNode->leftNodeEntityTree) {
                        newNode = tempNewNode;
                        rightRotateRelationTracker_1(entity, newNode);
                        tempNewNode = newNode->fatherEntityTree;
                    }
                    tempNewNode->nodeColorEntityTree = BLACK;
                    tempNewNode->fatherEntityTree->nodeColorEntityTree = RED;
                    leftRotateRelationTracker_1(entity, tempNewNode->fatherEntityTree);
                }
            }
        }
    }
}

struct relationTracker * minimumRelationTracker_1(struct relationTracker * node){
    while(node->leftNodeEntityTree != &nullRelationTracker){
        node = node->leftNodeEntityTree;
    }
    return  node;
}

struct relationTracker * maximumRelationTracker_1(struct relationTracker * node){
    while(node->rightNodeEntityTree != &nullRelationTracker){
        node = node->rightNodeEntityTree;
    }
    return  node;
}

struct relationTracker * successorRelationTracker_1(struct relationTracker * node){
    struct relationTracker * temp;
    if(node->rightNodeEntityTree != &nullRelationTracker){
        return minimumRelationTracker_1(node->rightNodeEntityTree);
    }
    temp = node->fatherEntityTree;
    while(temp != &nullRelationTracker && node == temp->rightNodeEntityTree){
        node = temp;
        temp = temp->fatherEntityTree;
    }
    return temp;
}

struct relationTracker * predecessorRelationTracker_1(struct relationTracker * node){
    struct relationTracker * temp;
    if(node->leftNodeEntityTree != &nullRelationTracker){
        return maximumRelationTracker_1(node->rightNodeEntityTree);
    }
    temp = node->fatherEntityTree;
    while(temp != &nullRelationTracker && node == temp->leftNodeEntityTree){
        node = temp;
        temp = temp->fatherEntityTree;
    }
    return temp;
}

void deleteRelationTracker_1(struct entity * entity, struct relationTracker * node){
    struct relationTracker * x;
    struct relationTracker * y;

    if(node->leftNodeEntityTree == &nullRelationTracker || node->rightNodeEntityTree == &nullRelationTracker){
        y = node;
    }
    else{
        y = successorRelationTracker_1(node);
    }
    if(y->leftNodeEntityTree != &nullRelationTracker){
        x = y->leftNodeEntityTree;
    }
    else{
        x = y->rightNodeEntityTree;
    }
    x->fatherEntityTree = y->fatherEntityTree;
    if(y->fatherEntityTree == &nullRelationTracker){
        entity->relationTrackerEntityTreeHead = x;
    }
    else if(y == y->fatherEntityTree->leftNodeEntityTree){
        y->fatherEntityTree->leftNodeEntityTree = x;
    }
    else{
        y->fatherEntityTree->rightNodeEntityTree = x;
    }
    if( y != node){
        node->relation = y->relation;
        node->entity = y->entity;
        node->nextEqualNode = y->nextEqualNode;
        node->numberOfRelationReceived = y->numberOfRelationReceived;
        node->receivedFromTreeHead = y->receivedFromTreeHead;
        node->sentToTreeHead = y->sentToTreeHead;
    }
    if(y->nodeColorEntityTree == BLACK){
        deleteRelationTracker_1_FIXUP(entity, x);
    }
    return;
}

void deleteRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * node){
    struct relationTracker * temp;
    if(node->nodeColorEntityTree == RED || node->fatherEntityTree == &nullRelationTracker){
        node->nodeColorEntityTree = BLACK;
    }
    else if(node == node->fatherEntityTree->leftNodeEntityTree){
        temp = node->fatherEntityTree->rightNodeEntityTree;
        if(temp->nodeColorEntityTree == RED){
            temp->nodeColorEntityTree = BLACK;
            node->fatherEntityTree->nodeColorEntityTree = RED;
            leftRotateRelationTracker_1(node->fatherEntityTree,entity);
            temp = node->fatherEntityTree->rightNodeEntityTree;
        }
        if((temp->nodeColorEntityTree = BLACK) && (temp->rightNodeEntityTree->nodeColorEntityTree == BLACK)){
            temp->nodeColorEntityTree = RED;
            deleteRelationTracker_1_FIXUP(entity, node->fatherEntityTree);
        }
        else{
            if(temp->rightNodeEntityTree->nodeColorEntityTree == BLACK){
                temp->leftNodeEntityTree->nodeColorEntityTree = BLACK;
                temp->nodeColorEntityTree = RED;
                rightRotateRelationTracker_1(temp, entity);
                temp = node->fatherEntityTree->rightNodeEntityTree;
            }
            temp->nodeColorEntityTree = node->fatherEntityTree->nodeColorEntityTree;
            node->fatherEntityTree->nodeColorEntityTree = BLACK;
            temp->rightNodeEntityTree->nodeColorEntityTree = BLACK;
            leftRotateRelationTracker_1(node->fatherEntityTree, entity);
        }
    }
    else{
        temp = node->fatherEntityTree->leftNodeEntityTree;
        if(temp->nodeColorEntityTree == RED){
            temp->nodeColorEntityTree = BLACK;
            node->fatherEntityTree->nodeColorEntityTree = RED;
            rightRotateRelationTracker_1(entity, node->fatherEntityTree);
            temp = node->fatherEntityTree->leftNodeEntityTree;
        }
        if((temp->nodeColorEntityTree = BLACK) && (temp->leftNodeEntityTree->nodeColorEntityTree == BLACK)){
            temp->nodeColorEntityTree = RED;
            deleteRelationTracker_1_FIXUP(entity, node->fatherEntityTree);
        }
        else{
            if(temp->leftNodeEntityTree->nodeColorEntityTree == BLACK){
                temp->rightNodeEntityTree->nodeColorEntityTree = BLACK;
                temp->nodeColorEntityTree = RED;
                leftRotateRelationTracker_1(temp, entity);
                temp = node->fatherEntityTree->leftNodeEntityTree;
            }
            temp->nodeColorEntityTree = node->fatherEntityTree->nodeColorEntityTree;
            node->fatherEntityTree->nodeColorEntityTree = BLACK;
            temp->leftNodeEntityTree->nodeColorEntityTree = BLACK;
            rightRotateRelationTracker_1(node->fatherEntityTree, entity);
        }
    }
}

//ALBERO 2
//todo



/* relation sent to (rombo1) */
// nota: è accessibile dall'entity (che si sta eliminando)
// è in un albero ordinato in base all'indirizzo di memeoria della relation tracker receiver e con posibilità di essere iterato
// riferimento alla relation tracker receiver


/* relation received from (rombo2) */
// nota: è accessibile dall'entity destinataria
// è in un albero ordinato in base all'indirizzo di memoria della relation tracker origin e con possibilità di essere iterato
// riferimento alla relation tracker origin



/* main */
int main(void) {

    //initilizing hash table
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION ; ++i) {
        entityHashTable[i].hashChain = NULL;
    }

    char input[STRING_DIMENSION];

    while(true){
        scanf("%s", input);
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
    scanf("%s", idEnt);
    printf("idEnt: %s - hash: %d\n", idEnt, SDBMHash(idEnt)%ENTITY_HASHTABLE_DIMENSION);

    insertEntityHashTable(idEnt);

    return;

    //O(1):  aggiunge entity all'hash table delle entities se non esiste già
    //
    //TOTAL: O(1)
}

void addrel(){

    scanf("%s", idOrig);
    printf("idOrig: %s\n", idOrig);
    scanf("%s", idDest);
    printf("idDest: %s\n", idDest);
    scanf("%s", idRel);
    printf("idRel: %s\n", idRel);

    struct entity * entityOrig = searchEntityHashTable(idOrig);
    if(entityOrig == NULL){
        printf("entity %s doesn't exists\n", idOrig);
        return;
    }
    struct entity * entityDest = searchEntityHashTable(idDest);
    if(entityDest == NULL){
        printf("entity %s doesn't exists\n", idDest);
        return;
    }

    struct relationTracker * relTrackerOrig = searchRelationTracker_1(idRel, entityOrig);

    struct relationTracker * relTrackerDest;
    if(relTrackerOrig == &nullRelationTracker){
        relTrackerOrig = malloc(sizeof(struct relationTracker));
        relTrackerOrig->leftNodeEntityTree = &nullRelationTracker;
        relTrackerOrig->rightNodeEntityTree = &nullRelationTracker;
        relTrackerOrig->fatherEntityTree = &nullRelationTracker;
        relTrackerOrig->sentToTreeHead = NULL;
        relTrackerOrig->receivedFromTreeHead = NULL;
        relTrackerOrig->entity = entityOrig;

        relTrackerDest = malloc(sizeof(struct relationTracker));
        relTrackerDest->leftNodeEntityTree = &nullRelationTracker;
        relTrackerDest->rightNodeEntityTree = &nullRelationTracker;
        relTrackerDest->fatherEntityTree = &nullRelationTracker;
        relTrackerDest->sentToTreeHead = NULL;
        relTrackerDest->receivedFromTreeHead = NULL;
        relTrackerDest->entity = entityOrig;

        //provvisorio:
        relTrackerOrig->relation = malloc(sizeof(struct relation));
        strcpy(relTrackerOrig->relation->name,idRel);
        relTrackerDest->relation = malloc(sizeof(struct relation));
        strcpy(relTrackerDest->relation->name,idRel);

        insertRelationTracker_1(entityOrig, relTrackerOrig);
        insertRelationTracker_1(entityDest, relTrackerDest);
        printf("added relation %s to entities %s and %s\n", idRel, idOrig, idDest);
    }
    else {
        relTrackerDest = searchRelationTracker_1(idRel, entityDest);
        printf("relation %s already exists in entities %s and %s\n", idRel, idOrig, idDest);
    }

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

    scanf("%s", idEnt);
    printf("idEnt: %s - hash: %d\n", idEnt, SDBMHash(idEnt)%ENTITY_HASHTABLE_DIMENSION);

    deleteEntityHashTable(idEnt);

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
    scanf("%s", idOrig);
    printf("idOrig: %s\n", idOrig);
    scanf("%s", idDest);
    printf("idDest: %s\n", idDest);
    scanf("%s", idRel);
    printf("idRel: %s\n", idRel);

    struct entity * entityOrig = searchEntityHashTable(idOrig);
    if(entityOrig == NULL){
        printf("entity %s doesn't exists\n", idOrig);
        return;
    }
    struct entity * entityDest = searchEntityHashTable(idDest);
    if(entityDest == NULL){
        printf("entity %s doesn't exists\n", idDest);
        return;
    }

    struct relationTracker * relTrackerOrig = searchRelationTracker_1(idRel, entityOrig->relationTrackerEntityTreeHead);
    struct relationTracker *relTrackerDest;
    if(relTrackerOrig == &nullRelationTracker){
        printf("relation %s doesn't exists in entities %s and %s\n", idRel, idOrig, idDest);
        return;
    }
    relTrackerDest = searchRelationTracker_1(idRel, entityDest->relationTrackerEntityTreeHead);

    deleteRelationTracker_1(entityOrig, relTrackerOrig);
    deleteRelationTracker_1(entityDest, relTrackerDest);
    printf("relation %s deleted from entities %s and %s", idRel, idOrig, idDest);

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


void printAllRelationRecursive(struct relationTracker * x);
void printAllRelationRecursive(struct relationTracker * x){
    if(x != &nullRelationTracker){
        printAllRelationRecursive(x->leftNodeEntityTree);
        printf("\t\t%s\n", x->relation->name);
        printAllRelationRecursive(x->rightNodeEntityTree);
    }
}
void report(){
    printf("command: report\n");

    int numberOfEntityPrinted = 0;
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION ; ++i) {
        if(entityHashTable[i].hashChain != NULL){
            printf("hash table cell number [%d]\n", i);

            struct hashChain * tempEnt = entityHashTable[i].hashChain;
            while(tempEnt != NULL){
                printf("\tentity: %s\n", tempEnt->entity->name);
                numberOfEntityPrinted++;
                printAllRelationRecursive(tempEnt->entity->relationTrackerEntityTreeHead);
                tempEnt= tempEnt->nextHashChain;
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
