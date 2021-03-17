// De bai : tao mot index table doc ra tu van ban cho truoc. Hien thi ra man hinh nhung tu CO NGHIA thoa man cac yeu cau cho truoc và khong thuoc list cam trong file stop_word


// ---------------------------------Khai bao thu vien--------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
// #include <conio.h>
#define MAX_LENTH 50
const int LENGTH = 50;


// --------------------------------------Dinh nghia ctdl Binary search tree ---------------

typedef void * BofDATA;

struct _BST_NODE;

typedef struct _BST_NODE BST_NODE,*NODE;

struct _BST_NODE
{
    BofDATA data;
    BST_NODE *left, *right;
};

typedef enum // liet ke cac cach dich cay
{
    BST_LVR,
    BST_LRV,
    BST_VLR,
    BST_VRL,
    BST_RLV,
    BST_RVL
} BST_TRAVERSE_MODE;

void BstInit(NODE * root); /*define root*/
NODE BstCreateNode(BofDATA data,int size); /*create new node  node*/
void BstInsertNode(NODE * root,int (*func)(BofDATA,void *),void * user,int size);/*insert node -1 : left 0 : none 1 : right*/
NODE BstFindNode(NODE * root,int (*func)(BofDATA,void*),void *user); /*find node ...*/
void BstTraverse(NODE root,BST_TRAVERSE_MODE mode,void (*func)(BofDATA,void*),void *user);/*traverse ....*/
// -----
void BstInit(NODE * root)
{
    *root = NULL;
}
NODE BstCreateNode(BofDATA data,int size)
{
    BST_NODE* n = (BST_NODE*)malloc(sizeof(BST_NODE));
    n->data = malloc(size);
    memcpy(n->data,data,size);
    n->left = n->right = NULL;
    return n;
}

void BstInsertNode(NODE * root,int (*func)(BofDATA,void *),void * user,int size)
{
    if(*root == NULL)
    {
        *root = BstCreateNode(user,size);
        return;
    }
    int bst_cmp = func((*root)->data,user);
    if(bst_cmp <= 0)
        BstInsertNode(&((*root)->left),func,user,size);
    else if(bst_cmp > 0)
        BstInsertNode(&((*root)->right),func,user,size);
}

NODE BstFindNode(NODE * root,int (*func)(BofDATA,void*),void *user)
{
    if(*root == NULL)return NULL;
    int bst_cmp = func((*root)->data,user);
    if(bst_cmp == 0)
    {
        return *root;
    }
    else if(bst_cmp > 0)return BstFindNode(&(*root)->right,func,user);
    else if(bst_cmp < 0)return BstFindNode(&(*root)->left,func,user);

}

void BstTraverse(NODE root,BST_TRAVERSE_MODE mode,void (*func)(BofDATA,void*),void *user)
{
    if (root == NULL)
        return;

    switch (mode)
    {
    case BST_LVR:
        if (root->left != NULL)
            BstTraverse(root->left, mode, func, user);
        func(root->data, user);
        if (root->right != NULL)
            BstTraverse(root->right, mode, func, user);
        break;
    case BST_LRV:
        if (root->left != NULL)
            BstTraverse(root->left, mode, func, user);
        if (root->right != NULL)
            BstTraverse(root->right, mode, func, user);
        func(root->data, user);
        break;
    case BST_RLV:
        if (root->right != NULL)
            BstTraverse(root->right, mode, func, user);
        if (root->left != NULL)
            BstTraverse(root->left, mode, func, user);
        func(root->data, user);
        break;
    case BST_RVL:
        if (root->right != NULL)
            BstTraverse(root->right, mode, func, user);
        func(root->data, user);
        if (root->left != NULL)
            BstTraverse(root->left, mode, func, user);
        break;
    case BST_VLR:
        func(root->data, user);
        if (root->left != NULL)
            BstTraverse(root->left, mode, func, user);
        if (root->right != NULL)
            BstTraverse(root->right, mode, func, user);
        break;
    case BST_VRL:
        func(root->data, user);
        if (root->right != NULL)
            BstTraverse(root->right, mode, func, user);
        if (root->left != NULL)
            BstTraverse(root->left, mode, func, user);
        break;
    }
}
// --------------------------------------------------Khai báo các hàm sử dụng ---------------------
typedef struct
{
    char word[MAX_LENTH];
    int quantity; // so luong tu
    int lineth[MAX_LENTH]; // mang chi thi dong hien thi
    int line_app; // chieu dai cua mang lineth
} doc;

int compare_stop(BofDATA x, BofDATA y);
int compare_doc(BofDATA x,BofDATA y);
void OUTPUT(BofDATA x,BofDATA y);

// -------------------------------Hàm main chính---------------

int main()
{
    int tmpg = 0;
    char str[2] = "\0";
    char t[MAX_LENTH] = "\0";
    int line = 1;
    int check_nline = 0;
    int first_name = 0;
    int new_word = 0;
    NODE T_stopw;
    NODE T_doc;
    BstInit(&T_doc);
    BstInit(&T_stopw);
    char s[LENGTH];
    FILE * f = fopen("stop_word.txt","r");
    while(fgets(s,LENGTH,f))
    {
        while(!isalpha(s[strlen(s)-1]))
            s[strlen(s)-1] = '\0';
        BstInsertNode(&T_stopw,compare_stop,s,sizeof(s));
    }

    f = fopen("completed/vanban.txt","r");
    
    while(fscanf(f,"%c",&str[0]) != EOF)
    {
        if(isalpha(str[0]))
        {
            if('A' <= str[0] && str[0] <= 'Z')
            {
                if(check_nline == 0 && new_word == 1)
                    first_name = 1;
                str[0] = tolower(str[0]); // upercase to none
            }
            strcpy(t,strcat(t,str));
            check_nline = 0;
            new_word = 0; //--------Doc tu kiem tra danh tu rieng hay den dau cach thi het mot tu 
        }
        else
        {
            new_word = 1;
            check_nline = 0;
            if(strcmp(t,"\0") != 0)
            {
                NODE node_stopw = BstFindNode(&T_stopw,compare_stop,t);
                if(node_stopw == NULL && first_name == 0)
                {
                    doc * temp = (doc*)malloc(sizeof(doc));
                    strcpy(temp->word,t);
                    NODE node_doc = BstFindNode(&T_doc,compare_doc,temp);
                    if(node_doc != NULL)
                    {
                        ((doc*)node_doc->data)->quantity++;
                        ((doc*)node_doc->data)->lineth[((doc*)node_doc->data)->line_app] = line;
                        ((doc*)node_doc->data)->line_app++;
                    }
                    else
                    {
                        temp->quantity = 1;
                        temp->lineth[0] = line;
                        temp->line_app = 1;
                        BstInsertNode(&T_doc,compare_doc,temp,sizeof(doc));
                    }
                }
            }
            if(str[0] == '\n')
            {
                line++;
                check_nline = 1;
            }
            first_name = 0;
            t[0] = '\0';
        }
    }
    printf("Duoi day la chuong trinh cua em. Neu co loi gi mong duoc thay giai dap a! Em cam on !\n");
    printf("\n");
    BstTraverse(T_doc,BST_RVL,OUTPUT,NULL);
     printf("\n");
    printf("Chuong trinh den day la ket thuc ! Thanks for watching!\n");

    return 0;
}


// ---------------------------------------------------Chi tiết các hàm đã sử dụng ------------------------
int compare_stop(BofDATA x, BofDATA y)
{
    return strcmp((char *)x,(char *)y);
}

int compare_doc(BofDATA x,BofDATA y)
{
    doc * a = (doc *)x;
    doc * b = (doc *)y;
    return strcmp(a->word,b->word);
}

void OUTPUT(BofDATA x,BofDATA y)
{
	int i;
    doc * a = (doc*)x;
    
    printf("Tu: %s                 So luong :%d                ",a->word,a->quantity);
    printf("Vi tri xuat hien la dong thu  : %d",a->lineth[0]);
    for( i=1; i<a->line_app; i++)
    {
        if(a->lineth[i] == a->lineth[i-1])
            continue;
        printf(",%d",a->lineth[i]);
    }
    printf("\n");
}

// ------------------------------------------------------Kết thúc -----------------------
