#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "libfdr/jrb.h"
#include "libfdr/jval.h"

#define MAX_LEN 1000
#define MAX_LEN_WORD 15

void read_sw(JRB);
void create_index_table(JRB, JRB);
char *to_lower(char *);
char *itoa(int);

int main() {
    JRB stop_work = make_jrb();
    JRB index_table = make_jrb();
    read_sw(stop_work);
    create_index_table(index_table, stop_work);
    JRB ptr, freq;
    jrb_traverse(ptr, index_table) {
        freq = (JRB) jval_v(ptr->val);
        printf("%s: %d %s\n", jval_s(ptr->key), jval_i(freq->key), jval_s(freq->val));
    }
    return 0;
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

void create_index_table(JRB index_table, JRB stop_word) {
    FILE *f = fopen("completed/vanban.txt", "r");
    if(f == NULL) {
        printf("Loi mo file van ban\n");
        exit(-1);
    }
    int new_word = 0, new_line = 1, first_name = 0;
    int lineNo = 1, colNo = 1, size;
    char word[MAX_LEN_WORD] = "", index_word[MAX_LEN] = "", c[2], temp[MAX_LEN_WORD];
    JRB node, freq;
    while(fscanf(f, "%c", c) != EOF) {
        if(c[0] == '\n') {
            new_line = 1;
            lineNo++;
            colNo = 1;
            continue;
        }
        if(isalnum(c[0])) {
            strcat(word, c);
        }
        if(c[0] == ' ') {
            new_word = 1;
            size = strlen(word);
            if(!(word[0] >= 'A' && word[0] <= 'Z' && first_name)) {
                to_lower(word);
                if(word[size-1] == '.') {
                first_name = 0;
                word[size-1] = '\0';
                size--;
                } else first_name = 1;
                if(!jrb_find_str(stop_word, word)) {
                    node = jrb_find_str(index_table, word);
                    if(node) {
                        freq = (JRB) jval_v(node->val);
                        freq->key.i++;
                        strcpy(temp, "");
                        strcat(temp, ", (");
                        strcat(temp, itoa(lineNo));
                        strcat(temp, ", ");
                        strcat(temp, itoa(colNo));
                        strcat(temp, ")");
                        strcat(freq->val.s, temp);
                    } else {
                        freq = NULL;
                        jrb_insert_str(index_table, word, new_jval_v(freq));
                        strcpy(temp, "");
                        strcat(temp, "(");
                        strcat(temp, itoa(lineNo));
                        strcat(temp, ", ");
                        strcat(temp, itoa(colNo));
                        strcat(temp, ")");
                        jrb_insert_int(freq, 1, new_jval_s(temp));
                    }
                }
            }
            colNo++;
        }
    }
    fclose(f);
}

char *to_lower(char *str) {
    int len = strlen(str), i;
    for(i = 0; i < len; i++) {
        str[i] = tolower(str[i]);
    }
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