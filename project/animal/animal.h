#ifndef OOPC_ANIMAL_H
#define OOPC_ANIMAL_H

typedef struct {
    char name[10];
    char sound[10];
} Animal_Attr;

typedef struct {
    int (*init)(void* t, Animal_Attr attr);
    int (*get_name)(void* t, char* name);
    int (*speak)(void* t);
} ANIMAL_CLASS_IMPLEMENTS;

typedef struct {
    ANIMAL_CLASS_IMPLEMENTS api;
    Animal_Attr attr;
} ANIMAL_CLASS;

typedef struct {
    char name[10];
    char sound[10];
} Dog_Attr;

typedef struct {
    ANIMAL_CLASS_IMPLEMENTS api;
    Dog_Attr attr;
} DOG_CLASS;

typedef struct {
    char name[10];
    char sound[10];
} Cat_Attr;

typedef struct {
    ANIMAL_CLASS_IMPLEMENTS api;
    Cat_Attr attr;
} CAT_CLASS;

ANIMAL_CLASS* ANIMAL_CLASS_CTOR(void);
void ANIMAL_CLASS_DTOR(ANIMAL_CLASS* t);

DOG_CLASS* DOG_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t);
void DOG_CLASS_DTOR(DOG_CLASS* t);

CAT_CLASS* CAT_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t);
void CAT_CLASS_DTOR(CAT_CLASS* t);

#endif
