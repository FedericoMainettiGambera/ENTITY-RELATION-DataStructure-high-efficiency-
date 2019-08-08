#include<stdio.h>

typedef enum { false, true } bool;

char idEnt[100];
char idOrig[100];
char idDest[100];
char idRel[100];

/* entity (quadrato) */
// è in una hash table
// stringa che la rappresenta
// riferimento all'albero delle sue relation tracker

unsigned int SDBMHash(char* str, unsigned int length) {
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < length; str++, i++)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

/* relation (stella) */
// è in un albero ordinato alfanumericamente e con possibiltà di essere iterato
// stringa che rappresenta la relazione
// riferimento all'albero 2 di relation tracker

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

/* relation sent to (rombo1) */
// nota: è accessibile dall'entity
// è in un albero ordinato in base all'indirizzo di memeoria della relation tracker receiver e con posibilità di essere iterato
// riferimento alla relation tracker receiver

/* relation received from (rombo2) */
// nota: è accessibile dall'entity destinataria
// è in un albero ordinato in base all'indirizzo di memoria della relation tracker origin e con possibilità di essere iterato
// riferimento alla relation tracker origin


/* prototipies */
void addrel();
void addent();
void delent();
void delrel();
void report();
unsigned int SDBMHash(char* str, unsigned int length);


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

}

void addrel(){
    printf("commend: addrel\n");

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
    printf("command: delent\n");

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
    printf("command: delrel\n");

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
    printf("command: report\n");
    //O(k):  itera in ogni relation e stampa il primo blocco
    //
    //TOTAL: O(k)
    // k= numero totale di tipi di relazioni esistenti
}
