#include "map.h"

#include <stdlib.h>

typedef struct pair
{
    MapDataElement data;
    MapKeyElement key;
    struct pair *next;
} * Pair;

struct Map_t
{
    Pair list;
    Pair iterator;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};
static void freeList(Map map, Pair list)
{
    Pair pair_to_delete;
    while (list != NULL)
    {
        pair_to_delete = list;
        list = list->next;
        map->freeDataElement(pair_to_delete->data);
        map->freeKeyElement(pair_to_delete->key);
        free(pair_to_delete);
    }
}
static Pair copyPairList(Map map, Pair list)
{
    Pair new_pair_list = malloc(sizeof(*new_pair_list));
    if (new_pair_list == NULL)
    {
        return NULL;
    }
    Pair new_list_iterator = new_pair_list;
    while (list != NULL)
    {
        MapDataElement new_data = map->copyDataElement(list->data);
        if (new_data == NULL)
        {
            return NULL;
        }
        MapKeyElement new_key = map->copyKeyElement(list->key);
        if (new_key == NULL)
        {
            return NULL;
        }
        new_list_iterator->data = new_data;
        new_list_iterator->key = new_key;
        new_list_iterator->next = NULL;
        if (list->next != NULL)
        {
            Pair new_pair = malloc(sizeof(*new_pair));
            if (new_pair == NULL)
            {
                freeList(map, new_pair_list);
                return NULL;
            }
            new_list_iterator->next = new_pair;
        }
        new_list_iterator = new_list_iterator->next;
        list = list->next;
    }
    return new_pair_list;
}

static MapResult addNewPairToMap(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    Pair new_pair = malloc(sizeof(*new_pair));
    if (new_pair == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    MapDataElement new_data = map->copyDataElement(dataElement);
    if (new_data == NULL)
    {
        free(new_pair);
        return MAP_OUT_OF_MEMORY;
    }
    MapKeyElement new_key = map->copyKeyElement(keyElement);
    if (new_key == NULL)
    {
        map->freeDataElement(new_data);
        free(new_pair);
        return MAP_OUT_OF_MEMORY;
    }
    new_pair->data = new_data;
    new_pair->key = new_key;
    new_pair->next = NULL;

    // case list is empty
    if (map->list == NULL)
    {
        map->list = new_pair;
        return MAP_SUCCESS;
    }
    // case we need to add new pair to the beginning of the list
    if (map->compareKeyElements(keyElement, map->list->key) < 0)
    {
        new_pair->next = map->list;
        map->list = new_pair;
        return MAP_SUCCESS;
    }
    //otherwise:
    Pair hold_previous_pair = map->list;
    map->iterator = map->list->next;
    while (map->iterator != NULL)
    {
        if (map->compareKeyElements(keyElement, map->iterator->key) < 0)
        {
            new_pair->next = map->iterator;
            hold_previous_pair->next = new_pair;
            return MAP_SUCCESS;
        }
        hold_previous_pair = map->iterator;
        map->iterator = map->iterator->next;
    }
    //if we got here then the new key is "bigger" than all of list's keys, add it to the end of the list
    hold_previous_pair->next = new_pair;
    return MAP_SUCCESS;
}

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if (copyDataElement == NULL || copyKeyElement == NULL ||
        freeDataElement == NULL || freeKeyElement == NULL || compareKeyElements == NULL)
    {
        return NULL;
    }
    Map new_map = malloc(sizeof(*new_map));
    if (new_map == NULL)
    {
        return NULL;
    }
    new_map->list = NULL;
    new_map->iterator = NULL;
    new_map->copyDataElement = copyDataElement;
    new_map->copyKeyElement = copyKeyElement;
    new_map->freeDataElement = freeDataElement;
    new_map->freeKeyElement = freeKeyElement;
    new_map->compareKeyElements = compareKeyElements;
    return new_map;
}

void mapDestroy(Map map)
{
    if (map == NULL)
    {
        return;
    }
    freeList(map, map->list);
    free(map);
}

Map mapCopy(Map map)
{
    if (map == NULL)
    {
        return NULL;
    }
    Map map_copy = malloc(sizeof(*map_copy));
    if (map_copy == NULL)
    {
        return NULL;
    }

    Pair map_copy_list = copyPairList(map, map->list);
    if (map_copy_list == NULL)
    {
        free(map_copy);
        return NULL;
    }
    map_copy->list = map_copy_list;

    map_copy->copyDataElement = map->copyDataElement;
    map_copy->copyKeyElement = map->copyKeyElement;
    map_copy->freeDataElement = map->freeDataElement;
    map_copy->freeKeyElement = map->freeKeyElement;
    map_copy->compareKeyElements = map->compareKeyElements;
    map_copy->iterator = NULL;

    return map_copy;
}

int mapGetSize(Map map)
{
    if (map == NULL)
    {
        return -1;
    }
    int len = 0;
    Pair temp_list = map->list;
    while (temp_list != NULL)
    {
        len++;
        temp_list = temp_list->next;
    }
    return len;
}

bool mapContains(Map map, MapKeyElement element)
{
    if (map == NULL || element == NULL)
    {
        return false;
    }
    MAP_FOREACH(MapKeyElement, key, map)
    {
        if (map->compareKeyElements(element, key) == 0)
        {
            return true;
        }
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if (map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    // check if key exist and change its data's value
    MAP_FOREACH(MapKeyElement, key, map)
    {
        if (map->compareKeyElements(keyElement, key) == 0)
        {
            MapDataElement new_data = map->copyDataElement(dataElement);
            if (new_data == NULL)
            {
                return MAP_OUT_OF_MEMORY;
            }
            map->freeDataElement(map->iterator->data);
            map->iterator->data = new_data;
            return MAP_SUCCESS;
        }
    }
    // if we got here key doesnt exist in the map, find where to add the new key and its data
    return addNewPairToMap(map, keyElement, dataElement);
}

MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if (map == NULL || keyElement == NULL)
    {
        return NULL;
    }
    MAP_FOREACH(MapKeyElement, key, map)
    {
        if (map->compareKeyElements(keyElement, key) == 0)
        {
            return map->iterator->data;
        }
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    if (map == NULL || keyElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    // if found in first pair in the list
    if (map->compareKeyElements(keyElement, map->list->key) == 0)
    {
        Pair temp_pair = map->list->next;
        map->freeDataElement(map->list->data);
        map->freeKeyElement(map->list->key);
        map->list = temp_pair;
        return MAP_SUCCESS;
    }
    //otherwise:
    map->iterator = map->list;
    while (map->iterator->next != NULL)
    {
        if (map->compareKeyElements(keyElement, map->iterator->next->key) == 0)
        {
            Pair temp_pair = map->iterator->next->next;
            map->freeDataElement(map->iterator->next->data);
            map->freeKeyElement(map->iterator->next->key);
            map->iterator->next = temp_pair;
            return MAP_SUCCESS;
        }
        map->iterator = map->iterator->next;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map)
{
    if (map == NULL || map->list == NULL)
    {
        return NULL;
    }
    map->iterator = map->list;
    return map->iterator->key;
}

MapKeyElement mapGetNext(Map map)
{
    if (map == NULL || map->iterator->next == NULL) // if the iterator is at an invalid state?
    {
        return NULL;
    }
    map->iterator = map->iterator->next;
    return map->iterator->key;
}

MapResult mapClear(Map map)
{
    if (map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    freeList(map, map->list);
    return MAP_SUCCESS;
}