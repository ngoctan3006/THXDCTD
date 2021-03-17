#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "libfdr/jrb.h"
#include "libfdr/jval.h"

#define MAX_LEN 1000
#define MAX_LEN_WORD 15

void clear() {
    while(getchar() != '\n');
}

void swap(int a[], int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

void quickSort(int a[], int l, int r) {
    int pivot, i, j;
    if(r <= l) return;
    i = l;
    j = r + 1;
    pivot = a[l];
    do {
        do i++; while(a[i] < pivot);
        do j--; while(a[j] > pivot);
        if(i < j) swap(a, i, j);
    } while(i < j);
    swap(a, l, j);
    quickSort(a, l, j-1);
    quickSort(a, j+1, r);
}

int binarySearch(int a[], int l, int r, int k) {
    if(l > r) return -1;
    int mid = l + (r-l)/2;
    if(a[mid] == k) return mid;
    if(a[mid] > k) return binarySearch(a, l, mid-1, k);
    return binarySearch(a, mid+1, r, k);
}

void strCat(char *dest, char c) {
    int len = strlen(dest);
    dest[len] = c;
    dest[len+1] = '\0';
}

void read_sw(JRB stop_word) {
    FILE *f = fopen("completed/stopw.txt", "r");
    if(f == NULL) {
        printf("Loi mo file stop word\n");
        exit(-1);
    }
    char word[MAX_LEN_WORD];
    while(!feof(f)) {
        fscanf(f, "%s", word);
        if(!jrb_find_str(stop_word, word))
            jrb_insert_str(stop_word, strdup(word), new_jval_i(1));
    }
    fclose(f);
}

char *itoa(int i) {
    char s[MAX_LEN_WORD];
    int c = 0, t = 0;
    while(i > 0) {
        t = t*10 + i%10;
        i /= 10;
    }
    while(t > 0) {
        s[c++] = t%10 + '0';
        t /= 10;
    }
    s[c] = '\0';
    return strdup(s);
}

void add_word(JRB word, JRB sw, char *s, int line, int col) {
    JRB node = jrb_find_str(word, s);
    JRB freq;
    char temp[MAX_LEN_WORD] = "";
    int f = 1;
    if(!jrb_find_str(sw, s)) {
        if(node) {
            freq = (JRB) jval_v(node->val);
            f = ++freq->val.i;
            strcpy(temp, freq->val.s);
            strcat(temp, ", ");
        } else freq = make_jrb();
        jrb_insert_str(word, s, new_jval_v(freq));
        strcat(temp, "(");
        strcat(temp, itoa(line));
        strcat(temp, ", ");
        strcat(temp, itoa(col));
        strcat(temp, ")");
        jrb_insert_int(freq, f, new_jval_s(temp));
    }
}

int main() {
    JRB word = make_jrb();
    JRB sw = make_jrb();
    read_sw(sw);
    char s[MAX_LEN_WORD];
    int line, col;
    do {
        printf("Enter word, line, col: ");
        scanf("%s%d%d", s, &line, &col);
        clear();
        add_word(word, sw, s, line, col);
    } while(strcmp(s, "stop") != 0);
    JRB ptr, freq;
    jrb_traverse(ptr, word) {
        freq = (JRB) jval_v(ptr->val);
        printf("%s: %d %s\n", jval_s(ptr->key), jval_i(freq->key), jval_s(freq->val));
    }
    // int i = 123;
    // printf("%s\n", itoa(i));
    // JRB stop_word = make_jrb();
    // read_sw(stop_word);
    // JRB ptr;
    // int c = 0;
    // jrb_traverse(ptr, stop_word) {
    //     c++;
    //     printf("%s\n", jval_s(ptr->key));
    // }
    // printf("%d\n", c);
    // char c[2];
    // FILE *f = fopen("completed/vanban.txt", "r");
    // fscanf(f, "%c", c);
    // printf("%s\n", c);

    // int a[] = {4,5,1,4,9,5,4,1,3,2,6,5,7,8,3};
    // for(int i = 0; i < 15; i++) printf("%d ", a[i]);
    // quickSort(a, 0, 14);
    // printf("\n");
    // for(int i = 0; i < 15; i++) printf("%d ", a[i]);
    // printf("\n");
    // int x = 10;
    // printf("Tim thay %d o vi tri %d\n", x, binarySearch(a, 0, 14, x));

    // char s[10] = "Hello ";
    // char c = 'T';
    // strCat(s, c);
    // printf("%s\n", s);
    return 0;
}