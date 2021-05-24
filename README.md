# High efficiency Entity-Relation Data Structure

## Topic

The purpose is to implement a mechanism for monitoring relations between
entities (e.g. people) that change over time.

Relationships between entities are not necessarily symmetrical. For
for example, Alice may be Bruno's "friend", but friendship is not
reciprocated.

More abstractly, the mechanism monitors the following phenomena:
- A new entity begins to be monitored;
- A monitored entity stops being monitored;
- A new relationship is established between 2 monitored entities;
- A relationship existing between 2 monitored entities ceases to exist.
- Following a specific command, the system returns, for each relationship, the entity that "receives" the most relationships

Some more useful informations:
- Each entity has an identifying name;
- There can be several types of relationships between entities, each identified by a name
- Every relationship has a direction and is not necessarily symmetrical
- The application must be optimized to manage a large number of entities and instances of relationships, but generally few (identifying) types of relationships

## Programming language

Standard **C language** implementation (with **libc only**) of a
program that reads a sequence of commands from standard input,
each corresponding to a change in the entities or in the
relations between entities and, when required, produces on standard output,
for each type of monitored relationship, the identifier of the entity which is the
recipient of the greatest number of instances of that relationship, and the
number of relationships the entity receives

## Commands

The commands that can be read are the following:
- **addent ⟨id_ent⟩**: adds an entity identified by "id_ent" to the set of monitored entities; if the entity is already monitored, it does nothing.
- **delent ⟨id_ent⟩**: deletes the entity identified by "id_ent" from the set of monitored entities; delete all relations of which "id_ent" is a part
(both as origin and as destination).
- **addrel ⟨id_ent⟩ ⟨id_ent⟩ ⟨id_ent⟩**: adds a relationship identified by "id_rel" between the entities "id_orig" and "id_dest", where "id_dest" is the recipient of the
report. If the relationship between "id_orig" and "id_dest" already exists, or if at least one of the entities is not monitored, it does nothing. The
monitoring the "id_rel" relationship type implicitly begins with the first "addrel" command that affects it.
- **delrel ⟨id_ent⟩ ⟨id_ent⟩ ⟨id_ent⟩**: deletes the relationship identified by "id_rel" between the entities "id_orig" and "id_dest" (where "id_dest" is the receiver of the relationship); if there is no relation "id_rel" between "id_orig" and "id_dest" (with "id_dest" as receiver), it does nothing 
- **report**: outputs the list of all types of relationships, reporting for each one of them the corrisponding entity with the largest number of incoming relationships
- **end**: end of the command sequence

## Repository structure

- main.c file contains all the code of the application;
- documents folder contains the specification of the project;
- test folder contains some test files to input to the main.c program;
- old_versions folder contains previous versions of the application;
