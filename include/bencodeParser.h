#ifndef __HASHCONPARSER
#define __HASHCONPARSER
#include<stdio.h>
#include<stdlib.h>
typedef unsigned long int lint_8;
typedef unsigned char Uchar;
#define BUFFER 1024
#define MALLOCALLOCATIONERROR(...) {printf("In file %s, line %i, memory allocation filed: %s",__FILE__, __LINE__, __VA_ARGS__);}
#define UNRECOGNIZEDCHARACTER(...) {printf("In file %s, line %i, given character not recognized: %s", __FILE__, __LINE__, __VA_ARGS__);}

#define UNRECOGNIZEDDATATYPE(...) {printf("In file %s, line %i, given data type could not be founud: %s", __FILE__, __LINE__, __VA_ARGS__);}

typedef struct Bencoder;

typedef enum : char{BYTE_STRING = 0x20,INTEGER = 0x69,LIST = 0x6C,DICTIONARY = 0x64}Bencoder;

typedef struct B bencodeParser;

typedef struct B{
	char dataType[100]; //assuming dataType will not surpass 100 characters
	union{
		lint_8 integerValue;
		Uchar* stringValue;
		bencodeParser* List_Dictionary;
	}
	bencodeParser* next;
}bencodeParser;

static lint_8 lexer(const Uchar* stream);

static void readBencode(bencodeParser* Head);

static void memoryAllocationFailureCallback(void (*exitProgram)(int), int errorCode, char* callbackMessage);

static void unrecognizedCharacterCallback(void (*exitProgram)(int), int errorCode, char* callbackMessage);

static void unrecognizedDataTypeCallback(void (*exitProgram)(int), int errorrCode, char* callbackMessage);

static void FAILURE_EXIT(int errorCode);

static int stringToInteger(Uchar* string);

static int compare(const char* string);

static int stringToInteger(Uchar* string){
	int res = 0;
	register int i = 0;
	for(;string[i] != '\0'; i++){
		res = (res * 10) + (string[i] - '0');
	}
	return res;
}

static int compare(const char* string){
	if(strcmp(string,"String") == 0) return 1;
	if(strcmp(string."Integer") == 0) return 2
	if((strcmp(string,"List") == 0) || (strcmp(string,"Dictionary") == 0)) return 3;
	return -1; 
}

static void readBencode(bencode* Head){
	while(Head){
		printf("\n%s\n",Head->dataType);
		if(compare(Head->dataType) == 1){
			printf("%s\n",Head->value.stringValue);
		}
		else if(compare(Head->dataType) == 2){
			printf("%li\n",Head->value.integerValue);
		}
		else if(compare(Head->dataType) == 3 || compare(Head->dataType) == 3){
		readBencode(Head->value.List_Dictionary);
		}
		else{
			unrecognizedDataTypeCallback(&FAIURE_EXIT, -1, (char *) ("Unrecoginzed character %s ",stream[pointer]));
		}
}

bencodeParser* Head = NULL;
bencodeParserr* Tail = NULL;
static void FAILURE_EXIT(int errorCode){
	exit(errorCode);
}

static void memoryAllocationFailureCallbac(void (*exitProgram)(int),int errorCode, char* callbackMessage){
	MALLOCALLOCATIONERROR(callbackMessage);
	(*exitProgram)(errorCodde);
}


static lint_8 lexer(const Uchar* stream){
	
	lint_8 pointer = 0;
	lint_8 pointer2 = 0;

	Uchar *lexerResult = (Uchar *) malloc(sizeof(bencodeParser));
	if(!lexerResult){
		memoryAllocationFailureCallback(&FAIURE_EXIT, -1, (char *) "Variable lexerResult could not be allocated");
	}

	bencodeParser* element = (bencodeParser*) malloc(sizeof(bencodeParser));
	if(!element){
		memoryAllocationFailureCallback(&FAIURE_EXIT, -1, (char *) "Variable element could not be allocated");
	}

	element->next = NULL;
	if(stream[pointer] == INTEGER){
		strcpy(element->dataType,"Integer");
		pointer++;
		while(stream[pointer] != 0x65){
			lint_8 currentBufferSize = BUFFER;
			if((pointer2) >= currentBufferSize){
				currentBufferSize+=BUFFER;
				lexerResult = (Uchar *) realloc(lexerResult,currentBufferSize);
				if(!lexerResult){
					memoryAllocationFailureCallback(&FAIURE_EXIT, -1, (char *) "Variable lexerResult could not be allocated");
				}
			}
			lexerResult[pointer++] = stream[pointer++];
		}
		lexerResult[pointer2] = '\0';
		element->value.integerValue = stringToInteger(lexerResult);
		pointer++;
	}
	else if(stream[pointer] == LIST){

		strcpy(element->dataType,"List");
		pointer++;
		while(stream[pointer] != 0x65){
			pointer+=lexer(stream + pointer);
		}
		pointer++;
	}
	else if(stream[pointer] == DICTIONARY){
		strcpy(element->dataType,"Dictionary");
		pointer++;
		while(stream[pointer] != 0x65){
			pointer+=lexer(stream + pointer);
		}
		pointer++;
	}
	else if(stream[pointer] == BYTE_STRING){
		Uchar num[1000] = {0};
		memset(num,0,sizeof(num));
		int tempPoint = 0;
		while(stream[pointer] != 0x3A){
			num[tempPointer++] = stream[pointer++];
		}
		num[tempPoint] = '\0';
		int lengthOfString = stringToInteger(num);
		pointer++;
		element->value.stringValue = (Uchar *) malloc((lengthOfString + 1) * sizeof(char));
		if(element->value.stringValue == NULL){
			memoryAllocationFailureCallback(&FAIURE_EXIT, -1, (char *) "Variable lexerResult could not be allocated");
		}
		memcpy(element->value.stringValue, &stream[pointer], lengthOfString);
		element->value.stringValue[lengthOfString] = '\0';
		pointer+=lengthOfString;
	}
	else{
		unrecognizedCharacterCallback(&FAIURE_EXIT, -1, (char *) ("Unrecoginzed character %s ",stream[pointer]));
	}
	
	if(Head == NULL){
		Head = element;
		Tail = element;
	}
	else{
		Tail->next = element;
		Tail = element;
	}

	free(lexerResult);
	return pointer;

}

#endif
