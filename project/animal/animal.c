#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"

static int animal_init(void* t, Animal_Attr attr) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)t;

    if (this == NULL) {
        return -1;
    }

    memcpy(&this->attr, &attr, sizeof(Animal_Attr));

    return 0;
}

static int animal_get_name(void* t, char* name) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)t;

    if (this == NULL || name == NULL) {
        return -1;
    }

    memcpy(name, this->attr.name, strlen(this->attr.name) + 1);
    return 0;
}

static int animal_speak(void* t) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)t;

    if (this == NULL) {
        return -1;
    }

    printf("animal %s say: %s!\n", this->attr.name, this->attr.sound);

    return 0;
}

static int animal_sound(void* t) {
    ANIMAL_CLASS_IMPLEMENTS* this = (ANIMAL_CLASS_IMPLEMENTS*)t;

    if (this == NULL || this->speak == NULL) {
        return -1;
    }

    /*
     * sound 是 Animal 提供的公共方法。
     * speak 可由子类重写，因此这里会自动调用实际对象的发声实现。
     */
    return this->speak(this);
}

ANIMAL_CLASS* ANIMAL_CLASS_CTOR(void) {
    ANIMAL_CLASS* this = (ANIMAL_CLASS*)malloc(sizeof(ANIMAL_CLASS));

    if (this == NULL) {
        return NULL;
    }

    this->api.init = animal_init;
    this->api.get_name = animal_get_name;
    this->api.speak = animal_speak;
    this->api.sound = animal_sound;

    return this;
}

void ANIMAL_CLASS_DTOR(ANIMAL_CLASS* t) {
    free(t);
}

static int dog_speak(void* t) {
    DOG_CLASS* this = (DOG_CLASS*)t;

    if (this == NULL) {
        return -1;
    }

    printf("animal %s say: Woof!\n", this->attr.name);

    return 0;
}

DOG_CLASS* DOG_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t) {
    DOG_CLASS* this = (DOG_CLASS*)malloc(sizeof(DOG_CLASS));
    ANIMAL_CLASS* parent = (ANIMAL_CLASS*)t;

    if (this == NULL || t == NULL) {
        free(this);
        return NULL;
    }

    /* 先复用 Animal 的接口和属性，再重写 Dog 自己的 speak。 */
    memcpy(&this->api, t, sizeof(ANIMAL_CLASS_IMPLEMENTS));
    memcpy(&this->attr, &parent->attr, sizeof(Dog_Attr));
    this->api.speak = dog_speak;

    return this;
}

void DOG_CLASS_DTOR(DOG_CLASS* t) {
    free(t);
}

static int cat_speak(void* t) {
    CAT_CLASS* this = (CAT_CLASS*)t;

    if (this == NULL) {
        return -1;
    }

    printf("animal %s say: Meow!\n", this->attr.name);

    return 0;
}

CAT_CLASS* CAT_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t) {
    CAT_CLASS* this = (CAT_CLASS*)malloc(sizeof(CAT_CLASS));
    ANIMAL_CLASS* parent = (ANIMAL_CLASS*)t;

    if (this == NULL || t == NULL) {
        free(this);
        return NULL;
    }

    /* 先复用 Animal 的接口和属性，再重写 Cat 自己的 speak。 */
    memcpy(&this->api, t, sizeof(ANIMAL_CLASS_IMPLEMENTS));
    memcpy(&this->attr, &parent->attr, sizeof(Cat_Attr));
    this->api.speak = cat_speak;

    return this;
}

void CAT_CLASS_DTOR(CAT_CLASS* t) {
    free(t);
}
