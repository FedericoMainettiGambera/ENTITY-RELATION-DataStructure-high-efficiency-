#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define RED 1
#define BLACK 0
#define RECEIVED 0
#define SENT 1

#define STRING_DIMENSION 100
#define ENTITY_HASHTABLE_DIMENSION 1237

int numberOfTotalAddrel = 0;
int numberOfTotalDelrel = 0;
int numberOfTotalAddent = 0;
int numberOfTotalDelent = 0;
int numberOfEntityInHashTable = 0;

char idEnt[STRING_DIMENSION];
int idEntLength = 0;
char idOrig[STRING_DIMENSION];
int idOrigLength = 0;
char idDest[STRING_DIMENSION];
int idDestLength = 0;
char idRel[STRING_DIMENSION];
int idRelLenght = 0;


/* entity (quadrato) */
// è in una hash table
// stringa che la rappresenta
// riferimento all'albero delle sue relation tracker

struct entity{
    char * name;
    struct relationTracker_1 * relationTrackerEntityTreeHead;
    struct entity * nextEntity;
};

/* relation (stella) */
// è in un albero ordinato alfanumericamente e con possibiltà di essere iterato
// stringa che rappresenta la relazione
// riferimento all'albero 2 di relation tracker

struct relation {
    char * name;
    struct relation * father;
    struct relation * left;
    struct relation * right;
    int nodeColor:1;
    struct relationTracker_2 * relationTrackerTreeHead;
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

struct relationTracker_1{
    struct entity * entity;
    struct relation * relation;

    struct relationTracker_1 * rightNodeEntityTree; // ALBERO 1
    struct relationTracker_1 * leftNodeEntityTree;
    struct relationTracker_1 * fatherEntityTree;
    int nodeColorEntityTree:1;

    struct relationSentToOrReceivedFrom * sentToTreeHead;
    struct relationSentToOrReceivedFrom * receivedFromTreeHead;

    struct relationTracker_2 * relationTracker_2;
    int numberOfRelationReceived;
};

struct relationTracker_2{
    struct relationTracker_1 * relationTracker_1;

    struct relationTracker_2 * rightNodeRelationTree; // ALBERO 2
    struct relationTracker_2 * leftNodeRelationTree;
    struct relationTracker_2 * fatherRelationTree;
    int nodeColorRelationTree:1;
};

/* relation sent to (rombo1) */
// nota: è accessibile dall'entity (che si sta eliminando)
// è in un albero ordinato in base all'indirizzo di memeoria della relation tracker receiver e con posibilità di essere iterato
// riferimento alla relation tracker receiver
/* relation received from (rombo2) */
// nota: è accessibile dall'entity destinataria
// è in un albero ordinato in base all'indirizzo di memoria della relation tracker origin e con possibilità di essere iterato
// riferimento alla relation tracker origin

struct relationSentToOrReceivedFrom{
    struct relationSentToOrReceivedFrom * rightSentTo;
    struct relationSentToOrReceivedFrom * leftSentTo;
    struct relationSentToOrReceivedFrom * fatherSentTo;
    int nodeColorSentTo:1;
    struct relationSentToOrReceivedFrom * rightReceivedFrom;
    struct relationSentToOrReceivedFrom * leftReceivedFrom;
    struct relationSentToOrReceivedFrom * fatherReceivedFrom;
    int nodeColorReceivedFrom:1;
    struct relationTracker_1 * origRelationTracker;
    struct relationTracker_1 * destRelationTracker;
};



/*declaration*/

struct entity * entityHashTable[ENTITY_HASHTABLE_DIMENSION];

struct relationTracker_1 nullRelationTrackerNode_1;
struct relationTracker_2 nullRelationTrackerNode_2;
struct relation nullRelationNode;
struct relationSentToOrReceivedFrom nullRelationSentToOrReceivedFromNode;

struct relation * relationTreeHead;


/* prototipies */
void addrel();
void addent();
void delent();
void delrel();
void report();

void insertEntityHashTable(char *entity);
void deleteEntityHashTable(char *entity);
struct entity * searchEntityHashTable(char *entity);
unsigned long hashFunction(unsigned char *str);

struct relationTracker_1 *  searchRelationTracker_1(char * idRelation, struct entity * entity);
void leftRotateRelationTracker_1(struct relationTracker_1 * originNode, struct entity * entity);
void rightRotateRelationTracker_1(struct relationTracker_1 * originNode, struct entity * entity);
void insertRelationTracker_1(struct entity * entity, struct relationTracker_1 * newNode);
void insertRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker_1 * newNode);
struct relationTracker_1 * minimumRelationTracker_1(struct relationTracker_1 * node);
struct relationTracker_1 * successorRelationTracker_1(struct relationTracker_1 * node);
void deleteRelationTracker_1(struct entity * entity, struct relationTracker_1 * z);
void deleteRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker_1 * node);

void fixReceivedFromRecursive(struct relationTracker_1 ** node, struct relationSentToOrReceivedFrom * receivedFromTreeHead);
void fixSentToRecursive(struct relationTracker_1 ** node, struct relationSentToOrReceivedFrom * SentToTreeHead);


void leftRotateRelationTracker_2(struct relationTracker_2 * x, struct relation * relation);
void rightRotateRelationTracker_2(struct relationTracker_2 * x, struct relation * relation);
void insertRelationTracker_2(struct relation * relation, struct relationTracker_2 * z);
void insertRelationTracker_2_FIXUP(struct relation * relation, struct relationTracker_2 * newNode);
struct relationTracker_2 * minimumRelationTracker_2(struct relationTracker_2 * x);
struct relationTracker_2 * successorRelationTracker_2(struct relationTracker_2 * x);
void deleteRelationTracker_2(struct relation * relation, struct relationTracker_2 * z);
void deleteRelationTracker_2_FIXUP(struct relation * relation, struct relationTracker_2 * x);


struct relation * searchRelation(char * idRelSearched);
void leftRotateRelation(struct relation * x);
void rightRotateRelation(struct relation * x);
void insertRelation(struct relation * z);
void insertRelation_FIXUP(struct relation * z);
struct relation * minimumRelation(struct relation * x);
struct relation * successorRelation(struct relation * x);
void deleteRelation(struct relation * z);
void deleteRelation_FIXUP(struct relation * x);

struct relationSentToOrReceivedFrom * searchRelationSentTo(char * destId, struct relationSentToOrReceivedFrom * head);
void leftRotateRelationSentTo(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void rightRotateRelationSentTo(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void insertRelationSentTo(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void insertRelationSentTo_FIXUP(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
struct relationSentToOrReceivedFrom * minimumRelationSentTo(struct relationSentToOrReceivedFrom * x);
struct relationSentToOrReceivedFrom * successorRelationSentTo(struct relationSentToOrReceivedFrom * x);
void deleteRelationSentTo(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void deleteRelationSentTo_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);

struct relationSentToOrReceivedFrom * searchRelationReceivedFrom(char * origId, struct relationSentToOrReceivedFrom * head);
void leftRotateRelationReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void rightRotateRelationReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void insertRelationReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void insertRelationReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
struct relationSentToOrReceivedFrom * minimumRelationReceivedFrom(struct relationSentToOrReceivedFrom * x);
struct relationSentToOrReceivedFrom * successorRelationReceivedFrom(struct relationSentToOrReceivedFrom * x);
void deleteRelationReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void deleteRelationReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);

void fullReport();

/* entity (quadrato) */
// è in una hash table
// stringa che la rappresenta
// riferimento all'albero delle sue relation tracker

unsigned long hashFunction(unsigned char *str) {
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash % ENTITY_HASHTABLE_DIMENSION;
}

struct entity * searchEntityHashTable(char * entity){
    unsigned int hash = hashFunction(entity) % ENTITY_HASHTABLE_DIMENSION;

    struct entity * currentEntity = entityHashTable[hash];
    while(currentEntity != NULL){
        if(strcmp(currentEntity->name, entity) == 0){
            //printf("entity %s found in the hash table\n", entity);
            return currentEntity;
        }
        currentEntity = currentEntity->nextEntity;
    }
    //printf("entity %s not found in the hash table\n", entity);
    return NULL;
}

void insertEntityHashTable(char *entity){
    unsigned int hash = hashFunction(entity) % ENTITY_HASHTABLE_DIMENSION;

    struct entity * currentEntity = entityHashTable[hash];

    if(currentEntity == NULL){
        //FIRST TIME ADD ELEMENT IN THAT HASH CELL
        currentEntity = malloc(sizeof(struct entity));
        currentEntity->nextEntity = NULL;
        currentEntity->relationTrackerEntityTreeHead = &nullRelationTrackerNode_1;
        currentEntity->name = malloc(idEntLength);
        memcpy(currentEntity->name, entity, idEntLength);
        numberOfEntityInHashTable++;
        //printf("entity added as first ever element of the hash chain\n");
        entityHashTable[hash] = currentEntity;
        numberOfTotalAddent++;
        return;
    }
    else{
        while(currentEntity != NULL){
            if(strcmp(currentEntity->name, entity) == 0){
                //ALREADY EXISTS
                //printf("entity already exists, not adding\n");
                return;
            }
            currentEntity = currentEntity->nextEntity;
        }
        //ADD ELEMENT TO THE HEAD OF THE HASH CHAIN
        currentEntity = malloc(sizeof(struct entity));
        currentEntity->nextEntity = entityHashTable[hash];
        currentEntity->relationTrackerEntityTreeHead = &nullRelationTrackerNode_1;
        currentEntity->name = malloc(idEntLength);
        memcpy(currentEntity->name, entity, idEntLength);
        entityHashTable[hash] = currentEntity;
        numberOfEntityInHashTable++;
        //printf("entity added as last element of the hash chain\n");
        numberOfTotalAddent++;
        return;
    }
}

void deleteEntityHashTable(char * entity){
    unsigned int hash = hashFunction(entity) % ENTITY_HASHTABLE_DIMENSION;

    struct entity * currentEntity = entityHashTable[hash];

    if(currentEntity == NULL){
        //ELEMENT DOESN'T EXISTS
        //printf("entity doesn't exists, the hash chain is empty\n");
        return;
    }
    else{
        //check if it is the first element
        if(strcmp(currentEntity->name, entity) == 0) {
            //ELEMENT FOUND, DELETE AND FREE MEMORY
            entityHashTable[hash] = currentEntity->nextEntity;
            free(currentEntity->name);
            free(currentEntity);
            numberOfEntityInHashTable--;
            //printf("entity deleted, it was the first element in the hash chain\n");
            return;
        }

        //set previous and current to iterate the chain
        struct entity * previousEntity = entityHashTable[hash];
        if(currentEntity->nextEntity != NULL){
            currentEntity = currentEntity->nextEntity;
        }
        else{
            //ELEMENT DOESN'T EXISTS
            //printf("entity doesn't exists\n");
            return;
        }

        //iterate the chain
        while(currentEntity->nextEntity != NULL){
            if (strcmp(currentEntity->name, entity) == 0) {
                //ELEMENT FOUND, DELETE AND FREE MEMORY
                previousEntity->nextEntity = currentEntity->nextEntity;
                free(currentEntity->name);
                free(currentEntity);
                numberOfEntityInHashTable--;
                //printf("entity deleted from the hash chain\n");
                return;
            }
            previousEntity = currentEntity;
            currentEntity = currentEntity->nextEntity;
        }
        if (strcmp(currentEntity->name, entity) == 0) {
            //ELEMENT FOUND, DELETE AND FREE MEMORY
            previousEntity->nextEntity = NULL;
            free(currentEntity->name);
            free(currentEntity);
            numberOfEntityInHashTable--;
            //printf("entity deleted from the hash chain\n");
            return;
        }

        //ELEMENT DOESN'T EXISTS
        //printf("entity doesn't exists\n");
        return;
    }
}

/* relation (stella) */
// è in un albero ordinato alfanumericamente e con possibiltà di essere iterato
// stringa che rappresenta la relazione
// riferimento all'albero 2 di relation tracker

struct relation * searchRelation(char * idRelSearched){
    struct relation * result = relationTreeHead;

    while (result != &nullRelationNode) {
        int cmp = strcmp(idRelSearched, result->name);

        if (cmp == 0) {
            //printf("relation %s found\n", idRelSearched);
            return result;
        } else if (cmp < 0) {
            //go left
            result = result->left;
        } else {
            //go right
            result = result->right;
        }
    }
    //printf("couldn't find the relation %s\n", idRelSearched);
    return result;
}

void leftRotateRelation(struct relation * x){
    struct relation * y = x->right;
    x->right = y->left;
    if(y->left != &nullRelationNode){
        y->left->father = x;
    }
    y->father = x->father;
    if(x->father == &nullRelationNode){
        relationTreeHead = y;
    }
    else if(x == x->father->left){
        x->father->left = y;
    }
    else{
        x->father->right = y;
    }
    y->left = x;
    x->father = y;
}

void rightRotateRelation(struct relation * x){
    struct relation * y = x->left;
    x->left = y->right;
    if(y->right!= &nullRelationNode){
        y->right->father = x;
    }
    y->father = x->father;
    if(x->father == &nullRelationNode){
        relationTreeHead = y;
    }
    else if(x == x->father->right){
        x->father->right = y;
    }
    else{
        x->father->left = y;
    }
    y->right = x;
    x->father = y;
}

void insertRelation(struct relation * z){
    struct relation * y = &nullRelationNode;
    struct relation * x = relationTreeHead;
    while(x != &nullRelationNode){
        y = x;
        if(strcmp(z->name, x->name) < 0){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    z->father = y;
    if(y == &nullRelationNode){
        relationTreeHead = z;
    }
    else if(strcmp(z->name, y->name) < 0){
        y->left = z;
    }
    else{
        y->right = z;
    }
    z->left = &nullRelationNode;
    z->right = &nullRelationNode;
    z->nodeColor = RED;
    insertRelation_FIXUP(z);
}

void insertRelation_FIXUP(struct relation * z){
    struct relation * y;
    struct relation * x;
    if(z == relationTreeHead){
        relationTreeHead->nodeColor = BLACK;
    }
    else{
        x = z->father;
        if(x->nodeColor == RED){
            if( x == x->father->left){
                y = x->father->right;
                if(y->nodeColor == RED){
                    x->nodeColor = BLACK;
                    y->nodeColor = BLACK;
                    x->father->nodeColor = RED;
                    insertRelation_FIXUP(x->father);
                }
                else{
                    if(z == x->right){
                        z = x;
                        leftRotateRelation(z);
                        x = z->father;
                    }
                    x->nodeColor = BLACK;
                    x->father->nodeColor = RED;
                    rightRotateRelation(x->father);
                }
            }
            else{
                y = x->father->left;
                if(y->nodeColor == RED){
                    x->nodeColor = BLACK;
                    y->nodeColor = BLACK;
                    x->father->nodeColor = RED;
                    insertRelation_FIXUP(x->father);
                }
                else{
                    if(z == x->left){
                        z = x;
                        rightRotateRelation(z);
                        x = z->father;
                    }
                    x->nodeColor = BLACK;
                    x->father->nodeColor = RED;
                    leftRotateRelation(x->father);
                }
            }
        }
    }
}

struct relation * minimumRelation(struct relation * x){
    while (x->left != &nullRelationNode){
        x = x->left;
    }
    return x;
}

struct relation * successorRelation(struct relation * x){
    struct relation * y;
    if(x->right != &nullRelationNode){
        return minimumRelation(x->right);
    }
    y = x->father;
    while((y!= &nullRelationNode) && (x == y->right)){
        x = y;
        y = y->father;
    }
    return y;
}

void deleteRelation(struct relation * z){
    struct relation * y;
    struct relation * x;
    if((z->left == &nullRelationNode) || (z->right == &nullRelationNode)){
        y = z;
    }
    else{
        y = successorRelation(z);
    }
    if(y->left != &nullRelationNode){
        x = y->left;
    }
    else{
        x = y->right;
    }
    x->father = y->father;
    if(y->father == &nullRelationNode){
        relationTreeHead = x;
    }
    else if(y == y->father->left){
        y->father->left = x;
    }
    else{
        y->father->right = x;
    }
    if (y != z){
        strcpy(z->name, y->name);
    }
    if(y->nodeColor == BLACK){
        deleteRelation_FIXUP(x);
    }
}

void deleteRelation_FIXUP(struct relation * x){
    struct relation * w;

    if((x->nodeColor == RED) || (x->father == &nullRelationNode)){
        x->nodeColor = BLACK;
    }
    else if(x == x->father->left){
        w = x->father->right;
        if(w->nodeColor == RED){
            w->nodeColor = BLACK;
            x->father->nodeColor = RED;
            leftRotateRelation(x->father);
            w = x->father->right;
        }
        if((w->left->nodeColor == BLACK) && (w->right->nodeColor == BLACK)){
            w->nodeColor = RED;
            deleteRelation_FIXUP(x->father);
        }
        else{
            if(w->right->nodeColor == BLACK){
                w->left->nodeColor = BLACK;
                w->nodeColor = RED;
                rightRotateRelation(w);
                w = x->father->right;
            }
            w->nodeColor = x->father->nodeColor;
            x->father->nodeColor = BLACK;
            w->right->nodeColor = BLACK;
            leftRotateRelation(x->father);
        }
    }
    else{
        w = x->father->left;
        if(w->nodeColor == RED){
            w->nodeColor = BLACK;
            x->father->nodeColor = RED;
            rightRotateRelation(x->father);
            w = x->father->left;
        }
        if((w->right->nodeColor == BLACK) && (w->left->nodeColor == BLACK)){
            w->nodeColor = RED;
            deleteRelation_FIXUP(x->father);
        }
        else{
            if(w->left->nodeColor == BLACK){
                w->right->nodeColor = BLACK;
                w->nodeColor = RED;
                leftRotateRelation(w);
                w = x->father->left;
            }
            w->nodeColor = x->father->nodeColor;
            x->father->nodeColor = BLACK;
            w->left->nodeColor = BLACK;
            rightRotateRelation(x->father);
        }
    }
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

struct relationTracker_1 *  searchRelationTracker_1(char * idRelation, struct entity * entity){
    struct relationTracker_1 * result = entity->relationTrackerEntityTreeHead;

    while (result != &nullRelationTrackerNode_1) {
        int cmp = strcmp(idRelation, result->relation->name);

        if (cmp == 0) {
            //printf("relationTracker related to the relation %s and the entity %s found\n", idRelation, entity->name);
            return result;
        } else if (cmp > 0) {
            //go left
            result = result->rightNodeEntityTree;
        } else {
            //go right
            result = result->leftNodeEntityTree;
        }
    }
    //printf("couldn't find the relationTracker related to the relation %s and the entity %s\n", idRelation, entity->name);
    return result;
}

void leftRotateRelationTracker_1(struct relationTracker_1 * originNode, struct entity * entity){
    struct relationTracker_1 * temp = originNode->rightNodeEntityTree;
    originNode->rightNodeEntityTree = temp->leftNodeEntityTree;

    if(temp->leftNodeEntityTree != &nullRelationTrackerNode_1){
        temp->leftNodeEntityTree->fatherEntityTree = originNode;
    }
    temp->fatherEntityTree = originNode->fatherEntityTree;

    if(originNode->fatherEntityTree == &nullRelationTrackerNode_1){
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

void rightRotateRelationTracker_1(struct relationTracker_1 * originNode, struct entity * entity){
    struct relationTracker_1 * temp = originNode->leftNodeEntityTree;
    originNode->leftNodeEntityTree = temp->rightNodeEntityTree;

    if(temp->rightNodeEntityTree != &nullRelationTrackerNode_1){
        temp->rightNodeEntityTree->fatherEntityTree = originNode;
    }
    temp->fatherEntityTree = originNode->fatherEntityTree;

    if(originNode->fatherEntityTree == &nullRelationTrackerNode_1){
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

void insertRelationTracker_1(struct entity * entity, struct relationTracker_1 * newNode) {
    struct relationTracker_1 * tempFather = &nullRelationTrackerNode_1;
    struct relationTracker_1 * tempNewNode = entity->relationTrackerEntityTreeHead;
    while(tempNewNode != &nullRelationTrackerNode_1){
        tempFather = tempNewNode;
        if(strcmp(newNode->relation->name, tempNewNode->relation->name) < 0){
            tempNewNode = tempNewNode->leftNodeEntityTree;
        }
        else{
            tempNewNode = tempNewNode->rightNodeEntityTree;
        }
    }
    newNode->fatherEntityTree = tempFather;
    if(tempFather == &nullRelationTrackerNode_1){
        entity->relationTrackerEntityTreeHead = newNode;
    }
    else if(strcmp(newNode->relation->name, tempFather->relation->name) < 0){
        tempFather->leftNodeEntityTree = newNode;
    }
    else{
        tempFather->rightNodeEntityTree = newNode;
    }
    newNode->leftNodeEntityTree = &nullRelationTrackerNode_1;
    newNode->rightNodeEntityTree = &nullRelationTrackerNode_1;
    newNode->nodeColorEntityTree = RED;
    insertRelationTracker_1_FIXUP(entity, newNode);
}

void insertRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker_1 * newNode){
    struct relationTracker_1 * tempFather;
    struct relationTracker_1 * tempNewNode;
    if(newNode == entity->relationTrackerEntityTreeHead){
        newNode->nodeColorEntityTree = BLACK;
    }
    else{
        tempNewNode = newNode->fatherEntityTree;
        if(newNode->nodeColorEntityTree == RED){
            if(newNode == newNode->fatherEntityTree->leftNodeEntityTree){
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
                        leftRotateRelationTracker_1(newNode, entity);
                        tempNewNode = newNode->fatherEntityTree;
                    }
                    tempNewNode->nodeColorEntityTree = BLACK;
                    tempNewNode->fatherEntityTree->nodeColorEntityTree = RED;
                    rightRotateRelationTracker_1(tempNewNode->fatherEntityTree,entity);
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
                        rightRotateRelationTracker_1(newNode, entity);
                        tempNewNode = newNode->fatherEntityTree;
                    }
                    tempNewNode->nodeColorEntityTree = BLACK;
                    tempNewNode->fatherEntityTree->nodeColorEntityTree = RED;
                    leftRotateRelationTracker_1(tempNewNode->fatherEntityTree,entity);
                }
            }
        }
    }
}

struct relationTracker_1 * minimumRelationTracker_1(struct relationTracker_1 * node){
    while(node->leftNodeEntityTree != &nullRelationTrackerNode_1){
        node = node->leftNodeEntityTree;
    }
    return  node;
}

struct relationTracker_1 * successorRelationTracker_1(struct relationTracker_1 * node){
    struct relationTracker_1 * temp;
    if(node->rightNodeEntityTree != &nullRelationTrackerNode_1){
        return minimumRelationTracker_1(node->rightNodeEntityTree);
    }
    temp = node->fatherEntityTree;
    while(temp != &nullRelationTrackerNode_1 && node == temp->rightNodeEntityTree){
        node = temp;
        temp = temp->fatherEntityTree;
    }
    return temp;
}

void fixReceivedFromRecursive(struct relationTracker_1 ** node, struct relationSentToOrReceivedFrom * receivedFromTreeHead){
    if(receivedFromTreeHead != &nullRelationSentToOrReceivedFromNode){
        fixReceivedFromRecursive(node, receivedFromTreeHead->leftReceivedFrom);

        receivedFromTreeHead->destRelationTracker = *node;

        fixReceivedFromRecursive(node, receivedFromTreeHead->rightReceivedFrom);
    }
}

void fixSentToRecursive(struct relationTracker_1 ** node, struct relationSentToOrReceivedFrom * SentToTreeHead){
    if(SentToTreeHead != &nullRelationSentToOrReceivedFromNode){
        fixSentToRecursive(node, SentToTreeHead->leftSentTo);

        SentToTreeHead->origRelationTracker = *node;

        fixSentToRecursive(node, SentToTreeHead->rightSentTo);
    }
}

void deleteRelationTracker_1(struct entity * entity, struct relationTracker_1 * z){
    struct relationTracker_1 * x;
    struct relationTracker_1 * y;

    if(z->leftNodeEntityTree == &nullRelationTrackerNode_1 || z->rightNodeEntityTree == &nullRelationTrackerNode_1){
        y = z;
    }
    else{
        y = successorRelationTracker_1(z);
    }
    if(y->leftNodeEntityTree != &nullRelationTrackerNode_1){
        x = y->leftNodeEntityTree;
    }
    else{
        x = y->rightNodeEntityTree;
    }
    x->fatherEntityTree = y->fatherEntityTree;
    if(y->fatherEntityTree == &nullRelationTrackerNode_1){
        entity->relationTrackerEntityTreeHead = x;
    }
    else if(y == y->fatherEntityTree->leftNodeEntityTree){
        y->fatherEntityTree->leftNodeEntityTree = x;
    }
    else{
        y->fatherEntityTree->rightNodeEntityTree = x;
    }
    if(y != z){
        int oldColor = y->nodeColorEntityTree;
        y->nodeColorEntityTree = z->nodeColorEntityTree;

        if(z == entity->relationTrackerEntityTreeHead){
            entity->relationTrackerEntityTreeHead = y;
        }
        else {
            if (z->fatherEntityTree->leftNodeEntityTree == z) {
                z->fatherEntityTree->leftNodeEntityTree = y;
            } else {
                z->fatherEntityTree->rightNodeEntityTree = y;
            }
        }
        z->leftNodeEntityTree->fatherEntityTree = y;
        z->rightNodeEntityTree->fatherEntityTree = y;

        y->fatherEntityTree = z->fatherEntityTree;
        y->leftNodeEntityTree = z->leftNodeEntityTree;
        y->rightNodeEntityTree = z->rightNodeEntityTree;

        if(oldColor == BLACK){
            deleteRelationTracker_2_FIXUP(entity, x);
        }

        free(z);
        return;
    }
    if(y->nodeColorEntityTree == BLACK){
        deleteRelationTracker_1_FIXUP(entity, x);
    }
    free(y);
}

void deleteRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker_1 * node){
    struct relationTracker_1 * temp;
    if(node->nodeColorEntityTree == RED || node->fatherEntityTree == &nullRelationTrackerNode_1){
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
        if((temp->nodeColorEntityTree == BLACK) && (temp->rightNodeEntityTree->nodeColorEntityTree == BLACK)){
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
            rightRotateRelationTracker_1(node->fatherEntityTree, entity);
            temp = node->fatherEntityTree->leftNodeEntityTree;
        }
        if((temp->nodeColorEntityTree == BLACK) && (temp->leftNodeEntityTree->nodeColorEntityTree == BLACK)){
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

void leftRotateRelationTracker_2(struct relationTracker_2 * x, struct relation * relation){
    struct relationTracker_2 * y = x->rightNodeRelationTree;
    x->rightNodeRelationTree = y->leftNodeRelationTree;

    if(y->leftNodeRelationTree != &nullRelationTrackerNode_2){
        y->leftNodeRelationTree->fatherRelationTree = x;
    }
    y->fatherRelationTree = x->fatherRelationTree;

    if(x->fatherRelationTree == &nullRelationTrackerNode_2){
        relation->relationTrackerTreeHead = y;
    }
    else if(x == x->fatherRelationTree->leftNodeRelationTree){
        x->fatherRelationTree->leftNodeRelationTree = y;
    }
    else{
        x->fatherRelationTree->rightNodeRelationTree = y;
    }
    y->leftNodeRelationTree = x;
    x->fatherRelationTree = y;
}

void rightRotateRelationTracker_2(struct relationTracker_2 * x, struct relation * relation){
    struct relationTracker_2 * y = x->leftNodeRelationTree;
    x->leftNodeRelationTree = y->rightNodeRelationTree;

    if(y->rightNodeRelationTree != &nullRelationTrackerNode_2){
        y->rightNodeRelationTree->fatherRelationTree = x;
    }
    y->fatherRelationTree = x->fatherRelationTree;

    if(x->fatherRelationTree == &nullRelationTrackerNode_2){
        relation->relationTrackerTreeHead = y;
    }
    else if(x == x->fatherRelationTree->rightNodeRelationTree){
        x->fatherRelationTree->rightNodeRelationTree = y;
    }
    else{
        x->fatherRelationTree->leftNodeRelationTree = y;
    }
    y->rightNodeRelationTree = x;
    x->fatherRelationTree = y;
}

void insertRelationTracker_2(struct relation * relation, struct relationTracker_2 * z) {
    struct relationTracker_2 * y = &nullRelationTrackerNode_2;
    struct relationTracker_2 * x = relation->relationTrackerTreeHead;

    int i = 0;
    while(x != &nullRelationTrackerNode_2){
        i++;
        y = x;

        if(z->relationTracker_1->numberOfRelationReceived < x->relationTracker_1->numberOfRelationReceived){
            x = x->leftNodeRelationTree;
        }
        else{
            x = x->rightNodeRelationTree;
        }
    }
    z->fatherRelationTree = y;
    if(y == &nullRelationTrackerNode_2){
        relation->relationTrackerTreeHead = z;
    }
    else if(z->relationTracker_1->numberOfRelationReceived < y->relationTracker_1->numberOfRelationReceived){
        y->leftNodeRelationTree = z;
    }
    else{
        y->rightNodeRelationTree = z;
    }
    z->leftNodeRelationTree = &nullRelationTrackerNode_2;
    z->rightNodeRelationTree = &nullRelationTrackerNode_2;
    z->nodeColorRelationTree = RED;
    insertRelationTracker_2_FIXUP(relation, z);
}

void insertRelationTracker_2_FIXUP(struct relation * relation, struct relationTracker_2 * newNode){
    struct relationTracker_2 * x;
    struct relationTracker_2 * y;
    if(newNode == relation->relationTrackerTreeHead){
        newNode->nodeColorRelationTree = BLACK;
    }
    else{
        x = newNode->fatherRelationTree;
        if(x->nodeColorRelationTree == RED){
            if(x == x->fatherRelationTree->leftNodeRelationTree){
                y = x->fatherRelationTree->rightNodeRelationTree;
                if(y->nodeColorRelationTree == RED){
                    x->nodeColorRelationTree= BLACK;
                    y->nodeColorRelationTree = BLACK;
                    x->fatherRelationTree->nodeColorRelationTree = RED;
                    insertRelationTracker_2_FIXUP(relation, x->fatherRelationTree);
                }
                else{
                    if(newNode == x->rightNodeRelationTree) {
                        newNode = x;
                        leftRotateRelationTracker_2(newNode, relation);
                        x = newNode->fatherRelationTree;
                    }
                    x->nodeColorRelationTree = BLACK;
                    x->fatherRelationTree->nodeColorRelationTree = RED;
                    rightRotateRelationTracker_2(x->fatherRelationTree,relation);
                }
            }
            else{
                y = x->fatherRelationTree->leftNodeRelationTree;
                if(y->nodeColorRelationTree == RED){
                    x->nodeColorRelationTree= BLACK;
                    y->nodeColorRelationTree = BLACK;
                    x->fatherRelationTree->nodeColorRelationTree = RED;
                    insertRelationTracker_2_FIXUP(relation, x->fatherRelationTree);
                }
                else{
                    if(newNode == x->leftNodeRelationTree) {
                        newNode = x;
                        rightRotateRelationTracker_2(newNode, relation);
                        x = newNode->fatherRelationTree;
                    }
                    x->nodeColorRelationTree = BLACK;
                    x->fatherRelationTree->nodeColorRelationTree = RED;
                    leftRotateRelationTracker_2(x->fatherRelationTree,relation);
                }
            }
        }
    }
}

struct relationTracker_2 * minimumRelationTracker_2(struct relationTracker_2 * x){
    while(x->leftNodeRelationTree != &nullRelationTrackerNode_2){
        x = x->leftNodeRelationTree;
    }
    return x;
}

struct relationTracker_2 * successorRelationTracker_2(struct relationTracker_2 * x){
    struct relationTracker_2 * y;
    if(x->rightNodeRelationTree != &nullRelationTrackerNode_2){
        return minimumRelationTracker_2(x->rightNodeRelationTree);
    }
    y = x->fatherRelationTree;
    while((y != &nullRelationTrackerNode_2) && (x == y->rightNodeRelationTree)){
        x = y;
        y = y->fatherRelationTree;
    }
    return y;
}

void deleteRelationTracker_2(struct relation * relation, struct relationTracker_2 * z) {
    struct relationTracker_2 *x;
    struct relationTracker_2 *y;
    if(z->leftNodeRelationTree == &nullRelationTrackerNode_2 || z->rightNodeRelationTree == &nullRelationTrackerNode_2){
        y = z;
    }
    else{
        y = successorRelationTracker_2(z);
    }
    if(y->leftNodeRelationTree != &nullRelationTrackerNode_2){
        x = y->leftNodeRelationTree;
    }
    else{
        x = y->rightNodeRelationTree;
    }
    x->fatherRelationTree = y->fatherRelationTree;
    if(y->fatherRelationTree == &nullRelationTrackerNode_2){
        relation->relationTrackerTreeHead = x;
    }
    else if(y == y->fatherRelationTree->leftNodeRelationTree){
        y->fatherRelationTree->leftNodeRelationTree = x;
    }
    else{
        y->fatherRelationTree->rightNodeRelationTree = x;
    }
    if(y != z){
        int oldColor = y->nodeColorRelationTree;
        y->nodeColorRelationTree = z->nodeColorRelationTree;

        if(z == relation->relationTrackerTreeHead) {
            relation->relationTrackerTreeHead = y;
        }
        else {
            if (z->fatherRelationTree->leftNodeRelationTree == z) {
                z->fatherRelationTree->leftNodeRelationTree = y;
            } else {
                z->fatherRelationTree->rightNodeRelationTree = y;
            }
        }
        z->leftNodeRelationTree->fatherRelationTree = y;
        z->rightNodeRelationTree->fatherRelationTree = y;

        y->fatherRelationTree = z->fatherRelationTree;
        y->leftNodeRelationTree = z->leftNodeRelationTree;
        y->rightNodeRelationTree = z->rightNodeRelationTree;

        if(oldColor == BLACK){
            deleteRelationTracker_2_FIXUP(relation, x);
        }

        free(z);
        return;
    }
    if(y->nodeColorRelationTree == BLACK){
        deleteRelationTracker_2_FIXUP(relation, x);
    }
    free(y);
}

void deleteRelationTracker_2_FIXUP(struct relation * relation, struct relationTracker_2 * x){
    struct relationTracker_2 * w;
    if(x->nodeColorRelationTree == RED || x->fatherRelationTree == &nullRelationTrackerNode_2){
        x->nodeColorRelationTree = BLACK;
    }
    else if(x == x->fatherRelationTree->leftNodeRelationTree){
        w = x->fatherRelationTree->rightNodeRelationTree;
        if(w->nodeColorRelationTree == RED){
            w->nodeColorRelationTree = BLACK;
            x->fatherRelationTree->nodeColorRelationTree = RED;
            leftRotateRelationTracker_2(x->fatherRelationTree, relation);
            w = x->fatherRelationTree->rightNodeRelationTree;
        }
        if(w->leftNodeRelationTree->nodeColorRelationTree == BLACK && w->rightNodeRelationTree->nodeColorRelationTree == BLACK){
            w->nodeColorRelationTree = RED;
            deleteRelationTracker_2_FIXUP(relation,x->fatherRelationTree);
        }
        else{
            if(w->rightNodeRelationTree->nodeColorRelationTree == BLACK){
                w->leftNodeRelationTree->nodeColorRelationTree  = BLACK;
                w->nodeColorRelationTree = RED;
                rightRotateRelationTracker_2(w, relation);
                w = x->fatherRelationTree->rightNodeRelationTree;
            }
            w->nodeColorRelationTree = x->fatherRelationTree->nodeColorRelationTree;
            w->fatherRelationTree->nodeColorRelationTree = BLACK;
            w->rightNodeRelationTree->nodeColorRelationTree = BLACK;
            leftRotateRelationTracker_2(x->fatherRelationTree, relation);
        }
    }
    else{
        w = x->fatherRelationTree->leftNodeRelationTree;
        if(w->nodeColorRelationTree == RED){
            w->nodeColorRelationTree = BLACK;
            x->fatherRelationTree->nodeColorRelationTree = RED;
            rightRotateRelationTracker_2(x->fatherRelationTree, relation);
            w = x->fatherRelationTree->leftNodeRelationTree;
        }
        if(w->rightNodeRelationTree->nodeColorRelationTree == BLACK && w->leftNodeRelationTree->nodeColorRelationTree == BLACK){
            w->nodeColorRelationTree = RED;
            deleteRelationTracker_2_FIXUP(relation,x->fatherRelationTree);
        }
        else{
            if(w->leftNodeRelationTree->nodeColorRelationTree == BLACK){
                w->rightNodeRelationTree->nodeColorRelationTree  = BLACK;
                w->nodeColorRelationTree = RED;
                leftRotateRelationTracker_2(w, relation);
                w = x->fatherRelationTree->leftNodeRelationTree;
            }
            w->nodeColorRelationTree = x->fatherRelationTree->nodeColorRelationTree;
            w->fatherRelationTree->nodeColorRelationTree = BLACK;
            w->leftNodeRelationTree->nodeColorRelationTree = BLACK;
            rightRotateRelationTracker_2(x->fatherRelationTree, relation);
        }
    }
}



/* relation sent to (rombo1) */
// nota: è accessibile dall'entity (che si sta eliminando)
// è in un albero ordinato in base all'indirizzo di memeoria della relation tracker receiver e con posibilità di essere iterato
// riferimento alla relation tracker receiver
/* relation received from (rombo2) */
// nota: è accessibile dall'entity destinataria
// è in un albero ordinato in base all'indirizzo di memoria della relation tracker origin e con possibilità di essere iterato
// riferimento alla relation tracker origin

//SENT
struct relationSentToOrReceivedFrom * searchRelationSentTo(char * destId, struct relationSentToOrReceivedFrom * head){
    struct relationSentToOrReceivedFrom *result = head;

    while (result != &nullRelationSentToOrReceivedFromNode) {
        int cmp = strcmp(result->destRelationTracker->entity->name, destId);
        if (cmp == 0) {
            //printf("relationSentToOrReceivedFrom referred to entity %s found\n", destId);
            return result;
        } else if (cmp > 0) {
            result = result->leftSentTo;
        } else {
            result = result->rightSentTo;
        }
    }
    //printf("relationSentToOrReceivedFrom referred to entity %s not found\n", destId);
    return result;
}

void leftRotateRelationSentTo(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = x->rightSentTo;
    x->rightSentTo = y->leftSentTo;
    if(y->leftSentTo != &nullRelationSentToOrReceivedFromNode){
        y->leftSentTo->fatherSentTo = x;
    }
    y->fatherSentTo = x->fatherSentTo;
    if(x->fatherSentTo == &nullRelationSentToOrReceivedFromNode){
        *head = y;
    }
    else if(x == x->fatherSentTo->leftSentTo){
        x->fatherSentTo->leftSentTo = y;
    }
    else{
        x->fatherSentTo->rightSentTo = y;
    }
    y->leftSentTo = x;
    x->fatherSentTo = y;
}

void rightRotateRelationSentTo(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = x->leftSentTo;
    x->leftSentTo = y->rightSentTo;
    if(y->rightSentTo != &nullRelationSentToOrReceivedFromNode){
        y->rightSentTo->fatherSentTo = x;
    }
    y->fatherSentTo = x->fatherSentTo;
    if(x->fatherSentTo == &nullRelationSentToOrReceivedFromNode){
        *head = y;
    }
    else if(x == x->fatherSentTo->rightSentTo){
        x->fatherSentTo->rightSentTo = y;
    }
    else{
        x->fatherSentTo->leftSentTo = y;
    }
    y->rightSentTo = x;
    x->fatherSentTo = y;
}

void insertRelationSentTo(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = &nullRelationSentToOrReceivedFromNode;
    struct relationSentToOrReceivedFrom * x = *head;
    while(x != &nullRelationSentToOrReceivedFromNode){
        y = x;
        if(strcmp(z->destRelationTracker->entity->name, x->destRelationTracker->entity->name) < 0){
            x = x->leftSentTo;
        }
        else{
            x = x->rightSentTo;
        }
    }
    z->fatherSentTo = y;
    if(y == &nullRelationSentToOrReceivedFromNode){
        *head = z;
    }
    else if(strcmp(z->destRelationTracker->entity->name, y->destRelationTracker->entity->name) < 0){
        y->leftSentTo = z;
    }
    else{
        y->rightSentTo = z;
    }
    z->leftSentTo = &nullRelationSentToOrReceivedFromNode;
    z->rightSentTo = &nullRelationSentToOrReceivedFromNode;
    z->nodeColorSentTo = RED;
    insertRelationSentTo_FIXUP(z, head);
}

void insertRelationSentTo_FIXUP(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y;
    struct relationSentToOrReceivedFrom * x;
    if(z == *head){
        z->nodeColorSentTo = BLACK;
    }
    else{
        x = z->fatherSentTo;
        if(x->nodeColorSentTo == RED){
            if( x == x->fatherSentTo->leftSentTo){
                y = x->fatherSentTo->rightSentTo;
                if(y->nodeColorSentTo == RED){
                    x->nodeColorSentTo = BLACK;
                    y->nodeColorSentTo = BLACK;
                    x->fatherSentTo->nodeColorSentTo = RED;
                    insertRelationSentTo_FIXUP(x->fatherSentTo, head);
                }
                else{
                    if(z == x->rightSentTo){
                        z = x;
                        leftRotateRelationSentTo(z, head);
                        x = z->fatherSentTo;
                    }
                    x->nodeColorSentTo = BLACK;
                    x->fatherSentTo->nodeColorSentTo = RED;
                    rightRotateRelationSentTo(x->fatherSentTo, head);
                }
            }
            else{
                y = x->fatherSentTo->leftSentTo;
                if(y->nodeColorSentTo == RED){
                    x->nodeColorSentTo = BLACK;
                    y->nodeColorSentTo = BLACK;
                    x->fatherSentTo->nodeColorSentTo = RED;
                    insertRelationSentTo_FIXUP(x->fatherSentTo, head);
                }
                else{
                    if(z == x->leftSentTo){
                        z = x;
                        rightRotateRelationSentTo(z, head);
                        x = z->fatherSentTo;
                    }
                    x->nodeColorSentTo = BLACK;
                    x->fatherSentTo->nodeColorSentTo = RED;
                    leftRotateRelationSentTo(x->fatherSentTo, head);
                }
            }
        }
    }
}

struct relationSentToOrReceivedFrom * minimumRelationSentTo(struct relationSentToOrReceivedFrom * x){
    while (x->leftSentTo != &nullRelationSentToOrReceivedFromNode){
        x = x->leftSentTo;
    }
    return x;
}

struct relationSentToOrReceivedFrom * successorRelationSentTo(struct relationSentToOrReceivedFrom * x){
    struct relationSentToOrReceivedFrom * y;
    if(x->rightSentTo != &nullRelationSentToOrReceivedFromNode){
        return minimumRelationSentTo(x->rightSentTo);
    }
    y = x->fatherSentTo;
    while((y!=&nullRelationSentToOrReceivedFromNode) && (x == y->rightSentTo)){
        x = y;
        y = y->fatherSentTo;
    }
    return y;
}

void deleteRelationSentTo(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y;
    struct relationSentToOrReceivedFrom * x;
    if((z->leftSentTo == &nullRelationSentToOrReceivedFromNode) || (z->rightSentTo == &nullRelationSentToOrReceivedFromNode)){
        y = z;
    }
    else{
        y = successorRelationSentTo(z);
    }
    if(y->leftSentTo != &nullRelationSentToOrReceivedFromNode){
        x = y->leftSentTo;
    }
    else{
        x = y->rightSentTo;
    }
    x->fatherSentTo = y->fatherSentTo;
    if(y->fatherSentTo == &nullRelationSentToOrReceivedFromNode){
        *head = x;
    }
    else if(y == y->fatherSentTo->leftSentTo){
        y->fatherSentTo->leftSentTo = x;
    }
    else{
        y->fatherSentTo->rightSentTo = x;
    }
    if (y != z){
        int oldColor = y->nodeColorSentTo;
        y->nodeColorSentTo = z->nodeColorSentTo;

        if(z == *head){
            *head = y;
        }
        else {
            if (z->fatherSentTo->leftSentTo == z) {
                z->fatherSentTo->leftSentTo = y;
            } else {
                z->fatherSentTo->rightSentTo = y;
            }
        }
        z->leftSentTo->fatherSentTo = y;
        z->rightSentTo->fatherSentTo = y;

        y->fatherSentTo = z->fatherSentTo;
        y->leftSentTo = z->leftSentTo;
        y->rightSentTo = z->rightSentTo;
        if(oldColor == BLACK){
            deleteRelationSentTo_FIXUP(x, head);
        }
        return;
    }
    if(y->nodeColorSentTo == BLACK){
        deleteRelationSentTo_FIXUP(x, head);
    }
}

void deleteRelationSentTo_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * w;

    if((x->nodeColorSentTo == RED) || (x->fatherSentTo == &nullRelationSentToOrReceivedFromNode)){
        x->nodeColorSentTo = BLACK;
    }
    else if(x == x->fatherSentTo->leftSentTo){
        w = x->fatherSentTo->rightSentTo;
        if(w->nodeColorSentTo == RED){
            w->nodeColorSentTo = BLACK;
            x->fatherSentTo->nodeColorSentTo = RED;
            leftRotateRelationSentTo(x->fatherSentTo, head);
            w = x->fatherSentTo->rightSentTo;
        }
        if((w->leftSentTo->nodeColorSentTo == BLACK) && (w->rightSentTo->nodeColorSentTo == BLACK)){
            w->nodeColorSentTo = RED;
            deleteRelationSentTo_FIXUP(x->fatherSentTo, head);
        }
        else{
            if(w->rightSentTo->nodeColorSentTo == BLACK){
                w->leftSentTo->nodeColorSentTo = BLACK;
                w->nodeColorSentTo = RED;
                rightRotateRelationSentTo(w,head);
                w = x->fatherSentTo->rightSentTo;
            }
            w->nodeColorSentTo = x->fatherSentTo->nodeColorSentTo;
            x->fatherSentTo->nodeColorSentTo = BLACK;
            w->rightSentTo->nodeColorSentTo = BLACK;
            rightRotateRelationSentTo(x->fatherSentTo, head);
        }
    }
    else{
        w = x->fatherSentTo->leftSentTo;
        if(w->nodeColorSentTo == RED){
            w->nodeColorSentTo = BLACK;
            x->fatherSentTo->nodeColorSentTo = RED;
            rightRotateRelationSentTo(x->fatherSentTo,head);
            w = x->fatherSentTo->leftSentTo;
        }
        if((w->rightSentTo->nodeColorSentTo == BLACK) && (w->leftSentTo->nodeColorSentTo == BLACK)){
            w->nodeColorSentTo = RED;
            deleteRelationSentTo_FIXUP(x->fatherSentTo,head);
        }
        else{
            if(w->leftSentTo->nodeColorSentTo == BLACK){
                w->rightSentTo->nodeColorSentTo = BLACK;
                w->nodeColorSentTo = RED;
                leftRotateRelationSentTo(w, head);
                w = x->fatherSentTo->leftSentTo;
            }
            w->nodeColorSentTo = x->fatherSentTo->nodeColorSentTo;
            x->fatherSentTo->nodeColorSentTo = BLACK;
            w->leftSentTo->nodeColorSentTo = BLACK;
            rightRotateRelationSentTo(x->fatherSentTo, head);
        }
    }
}

//RECEIVED
struct relationSentToOrReceivedFrom * searchRelationReceivedFrom(char * origId, struct relationSentToOrReceivedFrom * head){
    struct relationSentToOrReceivedFrom *result = head;

    while (result != &nullRelationSentToOrReceivedFromNode) {
        int cmp = strcmp(result->origRelationTracker->entity->name, origId);
        if (cmp == 0) {
            //printf("relationSentToOrReceivedFrom referred to entity %s found\n", origId);
            return result;
        } else if (cmp > 0) {
            result = result->leftReceivedFrom;
        } else {
            result = result->rightReceivedFrom;
        }
    }
    //printf("relationSentToOrReceivedFrom referred to entity %s not found\n", origId);
    return result;
}

void leftRotateRelationReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = x->rightSentTo;
    x->rightReceivedFrom = y->leftReceivedFrom;
    if(y->leftReceivedFrom != &nullRelationSentToOrReceivedFromNode){
        y->leftReceivedFrom->fatherReceivedFrom = x;
    }
    y->fatherReceivedFrom = x->fatherReceivedFrom;
    if(x->fatherReceivedFrom == &nullRelationSentToOrReceivedFromNode){
        *head = y;
    }
    else if(x == x->fatherReceivedFrom->leftReceivedFrom){
        x->fatherReceivedFrom->leftReceivedFrom = y;
    }
    else{
        x->fatherReceivedFrom->rightReceivedFrom = y;
    }
    y->leftReceivedFrom = x;
    x->fatherReceivedFrom = y;
}

void rightRotateRelationReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = x->leftReceivedFrom;
    x->leftReceivedFrom = y->rightReceivedFrom;
    if(y->rightReceivedFrom != &nullRelationSentToOrReceivedFromNode){
        y->rightReceivedFrom->rightReceivedFrom = x;
    }
    y->fatherReceivedFrom = x->fatherReceivedFrom;
    if(x->fatherReceivedFrom == &nullRelationSentToOrReceivedFromNode){
        *head = y;
    }
    else if(x == x->fatherReceivedFrom->rightReceivedFrom){
        x->fatherReceivedFrom->rightReceivedFrom = y;
    }
    else{
        x->fatherReceivedFrom->leftReceivedFrom = y;
    }
    y->rightReceivedFrom = x;
    x->fatherReceivedFrom = y;
}

void insertRelationReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = &nullRelationSentToOrReceivedFromNode;
    struct relationSentToOrReceivedFrom * x = *head;
    while(x != &nullRelationSentToOrReceivedFromNode){
        y = x;
        if(strcmp(z->origRelationTracker->entity->name, x->origRelationTracker->entity->name) < 0){
            x = x->leftReceivedFrom;
        }
        else{
            x = x->rightReceivedFrom;
        }
    }
    z->fatherReceivedFrom = y;
    if(y == &nullRelationSentToOrReceivedFromNode){
        *head = z;
    }
    else if(strcmp(z->origRelationTracker->entity->name, y->origRelationTracker->entity->name) < 0){
        y->leftReceivedFrom = z;
    }
    else{
        y->rightReceivedFrom = z;
    }
    z->leftReceivedFrom = &nullRelationSentToOrReceivedFromNode;
    z->rightReceivedFrom = &nullRelationSentToOrReceivedFromNode;
    z->nodeColorReceivedFrom = RED;
    insertRelationReceivedFrom_FIXUP(z, head);
}

void insertRelationReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y;
    struct relationSentToOrReceivedFrom * x;
    if(z == *head){
        z->nodeColorReceivedFrom = BLACK;
    }
    else{
        x = z->fatherReceivedFrom;
        if(x->nodeColorReceivedFrom == RED){
            if( x == x->fatherReceivedFrom->leftReceivedFrom){
                y = x->fatherReceivedFrom->rightReceivedFrom;
                if(y->nodeColorReceivedFrom == RED){
                    x->nodeColorReceivedFrom = BLACK;
                    y->nodeColorReceivedFrom = BLACK;
                    x->fatherReceivedFrom->nodeColorReceivedFrom = RED;
                    insertRelationReceivedFrom_FIXUP(x->fatherReceivedFrom, head);
                }
                else{
                    if(z == x->rightReceivedFrom){
                        z = x;
                        leftRotateRelationReceivedFrom(z, head);
                        x = z->fatherReceivedFrom;
                    }
                    x->nodeColorReceivedFrom = BLACK;
                    x->fatherReceivedFrom->nodeColorReceivedFrom = RED;
                    rightRotateRelationReceivedFrom(x->fatherReceivedFrom, head);
                }
            }
            else{
                y = x->fatherReceivedFrom->leftReceivedFrom;
                if(y->nodeColorReceivedFrom == RED){
                    x->nodeColorReceivedFrom = BLACK;
                    y->nodeColorReceivedFrom = BLACK;
                    x->fatherReceivedFrom->nodeColorReceivedFrom = RED;
                    insertRelationReceivedFrom_FIXUP(x->fatherReceivedFrom, head);
                }
                else{
                    if(z == x->leftReceivedFrom){
                        z = x;
                        rightRotateRelationReceivedFrom(z, head);
                        x = z->fatherReceivedFrom;
                    }
                    x->nodeColorReceivedFrom = BLACK;
                    x->fatherReceivedFrom->nodeColorReceivedFrom = RED;
                    leftRotateRelationReceivedFrom(x->fatherReceivedFrom, head);
                }
            }
        }
    }
}

struct relationSentToOrReceivedFrom * minimumRelationReceivedFrom(struct relationSentToOrReceivedFrom * x){
    while (x->leftReceivedFrom != &nullRelationSentToOrReceivedFromNode){
        x = x->leftReceivedFrom;
    }
    return x;
}

struct relationSentToOrReceivedFrom * successorRelationReceivedFrom(struct relationSentToOrReceivedFrom * x){
    struct relationSentToOrReceivedFrom * y;
    if(x->rightReceivedFrom != &nullRelationSentToOrReceivedFromNode){
        return minimumRelationReceivedFrom(x->rightReceivedFrom);
    }
    y = x->fatherReceivedFrom;
    while((y!=&nullRelationSentToOrReceivedFromNode) && (x == y->rightReceivedFrom)){
        x = y;
        y = y->fatherReceivedFrom;
    }
    return y;
}

void deleteRelationReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y;
    struct relationSentToOrReceivedFrom * x;
    if((z->leftReceivedFrom == &nullRelationSentToOrReceivedFromNode) || (z->rightReceivedFrom == &nullRelationSentToOrReceivedFromNode)){
        y = z;
    }
    else{
        y = successorRelationReceivedFrom(z);
    }
    if(y->leftReceivedFrom != &nullRelationSentToOrReceivedFromNode){
        x = y->leftReceivedFrom;
    }
    else{
        x = y->rightReceivedFrom;
    }
    x->fatherReceivedFrom = y->fatherReceivedFrom;
    if(y->fatherReceivedFrom == &nullRelationSentToOrReceivedFromNode){
        *head = x;
    }
    else if(y == y->fatherReceivedFrom->leftReceivedFrom){
        y->fatherReceivedFrom->leftReceivedFrom = x;
    }
    else{
        y->fatherReceivedFrom->rightReceivedFrom = x;
    }
    if (y != z){
        int oldColor = y->nodeColorReceivedFrom;
        y->nodeColorReceivedFrom = z->nodeColorReceivedFrom;

        if(z == *head){
            *head = y;
        }
        else {
            if (z->fatherReceivedFrom->leftReceivedFrom == z) {
                z->fatherReceivedFrom->leftReceivedFrom = y;
            } else {
                z->fatherReceivedFrom->rightReceivedFrom = y;
            }
        }
        z->leftReceivedFrom->fatherReceivedFrom = y;
        z->rightReceivedFrom->fatherReceivedFrom = y;

        y->fatherReceivedFrom = z->fatherReceivedFrom;
        y->leftReceivedFrom = z->leftReceivedFrom;
        y->rightReceivedFrom = z->rightReceivedFrom;

        if(oldColor == BLACK){
            deleteRelationReceivedFrom_FIXUP(x, head);
        }
        return;
    }
    if(y->nodeColorReceivedFrom == BLACK){
        deleteRelationReceivedFrom_FIXUP(x, head);
    }
}

void deleteRelationReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * w;

    if((x->nodeColorReceivedFrom == RED) || (x->fatherReceivedFrom == &nullRelationSentToOrReceivedFromNode)){
        x->nodeColorReceivedFrom = BLACK;
    }
    else if(x == x->fatherReceivedFrom->leftReceivedFrom){
        w = x->fatherReceivedFrom->rightReceivedFrom;
        if(w->nodeColorReceivedFrom == RED){
            w->nodeColorReceivedFrom = BLACK;
            x->fatherReceivedFrom->nodeColorReceivedFrom = RED;
            leftRotateRelationReceivedFrom(x->fatherReceivedFrom, head);
            w = x->fatherReceivedFrom->rightReceivedFrom;
        }
        if((w->leftReceivedFrom->nodeColorReceivedFrom == BLACK) && (w->rightReceivedFrom->nodeColorReceivedFrom == BLACK)){
            w->nodeColorReceivedFrom = RED;
            deleteRelationReceivedFrom_FIXUP(x->fatherReceivedFrom, head);
        }
        else{
            if(w->rightReceivedFrom->nodeColorReceivedFrom == BLACK){
                w->leftReceivedFrom->nodeColorReceivedFrom = BLACK;
                w->nodeColorReceivedFrom = RED;
                rightRotateRelationReceivedFrom(w,head);
                w = x->fatherReceivedFrom->rightReceivedFrom;
            }
            w->nodeColorReceivedFrom = x->fatherReceivedFrom->nodeColorReceivedFrom;
            x->fatherReceivedFrom->nodeColorReceivedFrom = BLACK;
            w->rightReceivedFrom->nodeColorReceivedFrom = BLACK;
            rightRotateRelationReceivedFrom(x->fatherReceivedFrom, head);
        }
    }
    else{
        w = x->fatherReceivedFrom->leftReceivedFrom;
        if(w->nodeColorReceivedFrom == RED){
            w->nodeColorReceivedFrom = BLACK;
            x->fatherReceivedFrom->nodeColorReceivedFrom = RED;
            rightRotateRelationReceivedFrom(x->fatherReceivedFrom,head);
            w = x->fatherReceivedFrom->leftReceivedFrom;
        }
        if((w->rightReceivedFrom->nodeColorReceivedFrom == BLACK) && (w->leftReceivedFrom->nodeColorReceivedFrom == BLACK)){
            w->nodeColorReceivedFrom = RED;
            deleteRelationReceivedFrom_FIXUP(x->fatherReceivedFrom,head);
        }
        else{
            if(w->leftReceivedFrom->nodeColorReceivedFrom == BLACK){
                w->rightReceivedFrom->nodeColorReceivedFrom = BLACK;
                w->nodeColorReceivedFrom = RED;
                leftRotateRelationReceivedFrom(w, head);
                w = x->fatherReceivedFrom->leftReceivedFrom;
            }
            w->nodeColorReceivedFrom = x->fatherReceivedFrom->nodeColorReceivedFrom;
            x->fatherReceivedFrom->nodeColorReceivedFrom = BLACK;
            w->leftReceivedFrom->nodeColorReceivedFrom = BLACK;
            rightRotateRelationReceivedFrom(x->fatherReceivedFrom, head);
        }
    }
}

/* main */
char input[300];
int main(void) {

    //initilizing hash table
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION ; ++i) {
        entityHashTable[i] = NULL;
    }

    //initializing relation tree
    relationTreeHead = &nullRelationNode;

    while(1){
        fgets(input, 300 , stdin);
        if(input[0] == 'a'){
            if(input[3] == 'e'){
                addent();
            }
            else{
                addrel();
            }
        }
        else if(input[0] == 'd'){
            if(input[3] == 'e'){
                delent();
            }
            else{
                delrel();
            }
        }
        else if(input[0] == 'r'){
            report();
        }
        else{
            //fullReport();
            return 0;
        }
    }
}



void addent(){
    int i = 0;
    int j = 8;
    idEntLength = 0;
    while(input[j] != '"'){
        idEnt[i] = input[j];
        j++;
        i++;
    }
    idEnt[i] = '\0';
    idEntLength = i+1;

    insertEntityHashTable(idEnt);
}

void addrel(){
    int i = 0;
    int j = 8;
    while(input[j] != '"'){
        idOrig[i] = input[j];
        j++;
        i++;
    }
    j+=3;
    idOrig[i] = '\0';
    idOrigLength = i+1;
    i=0;
    while(input[j] != '"'){
        idDest[i] = input[j];
        j++;
        i++;
    }
    j+=3;
    idDest[i] = '\0';
    idDestLength = i+1;
    i=0;
    while(input[j] != '"'){
        idRel[i] = input[j];
        j++;
        i++;
    }
    idRel[i] = '\0';
    idRelLenght = i+1;

    struct entity *entityOrig = searchEntityHashTable(idOrig);
    if (entityOrig == NULL) {
        return;
    }
    struct entity *entityDest = searchEntityHashTable(idDest);
    if (entityDest == NULL) {
        return;
    }

    struct relation * relation = searchRelation(idRel);
    if (relation == &nullRelationNode) {
        relation = malloc(sizeof(struct relation));
        relation->name = malloc(idRelLenght);
        memcpy(relation->name, idRel, idRelLenght);
        relation->relationTrackerTreeHead = &nullRelationTrackerNode_2;

        insertRelation(relation);
    }

    struct relationTracker_1 * relTrackerOrig = searchRelationTracker_1(idRel, entityOrig);
    if (relTrackerOrig == &nullRelationTrackerNode_1) {
        relTrackerOrig = malloc(sizeof(struct relationTracker_1));
        relTrackerOrig->leftNodeEntityTree = &nullRelationTrackerNode_1;
        relTrackerOrig->rightNodeEntityTree = &nullRelationTrackerNode_1;
        relTrackerOrig->fatherEntityTree = &nullRelationTrackerNode_1;
        relTrackerOrig->sentToTreeHead = &nullRelationSentToOrReceivedFromNode;
        relTrackerOrig->receivedFromTreeHead = &nullRelationSentToOrReceivedFromNode;
        relTrackerOrig->entity = entityOrig;
        relTrackerOrig->numberOfRelationReceived = 0;
        relTrackerOrig->relation = relation;
        relTrackerOrig->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
        relTrackerOrig->relationTracker_2->relationTracker_1=relTrackerOrig;

        insertRelationTracker_1(entityOrig, relTrackerOrig);
        insertRelationTracker_2(relation, relTrackerOrig->relationTracker_2);
    }

    struct relationTracker_1 *relTrackerDest;
    int firstTimeInsert = 0;
    if(entityDest == entityOrig){
        relTrackerDest = relTrackerOrig;
    }
    else {
        relTrackerDest = searchRelationTracker_1(idRel, entityDest);
        if (relTrackerDest == &nullRelationTrackerNode_1) {
            relTrackerDest = malloc(sizeof(struct relationTracker_1));
            relTrackerDest->sentToTreeHead = &nullRelationSentToOrReceivedFromNode;
            relTrackerDest->receivedFromTreeHead = &nullRelationSentToOrReceivedFromNode;
            relTrackerDest->numberOfRelationReceived = 1;
            relTrackerDest->entity = entityDest;
            relTrackerDest->relation = relation;
            relTrackerDest->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
            relTrackerDest->relationTracker_2->relationTracker_1=relTrackerDest;

            insertRelationTracker_1(entityDest, relTrackerDest);
            insertRelationTracker_2(relation, relTrackerDest->relationTracker_2);
            firstTimeInsert = 1;
        }
    }

    struct relationSentToOrReceivedFrom *relationSentToOrReceivedFrom = searchRelationSentTo(idDest, relTrackerOrig->sentToTreeHead);
    if(relationSentToOrReceivedFrom == &nullRelationSentToOrReceivedFromNode) {
        relationSentToOrReceivedFrom = malloc(sizeof(struct relationSentToOrReceivedFrom));
        relationSentToOrReceivedFrom->origRelationTracker = relTrackerOrig;
        relationSentToOrReceivedFrom->destRelationTracker = relTrackerDest;
    }
    else{
        return;
    }
    insertRelationSentTo(relationSentToOrReceivedFrom, &(relTrackerOrig->sentToTreeHead));
    insertRelationReceivedFrom(relationSentToOrReceivedFrom, &(relTrackerDest->receivedFromTreeHead));
    if(firstTimeInsert == 0) {
        deleteRelationTracker_2(relation, relTrackerDest->relationTracker_2);
        relTrackerDest->numberOfRelationReceived++;
        relTrackerDest->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
        relTrackerDest->relationTracker_2->relationTracker_1 = relTrackerDest;
        relTrackerDest->relationTracker_2->rightNodeRelationTree = &nullRelationTrackerNode_2;
        relTrackerDest->relationTracker_2->leftNodeRelationTree = &nullRelationTrackerNode_2;
        relTrackerDest->relationTracker_2->fatherRelationTree = &nullRelationTrackerNode_2;
        insertRelationTracker_2(relation, relTrackerDest->relationTracker_2);
    }
    else{
    }
    numberOfTotalAddrel++;
}

void deleteAllRelationTrackerRecursive_1(struct relationTracker_1 * currentRelationTracker);
void deleteAllRelationSentToRecursive(struct relationSentToOrReceivedFrom * currentRelationSentTo);
void deleteAllRelationReceivedFromRecursive(struct relationSentToOrReceivedFrom * currentRelationReceivedFrom);
void deleteAllRelationTrackerRecursive_1(struct relationTracker_1 * currentRelationTracker){
    if(currentRelationTracker != &nullRelationTrackerNode_1){
        deleteAllRelationTrackerRecursive_1(currentRelationTracker->leftNodeEntityTree);

        //printf("\tanalizing relationTracker_1: %s\n", currentRelationTracker->relation->name);
        //printf("\tdeleting Sent to:\n");
        deleteAllRelationSentToRecursive(currentRelationTracker->sentToTreeHead);
        //printf("\tdeleting Received From:\n");
        deleteAllRelationReceivedFromRecursive(currentRelationTracker->receivedFromTreeHead);

        deleteAllRelationTrackerRecursive_1(currentRelationTracker->rightNodeEntityTree);

        //printf("\tfreeing relationTracker_2(%s)\n", currentRelationTracker->relationTracker_2->relationTracker_1->relation->name);
        deleteRelationTracker_2(currentRelationTracker->relation, currentRelationTracker->relationTracker_2);
        //printf("\tfreeing relationTracker_1(%s)\n", currentRelationTracker->relation->name);
        free(currentRelationTracker);
    }
}
void deleteAllRelationSentToRecursive(struct relationSentToOrReceivedFrom * currentRelationSentTo){//TODO RISCRIVI QUESTA FUNZIONE
    if(currentRelationSentTo != &nullRelationSentToOrReceivedFromNode){
        deleteAllRelationSentToRecursive(currentRelationSentTo->leftSentTo);

        //printf("\t\tanalizing %s->%s\n", currentRelationSentTo->origRelationTracker->entity->name, currentRelationSentTo->destRelationTracker->entity->name);

        //printf("\t\t\tnumber of relation received of (%s) was (%d)\n", currentRelationSentTo->destRelationTracker->entity->name, currentRelationSentTo->destRelationTracker->numberOfRelationReceived);
        deleteRelationTracker_2(currentRelationSentTo->destRelationTracker->relation,
                                currentRelationSentTo->destRelationTracker->relationTracker_2);
        currentRelationSentTo->destRelationTracker->numberOfRelationReceived--;
        currentRelationSentTo->destRelationTracker->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
        currentRelationSentTo->destRelationTracker->relationTracker_2->relationTracker_1 = currentRelationSentTo->destRelationTracker;
        //currentRelationSentTo->destRelationTracker->relationTracker_2->rightNodeRelationTree = &nullRelationTrackerNode_2;
        //currentRelationSentTo->destRelationTracker->relationTracker_2->leftNodeRelationTree = &nullRelationTrackerNode_2;
        //currentRelationSentTo->destRelationTracker->relationTracker_2->fatherRelationTree = &nullRelationTrackerNode_2;
        insertRelationTracker_2(currentRelationSentTo->destRelationTracker->relation,
                                currentRelationSentTo->destRelationTracker->relationTracker_2);
        //printf("\t\t\tdecreased numberOfRelationReceived\n");
        //printf("\t\t\tnumber of relation received of (%s) is now (%d)\n", currentRelationSentTo->destRelationTracker->entity->name, currentRelationSentTo->destRelationTracker->numberOfRelationReceived);


        //printf("\t\t\tdeleting from the destRelationTracker(%s)->receivedFrom the relation (%s->%s), %u\n",
        //       currentRelationSentTo->destRelationTracker->entity->name,
        //       currentRelationSentTo->origRelationTracker->entity->name, currentRelationSentTo->destRelationTracker->entity->name,
        //       currentRelationSentTo);
        deleteRelationReceivedFrom(
                currentRelationSentTo,
                &(currentRelationSentTo->destRelationTracker->receivedFromTreeHead)
        );

        if (currentRelationSentTo->destRelationTracker->sentToTreeHead == &nullRelationSentToOrReceivedFromNode &&
            currentRelationSentTo->destRelationTracker->receivedFromTreeHead == &nullRelationSentToOrReceivedFromNode) {
            //printf("\t\t\tdeleting the relationTracker(_1 and _2) of the dest, it has no sentTo or receivedFrom relations\n");
            deleteRelationTracker_2(currentRelationSentTo->destRelationTracker->relation,
                                    currentRelationSentTo->destRelationTracker->relationTracker_2);
            deleteRelationTracker_1(currentRelationSentTo->destRelationTracker->entity,
                                    currentRelationSentTo->destRelationTracker);
        }

        numberOfTotalDelrel++;

        deleteAllRelationSentToRecursive(currentRelationSentTo->rightSentTo);

        free(currentRelationSentTo);
    }
}
void deleteAllRelationReceivedFromRecursive(struct relationSentToOrReceivedFrom * currentRelationReceivedFrom){ //TODO RISCRIVI QUESTA FUNZIONE
    //N.B. in questa funzione non può succedere che si stia analizzando una relazione ricorsiva,
    //     sono già state eliminate tutte nella deleteAllRelationSentToRecursive

    if(currentRelationReceivedFrom != &nullRelationSentToOrReceivedFromNode){
        deleteAllRelationReceivedFromRecursive(currentRelationReceivedFrom->leftReceivedFrom);

        //printf("\t\tanalizing %s<-%s\n", currentRelationReceivedFrom->destRelationTracker->entity->name, currentRelationReceivedFrom->origRelationTracker->entity->name);
        //printf("\t\t\tdeleting from the origRelationTracker(%s)->sentToTreeHead the relation (%s->%s), %u\n",
        //        currentRelationReceivedFrom->origRelationTracker->entity->name,
        //        currentRelationReceivedFrom->origRelationTracker->entity->name, currentRelationReceivedFrom->destRelationTracker->entity->name,
        //        currentRelationReceivedFrom);
        deleteRelationSentTo(
                currentRelationReceivedFrom,
                &(currentRelationReceivedFrom->origRelationTracker->sentToTreeHead)
        );

        if(currentRelationReceivedFrom->origRelationTracker->sentToTreeHead == &nullRelationSentToOrReceivedFromNode &&
            currentRelationReceivedFrom->origRelationTracker->receivedFromTreeHead == &nullRelationSentToOrReceivedFromNode){
            //printf("\t\t\t deleting the relationTracker(_1 and _2) of the orig, it has no sentTo or receivedFrom relations");
            deleteRelationTracker_2(currentRelationReceivedFrom->origRelationTracker->relation,
                                    currentRelationReceivedFrom->origRelationTracker->relationTracker_2);
            deleteRelationTracker_1(currentRelationReceivedFrom->origRelationTracker->entity,
                                    currentRelationReceivedFrom->origRelationTracker);
        }

        numberOfTotalDelrel++;

        deleteAllRelationReceivedFromRecursive(currentRelationReceivedFrom->rightReceivedFrom);

        free(currentRelationReceivedFrom);
    }
}

void delent(){
    int i = 0;
    int j = 8;
    while(input[j] != '"'){
        idEnt[i] = input[j];
        j++;
        i++;
    }
    idEnt[i] = '\0';
    idEntLength = i+1;

    struct entity * entity = searchEntityHashTable(idEnt);
    if(entity == NULL){
        return;
    }

    deleteAllRelationTrackerRecursive_1(entity->relationTrackerEntityTreeHead);

    deleteEntityHashTable(idEnt);

    numberOfTotalDelent++;
}

void delrel(){
    int i = 0;
    int j = 8;
    while(input[j] != '"'){
        idOrig[i] = input[j];
        j++;
        i++;
    }
    idOrig[i] = '\0';
    idOrigLength = i+1;
    i=0;
    j+=3;
    while(input[j] != '"'){
        idDest[i] = input[j];
        j++;
        i++;
    }
    idDest[i] = '\0';
    idDestLength = i+1;
    i=0;
    j+=3;
    while(input[j] != '"'){
        idRel[i] = input[j];
        j++;
        i++;
    }
    idRel[i] = '\0';
    idDestLength = i +1;

    struct entity * entityOrig = searchEntityHashTable(idOrig);
    if(entityOrig == NULL){
        return;
    }
    struct entity * entityDest = searchEntityHashTable(idDest);
    if(entityDest == NULL){
        return;
    }

    struct relation * relation = searchRelation(idRel);
    if(relation == &nullRelationNode){
        return;
    }

    struct relationTracker_1 * relTrackerOrig = searchRelationTracker_1(idRel, entityOrig);
    if(relTrackerOrig == &nullRelationTrackerNode_1){
        return;
    }

    struct relationSentToOrReceivedFrom * relationReceivedFromOrSentTo = searchRelationSentTo(idDest, relTrackerOrig->sentToTreeHead);
    if(relationReceivedFromOrSentTo == &nullRelationSentToOrReceivedFromNode){
        return;
    }

    struct relationTracker_1 * relTrackerDest = relationReceivedFromOrSentTo->destRelationTracker;

    deleteRelationSentTo(
            relationReceivedFromOrSentTo,
            &(relTrackerOrig->sentToTreeHead));
    deleteRelationReceivedFrom(
            relationReceivedFromOrSentTo,
            &(relTrackerDest->receivedFromTreeHead));
    free(relationReceivedFromOrSentTo);

    deleteRelationTracker_2(relation, relTrackerDest->relationTracker_2);
    relTrackerDest->numberOfRelationReceived--;
    relTrackerDest->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
    relTrackerDest->relationTracker_2->relationTracker_1 = relTrackerDest;
    insertRelationTracker_2(relation, relTrackerDest->relationTracker_2);

    if(relTrackerDest != relTrackerOrig) {
        if (relTrackerOrig->receivedFromTreeHead == &nullRelationSentToOrReceivedFromNode &&
            relTrackerOrig->sentToTreeHead == &nullRelationSentToOrReceivedFromNode) {
            deleteRelationTracker_2(relation, relTrackerOrig->relationTracker_2);
            deleteRelationTracker_1(entityOrig, relTrackerOrig);
        }
        if (relTrackerDest->receivedFromTreeHead == &nullRelationSentToOrReceivedFromNode &&
            relTrackerDest->sentToTreeHead == &nullRelationSentToOrReceivedFromNode) {
            deleteRelationTracker_2(relation, relTrackerDest->relationTracker_2);
            deleteRelationTracker_1(entityDest, relTrackerDest);
        }
    }
    else{
        if (relTrackerOrig->receivedFromTreeHead == &nullRelationSentToOrReceivedFromNode &&
            relTrackerOrig->sentToTreeHead == &nullRelationSentToOrReceivedFromNode) {
            deleteRelationTracker_2(relation, relTrackerOrig->relationTracker_2);
            deleteRelationTracker_1(entityOrig, relTrackerOrig);
        }
    }

    numberOfTotalDelrel++;
}

int numberOfRelationReceivedPrinted_1 = 0;
int numberOfRelationReceivedPrinted_2 = 0;
void printAllRelationSentToRecursive(struct relationSentToOrReceivedFrom * x);
void printALlRelationReceivedFromRecursive(struct relationSentToOrReceivedFrom * x);
void printAllRelationRecursive(struct relation * x);
void printAllRelationTrackerRecursive_1(struct relationTracker_1 * x);
void printAllRelationTrackerRecursive_2(struct relationTracker_2 * x);
void printAllRelationSentToRecursive(struct relationSentToOrReceivedFrom * x){
    if(x != &nullRelationSentToOrReceivedFromNode){
        printAllRelationSentToRecursive(x->leftSentTo);
        printf("\t\t\t%s->%s, %u\n", x->origRelationTracker->entity->name, x->destRelationTracker->entity->name, x);
        printAllRelationSentToRecursive(x->rightSentTo);
    }
}
void printALlRelationReceivedFromRecursive(struct relationSentToOrReceivedFrom * x){
    if(x != &nullRelationSentToOrReceivedFromNode){
        printALlRelationReceivedFromRecursive(x->leftReceivedFrom);
        printf("\t\t\t%s<-%s, %u\n", x->destRelationTracker->entity->name, x->origRelationTracker->entity->name, x);
        numberOfRelationReceivedPrinted_1++;
        printALlRelationReceivedFromRecursive(x->rightReceivedFrom);
    }
}
void printAllRelationTrackerRecursive_1(struct relationTracker_1 * x){
    if(x != &nullRelationTrackerNode_1){
        printAllRelationTrackerRecursive_1(x->leftNodeEntityTree);
        printf("\t\t%s | relation received: %d\n", x->relation->name, x->numberOfRelationReceived);
        if(x->receivedFromTreeHead == &nullRelationSentToOrReceivedFromNode && x->sentToTreeHead == &nullRelationSentToOrReceivedFromNode){
            printf("incoherence 4\n");
            exit(1);
        }
        printALlRelationReceivedFromRecursive(x->receivedFromTreeHead);
        printAllRelationSentToRecursive(x->sentToTreeHead);
        printAllRelationTrackerRecursive_1(x->rightNodeEntityTree);
    }
}
void printAllRelationTrackerRecursive_2(struct relationTracker_2 * x){
    if(x!= &nullRelationTrackerNode_2){
        printAllRelationTrackerRecursive_2(x->leftNodeRelationTree);
        printf("\t\t%s has received %d\n", x->relationTracker_1->entity->name, x->relationTracker_1->numberOfRelationReceived);
        numberOfRelationReceivedPrinted_2 += x->relationTracker_1->numberOfRelationReceived;
        printAllRelationTrackerRecursive_2(x->rightNodeRelationTree);
    }
}
void printAllRelationRecursive(struct relation * x){
    if(x != &nullRelationNode){
        printAllRelationRecursive(x->left);
        printf("\t%s\n", x->name);
        printAllRelationTrackerRecursive_2(x->relationTrackerTreeHead);
        printAllRelationRecursive(x->right);
    }
}

struct stringList * newStringList(char * str);
void printReportRecursive(struct relation * relation);
void printList(struct stringList * head);
void deleteList(struct stringList * head);
void sortedInsert(struct stringList** head_ref, struct stringList* new_node);
int alreadyPrintedSomething = 0;
struct stringList {
    char * name;
    struct stringList * next;
};
struct stringList * newStringList(char * str){
    struct stringList * newStringList = malloc(sizeof(struct stringList));

    newStringList->name = str;
    newStringList->next = NULL;

    return newStringList;
}
void printList(struct stringList * head){
    struct stringList *temp = head;
    while(temp != NULL){
        fputs("\"",stdout);
        fputs(temp->name, stdout);
        fputs("\" ", stdout);
        temp = temp->next;
    }
}
void deleteList(struct stringList * head){
    struct stringList *tmp = head;

    while (head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
}
void sortedInsert(struct stringList** head_ref, struct stringList* new_node) {
    struct stringList *current;
    /* Special case for the head end */
    if ((*head_ref == NULL) || (strcmp((*head_ref)->name, new_node->name) >= 0)) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    } else {
        /* Locate the node before the point of insertion */
        current = *head_ref;
        while (current->next != NULL && strcmp(current->next->name, new_node->name) < 0) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}
int reportDone = 0;
void printReportRecursive(struct relation * relation){
    if(relation != &nullRelationNode){

        printReportRecursive(relation->left);

        struct relationTracker_2 * max = relation->relationTrackerTreeHead;
        if(max != &nullRelationTrackerNode_2) {
            while (max->rightNodeRelationTree != &nullRelationTrackerNode_2) {
                max = max->rightNodeRelationTree;
            }
            unsigned int quantity = max->relationTracker_1->numberOfRelationReceived;
            if (quantity != 0) {
                if(alreadyPrintedSomething != 0){
                    fputs(" ", stdout);
                }
                else{
                    reportDone = 1;
                }
                alreadyPrintedSomething = 1;
                fputs("\"",stdout);
                fputs(max->relationTracker_1->relation->name, stdout);
                fputs("\" ",stdout);

                struct stringList * head = NULL;
                while (max->fatherRelationTree != &nullRelationTrackerNode_2 && quantity == max->fatherRelationTree->relationTracker_1->numberOfRelationReceived) {
                    sortedInsert(&head, newStringList(max->relationTracker_1->entity->name));
                    max = max->fatherRelationTree;
                }
                sortedInsert(&head, newStringList(max->relationTracker_1->entity->name));

                //max here is the top node of the sub tree of the max
                printList(head);

                deleteList(head);

                printf("%d", quantity);
                fputs(";", stdout);
            }
        }

        printReportRecursive(relation->right);
    }
}

void printEntity(struct entity * entity);
void printEntityRelationTrackerRecursive_1(struct relationTracker_1 * x);
void printEntity(struct entity * entity){
    if(entity != NULL) {
        printf("hash table cell number [%d]\n", hashFunction(entity->name));
        printf("\tentity: %s\n", entity->name);
        printEntityRelationTrackerRecursive_1(entity->relationTrackerEntityTreeHead);
        printf("\n");
    }
    else{
        printf("entity is NULL, couldn't print it\n");
    }
}
void printEntityRelationTrackerRecursive_1(struct relationTracker_1 * x){
    if(x != &nullRelationTrackerNode_1){
        printEntityRelationTrackerRecursive_1(x->leftNodeEntityTree);
        printf("\t\t%s | relation received: %d\n", x->relation->name, x->numberOfRelationReceived);
        printALlRelationReceivedFromRecursive(x->receivedFromTreeHead);
        printAllRelationSentToRecursive(x->sentToTreeHead);
        printEntityRelationTrackerRecursive_1(x->rightNodeEntityTree);
    }
}
void fullReport(){
    int numberOfEntityPrinted = 0;
    numberOfRelationReceivedPrinted_1 = 0;
    numberOfRelationReceivedPrinted_2 = 0;
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION; ++i) {
        if (entityHashTable[i] != NULL) {
            printf("hash table cell number [%d]\n", i);

            struct entity * currentEntity = entityHashTable[i];
            while (currentEntity != NULL) {

                printf("\tentity: %s\n", currentEntity->name);

                printAllRelationTrackerRecursive_1(currentEntity->relationTrackerEntityTreeHead);

                numberOfEntityPrinted++;

                currentEntity = currentEntity->nextEntity;
            }

            printf("\n");

        }
    }

    printf("hash table dimension: %d\n", ENTITY_HASHTABLE_DIMENSION);
    printf("amount of entities stored in the hash table: %d\n", numberOfEntityInHashTable);
    printf("amount of entities printed: %d\n", numberOfEntityPrinted );
    printf("amount of well formed addent executed: %d\n", numberOfTotalAddent);
    printf("amount of well formed delent executed: %d\n", numberOfTotalDelent);
    printf("amount of total current entities registered: %d\n", (numberOfTotalAddent - numberOfTotalDelent));
    if (numberOfEntityInHashTable != numberOfEntityPrinted) {
        printf("incoherence 0\n");
        exit(1);
    }
    if ((numberOfTotalAddent - numberOfTotalDelent) != numberOfEntityPrinted) {
        printf("incoherence 1\n");
        exit(1);
    }
    printf("\n");
    printf("All relation names registered: \n");
    if (relationTreeHead == &nullRelationNode) {
        printf("\tnone\n");
    }
    printAllRelationRecursive(relationTreeHead);
    printf("\n");
    printf("amount of relations received printed in ALBERO 1: %d\n", numberOfRelationReceivedPrinted_1);
    printf("amount of relations received printed in ALBERO 2: %d\n", numberOfRelationReceivedPrinted_2);
    printf("amount of well defined addrel executed: %d\n", numberOfTotalAddrel);
    printf("amount of well defined delrel executed: %d\n", numberOfTotalDelrel);
    printf("amount of total current relations registered: %d\n", (numberOfTotalAddrel - numberOfTotalDelrel));

    if (numberOfRelationReceivedPrinted_2 != numberOfRelationReceivedPrinted_1) {
        printf("incoherence 2\n");
        exit(1);
    }
    if ((numberOfTotalAddrel - numberOfTotalDelrel) != numberOfRelationReceivedPrinted_2) {
        printf("incoherence 3\n");
        exit(1);
    }
}
void report() {

    alreadyPrintedSomething = 0;
    printReportRecursive(relationTreeHead);
    if(reportDone == 0){
        fputs("none", stdout);
    }
    reportDone = 0;
    fputs("\n", stdout);
}