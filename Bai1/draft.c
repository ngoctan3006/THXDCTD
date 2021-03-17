#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 1000

typedef struct {
    char word[15];
    char freq[MAX_SIZE];
} Word;

int readFileS(char [][15]);
int readFile(Word [], char [][15], int);
char *toLowerCase(char *);
void swap(char [][15], int, int);
void sort(char [][15], int, int);
int search(char [][15], int, int, char *);
char *toString(char);

int main() {
    return 0;
}

int readFile(Word w[], char sw[][15], int sizeSW) {
    FILE *f = fopen("completed/vanban.txt", "r");
    if(f == NULL) {
        printf("Loi mo file van ban!\n");
        exit(1);
    }
    int size = 0, line = 1, maybeFN = 0, isNW = 1, isFN = 0;
    char c;
    char word[15] = "";
    loop: while(!feof(f)) {
        fscanf(f, "%c", &c);
        if(isalpha(c)) {
            if(c >= 'A' && c <= 'Z') {
                if(isNW && maybeFN) isFN = 1;
                strcat(word, toString(c));
            }
        }
    }
    fclose(f);
    return size;
}

char *toString(char c) {
    char *str[2];
    str[0] = c;
    str[1] = '\0';
    return str;
}

int search(char s[][15], int l, int r, char *key) {
    if(l > r) return -1;
    int mid = l + (r-l)/2;
    if(strcmp(s[mid], key) == 0) return mid;
    if(strcmp(s[mid], key) > 0) return search(s, l, mid-1, key);
    return search(s, mid+1, r, key);
}

void sort(char s[][15], int l, int r) {
    if(r <= l) return;
    int i = l, j = r + 1;
    char pivot[15];
    strcpy(pivot, s[l]);
    do {
        do i++; while(strcmp(s[i], pivot) < 0);
        do j--; while(strcmp(s[j], pivot) > 0);
        if(i < j) swap(s, i, j);
    } while(i < j);
    swap(s, l, j);
    sort(s, l, j-1);
    sort(s, j+1, r);
}

void swap(char s[][15], int i, int j) {
    char t[15];
    strcpy(t, s[i]);
    strcpy(s[i], s[j]);
    strcpy(s[j], t);
}

int readFileS(char sw[][15]) {
    FILE *f = fopen("completed/stopw.txt", "r");
    if(f == NULL) {
        printf("Loi mo file stop word!\n");
        exit(1);
    }
    int size = 0;
    while(!feof(f)) {
        fscanf(f, "%s", sw[size++]);
    }
    fclose(f);
    sort(sw, 0, size-1);
    return size;
}

char *toLowerCase(char *source) {
    int len = strlen(source);
    for(int i = 0; i < len; i++) {
        if(source[i] >= 'A' && source[i] <= 'Z')
            source[i] += 32;
    }
    return source;
}