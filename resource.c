#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Resource functions */

/**
 * Creates a new `Resource` object.
 *
 * Allocates memory for a new `Resource` and initializes its fields.
 * The `name` is dynamically allocated.
 *
 * @param[out] resource      Pointer to the `Resource*` to be allocated and initialized.
 * @param[in]  name          Name of the resource (the string is copied).
 * @param[in]  amount        Initial amount of the resource.
 * @param[in]  max_capacity  Maximum capacity of the resource.
 */
void resource_create(Resource **resource, const char *name, int amount, int max_capacity) {
    
    //Allocate memory
    *resource = (Resource *)malloc(sizeof(Resource));
    (*resource)->name = (char *)malloc(strlen(name) + 1);

    //Initialize all the data
    strcpy((*resource)->name, name);
    (*resource)->amount = amount;
    (*resource)->max_capacity = max_capacity;

    sem_init(&(*resource)->semaphore, 0, 1); // Initialize semaphore
}

/**
 * Destroys a `Resource` object.
 *
 * Frees all memory associated with the `Resource`.
 *
 * @param[in,out] resource  Pointer to the `Resource` to be destroyed.
 */
void resource_destroy(Resource *resource) {
    sem_destroy(&resource->semaphore); // Destroy semaphore
    free(resource->name);
    free(resource);
}

/* ResourceAmount functions */

/**
 * Initializes a `ResourceAmount` structure.
 *
 * Associates a `Resource` with a specific `amount`.
 *
 * @param[out] resource_amount  Pointer to the `ResourceAmount` to initialize.
 * @param[in]  resource         Pointer to the `Resource`.
 * @param[in]  amount           The amount associated with the `Resource`.
 */
void resource_amount_init(ResourceAmount *resource_amount, Resource *resource, int amount) {
    resource_amount->resource = resource;
    resource_amount->amount = amount;
}

/**
 * Initializes the `ResourceArray`.
 *
 * Allocates memory for the array of `Resource*` pointers and sets initial values.
 *
 * @param[out] array  Pointer to the `ResourceArray` to initialize.
 */
void resource_array_init(ResourceArray *array) {
    array->resources = (Resource **)malloc(sizeof(Resource *));
    array->capacity = 1;
    array->size = 0;
}

/**
 * Cleans up the `ResourceArray` by destroying all resources and freeing memory.
 *
 * Iterates through the array, calls `resource_destroy` on each `Resource`,
 * and frees the array memory.
 *
 * @param[in,out] array  Pointer to the `ResourceArray` to clean.
 */
void resource_array_clean(ResourceArray *array) {
    for (int i = 0; i < array->size; i++) {
        resource_destroy(array->resources[i]);
    }

    free(array->resources);
}

/**
 * Adds a `Resource` to the `ResourceArray`, resizing if necessary (doubling the size).
 *
 * Resizes the array when the capacity is reached and adds the new `Resource`.
 * Use of realloc is NOT permitted.
 * 
 * @param[in,out] array     Pointer to the `ResourceArray`.
 * @param[in]     resource  Pointer to the `Resource` to add.
 */
void resource_array_add(ResourceArray *array, Resource *resource) {
    // Resize array if capacity is reached
    if (array->size >= array->capacity) {
        // Double the capacity
        int newCapacity = array->capacity * 2;
        Resource **newResource = (Resource **)malloc(sizeof(Resource *) * newCapacity);

        // Copy existing resources to new array
        for (int i = 0; i < array->size; i++) {
            newResource[i] = array->resources[i];
        }

        // Replace old array with new one
        free(array->resources);
        array->resources = newResource;
        array->capacity = newCapacity;
    }

    // Add new resource and increment size
    array->resources[array->size] = resource;
    array->size++;
}