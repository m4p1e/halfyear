/*the struct of hashtable*/

typedef struct _bucket{
    void *attr;     //实际内容
    int next;       //解决冲突
    char key[1];    //关键字
}bucket;

typedef struct _hashtable{
    int cap;            //容量
    int used;           //已经使用了的容量
    bucket* buckets;    //顺序表
    
}hashtable;