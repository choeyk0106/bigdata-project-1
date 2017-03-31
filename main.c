#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS

int iUnigramCount[65536];

void wordNGramAccess(int n, char *searchWord) {

	FILE *inFile = NULL;

	int state = 0;
	int wordFreq = 0;
	char word[1024];

	if (n == 1)
		inFile = fopen("word_uni_freq.txt", "r");
	else if (n == 2)
		inFile = fopen("word_bi_freq.txt", "r");
	else if (n == 3)
		inFile = fopen("word_tri_freq.txt", "r");
	else if (n == 4)
		inFile = fopen("word_4_freq.txt", "r");
	else if (n == 5)
		inFile = fopen("word_5_freq.txt", "r");
	else {
		printf("wrong searching\n");
		return 0;
	}

	if (inFile == NULL) {
		printf("File is not open..\n");
		return 0;
	}
	else {
		while (!feof(inFile)) {
			memset(word, 0, sizeof(word));
			fscanf(inFile, "%d %s", &wordFreq, word);
			if (strcmp(word, searchWord) == 0) {
				state = 1;
				printf("\n                 %d 번 출현\n\n", wordFreq);
				break;
			}
		}
		if (state == 0)
			printf("해당 단어를 찾지 못하였습니다.\n");
	}
	fclose(inFile);
}


void nGramAccess(int n, char *searchWord) {
	
	FILE *inFile = NULL;

	int state = 0;
	int wordFreq = 0;
	char word[1024];
	
	if(n==1)
		inFile = fopen("unigram_freq.txt", "r");
	else if (n == 2)
		inFile = fopen("bigram_freq.txt", "r");
	else if (n == 3)
		inFile = fopen("trigram_freq.txt", "r");
	else if (n == 4)
		inFile = fopen("4gram_freq.txt", "r");
	else if (n == 5)
		inFile = fopen("5gram_freq.txt", "r");
	else{
		printf("wrong searching\n");
		return 0;
	}

	if (inFile == NULL) {
		printf("File is not open..\n");
		return 0;
	}
	else {
		while (!feof(inFile)) {
			memset(word, 0, sizeof(word));
			fscanf(inFile, "%d %s", &wordFreq, word);
			if (strcmp(word, searchWord) == 0) {
				state = 1;
				printf("\n                 %d 번 출현\n\n", wordFreq);
				break;
			}
		}
		if(state == 0)
			printf("해당 단어를 찾지 못하였습니다.\n");
	}
	fclose(inFile);
}

void findUnigramCount() {
	int i = 0;
	int word;
	FILE *fp = fopen("input.txt", "r");
	FILE *outFile = fopen("unigram_freq.txt", "w");

	if (fp == NULL)
	{
		printf("File is not open..\n");
		return 0;
	}
	else
	{
		while (!feof(fp)) {
			word = fgetc(fp);

			if (word & 0x80)
			{
				int temp = word;
				word = fgetc(fp);
				//printf("%c%c", temp, word);
				iUnigramCount[temp << 8 | word]++;
			}
			else {
				//printf("%c", word);
				iUnigramCount[word]++;
			}
		}

		for (i = 0; i < 65536; i++) {
			if (iUnigramCount[i] != 0) {		//korea
				if (0xb0a1 <= i && i <= 0xc8fe) {
					fprintf(outFile, "%d %c%c\n", iUnigramCount[i], (i & 0xff00) >> 8, i & 0xff);
				}
				else {									//english
					if (i == 10 || i == 13 || i == 3338 || i == 2573)
						fprintf(outFile, "\n");
					else if (i == 32)
						printf(outFile, "%d _\n", iUnigramCount[i], i & 0xff);
					else 
						fprintf(outFile, "%d %c\n", iUnigramCount[i], i & 0xff);
				}
			}
		}
	}
	fclose(fp);
	fclose(outFile);
}

void findNGram(int n, char* fileName) {
	int i = 0;
	int word, temp; 
	unsigned int range;
	FILE *inFile = fopen("input.txt", "r");
	FILE *outFile = fopen(fileName, "w");

	if (inFile == NULL)
	{
		printf("File is not open..\n");
		return 0;
	}
	else
	{
		while (!feof(inFile)) {
			for (i = 0; i < n; i++) {
				word = fgetc(inFile);

				if (word == 10 || word == 13 || word == 3338 || word == 2573)
					word = fgetc(inFile);

				if(word & 0x80){
					temp = word;
					word = fgetc(inFile);
					fprintf(outFile, "%c%c", temp, word);
				}
				else {
					if (word == 32) {
						fprintf(outFile, "_");
					}
					else if (word != 10 && word != 13 && word != 3338 && word != 2573) {
						fprintf(outFile, "%c", word);
					}
				}
				if(i==0)
					range = ftell(inFile);
			}
			fprintf(outFile, "\n");

			word = fgetc(inFile);
			if (word == EOF)				//종료 조건
				break;
			fseek(inFile, -1, 1);

			fseek(inFile, range, 0);
		}
	}
	fclose(inFile);
	fclose(outFile);
}

void findWordUniGram(int n, char* fileName) {
	int i = 0;
	int word, temp;
	int state = 0;
	unsigned int range;
	FILE *inFile = fopen("input.txt", "r");
	FILE *outFile = fopen(fileName, "w");

	if (inFile == NULL)
	{
		printf("File is not open..\n");
		return 0;
	}
	else
	{
		while (1) {
			while (1) {
				word = fgetc(inFile);
				if (word == EOF) {		//종료 조건
					state = 1;
					break;
				}
				if (word == 32 || word == 10 || word == 13 || word == 3338 || word == 2573) {
					range = ftell(inFile); 
					break;
				}

				if (word & 0x80) {
					temp = word;
					word = fgetc(inFile);
					fprintf(outFile, "%c%c", temp, word);
				}
				else {
					if (word != 32 && word != 10 && word != 13 && word != 3338 && word != 2573) {
						fprintf(outFile, "%c", word);
					}
				}
			}
			fprintf(outFile, "\n");

			if (state == 1) {				//종료 
				break;
			}
			fseek(inFile, range, 0);
		}
	}
	fclose(inFile);
	fclose(outFile);
}

void findWordNGram(int n, char* fileName) {
	int i = 0;
	int word, temp;
	int wordCount = 0;
	int state = 0;
	unsigned int range;
	FILE *inFile = fopen("input.txt", "r");
	FILE *outFile = fopen(fileName, "w");

	if (inFile == NULL)
	{
		printf("File is not open..\n");
		return 0;
	}
	else
	{
		while (1) {
			wordCount = 0;
			while (1) {
				word = fgetc(inFile);
				if (word == EOF) {		//종료 조건
					state = 1;
					break;
				}
				if (word == 32 || word == 10 || word == 13 || word == 3338 || word == 2573) {
					if (wordCount == n-1) {
						break;
					}
					else if (0 < wordCount < n)
						fprintf(outFile, "_");
					wordCount++;	
					if (wordCount == 1) {
						range = ftell(inFile);
					}
				}

				if (word & 0x80) {
					temp = word;
					word = fgetc(inFile);
					fprintf(outFile, "%c%c", temp, word);
				}
				else {
					if (word != 32 && word != 10 && word != 13 && word != 3338 && word != 2573) {
						fprintf(outFile, "%c", word);
					}
				}
			}
			fprintf(outFile, "\n");

			if (state == 1) {				//종료 
				break;
			}
			fseek(inFile, range, 0);
		}
	}
	fclose(inFile);
	fclose(outFile);
}

int main(void)
{
	//printf("%d %d %d %d %d %d\n", '\n', '\r', '\n\r', '\r\n', '.', '가');
	int iNumber1, iNumber2;
	int count;
	char word[1024];
	while (1) {
		memset(word, 0, sizeof(word));
		printf("1 : 음절단위        2 : 어절단위        3 : exit \n insert : ");
		scanf("%d", &iNumber1);

		if (iNumber1 == 1) {
			printf("1:execute All Gram, 2: search, 7:exit \n insert : ");
			scanf("%d", &iNumber2);

			switch (iNumber2) {
			case 1:
				findUnigramCount();
				//system("wordcount.exe -new unigram_output.txt unigram_freq.txt");
				findNGram(2, "bigram_output.txt");
				system("wordcount.exe -new bigram_output.txt bigram_freq.txt");
				findNGram(3, "trigram_output.txt");
				system("wordcount.exe -new trigram_output.txt trigram_freq.txt");
				findNGram(4, "4gram_output.txt");
				system("wordcount.exe -new 4gram_output.txt 4gram_freq.txt");
				findNGram(5, "5gram_output.txt");
				system("wordcount.exe -new 5gram_output.txt 5gram_freq.txt");
				break;
			case 2:
				scanf("%d %s", &count, word);
				nGramAccess(count, word);
				break;
			case 3:
				break;
			
			}
		}
		else if (iNumber1 == 2) {
			printf("1:execute All Gram, 2: search, 7:exit \n insert : ");
			scanf("%d", &iNumber2);

			switch (iNumber2) {
			case 1:
				findWordUniGram(1, "word_uni_output.txt");
				system("wordcount.exe -new word_uni_output.txt word_uni_freq.txt");
				findWordNGram(2, "word_bi_output.txt");
				system("wordcount.exe -new word_bi_output.txt word_bi_freq.txt");
				findWordNGram(3, "word_tri_output.txt");
				system("wordcount.exe -new word_tri_output.txt word_tri_freq.txt");
				findWordNGram(4, "word_4_output.txt");
				system("wordcount.exe -new word_4_output.txt word_4_freq.txt");
				findWordNGram(5, "word_5_output.txt");
				system("wordcount.exe -new word_5_output.txt word_5_freq.txt");
				break;
			case 2:
				scanf("%d %s", &count, word);
				wordNGramAccess(count, word);
				break;
			case 3:
				break;
			}
		}
		else if (iNumber1 == 3) {
			return 0;
		}
	}
	return 0;
}

