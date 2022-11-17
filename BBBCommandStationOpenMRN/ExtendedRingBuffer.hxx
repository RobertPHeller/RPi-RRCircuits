// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 25 09:42:21 2021
//  Last Modified : <221117.1652>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//! @copyright
//!    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
//!			51 Locke Hill Road
//!			Wendell, MA 01379-9728
//!
//!    This program is free software; you can redistribute it and/or modify
//!    it under the terms of the GNU General Public License as published by
//!    the Free Software Foundation; either version 2 of the License, or
//!    (at your option) any later version.
//!
//!    This program is distributed in the hope that it will be useful,
//!    but WITHOUT ANY WARRANTY; without even the implied warranty of
//!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//!    GNU General Public License for more details.
//!
//!    You should have received a copy of the GNU General Public License
//!    along with this program; if not, write to the Free Software
//!    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//! @file ExtendedRingBuffer.hxx
//! 
//!
//////////////////////////////////////////////////////////////////////////////

#ifndef __EXTENDEDRINGBUFFER_HXX
#define __EXTENDEDRINGBUFFER_HXX

/** Implements an extended ring buffer.  (Based on RingBuffer & 
    DevicedBuffer.)
 */
template <typename T> class ExtendedRingBuffer
{
public:
    /** Factory method to create a ring buffer instance.
     * @param size size in items for the ring buffer
     * @return the newly cleated ExtendedRingBuffer object.
     */
    static inline ExtendedRingBuffer *create(size_t size)
    {
        ExtendedRingBuffer *ring_buffer =
            (ExtendedRingBuffer*)malloc(sizeof(ExtendedRingBuffer) + (size * sizeof(T)));
        /* placement new allows for runtime ring buffer size */
        new (ring_buffer) ExtendedRingBuffer(size);
        
        return ring_buffer;
    }

    /** Destroy an existing ring buffer instance.
     */
    void destroy()
    {
        free(this);
    }

    /** Insert a number of items to the buffer.
     * @param buf reference to the first item to insert
     * @param items total number of items to insert
     * @return total number of items inserted
     */
    size_t put(const T *buf, size_t items)
    {
        /** @todo (Stuart Baker) significant optimization opportunity */
        size_t inserted = items < (_size - count) ? items : (_size - count);
        
        for (size_t i = 0; i < inserted; ++i)
        {
            data[writeIndex++] = buf[i];
            
            if (writeIndex == _size)
            {
                writeIndex = 0;
            }
        }
        
        count += inserted;
        return inserted;
    }
    
    /** remove a number of items from the buffer.
     * @param buf reference to the data removed
     * @param items total number of items to remove
     * @return total number of items removed
     */
    size_t get(T *buf, size_t items)
    {
        /** @todo (Stuart Baker) significant optimization opportunity */
        size_t removed = items < count ? items : count;
        
        for (size_t i = 0; i < removed; ++i)
        {
            buf[i] = data[readIndex++];
            
            if (readIndex == _size)
            {
                readIndex = 0;
            }
        }
        
        count -= removed;
        return removed;
    }
    
    /** Number of items in the buffer.
     * @return number of items in the buffer
     */
    size_t items()
    {
        return count;
    }
    
    /** Size of buffer in number of items.
     * @return size of buffer in number of items
     */
    size_t size()
    {
        return _size;
    }
    
    /** space left in buffer of buffer in number items.
     * @return space left in buffer in number of items
     */
    size_t space()
    {
        return _size - count;
    }
    
    /** Add a number of items to the buffer by advancing the writeIndex.
     * @param items total number of items to add
     * @return total number of items added
     */
    size_t advance(size_t items)
    {
        if (items > space())
        {
            items = space();
        }
        size_t added = items;
        count += items;
        if ((writeIndex + items) >= _size)
        {
            items -= (_size - writeIndex);
            writeIndex = 0;
        }
        writeIndex += items;

        return added;
    }
    
    /** Get a reference to the current location in the buffer for read.
     * @param buf location to store resulting reference
     * @return number of items in continuous memory.  May be less than total
     *         number of items in the buffer.
     */
    size_t data_read_pointer(T **buf)
    {
        size_t result = _size - readIndex;
        if (count < result)
        {
            result = count;
        }
        *buf = data + readIndex;
        return result;
    }

    /** Get a reference to the current location in the buffer for write.
     * @param buf location to store resulting reference
     * @return amount of space in continuous memory.  May be less than total
     *         amount of space avaiable.
     */
    size_t data_write_pointer(T **buf)
    {
        size_t result = _size - writeIndex;
        if (space() < result)
        {
            result = space();
        }
        *buf = data + writeIndex;
        return result;
    }
    
private:
    /** Constructor.
     * @param size size in bytes for the ring buffer
     */
    ExtendedRingBuffer(size_t size)
        : _size(size),
          count(0),
          readIndex(0),
          writeIndex(0)
    {
    }

    /** Default Constructor.
     */
    ExtendedRingBuffer();
    
    /** Default destructor.
     */
    ~ExtendedRingBuffer();

    DISALLOW_COPY_AND_ASSIGN(ExtendedRingBuffer);
    
    /** size in items of ring buffer */
    size_t _size;
    
    /** total number of items in ring buffer */
    size_t count;
    
    /** read index */
    size_t readIndex;
    
    /** write index */
    size_t writeIndex;
    
    /** ring buffer data */
    T data[];
};



#endif // __EXTENDEDRINGBUFFER_HXX

