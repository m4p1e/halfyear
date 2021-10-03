/*the struct of hashtable*/

typedef struct _bucket{
    void *attr;     //实际内容
    int next;       //解决冲突
    char key[1];    //关键字
}bucket;

typedef struct _hashtable{

}hashtable;