#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

/* Event functions */

/**
 * Initializes an `Event` structure.
 *
 * Sets up an `Event` with the provided system, resource, status, priority, and amount.
 *
 * @param[out] event     Pointer to the `Event` to initialize.
 * @param[in]  system    Pointer to the `System` that generated the event.
 * @param[in]  resource  Pointer to the `Resource` associated with the event.
 * @param[in]  status    Status code representing the event type.
 * @param[in]  priority  Priority level of the event.
 * @param[in]  amount    Amount related to the event (e.g., resource amount).
 */
void event_init(Event *event, System *system, Resource *resource, int status, int priority, int amount) {
    //Initialize all the data
    event->system = system;
    event->resource = resource;
    event->status = status;
    event->priority = priority;
    event->amount = amount;
}

/* EventQueue functions */

/**
 * Initializes the `EventQueue`.
 *
 * Sets up the queue for use, initializing any necessary data (e.g., semaphores when threading).
 *
 * @param[out] queue  Pointer to the `EventQueue` to initialize.
 */
void event_queue_init(EventQueue *queue) {
    queue->head = NULL;
    queue->size = 0;
    sem_init(&queue->semaphore, 0, 1); // Initialize semaphore
}

/**
 * Cleans up the `EventQueue`.
 *
 * Frees any memory and resources associated with the `EventQueue`.
 * 
 * @param[in,out] queue  Pointer to the `EventQueue` to clean.
 */
void event_queue_clean(EventQueue *queue) {
    EventNode *curr = queue->head;
    EventNode *next;

    while (curr != NULL) {
        next = curr->next;  // Save next pointer before freeing
        free(curr);         // Free current node
        curr = next;        // Move to next node
    }

    sem_destroy(&queue->semaphore);  // Clean up semaphore
}

/**
 * Pushes an `Event` onto the `EventQueue`.
 *
 * Adds the event to the queue in a thread-safe manner, maintaining priority order (highest first).
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[in]     event  Pointer to the `Event` to push onto the queue.
 */
void event_queue_push(EventQueue *queue, const Event *event) {
    sem_wait(&queue->semaphore);  //Lock thread

    EventNode *newNode = (EventNode *)malloc(sizeof(EventNode));
    newNode->event = *event;
    newNode->next = NULL;

    if (queue->head == NULL) {
        queue->head = newNode; //Empty queue case
    } else {

        // Find insertion point based on event priority (higher priority first)
        EventNode *curr = queue->head;
        EventNode *prev = NULL;
        while (curr != NULL && curr->event.priority >= event->priority) {
            prev = curr;
            curr = curr->next;
        }

        if (prev == NULL) {  // Insert at head 
            newNode->next = queue->head;
            queue->head = newNode;
        } else {  // Insert in middle or tail
            newNode->next = curr;
            prev->next = newNode;
        }
    }

    queue->size++;  // Maintain size counter
    sem_post(&queue->semaphore);  // Release lock
}

/**
 * Pops an `Event` from the `EventQueue`.
 *
 * Removes the highest priority event from the queue in a thread-safe manner.
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[out]    event  Pointer to the `Event` structure to store the popped event.
 * @return               Non-zero if an event was successfully popped; zero otherwise.
 */
int event_queue_pop(EventQueue *queue, Event *event) {
    sem_wait(&queue->semaphore); // Lock semaphore

    
    if (queue->head == NULL) {
        sem_post(&queue->semaphore);
        return 0; // Return false if queue is empty
    }

    // Remove and return head
    EventNode *node = queue->head;
    *event = node->event;          // Copy event data to output
    queue->head = node->next;      // Update head pointer
    
    free(node);          
    queue->size--;                

    sem_post(&queue->semaphore);
    return 1;  // Return success
}