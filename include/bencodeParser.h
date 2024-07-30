#ifndef __HASHCONPARSER
#define __HASHCONPARSER
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef unsigned long int lint_8;
typedef unsigned char Uchar;
#define BUFFER 1024
#define MALLOCALLOCATIONERROR(line, ...) {printf("In file %s, line %i, memory allocation filed: %s",__FILE__, line, __VA_ARGS__);}
#define UNRECOGNIZEDCHARACTER(line, ...) {printf("In file %s, line %i, given character not recognized: %s", __FILE__, line, __VA_ARGS__);}

#define UNRECOGNIZEDDATATYPE(line, ...) {printf("In file %s, line %i, given data type could not be founud: %s", __FILE__, line, __VA_ARGS__);}

#define BENCODESTRING 0x20
#define BENCODEINTEGER 0x69
#define BENCODELIST 0x6C
#define BENCODEDICTIONARY 0x64

char BYTE_STRING = BENCODESTRING;
char INTEGER = BENCODEINTEGER;
char LIST = BENCODELIST;
char DICTIONARY = BENCODEDICTIONARY;

typedef struct B bencodeParser;

typedef struct B{
	char dataType[100]; //assuming dataType will not surpass 100 characters
	union{
		lint_8 integerValue;
		Uchar* stringValue;
		bencodeParser* List_Dictionary;
	}value;
	bencodeParser* next;
}bencodeParser;

static lint_8 lexer(const Uchar* stream);

static void readBencode(bencodeParser* Head);

static void memoryAllocationFailureCallback(void (*exitProgram)(int), int errorCode, char* callbackMessage, int line);

static void unrecognizedCharacterCallback(void (*exitProgram)(int), int errorCode, char* callbackMessage, int line);

static void unrecognizedDataTypeCallback(void (*exitProgram)(int), int errorrCode, char* callbackMessage, int line);

static void FAILURE_EXIT(int errorCode);

static int stringToInteger(Uchar* string);

static int compare(const char* string);

bencodeParser* Head = NULL;
bencodeParser* Tail = NULL;

static void FAILURE_EXIT(int errorCode){
	exit(errorCode);
}

static void memoryAllocationFailureCallback(void (*exitProgram)(int), int errorCode, char* callbackMessage, int line){
	MALLOCALLOCATIONERROR(line,callbackMessage);
	(*exitProgram)(errorCode);
}

static void unrecognizedCharacterCallback(void (*exitProgram)(int), int errorCode, char* callbackMessage, int line){
	UNRECOGNIZEDCHARACTER(line,callbackMessage);
	(*exitProgram)(errorCode);
}

static void unrecognizedDataTypeCallback(void (*exitProgram)(int), int errorCode, char* callbackMessage, int line){
	UNRECOGNIZEDDATATYPE(line,callbackMessage);
	(*exitProgram)(errorCode);
}

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
	if(strcmp(string,"Integer") == 0) return 2;
	if((strcmp(string,"List") == 0) || (strcmp(string,"Dictionary") == 0)) return 3;
	return -1; 
}

static void readBencode(bencodeParser* Head){
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
			char buffer[BUFFER];
			sprintf(buffer,"Unrecoginzed Datatype: %s",Head->dataType);
			unrecognizedDataTypeCallback(&FAILURE_EXIT, -1, (char *) (buffer), __LINE__);
		}
	}
}

static lint_8 lexer(const Uchar* stream){
	
	lint_8 pointer = 0;
	lint_8 pointer2 = 0;

	Uchar *lexerResult = (Uchar *) malloc(sizeof(bencodeParser));
	if(!lexerResult){
		memoryAllocationFailureCallback(&FAILURE_EXIT, -1, (char *) "Variable lexerResult could not be allocated\n", __LINE__);
	}

	bencodeParser* element = (bencodeParser*) malloc(sizeof(bencodeParser));
	if(!element){
		memoryAllocationFailureCallback(&FAILURE_EXIT, -1, (char *) "Variable element could not be allocated\n", __LINE__);
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
					memoryAllocationFailureCallback(&FAILURE_EXIT, -1, (char *) "Variable lexerResult could not be allocated\n",__LINE__);
				}
			}
			lexerResult[pointer2++] = stream[pointer++];
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
	else{//(stream[pointer] == BYTE_STRING){
		Uchar num[1000] = {0};
		memset(num,0,sizeof(num));
		int tempPoint = 0;
		while(stream[pointer] != 0x3A){
			num[tempPoint++] = stream[pointer++];
		}
		num[tempPoint] = '\0';
		int lengthOfString = stringToInteger(num);
		pointer++;
		element->value.stringValue = (Uchar *) malloc((lengthOfString + 1) * sizeof(char));
		if(element->value.stringValue == NULL){
			memoryAllocationFailureCallback(&FAILURE_EXIT, -1, (char *) "Variable lexerResult could not be allocated\n", __LINE__);
		}
		memcpy(element->value.stringValue, &stream[pointer], lengthOfString);
		element->value.stringValue[lengthOfString] = '\0';
		pointer+=lengthOfString;
	}
	/*else{
		char buffer[BUFFER];
		sprintf(buffer,"Unrecoginzed character: %d\n",stream[pointer]);
		unrecognizedCharacterCallback(&FAILURE_EXIT, -1, (char *) (buffer),__LINE__);
	}*/
	
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
