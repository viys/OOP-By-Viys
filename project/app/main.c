#include <stdio.h>
#include <string.h>
#include "animal.h"

ANIMAL_CLASS_IMPLEMENTS* animal_cat = NULL;
ANIMAL_CLASS_IMPLEMENTS* animal_dog = NULL;

int main(void)
{
    char name[10] = {0};
    ANIMAL_CLASS_IMPLEMENTS* dog = NULL;
    ANIMAL_CLASS_IMPLEMENTS* cat = NULL;

    Animal_Attr Cat_Attr = {
        .name = "cat",
        .sound = "Meow",
    };

    Animal_Attr Dog_Attr = {
        .name = "dog",
        .sound = "Woof",
    };

    printf(">\n");

    /* 抽象：使用统一的 Animal 接口创建并初始化对象。 */
    animal_cat = (ANIMAL_CLASS_IMPLEMENTS*)ANIMAL_CLASS_CTOR();
    animal_dog = (ANIMAL_CLASS_IMPLEMENTS*)ANIMAL_CLASS_CTOR();
    if (animal_cat == NULL || animal_dog == NULL) {
        ANIMAL_CLASS_DTOR((ANIMAL_CLASS*)animal_cat);
        ANIMAL_CLASS_DTOR((ANIMAL_CLASS*)animal_dog);
        return 1;
    }

    animal_cat->init(animal_cat, Cat_Attr);
    animal_dog->init(animal_dog, Dog_Attr);

    /* 封装：通过 get_name 获取名称，不直接访问对象内部属性。 */
    memset(name, 0x00, sizeof(name));
    animal_cat->get_name(animal_cat, name);
    printf("animal name is %s\n", name);

    memset(name, 0x00, sizeof(name));
    animal_dog->get_name(animal_dog, name);
    printf("animal name is %s\n", name);

    /* 继承：Cat 和 Dog 复用 Animal 接口，并重写 speak。 */
    cat = (ANIMAL_CLASS_IMPLEMENTS*)CAT_CLASS_CTOR(animal_cat);
    dog = (ANIMAL_CLASS_IMPLEMENTS*)DOG_CLASS_CTOR(animal_dog);
    if (cat == NULL || dog == NULL) {
        CAT_CLASS_DTOR((CAT_CLASS*)cat);
        DOG_CLASS_DTOR((DOG_CLASS*)dog);
        ANIMAL_CLASS_DTOR((ANIMAL_CLASS*)animal_cat);
        ANIMAL_CLASS_DTOR((ANIMAL_CLASS*)animal_dog);
        return 1;
    }

    cat->speak(cat);
    dog->speak(dog);

    /* 多态：继承同一个 sound 方法，实际调用各自重写的 speak。 */
    cat->sound(cat);
    dog->sound(dog);

    CAT_CLASS_DTOR((CAT_CLASS*)cat);
    DOG_CLASS_DTOR((DOG_CLASS*)dog);
    ANIMAL_CLASS_DTOR((ANIMAL_CLASS*)animal_cat);
    ANIMAL_CLASS_DTOR((ANIMAL_CLASS*)animal_dog);

    printf("<\n");

    return 0;
}
