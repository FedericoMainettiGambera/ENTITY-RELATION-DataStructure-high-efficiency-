# High efficiency Entity-Relation Data Structure

Si vuole implementare un meccanismo di monitoraggio di relazioni tra
entità (per esempio persone) che cambiano nel tempo

Le relazioni tra entità non sono necessariamente simmetriche. Per
esempio, Alice può essere “amica” di Bruno, ma l’amicizia non è
reciprocata

In maniera più astratta, il meccanismo monitora i seguenti fenomeni:
• Una nuova entità comincia ad essere monitorata
• Una entità monitorata smette di esserlo
• Una nuova relazione viene stabilita tra 2 entità monitorate
• Una relazione esistente tra 2 entità monitorate cessa di esistere
Ogni entità ha un nome identificativo
Ci possono essere diversi tipi di relazioni tra entità, ognuna identificata da un
nome
Ogni relazione ha un verso e non è necessariamente simmetrica
A seguito di un apposito comando, il sistema restituisce, per ogni relazione,
l’entità che “riceve” più relazioni
L’applicativo dovrà essere ottimizzato per gestire un grande numero di entità e
istanze di relazioni, ma generalmente pochi tipi (identificativi) di relazione

Implementazione in linguaggio C standard (con la sola libc) di un
programma che legge da standard input una sequenza di comandi,
ognuno corrispondente ad un cambiamento nelle entità o nelle
relazioni tra entità e, quando richiesto, produce su standard output,
per ogni tipo di relazione monitorata, l'identificativo dell'entità che è il
ricevente del maggior numero di istanze di quella relazione, e il
numero di relazioni che l'entità riceve

I comandi che possono essere letti sono i seguenti:
• addent id_ent ⟨id_ent⟩ ⟩
• aggiunge un'entità identificata da "id_ent" all'insieme delle entità monitorate; se l'entità è già monitorata, non fa nulla
• delent id_ent ⟨id_ent⟩ ⟩
• elimina l'entità identificata da "id_ent" dall'insieme delle entità monitorate; elimina tutte le relazioni di cui "id_ent" fa parte
(sia come origine, che come destinazione)
• addrel id_orig id_dest id_rel ⟨id_ent⟩ ⟩ ⟨id_ent⟩ ⟩ ⟨id_ent⟩ ⟩
• aggiunge una relazione – identificata da "id_rel" – tra le entità "id_orig" e "id_dest", in cui "id_dest" è il ricevente della
relazione. Se la relazione tra "id_orig" e "id_dest" già esiste, o se almeno una delle entità non è monitorata, non fa nulla. Il
monitoraggio del tipo di relazione "id_rel" inizia implicitamente con il primo comando "addrel" che la riguarda.
• delrel id_orig id_dest id_rel ⟨id_ent⟩ ⟩ ⟨id_ent⟩ ⟩ ⟨id_ent⟩ ⟩
• elimina la relazione identificata da "id_rel" tra le entità "id_orig" e "id_dest" (laddove "id_dest" è il ricevente della relazione);
se non c'è relazione "id_rel" tra "id_orig" e "id_dest" (con "id_dest" come ricevente), non fa nulla
• report
• emette in output l’elenco delle relazioni, riportando per ciascuna le entità con il maggior numero di relazioni entranti, come
spiegato in seguito
• end
• termine della sequenza di comandi

TEST

DOCUMENTS

COME HO FATTO IN BREVE
