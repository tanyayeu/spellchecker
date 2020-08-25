#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXME: implement
    char* word = NULL;
    do {
        word = nextWord(file);
        if(word != NULL) {
            hashMapPut(map, word, 0);
            free(word);
        }
    }while(word != NULL);
}
/**
 * Returns the length of the string
 * @param char s[]
 */
int stringLength(char s[])
{
    int numChars = 0;
    while(s[numChars]!='\0')
    {
        numChars++;
    }
    return numChars;
}
/**
 * Returns each character as a lower case
 * @param char ch
 * @return char 
 */
char toLowerCase(char ch)
{
    if(ch>='A' && ch <='Z')
    {
        ch = 'a' + (ch - 'A');
    }
    return ch;
}
/**
 * Converts a string to all lower case
 * @param char*
 */
void convertToLower(char* word)
{
    for(int i = 0; i<stringLength(word); i++) {
        word[i] = toLowerCase(word[i]);
    }
}

/**
 * Finds the minimum of 3 ints
 * @param int x, y, z
 * @return minimum
 */
int min(int x, int y, int z)
{
    int min;
    min = x;
    if(y < min) {
        min = y;
        if(z < min) {
            min = z;
        }
    }
    if(z < min) {
        min = z;
        if(y < min) {
            min = y;
        }
    }
    return min;
}

/**
 * Finds the smallest difference in the hashmap
 * @param HashMap* map
 * @param int capacity
 * @return min
 */
int smallest(HashMap* map, int capacity)
{
    HashLink* link;
    int min = 10;
    for(int i = 0; i < capacity; i++) {
        link = map->table[i];
        while(link != NULL) {
            if(min > link->value) {
                min = link->value;
            }
            link = link->next;
        }
    }
    return min;
}
/**
 * Finds the minimum of 3 ints
 * @param string1
 * @param string2
 * @param stringlenghth1
 * @param stringlength2
 * @return int with lowest number of operations
 */
int editDistance(char* word, char* dictionaryWord, int strLength1, int strLength2)
{
    //create a table with the length of each word
    int table[strLength1 + 1][strLength2 + 1];
    for(int i = 0; i <= strLength1; i++) {
        for(int x = 0; x <= strLength2; x++) {
            //if first string is empty
            if(i == 0) {
                table[i][x] = x;
            }
            //if second string is empty
            else if(x == 0) {
                table[i][x] = i;
            } //
            //if the letters are the same then it can be ignored
            else if(word[i-1] == dictionaryWord[x-1]) {
                table[i][x] = table[i -1][x -1];
            }
            //if the last character is different
            else {
                table[i][x] = 1 + min(table[i][x-1], table[i-1][x], table[i-1][x-1]);
            }
        }
    }
    return table[strLength1][strLength2];
}

/**
 * Computes the Levenshtein distance for each word in the dictionary compared to input buffer
 * @param hashmap
 * @param string
 */
void putEditValue(HashMap* map, char* word, char** array)
{
    HashLink* link;
    // HashMap* bag = hashMapNew(500); //the bag i shall use to sort these values into
    int x = 0;
    for(int i = 0; i < map->capacity; i++) {
        link = map->table[i];
        while(link != NULL) {
            
            link->value = editDistance(word, link->key,stringLength(word), stringLength(link->key));
            link = link->next;
        }
    }
    int min = smallest(map, map->capacity);
    while(x < 5){
        for(int i = 0; i < map->capacity; i++) {
            link = map->table[i];
            while(link != NULL) {
                if(link->value == min && x < 5){                
                    strcpy(array[x], link->key);                    
                    x++;
                }
                link = link->next;
            }
        }
        for(int i = 0; i < map->capacity; i++) {
            link = map->table[i];
            while(link != NULL) {
                if(link->value == min+1 && x < 5){
                    strcpy(array[x], link->key);
                    x++;
                }
                link = link->next;
            }
        }
    }
}
/**
 * Prints the array of suggestions
 * @param char** array
 */
void printArr(char** array)
{
    for(int i =0; i < 5; i++) {
        // printf("%s\n", array[i]);
        if(array[i] != NULL){
            printf("%s\n", array[i]);
        }
    }
}
/**
 * Checks if it is a letter
 * @param char c
 * @return 1 for true, 0 for false
 */
int isLetter(char c)
{
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        return 1;
    }
    return 0;
}
/**
 * Checks if a string has letters
 * @param char* string
 * @return 1 for true, 0 for false
 */
int hasLetters(char* string)
{
    //does it contain letters. make sure its not just spaces or an enter
    for(int i = 0; i < stringLength(string); i++)
    {
        if(isLetter(string[i]) == 1)
        {
            return 1;
        }
    }
    return 0;
}
/**
 * Checks if a string has numbers
 * @param char* string
 * @return 1 for true, 0 for false
 */
int hasNum(char* word)
{
    for(int i=0; i < stringLength(word); i++)
    {
        if(!isLetter(word[i]))
        {
            return 1;
        }
    }
    return 0;
}
/*
 * ===  FUNCTION  ==========================================================
 *         Name:  wordCount(char*)
 *  Description:  This function counts the number of words in the string
 * =========================================================================
 */
int wordCount(char* word)
{
    int count = 1;
    for(int i = 0; i < stringLength(word); i++)
    {
        if(isLetter(word[i]) == -1 && isLetter(word[i+1]) == 1)
        {
            count++;
        }
    }
    return count;
}
/**
 * Checks if the word is a valid input
 * @param char* word
 * @return 1 for bad, 0 for good
 */
int isBad(char* word)
{
    if(!isLetter(word[0]) || hasNum(word) || wordCount(word) > 1)
    {
        return 1;
    }
    return 0;
}

/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    HashMap* map = hashMapNew(1000);
    char* array[5];
    for(int i = 0; i < 5; i++){
        array[i] = malloc(sizeof(char) * 16);
    }
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    // printf("Has number: %d\n", hasNum(string));
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        // scanf("%s", inputBuffer);
        fgets(inputBuffer, 256, stdin);
        convertToLower(inputBuffer);
        inputBuffer[strcspn(inputBuffer, "\n")]=0;  //removes the trailing \n
        // Implement the spell checker code here..
        //if user enters nothing or some kind of invalid input
        if(isBad(inputBuffer)){
            printf("Invalid input\n");
        }
        else if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
        else if(hashMapContainsKey(map, inputBuffer)) {      //if contains the input buffer
            printf("The inputted word \"%s\" is spelled correctly.\n", inputBuffer);
        }
        else {
            printf("The inputted word \"%s\" is spelled incorrectly...\n", inputBuffer);
            putEditValue(map, inputBuffer, array);
            // printf("Lowest diff: %d\n", smallest(map, map->capacity));
            printf("Did you mean:\n");
            printArr(array);
        }
    }
 
    hashMapDelete(map);
    for(int i = 0; i < 5; i++){
        free(array[i]);
        array[i] = 0;
    }
    map = NULL;
    return 0;
}