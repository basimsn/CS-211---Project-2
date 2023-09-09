//  bestWordle.c
//  Author: 
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
//

#include <stdio.h>
#include <stdlib.h>   // for exit( -1)
#include <string.h>

typedef struct{
	int score;
	char word[81];
}wordCountStruct;


char **readWordsInto(char *answersFileName, char *guessesFileName, int *size, int *length){
	FILE *inFilePtr  = fopen(answersFileName, "r");  // Connect logical name to filename
  char inputString[ 81];
	FILE *inFilePtr2  = fopen(guessesFileName, "r");  // Connect logical name to filename
	char inputString2[ 81];
	int i = 0;
	char **lines = (char**)malloc((*size) * sizeof(char*));
	while( fscanf( inFilePtr, "%s", inputString) != EOF) {
			int strLen = strlen(inputString);
			char *str = (char*)malloc((strLen+1)*sizeof(char));
			strcpy(str, inputString);
			lines[i] = str;
			i++;
		}
	while( fscanf( inFilePtr2, "%s", inputString2) != EOF) {
			int strLen = strlen(inputString2);
			char *str = (char*)malloc((strLen+1)*sizeof(char));
			strcpy(str, inputString2);
			lines[i] = str;
			i++;
		}
    *length = i;
    return lines;
    fclose( inFilePtr);
    fclose( inFilePtr2);
}

//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // // Before using parameters we have cast them into the actual type they are in our program
    // // and then extract the numerical value used in comparison
	
    int firstScore = ((wordCountStruct*) a)->score;
    int secondScore = ((wordCountStruct*) b)->score;

    // // If scores are different, then that's all we need for our comparison.
	
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        return strcmp(((wordCountStruct*) a)->word, ((wordCountStruct*) b)->word);
        // Scores are equal, so check words themselves, to put them in alphabetical order
	  // Hint: return the value of strcmp( parameter a word,  parameter b word)
        // return strcmp( ...);
    }
} //end compareFunction(..)


    // ...
    // Sort the allWords array in descending order by score, and within score they should also
    // be sorted into ascending order alphabetically.  Use the built-in C quick sort qsort(...).
    // The last parameter is the name of the comparison function we use (a function pointer).
    //
    // ... uncomment out the line below once you have it in a meaningful context in your code.
    // qsort( theArray, numElementsInArray, sizeof( your struct), compareFunction);

void findBestFirstWord(int *length, int *count, char **words, wordCountStruct info[]){
    char word[81];
    char answerWord[81];
    int wordScore = 0;
    int repeat = 0;
    char rep = ' ';
    for(int i=0; i< (*length); i++){
        wordScore = 0;
        strcpy(word, words[i]);
        for(int j=0; j< (*count); j++){
            strcpy(answerWord, words[j]);
            for(int k = 0; word[k] != '\0'; k++){
                if(answerWord[k] == word[k]){ //3 pointer
                    wordScore += 3;
                    answerWord[k] = '~';
                    char targetChar = word[k];
                    for(int p=0; answerWord[p] != '\0'; p++){
                        if(targetChar == answerWord[p] && word[p] != answerWord[p]){
                            answerWord[p] ='{';
                        }
                    }
                }
            }
            for(int k = 0; word[k] != '\0'; k++){
                for(int l = 0; answerWord[l] != '\0'; l++){
                    if(answerWord[l] == word[k]){//1 pointer
                        repeat++;
                        if(repeat <= 1){     
                            answerWord[l] = '-';
                            wordScore += 1;
                        }
                    }
                }
                repeat = 0;
            }
        }
        strcpy(info[i].word, word);
        info[i].score = wordScore;
    }
}

void secondBestWord(int *length, int *count, char **words2, char **totalWords, wordCountStruct secondBest[], char *bestWord){
    char word[81];
    char allWords[81];
    char answerWord[81];
    int wordScore = 0;
    int repeat = 0;
    char rep = ' ';

    // strcpy(word, bestWord);
    // printf("%s\n", word);
    for(int i=0; i<(*count); i++){
        wordScore = 0;
        strcpy(answerWord, words2[i]);
        for(int k = 0; answerWord[k] != '\0'; k++){
            if(answerWord[k] == word[k]){ //3 pointer
                answerWord[k] = '~';
                word[k] = '_';
                repeat++;
            }
            for(int l = 0; answerWord[l] != '\0'; l++){
                if(answerWord[l] == word[k]){//1 pointer
                    if(repeat <= 1){     
                        answerWord[l] = '-';
                        wordScore += 1;
                    }
                }
            }
            strcpy(word, bestWord);
        }
        repeat = 0;
        strcpy(words2[i], answerWord);
    }
    for(int i = 0; i < *(length); i++){
        repeat = 0;
        wordScore = 0;
        strcpy(allWords, totalWords[i]);
        for(int j=0; j< (*count); j++){
            strcpy(answerWord, words2[j]);
            for(int k = 0; allWords[k] != '\0'; k++){
                if(answerWord[k] == allWords[k]){ //3 pointer
                    wordScore += 3;
                    answerWord[k] = ',';
                    char targetChar = allWords[k];
                    for(int p=0; answerWord[p] != '\0'; p++){
                        if(targetChar == answerWord[p] && allWords[p] != answerWord[p]){
                            answerWord[p] ='{';
                        }
                    }
                }
            }
            // printf("%s %s %d\n ",answerWord, allWords, wordScore);
            for(int k = 0; allWords[k] != '\0'; k++){
                for(int l = 0; answerWord[l] != '\0'; l++){
                    if(answerWord[l] == allWords[k]){//1 pointer
                        repeat++;
                        if(repeat <= 1){     
                            answerWord[l] = '.';
                            wordScore += 1;
                        }
                    }
                }
                repeat = 0;
            }
        }
        strcpy(secondBest[i].word, allWords);
        secondBest[i].score = wordScore;
        // printf("%s ", secondBest[i].word);
        // printf("%d \n", secondBest[i].score);
    }
}


// -----------------------------------------------------------------------------------------
int main() {
    char answersFileName[81];  // Stores the answers file name
    char guessesFileName[81];  // Stores the guesses file name
    // Set default file names
		strcpy(answersFileName, "answersTiny.txt");
		strcpy(guessesFileName, "guessesTiny.txt");
		int count = 0;
		int count2 = 0;
		// wordCountStruct input;

    // ...

    printf("Default file names are %s and %s\n", answersFileName, guessesFileName);

    // Display menu, to allow partial credit for different program components
    int menuOption = 0;
    do {
        printf("\n");
        printf("Menu Options:\n");
        printf("  1. Display best first words only\n");
        printf("  2. Display best first and best second words\n");
        printf("  3. Change answers and guesses filenames\n");
        printf("  4. Exit\n");
        printf("Your choice: ");
        scanf("%d", &menuOption);

        // Handle menu option 3 to exit the program
        if (menuOption == 4) {
            exit(1); // Exit the program
        } else if (menuOption == 3) {
            // Change file names.  Menu will then be redisplayed.
            printf("Enter new answers and guesses filenames: ");
            scanf("%s %s", answersFileName, guessesFileName);

        }
    } while( menuOption == 3);

    // Sample code to read in from a file
    FILE *inFilePtr  = fopen(answersFileName, "r");  // Connect logical name to filename
    char inputString[ 81];

	FILE *inFilePtr2  = fopen(guessesFileName, "r");  // Connect logical name to filename
    char inputString2[ 81];
    // Sanity check: ensure file open worked correctly
    if( inFilePtr == NULL ) {
        printf("Error: could not open %s for reading\n", answersFileName);
        exit(-1);    // must include stdlib.h
    }

    // Read each word from file and print it.  You could do other things along the
    // way, like counting how many words there are.
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
		count++;
        // printf("%s\n", inputString);
    }
    while( fscanf( inFilePtr2, "%s", inputString2) != EOF) {
    count2++;
            // printf("%s\n", inputString2);
    }
    int size = count + count2;
    fclose( inFilePtr);
    fclose( inFilePtr2);
    int length = 0;
    char bestWord[81];
    char **words = readWordsInto(answersFileName, guessesFileName, &size, &length);
    char **words2 = readWordsInto(answersFileName, guessesFileName, &size, &length);
    char **totalWords = readWordsInto(answersFileName, guessesFileName, &size, &length);
    char **totalWords2 = readWordsInto(answersFileName, guessesFileName, &size, &length);
    wordCountStruct info[length];
    if(menuOption == 1){
        findBestFirstWord(&length, &count, words, info);
        int sizeForStruct = sizeof(info)/sizeof(*info);
        qsort(info, sizeForStruct, sizeof(wordCountStruct), compareFunction);
        // for(int i = 0; i < sizeForStruct; i++ ){ 
        //     printf("%s %d \n", info[i].word, info[i].score);
        // }
        printf("%s has %d words\n", answersFileName, count);
        printf("%s has %d words\n\n", guessesFileName, count2);
        printf("Words and scores for top first words:\n");
        printf("%s %d \n", info[0].word, info[0].score);
        if(info[0].score == info[1].score){
            printf("%s %d \n", info[1].word, info[1].score);
        }
    }
    wordCountStruct secondBest[length];
    wordCountStruct thirdBest[length];
    if(menuOption == 2){
        findBestFirstWord(&length, &count, words, info);
        int sizeForStruct = sizeof(info)/sizeof(*info);
        qsort(info, sizeForStruct, sizeof(wordCountStruct), compareFunction);
        strcpy(bestWord, info[0].word);
        printf("%s has %d words\n", answersFileName, count);
        printf("%s has %d words\n\n", guessesFileName, count2);
        printf("Words and scores for top first words and second words:\n");
        secondBestWord(&length, &count, words, totalWords, secondBest, bestWord);

        int sizeForStruct2 = sizeof(secondBest)/sizeof(*secondBest);
        qsort(secondBest, sizeForStruct2, sizeof(wordCountStruct), compareFunction);

        // for(int i = 0; i < sizeForStruct2; i++ ){ 
        //     printf("%s %d \n", secondBest[i].word, secondBest[i].score);
        // }
        
        // printf("Top scoring words:\n");
        printf("%s %d \n", info[0].word, info[0].score);
        printf("  %s %d \n", secondBest[0].word, secondBest[0].score);
      //SECOND------------------------------------------------------
        if(info[0].score == info[1].score){
            findBestFirstWord(&length, &count, totalWords, info);
            int sizeForStruct3 = sizeof(info)/sizeof(*info);
            qsort(info, sizeForStruct3, sizeof(wordCountStruct), compareFunction);
            strcpy(bestWord, info[1].word);
            // printf("\n%s has %d words\n", answersFileName, count);
            // printf("%s has %d words\n", guessesFileName, count2);
            // printf("Words and scores for top first words and second words:\n");
            // printf("answerWordsCopy after letters from %s removed:\n", bestWord);
            secondBestWord(&length, &count, totalWords, totalWords2, thirdBest, bestWord);

            int sizeForStruct4 = sizeof(thirdBest)/sizeof(*thirdBest);
            qsort(thirdBest, sizeForStruct4, sizeof(wordCountStruct), compareFunction);

            // for(int i = 0; i < sizeForStruct4; i++ ){ 
            //     printf("%s %d \n", thirdBest[i].word, thirdBest[i].score);
            // }

            // printf("Top scoring words:\n");
            printf("%s %d \n", info[1].word, info[1].score);
            printf("  %s %d \n", thirdBest[0].word, thirdBest[0].score);
        }
    }
    printf("Done\n");
    return 0;
} // end main()
