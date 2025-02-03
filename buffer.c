#include "buffer.h"

// Define the external buffer instance
Buffer b = {.w = 0, .r = 0};

// Function to read from the buffer
Thread leggi(pthread_mutex_t* mutex) {
    Thread obj = {0, {0}}; // Initialize an empty Thread object

    pthread_mutex_lock(mutex);

    // Wait until the buffer is not empty
    while (b.w == b.r) {
        pthread_mutex_unlock(mutex);
        // Optionally, sleep or yield to avoid busy-waiting
        pthread_mutex_lock(mutex);
    }

    // Read the Thread object from the buffer
    obj = b.buffer[b.r];
    b.r = (b.r + 1) % DIM_BUFFER; // Increment the read index

    pthread_mutex_unlock(mutex);

    return obj;
}

// Function to write to the buffer
void scrivi(pthread_mutex_t* mutex, Thread boh) {
    pthread_mutex_lock(mutex);

    // Wait until the buffer is not full
    while ((b.w + 1) % DIM_BUFFER == b.r) {
        pthread_mutex_unlock(mutex);
        // Optionally, sleep or yield to avoid busy-waiting
        pthread_mutex_lock(mutex);
    }

    // Write the Thread object to the buffer
    b.buffer[b.w] = boh;
    b.w = (b.w + 1) % DIM_BUFFER; // Increment the write index

    pthread_mutex_unlock(mutex);
}