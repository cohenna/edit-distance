#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "editdistance.h"

#define TEST_PASS -100
#define MAX_BUF_SIZE 1048576

int test_levenshtein(const char *str1, const char *str2, int expected) {
	int d = levenshtein(str1, str2);
	if(d != expected) {
		return d;
	}
	return TEST_PASS;
}

int test_damerau_levenshtein(const char *str1, const char *str2, int expected) {
	int d = damerau_levenshtein(str1, str2);
	if(d != expected) {
		return d;
	}
	return TEST_PASS;
}

void get_next(char *buf, char *tmp_buf, int *i, int len) {
	int j = 0;
	while(*i < len) {
		char c = buf[*i];
		if(c == ',') {
			(*i)++;
			break;
		}
		tmp_buf[j] = c;
		(*i)++;
		j++;
	}
	tmp_buf[j] = '\0';
}

void run_tests(const char *test_file_name) {
	int levenshtein_expected, damerau_levenshtein_expected, len, i, damerau_passes, levenshtein_passes, total;
	const char *str1, *str2;
	char *buf, *tmp_buf;
	FILE *file;
	file = fopen(test_file_name, "r");
	if(!file) return;
	buf = (char*)malloc(MAX_BUF_SIZE*sizeof(char));
	tmp_buf = (char*)malloc(MAX_BUF_SIZE*sizeof(char));
	if(!buf || !tmp_buf) {
		if(buf) free(buf);
		if(tmp_buf) free(tmp_buf);
		fclose(file);
		return;
	}
	
	levenshtein_passes = 0;
	damerau_passes = 0;
	total = 0;
	while(fgets(buf, MAX_BUF_SIZE, file)) {
		len = strlen(buf);
		if(buf[0] == '#') continue;
		i = 0;
		get_next(buf, tmp_buf, &i, len);
		str1 = strdup(tmp_buf);
		get_next(buf, tmp_buf, &i, len);
		str2 = strdup(tmp_buf);
		get_next(buf, tmp_buf, &i, len);
		levenshtein_expected = atoi(tmp_buf);
		get_next(buf, tmp_buf, &i, len);
		damerau_levenshtein_expected = atoi(tmp_buf);
		i = test_levenshtein(str1, str2, levenshtein_expected);
		if(i != TEST_PASS) {
			printf("FAIL: levenshtein(%s,%s) yields %d.  Expected = %d\n", str1, str2, i, levenshtein_expected);
		}
		else {
			levenshtein_passes++;
		}
		i = test_damerau_levenshtein(str1, str2, damerau_levenshtein_expected);
		if(i != TEST_PASS) {
			printf("FAIL: damerau_levenshtein(%s,%s) yields %d.  Expected = %d\n", str1, str2, i, damerau_levenshtein_expected);
		}
		else {
			damerau_passes++;
		}
		total++;
	}
	
	printf("levenshtein %d passes out of %d total tests\n", levenshtein_passes, total);
	printf("damerau_levenshtein %d passes out of %d total tests\n", damerau_passes, total);
	
	fclose(file);
	free(buf);
	free(tmp_buf);
	
}

int main(int c, const char** args) {
	if(c > 1) {
		run_tests(args[1]);
	}
	
	return 0;
}
