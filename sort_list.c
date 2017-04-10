#include "utils.h"
#include "sort_list.h"

int count_list(const void *list)
{
    struct list_lite *pl = (struct list_lite *)list;
    int l;
    for ( l = 0; pl; l++ )
        pl = pl->next;
    return l;
}

int sort_list(void *plist, comp_func *func)
{
    struct list_lite **pp = (struct list_lite **)plist;
    struct list_lite *list = *pp;
    int l;
    l = count_list(list);
    // Too short to sort
    if ( l <= 1 )
        return 1;
    struct list_lite *el;
    struct list_lite **array;
    array = malloc(l * sizeof(struct list_lite*));
    int i;
    for ( el = list, i= 0; el != NULL; el = el->next, i++ )
        array[i] = el;
    
    qsort(array, l, sizeof(array[0]), func);

    *pp = array[0];
    for ( i =0; i < l-1; ++i ) {
        array[i]->next = array[i+1];
        array[i+1]->next = NULL;
    }
    free(array);
    return 0;      
}

int sort_rmdup_list(void *plist, comp_func *func, delete_func delete)
{
    struct list_lite **pp = (struct list_lite**)plist;
    struct list_lite *list = *pp;
    int l;
    l = count_list(list);
    if (l <= 1 )
        return 1;
    struct list_lite *el;
    struct list_lite **array;
    array = malloc(l*sizeof(struct list_lite*));
    int i;
    for ( el = list, i = 0; el != NULL; el = el->next, i++)
        array[i] = el;
    qsort(array, l, sizeof(array[0]), func);
    *pp = array[0];
    for ( el = *pp, i = 1; i < l; i++ ) {
        if ( func(array[i-1], array[i]) == 0 ) {
            delete(array[i]);
        } else {
            el->next = array[i];
            el = array[i];
            el->next = NULL;
        }
    }
    free(array);
    return 0;        
}
void list_lite_delete(void *plist, delete_func delete)
{
    struct list_lite **pp = (struct list_lite**)plist;
    struct list_lite *list = *pp;

    while ( *pp ) {
        list = *pp;
        *pp = list->next;
        delete(list);
    }
}