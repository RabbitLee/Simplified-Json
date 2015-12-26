#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#include"JSON.h"

void TestSomeFuncs()
{
	/*JSON *true_1 = CreateBool(1);
	JSON *number_1 = CreateNumber(3.14159);
	JSON *string_1 = CreateString("School of Software Engineering");
	JSON *array_1 = CreateArray();
	JSON *array_2 = CreateArray();
	
	AddItemToArray(array_1, number_1);
	AddItemToArray(array_1, string_1);
	AddItemToArray(array_1, true_1);
	AddItemToArray(array_1, array_2);

	printf("array_1: ");
	PrintJSON(array_1);
	putchar('\n');

	JSON *object_1 = CreateObject(), *object_2 = CreateObject();
	AddItemToObject(object_1, "nothing serious", array_1);
	AddItemToObject(object_2, "hehe", array_2);
	AddItemToObject(object_1, "null", object_2);
	printf("object_1: ");
	PrintJSON(object_1);
	putchar('\n');*/

	/* Append */
	JSON *object = CreateObject();
	AddItemToObject(object, "name", CreateString("Li Yichao"));
	AddItemToObject(object, "sex", CreateString("male"));

	JSON *array = CreateArray();
	AddItemToArray(array, CreateString("Calculus"));
	AddItemToArray(array, CreateString("C Language"));
	AddItemToArray(array, CreateString("Linear Algebra"));

	AddItemToObject(object, "subject", array);

	JSON *object_2 = CreateObject();
	JSON *array_2 = CreateArray();
	AddItemToObject(object_2, "Xu Song", array_2);
	AddItemToArray(array_2, CreateString("Start From Here"));
	AddItemToArray(array_2, CreateString("Lemon Tree"));
	AddItemToObject(object_2, "Joanna Wang", array_2);

	AddItemToObject(object, "favourite singer", object_2);

	AddItemToObject(object, "Have a rest", CreateNULL());

	/*PrintJSON(object);
	putchar('\n');*/

	/* Update */
	ReplaceItemInObject(object, "name", CreateString("Rabbit Lee"));
	ReplaceItemInArray(array, 2, CreateString("Physics"));
	/*PrintJSON(array);
	putchar('\n');

	PrintJSON(object);
	putchar('\n');*/

	/* Remove/Delete */
	/*PrintJSON(DetachItemFromArray(array, 0));
	putchar('\n');
	PrintJSON(array);
	putchar('\n');*/

	/*PrintJSON(DetachItemFromObject(object, "favourite singer"));
	putchar('\n');
	PrintJSON(object);
	putchar('\n');*/
	/*PrintJSON(object);
	putchar('\n');*/

	/* Duplicate */
	object_2 = Duplicate(object, 0);
	/*DeleteItemFromObject(object, "Have a rest");
	PrintJSON(object_2);
	putchar('\n');*/

	/* Read */
	/*PrintJSON(GetItemInObject(object, "favourite singer"));
	putchar('\n');
	PrintJSON(GetItemInArray(array, 2));
	putchar('\n');*/
	//PrintJSON(GetItemInObject(object, "subject"));
	/*PrintJSON(object);
	putchar('\n');
	PrintJSON(GetItemInJSON(object, "/subject/2"));
	putchar('\n');	*/

	/*PrintJSON(GetItemInJSON(object, "/favourite singer/Joanna Wang"));
	putchar('\n');*/

	ReplaceItemInArray(GetItemInJSON(object_2, "/favourite singer/Joanna Wang"), 1, CreateString("Can't take my eyes off you"));
	ReplaceItemInObject(object_2, "name", CreateString("yclissetj"));
	/*PrintJSON(object);
	putchar('\n');
	PrintJSON(object_2);
	putchar('\n');*/


	/* Parse & Print */
	
	//PrintJSONToFile(object, "output.out");
	
	PrintJSONToFile(ParseJSONFromFile("input.in"), "output.out");
}

int main()
{
	//TestSomeFuncs();

	char name[10] = "ycli";
	strcpy(name, "ssetj");

	//JSON *string = CreateString("yclissetj");
	return 0;
}
