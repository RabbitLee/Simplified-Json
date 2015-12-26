#define _CRT_SECURE_NO_WARNINGS

#include"JSON.h"
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

/* Create */
JSON *CreateNULL(void)
{
	JSON *p;
	if ((p = (JSON *)malloc(sizeof(JSON))) == NULL)
		return NULL;
	p->type = JSON_NULL;
	return p;
}
JSON *CreateTrue(void)
{
	JSON *p;
	if ((p = (JSON *)malloc(sizeof(JSON))) == NULL)
		return NULL;
	p->type = JSON_TRUE;
	p->valueint = 1;
	return p;
}
JSON *CreateFalse(void)
{
	JSON *p;
	if ((p = (JSON *)malloc(sizeof(JSON))) == NULL)
		return NULL;
	p->type = JSON_FALSE;
	p->valueint = 0;
	return p;
}
JSON *CreateBool(int b)
{
	if (b == 0)
		return CreateFalse();
	else
		return CreateTrue();
}
JSON *CreateNumber(double num)
{
	JSON *p;
	if ((p = (JSON *)malloc(sizeof(JSON))) == NULL)
		return NULL;
	p->type = JSON_NUMBER;
	p->valuedouble = num;
	return p;
}
JSON *CreateString(const char *string)
{
	JSON *p;
	if ((p = (JSON *)malloc(sizeof(JSON))) == NULL)
		return NULL;
	p->type = JSON_STRING;
	p->valuestring = (char *)calloc(strlen(string) + 1, sizeof(char));
	strcpy(p->valuestring, string);
	return p;
}
JSON *CreateArray(void)
{
	JSON *p;
	if ((p = (JSON *)malloc(sizeof(JSON))) == NULL)
		return NULL;
	p->type = JSON_ARRAY;
	p->value = p->next = NULL;
	return p;
}
JSON *CreateObject(void)
{
	JSON *p;
	if ((p = (JSON *)malloc(sizeof(JSON))) == NULL)
		return  NULL;
	p->type = JSON_OBJECT;
	p->value = p->next = NULL;
	return p;
}

/* Append */
void AddItemToArray(JSON *array, JSON *item)
{
	if (array == NULL || item == NULL)
		return;
	if (array->value == NULL)
		array->value = Duplicate(item, 1);
	else
	{
		while (array->next != NULL)
			array = array->next;
		JSON *p = CreateArray();
		array->next = p;
		p->value = Duplicate(item, 1);
	}
}
void AddItemToObject(JSON *object, const char *key, JSON *value)
{
	if (object == NULL || value == NULL)
		return;
	if (object->value == NULL)
	{
		object->key = (char *)calloc(strlen(key) + 1, sizeof(char));
		strcpy(object->key, key);
		object->value = Duplicate(value, 1);		
	}
	else
	{
		while (object->next != NULL)
			object = object->next;
		JSON *p = CreateObject();
		object->next = p;
		p->key = (char *)calloc(strlen(key) + 1, sizeof(char));
		strcpy(p->key, key);
		p->value = Duplicate(value, 1);
	}
}

/* Update */
void ReplaceItemInArray(JSON *array, int which, JSON *new_item)
{
	if (array == NULL || which < 0)
		return;
	int i;
	for (i = 0; i < which; i++)
	{
		array = array->next;
		if (array == NULL)
			return;
	}
	DeleteJSON(array->value);
	array->value = Duplicate(new_item, 1);
}
void ReplaceItemInObject(JSON *object, const char *key, JSON *new_value)
{
	if (object == NULL)
		return;
	while (strcmp(object->key, key) != 0)
	{
		object = object->next;
		if (object == NULL)
			return;
	}
	DeleteJSON(object->value);
	object->value = Duplicate(new_value, 1);
}

/* Remove/Delete */
JSON *DetachItemFromArray(JSON *array, int which)
{
	if (array == NULL || array->value == NULL || which < 0)
		return NULL;
	if (which == 0)
	{
		JSON *p = array->value;/* The detached item */
		if (array->next == NULL)
			array->value = array->next = NULL;
		else
		{
			JSON *next = array->next;
			array->value = next->value;
			array->next = next->next;
			free(next);
		}
		return p;
	}
	else
	{
		int i;
		JSON *p;/* The former item of the detached item */
		for (i = 0; i < which - 1; i++)
		{
			array = array->next;
			if (array == NULL)
				return NULL;
		}
		p = array;
		array = array->next;
		if (array == NULL)
			return NULL;
		p->next = array->next;
		free(array);
		return array->value;
	}
}
void DeleteItemFromArray(JSON *array, int which)
{
	DeleteJSON(DetachItemFromArray(array, which));
}
JSON *DetachItemFromObject(JSON *object, const char *key)
{
	if (object == NULL || object->value == NULL)
		return NULL;
	if (strcmp(object->key, key) == 0)
	{
		JSON *p = object->value;/* The detached item */
		if (object->next == NULL)
			object->value = object->next = NULL;
		else
		{
			JSON *next = object->next;
			strcpy(object->key, next->key);
			object->value = next->value;
			object->next = next->next;
			free(next);
		}
		return p;
	}
	else 
	{
		JSON *p;/* The former item of the detached item*/
		do
		{
			p = object;
			object = object->next;
			if (object == NULL)
				return NULL;
		} while (strcmp(object->key, key) != 0);
		p->next = object->next;
		return object->value;
	}
}
void DeleteItemFromObject(JSON *object, const char *key)
{
	DeleteJSON(DetachItemFromObject(object, key));
}

void DeleteJSON(JSON *item)
{
	if (item == NULL)
		return;
	switch(item->type)
	{
		case JSON_FALSE:
		case JSON_TRUE:
		case JSON_NULL:
		case JSON_NUMBER:
		case JSON_STRING:
			free(item);
			break;
		case JSON_ARRAY:
		case JSON_OBJECT:
			DeleteJSON(item->value);
			DeleteJSON(item->next);
			free(item);
			break;
	}
}

/* Duplicate */
JSON *Duplicate(JSON *item, int recurse)
{
	if (item == NULL)
		return NULL;
	if (recurse == 0)
		return item;
	JSON *p;
	switch(item->type)
	{
		case JSON_FALSE:
			p = CreateFalse();
			break;
		case JSON_TRUE:
			p = CreateTrue();
			break;
		case JSON_NULL:
			p = CreateNULL();
			break;
		case JSON_NUMBER:
			p = CreateNumber(item->valuedouble);
			break;
		case JSON_STRING:
			p = CreateString(item->valuestring);
			break;
		case JSON_ARRAY:
			p = CreateArray();
			break;
		case JSON_OBJECT:
			p = CreateObject();
			if (item->value != NULL)
			{
				p->key = (char *)calloc(strlen(item->key) + 1, sizeof(char));
				strcpy(p->key, item->key);
			}
			else
				return p;
			break;
		default:
			p = NULL;
	}
	switch (item->type)
	{
		case JSON_ARRAY:
		case JSON_OBJECT:
			p->value = Duplicate(item->value, recurse);
			p->next = Duplicate(item->next, recurse);
	}
	return p;
}

/* Read */
JSON *GetItemInArray(JSON *array, int which)
{
	if (array == NULL || which < 0)
		return NULL;
	int i;
	for (i = 0; i < which; i++)
	{
		array = array->next;
		if (array == NULL)
			return NULL;
	}
	return array->value;
}
JSON *GetItemInObject(JSON *object, const char *key)
{
	if (object == NULL || object->value == NULL)
		return NULL;
	while (strcmp((object->key), key) != 0)
	{
		object = object->next;
		if (object == NULL)
			return NULL;
	}
	return object->value;
}
JSON *GetItemInJSON(JSON *json, const char *path)
{
	if (json == NULL || path == '\0')
		return json;
	switch (json->type)
	{
		case JSON_FALSE:
		case JSON_TRUE:
		case JSON_NULL:
		case JSON_NUMBER:
		case JSON_STRING:
			return json;
			break;
		case JSON_ARRAY:
			path++;
			int number = 0;
			while (*path != '/' && *path != '\0')
			{
				number = 10 * number + (*path - '0');
				path++;
			}
			if (*path == '\0')
				return GetItemInArray(json, number);
			else 
				return GetItemInJSON(GetItemInArray(json, number), path);
		case JSON_OBJECT:
			path++;
			char *path_raw=path;
			int length = 0;
			while (*path != '/' && *path != '\0')
			{
				path++;
				length++;
			}
			char *key = (char *)calloc(length + 1, sizeof(char));
			strncpy(key, path_raw, length);
			*(key + length) = '\0';
			if (*path == '\0')
				return GetItemInObject(json, key);
			else 
				return GetItemInJSON(GetItemInObject(json, key), path);
	}
}

/* Parse & Print */
JSON *ParseJSON(const char *value)
{
	while (*value == ' ')
		value++;
	switch (*value)
	{
		case ']':
		case '}':
			return NULL;
			break;
		case '\"':
			return ParseString(value+1);
			break;
		case '[':
			value++;
			JSON *array = CreateArray();
			AddItemToArray(array, ParseJSON(value));
			while ((value = ScanItem(value)) != NULL)
				AddItemToArray(array, ParseJSON(value));
			return array;
			break;
		case '{':
			value++;
			JSON *object = CreateObject();
			char key[KEY_LENGTH];
			value = ScanKey(value, key);
			AddItemToObject(object, key, ParseJSON(value));
			value = ScanItem(value);
			while (value != NULL)
			{
				value = ScanKey(value, key);
				AddItemToObject(object, key, ParseJSON(value));
				value = ScanItem(value);
			}
			return object;
			break;
		default :
			if (strncmp(value, "false",5) == 0)
				return CreateFalse( );
			if (strncmp(value, "true",4) == 0)
				return CreateTrue();
			if (strncmp(value, "null",4) == 0)
				return CreateNULL();
			return ParseNumber(value);
			break;
	}
}
JSON *ParseJSONFromFile(const char *file_name)
{ 
	FILE *fp;
	if ((fp = fopen(file_name, "r")) == NULL)
		return NULL;
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	char *file = (char *)calloc(length, sizeof(char));
	//rewind(fp);
	fseek(fp, 0, SEEK_SET);
	fread(file, sizeof(char), length,fp);
	fclose(fp);

	*(file + length) = '\0';
	char *f1, *f2;
	f1 = f2 = file;
	while (*f2 != '\0')
	{
		if (*f2 != '\n')
			*f1++ = *f2;
		f2++;
	}
	*f1 = '\0';
	return ParseJSON(file);
}

void PrintJSON(JSON *item)
{
	if (item == NULL)
		return;
	switch (item->type)
	{
		case JSON_FALSE:
			printf("false");
			break;
		case JSON_TRUE:
			printf("true");
			break;
		case JSON_NULL:
			printf("null");
			break;
		case JSON_NUMBER:
			printf("%s", NumberFormat(item->valuedouble));
			break;
		case JSON_STRING:
			printf("\"%s\"", item->valuestring);
			break;
		case JSON_ARRAY:
			putchar('[');
			while (item->next != NULL)
			{
				PrintJSON(item->value);
				putchar(','); 
				item = item->next;
			}
			PrintJSON(item->value);
			putchar(']');
			break;
		case JSON_OBJECT:
			putchar('{');
			while (item->next != NULL)
			{
				printf("%c%s%c:", 34, item->key, 34);
				PrintJSON(item->value);
				putchar(',');
				item = item->next;
			}
			if (item->value != NULL)
				printf("%c%s%c:", 34, item->key, 34);
			PrintJSON(item->value);
			putchar('}');
			break;
	}
}
void PrintJSONToFile(JSON *item, const char *file_name)
{
	FILE *fp;
	if ((fp = fopen(file_name, "w")) == NULL)
		return;
	PrintFile(item, 0, fp);
	fclose(fp);
}

/* Addition */
void printblank(int num,FILE *fp)
{
	int i;
	for (i = 1; i <= num; i++)
		fputc(' ', fp);
}
void PrintFile(JSON *item, int blank, FILE *fp)
{
	if (item == NULL)
		return;
	switch (item->type)
	{
		case JSON_FALSE:
			fprintf(fp, "false");
			break;
		case JSON_TRUE:
			fprintf(fp, "true");
			break;
		case JSON_NULL:
			fprintf(fp, "null");
			break;
		case JSON_NUMBER:
			fprintf(fp, "%s", NumberFormat(item->valuedouble));
			break;
		case JSON_STRING:
			fprintf(fp, "\"%s\"", item->valuestring);
			break;
		case JSON_ARRAY:
			fprintf(fp, "[\n");
			while (item->next != NULL)
			{
				printblank(blank + 4, fp);
				PrintFile(item->value, blank + 4, fp);
				fprintf(fp,",\n");
				item = item->next;
			}
			if (item->value != NULL)
			{
				printblank(blank + 4, fp);
				PrintFile(item->value, blank + 4, fp);
				fputc('\n', fp);
			}
			printblank(blank, fp);
			fputc(']', fp);
			break;
		case JSON_OBJECT:
			fprintf(fp,"{\n");
			while (item->next != NULL)
			{
				printblank(blank + 4, fp);
				fprintf(fp, "%c%s%c: ", 34, item->key, 34);
				PrintFile(item->value, blank + 4, fp);
				fprintf(fp, ",\n");
				item = item->next;
			}
			if (item->value != NULL)
			{
				printblank(blank + 4, fp);
				fprintf(fp, "%c%s%c: ", 34, item->key, 34);
				PrintFile(item->value, blank + 4, fp);
			}
			fputc('\n', fp);
			printblank(blank, fp);
			fputc('}',fp);
			break;
	}
}
JSON *ParseString(const char *value)
{
	char *string, *value_raw = value;
	int length = 0;
	while (*value != '"')
	{
		if (*value == '\\')
		{
			value++;
			length++;
		}
		value++;
		length++;
	}
	string = (char *)calloc(length + 1, sizeof(char));
	strncpy(string, value_raw, length);
	*(string + length) = '\0';
	return CreateString(string);
}
JSON *ParseNumber(const char *value)
{
	int t = 1;
	if (*value == '-')
	{
		value++;
		t = -1;
	}
	double number = 0;
	while (*value != '.'&&*value != '}'&&*value != ']'&&*value != ','&&*value != '\0')
	{
		number = number * 10 + (*value - '0');
		value++;
	}
	if (*value == '.')
	{
		double t = 0.1;
		value++;
		while (*value != '}'&&*value != ']'&&*value != ','&&*value != '\0')
		{
			number += (*value - '0')*t;
			t *= 0.1;
			value++;
		}
	}
	return CreateNumber(number*t);
}
char *ScanItem(char *value)
{

	while (*value == ' ' || *value == '\t')
		value++;
	if (*value == '[' || *value == '{')
	{
		int count = 1;/* The minus between the number of '[' & '{' and ']' & '}'. */
		value++;
		while (count >0 )
		{
			if (*value == '[' || *value == '{')
				count++;
			else if (*value == ']' || *value == '}')
				count--;
			value++;
		}
		while (*value == ' ')
			value++;
	}
	else
	{
		while (*value != ']'&&*value != '}'&&*value != ',')
			value++;
	}
	if (*value == ',')
		return value + 1;
	else
		return NULL;
}
char *ScanKey(char *value, char *key)
{
	while (*value == ' ' || *value == '\t')
		value++;
	if (*value == '\"')
	{
		value++;
		while (*value != '\"')
			*(key++) = *(value++);
		value++;
	}
	else
	{
		while (*value != ' ' && *value != ':')
		{
			*key = *value;
			key++;
			value++;
		}
	}
	*key = '\0';
	while (*value == ' ')
		value++;
	return value + 1;
}
char *NumberFormat(double num)
{
	char *string = (char *)calloc(NUMBER_LENGTH, sizeof(char)), *p_string = string;
	if (num == 0)
	{
		*(p_string++) = '0';
		*p_string = '\0';
		return string;
	}
	if (num < 0)
	{
		num = -num;
		*(p_string++) = '-';
	}
	int num_int;
	if (num >= 1)
	{
		num_int = (int)(num);
		int t = 1;
		while (t <= num_int)
			t *= 10;
		t /= 10;
		while (t >= 1)
		{
			*(p_string++) = (num_int / t) + '0';
			num_int -= (num_int / t) * t;
			t /= 10;
		}
		if (num == (int)(num))
		{
			*p_string = '\0';
			return string;
		}
		num -= (int)(num);
	}
	else 
		*(p_string++) = '0';
	*(p_string++) = '.';
	int number_length = 0;
	while (num > 0 && number_length < NUMBER_LENGTH_MAX)
	{
		num *= 10;
		num_int = (int)(num);
		if (num_int == 0)
			number_length++;
		else
			number_length = 0;
		*(p_string++) = num_int + '0';
		num -= num_int;
	}
	if (number_length >= NUMBER_LENGTH_MAX)
		p_string -= NUMBER_LENGTH_MAX;
	*p_string = '\0';
	return string;
}