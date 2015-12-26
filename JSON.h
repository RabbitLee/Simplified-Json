#ifndef JSON_H
#define JSON_H

#include<stdio.h>

/* The max length of a key, a string or a number */
#define KEY_LENGTH 300
#define NUMBER_LENGTH 100

/* Something hard to describe... */
#define NUMBER_LENGTH_MAX 10

/* cJSON Types: */
#define JSON_FALSE 0
#define JSON_TRUE 1
#define JSON_NULL 2
#define JSON_NUMBER 3
#define JSON_STRING 4
#define JSON_ARRAY 5
#define JSON_OBJECT 6

/* The cJSON structure: */
typedef struct JSON {
	int type;					/* The type of the item, as above. */

	char *valuestring;			/* The item's string, if type==JSON_STRING */
	int valueint;				/* The item's number, if type==JSON_TRUE||JSON_FALSE */
	double valuedouble;			/* The item's number, if type==JSON_NUMBER  */

	char *key;                  /* The item's key, if type==JSON_OBJECT */
	struct JSON *value;         /* the item's value, if type==JSON_ARRAY||JSON_OBJECT */
	struct JSON *next;          /* The next item, if type==JSON_ARRAY||JSON_OBJECT */
} JSON;

/* Parse & Print */
extern JSON *ParseJSON(const char *value);
extern JSON *ParseJSONFromFile(const char *file_name);

extern void PrintJSON(JSON *item);
extern void PrintJSONToFile(JSON *item, const char *file_name);

/* Create */
extern JSON *CreateNULL(void);
extern JSON *CreateTrue(void);
extern JSON *CreateFalse(void);
extern JSON *CreateBool(int b);
extern JSON *CreateNumber(double num);
extern JSON *CreateString(const char *string);
extern JSON *CreateArray(void);
extern JSON *CreateObject(void);

/* Append */
extern void AddItemToArray(JSON *array, JSON *item);
extern void AddItemToObject(JSON *object, const char *key, JSON *value);

/* Update */
extern void ReplaceItemInArray(JSON *array, int which, JSON *new_item);
extern void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value);

/* Remove/Delete */
extern JSON *DetachItemFromArray(JSON *array, int which);
extern void DeleteItemFromArray(JSON *array, int which);
extern JSON *DetachItemFromObject(JSON *object, const char *key);
extern void DeleteItemFromObject(JSON *object, const char *key);

extern void DeleteJSON(JSON *item);

/* Duplicate */
extern JSON *Duplicate(JSON *item, int recurse);

/* Read */
extern JSON *GetItemInArray(JSON *array, int which);
extern JSON *GetItemInObject(JSON *object, const char *key);
extern JSON *GetItemInJSON(JSON *json, const char *path);

/* Addition */
extern void printblank(int num, FILE *fp);/* Print blank space to the file(fp), the number of which is num. */
extern void PrintFile(JSON *item, int blank, FILE *fp);/* Print json to the file(fp), blank determines the number of blank space in front of each line. */
extern JSON *ParseString(const char *value);/* Prase a string which starts from value, return the relative json. */
extern JSON *ParseNumber(const char *value);/* Prase a number which starts from value, return the relative json. */
extern char *ScanItem(char *value);/* Scan the item which starts from value, return the position of next item. */
extern char *ScanKey(char *value, char *key);/* Scan the item's key which starts from value and copy it to key, return the position of item's value. */
extern char *NumberFormat(double num);/* Change a number to a string. */

#endif
