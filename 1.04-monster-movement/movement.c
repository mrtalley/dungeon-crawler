#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int genMonsterCode()
{
    int monster = 0000;
    int rando = 0;

    // Intelligent monsters
    rando = rand() % 2;
    if(rando == 1) {
        monster += 1;
    }

    // Telepathic monsters
    rando = rand() % 2;
    if(rando == 1) {
        monster += 10;
    }

    // Tunneling monsters
    rando = rand() % 2;
    if(rando == 1) {
        monster += 100;
    }

    // Erratic monsters
    rando = rand() % 2;
    if(rando == 1) {
        monster += 1000;
    }

    return monster;
}

int genRando()
{
    return rand() % 2;
}

int genMonsterCodeFor()
{
    int monster = 0000;
    int rando = 0, i = 0;
    time_t t;
    srand((unsigned) time(&t));

    for(i = 0; i < 4; i++) {
        rando = rand() % 2;

        // Intelligent monsters
        if(rando == 1 && i == 0) {
            monster += 1;
        }

        // Telepathic monsters
        if(rando == 1 && i == 1) {
            monster += 10;
        }

        // Tunneling monsters
        if(rando == 1 && i == 2) {
            monster += 100;
        }

        // Erratic monsters
        if(rando == 1 && i == 3) {
            monster += 1000;
        }
    }

    return monster;
}

char *intToArray(int intr)
{
    char *array = malloc(4 * sizeof(*array));
    int i = 0;

    do {
        array[i] = intr % 10;
        intr /= 10;
        i++;
    } while(intr != 0);

    return array;
}

void determineMonsterTraits(int monsterCode)
{
    char *code = intToArray(monsterCode);
    int i = 0, hex = 0;;

    // Intelligent
    if(code[0]) {
        hex += 1;
    }

    // Telepathic
    if(code[1]) {
        hex += 2;
    }

    // Tunneling
    if(code[2]) {
        hex += 4;
    }

    // Erratic
    if(code[3]) {
        hex += 8;
    }

    printf("\n HEX: %x\n", hex);
}

int main(void) {
    int i = 0;
    int monsterCode = 0;
    srand((unsigned) time(NULL));



    for(i = 0; i < 10; i++) {
        monsterCode = genMonsterCode();
        determineMonsterTraits(monsterCode);
        printf("CODE: %d\n\n", monsterCode);
    }

    return 0;
}
