#include<stdio.h>
#include<stdlib.h>
#include<string.h>

# define LINESIZE 100
# define WORDSIZE 50
# define WORDNEWSIZE 10

struct listNode {
	int lineNum;
	struct listNode* next;
};
typedef struct listNode ListNode;
typedef ListNode* ListNodePtr;

struct identifier {
	char name[11];
	int count;
	ListNodePtr* sPtr;
};
typedef struct identifier Identifier;

int checkSuffix(char* word, char suf[2]);
int checkPre(char* word, char pre[2]);
int find(ListNodePtr* sPtr, int val);
void insertion(ListNodePtr* sPtr, int lineN);
int search(Identifier identifiers[], int identNum, int lineNum, char* name);
void get_table(FILE* finPtr, FILE* foutPtr);
void print_source(FILE* finPtr, FILE* foutPtr);

int main(int argc, char* argv[]) {
	char* flag = argv[1];
	char sour_Mal[] = "-1";
	char table[] = "-c";
	char both[] = "-b";

	if (argc != 4) {
		fprintf(stderr, "Usage: flag, inputfile, outputfile");
		exit(1);
	}

	if (strcmp(flag, sour_Mal) != 0 && (strcmp(flag, table) != 0)
			&& (strcmp(flag, both) != 0)) {
		fprintf(stderr, "Invalid flag: %s", flag);
		exit(1);
	}

	char* infile = argv[2];
	char* outfile = argv[3];
	FILE* finPtr;
	FILE* fin;
	FILE* foutPtr;

	if ((finPtr = fopen(infile, "r")) == NULL) {
		fprintf(stderr, "Could not open mal_prog.txt file for reading\n");
		exit(1);
	}
	if ((fin = fopen(infile, "r")) == NULL) {
		fprintf(stderr, "Could not open mal_prog.txt file for reading\n");
		exit(1);
	}

	if ((foutPtr = fopen(outfile, "w")) == NULL) {
		fprintf(stderr, "Could not open output.txt file for writing\n");
		exit(1);
	}

	if (strcmp(flag, sour_Mal) == 0) {
		print_source(finPtr, foutPtr);
	} else if (strcmp(flag, table) == 0) {
		get_table(finPtr, foutPtr);
	} else if (strcmp(flag, both) == 0) {
		print_source(finPtr, foutPtr);
		get_table(fin, foutPtr);
	} else {
		fprintf(stderr, "Invalid flag: %s", flag);
		exit(1);
	}

	if (fclose(finPtr) == EOF) {
		fprintf(stderr, "Can't close input file: %s", infile);
		exit(1);
	}
	if (fclose(fin) == EOF) {
		fprintf(stderr, "Can't close input file: %s", infile);
		exit(1);
	}
	if (fclose(foutPtr) == EOF) {
		fprintf(stderr, "Can't close output file: %s", outfile);
		exit(1);
	}
	return 0;
} // end if main

int checkSuffix(char* word, char suf[2]) {
	int n1 = strlen(suf);
	int n2 = strlen(word);
	if (word != NULL && (strncmp(suf, (word + n2 - n1), n1) == 0)) {
		return 1;
	}
	return 0;
}

int checkPre(char* word, char pre[2]) {
	int n = strlen(pre);
	if (strncmp(pre, word, n) == 0) {
		return 1;
	}
	return 0;
}

int find(ListNodePtr* sPtr, int val) {
	ListNodePtr previousPtr;
	ListNodePtr currentPtr;

	previousPtr = NULL;
	currentPtr = *sPtr;

	while (currentPtr != NULL) {
		if (currentPtr->lineNum == val) {
			return 1;
		}
		previousPtr = currentPtr;
		currentPtr = currentPtr->next;
	}
	return 0;
}

void insertion(ListNodePtr* sPtr, int lineN) {
	ListNodePtr newPtr;
	ListNodePtr previousPtr;
	ListNodePtr currentPtr;

	previousPtr = NULL;
	currentPtr = *sPtr;

	if ((newPtr = malloc(sizeof(ListNode))) == NULL) {
		printf("%s", "The allocation of newPtr has failed");
		fflush(stdout);
		exit(1);
	}
	newPtr->lineNum = lineN;

	while (currentPtr != NULL) {
		previousPtr = currentPtr;
		currentPtr = currentPtr->next;
	}
	if (!find(sPtr, newPtr->lineNum)) {
		if (previousPtr == NULL) {
			newPtr->next = *sPtr;
			*sPtr = newPtr;

		} else {
			previousPtr->next = newPtr;
			newPtr->next = currentPtr;
		}
	}
	return;
}

int search(Identifier identifiers[], int identNum, int lineNum, char* name) {
	int i;
	for (i = 0; i < identNum; i++) {
		if (strcmp(identifiers[i].name, name) == 0) {

			insertion(identifiers[i].sPtr, lineNum);
			return 1;
		}
	}
	return 0;
}

void print_source(FILE* finPtr, FILE* foutPtr) {
	char cha_original[LINESIZE];
	int lineNum, found;
	lineNum = found = 0;
	while (fgets(cha_original, LINESIZE, finPtr) != NULL) {
		if (cha_original[0] != '\n') {
			lineNum++;
			fprintf(foutPtr, "%-3d", lineNum);
			fprintf(foutPtr, "%s", cha_original);
		} else {
			fprintf(foutPtr, "%s", cha_original);
		}

	}
	fprintf(foutPtr, "\n\n");
	return;
}

void get_table(FILE* finPtr, FILE* foutPtr) {
	Identifier identifiers[100];
	char cha_original[LINESIZE];
	char* tokenPtr;
	char* words[WORDSIZE];
	char* wordsNew[WORDNEWSIZE];
	int i, n, lineNum, identNum, wordNum, wordNumNew;
	int wordNum_all;
	char prefix[] = "#";
	char suffix[] = ":";
	char prefix1[] = "$";
	ListNodePtr sPtr[100];

	i = 0;
	n = 0;
	lineNum = 0;
	wordNumNew = 0;
	identNum = 0;

	for (i = 0; i < 100; i++) {
		sPtr[i] = NULL;
		identifiers[i].sPtr = &sPtr[i];
	}

	while (fgets(cha_original, LINESIZE, finPtr) != NULL) {
		wordNum_all = 0;
		tokenPtr = strtok(cha_original, " ");
		while (tokenPtr != NULL) {
			words[wordNum_all] = tokenPtr;
			tokenPtr = strtok(NULL, " ");
			wordNum_all++;
		}

		if ('\n' == words[0][0]) {
			continue;
		} else {
			++lineNum;
		}

		wordNum = 0;
		for (i = 0; i < wordNum_all; ++i) {
			if (checkPre(words[i], prefix)) {
				break;
			} else {
				wordNum++;
			}
		}

		for (i = 0; i < 1; i++) {
			if (checkSuffix(words[i], suffix)
					&& (!checkPre(words[i], prefix))) {
				if (!search(identifiers, identNum, lineNum, words[i])) {
					int n1 = strlen(words[i]);
					strncpy(identifiers[identNum].name, words[i], n1 - 1);
					identifiers[identNum].name[n1 - 1] = '\0';
					identifiers[identNum].count = lineNum;
					identNum++;
				}
			}
		}

		wordNumNew = 0;
		for (i = 1; i < wordNum; i++) {
			tokenPtr = strtok(words[i], ",");
			if (tokenPtr == NULL)
				continue;

			while (tokenPtr != NULL) {
				wordsNew[wordNumNew] = tokenPtr;
				if ('\n'
						== wordsNew[wordNumNew][strlen(wordsNew[wordNumNew]) - 1]) {
					wordsNew[wordNumNew][strlen(wordsNew[wordNumNew]) - 1] =
							'\0';
				}
				tokenPtr = strtok(NULL, ",");
				wordNumNew++;
			}
		}

		if (wordNumNew != 0) {
			for (i = 0; i < wordNumNew; i++) {
				if ((!checkPre(wordsNew[i], prefix1))
						&& (!checkPre(wordsNew[i], prefix))) {
					search(identifiers, identNum, lineNum, wordsNew[i]);
				}
			}
		}
	}

	fprintf(foutPtr, "%s\n\n", "Cross Reference Table");
	fprintf(foutPtr, "  %-15s%-15s%-20s\n", "Identifier", "Definition", "Use");

	for (i = 0; i < identNum; i++) {

		fprintf(foutPtr, "  %-15s%-15d", identifiers[i].name,
				identifiers[i].count);

		ListNodePtr previousPtr;
		ListNodePtr currentPtr;
		previousPtr = NULL;
		currentPtr = *identifiers[i].sPtr;
		while (currentPtr != NULL) {
			fprintf(foutPtr, "%-4d", currentPtr->lineNum);
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}

		fprintf(foutPtr, "\n");

	}
	return;
}

