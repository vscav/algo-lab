#include <stdio.h>

#define ARRAY_SIZE 40
int hash(char* string)
{
	int i = 0;
	int hash_value = 0;
	while(string[i] != '\0')
	{
		hash_value += (int)string[i];
		i++;
	}
	return hash_value % ARRAY_SIZE;
}

int main()
{
	char* hash_table[ARRAY_SIZE];
	char* my_element = "Yolo\0";
	hash_table[hash(my_element)] = my_element;
	printf("Content of hash_table at %d is %s\n", hash(my_element), my_element);
	// Content of hash_table at 19 is Yolo
}
