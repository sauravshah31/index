struct info
{
    char* DB; /*name of database*/
};

struct  table: struct info
{
    char* table_name; /*name of the table currently woring  on*/
};

typedef struct info info;
typedef struct table table;

