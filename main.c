#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define RED 1
#define BLACK 0

#define STRING_DIMENSION 400
#define ENTITY_HASHTABLE_DIMENSION 30

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
    int nodeColorEntityTree:1;

    struct relationTracker * fatherRelationTree;
    struct relationTracker * rightNodeRelationTree; // ALBERO 2
    struct relationTracker * leftNodeRelationTree;
    struct relationTracker * nextEqualNode;
    int nodeColorRelationTree:1;

    struct relationSentToOrReceivedFrom * sentToTreeHead;
    struct relationSentToOrReceivedFrom * receivedFromTreeHead;

    unsigned int numberOfRelationReceived;
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
    struct relationTracker * externalRelationTracker;
};



/*declaration*/

struct hashCell entityHashTable[ENTITY_HASHTABLE_DIMENSION];
int numberOfEntityInHashTable = 0; //temporaneo

struct relationTracker nullRelationTrackerNode;
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
unsigned int SDBMHash(char* str);

struct relationTracker *  searchRelationTracker_1(char * idRelation, struct entity * entity);
void leftRotateRelationTracker_1(struct relationTracker * originNode, struct entity * entity);
void rightRotateRelationTracker_1(struct relationTracker * originNode, struct entity * entity);
void insertRelationTracker_1(struct entity * entity, struct relationTracker * newNode);
void insertRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * newNode);
struct relationTracker * minimumRelationTracker_1(struct relationTracker * node);
struct relationTracker * successorRelationTracker_1(struct relationTracker * node);
void deleteRelationTracker_1(struct entity * entity, struct relationTracker * node);
void deleteRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * node);

struct relationTracker *  searchRelationTracker_2(char * idRelation, struct entity * entity);
void leftRotateRelationTracker_2(struct relationTracker * originNode, struct entity * entity);
void rightRotateRelationTracker_2(struct relationTracker * originNode, struct entity * entity);
void insertRelationTracker_2(struct entity * entity, struct relationTracker * newNode);
void insertRelationTracker_2_FIXUP(struct entity * entity, struct relationTracker * newNode);
struct relationTracker * minimumRelationTracker_2(struct relationTracker * node);
struct relationTracker * successorRelationTracker_2(struct relationTracker * node);
void deleteRelationTracker_2(struct entity * entity, struct relationTracker * node);
void deleteRelationTracker_2_FIXUP(struct entity * entity, struct relationTracker * node);

struct relation * searchRelation(char * idRelSearched);
void leftRotateRelation(struct relation * x);
void rightRotateRelation(struct relation * x);
void insertRelation(struct relation * z);
void insertRelation_FIXUP(struct relation * z);
struct relation * minimumRelation(struct relation * x);
struct relation * successorRelation(struct relation * x);
void deleteRelation(struct relation * z);
void deleteRelation_FIXUP(struct relation * x);

struct relationSentToOrReceivedFrom * searchRelationSentToOrReceivedFrom(char * id,  struct relationSentToOrReceivedFrom ** head);
void leftRotateRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void rightRotateRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void insertRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void insertRelationSentToOrReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
struct relationSentToOrReceivedFrom * minimumRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
struct relationSentToOrReceivedFrom * successorRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);
void deleteRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * z, struct relationSentToOrReceivedFrom ** head);
void deleteRelationSentToOrReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head);


/* entity (quadrato) */
// è in una hash table
// stringa che la rappresenta
// riferimento all'albero delle sue relation tracker

unsigned int SDBMHash(char* str) {
    unsigned int hash = 0;
    unsigned int i = 0;

    while(1){
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

    struct hashChain * currentHashChainElement = entityHashTable[hash].hashChain;

    if(entityHashTable[hash].hashChain == NULL){
        //FIRST TIME ADD ELEMENT IN THAT HASH CELL
        entityHashTable[hash].hashChain = malloc(sizeof(struct hashChain));
        (*entityHashTable[hash].hashChain).nextHashChain = NULL;
        (*entityHashTable[hash].hashChain).entity = malloc(sizeof(struct entity));
        (*entityHashTable[hash].hashChain).entity->relationTrackerEntityTreeHead = &nullRelationTrackerNode;
        strcpy((*(*entityHashTable[hash].hashChain).entity).name, entity);
        numberOfEntityInHashTable++;
        printf("entity added as first ever element of the hash chain\n");
        return;
    }
    else{
        while(currentHashChainElement != NULL){
            if(strcmp(currentHashChainElement->entity->name, entity) == 0){
                //ALREADY EXISTS
                printf("entity already exists, not adding\n");
                return;
            }
            currentHashChainElement = currentHashChainElement->nextHashChain;
        }
        //ADD ELEMENT TO THE END OF THE HASH CHAIN
        struct hashChain * new = malloc(sizeof(struct hashChain));
        new->nextHashChain = entityHashTable[hash].hashChain;
        new->entity = malloc(sizeof(struct entity));
        memcpy(new->entity->name, entity, sizeof(char[STRING_DIMENSION]));
        new->entity->relationTrackerEntityTreeHead = &nullRelationTrackerNode;
        entityHashTable[hash].hashChain = new;
        numberOfEntityInHashTable++;
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
            free(currentHashChainElement->entity);
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
                free(currentHashChainElement->entity);
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
            free(currentHashChainElement->entity);
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

struct relation * searchRelation(char * idRelSearched){
    struct relation * result = relationTreeHead;

    while (result != &nullRelationNode) {
        int cmp = strcmp(result->name, idRelSearched);

        if (cmp == 0) {
            printf("relation %s found\n", idRelSearched);
            return result;
        } else if (cmp < 0) {
            //go left
            result = result->left;
        } else {
            //go right
            result = result->right;
        }
    }
    printf("couldn't find the relation %s\n", idRelSearched);
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
struct relationTracker *  searchRelationTracker_1(char * idRelation, struct entity * entity){
    struct relationTracker * result = entity->relationTrackerEntityTreeHead;

    while (result != &nullRelationTrackerNode) {
        int cmp = strcmp(result->relation->name, idRelation);

        if (cmp == 0) {
            printf("relationTracker related to the relation %s and the entity %s found\n", idRelation, entity->name);
            return result;
        } else if (cmp < 0) {
            //go left
            result = result->leftNodeEntityTree;
        } else {
            //go right
            result = result->rightNodeEntityTree;
        }
    }
    printf("could'n find the relationTracker related to the relation %s and the entity %s\n", idRelation, entity->name);
    return result;
}

void leftRotateRelationTracker_1(struct relationTracker * originNode, struct entity * entity){
    struct relationTracker * temp = originNode->rightNodeEntityTree;
    originNode->rightNodeEntityTree = temp->leftNodeEntityTree;

    if(temp->leftNodeEntityTree != &nullRelationTrackerNode){
        temp->leftNodeEntityTree->fatherEntityTree = originNode;
    }
    temp->fatherEntityTree = originNode->fatherEntityTree;

    if(originNode->fatherEntityTree == &nullRelationTrackerNode){
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

    if(temp->rightNodeEntityTree != &nullRelationTrackerNode){
        temp->rightNodeEntityTree->fatherEntityTree = originNode;
    }
    temp->fatherEntityTree = originNode->fatherEntityTree;

    if(originNode->fatherEntityTree == &nullRelationTrackerNode){
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
    struct relationTracker * tempFather = &nullRelationTrackerNode;
    struct relationTracker * tempNewNode = entity->relationTrackerEntityTreeHead;
    while(tempNewNode != &nullRelationTrackerNode){
        tempFather = tempNewNode;
        if(strcmp(newNode->relation->name, tempNewNode->relation->name) < 0){
            tempNewNode = tempNewNode->leftNodeEntityTree;
        }
        else{
            tempNewNode = tempNewNode->rightNodeEntityTree;
        }
    }
    newNode->fatherEntityTree = tempFather;
    if(tempFather == &nullRelationTrackerNode){
        entity->relationTrackerEntityTreeHead = newNode;
    }
    else if(strcmp(newNode->relation->name, tempFather->relation->name) < 0){
        tempFather->leftNodeEntityTree = newNode;
    }
    else{
        tempFather->rightNodeEntityTree = newNode;
    }
    newNode->leftNodeEntityTree = &nullRelationTrackerNode;
    newNode->rightNodeEntityTree = &nullRelationTrackerNode;
    newNode->nodeColorEntityTree = RED;
    insertRelationTracker_1_FIXUP(entity, newNode);
}

void insertRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * newNode){
    struct relationTracker * tempFather;
    struct relationTracker * tempNewNode;
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

struct relationTracker * minimumRelationTracker_1(struct relationTracker * node){
    while(node->leftNodeEntityTree != &nullRelationTrackerNode){
        node = node->leftNodeEntityTree;
    }
    return  node;
}

struct relationTracker * successorRelationTracker_1(struct relationTracker * node){
    struct relationTracker * temp;
    if(node->rightNodeEntityTree != &nullRelationTrackerNode){
        return minimumRelationTracker_1(node->rightNodeEntityTree);
    }
    temp = node->fatherEntityTree;
    while(temp != &nullRelationTrackerNode && node == temp->rightNodeEntityTree){
        node = temp;
        temp = temp->fatherEntityTree;
    }
    return temp;
}

void deleteRelationTracker_1(struct entity * entity, struct relationTracker * node){
    struct relationTracker * x;
    struct relationTracker * y;

    if(node->leftNodeEntityTree == &nullRelationTrackerNode || node->rightNodeEntityTree == &nullRelationTrackerNode){
        y = node;
    }
    else{
        y = successorRelationTracker_1(node);
    }
    if(y->leftNodeEntityTree != &nullRelationTrackerNode){
        x = y->leftNodeEntityTree;
    }
    else{
        x = y->rightNodeEntityTree;
    }
    x->fatherEntityTree = y->fatherEntityTree;
    if(y->fatherEntityTree == &nullRelationTrackerNode){
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
}

void deleteRelationTracker_1_FIXUP(struct entity * entity, struct relationTracker * node){
    struct relationTracker * temp;
    if(node->nodeColorEntityTree == RED || node->fatherEntityTree == &nullRelationTrackerNode){
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
struct relationTracker *  searchRelationTracker_2(char * idRelation, struct entity * entity){
    return NULL;
}

void leftRotateRelationTracker_2(struct relationTracker * originNode, struct entity * entity){
    return;
}

void rightRotateRelationTracker_2(struct relationTracker * originNode, struct entity * entity){
    return;
}

void insertRelationTracker_2(struct entity * entity, struct relationTracker * newNode) {
    return;
}

void insertRelationTracker_2_FIXUP(struct entity * entity, struct relationTracker * newNode){
    return;
}

struct relationTracker * minimumRelationTracker_2(struct relationTracker * node){
    return NULL;
}

struct relationTracker * successorRelationTracker_2(struct relationTracker * node){
    return NULL;
}

void deleteRelationTracker_2(struct entity * entity, struct relationTracker * node){
    return;
}

void deleteRelationTracker_2_FIXUP(struct entity * entity, struct relationTracker * node){
    return;
}



/* relation sent to (rombo1) */
// nota: è accessibile dall'entity (che si sta eliminando)
// è in un albero ordinato in base all'indirizzo di memeoria della relation tracker receiver e con posibilità di essere iterato
// riferimento alla relation tracker receiver
/* relation received from (rombo2) */
// nota: è accessibile dall'entity destinataria
// è in un albero ordinato in base all'indirizzo di memoria della relation tracker origin e con possibilità di essere iterato
// riferimento alla relation tracker origin

struct relationSentToOrReceivedFrom * searchRelationSentToOrReceivedFrom(char * id, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * result = *head;

    while (result != &nullRelationSentToOrReceivedFromNode) {
        int cmp = strcmp(result->externalRelationTracker->entity->name, id);

        if (cmp == 0) {
            printf("relationSentToOrReceivedFrom referred to entity %s found\n", id);
            return result;
        } else if (cmp < 0) {
            //go left
            result = result->left;
        } else {
            //go right
            result = result->right;
        }
    }
    printf("couldn't find the relationSentToOrReceivedFrom riferred to entity %s\n", id);
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
    struct relationSentToOrReceivedFrom * y = &nullRelationSentToOrReceivedFromNode;
    struct relationSentToOrReceivedFrom * x = *head;
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

struct relationSentToOrReceivedFrom * minimumRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    while (x->left != &nullRelationSentToOrReceivedFromNode){
        x = x->left;
    }
    return x;
}

struct relationSentToOrReceivedFrom * successorRelationSentToOrReceivedFrom(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * y;
    if(x->right != &nullRelationSentToOrReceivedFromNode){
        return minimumRelationSentToOrReceivedFrom(x->right,head);
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
        y = successorRelationSentToOrReceivedFrom(z,head);
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
        //copia dati di y in z
        //TODO
    }
    if(y->nodeColor == BLACK){
        deleteRelationSentToOrReceivedFrom_FIXUP(x, head);
    }
}

void deleteRelationSentToOrReceivedFrom_FIXUP(struct relationSentToOrReceivedFrom * x, struct relationSentToOrReceivedFrom ** head){
    struct relationSentToOrReceivedFrom * w;
    struct relationSentToOrReceivedFrom * y;

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
            printf("******************* ADDENT *******************************************\n");
            addent();
            printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "addrel") == 0){
            printf("******************* ADDREL *******************************************\n");
            addrel();
            printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "delent") == 0){
            printf("******************* DELENT *******************************************\n");
            delent();
            printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "delrel") == 0){
            printf("******************* DELREL *******************************************\n");
            delrel();
            printf("**********************************************************************\n\n");
        }
        else if(strcmp(input, "report") == 0){
            printf("******************* REPORT *******************************************\n");
            report();
            printf("**********************************************************************\n\n");
        }
        else { //end
            printf("******************* END **********************************************\n");
            report();
            printf("**********************************************************************\n\n");

            return 0;
        }
    }
}

void addent(){
    scanf("%s", idEnt);
    printf("idEnt: %s - hash: %d\n", idEnt, SDBMHash(idEnt)%ENTITY_HASHTABLE_DIMENSION);

    insertEntityHashTable(idEnt);

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

    struct relation * relation = searchRelation(idRel);
    if(relation == &nullRelationNode){
        relation = malloc(sizeof(struct relation));
        relation->left = &nullRelationNode;
        relation->right = &nullRelationNode;
        relation->father = &nullRelationNode;
        relation->nodeColor = BLACK;
        strcpy(relation->name, idRel);

        insertRelation(relation);

        printf("first time relation %s has been used, added to the relation tree\n", idRel);
    }

    struct relationTracker * relTrackerOrig = searchRelationTracker_1(idRel, entityOrig);
    struct relationTracker * relTrackerDest = searchRelationTracker_1(idRel, entityDest);

    if(relTrackerOrig == &nullRelationTrackerNode) {
        relTrackerOrig = malloc(sizeof(struct relationTracker));
        relTrackerOrig->leftNodeEntityTree = &nullRelationTrackerNode;
        relTrackerOrig->rightNodeEntityTree = &nullRelationTrackerNode;
        relTrackerOrig->fatherEntityTree = &nullRelationTrackerNode;
        relTrackerOrig->sentToTreeHead = NULL;
        relTrackerOrig->receivedFromTreeHead = NULL;
        relTrackerOrig->entity = entityOrig;
        relTrackerOrig->relation = relation;
        printf("relation tracker origin created\n");

        insertRelationTracker_1(entityOrig, relTrackerOrig);
        printf("added relationTracker about %s to entity %s\n", idRel, idOrig);
    }
    if(relTrackerDest == &nullRelationTrackerNode) {
        relTrackerDest = malloc(sizeof(struct relationTracker));
        relTrackerDest->leftNodeEntityTree = &nullRelationTrackerNode;
        relTrackerDest->rightNodeEntityTree = &nullRelationTrackerNode;
        relTrackerDest->fatherEntityTree = &nullRelationTrackerNode;
        relTrackerDest->sentToTreeHead = NULL;
        relTrackerDest->receivedFromTreeHead = NULL;
        relTrackerDest->entity = entityOrig;
        relTrackerDest->relation = relation;
        printf("relation tracker destination created\n");

        insertRelationTracker_1(entityDest, relTrackerDest);
        printf("added relationTracker about %s to entity %s\n", idRel, idDest);
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
    if(relTrackerOrig == &nullRelationTrackerNode){
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


void printAllRelationTrackerRecursive(struct relationTracker * x);
void printAllRelationTrackerRecursive(struct relationTracker * x){
    if(x != &nullRelationTrackerNode){
        printAllRelationTrackerRecursive(x->leftNodeEntityTree);
        printf("\t\t%s\n", x->relation->name);
        printAllRelationTrackerRecursive(x->rightNodeEntityTree);
    }
}
void printAllRelationRecursive(struct relation * x);
void printAllRelationRecursive(struct relation * x){
    if( x != &nullRelationNode){
        printAllRelationRecursive(x->left);
        printf("\t%s\n", x->name);
        printAllRelationRecursive(x->right);
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
                printAllRelationTrackerRecursive(tempEnt->entity->relationTrackerEntityTreeHead);
                tempEnt= tempEnt->nextHashChain;
            }

            printf("\n");
        }
    }

    printf("hash table dimension: %d\n", ENTITY_HASHTABLE_DIMENSION);
    printf("amount of entity stored in the hash table: %d\n", numberOfEntityInHashTable);
    printf("amount of entity printed: %d\n", numberOfEntityPrinted );
    printf("\n");
    printf("All relation names registered: \n");
    printAllRelationRecursive(relationTreeHead);

    return;
    //O(k):  itera in ogni relation e stampa il primo blocco
    //
    //TOTAL: O(k)
    // k= numero totale di tipi di relazioni esistenti
}
