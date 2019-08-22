#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define RED 1
#define BLACK 0

#define STRING_DIMENSION 400
#define ENTITY_HASHTABLE_DIMENSION 307

int numberOfTotalAddrel = 0;
int numberOfTotalDelrel = 0;
int numberOfTotalAddent = 0;
int numberOfTotalDelent = 0;

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
    struct relationTracker_1 * relationTrackerEntityTreeHead;
};

struct hashCell{
    struct hashChain * hashChain;
};

struct hashChain{
    struct hashChain * nextHashChain;
    struct entity * entity;
};

/* relation (stella) */
// è in un albero ordinato alfanumericamente e con possibiltà di essere iterato
// stringa che rappresenta la relazione
// riferimento all'albero 2 di relation tracker

struct relation {
    char name[STRING_DIMENSION];
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
    unsigned int numberOfRelationReceived;
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
    struct relationSentToOrReceivedFrom * right;
    struct relationSentToOrReceivedFrom * left;
    struct relationSentToOrReceivedFrom * father;
    int nodeColor:1;
    struct entity * internalEntity;
    struct relationTracker_1 * externalRelationTracker;
};



/*declaration*/

struct hashCell entityHashTable[ENTITY_HASHTABLE_DIMENSION];
int numberOfEntityInHashTable = 0; //temporaneo

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

void insertEntityHashTable(char entity[STRING_DIMENSION]);
void deleteEntityHashTable(char entity[STRING_DIMENSION]);
struct entity * searchEntityHashTable(char entity[STRING_DIMENSION]);
unsigned long hashFunction(char* str);

struct relationTracker_1 *  searchRelationTracker_1(char * idRelation, struct entity * entity);
void leftRotateRelationTracker_1(struct relationTracker_1 * originNode, struct entity * entity);
void rightRotateRelationTracker_1(struct relationTracker_1 * originNode, struct entity * entity);
void insertRelationTracker_1(struct entity * entity, struct relationTracker_1 * newNode);
void insertRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker_1 * newNode);
struct relationTracker_1 * minimumRelationTracker_1(struct relationTracker_1 * node);
struct relationTracker_1 * successorRelationTracker_1(struct relationTracker_1 * node);
void deleteRelationTracker_1(struct entity * entity, struct relationTracker_1 * node);
void deleteRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker_1 * node);


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

struct relationSentToOrReceivedFrom * searchRelationSentToOrReceivedFrom(char * externalId, struct relationSentToOrReceivedFrom * head);
void leftRotateRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void rightRotateRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void insertRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void insertRelationSentToOrReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
struct relationSentToOrReceivedFrom * minimumRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x);
struct relationSentToOrReceivedFrom * successorRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x);
void deleteRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void deleteRelationSentToOrReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);


/* entity (quadrato) */
// è in una hash table
// stringa che la rappresenta
// riferimento all'albero delle sue relation tracker

unsigned long hashFunction(char* word) {
    /*SDBMHash*/
    /*
    unsigned int hash = 0;
    unsigned int i = 0;
    while(1){
        if(word[i] == '\0'){
            break;
        }
        hash = (*word) + (hash << 6) + (hash << 16) - hash;
        word++;
        i++;
    }
     return hash;
    */

    /*djb2Hash*/
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash);
    }
    return hash % ENTITY_HASHTABLE_DIMENSION;
}

struct entity * searchEntityHashTable(char entity[STRING_DIMENSION]){
    unsigned int hash = hashFunction(entity) % ENTITY_HASHTABLE_DIMENSION;

    struct hashChain * currentElement = entityHashTable[hash].hashChain;
    while(currentElement != NULL){
        if(strcmp(currentElement->entity->name, entity) == 0){
            //printf("entity %s found in the hash table\n", entity);
            return currentElement->entity;
        }
        currentElement = currentElement->nextHashChain;
    }
    //printf("entity %s not found in the hash table\n", entity);
    return NULL;
}

void insertEntityHashTable(char entity[STRING_DIMENSION]){
    unsigned int hash = hashFunction(entity) % ENTITY_HASHTABLE_DIMENSION;

    struct hashChain * currentHashChainElement = entityHashTable[hash].hashChain;

    if(entityHashTable[hash].hashChain == NULL){
        //FIRST TIME ADD ELEMENT IN THAT HASH CELL
        entityHashTable[hash].hashChain = malloc(sizeof(struct hashChain));
        (*entityHashTable[hash].hashChain).nextHashChain = NULL;
        (*entityHashTable[hash].hashChain).entity = malloc(sizeof(struct entity));
        (*entityHashTable[hash].hashChain).entity->relationTrackerEntityTreeHead = &nullRelationTrackerNode_1;
        strcpy((*(*entityHashTable[hash].hashChain).entity).name, entity);
        numberOfEntityInHashTable++;
        //printf("entity added as first ever element of the hash chain\n");
        numberOfTotalAddent++;
        return;
    }
    else{
        while(currentHashChainElement != NULL){
            if(strcmp(currentHashChainElement->entity->name, entity) == 0){
                //ALREADY EXISTS
                //printf("entity already exists, not adding\n");
                return;
            }
            currentHashChainElement = currentHashChainElement->nextHashChain;
        }
        //ADD ELEMENT TO THE END OF THE HASH CHAIN
        struct hashChain * new = malloc(sizeof(struct hashChain));
        new->nextHashChain = entityHashTable[hash].hashChain;
        new->entity = malloc(sizeof(struct entity));
        memcpy(new->entity->name, entity, sizeof(char[STRING_DIMENSION]));
        new->entity->relationTrackerEntityTreeHead = &nullRelationTrackerNode_1;
        entityHashTable[hash].hashChain = new;
        numberOfEntityInHashTable++;
        //printf("entity added as last element of the hash chain\n");
        numberOfTotalAddent++;
        return;
    }
}

void deleteEntityHashTable(char entity[STRING_DIMENSION]){
    unsigned int hash = hashFunction(entity) % ENTITY_HASHTABLE_DIMENSION;

    if(entityHashTable[hash].hashChain == NULL){
        //ELEMENT DOESN'T EXISTS
        //printf("entity doesn't exists, the hash chain is empty\n");
        return;
    }
    else{
        struct hashChain * currentHashChainElement = entityHashTable[hash].hashChain;

        //check if it is the first element
        if(strcmp((*currentHashChainElement).entity->name, entity) == 0) {
            //ELEMENT FOUND, DELETE AND FREE MEMORY
            entityHashTable[hash].hashChain = (*currentHashChainElement).nextHashChain;
            free(currentHashChainElement->entity);
            free(currentHashChainElement);
            numberOfEntityInHashTable--;
            //printf("entity deleted, it was the first element in the hash chain\n");
            return;
        }

        //set previous and current to iterate the chain
        struct hashChain * previousHashChainElement = entityHashTable[hash].hashChain;
        if((*currentHashChainElement).nextHashChain != NULL){
            currentHashChainElement = (*currentHashChainElement).nextHashChain;
        }
        else{
            //ELEMENT DOESN'T EXISTS
            //printf("entity doesn't exists\n");
            return;
        }

        //iterate the chain
        while(currentHashChainElement->nextHashChain != NULL){
            if (strcmp((*currentHashChainElement).entity->name, entity) == 0) {
                //ELEMENT FOUND, DELETE AND FREE MEMORY
                (*previousHashChainElement).nextHashChain = (*currentHashChainElement).nextHashChain;
                free(currentHashChainElement->entity);
                free(currentHashChainElement);
                numberOfEntityInHashTable--;
                //printf("entity deleted from the hash chain\n");
                return;
            }
            previousHashChainElement = currentHashChainElement;
            currentHashChainElement = currentHashChainElement->nextHashChain;
        }
        if (strcmp((*currentHashChainElement).entity->name, entity) == 0) {
            //ELEMENT FOUND, DELETE AND FREE MEMORY
            (*previousHashChainElement).nextHashChain = (*currentHashChainElement).nextHashChain;
            free(currentHashChainElement->entity);
            free(currentHashChainElement);
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

void deleteRelationTracker_1(struct entity * entity, struct relationTracker_1 * node){
    struct relationTracker_1 * x;
    struct relationTracker_1 * y;

    if(node->leftNodeEntityTree == &nullRelationTrackerNode_1 || node->rightNodeEntityTree == &nullRelationTrackerNode_1){
        y = node;
    }
    else{
        y = successorRelationTracker_1(node);
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
    if( y != node){
        node->relation = y->relation;
        node->entity = y->entity;
        node->numberOfRelationReceived = y->numberOfRelationReceived;
        node->receivedFromTreeHead = y->receivedFromTreeHead;
        node->sentToTreeHead = y->sentToTreeHead;
        node->relationTracker_2 = y->relationTracker_2;
        y->relationTracker_2->relationTracker_1 = node;
    }
    if(y->nodeColorEntityTree == BLACK){
        deleteRelationTracker_1_FIXUP(entity, x);
    }
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
        z->relationTracker_1 = y->relationTracker_1;
        z->relationTracker_1->relationTracker_2 = z;
    }
    if(y->nodeColorRelationTree == BLACK){
        deleteRelationTracker_2_FIXUP(relation, x);
    }
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

struct relationSentToOrReceivedFrom * searchRelationSentToOrReceivedFrom(char * externalId, struct relationSentToOrReceivedFrom * head){
    struct relationSentToOrReceivedFrom *result = head;

    while (result != &nullRelationSentToOrReceivedFromNode) {
        int cmp = strcmp(result->externalRelationTracker->entity->name, externalId);
        if (cmp == 0) {
            //printf("relationSentToOrReceivedFrom referred to entity %s found\n", externalId);
            return result;
        } else if (cmp > 0) {
            result = result->left;
        } else {
            result = result->right;
        }
    }
    //printf("relationSentToOrReceivedFrom referred to entity %s not found\n", externalId);
    return result;
}

void leftRotateRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = x->right;
    x->right = y->left;
    if(y->left != &nullRelationSentToOrReceivedFromNode){
        y->left->father = x;
    }
    y->father = x->father;
    if(x->father == &nullRelationSentToOrReceivedFromNode){
        *head = y;
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

void rightRotateRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = x->left;
    x->left = y->right;
    if(y->right != &nullRelationSentToOrReceivedFromNode){
        y->right->father = x;
    }
    y->father = x->father;
    if(x->father == &nullRelationSentToOrReceivedFromNode){
        *head = y;
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

void insertRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y = &nullRelationSentToOrReceivedFromNode;
    struct relationSentToOrReceivedFrom * x = *head;
    while(x != &nullRelationSentToOrReceivedFromNode){
        y = x;
        if(strcmp(z->externalRelationTracker->entity->name, x->externalRelationTracker->entity->name) < 0){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    z->father = y;
    if(y == &nullRelationSentToOrReceivedFromNode){
        *head = z;
    }
    else if(strcmp(z->externalRelationTracker->entity->name, y->externalRelationTracker->entity->name) < 0){
        y->left = z;
    }
    else{
        y->right = z;
    }
    z->left = &nullRelationSentToOrReceivedFromNode;
    z->right = &nullRelationSentToOrReceivedFromNode;
    z->nodeColor = RED;
    insertRelationSentToOrReceivedFrom_FIXUP(z, head);
}

void insertRelationSentToOrReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y;
    struct relationSentToOrReceivedFrom * x;
    if(z == *head){
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
                    insertRelationSentToOrReceivedFrom_FIXUP(x->father, head);
                }
                else{
                    if(z == x->right){
                        z = x;
                        leftRotateRelationSentToOrReceivedFrom(z, head);
                        x = z->father;
                    }
                    x->nodeColor = BLACK;
                    x->father->nodeColor = RED;
                    rightRotateRelationSentToOrReceivedFrom(x->father, head);
                }
            }
            else{
                y = x->father->left;
                if(y->nodeColor == RED){
                    x->nodeColor = BLACK;
                    y->nodeColor = BLACK;
                    x->father->nodeColor = RED;
                    insertRelationSentToOrReceivedFrom_FIXUP(x->father, head);
                }
                else{
                    if(z == x->left){
                        z = x;
                        rightRotateRelationSentToOrReceivedFrom(z, head);
                        x = z->father;
                    }
                    x->nodeColor = BLACK;
                    x->father->nodeColor = RED;
                    leftRotateRelationSentToOrReceivedFrom(x->father, head);
                }
            }
        }
    }
}

struct relationSentToOrReceivedFrom * minimumRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x){
    while (x->left != &nullRelationSentToOrReceivedFromNode){
        x = x->left;
    }
    return x;
}

struct relationSentToOrReceivedFrom * successorRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x){
    struct relationSentToOrReceivedFrom * y;
    if(x->right != &nullRelationSentToOrReceivedFromNode){
        return minimumRelationSentToOrReceivedFrom(x->right);
    }
    y = x->father;
    while((y!=&nullRelationSentToOrReceivedFromNode) && (x == y->right)){
        x = y;
        y = y->father;
    }
    return y;
}

void deleteRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y;
    struct relationSentToOrReceivedFrom * x;
    if((z->left == &nullRelationSentToOrReceivedFromNode) || (z->right == &nullRelationSentToOrReceivedFromNode)){
        y = z;
    }
    else{
        y = successorRelationSentToOrReceivedFrom(z);
    }
    if(y->left != &nullRelationSentToOrReceivedFromNode){
        x = y->left;
    }
    else{
        x = y->right;
    }
    x->father = y->father;
    if(y->father == &nullRelationSentToOrReceivedFromNode){
        *head = x;
    }
    else if(y == y->father->left){
        y->father->left = x;
    }
    else{
        y->father->right = x;
    }
    if (y != z){
        z->externalRelationTracker = y->externalRelationTracker;
        z->internalEntity = y->internalEntity;
    }
    if(y->nodeColor == BLACK){
        deleteRelationSentToOrReceivedFrom_FIXUP(x, head);
    }
}

void deleteRelationSentToOrReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * w;

    if((x->nodeColor == RED) || (x->father == &nullRelationSentToOrReceivedFromNode)){
        x->nodeColor = BLACK;
    }
    else if(x == x->father->left){
        w = x->father->right;
        if(w->nodeColor == RED){
            w->nodeColor = BLACK;
            x->father->nodeColor = RED;
            leftRotateRelationSentToOrReceivedFrom(x->father, head);
            w = x->father->right;
        }
        if((w->left->nodeColor == BLACK) && (w->right->nodeColor == BLACK)){
            w->nodeColor = RED;
            deleteRelationSentToOrReceivedFrom_FIXUP(x->father, head);
        }
        else{
            if(w->right->nodeColor == BLACK){
                w->left->nodeColor = BLACK;
                w->nodeColor = RED;
                rightRotateRelationSentToOrReceivedFrom(w,head);
                w = x->father->right;
            }
            w->nodeColor = x->father->nodeColor;
            x->father->nodeColor = BLACK;
            w->right->nodeColor = BLACK;
            rightRotateRelationSentToOrReceivedFrom(x->father, head);
        }
    }
    else{
        w = x->father->left;
        if(w->nodeColor == RED){
            w->nodeColor = BLACK;
            x->father->nodeColor = RED;
            rightRotateRelationSentToOrReceivedFrom(x->father,head);
            w = x->father->left;
        }
        if((w->right->nodeColor == BLACK) && (w->left->nodeColor == BLACK)){
            w->nodeColor = RED;
            deleteRelationSentToOrReceivedFrom_FIXUP(x->father,head);
        }
        else{
            if(w->left->nodeColor == BLACK){
                w->right->nodeColor = BLACK;
                w->nodeColor = RED;
                leftRotateRelationSentToOrReceivedFrom(w, head);
                w = x->father->left;
            }
            w->nodeColor = x->father->nodeColor;
            x->father->nodeColor = BLACK;
            w->left->nodeColor = BLACK;
            rightRotateRelationSentToOrReceivedFrom(x->father, head);
        }
    }
}


/* main */
int main(void) {

    //initilizing hash table
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION ; ++i) {
        entityHashTable[i].hashChain = NULL;
    }

    relationTreeHead = &nullRelationNode;

    char input[STRING_DIMENSION];

    while(1){
        scanf("%s", input);
        if(strcmp(input, "addent") == 0){
            //printf("******************* ADDENT *******************************************\n");
            addent();
            //printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "addrel") == 0){
            //printf("******************* ADDREL *******************************************\n");
            addrel();
            //printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "delent") == 0){
            //printf("******************* DELENT *******************************************\n");
            delent();
            //printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "delrel") == 0){
            //printf("******************* DELREL *******************************************\n");
            delrel();
            //printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "report") == 0){
            //printf("******************* REPORT *******************************************\n");
            report();
            //printf("**********************************************************************\n\n");
        }
        else { //end
            //printf("******************* END **********************************************\n");
            //printf("ending line input was: %s\n", input);
            //report();
            //printf("**********************************************************************\n\n");

            return 0;
        }
    }
}

void addent(){
    scanf("%s", idEnt);
    //printf("idEnt: %s - hash: %lu\n", idEnt, hashFunction(idEnt) % ENTITY_HASHTABLE_DIMENSION);

    insertEntityHashTable(idEnt);

    //O(1):  aggiunge entity all'hash table delle entities se non esiste già
    //
    //TOTAL: O(1)
}

void addrel(){

    scanf("%s", idOrig);
    //printf("idOrig: %s\n", idOrig);
    scanf("%s", idDest);
    //printf("idDest: %s\n", idDest);
    scanf("%s", idRel);
    //printf("idRel: %s\n", idRel);

    struct entity *entityOrig = searchEntityHashTable(idOrig);
    if (entityOrig == NULL) {
        //printf("entity %s doesn't exists\n", idOrig);
        return;
    }
    struct entity *entityDest = searchEntityHashTable(idDest);
    if (entityDest == NULL) {
        //printf("entity %s doesn't exists\n", idDest);
        return;
    }

    struct relation * relation = searchRelation(idRel);
    if (relation == &nullRelationNode) {
        relation = malloc(sizeof(struct relation));
        relation->left = &nullRelationNode;
        relation->right = &nullRelationNode;
        relation->father = &nullRelationNode;
        relation->nodeColor = BLACK;
        strcpy(relation->name, idRel);
        relation->relationTrackerTreeHead = &nullRelationTrackerNode_2;

        insertRelation(relation);

        //printf("first time relation %s has been used, added to the relation tree\n", idRel);
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
        relTrackerOrig->relation = relation;
        relTrackerOrig->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
        relTrackerOrig->relationTracker_2->relationTracker_1=relTrackerOrig;
        relTrackerOrig->relationTracker_2->fatherRelationTree = &nullRelationTrackerNode_2;
        relTrackerOrig->relationTracker_2->rightNodeRelationTree = &nullRelationTrackerNode_2;
        relTrackerOrig->relationTracker_2->leftNodeRelationTree = &nullRelationTrackerNode_2;
        //printf("relation tracker origin created\n");

        insertRelationTracker_1(entityOrig, relTrackerOrig);
        insertRelationTracker_2(relation, relTrackerOrig->relationTracker_2);
        //printf("added relationTracker about %s to entity %s in ALBERO 1 and ALBERO 2\n", idRel, idOrig);
    }

    struct relationTracker_1 *relTrackerDest;
    if(entityDest == entityOrig){
        relTrackerDest = relTrackerOrig;
        //printf("entity orig and dest is the same (%s)\n", idOrig);
    }
    else {
        relTrackerDest = searchRelationTracker_1(idRel, entityDest);
        if (relTrackerDest == &nullRelationTrackerNode_1) {
            relTrackerDest = malloc(sizeof(struct relationTracker_1));
            relTrackerDest->leftNodeEntityTree = &nullRelationTrackerNode_1;
            relTrackerDest->rightNodeEntityTree = &nullRelationTrackerNode_1;
            relTrackerDest->fatherEntityTree = &nullRelationTrackerNode_1;
            relTrackerDest->sentToTreeHead = &nullRelationSentToOrReceivedFromNode;
            relTrackerDest->receivedFromTreeHead = &nullRelationSentToOrReceivedFromNode;
            relTrackerDest->entity = entityDest;
            relTrackerDest->relation = relation;
            relTrackerDest->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
            relTrackerDest->relationTracker_2->relationTracker_1=relTrackerDest;
            relTrackerDest->relationTracker_2->fatherRelationTree = &nullRelationTrackerNode_2;
            relTrackerDest->relationTracker_2->rightNodeRelationTree = &nullRelationTrackerNode_2;
            relTrackerDest->relationTracker_2->leftNodeRelationTree = &nullRelationTrackerNode_2;
            //printf("relation tracker destination created\n");

            insertRelationTracker_1(entityDest, relTrackerDest);
            insertRelationTracker_2(relation, relTrackerDest->relationTracker_2);
            //printf("added relationTracker about %s to entity %sin ALBERO 1 and ALBERO 2\n", idRel, idDest);
        }
    }

    struct relationSentToOrReceivedFrom *relationSentTo = searchRelationSentToOrReceivedFrom(entityDest->name, relTrackerOrig->sentToTreeHead);
    if(relationSentTo == &nullRelationSentToOrReceivedFromNode) {
        //printf("creating new relationSentTo\n");
        relationSentTo = malloc(sizeof(struct relationSentToOrReceivedFrom));
        relationSentTo->internalEntity = entityOrig;
        relationSentTo->externalRelationTracker = relTrackerDest;
        relationSentTo->left = &nullRelationSentToOrReceivedFromNode;
        relationSentTo->right = &nullRelationSentToOrReceivedFromNode;
        relationSentTo->father = &nullRelationSentToOrReceivedFromNode;
    }
    else{
        //printf("relation already exists\n");
        return;
    }
    insertRelationSentToOrReceivedFrom(relationSentTo, &(relTrackerOrig->sentToTreeHead));
    //printf("added relationSentTo to the orig (%s)\n", idOrig);

    //printf("creating new relationSentTo\n");
    struct relationSentToOrReceivedFrom *relationReceivedFrom = malloc(sizeof(struct relationSentToOrReceivedFrom));
    relationReceivedFrom->internalEntity = entityDest;
    relationReceivedFrom->externalRelationTracker = relTrackerOrig;
    relationReceivedFrom->left = &nullRelationSentToOrReceivedFromNode;
    relationReceivedFrom->right = &nullRelationSentToOrReceivedFromNode;
    relationReceivedFrom->father = &nullRelationSentToOrReceivedFromNode;

    insertRelationSentToOrReceivedFrom(relationReceivedFrom, &(relTrackerDest->receivedFromTreeHead));
    //printf("added relationReceivedFrom to the dest (%s)\n", idDest);

    deleteRelationTracker_2(relation, relTrackerDest->relationTracker_2);
    relTrackerDest->numberOfRelationReceived++;
    relTrackerDest->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
    relTrackerDest->relationTracker_2->relationTracker_1 = relTrackerDest;
    relTrackerDest->relationTracker_2->rightNodeRelationTree = &nullRelationTrackerNode_2;
    relTrackerDest->relationTracker_2->leftNodeRelationTree = &nullRelationTrackerNode_2;
    relTrackerDest->relationTracker_2->fatherRelationTree = &nullRelationTrackerNode_2;
    insertRelationTracker_2(relation, relTrackerDest->relationTracker_2);
    //printf("incremented numberOfRelationReceived of relation %s of entity dest %s\n", idRel, idDest);

    numberOfTotalAddrel++;


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

void deleteAllRelationTrackerRecursive_1(struct relationTracker_1 * currentRelationTracker);
void deleteAllRelationSentToRecursive(struct relationSentToOrReceivedFrom * currentRelationSentTo);
void deleteAllRelationReceivedFromRecursive(struct relationSentToOrReceivedFrom * currentRelationReceivedFrom);
void deleteAllRelationTrackerRecursive_1(struct relationTracker_1 * currentRelationTracker){
    if(currentRelationTracker != &nullRelationTrackerNode_1){
        deleteAllRelationTrackerRecursive_1(currentRelationTracker->leftNodeEntityTree);

        //printf("\tanalizing relationTracker: %s\n", currentRelationTracker->relation->name);
        deleteAllRelationSentToRecursive(currentRelationTracker->sentToTreeHead);
        deleteAllRelationReceivedFromRecursive(currentRelationTracker->receivedFromTreeHead);
        deleteRelationTracker_2(currentRelationTracker->relation, currentRelationTracker->relationTracker_2);

        deleteAllRelationTrackerRecursive_1(currentRelationTracker->rightNodeEntityTree);
    }
}
void deleteAllRelationSentToRecursive(struct relationSentToOrReceivedFrom * currentRelationSentTo){
    if(currentRelationSentTo != &nullRelationSentToOrReceivedFromNode){
        deleteAllRelationSentToRecursive(currentRelationSentTo->left);

        //printf("\t\t%s->%s\n", currentRelationSentTo->internalEntity->name, currentRelationSentTo->externalRelationTracker->entity->name);
        struct relationSentToOrReceivedFrom * relationReceivedFromExternalEntity = searchRelationSentToOrReceivedFrom(
                currentRelationSentTo->internalEntity->name,
                currentRelationSentTo->externalRelationTracker->receivedFromTreeHead
        );
        deleteRelationSentToOrReceivedFrom(
                relationReceivedFromExternalEntity,
                &(currentRelationSentTo->externalRelationTracker->receivedFromTreeHead)
        );
        //printf("\t\t\tdeleted from the external entity (%s) the relationReceivedFrom\n", currentRelationSentTo->externalRelationTracker->entity->name);

        deleteRelationTracker_2(currentRelationSentTo->externalRelationTracker->relation, currentRelationSentTo->externalRelationTracker->relationTracker_2);
        currentRelationSentTo->externalRelationTracker->numberOfRelationReceived--;
        currentRelationSentTo->externalRelationTracker->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
        currentRelationSentTo->externalRelationTracker->relationTracker_2->relationTracker_1 = currentRelationSentTo->externalRelationTracker;
        currentRelationSentTo->externalRelationTracker->relationTracker_2->rightNodeRelationTree = &nullRelationTrackerNode_2;
        currentRelationSentTo->externalRelationTracker->relationTracker_2->leftNodeRelationTree = &nullRelationTrackerNode_2;
        currentRelationSentTo->externalRelationTracker->relationTracker_2->fatherRelationTree = &nullRelationTrackerNode_2;
        insertRelationTracker_2(currentRelationSentTo->externalRelationTracker->relation, currentRelationSentTo->externalRelationTracker->relationTracker_2);
        //printf("\t\t\tdiminished the number of relation received from the relationTracker of the dest (%s)\n", currentRelationSentTo->externalRelationTracker->entity->name);
        numberOfTotalDelrel++;

        deleteAllRelationSentToRecursive(currentRelationSentTo->right);
    }
}
void deleteAllRelationReceivedFromRecursive(struct relationSentToOrReceivedFrom * currentRelationReceivedFrom){
    if(currentRelationReceivedFrom != &nullRelationSentToOrReceivedFromNode){
        deleteAllRelationReceivedFromRecursive(currentRelationReceivedFrom->left);

        //printf("\t\t%s<-%s\n", currentRelationReceivedFrom->internalEntity->name, currentRelationReceivedFrom->externalRelationTracker->entity->name);
        struct relationSentToOrReceivedFrom * relationSentToExternalEntity = searchRelationSentToOrReceivedFrom(
                currentRelationReceivedFrom->internalEntity->name,
                currentRelationReceivedFrom->externalRelationTracker->sentToTreeHead
        );
        deleteRelationSentToOrReceivedFrom(
                relationSentToExternalEntity,
                &(currentRelationReceivedFrom->externalRelationTracker->sentToTreeHead)
        );
        //printf("\t\t\tdeleted from the external entity (%s) the relationSentTo\n", currentRelationReceivedFrom->externalRelationTracker->entity->name);
        numberOfTotalDelrel++;

        deleteAllRelationReceivedFromRecursive(currentRelationReceivedFrom->right);
    }
}

void delent(){

    scanf("%s", idEnt);
    //printf("idEnt: %s - hash: %lu\n", idEnt, hashFunction(idEnt) % ENTITY_HASHTABLE_DIMENSION);

    struct entity * entity = searchEntityHashTable(idEnt);
    if(entity == NULL){
        //printf("couldn't find the entity in the hash table\n");
        return;
    }

    deleteAllRelationTrackerRecursive_1(entity->relationTrackerEntityTreeHead);

    deleteEntityHashTable(idEnt);

    numberOfTotalDelent++;

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
    //printf("idOrig: %s\n", idOrig);
    scanf("%s", idDest);
    //printf("idDest: %s\n", idDest);
    scanf("%s", idRel);
    //printf("idRel: %s\n", idRel);

    struct entity * entityOrig = searchEntityHashTable(idOrig);
    if(entityOrig == NULL){
        //printf("entity %s doesn't exists\n", idOrig);
        return;
    }
    struct entity * entityDest = searchEntityHashTable(idDest);
    if(entityDest == NULL){
        //printf("entity %s doesn't exists\n", idDest);
        return;
    }

    struct relation * relation = searchRelation(idRel);
    if(relation == &nullRelationNode){
        //printf("relation %s doesn't exists in entities %s and %s\n", idRel, idOrig, idDest);
        //printf("\tnote: couldn't find relation\n");
        return;
    }

    struct relationTracker_1 * relTrackerOrig = searchRelationTracker_1(idRel, entityOrig);
    if(relTrackerOrig == &nullRelationTrackerNode_1){
        //printf("relation %s doesn't exists in entities %s and %s\n", idRel, idOrig, idDest);
        //printf("\tnote: couldn't find relationTracker orig\n");
        return;
    }

    struct relationSentToOrReceivedFrom * relationSentTo = searchRelationSentToOrReceivedFrom(idDest, relTrackerOrig->sentToTreeHead);
    if(relationSentTo == &nullRelationSentToOrReceivedFromNode){
        //printf("relation %s doesn't exists in entities %s and %s\n", idRel, idOrig, idDest);
        //printf("\tnote: couldn't find relationSentTo from the relationTracker orig\n");
        return;
    }

    struct relationTracker_1 * relTrackerDest = relationSentTo->externalRelationTracker;

    struct relationSentToOrReceivedFrom * relationReceivedFrom = searchRelationSentToOrReceivedFrom(idOrig, relTrackerDest->receivedFromTreeHead);

    deleteRelationSentToOrReceivedFrom(relationReceivedFrom, &(relTrackerDest->receivedFromTreeHead));
    //printf("deleted the relationReceivedFrom from the dest\n");

    deleteRelationTracker_2(relation, relTrackerDest->relationTracker_2);
    relTrackerDest->numberOfRelationReceived--;
    relTrackerDest->relationTracker_2 = malloc(sizeof(struct relationTracker_2));
    relTrackerDest->relationTracker_2->relationTracker_1 = relTrackerDest;
    relTrackerDest->relationTracker_2->rightNodeRelationTree = &nullRelationTrackerNode_2;
    relTrackerDest->relationTracker_2->leftNodeRelationTree = &nullRelationTrackerNode_2;
    relTrackerDest->relationTracker_2->fatherRelationTree = &nullRelationTrackerNode_2;
    insertRelationTracker_2(relation, relTrackerDest->relationTracker_2);
    //printf("decremented numberOfRelationReceived of relation %s of entity dest %s\n", idRel, idDest);

    deleteRelationSentToOrReceivedFrom(relationSentTo, &(relTrackerOrig->sentToTreeHead));
    //printf("deleted the relationReceivedFrom from the orig\n");

    numberOfTotalDelrel++;

    //O(1):     cerca se le entities esistono nell'hash table, se non esistono non fa nulla
    //O(log k): dalla entity origin accede all'albero 1 delle relation tracker e cerca la relation da eliminare, se non esiste non fa nulla
    //O(log n): una volta trovata la relation, accede all'albero delle entity sent to e cerca quella corretta
    //O(log n): una volta trovata, accede alla relation tracker del destinatario e cerca nell'albero delle relation received from il nodo da eliminare
    //O(1):     elimina tutto ciò che rimane da eliminare
    //
    //TOTAL:    O(1) + O(log k) + O(log n) + O(log n) + O(1)
    // k= numero di relation tracker dell'entity considerata
    // n= dimesione dell'albero delle relation sent to o relation received from
    // j= dimensione dell'albero 2 di relation tracker della relation considerata
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
        printAllRelationSentToRecursive(x->left);
        //printf("\t\t\t%s->%s\n", x->internalEntity->name, x->externalRelationTracker->entity->name);
        printAllRelationSentToRecursive(x->right);
    }
}
void printALlRelationReceivedFromRecursive(struct relationSentToOrReceivedFrom * x){
    if(x != &nullRelationSentToOrReceivedFromNode){
        printALlRelationReceivedFromRecursive(x->left);
        //printf("\t\t\t%s<-%s\n", x->internalEntity->name, x->externalRelationTracker->entity->name);
        numberOfRelationReceivedPrinted_1++;
        printALlRelationReceivedFromRecursive(x->right);
    }
}
void printAllRelationTrackerRecursive_1(struct relationTracker_1 * x){
    if(x != &nullRelationTrackerNode_1){
        printAllRelationTrackerRecursive_1(x->leftNodeEntityTree);
        //printf("\t\t%s | relation received: %d\n", x->relation->name, x->numberOfRelationReceived);
        printALlRelationReceivedFromRecursive(x->receivedFromTreeHead);
        printAllRelationSentToRecursive(x->sentToTreeHead);
        printAllRelationTrackerRecursive_1(x->rightNodeEntityTree);
    }
}
void printAllRelationTrackerRecursive_2(struct relationTracker_2 * x){
    if(x!= &nullRelationTrackerNode_2){
        printAllRelationTrackerRecursive_2(x->leftNodeRelationTree);
        //printf("\t\t%s has received %d\n", x->relationTracker_1->entity->name, x->relationTracker_1->numberOfRelationReceived);
        numberOfRelationReceivedPrinted_2 += x->relationTracker_1->numberOfRelationReceived;
        printAllRelationTrackerRecursive_2(x->rightNodeRelationTree);
    }
}
void printAllRelationRecursive(struct relation * x){
    if(x != &nullRelationNode){
        printAllRelationRecursive(x->left);
        //printf("\t%s\n", x->name);
        printAllRelationTrackerRecursive_2(x->relationTrackerTreeHead);
        printAllRelationRecursive(x->right);
    }
}

struct stringList * newStringList(char * str);
void printReportRecursive(struct relation * relation);
void printList(struct stringList * head);
void sortedInsert(struct stringList** head_ref, struct stringList* new_node);
int alreadyPrintedSomething = 0;
struct stringList {
    char name [50];
    struct stringList * next;
};
struct stringList * newStringList(char * str){
    struct stringList * newStringList = malloc(sizeof(struct stringList));

    strcpy(newStringList->name, str);
    newStringList->next = NULL;

    return newStringList;
}
void printList(struct stringList * head){
    struct stringList *temp = head;
    while(temp != NULL)
    {
        printf("%s  ", temp->name);
        temp = temp->next;
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
                    printf(" ");
                    fflush(stdout);
                }
                alreadyPrintedSomething = 1;
                printf("%s ", max->relationTracker_1->relation->name);
                fflush(stdout);

                struct stringList * head = NULL;
                while (max->fatherRelationTree != &nullRelationTrackerNode_2 && quantity == max->fatherRelationTree->relationTracker_1->numberOfRelationReceived) {
                    sortedInsert(&head, newStringList(max->relationTracker_1->entity->name));
                    max = max->fatherRelationTree;
                }
                sortedInsert(&head, newStringList(max->relationTracker_1->entity->name));

                //max here is the top node of the sub tree of the max
                printList(head);

                printf("%d;", quantity);
                fflush(stdout);
            }
        }

        printReportRecursive(relation->right);
    }
}

void report() {
    int numberOfEntityPrinted = 0;
    numberOfRelationReceivedPrinted_1 = 0;
    numberOfRelationReceivedPrinted_2 = 0;
    for (int i = 0; i < ENTITY_HASHTABLE_DIMENSION; ++i) {
        if (entityHashTable[i].hashChain != NULL) {

            //printf("hash table cell number [%d]\n", i);

            struct hashChain *tempEnt = entityHashTable[i].hashChain;
            while (tempEnt != NULL) {

                //printf("\tentity: %s\n", tempEnt->entity->name);
                numberOfEntityPrinted++;

                printAllRelationTrackerRecursive_1(tempEnt->entity->relationTrackerEntityTreeHead);

                tempEnt = tempEnt->nextHashChain;
            }

            //printf("\n");
        }
    }


    //printf("hash table dimension: %d\n", ENTITY_HASHTABLE_DIMENSION);
    //printf("amount of entities stored in the hash table: %d\n", numberOfEntityInHashTable);
    //printf("amount of entities printed: %d\n", numberOfEntityPrinted );
    //printf("amount of well formed addent executed: %d\n", numberOfTotalAddent);
    //printf("amount of well formed delent executed: %d\n", numberOfTotalDelent);
    //printf("amount of total current entities registered: %d\n", (numberOfTotalAddent - numberOfTotalDelent));
    if (numberOfEntityInHashTable != numberOfEntityPrinted) {
        //printf("incoherence 0\n");
    }
    if ((numberOfTotalAddent - numberOfTotalDelent) != numberOfEntityPrinted) {
        //printf("incoherence 1\n");
    }
    //printf("\n");
    //printf("All relation names registered: \n");
    if (relationTreeHead == &nullRelationNode) {
        //printf("\tnone\n");
    }
    printAllRelationRecursive(relationTreeHead);
    //printf("\n");
    //printf("amount of relations received printed in ALBERO 1: %d\n", numberOfRelationReceivedPrinted_1);
    //printf("amount of relations received printed in ALBERO 2: %d\n", numberOfRelationReceivedPrinted_2);
    //printf("amount of well defined addrel executed: %d\n", numberOfTotalAddrel);
    //printf("amount of well defined delrel executed: %d\n", numberOfTotalDelrel);
    //printf("amount of total current relations registered: %d\n", (numberOfTotalAddrel - numberOfTotalDelrel));

    if (numberOfRelationReceivedPrinted_2 != numberOfRelationReceivedPrinted_1) {
        //printf("incoherence 2\n");
    }
    if ((numberOfTotalAddrel - numberOfTotalDelrel) != numberOfRelationReceivedPrinted_2) {
        //printf("incoherence 3\n");
    }

    if (relationTreeHead != &nullRelationNode) {
        alreadyPrintedSomething = 0;
        printReportRecursive(relationTreeHead);
        printf("\n");
        fflush(stdout);
    }
    else{
        printf("none\n");
        fflush(stdout);
    }

    //O(k*n):  itera in ogni relation e stampa la lista di max
    //
    //TOTAL: O(k*n)
    // k= numero totale di tipi di relazioni esistenti
    // n= numero di parimerito con max numero direlazioni entranti
}