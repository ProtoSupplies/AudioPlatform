#if 0 // Test Version 240511-1
#ifndef RINGBUFARDUINO_H
#define RINGBUFARDUINO_H

#include <Arduino.h>

// Replace EXTMEM with your platform-specific keyword for external PSRAM allocation
#define EXTMEM PSRAM_XMEM

template <typename T>
class RingBufArduino {
public:
    // Constructor: Initializes the ring buffer with a specified length
    // Argument: len - The desired length of the ring buffer
    RingBufArduino(size_t len);

    // Destructor: Deallocates memory used by the ring buffer
    ~RingBufArduino();

    // Add an element to the ring buffer
    // Argument: val - The value to be added
    // Returns: True if the element was added successfully, false otherwise
    bool push(const T& val);

    // Remove an element from the ring buffer
    // Returns: The removed element, or a default value if the buffer is empty
    T pop();

    // Get the number of elements currently in the ring buffer
    // Returns: The number of elements in the ring buffer
    size_t size() const;

    // Clear the contents of the ring buffer
    void clear();

private:
    // Pointer to the EXTMEM array used to store the ring buffer data
    T* buffer;

    // Length of the ring buffer
    size_t bufferLen;

    // Index of the head (next element to be removed)
    size_t head;

    // Index of the tail (next available position for insertion)
    size_t tail;

    // Number of elements currently in the ring buffer
    size_t count;
};

#endif // RINGBUFARDUINO_H
#endif // Test Version 240511-1



#if 1 // Working Version 240509-1
/**
 * @file RingBufArduino.h
 * @brief This file contains the definition of the RingBufArduino class,
 *        which is a generic ring buffer data structure compatible with Arduino 2.0.
 */

#ifndef RINGBUFARDUINO_H
#define RINGBUFARDUINO_H

#include <Arduino.h>

/**
 * @class RingBufArduino
 * @brief A generic ring buffer data structure compatible with Arduino 2.0.
 *
 * The RingBufArduino class is a fixed-size circular buffer implementation that can store
 * elements of any data type. It provides constant-time operations for adding and
 * removing elements from the front and back of the buffer, as well as checking
 * the number of elements currently stored.
 *
 * @tparam T The data type of the elements to be stored in the ring buffer.
 */
template <typename T>
class RingBufArduino {
public:
    /**
     * @brief Constructs a RingBufArduino object with a specified size.
     *
     * @param size The maximum number of elements the ring buffer can hold.
     */
    RingBufArduino(size_t size);

    /**
     * @brief Destructor for the RingBufArduino class.
     */
    ~RingBufArduino();

    /**
     * @brief Adds an element to the back of the ring buffer.
     *
     * @param value The element to be added.
     * @return true if the element was successfully added, false otherwise.
     */
    bool enqueue(const T& value);

    /**
     * @brief Removes an element from the front of the ring buffer.
     *
     * @param value A reference to store the removed element.
     * @return true if an element was successfully removed, false otherwise.
     */
    bool dequeue(T& value);

    /**
     * @brief Returns the number of elements currently stored in the ring buffer.
     *
     * @return The number of elements in the ring buffer.
     */
    size_t size() const;

    /**
     * @brief Clears the contents of the ring buffer.
     */
    void clear();

private:
    T* buf_;            // Storage for the ring buffer
    size_t head_;       // Index of the front element
    size_t tail_;       // Index of the next free slot
    size_t count_;      // Number of elements currently in the buffer
    size_t max_size_;   // Maximum size of the ring buffer
};

#endif // RINGBUFARDUINO_H
#endif // Working Version 240509-1
