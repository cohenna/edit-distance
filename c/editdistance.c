#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "editdistance.h"

#define DEBUG 0
#define MEMORY_ALLOCATE malloc
#define MEMORY_FREE free

static int min(int x, int y) {
	return x > y ? y : x;
}

static int min3(int x, int y, int z) {
	return min(min(x,y), min(x,z));
}

static int min4(int x, int y, int z, int w) {
	return min(min3(x,y,z), w);
}

#if defined(DEBUG) && DEBUG == 1
static int print_table(int **table, int bound1, int bound2) {
	int i, j;
	for(i = 0; i < bound1; i++) {
		for(j = 0; j < bound2; j++) {
			printf("%d\t", table[i][j]);
		}
		printf("\n");
	}
}
#endif

int levenshtein(const char *str1, const char *str2) {
	int bound1, bound2, i, j, cost, delete, insert, subs, prev_i, prev_j;
	int **table;
	if(!str1 || !str2) {
		return -1;
	}
	bound1 = strlen(str1)+1;
	bound2 = strlen(str2)+1;
	if(bound1 == 1) {
		return bound2-1;
	}
	if(bound2 == 1) {
		return bound1-1;
	}
	table = (int**)MEMORY_ALLOCATE(bound1* sizeof(int *));
	if(!table) {
		return -1;
	}
	cost = bound2 * sizeof(int);
	for(i = 0; i < bound1; i++) {
		table[i] = (int*)MEMORY_ALLOCATE(cost);
		table[i][0] = i;
	}
	for(j = 1; j < bound2; j++) {
		table[0][j] = j;
	}
	prev_i = 0;
	
	for(i = 1; i < bound1; i++) {
		prev_j = 0;
		for(j = 1; j < bound2; j++) {
			cost = str1[prev_i] == str2[prev_j] ? 0 : 1;
			delete = table[i][prev_j] + 1;
			insert = table[prev_i][j] + 1;
			subs = table[prev_i][prev_j] + cost;
			table[i][j] = min3(delete, insert, subs);
#if defined(DEBUG) && DEBUG == 1
			printf("[c1=%c,c2=%c,i=%d,j=%d,pi=%d,pj=%d,c=%d,d=%d,i=%d,s=%d,t[i,j]=%d]\n", str1[prev_i], str2[prev_j], i, j,prev_i,prev_j,cost,delete,insert,subs,table[i][j]);
			print_table(table, bound1, bound2);
#endif
			prev_j++;
		}
		prev_i++;
	}
	cost = table[bound1-1][bound2-1];
	for(i = bound1-1; i >= 0; i--) {
		MEMORY_FREE(table[i]);
	}
	MEMORY_FREE(table);
	
	return cost;
}

int damerau_levenshtein(const char *str1, const char *str2) {
	int bound1, bound2, i, j, cost, delete, insert, subs, prev_i, prev_j;
	int **table;
	if(!str1 || !str2) {
		return -1;
	}
	bound1 = strlen(str1)+1;
	bound2 = strlen(str2)+1;
	if(bound1 == 1) {
		return bound2-1;
	}
	if(bound2 == 1) {
		return bound1-1;
	}
	table = (int**)MEMORY_ALLOCATE(bound1* sizeof(int *));
	if(!table) {
		return -1;
	}
	cost = bound2 * sizeof(int);
	for(i = 0; i < bound1; i++) {
		table[i] = (int*)MEMORY_ALLOCATE(cost);
		table[i][0] = 0;
	}
	for(j = 0; j < bound2; j++) {
		table[0][j] = 0;
	}
	prev_i = 0;
	for(i = 1; i < bound1; i++) {
		prev_j = 0;
		for(j = 1; j < bound2; j++) {
			cost = str1[prev_i] == str2[j-1] ? 0 : 1;
			delete = table[i][prev_j] + 1;
			insert = table[prev_i][j] + 1;
			subs = table[prev_i][prev_j] + cost;
			if(i > 1 && j > 1 && str1[prev_i] == str2[j-2] && str1[prev_i-1] == str2[j-1]) {
				cost = table[i-2][j-2] + cost; // borrowing cost variable for transposition
				table[i][j] = min4(delete, insert, subs, cost);
			}
			else {
				table[i][j] = min3(delete, insert, subs);
			}
#if defined(DEBUG) && DEBUG == 1
			printf("[c1=%c,c2=%c,i=%d,j=%d,pi=%d,pj=%d,c=%d,d=%d,i=%d,s=%d,t[i,j]=%d]\n", str1[prev_i], str1[j-1], i, j,prev_i,prev_j,cost,delete,insert,subs,table[i][j]);
			print_table(table, bound1, bound2);
#endif
			prev_j++;
		}
		prev_i++;
	}
	cost = table[bound1-1][bound2-1];
	for(i = bound1-1; i >= 0; i--) {
		MEMORY_FREE(table[i]);
	}
	MEMORY_FREE(table);
	
	return cost;
}
