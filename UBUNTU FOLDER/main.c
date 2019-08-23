//
//  main.c
//  Api 2019
//
//  Created by Ludovica Lerma on 06/08/2019.
//  Copyright © 2019 Ludovica Lerma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



//****************** strutture per albero delle richieste di relazione inviate**********************************
struct richiesteInviateTree{
    struct richiestaInviataNode*root;
    struct richiestaInviataNode*null;
};

struct richiestaInviataNode{
    char key[32];
    char color;
    struct richiestaInviataNode*padre;
    struct richiestaInviataNode*figlioDestro;
    struct richiestaInviataNode*figlioSinistro;
    struct entityNode *referredEntity;
};
//****************** strutture per albero delle richieste di relazione inviate**********************************


//****************** strutture per gli alberi di relazioni specifiche *************************************
struct relationNode{
    int key;
    char color;
    struct relationNode*padre;
    struct relationNode*figlioDestro;
    struct relationNode*figlioSinistro;
    struct entityNode*entitaDestinataria;
};

struct relationTree{
    struct relationNode*root;
    struct relationNode*null;
};
//****************** strutture per gli alberi di relazioni specifiche**************************************


//****************** strutture per albero delle entità monitorate**************************************

struct entityNode{
    char nomeEntita[32];
    char color;
    struct entityNode*padre;
    struct entityNode*figlioDestro;
    struct entityNode*figlioSinistro;
    struct listaPtrNodiRelazioniTree *relazioni;
    struct listaPuntatoriRichiesteInviateTree*listaPuntatoriRichiesteInviate;
};

struct entityTree{
    struct entityNode*root;
    struct entityNode*null;
};
//****************** strutture per albero delle entità monitorate**************************************

//****************** strutture per albero dei puntatori delle richieste inviate**************************************
struct listaPuntatoriRichiesteInviateNodo{
    char nomeRelazione[32];
    char color;
    struct listaPuntatoriRichiesteInviateNodo*padre;
    struct listaPuntatoriRichiesteInviateNodo*figlioDestro;
    struct listaPuntatoriRichiesteInviateNodo*figlioSinistro;
    struct richiesteInviateTree *relazioni;
};

struct listaPuntatoriRichiesteInviateTree{
    struct listaPuntatoriRichiesteInviateNodo*root;
    struct listaPuntatoriRichiesteInviateNodo*null;
};
//****************** strutture per albero dei puntatori delle richieste inviate**************************************

//****************** strutture per la lista di puntatori alle radici degli alberi delle relazioni********
struct listaAlberiDiRelazioniNodo{
    char nomeRelazione [32];
    char color;
    struct relationTree*albero;
    struct listaAlberiDiRelazioniNodo*padre;
    struct listaAlberiDiRelazioniNodo*figlioDestro;
    struct listaAlberiDiRelazioniNodo*figlioSinistro;
};

struct listaAlberiDiRelazioniTree{
    struct listaAlberiDiRelazioniNodo*root;
    struct listaAlberiDiRelazioniNodo*null;
};
//****************** strutture per la lista di puntatori alle radici degli alberi delle relazioni********

//****************** strutture per albero di puntatori alle proprie istanze di relazione******************
struct listaPtrNodiRelazioniNodo{
    struct relationNode*relazione;
    char nome [32];
    char color;
    struct listaPtrNodiRelazioniNodo*padre;
    struct listaPtrNodiRelazioniNodo*figlioDestro;
    struct listaPtrNodiRelazioniNodo*figlioSinistro;
};
struct listaPtrNodiRelazioniTree{
    struct listaPtrNodiRelazioniNodo*root;
    struct listaPtrNodiRelazioniNodo*null;
};
//****************** strutture per albero di puntatori alle proprie istanze di relazione******************


typedef struct entita entity;
typedef struct relazione relation;
typedef struct entityTree entityTree;
typedef struct entityNode entityNode;
typedef struct relationNode relationNode;
typedef struct relationTree relationTree;
typedef struct listaAlberiDiRelazioniNodo listaAlberiDiRelazioniNodo;
typedef struct listaAlberiDiRelazioniTree listaAlberiDiRelazioniTree;
typedef struct listaPtrNodiRelazioniTree listaPtrNodiRelazioniTree;
typedef struct listaPtrNodiRelazioniNodo listaPtrNodiRelazioniNodo;
typedef struct richiestaInviataNode richiestaInviataNode;
typedef struct richiesteInviateTree richiesteInviateTree;
typedef struct listaPuntatoriRichiesteInviateNodo listaPuntatoriRichiesteInviateNodo;
typedef struct listaPuntatoriRichiesteInviateTree listaPuntatoriRichiesteInviateTree;


//****************** funzioni per albero delle entita monitorate***************************************
void addEntityNode(entityTree*tree, entityNode*daInserire);
entityNode* eliminateEntityNode(entityTree*tree, entityNode*daEliminare);
void treeInsertFixUpEntity(entityTree*tree, entityNode*nuovo);
void leftRotateEntity(entityTree*tree, entityNode*x);
void rightRotateEntity(entityTree*tree, entityNode*y);
entityNode* treeSuccessorEntity(entityNode*x, entityTree*tree);
void treeDeleteFixUpEntity(entityTree*tree, entityNode*x);
entityNode* treeMinimumEntity(entityNode*x, entityTree*tree);
entityNode* treeMaximumEntity(entityNode*x, entityTree*tree);
void inorderTreeWalkEntity(entityNode*x, entityTree*tree);
entityNode* treeSearchEntity(entityNode*nodo, char key[32], entityTree*tree);
//****************** funzioni per gli albero delle entita monitorate************************************


//****************** funzioni per gli alberi delle relazioni specifiche*********************************
void addRelationNode(relationTree*tree, relationNode*daInserire);
relationNode* eliminateRelationNode(relationTree*tree, relationNode*daEliminare);
void treeInsertFixUpRelation(relationTree*tree, relationNode*nuovo);
void leftRotateRelation(relationTree*tree, relationNode*x);
void rightRotateRelation(relationTree*tree, relationNode*y);
relationNode* treeSuccessorRelation(relationNode*x, relationTree*tree);
void treeDeleteFixUpRelation(relationTree*tree, relationNode*x);
relationNode* treeMinimumRelation(relationNode*x, relationTree*tree);
relationNode* treeMaximumRelation(relationNode*x, relationTree*tree);
void inorderTreeWalkRelation(relationNode*x, relationTree*tree);
relationNode* treeSearchRelation(relationNode*nodo, int key, entityNode*,relationTree*tree);
relationNode* treeSearchRelationReport(relationNode*nodo, int key ,relationTree*tree);
//****************** funzioni per gli alberi delle relazioni specifiche**********************************


//******************funzioni per la lista di puntatori alle radici degli alberi delle relazioni********
void addlistaAlberiDiRelazioniNodo(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*daInserire);
listaAlberiDiRelazioniNodo* eliminateListaAlberiDiRelazioniNodo(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*daEliminare);
void treeInsertFixUpListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*nuovo);
void leftRotateListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*x);
void rightRotateListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*y);
listaAlberiDiRelazioniNodo* treeSuccessorListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree);
void treeDeleteFixUpListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*x);
listaAlberiDiRelazioniNodo* treeMinimumListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree);
listaAlberiDiRelazioniNodo* treeMaximumListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree);
void inorderTreeWalkListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree);
listaAlberiDiRelazioniNodo* treeSearchListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*nodo, char nomeRelazione[32], listaAlberiDiRelazioniTree*tree);
//****************** funzioni per la lista di puntatori alle radici degli alberi delle relazioni********


//******************funzioni per la lista di puntatori alle istanze delle relazioni**********************
void addListaPtrNodiRelazioniNodo(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*daInserire);
listaPtrNodiRelazioniNodo* eliminateListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*daEliminare);
void treeInsertFixUpListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*nuovo);
void leftRotateListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*x);
void rightRotateListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*y);
listaPtrNodiRelazioniNodo* treeSuccessorListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree);
void treeDeleteFixUpListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*x);
listaPtrNodiRelazioniNodo* treeMinimumListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree);
listaPtrNodiRelazioniNodo* treeMaximumListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree);
void inorderTreeWalkListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree);
listaPtrNodiRelazioniNodo* treeSearchListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*nodo, char nomeRelazione[32], listaPtrNodiRelazioniTree*tree);
//******************funzioni per la lista di puntatori alle istanze delle relazioni**********************

//****************** funzioni per albero delle richieste inviate***************************************
void addRichiestaInviataNode(richiesteInviateTree*tree, richiestaInviataNode*daInserire);
richiestaInviataNode* eliminateRichiestaInviataNode(richiesteInviateTree*tree, richiestaInviataNode*daEliminare);
void treeInsertFixUpRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*nuovo);
void leftRotateRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*x);
void rightRotateRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*y);
richiestaInviataNode* treeSuccessorRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree);
void treeDeleteFixUpRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*x);
richiestaInviataNode* treeMinimumRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree);
richiestaInviataNode* treeMaximumRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree);
void inorderTreeWalkRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree);
richiestaInviataNode* treeSearchRichiestaInviata(richiestaInviataNode*nodo, char key[32], richiesteInviateTree*tree);
//****************** funzioni per gli albero delle richieste inviate************************************

//****************** funzioni per albero dei puntatori delle richieste inviate***************************************
void addPuntatoreRichiestainviataNode(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*daInserire);
listaPuntatoriRichiesteInviateNodo* eliminatePuntatoreRichiestaInviataNode(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*daEliminare);
void treeInsertFixUpListaRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*nuovo);
void leftRotateListaRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*x);
void rightListaRotateRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*y);
listaPuntatoriRichiesteInviateNodo* treeSuccessorListaRichiesteInviata(listaPuntatoriRichiesteInviateNodo*nodo, listaPuntatoriRichiesteInviateTree*tree);
void treeDeleteFixUpListaRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*x);
listaPuntatoriRichiesteInviateNodo* treeMinimumListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree);
listaPuntatoriRichiesteInviateNodo* treeMaximumListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree);
void inorderTreeWalkListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree);
listaPuntatoriRichiesteInviateNodo* treeSearchListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*nodo, char key[32], listaPuntatoriRichiesteInviateTree*tree);
//****************** funzioni per gli albero deli puntatori delle richieste inviate************************************


//****************** funzioni programma************************************
void aggiungiRelazione(char nomeMittente[32], char nomeDestinatario[32], char nomeRelazione[32], entityTree*, listaAlberiDiRelazioniTree*);
void eliminaRelazione(char nomeMittente[32], char nomeDestinatario[32], char nomeRelazione[32],entityTree*, listaAlberiDiRelazioniTree*);
//void report(relationList report);
void end(void);
void aggiungiEntita(char*, entityTree*);
void eliminaEntita(char*, entityTree*, listaAlberiDiRelazioniTree*);
void decrementaRichieste(richiestaInviataNode*x, richiesteInviateTree*tree, relationTree*albero, char nomeRelazione[32], entityTree*entityTree);
void riaggiungiAllAlbero(relationNode*x, relationTree*tree, relationTree*albero);
void inorderStampaReport(relationNode*x, relationTree*albero, int y);
//****************** funzioni programma************************************


//*****************************ALBERO RB ENTITA_************************************************************
entityNode* treeMinimumEntity(entityNode*x, entityTree*tree){
    while(x->figlioSinistro!=tree->null){
        x=x->figlioSinistro;
    }
    return x;
}

entityNode* treeMaximumEntity(entityNode*x, entityTree*tree){
    while(x->figlioDestro!=tree->null){
        x=x->figlioDestro;
    }
    return x;
}

entityNode* treeSuccessorEntity(entityNode*x, entityTree*tree){
    entityNode*y;
    if(x->figlioDestro!=tree->null){
        return treeMinimumEntity(x->figlioDestro, tree);
    }
    y=x->padre;
    while(y!=tree->null&&x==y->figlioDestro){
        x=y;
        y=y->padre;
    }
    return y;
}

void addEntityNode(entityTree*tree, entityNode*daInserire){
    entityNode*y=tree->null;
    entityNode*x=tree->root;
    
    while(x!=tree->null){
        y=x;
        if(strcmp(daInserire->nomeEntita,x->nomeEntita)<0){
            x=x->figlioSinistro;
        }
        else x=x->figlioDestro;
    }
    daInserire->padre=y;
    if(y==tree->null){
        tree->root=daInserire;
    }
    else if(strcmp(daInserire->nomeEntita,y->nomeEntita)<0){
        y->figlioSinistro=daInserire;
    }
    else y->figlioDestro=daInserire;
    
    daInserire->figlioDestro=tree->null;
    daInserire->figlioSinistro=tree->null;
    
    daInserire->color='R';
    
    treeInsertFixUpEntity(tree, daInserire);
}

void treeInsertFixUpEntity(entityTree*tree, entityNode*nuovo){
    entityNode*x;
    entityNode*y;
    
    if(nuovo==tree->root){
        tree->root->color='B';
    }
    else{
        x=nuovo->padre;
        if(x->color=='R'){
            if(x==x->padre->figlioSinistro){
            y=x->padre->figlioDestro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpEntity(tree,x->padre);
                   }
            
                else{
                    if(nuovo==x->figlioDestro){
                    nuovo=x;
                    leftRotateEntity(tree, nuovo);
                    x=nuovo->padre;
                    }
                x->color='B';
                x->padre->color='R';
                rightRotateEntity(tree, x->padre);
                }
            }
            else{
                y=x->padre->figlioSinistro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpEntity(tree,x->padre);
                }
                else{
                    if(nuovo==x->figlioSinistro){
                        nuovo=x;
                        rightRotateEntity(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    leftRotateEntity(tree, x->padre);
                }

            }
        }
    }
}

void leftRotateEntity(entityTree*tree, entityNode*x){
    entityNode*y;
    y=x->figlioDestro;
    x->figlioDestro=y->figlioSinistro;
    
    if(y->figlioSinistro!=tree->null){
        y->figlioSinistro->padre=x;
    }
    y->padre=x->padre;
    if(x->padre==tree->null){
        tree->root=y;
    }
    else if(x==x->padre->figlioSinistro){
        x->padre->figlioSinistro=y;
    }
    else x->padre->figlioDestro=y;
    y->figlioSinistro=x;
    x->padre=y;
}

void rightRotateEntity(entityTree*tree, entityNode*y){
    entityNode*x;
    x=y->figlioSinistro;
    y->figlioSinistro=x->figlioDestro;
    
    if(x->figlioDestro!=tree->null){
        x->figlioDestro->padre=y;
    }
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    x->figlioDestro=y;
    y->padre=x;
}

entityNode* eliminateEntityNode(entityTree*tree, entityNode*daEliminare){
    entityNode*y;
    entityNode*x;
    if(daEliminare->figlioSinistro==tree->null||daEliminare->figlioDestro==tree->null){
        y=daEliminare;
    }
    else y=treeSuccessorEntity(daEliminare, tree);
    if(y->figlioSinistro!=tree->null){
        x=y->figlioSinistro;
    }
    else x=y->figlioDestro;
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    if(y!=daEliminare){

        int oldcolor=y->color;
        y->color=daEliminare->color;

        if(daEliminare==tree->root){
            tree->root=y;
        }
        else {
            if (daEliminare->padre->figlioSinistro == daEliminare) {
                daEliminare->padre->figlioSinistro = y;
            }
            else daEliminare->padre->figlioDestro = y;
        }
        daEliminare->figlioSinistro->padre = y;
        daEliminare->figlioDestro->padre = y;
        y->padre = daEliminare->padre;
        y->figlioSinistro = daEliminare->figlioSinistro;
        y->figlioDestro = daEliminare->figlioDestro;
        if(oldcolor=='B'){
            treeDeleteFixUpEntity(tree, x);
        }

        return daEliminare;
    }
    if(y->color=='B'){
        treeDeleteFixUpEntity(tree, x);
    }
    return y;
}

void treeDeleteFixUpEntity(entityTree*tree, entityNode*x){
    entityNode*w;
    if(x->color=='R' || x->padre==tree->null){
        x->color='B';
    }
    else if(x==x->padre->figlioSinistro){
        w=x->padre->figlioDestro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            leftRotateEntity(tree, x->padre);
            w=x->padre->figlioDestro;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpEntity(tree, x->padre);
        }
        else{
            if(w->figlioDestro->color=='B'){
                w->figlioSinistro->color='B';
                w->color='R';
                rightRotateEntity(tree, w);
                w=x->padre->figlioDestro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioDestro->color='B';
            leftRotateEntity(tree, x->padre);
        }
    }
    else{
        w=x->padre->figlioSinistro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            rightRotateEntity(tree, x->padre);
            w=x->padre->figlioSinistro;;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpEntity(tree, x->padre);
        }
        else{
            if(w->figlioSinistro->color=='B'){
            w->figlioDestro->color='B';
            w->color='R';
            leftRotateEntity(tree, w);
            w=x->padre->figlioSinistro;
        }
        w->color=x->padre->color;
        x->padre->color='B';
        w->figlioSinistro->color='B';
        rightRotateEntity(tree, x->padre);
     }
  }
    
}
    
void inorderTreeWalkEntity(entityNode*x, entityTree*tree){
    if(x!=tree->null){
        inorderTreeWalkEntity(x->figlioSinistro, tree);
        printf("\"%s\" ", x->nomeEntita);
        inorderTreeWalkEntity(x->figlioDestro,tree);
    }
}

entityNode* treeSearchEntity(entityNode*nodo, char key[32], entityTree*tree){
    if(nodo==tree->null||strcmp(key,nodo->nomeEntita)==0){
        return nodo;
    }
    if(strcmp(key,nodo->nomeEntita)<0){
        return treeSearchEntity(nodo->figlioSinistro, key, tree);
    }
    else return treeSearchEntity(nodo->figlioDestro, key, tree);
}

//***************************** FINE ALBERO RB ENTITA_***************************************************


//*****************************ALBERO RB RELAZIONI_******************************************************
relationNode* treeMinimumRelation(relationNode*x, relationTree*tree){
    while(x->figlioSinistro!=tree->null){
        x=x->figlioSinistro;
    }
    return x;
}

relationNode* treeMaximumRelation(relationNode*x, relationTree*tree){
    while(x->figlioDestro!=tree->null){
        x=x->figlioDestro;
    }
    return x;
}

relationNode* treeSuccessorRelation(relationNode*x, relationTree*tree){
    relationNode*y;
    if(x->figlioDestro!=tree->null){
        return treeMinimumRelation(x->figlioDestro, tree);
    }
    y=x->padre;
    while(y!=tree->null&&x==y->figlioDestro){
        x=y;
        y=y->padre;
    }
    return y;
}

void addRelationNode(relationTree*tree, relationNode*daInserire){
    relationNode*y=tree->null;
    relationNode*x=tree->root;
    
    while(x!=tree->null){
        y=x;
        if(daInserire->key<x->key){
            x=x->figlioSinistro;
        }
        else if(daInserire->key==x->key){
            if (strcmp(daInserire->entitaDestinataria->nomeEntita, x->entitaDestinataria->nomeEntita) < 0)
                x = x->figlioSinistro;
            else {
                x = x->figlioDestro;
            }
        }
        else x=x->figlioDestro;
    }
    daInserire->padre=y;
    if(y==tree->null){
        tree->root=daInserire;
    }
    else if(daInserire->key<y->key){
        y->figlioSinistro=daInserire;
    }
    else if(daInserire->key==y->key){
        if(strcmp(daInserire->entitaDestinataria->nomeEntita,y->entitaDestinataria->nomeEntita)<0){
            y->figlioSinistro=daInserire;
        }
        else y->figlioDestro=daInserire;
    }
    else y->figlioDestro=daInserire;
    
    daInserire->figlioDestro=tree->null;
    daInserire->figlioSinistro=tree->null;
    
    daInserire->color='R';
    
    treeInsertFixUpRelation(tree, daInserire);
}

void treeInsertFixUpRelation(relationTree*tree, relationNode*nuovo){
    relationNode*x;
    relationNode*y;
    
    if(nuovo==tree->root){
        tree->root->color='B';
    }
    else{
        x=nuovo->padre;
        if(x->color=='R'){
            if(x==x->padre->figlioSinistro){
                y=x->padre->figlioDestro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpRelation(tree,x->padre);
                }
                
                else{
                    if(nuovo==x->figlioDestro){
                        nuovo=x;
                        leftRotateRelation(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    rightRotateRelation(tree, x->padre);
                }
            }
            else{   y=x->padre->figlioSinistro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpRelation(tree,x->padre);
                }
                else{
                    if(nuovo==x->figlioSinistro){
                        nuovo=x;
                        rightRotateRelation(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    leftRotateRelation(tree, x->padre);
                }
                
            }
        }
    }
}

void leftRotateRelation(relationTree*tree, relationNode*x){
    relationNode*y;
    y=x->figlioDestro;
    x->figlioDestro=y->figlioSinistro;
    
    if(y->figlioSinistro!=tree->null){
        y->figlioSinistro->padre=x;
    }
    y->padre=x->padre;
    if(x->padre==tree->null){
        tree->root=y;
    }
    else if(x==x->padre->figlioSinistro){
        x->padre->figlioSinistro=y;
    }
    else x->padre->figlioDestro=y;
    y->figlioSinistro=x;
    x->padre=y;
}

void rightRotateRelation(relationTree*tree, relationNode*y){
    relationNode*x;
    x=y->figlioSinistro;
    y->figlioSinistro=x->figlioDestro;
    
    if(x->figlioDestro!=tree->null){
        x->figlioDestro->padre=y;
    }
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    x->figlioDestro=y;
    y->padre=x;
}

relationNode* eliminateRelationNode(relationTree*tree, relationNode*daEliminare){
    relationNode*y;
    relationNode*x;
    if(daEliminare->figlioSinistro==tree->null||daEliminare->figlioDestro==tree->null){
        y=daEliminare;
    }
    else y=treeSuccessorRelation(daEliminare, tree);
    if(y->figlioSinistro!=tree->null){
        x=y->figlioSinistro;
    }
    else x=y->figlioDestro;
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    if(y!=daEliminare){
        printf("entering here?\n");
        // inorderTreeWalkRelation(tree->root, tree);
        int oldcolor=y->color;
        y->color=daEliminare->color;

        if(daEliminare==tree->root){
            tree->root=y;
        }
        else {
            if (daEliminare->padre->figlioSinistro == daEliminare) {
                daEliminare->padre->figlioSinistro = y;
            }
            else daEliminare->padre->figlioDestro = y;
        }
        daEliminare->figlioSinistro->padre = y;
        daEliminare->figlioDestro->padre = y;
        y->padre = daEliminare->padre;
        y->figlioSinistro = daEliminare->figlioSinistro;
        y->figlioDestro = daEliminare->figlioDestro;
        if(oldcolor=='B'){
            treeDeleteFixUpRelation(tree, x);
        }

       // inorderTreeWalkRelation(tree->root, tree);
        return daEliminare;
    }
    if(y->color=='B'){
        treeDeleteFixUpRelation(tree, x);
    }
    return y;
}

void treeDeleteFixUpRelation(relationTree*tree, relationNode*x){
    relationNode*w;
    if(x->color=='R' || x->padre==tree->null){
        x->color='B';
    }
    else if(x==x->padre->figlioSinistro){
        w=x->padre->figlioDestro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            leftRotateRelation(tree, x->padre);
            w=x->padre->figlioDestro;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpRelation(tree, x->padre);
        }
        else{
            if(w->figlioDestro->color=='B'){
                w->figlioSinistro->color='B';
                w->color='R';
                rightRotateRelation(tree, w);
                w=x->padre->figlioDestro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioDestro->color='B';
            leftRotateRelation(tree, x->padre);
        }
    }
    else{
        w=x->padre->figlioSinistro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            rightRotateRelation(tree, x->padre);
            w=x->padre->figlioSinistro;;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpRelation(tree, x->padre);
        }
        else{
            if(w->figlioSinistro->color=='B'){
                w->figlioDestro->color='B';
                w->color='R';
                leftRotateRelation(tree, w);
                w=x->padre->figlioSinistro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioSinistro->color='B';
            rightRotateRelation(tree, x->padre);
        }
    }
    
}

void inorderTreeWalkRelation(relationNode*x, relationTree*tree){
    if(x!=tree->null){
        inorderTreeWalkRelation(x->figlioSinistro, tree);
        printf(" %d \n %s \n", x->key, x->entitaDestinataria->nomeEntita);
        inorderTreeWalkRelation(x->figlioDestro,tree);
    }
}

relationNode* treeSearchRelation(relationNode*nodo, int key, entityNode*entita, relationTree*tree){
   
    
    if(nodo==tree->null||(key==nodo->key&&strcmp(entita->nomeEntita,nodo->entitaDestinataria->nomeEntita)==0)) {
        return nodo;
    }
    if(key<nodo->key){
        return treeSearchRelation(nodo->figlioSinistro, key, entita, tree);
    }
    else if(key==nodo->key){
       
        if(strcmp(entita->nomeEntita,nodo->entitaDestinataria->nomeEntita)<0){
            return treeSearchRelation(nodo->figlioSinistro, key, entita, tree);
        }
        else return treeSearchRelation(nodo->figlioDestro, key, entita, tree);
    }
    else return treeSearchRelation(nodo->figlioDestro, key, entita, tree);
}

relationNode* treeSearchRelationReport(relationNode*nodo, int key, relationTree*tree){
    if(nodo==tree->null||(key==nodo->key)){
        return nodo;
    }
    if(key<nodo->key){
        return treeSearchRelationReport(nodo->figlioSinistro, key, tree);
    }
    else return treeSearchRelationReport(nodo->figlioDestro, key, tree);
}
//*****************************FINE ALBERO RB RELAZIONI_***********************************************


//*****************************ALBERO RB ALBERO DELLE RADICI DEGLI ALBERI DI RELAZIONI***********************
listaAlberiDiRelazioniNodo* treeMinimumListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree){
    while(x->figlioSinistro!=tree->null){
        x=x->figlioSinistro;
    }
    return x;
}

listaAlberiDiRelazioniNodo* treeMaximumListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree){
    while(x->figlioDestro!=tree->null){
        x=x->figlioDestro;
    }
    return x;
}

listaAlberiDiRelazioniNodo* treeSuccessorListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree){
    listaAlberiDiRelazioniNodo*y;
    if(x->figlioDestro!=tree->null){
        return treeMinimumListaAlberiDiRelazioni(x->figlioDestro, tree);
    }
    y=x->padre;
    while(y!=tree->null&&x==y->figlioDestro){
        x=y;
        y=y->padre;
    }
    return y;
}

void addListaAlberiDiRelazioniNodo(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*daInserire){
    listaAlberiDiRelazioniNodo*y=tree->null;
    listaAlberiDiRelazioniNodo*x=tree->root;
    
    while(x!=tree->null){
        y=x;
        if(strcmp(daInserire->nomeRelazione,x->nomeRelazione)<0){
            x=x->figlioSinistro;
        }
        else x=x->figlioDestro;
    }
    daInserire->padre=y;
    if(y==tree->null){
        tree->root=daInserire;
    }
    else if(strcmp(daInserire->nomeRelazione,y->nomeRelazione)<0){
        y->figlioSinistro=daInserire;
    }
    else y->figlioDestro=daInserire;
    
    daInserire->figlioDestro=tree->null;
    daInserire->figlioSinistro=tree->null;
    
    daInserire->color='R';
    
    treeInsertFixUpListaAlberiDiRelazioni(tree, daInserire);
}

void treeInsertFixUpListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*nuovo){
    listaAlberiDiRelazioniNodo*x;
    listaAlberiDiRelazioniNodo*y;
    
    if(nuovo==tree->root){
        tree->root->color='B';
    }
    else{
        x=nuovo->padre;
        if(x->color=='R'){
            if(x==x->padre->figlioSinistro){
                y=x->padre->figlioDestro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                     treeInsertFixUpListaAlberiDiRelazioni(tree,x->padre);
                }
                
                else{
                    if(nuovo==x->figlioDestro){
                        nuovo=x;
                        leftRotateListaAlberiDiRelazioni(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    rightRotateListaAlberiDiRelazioni(tree, x->padre);
                }
            }
            else{   y=x->padre->figlioSinistro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpListaAlberiDiRelazioni(tree,x->padre);
                }
                else{
                    if(nuovo==x->figlioSinistro){
                        nuovo=x;
                        rightRotateListaAlberiDiRelazioni(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    leftRotateListaAlberiDiRelazioni(tree, x->padre);
                }
                
            }
        }
    }
}

void leftRotateListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*x){
    listaAlberiDiRelazioniNodo*y;
    y=x->figlioDestro;
    x->figlioDestro=y->figlioSinistro;
    
    if(y->figlioSinistro!=tree->null){
        y->figlioSinistro->padre=x;
    }
    y->padre=x->padre;
    if(x->padre==tree->null){
        tree->root=y;
    }
    else if(x==x->padre->figlioSinistro){
        x->padre->figlioSinistro=y;
    }
    else x->padre->figlioDestro=y;
    y->figlioSinistro=x;
    x->padre=y;
}

void rightRotateListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*y){
    listaAlberiDiRelazioniNodo*x;
    x=y->figlioSinistro;
    y->figlioSinistro=x->figlioDestro;
    
    if(x->figlioDestro!=tree->null){
        x->figlioDestro->padre=y;
    }
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    x->figlioDestro=y;
    y->padre=x;
}

listaAlberiDiRelazioniNodo* eliminateListaAlberiDiRelazioniNodo(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*daEliminare){
    listaAlberiDiRelazioniNodo*y;
    listaAlberiDiRelazioniNodo*x;
    if(daEliminare->figlioSinistro==tree->null||daEliminare->figlioDestro==tree->null){
        y=daEliminare;
    }
    else y=treeSuccessorListaAlberiDiRelazioni(daEliminare, tree);
    if(y->figlioSinistro!=tree->null){
        x=y->figlioSinistro;
    }
    else x=y->figlioDestro;
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    if(y!=daEliminare){
            strcpy(daEliminare->nomeRelazione,y->nomeRelazione);
            daEliminare->albero=y->albero;
        
    }
    if(y->color=='B'){
        treeDeleteFixUpListaAlberiDiRelazioni(tree, x);
    }
    return y;
}

void treeDeleteFixUpListaAlberiDiRelazioni(listaAlberiDiRelazioniTree*tree, listaAlberiDiRelazioniNodo*x){
    listaAlberiDiRelazioniNodo*w;
    if(x->color=='R' || x->padre==tree->null){
        x->color='B';
    }
    else if(x==x->padre->figlioSinistro){
        w=x->padre->figlioDestro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            leftRotateListaAlberiDiRelazioni(tree, x->padre);
            w=x->padre->figlioDestro;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpListaAlberiDiRelazioni(tree, x->padre);
        }
        else{
            if(w->figlioDestro->color=='B'){
                w->figlioSinistro->color='B';
                w->color='R';
                rightRotateListaAlberiDiRelazioni(tree, w);
                w=x->padre->figlioDestro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioDestro->color='B';
            leftRotateListaAlberiDiRelazioni(tree, x->padre);
        }
    }
    else{
        w=x->padre->figlioSinistro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            rightRotateListaAlberiDiRelazioni(tree, x->padre);
            w=x->padre->figlioSinistro;;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpListaAlberiDiRelazioni(tree, x->padre);
        }
        else{
            if(w->figlioSinistro->color=='B'){
                w->figlioDestro->color='B';
                w->color='R';
                leftRotateListaAlberiDiRelazioni(tree, w);
                w=x->padre->figlioSinistro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioSinistro->color='B';
            rightRotateListaAlberiDiRelazioni(tree, x->padre);
        }
    }
    
}

void inorderTreeWalkListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*x, listaAlberiDiRelazioniTree*tree){
    if(x!=tree->null){
        inorderTreeWalkListaAlberiDiRelazioni(x->figlioSinistro, tree);
        printf("key is: %s \n color is: %c \n", x->nomeRelazione, x->color);
        inorderTreeWalkListaAlberiDiRelazioni(x->figlioDestro,tree);
    }
}

listaAlberiDiRelazioniNodo* treeSearchListaAlberiDiRelazioni(listaAlberiDiRelazioniNodo*nodo, char nomeRelazione[32], listaAlberiDiRelazioniTree*tree){
    if(nodo==tree->null||strcmp(nomeRelazione,nodo->nomeRelazione)==0){
        return nodo;
    }
    if(strcmp(nomeRelazione,nodo->nomeRelazione)<0){
        return treeSearchListaAlberiDiRelazioni(nodo->figlioSinistro, nomeRelazione, tree);
    }
    else return treeSearchListaAlberiDiRelazioni(nodo->figlioDestro, nomeRelazione, tree);
}
//*****************************FINE ALBERO RB ALBERO DELLE RADICI DEGLI ALBERI DI RELAZIONI****************


//*****************************ALBERO RB LISTA ISTANZE DI RELAZIONI DI UNA ENTITA*********************
listaPtrNodiRelazioniNodo* treeMinimumListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree){
    while(x->figlioSinistro!=tree->null){
        x=x->figlioSinistro;
    }
    return x;
}

listaPtrNodiRelazioniNodo* treeMaximumListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree){
    while(x->figlioDestro!=tree->null){
        x=x->figlioDestro;
    }
    return x;
}

listaPtrNodiRelazioniNodo* treeSuccessorListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree){
    listaPtrNodiRelazioniNodo*y;
    if(x->figlioDestro!=tree->null){
        return treeMinimumListaPtrNodiRelazioni(x->figlioDestro, tree);
    }
    y=x->padre;
    while(y!=tree->null&&x==y->figlioDestro){
        x=y;
        y=y->padre;
    }
    return y;
}

void addListaPtrNodiRelazioniNodo(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*daInserire){
    listaPtrNodiRelazioniNodo*y=tree->null;
    listaPtrNodiRelazioniNodo*x=tree->root;
    
    while(x!=tree->null){
        y=x;
        if(strcmp(daInserire->nome,x->nome)<0){
            x=x->figlioSinistro;
        }
        else x=x->figlioDestro;
    }
    daInserire->padre=y;
    if(y==tree->null){
        tree->root=daInserire;
    }
    else if(strcmp(daInserire->nome,y->nome)<0){
        y->figlioSinistro=daInserire;
    }
    else y->figlioDestro=daInserire;
    
    daInserire->figlioDestro=tree->null;
    daInserire->figlioSinistro=tree->null;
    
    daInserire->color='R';
    
    treeInsertFixUpListaPtrNodiRelazioni(tree, daInserire);
}

void treeInsertFixUpListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*nuovo){
    listaPtrNodiRelazioniNodo*x;
    listaPtrNodiRelazioniNodo*y;
    
    if(nuovo==tree->root){
        tree->root->color='B';
    }
    else{
        x=nuovo->padre;
        if(x->color=='R'){
            if(x==x->padre->figlioSinistro){
                y=x->padre->figlioDestro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpListaPtrNodiRelazioni(tree,x->padre);
                }
                
                else{
                    if(nuovo==x->figlioDestro){
                        nuovo=x;
                        leftRotateListaPtrNodiRelazioni(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    rightRotateListaPtrNodiRelazioni(tree, x->padre);
                }
            }
            else{   y=x->padre->figlioSinistro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpListaPtrNodiRelazioni(tree,x->padre);
                }
                else{
                    if(nuovo==x->figlioSinistro){
                        nuovo=x;
                        rightRotateListaPtrNodiRelazioni(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    leftRotateListaPtrNodiRelazioni(tree, x->padre);
                }
                
            }
        }
    }
}

void leftRotateListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*x){
    listaPtrNodiRelazioniNodo*y;
    y=x->figlioDestro;
    x->figlioDestro=y->figlioSinistro;
    
    if(y->figlioSinistro!=tree->null){
        y->figlioSinistro->padre=x;
    }
    y->padre=x->padre;
    if(x->padre==tree->null){
        tree->root=y;
    }
    else if(x==x->padre->figlioSinistro){
        x->padre->figlioSinistro=y;
    }
    else x->padre->figlioDestro=y;
    y->figlioSinistro=x;
    x->padre=y;
}

void rightRotateListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*y){
    listaPtrNodiRelazioniNodo*x;
    x=y->figlioSinistro;
    y->figlioSinistro=x->figlioDestro;
    
    if(x->figlioDestro!=tree->null){
        x->figlioDestro->padre=y;
    }
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    x->figlioDestro=y;
    y->padre=x;
}

listaPtrNodiRelazioniNodo* eliminateListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*daEliminare){
    listaPtrNodiRelazioniNodo*y;
    listaPtrNodiRelazioniNodo*x;
    if(daEliminare->figlioSinistro==tree->null||daEliminare->figlioDestro==tree->null){
        y=daEliminare;
    }
    else y=treeSuccessorListaPtrNodiRelazioni(daEliminare, tree);
    if(y->figlioSinistro!=tree->null){
        x=y->figlioSinistro;
    }
    else x=y->figlioDestro;
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    if(y!=daEliminare){
        int oldcolor=y->color;
        y->color=daEliminare->color;

        if(daEliminare==tree->root){
            tree->root=y;
        }
        else {
            if (daEliminare->padre->figlioSinistro == daEliminare) {
                daEliminare->padre->figlioSinistro = y;
            }
            else daEliminare->padre->figlioDestro = y;
        }
        daEliminare->figlioSinistro->padre = y;
        daEliminare->figlioDestro->padre = y;
        y->padre = daEliminare->padre;
        y->figlioSinistro = daEliminare->figlioSinistro;
        y->figlioDestro = daEliminare->figlioDestro;
        if(oldcolor=='B'){
            treeDeleteFixUpListaPtrNodiRelazioni(tree, x);
        }

        return daEliminare;
    }
    if(y->color=='B'){
        treeDeleteFixUpListaPtrNodiRelazioni(tree, x);
    }
    return y;
}

void treeDeleteFixUpListaPtrNodiRelazioni(listaPtrNodiRelazioniTree*tree, listaPtrNodiRelazioniNodo*x){
    listaPtrNodiRelazioniNodo*w;
    if(x->color=='R' || x->padre==tree->null){
        x->color='B';
    }
    else if(x==x->padre->figlioSinistro){
        w=x->padre->figlioDestro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            leftRotateListaPtrNodiRelazioni(tree, x->padre);
            w=x->padre->figlioDestro;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpListaPtrNodiRelazioni(tree, x->padre);
        }
        else{
            if(w->figlioDestro->color=='B'){
                w->figlioSinistro->color='B';
                w->color='R';
                rightRotateListaPtrNodiRelazioni(tree, w);
                w=x->padre->figlioDestro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioDestro->color='B';
            leftRotateListaPtrNodiRelazioni(tree, x->padre);
        }
    }
    else{
        w=x->padre->figlioSinistro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            rightRotateListaPtrNodiRelazioni(tree, x->padre);
            w=x->padre->figlioSinistro;;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpListaPtrNodiRelazioni(tree, x->padre);
        }
        else{
            if(w->figlioSinistro->color=='B'){
                w->figlioDestro->color='B';
                w->color='R';
                leftRotateListaPtrNodiRelazioni(tree, w);
                w=x->padre->figlioSinistro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioSinistro->color='B';
            rightRotateListaPtrNodiRelazioni(tree, x->padre);
        }
    }
    
}




void inorderTreeWalkListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree){
    if(x!=tree->null){
        inorderTreeWalkListaPtrNodiRelazioni(x->figlioSinistro, tree);
        printf("key is: %s \n color is: %c \n", x->nome, x->color);
        inorderTreeWalkListaPtrNodiRelazioni(x->figlioDestro,tree);
    }
}

listaPtrNodiRelazioniNodo* treeSearchListaPtrNodiRelazioni(listaPtrNodiRelazioniNodo*nodo, char nomeRelazione[32], listaPtrNodiRelazioniTree*tree){
    if(nodo==tree->null||strcmp(nomeRelazione,nodo->nome)==0){
        return nodo;
    }
    if(strcmp(nomeRelazione,nodo->nome)<0){
        return treeSearchListaPtrNodiRelazioni(nodo->figlioSinistro, nomeRelazione, tree);
    }
    else return treeSearchListaPtrNodiRelazioni(nodo->figlioDestro, nomeRelazione, tree);
}
//*****************************FINE ALBERO RB LISTA ISTANZE DI RELAZIONI DI UNA ENTITA*********************


//*****************************ALBERO RB RICHIESTE INVIATE*********************
richiestaInviataNode* treeMinimumRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree){
    while(x->figlioSinistro!=tree->null){
        x=x->figlioSinistro;
    }
    return x;
}

richiestaInviataNode* treeMaximumRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree){
    while(x->figlioDestro!=tree->null){
        x=x->figlioDestro;
    }
    return x;
}

richiestaInviataNode* treeSuccessorRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree){
    richiestaInviataNode*y;
    if(x->figlioDestro!=tree->null){
        return treeMinimumRichiestaInviata(x->figlioDestro, tree);
    }
    y=x->padre;
    while(y!=tree->null&&x==y->figlioDestro){
        x=y;
        y=y->padre;
    }
    return y;
}

void addRichiestaInviataNode(richiesteInviateTree*tree, richiestaInviataNode*daInserire){
    richiestaInviataNode*y=tree->null;
    richiestaInviataNode*x=tree->root;
    
    while(x!=tree->null){
        y=x;
        if(strcmp(daInserire->key,x->key)<0){
            x=x->figlioSinistro;
        }
        else x=x->figlioDestro;
    }
    daInserire->padre=y;
    if(y==tree->null){
        tree->root=daInserire;
    }
    else if(strcmp(daInserire->key,y->key)<0){
        y->figlioSinistro=daInserire;
    }
    else y->figlioDestro=daInserire;
    
    daInserire->figlioDestro=tree->null;
    daInserire->figlioSinistro=tree->null;
    
    daInserire->color='R';
    
     treeInsertFixUpRichiestaInviata(tree, daInserire);
}



void treeInsertFixUpRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*nuovo){
    richiestaInviataNode*x;
    richiestaInviataNode*y;
    
    if(nuovo==tree->root){
        tree->root->color='B';
    }
    else{
        x=nuovo->padre;
        if(x->color=='R'){
            if(x==x->padre->figlioSinistro){
                y=x->padre->figlioDestro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpRichiestaInviata(tree,x->padre);
                }
                
                else{
                    if(nuovo==x->figlioDestro){
                        nuovo=x;
                        leftRotateRichiestaInviata(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    rightRotateRichiestaInviata(tree, x->padre);
                }
            }
            else{   y=x->padre->figlioSinistro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpRichiestaInviata(tree,x->padre);
                }
                else{
                    if(nuovo==x->figlioSinistro){
                        nuovo=x;
                        rightRotateRichiestaInviata(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    leftRotateRichiestaInviata(tree, x->padre);
                }
                
            }
        }
    }
}

void leftRotateRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*x){
    richiestaInviataNode*y;
    y=x->figlioDestro;
    x->figlioDestro=y->figlioSinistro;
    
    if(y->figlioSinistro!=tree->null){
        y->figlioSinistro->padre=x;
    }
    y->padre=x->padre;
    if(x->padre==tree->null){
        tree->root=y;
    }
    else if(x==x->padre->figlioSinistro){
        x->padre->figlioSinistro=y;
    }
    else x->padre->figlioDestro=y;
    y->figlioSinistro=x;
    x->padre=y;
}

void rightRotateRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*y){
    richiestaInviataNode*x;
    x=y->figlioSinistro;
    y->figlioSinistro=x->figlioDestro;
    
    if(x->figlioDestro!=tree->null){
        x->figlioDestro->padre=y;
    }
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    x->figlioDestro=y;
    y->padre=x;
}

richiestaInviataNode* eliminateRichiestaInviataNode(richiesteInviateTree*tree, richiestaInviataNode*daEliminare){
    richiestaInviataNode*y;
    richiestaInviataNode*x;
    if(daEliminare->figlioSinistro==tree->null||daEliminare->figlioDestro==tree->null){
        y=daEliminare;
    }
    else y=treeSuccessorRichiestaInviata(daEliminare, tree);
    if(y->figlioSinistro!=tree->null){
        x=y->figlioSinistro;
    }
    else x=y->figlioDestro;
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    if(y!=daEliminare){

        int oldcolor=y->color;
        y->color=daEliminare->color;

        if(daEliminare==tree->root){
            tree->root=y;
        }
        else {
            if (daEliminare->padre->figlioSinistro == daEliminare) {
                daEliminare->padre->figlioSinistro = y;
            }
            else daEliminare->padre->figlioDestro = y;
        }
        daEliminare->figlioSinistro->padre = y;
        daEliminare->figlioDestro->padre = y;
        y->padre = daEliminare->padre;
        y->figlioSinistro = daEliminare->figlioSinistro;
        y->figlioDestro = daEliminare->figlioDestro;
        if(oldcolor=='B'){
            treeDeleteFixUpRichiestaInviata(tree, x);
        }

        return daEliminare;
    }
    if(y->color=='B'){
        treeDeleteFixUpRichiestaInviata(tree, x);
    }
    return y;
}

void treeDeleteFixUpRichiestaInviata(richiesteInviateTree*tree, richiestaInviataNode*x){
    richiestaInviataNode*w;
    if(x->color=='R' || x->padre==tree->null){
        x->color='B';
    }
    else if(x==x->padre->figlioSinistro){
        w=x->padre->figlioDestro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            leftRotateRichiestaInviata(tree, x->padre);
            w=x->padre->figlioDestro;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpRichiestaInviata(tree, x->padre);
        }
        else{
            if(w->figlioDestro->color=='B'){
                w->figlioSinistro->color='B';
                w->color='R';
                rightRotateRichiestaInviata(tree, w);
                w=x->padre->figlioDestro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioDestro->color='B';
            leftRotateRichiestaInviata(tree, x->padre);
        }
    }
    else{
        w=x->padre->figlioSinistro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            rightRotateRichiestaInviata(tree, x->padre);
            w=x->padre->figlioSinistro;;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpRichiestaInviata(tree, x->padre);
        }
        else{
            if(w->figlioSinistro->color=='B'){
                w->figlioDestro->color='B';
                w->color='R';
                leftRotateRichiestaInviata(tree, w);
                w=x->padre->figlioSinistro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioSinistro->color='B';
            rightRotateRichiestaInviata(tree, x->padre);
        }
    }
    
}



void inorderTreeWalkRichiestaInviata(richiestaInviataNode*x, richiesteInviateTree*tree){
    if(x!=tree->null){
        inorderTreeWalkRichiestaInviata(x->figlioSinistro, tree);
        printf("key is: %s \n color is: %c \n", x->key, x->color);
        inorderTreeWalkRichiestaInviata(x->figlioDestro,tree);
    }
}

richiestaInviataNode* treeSearchRichiestaInviata(richiestaInviataNode*nodo, char key[32], richiesteInviateTree*tree){
    if(nodo==tree->null||strcmp(key,nodo->key)==0){
        return nodo;
    }
    if(strcmp(key,nodo->key)<0){
        return treeSearchRichiestaInviata(nodo->figlioSinistro, key, tree);
    }
    else return treeSearchRichiestaInviata(nodo->figlioDestro, key, tree);
}
//*****************************FINE ALBERO RB LISTA ISTANZE DI RELAZIONI DI UNA ENTITA*********************


//*****************************ALBERO RB Lista puntatori di RICHIESTE INVIATE*********************
listaPuntatoriRichiesteInviateNodo* treeMinimumListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree){
    while(x->figlioSinistro!=tree->null){
        x=x->figlioSinistro;
    }
    return x;
}

listaPuntatoriRichiesteInviateNodo* treeMaximumListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree){
    while(x->figlioDestro!=tree->null){
        x=x->figlioDestro;
    }
    return x;
}

listaPuntatoriRichiesteInviateNodo* treeSuccessorListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree){
    listaPuntatoriRichiesteInviateNodo*y;
    if(x->figlioDestro!=tree->null){
        return treeMinimumListaRichiestaInviata(x->figlioDestro, tree);
    }
    y=x->padre;
    while(y!=tree->null&&x==y->figlioDestro){
        x=y;
        y=y->padre;
    }
    return y;
}

void addListaRichiestaInviataNode(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*daInserire){
    listaPuntatoriRichiesteInviateNodo*y=tree->null;
    listaPuntatoriRichiesteInviateNodo*x=tree->root;
    
    while(x!=tree->null){
        y=x;
        if(strcmp(daInserire->nomeRelazione,x->nomeRelazione)<0){
            x=x->figlioSinistro;
        }
        else x=x->figlioDestro;
    }
    daInserire->padre=y;
    if(y==tree->null){
        tree->root=daInserire;
    }
    else if(strcmp(daInserire->nomeRelazione,y->nomeRelazione)<0){
        y->figlioSinistro=daInserire;
    }
    else y->figlioDestro=daInserire;
    
    daInserire->figlioDestro=tree->null;
    daInserire->figlioSinistro=tree->null;
    
    daInserire->color='R';
    
    treeInsertFixUpListaRichiestaInviata(tree, daInserire);
}



void treeInsertFixUpListaRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*nuovo){
    listaPuntatoriRichiesteInviateNodo*x;
    listaPuntatoriRichiesteInviateNodo*y;
    
    if(nuovo==tree->root){
        tree->root->color='B';
    }
    else{
        x=nuovo->padre;
        if(x->color=='R'){
            if(x==x->padre->figlioSinistro){
                y=x->padre->figlioDestro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpListaRichiestaInviata(tree,x->padre);
                }
                
                else{
                    if(nuovo==x->figlioDestro){
                        nuovo=x;
                        leftRotateListaRichiestaInviata(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    rightListaRotateRichiestaInviata(tree, x->padre);
                }
            }
            else{   y=x->padre->figlioSinistro;
                if(y->color=='R'){
                    x->color='B';
                    y->color='B';
                    x->padre->color='R';
                    treeInsertFixUpListaRichiestaInviata(tree,x->padre);
                }
                else{
                    if(nuovo==x->figlioSinistro){
                        nuovo=x;
                        rightListaRotateRichiestaInviata(tree, nuovo);
                        x=nuovo->padre;
                    }
                    x->color='B';
                    x->padre->color='R';
                    leftRotateListaRichiestaInviata(tree, x->padre);
                }
                
            }
        }
    }
}

void leftRotateListaRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*x){
    listaPuntatoriRichiesteInviateNodo*y;
    y=x->figlioDestro;
    x->figlioDestro=y->figlioSinistro;
    
    if(y->figlioSinistro!=tree->null){
        y->figlioSinistro->padre=x;
    }
    y->padre=x->padre;
    if(x->padre==tree->null){
        tree->root=y;
    }
    else if(x==x->padre->figlioSinistro){
        x->padre->figlioSinistro=y;
    }
    else x->padre->figlioDestro=y;
    y->figlioSinistro=x;
    x->padre=y;
}

void rightListaRotateRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*y){
    listaPuntatoriRichiesteInviateNodo*x;
    x=y->figlioSinistro;
    y->figlioSinistro=x->figlioDestro;
    
    if(x->figlioDestro!=tree->null){
        x->figlioDestro->padre=y;
    }
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    x->figlioDestro=y;
    y->padre=x;
}

listaPuntatoriRichiesteInviateNodo* eliminateListaRichiestaInviataNode(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*daEliminare){
    listaPuntatoriRichiesteInviateNodo*y;
    listaPuntatoriRichiesteInviateNodo*x;
    if(daEliminare->figlioSinistro==tree->null||daEliminare->figlioDestro==tree->null){
        y=daEliminare;
    }
    else y=treeSuccessorListaRichiestaInviata(daEliminare, tree);
    if(y->figlioSinistro!=tree->null){
        x=y->figlioSinistro;
    }
    else x=y->figlioDestro;
    x->padre=y->padre;
    if(y->padre==tree->null){
        tree->root=x;
    }
    else if(y==y->padre->figlioSinistro){
        y->padre->figlioSinistro=x;
    }
    else y->padre->figlioDestro=x;
    if(y!=daEliminare){
        int oldcolor=y->color;
        y->color=daEliminare->color;

        if(daEliminare==tree->root){
            tree->root=y;
        }
        else {
            if (daEliminare->padre->figlioSinistro == daEliminare) {
                daEliminare->padre->figlioSinistro = y;
            }
            else daEliminare->padre->figlioDestro = y;
        }
        daEliminare->figlioSinistro->padre = y;
        daEliminare->figlioDestro->padre = y;
        y->padre = daEliminare->padre;
        y->figlioSinistro = daEliminare->figlioSinistro;
        y->figlioDestro = daEliminare->figlioDestro;
        if(oldcolor=='B'){
            treeDeleteFixUpListaRichiestaInviata(tree, x);
        }

        return daEliminare;
    }
    if(y->color=='B'){
        treeDeleteFixUpListaRichiestaInviata(tree, x);
    }
    return y;
}

void treeDeleteFixUpListaRichiestaInviata(listaPuntatoriRichiesteInviateTree*tree, listaPuntatoriRichiesteInviateNodo*x){
    listaPuntatoriRichiesteInviateNodo*w;
    if(x->color=='R' || x->padre==tree->null){
        x->color='B';
    }
    else if(x==x->padre->figlioSinistro){
        w=x->padre->figlioDestro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            leftRotateListaRichiestaInviata(tree, x->padre);
            w=x->padre->figlioDestro;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpListaRichiestaInviata(tree, x->padre);
        }
        else{
            if(w->figlioDestro->color=='B'){
                w->figlioSinistro->color='B';
                w->color='R';
                rightListaRotateRichiestaInviata(tree, w);
                w=x->padre->figlioDestro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioDestro->color='B';
            leftRotateListaRichiestaInviata(tree, x->padre);
        }
    }
    else{
        w=x->padre->figlioSinistro;
        if(w->color=='R'){
            w->color='B';
            x->padre->color='R';
            rightListaRotateRichiestaInviata(tree, x->padre);
            w=x->padre->figlioSinistro;;
        }
        if(w->figlioSinistro->color=='B'&&w->figlioDestro->color=='B'){
            w->color='R';
            treeDeleteFixUpListaRichiestaInviata(tree, x->padre);
        }
        else{
            if(w->figlioSinistro->color=='B'){
                w->figlioDestro->color='B';
                w->color='R';
                leftRotateListaRichiestaInviata(tree, w);
                w=x->padre->figlioSinistro;
            }
            w->color=x->padre->color;
            x->padre->color='B';
            w->figlioSinistro->color='B';
            rightListaRotateRichiestaInviata(tree, x->padre);
        }
    }
    
}



void inorderTreeWalkListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree){
    if(x!=tree->null){
        inorderTreeWalkListaRichiestaInviata(x->figlioSinistro, tree);
        printf("key is: %s \n color is: %c \n", x->nomeRelazione, x->color);
        inorderTreeWalkListaRichiestaInviata(x->figlioDestro,tree);
    }
}

listaPuntatoriRichiesteInviateNodo* treeSearchListaRichiestaInviata(listaPuntatoriRichiesteInviateNodo*nodo, char key[32], listaPuntatoriRichiesteInviateTree*tree){
    if(nodo==tree->null||strcmp(key,nodo->nomeRelazione)==0){
        return nodo;
    }
    if(strcmp(key,nodo->nomeRelazione)<0){
        return treeSearchListaRichiestaInviata(nodo->figlioSinistro, key, tree);
    }
    else return treeSearchListaRichiestaInviata(nodo->figlioDestro, key, tree);
}
//*****************************FINE ALBERO RB LISTA ISTANZE DI RELAZIONI DI UNA ENTITA*********************

/**********************+       FUNZIONI AUSILIARI PROGRAMMA                    ***********************************************************************/
void eliminaRelazioni(listaPtrNodiRelazioniNodo*x, listaPtrNodiRelazioniTree*tree, listaAlberiDiRelazioniTree*relationList){
    if (x != tree->null) {
        eliminaRelazioni(x->figlioSinistro, tree, relationList);

        relationTree *albero = treeSearchListaAlberiDiRelazioni(relationList->root, x->nome, relationList)->albero;
        printf("elimino relazioni dall'albero: %s \n", x->nome );

        printf("delent albero prima della eliminate \n");
        inorderTreeWalkRelation(albero->root, albero);

        eliminateRelationNode(albero, x->relazione);

        printf("delent albero dopo della eliminate \n");
        inorderTreeWalkRelation(albero->root, albero);

        if (albero->root == albero->null) {
            printf("prima di eliminare l'albero dall'albero degli alberi \n");
            inorderTreeWalkRelation(albero->root, albero);

            eliminateListaAlberiDiRelazioniNodo(relationList,treeSearchListaAlberiDiRelazioni(relationList->root, x->nome,relationList));

            printf("dopo di eliminare l'albero dall'albero degli alberi \n");
            inorderTreeWalkRelation(albero->root, albero);
        }
        eliminaRelazioni(x->figlioDestro, tree, relationList);
    }
}

void eliminaRichiesteInviate(listaPuntatoriRichiesteInviateNodo*x, listaPuntatoriRichiesteInviateTree*tree, listaAlberiDiRelazioniTree*relationList, entityTree*entityTree){
    if(x!=tree->null){
        eliminaRichiesteInviate(x->figlioSinistro, tree, relationList,entityTree);
        relationTree*albero=treeSearchListaAlberiDiRelazioni(relationList->root, x->nomeRelazione, relationList)->albero;
        printf("delent albero prima \n");
        inorderTreeWalkRelation(albero->root, albero);
        decrementaRichieste(x->relazioni->root, x->relazioni, albero, x->nomeRelazione, entityTree);
        if(albero->root==albero->null){
            eliminateListaAlberiDiRelazioniNodo(relationList, treeSearchListaAlberiDiRelazioni(relationList->root, x->nomeRelazione, relationList));
        }
        //printf("root is:%s\n", albero->root->entitaDestinataria->nomeEntita);
        printf("albero dopo \n");
        inorderTreeWalkRelation(albero->root, albero);
        eliminaRichiesteInviate(x->figlioDestro,tree, relationList,entityTree);

    }

}

void decrementaRichieste(richiestaInviataNode*x, richiesteInviateTree*tree, relationTree*albero, char nomeRelazione[32],entityTree*entityTree){
    if(x!=tree->null){
        decrementaRichieste(x->figlioSinistro, tree, albero, nomeRelazione,entityTree);
        printf("\tanalizing x: x->key(%s), x->referredEntity(%s)\n", x->key, x->referredEntity->nomeEntita);

        printf("\t\tsearching: nomeRelazione(%s) in x(%s)->referredEntity->relation->root\n", nomeRelazione, x->referredEntity->nomeEntita);
        listaPtrNodiRelazioniNodo * puntatore = treeSearchListaPtrNodiRelazioni(x->referredEntity->relazioni->root, nomeRelazione, x->referredEntity->relazioni);
        printf("\t\telement found is puntatore(%s)\n", puntatore->nome);

        printf("\t\tdeleting from albero(?) the node puntatore(%s)->relazione(%d)\n", puntatore->nome, puntatore->relazione->key);

        printf("before delete the tree is:\n");
        //inorderTreeWalkRelation(albero->root, albero);
        relationNode * nuova = eliminateRelationNode(albero, puntatore->relazione);
        printf("after delete the tree is:\n");
        //inorderTreeWalkRelation(albero->root, albero);
        printf("\t\tnuova after eliminateRelationNode(...) is: key(%d) entitaDestinataria(%s)\n", nuova->key, nuova->entitaDestinataria->nomeEntita);

        nuova->key=puntatore->relazione->key-1;
        nuova->entitaDestinataria=puntatore->relazione->entitaDestinataria;
        printf("\t\tnuova after has changed in: key(%d) entitaDestinataria(%s)\n", nuova->key, nuova->entitaDestinataria->nomeEntita);


        if(nuova->key>0){
            printf("the nuova->key(%d) is > 0\n", nuova->key);
            printf("\t\tadding relation to albero(?) the node nuova\n");

            printf("before add the tree is:\n");
           // inorderTreeWalkRelation(albero->root, albero);

            addRelationNode(albero, nuova);

            printf("after add the tree is:\n");
           // inorderTreeWalkRelation(albero->root, albero);
            /*if(strcmp(x->key, "Grunt") == 0){
                printf("exiting\n");
                char pausa[6];
               scanf("%s", pausa);
            }*/

        }
        else{
            printf("\t\teliminate from x->referredEntity(%s)->relazioni the node puntatore(%s)\n", x->referredEntity->nomeEntita, puntatore->nome);
            eliminateListaPtrNodiRelazioni(x->referredEntity->relazioni, puntatore);
        }

        decrementaRichieste(x->figlioDestro,tree, albero, nomeRelazione,entityTree);
    }
}



/**********************+   END    FUNZIONI AUSILIARI PROGRAMMA                    ***********************************************************************/


//*****************************FUNZIONI PROGRAMMA ***********************************************
void aggiungiEntita(char*name, entityTree*tree){
    entityNode*nodo=malloc(sizeof(entityNode));
    if(treeSearchEntity(tree->root, name, tree)!=tree->null){
        return;
    };

    for(int i=0; i<32; i++){
    nodo->nomeEntita[i]=name[i];
    }
    nodo->relazioni=NULL;
    nodo->listaPuntatoriRichiesteInviate=NULL;
    addEntityNode(tree, nodo);
}

void eliminaEntita(char*name, entityTree*tree, listaAlberiDiRelazioniTree*relationList){

    printf("searching entity %s\n", name);
    entityNode*node=treeSearchEntity(tree->root, name, tree);
    printf("entity found: node(%s)\n", node->nomeEntita);
    node=eliminateEntityNode(tree, node);
    printf("delented node from the entity tree\n");

    if(node->relazioni!=NULL){
        printf("eliminaRelazioni started!\n");
        eliminaRelazioni(node->relazioni->root, node->relazioni, relationList);
        printf("eliminaRelazioni ended!\n");
    }


    if(node->listaPuntatoriRichiesteInviate!=NULL){
        printf("eliminaRichiesteInviate started!\n");
        eliminaRichiesteInviate(node->listaPuntatoriRichiesteInviate->root, node->listaPuntatoriRichiesteInviate, relationList, tree);
        printf("eliminaRIchiesteInviate ended!\n");
    }



}

void aggiungiRelazione(char nomeMittente[32], char nomeDestinatario[32], char nomeRelazione[32], entityTree*entityTree, listaAlberiDiRelazioniTree*relationList){
   
    entityNode*mittente=treeSearchEntity(entityTree->root, nomeMittente, entityTree);
    entityNode*destinatario=treeSearchEntity(entityTree->root, nomeDestinatario, entityTree);
   
    //controllo che mittente e destinatario siano entità monitorate
    if(mittente!=entityTree->null&&destinatario!=entityTree->null){
      //nel caso lo siano, aggiungo relazione alla lista delle relazioni, se questa non esiste gia
        listaAlberiDiRelazioniNodo*puntatoreRelazione=treeSearchListaAlberiDiRelazioni(relationList->root, nomeRelazione, relationList);
        if(puntatoreRelazione==relationList->null){
       //se non esiste, la aggiungo e creo un albero
            listaAlberiDiRelazioniNodo*nuovaRelazione=malloc(sizeof(listaAlberiDiRelazioniNodo));
            strcpy(nuovaRelazione->nomeRelazione, nomeRelazione);
            nuovaRelazione->albero=malloc(sizeof(relationTree));
            nuovaRelazione->albero->null=malloc(sizeof(relationNode));
            nuovaRelazione->albero->null->padre=nuovaRelazione->albero->null;
            nuovaRelazione->albero->null->figlioDestro=nuovaRelazione->albero->null;
            nuovaRelazione->albero->null->figlioSinistro=nuovaRelazione->albero->null;
            nuovaRelazione->albero->null->color='B';
            nuovaRelazione->albero->null->key=0;
            nuovaRelazione->albero->root=nuovaRelazione->albero->null;
            addListaAlberiDiRelazioniNodo(relationList, nuovaRelazione);
            relationNode*relazione=malloc(sizeof(relationNode));
            relazione->entitaDestinataria=destinatario;
            relazione->key=1;
            if(mittente->listaPuntatoriRichiesteInviate==NULL){
                mittente->listaPuntatoriRichiesteInviate=malloc(sizeof(listaPuntatoriRichiesteInviateTree));
                mittente->listaPuntatoriRichiesteInviate->null=malloc(sizeof(listaPuntatoriRichiesteInviateNodo));
                mittente->listaPuntatoriRichiesteInviate->null->padre=mittente->listaPuntatoriRichiesteInviate->null;
                mittente->listaPuntatoriRichiesteInviate->null->figlioDestro=mittente->listaPuntatoriRichiesteInviate->null;
                mittente->listaPuntatoriRichiesteInviate->null->figlioSinistro=mittente->listaPuntatoriRichiesteInviate->null;
                mittente->listaPuntatoriRichiesteInviate->null->color='B';
                mittente->listaPuntatoriRichiesteInviate->root=mittente->listaPuntatoriRichiesteInviate->null;
            }
            listaPuntatoriRichiesteInviateNodo*puntatoreAlleRichiesteInviate=treeSearchListaRichiestaInviata(mittente->listaPuntatoriRichiesteInviate->root, nomeRelazione, mittente->listaPuntatoriRichiesteInviate);
            //se non esiste ancora alcuna richiesta inviata dal suddetto mittente per suddetta relazione, creo un albero
            if(puntatoreAlleRichiesteInviate==mittente->listaPuntatoriRichiesteInviate->null){
                puntatoreAlleRichiesteInviate=malloc(sizeof(listaPuntatoriRichiesteInviateNodo));
                strcpy(puntatoreAlleRichiesteInviate->nomeRelazione, nomeRelazione);
                puntatoreAlleRichiesteInviate->relazioni=malloc(sizeof(richiesteInviateTree));
                puntatoreAlleRichiesteInviate->relazioni->null=malloc(sizeof(richiestaInviataNode));
                puntatoreAlleRichiesteInviate->relazioni->null->padre=puntatoreAlleRichiesteInviate->relazioni->null;
                puntatoreAlleRichiesteInviate->relazioni->null->figlioDestro=puntatoreAlleRichiesteInviate->relazioni->null;
                puntatoreAlleRichiesteInviate->relazioni->null->figlioSinistro=puntatoreAlleRichiesteInviate->relazioni->null;
                puntatoreAlleRichiesteInviate->relazioni->null->color='B';
                puntatoreAlleRichiesteInviate->relazioni->root=puntatoreAlleRichiesteInviate->relazioni->null;
                addListaRichiestaInviataNode(mittente->listaPuntatoriRichiesteInviate,puntatoreAlleRichiesteInviate);
            }
            //creo e aggiungo la relazione all'albero e modifico i dati relativi nel nodo dell'entità
            //devo: 1 creare la relazione, inizializzare la chiave a uno (che è il numero di richieste entranti), aggiungerla all'albero appena creato
            //2: creare l'albero delle richieste inviate dal mittente, se questo non esiste già, e aggiungervi un puntatore relativo alla relazione in questione, se questo non esiste già
            //creo la richiesta
            addRelationNode(nuovaRelazione->albero, relazione);
            richiestaInviataNode*nuovaRichiestaInviata=malloc(sizeof(richiestaInviataNode));
            nuovaRichiestaInviata->referredEntity=destinatario;
            strcpy(nuovaRichiestaInviata->key,nomeDestinatario);
                // se l'entità non è involved ancora in nemmeno una relazione, creo il primo puntatore
            if(destinatario->relazioni==NULL){
                destinatario->relazioni=malloc(sizeof(listaPtrNodiRelazioniTree));
                destinatario->relazioni->null=malloc(sizeof(listaPtrNodiRelazioniNodo));
                destinatario->relazioni->null->padre=destinatario->relazioni->null;
                destinatario->relazioni->null->figlioDestro=destinatario->relazioni->null;
                destinatario->relazioni->null->figlioSinistro=destinatario->relazioni->null;
                destinatario->relazioni->null->color='B';
                destinatario->relazioni->root=destinatario->relazioni->null;
            }
            // se no, agginugo semplicemente il nuovo puntatore a istanza di relazione
                listaPtrNodiRelazioniNodo*nuovoPuntatore=malloc(sizeof(listaPtrNodiRelazioniNodo));
                nuovoPuntatore->relazione=relazione;
                strcpy(nuovoPuntatore->nome, nomeRelazione);
            addListaPtrNodiRelazioniNodo(destinatario->relazioni, nuovoPuntatore);
            addRichiestaInviataNode(puntatoreAlleRichiesteInviate->relazioni, nuovaRichiestaInviata);

        }
        //se esiste già
        else{
            //controllo che il destinatario abbia qualsiasi tipo di relazione
                if(destinatario->relazioni==NULL){
                    destinatario->relazioni=malloc(sizeof(listaPtrNodiRelazioniTree));
                    destinatario->relazioni->null=malloc(sizeof(listaPtrNodiRelazioniNodo));
                    destinatario->relazioni->null->padre=destinatario->relazioni->null;
                    destinatario->relazioni->null->figlioDestro=destinatario->relazioni->null;
                    destinatario->relazioni->null->figlioSinistro=destinatario->relazioni->null;
                    destinatario->relazioni->null->color='B';
                    destinatario->relazioni->root=destinatario->relazioni->null;
                }
                listaPtrNodiRelazioniNodo*puntatoreNodoRelazione=treeSearchListaPtrNodiRelazioni(destinatario->relazioni->root, nomeRelazione, destinatario->relazioni);
            //se non ne ha, evidentemente non è presente in nessun albero
                if(puntatoreNodoRelazione==destinatario->relazioni->null){
                    relationNode*nuova=malloc(sizeof(relationNode));
                    nuova->key=0;
                    nuova->entitaDestinataria=destinatario;
                    addRelationNode(puntatoreRelazione->albero, nuova);
                    listaPtrNodiRelazioniNodo*nuovoPuntatore=malloc(sizeof(listaPtrNodiRelazioniNodo));
                    strcpy(nuovoPuntatore->nome, nomeRelazione);
                    nuovoPuntatore->relazione=nuova;
                    addListaPtrNodiRelazioniNodo(destinatario->relazioni, nuovoPuntatore);
                }
            //controllo se il mittente ha mai inviato un richiesta//se non ne ha mai inviata nemmeno una,
                if(mittente->listaPuntatoriRichiesteInviate==NULL){
                    mittente->listaPuntatoriRichiesteInviate=malloc(sizeof(listaPuntatoriRichiesteInviateTree));
                    mittente->listaPuntatoriRichiesteInviate->null=malloc(sizeof(listaPuntatoriRichiesteInviateNodo));
                    mittente->listaPuntatoriRichiesteInviate->null->padre=mittente->listaPuntatoriRichiesteInviate->null;
                    mittente->listaPuntatoriRichiesteInviate->null->figlioDestro=mittente->listaPuntatoriRichiesteInviate->null;
                    mittente->listaPuntatoriRichiesteInviate->null->figlioSinistro=mittente->listaPuntatoriRichiesteInviate->null;
                    mittente->listaPuntatoriRichiesteInviate->null->color='B';
                    mittente->listaPuntatoriRichiesteInviate->root=mittente->listaPuntatoriRichiesteInviate->null;
                }
                listaPuntatoriRichiesteInviateNodo*puntatoreAlleRichiesteInviate=treeSearchListaRichiestaInviata(mittente->listaPuntatoriRichiesteInviate->root, nomeRelazione,mittente->listaPuntatoriRichiesteInviate);
            //se non ne ha mai inviata una per quella relazione, la creo
                if(puntatoreAlleRichiesteInviate==mittente->listaPuntatoriRichiesteInviate->null){
                    puntatoreAlleRichiesteInviate=malloc(sizeof(listaPuntatoriRichiesteInviateNodo));
                    strcpy(puntatoreAlleRichiesteInviate->nomeRelazione, nomeRelazione);
                    puntatoreAlleRichiesteInviate->relazioni=malloc(sizeof(richiesteInviateTree));
                    puntatoreAlleRichiesteInviate->relazioni->null=malloc(sizeof(richiestaInviataNode));
                    puntatoreAlleRichiesteInviate->relazioni->null->padre=puntatoreAlleRichiesteInviate->relazioni->null;
                    puntatoreAlleRichiesteInviate->relazioni->null->figlioDestro=puntatoreAlleRichiesteInviate->relazioni->null;
                    puntatoreAlleRichiesteInviate->relazioni->null->figlioSinistro=puntatoreAlleRichiesteInviate->relazioni->null;
                    puntatoreAlleRichiesteInviate->relazioni->null->color='B';
                    puntatoreAlleRichiesteInviate->relazioni->root=puntatoreAlleRichiesteInviate->relazioni->null;
                    addListaRichiestaInviataNode(mittente->listaPuntatoriRichiesteInviate,puntatoreAlleRichiesteInviate);
                }
                puntatoreNodoRelazione=treeSearchListaPtrNodiRelazioni(destinatario->relazioni->root, nomeRelazione, destinatario->relazioni);
                puntatoreAlleRichiesteInviate=treeSearchListaRichiestaInviata(mittente->listaPuntatoriRichiesteInviate->root, nomeRelazione,mittente->listaPuntatoriRichiesteInviate);
                if(treeSearchRichiestaInviata(puntatoreAlleRichiesteInviate->relazioni->root, nomeDestinatario, puntatoreAlleRichiesteInviate->relazioni)==puntatoreAlleRichiesteInviate->relazioni->null){
                    //creo e aggiungo una richiesta alla lista del mittente con riferimento al destinatario della richiesta
                    richiestaInviataNode*nuovaRichiesta=malloc(sizeof(richiestaInviataNode));
                    strcpy(nuovaRichiesta->key,nomeDestinatario);
                    nuovaRichiesta->referredEntity=destinatario;
                    //aggiungo un nuovo nodo con chiave incrementata che va a sostituire il precedente per presevare l'ordine dell'albero
                    relationNode*sostitusco=malloc(sizeof(relationNode));
                    sostitusco->key=puntatoreNodoRelazione->relazione->key+1;
                    addRichiestaInviataNode( puntatoreAlleRichiesteInviate->relazioni, nuovaRichiesta);
                    sostitusco->entitaDestinataria=puntatoreNodoRelazione->relazione->entitaDestinataria;
                     //printf("albero prima:\n");
                    // printf("sostituisco: %s",puntatoreNodoRelazione->relazione->entitaDestinataria->nomeEntita );
                     //inorderTreeWalkRelation(puntatoreRelazione->albero->root, puntatoreRelazione->albero);
                     eliminateRelationNode(puntatoreRelazione->albero, treeSearchRelation(puntatoreRelazione->albero->root, puntatoreNodoRelazione->relazione->key,  puntatoreNodoRelazione->relazione->entitaDestinataria, puntatoreRelazione->albero));
                    //printf("albero ora:\n");
                    //inorderTreeWalkRelation(puntatoreRelazione->albero->root, puntatoreRelazione->albero);
                     addRelationNode(puntatoreRelazione->albero, sostitusco);
                     puntatoreNodoRelazione->relazione=treeSearchRelation(puntatoreRelazione->albero->root, sostitusco->key, sostitusco->entitaDestinataria, puntatoreRelazione->albero);
                     //printf("albero ora:\n");
                     //inorderTreeWalkRelation(puntatoreRelazione->albero->root, puntatoreRelazione->albero);
                }
                    
                else{
                    return;
                }
        }
    }
  
}

void eliminaRelazione(char nomeMittente[32], char nomeDestinatario[32], char nomeRelazione[32], entityTree*entityTree, listaAlberiDiRelazioniTree*relationList){
    entityNode*mittente=treeSearchEntity(entityTree->root, nomeMittente, entityTree);
    entityNode*destinatario=treeSearchEntity(entityTree->root, nomeDestinatario, entityTree);
    //controllo che mittente e destinatario siano entità monitorate
    if(mittente!=entityTree->null&&destinatario!=entityTree->null){
        //controllo che il mittente abbia inviato almeno una richiesta
        if(mittente->listaPuntatoriRichiesteInviate!=NULL){
            listaPuntatoriRichiesteInviateNodo*alberoRichieste=treeSearchListaRichiestaInviata(mittente->listaPuntatoriRichiesteInviate->root, nomeRelazione, mittente->listaPuntatoriRichiesteInviate);
            if(alberoRichieste!=mittente->listaPuntatoriRichiesteInviate->null){
                richiestaInviataNode*richiesta=treeSearchRichiestaInviata(alberoRichieste->relazioni->root, nomeDestinatario, alberoRichieste->relazioni);
               //controllo che la richiesta sia quella giusta, specifica per il destinatario indicato
                if(richiesta!=alberoRichieste->relazioni->null){
                    //se prsente, rilascio la richiesta
                    richiesta=eliminateRichiestaInviataNode(alberoRichieste->relazioni, richiesta);
                     //richiesta;
                    listaPtrNodiRelazioniNodo*daDecrementare=treeSearchListaPtrNodiRelazioni(destinatario->relazioni->root, nomeRelazione, destinatario->relazioni);
                    listaAlberiDiRelazioniNodo*alberoDellaRelazione=treeSearchListaAlberiDiRelazioni(relationList->root, nomeRelazione, relationList);
                    printf(" delrel albero prima \n");
                    inorderTreeWalkRelation(alberoDellaRelazione->albero->root, alberoDellaRelazione->albero);
                    if(daDecrementare->relazione->key==1){
                        eliminateListaPtrNodiRelazioni(destinatario->relazioni, daDecrementare);
                        eliminateRelationNode(alberoDellaRelazione->albero, daDecrementare->relazione);
                        //free(daEliminare);
                        if(alberoDellaRelazione->albero->root==alberoDellaRelazione->albero->null){
                            eliminateListaAlberiDiRelazioniNodo(relationList, alberoDellaRelazione);
                        }
                    }
                        else{
                        relationNode*sostituisco=eliminateRelationNode(alberoDellaRelazione->albero, daDecrementare->relazione);
                        sostituisco->key=daDecrementare->relazione->key-1;
                        sostituisco->entitaDestinataria=daDecrementare->relazione->entitaDestinataria;
                        addRelationNode(alberoDellaRelazione->albero, sostituisco);
                        }


                    printf("delrel albero dopo \n");
                    inorderTreeWalkRelation(alberoDellaRelazione->albero->root, alberoDellaRelazione->albero);
                    if(alberoRichieste->relazioni->root==alberoRichieste->relazioni->null){
                        alberoRichieste=eliminateListaRichiestaInviataNode(mittente->listaPuntatoriRichiesteInviate, alberoRichieste);
                        //free(alberoRichieste);
                    }
                }
            }
        }
    }
}


void report(listaAlberiDiRelazioniNodo*x,listaAlberiDiRelazioniTree*tree){
    if(x!=tree->null){
        int y;
        report(x->figlioSinistro, tree);
        relationNode*node=treeMaximumRelation(x->albero->root, x->albero);
        y=node->key;

        while(node->padre->key==node->key){
            node=node->padre;
        }
        printf("\"%s\" ", x->nomeRelazione);
        inorderStampaReport(node, x->albero, y);
        printf("%d; ", y);

      
        report(x->figlioDestro,tree);
    }
}

void inorderStampaReport(relationNode*x, relationTree*albero, int y){
    if(x!=albero->null){
        inorderStampaReport(x->figlioSinistro, albero, y);
        if(x->key==y){
        printf("\"%s\" ", x->entitaDestinataria->nomeEntita);
        }
        inorderStampaReport(x->figlioDestro, albero, y);
    }
}


//***************************** END FUNZIONI PROGRAMMA ***********************************************



int main(){
   //inizializzo albero RB

    char input[120];
    
    
    entityTree*entityTree=malloc(sizeof(entityTree));
    listaAlberiDiRelazioniTree*relationList=malloc(sizeof(listaAlberiDiRelazioniTree));
    
    relationList->null=malloc(sizeof(listaAlberiDiRelazioniNodo));
    relationList->null->padre=relationList->null;
    relationList->null->figlioDestro=relationList->null;
    relationList->null->figlioSinistro=relationList->null;
    relationList->null->color='B';
    //fine inizializzazione null
    
    relationList->root=relationList->null;
    
    //inizializzo null
    entityTree->null=malloc(sizeof(entityNode));
    entityTree->null->padre=entityTree->null;
    entityTree->null->figlioDestro=entityTree->null;
    entityTree->null->figlioSinistro=entityTree->null;
    entityTree->null->color='B';
    //fine inizializzazione null
    
    entityTree->root=entityTree->null;

    
    while(strncmp(fgets(input, 120, stdin), "end", 3)!=0){
        printf("**********************************************************************************\n");
        printf("reding command: %s\n", input);
        if(strncmp(input, "addent", 6)==0){
            char name[32];
            int i=0;
          
            while(input[i]!='"'){
                i++;
            }
            i++;
            int j=0;
            
            while(input[i]!='"'){
                name[j]=input[i];
                j++;
                i++;
            }
           
            while(j<32){
                name[j]='\0';
                j++;
            }
            
            fflush(stdin);
            aggiungiEntita(name, entityTree);
        }
        if(strncmp(input, "delent", 6)==0){
            char name[32];
            int i=0;
            
            while(input[i]!='"'){
                i++;
            }
            i++;
            int j=0;
            
            while(input[i]!='"'){
                name[j]=input[i];
                j++;
                i++;
            }
            
            while(j<32){
                name[j]='\0';
                j++;
            }
            
            fflush(stdin);
            eliminaEntita(name, entityTree, relationList);
        }
        if(strncmp(input, "addrel", 6)==0){
            char nomeRelazione[32];
            char nome1[32];
            char nome2[32];
            int i=0;
            
            while(input[i]!='"'){
                i++;
            }
            i++;
            int j=0;
            
            while(input[i]!='"'){
                nome1[j]=input[i];
                j++;
                i++;
            }
            i++;
            
            while(j<32){
                nome1[j]='\0';
                j++;
            }
            
            while(input[i]!='"'){
                i++;
            }
            i++;
            j=0;
            
            while(input[i]!='"'){
                nome2[j]=input[i];
                j++;
                i++;
            }
            
            while(j<32){
                nome2[j]='\0';
                j++;
            }
            i++;
            while(input[i]!='"'){
                i++;
            }
            i++;
            j=0;
            
            while(input[i]!='"'){
                nomeRelazione[j]=input[i];
                j++;
                i++;
            }
            
            while(j<32){
                nomeRelazione[j]='\0';
                j++;
            }
            
            fflush(stdin);
            aggiungiRelazione(nome1, nome2, nomeRelazione, entityTree, relationList) ;
            
        }
        if(strncmp(input, "delrel", 6)==0){
            char nomeRelazione[32];
            char nome1[32];
            char nome2[32];
            int i=0;
            
            while(input[i]!='"'){
                i++;
            }
            i++;
            int j=0;
            
            while(input[i]!='"'){
                nome1[j]=input[i];
                j++;
                i++;
            }
            i++;
            
            while(j<32){
                nome1[j]='\0';
                j++;
            }
            
            while(input[i]!='"'){
                i++;
            }
            i++;
            j=0;
            
            while(input[i]!='"'){
                nome2[j]=input[i];
                j++;
                i++;
            }
            
            while(j<32){
                nome2[j]='\0';
                j++;
            }
            i++;
            while(input[i]!='"'){
                i++;
            }
            i++;
            j=0;
            
            while(input[i]!='"'){
                nomeRelazione[j]=input[i];
                j++;
                i++;
            }
            
            while(j<32){
                nomeRelazione[j]='\0';
                j++;
            }
            
            fflush(stdin);
            eliminaRelazione(nome1, nome2, nomeRelazione, entityTree, relationList) ;
            
        }
        if(strncmp(input, "report", 6)==0){
            if(relationList->root==relationList->null){
                 fflush(stdin);
                printf("none\n");
                fflush(stdin);
            }
            else{
                 fflush(stdin);
            report(relationList->root, relationList);
               // inorderTreeWalkRelation(relationList->root->albero->root, relationList->root->albero);
                fflush(stdin);
                printf("\n");
                 fflush(stdin);
            }
            fflush(stdin);
        }
        printf("**********************************************************************************\n\n");

        // fflush(stdin);
    }
   /* inorderTreeWalkEntity(entityTree->root, entityTree);
    inorderTreeWalkListaAlberiDiRelazioni(relationList->root, relationList);
    if(relationList->root!=relationList->null){
    inorderTreeWalkRelation(relationList->root->albero->root, relationList->root->albero);
    }*/
};
