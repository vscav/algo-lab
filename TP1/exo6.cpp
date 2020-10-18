#include <iostream>

using namespace std;

/** Structures definition */

typedef struct List{
    int value;
    struct List *next;
} List;

typedef struct DynaArray{
    int* array;
} DynaArray;

typedef struct File{
    struct FileElement* first;
} File;

typedef struct FileElement {
    int value;
    struct FileElement* next;
} FileElement;

typedef struct Pile{
    struct PileElement* first;
} Pile;

typedef struct PileElement {
    int value;
    struct PileElement* next;
} PileElement;

/** File functions */

File* initFile(int value)
{
    File *file = (File *)malloc(sizeof(File));
    FileElement *element = (FileElement *)malloc(sizeof(FileElement));
    if (file == NULL || file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    element->value = value;
    element->next = NULL;
    file->first = element;
    return file;
}

void enfile(File *file, int value)
{
    FileElement *element = (FileElement *) malloc(sizeof(FileElement));
    if (file == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }
    element->value = value;
    element->next = NULL;
    if (file->first != NULL)
    {
        FileElement *actualElement = file->first;
        while (actualElement->next != NULL)
        {
            actualElement = actualElement->next;
        }
        actualElement->next = element;
    }
    else file->first = element;
}

int defile(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    int number = 0;
    if (file->first != NULL)
    {
        FileElement *element = file->first;
        number = element->value;
        file->first = element->next;
        free(element);
    }
    return number;
}

int fileLength(File *file)
{
    FileElement *tmp = file->first;
    int length = 0;
    while (tmp != NULL)
    {
        length++;
        tmp = tmp->next;
    }
    return length;
}

void deleteFile(File* file, int length){
    for(int i = 0; i < length; i++){
        defile(file);
    }
    cout << "Free memory." << endl;
    cout << "\n";
}

void printFile(File* f)
{
    if (f == NULL)
    {
        exit(EXIT_FAILURE);
    }
    FileElement *actual = f->first;
    cout << "Les valeurs contenues dans le file sont : \n" << endl;
    while (actual != NULL)
    {
        cout << actual->value << endl;
        actual = actual->next;
    }
    cout << "\n" << endl;
}

/** Pile functions */

Pile* initPile(int value)
{
    Pile *pile = (Pile *)malloc(sizeof(Pile));
    PileElement *element = (PileElement *)malloc(sizeof(PileElement));
    if (pile == NULL || pile == NULL)
    {
        exit(EXIT_FAILURE);
    }
    element->value = value;
    element->next = NULL;
    pile->first = element;
    return pile;
}

void empile(Pile *pile, int value)
{
    PileElement *nouveau = (PileElement *) malloc(sizeof(PileElement));
    if (pile == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->value = value;
    nouveau->next = pile->first;
    pile->first = nouveau;
}

int depile(Pile *pile)
{
    if (pile == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int number = 0;
    PileElement *element = pile->first;

    if (pile != NULL && pile->first != NULL)
    {
        number = element->value;
        pile->first = element->next;
        free(element);
    }

    return number;
}

int pileLength(Pile *pile)
{
    PileElement *tmp = pile->first;
    int length = 0;
    while (tmp != NULL)
    {
        length++;
        tmp = tmp->next;
    }
    return length;
}

void deletePile(Pile* pile, int length){
    for(int i = 0; i < length; i++){
        depile(pile);
    }
    cout << "Free memory." << endl;
    cout << "\n";
}

void printPile(Pile* p)
{
    if (p == NULL)
    {
        exit(EXIT_FAILURE);
    }
    PileElement *actual = p->first;
    cout << "Les valeurs contenues dans la pile sont : \n" << endl;
    while (actual != NULL)
    {
        cout << actual->value << endl;
        actual = actual->next;
    }
    cout << "\n" << endl;
}

/** List functions */

List* initList(int value)
{
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL)
    {
        cout << "Memory allocation failed." << endl;
        exit(EXIT_FAILURE);
    } else cout << "Memory allocation : success." << endl;
    list->value = value;
    list->next = NULL;

    return list;
}

List* addList(List* l, int value)
{
    List *newList = (List*) malloc(sizeof(List));
    if (newList == NULL)
    {
        cout << "Memory allocation failed." << endl;
        exit(EXIT_FAILURE);
    } else cout << "Memory allocation : success." << endl;
    newList->value = value;
    newList->next = nullptr;
    if (l == NULL)
    {
        return newList;
    }
    else
    {
        List *tmp = l;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = newList;
        return l;
    }
}

int searchList_i(List* l, int index)
{
    for (int i = 0; i < index && l != NULL; i++)
    {
        l = l->next;
    }
    if (l == NULL)
        return (int) NULL;
    else
        return l->value;
}

int searchList(List* l, int value)
{
    if (l != NULL)
    {
        List* tmp = l;
        int index = 0;
        while (tmp->next != NULL)
        {
            if (tmp->value == value)
                return index;
            index += 1;
            tmp = tmp->next;
        }
        if (tmp->value == value) return index;
    }
    return -1;
}

void stockList(List* l, int n, int value)
{
    for (int i = 0; i < n-1 && l != NULL; i++)
    {
        l = l->next;
    }
    l->value = value;
}

void deleteList(List* list)
{
    List* current = list;
    List* next;
    while (NULL != current)
    {
        next = current->next;
        free(current);
        current = next;
    }
    list = NULL;
    cout << "Free memory." << endl;
    cout << "\n";
}

void printList(List* l)
{
    if (l == NULL)
    {
        exit(EXIT_FAILURE);
    }

    List* actualList = l;

    cout << "\nLes valeurs des cellules de la liste sont : \n"
         << endl;

    while (actualList != NULL)
    {
        cout << actualList->value << endl;
        actualList = actualList->next;
    }
    cout << "\n" << endl;
}

/** DynaArray functions */

DynaArray* initArray(int value, int* size){
    *size += 1;
    DynaArray *dynaArray = (DynaArray *)malloc(sizeof(DynaArray));
    if (dynaArray == NULL)
    {
        cout << "Memory allocation failed." << endl;
        exit(EXIT_FAILURE);
    } else cout << "Memory allocation : success." << endl;
    dynaArray->array = (int*) malloc(sizeof(int)*(*size));
    if (dynaArray->array == NULL)
    {
        cout << "Memory allocation failed." << endl;
        exit(EXIT_FAILURE);
    } else cout << "Memory allocation : success." << endl;
    dynaArray->array[*size - 1] = value;

    return dynaArray;
}

void addArray(DynaArray* array, int value, int *size)
{
    *size += 1;
    array->array = (int*) realloc(array->array, sizeof(int)*(*size));
    if (array->array == NULL)
    {
        cout << "Memory allocation failed." << endl;
        exit(EXIT_FAILURE);
    } else cout << "Memory allocation : success." << endl;
    array->array[*size-1] = value;
}

int searchArray_i(DynaArray* array, int index, int* size)
{
    for (int i = 0; i < *size; i++)
    {
        if(i == index) return array->array[i];
    }
    return -1;
}

int searchArray(DynaArray* array, int value, int* size)
{
    for (int i = 0; i < *size; i++)
    {
        if(array->array[i] == value) return i;
    }
    return -1;
}

void stockArray(DynaArray* array, int value, int index, int* size)
{
    for (int i = 0; i < *size; i++)
    {
        if(i == index)
        {
            array->array[i] = value;
        }
    }
}

void deleteArray(DynaArray* array)
{
    free(array->array);
    array->array = NULL;
    cout << "Free memory." << endl;
    cout << "\n";
}

void printArray(DynaArray* array, int *size)
{
    if (array == NULL)
    {
        exit(EXIT_FAILURE);
    }
    cout << "Les valeurs contenues dans le tableau sont : \n" << endl;
    for(int i = 0; i < *size; i++)
    {
        cout << array->array[i] << endl;
    }
    cout << "\n" << endl;
}

/** Main function : cases testing */

int main()
{
    /* Tests result variable */

    int result;

    /* Dynamic array : size variable initialized with 0 */

    int size = 0;

    /** Pile functions test */

    /* Initialize the pile a specific value */

    Pile *pile = initPile(4);

    /* Add values */

    empile(pile, 8);
    empile(pile, 15);
    empile(pile, 16);
    empile(pile, 23);
    empile(pile, 42);

    /* Print */

    printPile(pile);

    /* Depile (1) */

    result = depile(pile);
    cout << result << " a été dépilé." << endl;

    /* Depile (2) */

    result = depile(pile);
    cout << result << " a été dépilé.\n" << endl;

    /* Print */

    printPile(pile);

    /* Free memory */

    deletePile(pile, pileLength(pile));

    /** File functions test */

    /* Initialize the file with a specific value */

    File *file = initFile(4);

    /* Add values */

    enfile(file, 8);
    enfile(file, 15);
    enfile(file, 16);
    enfile(file, 23);
    enfile(file, 42);

    /* Print */

    printFile(file);

    /* Defile (1) */

    result = defile(file);
    cout << result << " a été défilé." << endl;

    /* Defile (2) */

    result = defile(file);
    cout << result << " a été défilé.\n" << endl;

    /* Print */

    printFile(file);

    /* Free memory */

    deleteFile(file, fileLength(file));

    /** List functions test */

    /* Initialize the list with a specific value */

    List *list = initList(10);

    /* Add values */

    list = addList(list, 9);
    list = addList(list, 12);

    /* Search by index */

    result = searchList_i(list, 0);
    cout << "\nValeur de l'élement à l'indice 0 : " << result << "\n"<< endl;

    /* Search (-1 : the value does not exist) */

    result = searchList(list, 10);
    cout << "\nIndice de l'élement à la valeur égale à 4 : " << result << "\n"<< endl;

    result = searchList(list, 9);
    cout << "\nIndice de l'élement à la valeur égale à 8 : " << result << "\n"<< endl;

    result = searchList(list, 12);
    cout << "\nIndice de l'élement à la valeur égale à 13 : " << result << "\n"<< endl;

    result = searchList(list, 8);
    cout << "\nIndice de l'élement à la valeur égale à 8 : " << result << "\n"<< endl;

    /* Stock at a specific index */

    stockList(list->next, 1, 11);

    /* Print */

    printList(list);

    /* Free memory */

    deleteList(list);

    /** Dyanmic array functions test */

    /* Initialize the dynamic array with a specific value */

    DynaArray *array = initArray(108, &size);

    /* Add values */

    addArray(array, 97, &size);
    addArray(array, 111, &size);
    addArray(array, 32, &size);
    addArray(array, 73, &size);

    /* Search by index */

    result = searchArray_i(array, 0, &size);
    cout << "\nValeur de l'élement à l'indice 0 : " << result << "\n"<< endl;

    /* Search (-1 : the value does not exist) */

    result = searchArray(array, 97, &size);
    cout << "\nIndice de l'élement à la valeur égale à 97 : " << result << "\n"<< endl;

    result = searchArray(array, 111, &size);
    cout << "\nIndice de l'élement à la valeur égale à 111 : " << result << "\n"<< endl;

    result = searchArray(array, 32, &size);
    cout << "\nIndice de l'élement à la valeur égale à 32 : " << result << "\n"<< endl;

    result = searchArray(array, 76, &size);
    cout << "\nIndice de l'élement à la valeur égale à 76 : " << result << "\n"<< endl;

    /* Stock at a specific index */

    stockArray(array, 86, 2, &size);

    /* Print */

    printArray(array, &size);

    /* Free memory */

    deleteArray(array);

    return 0;
}
