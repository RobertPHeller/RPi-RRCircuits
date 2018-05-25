// -----------------------------------------------------------------------------
/*
 * Copyright (c) 2007 Fabian Greif, Roboterclub Aachen e.V.
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */
// -----------------------------------------------------------------------------

#ifndef	CAN_BUFFER_H
#define	CAN_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include "can.h"

// -----------------------------------------------------------------------------
typedef struct {
	tCAN *buf;
	uint8_t size;
	
	uint8_t used;
	uint8_t head;
	uint8_t tail;
} tCANBuffer;

// -----------------------------------------------------------------------------
extern void can_buffer_init(tCANBuffer *buf, uint8_t size, tCAN *list);

// -----------------------------------------------------------------------------
extern bool can_buffer_empty(tCANBuffer *buf);

// -----------------------------------------------------------------------------
extern bool can_buffer_full(tCANBuffer *buf);

// -----------------------------------------------------------------------------
extern tCAN *can_buffer_get_enqueue_ptr(tCANBuffer *buf);

// -----------------------------------------------------------------------------
/**
 * 
 * \warning	
 */
extern void can_buffer_enqueue(tCANBuffer *buf);

// -----------------------------------------------------------------------------
extern tCAN *can_buffer_get_dequeue_ptr(tCANBuffer *buf);

// -----------------------------------------------------------------------------
/**
 * 
 * \warning	
 */
extern void can_buffer_dequeue(tCANBuffer *buf);

#endif	// CAN_BUFFER_H
