/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_framed_transport.h"
#include "erpc_message_buffer.h"
#include <cassert>
#include <cstdio>

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

FramedTransport::FramedTransport(void)
: Transport()
, m_crcImpl(NULL)
#if ERPC_THREADS
, m_sendLock()
, m_receiveLock()
#endif
{
}

FramedTransport::~FramedTransport(void) {}

void FramedTransport::setCrc16(Crc16 *crcImpl)
{
    assert(crcImpl);
    m_crcImpl = crcImpl;
}

erpc_status_t FramedTransport::receive(MessageBuffer *message)
{
    assert(m_crcImpl && "Uninitialized Crc16 object.");
    Header h;

    char buffer[50]; // for debug purpose
    {
#if ERPC_THREADS
        Mutex::Guard lock(m_receiveLock);
#endif
        // Receive header first.
        erpc_status_t ret = underlyingReceive((uint8_t *)&h, sizeof(h));
        if (ret != kErpcStatus_Success)
        {
	
            return ret;
        }
          //	    memset(buffer, 0, 50);
         //            sprintf(buffer, "mssiz %d msglen %d\r\n", h.m_messageSize, message->getLength());
         //underlyingSend((uint8_t *)buffer, strlen(buffer));
        // received size can't be larger then buffer length.
        if (h.m_messageSize > message->getLength())
        {
	    memset(buffer, 0, 50);
            sprintf(buffer, "less size\r\n");
            underlyingSend((uint8_t *)buffer, strlen(buffer));
            return kErpcStatus_ReceiveFailed;
        }
	//    memset(buffer, 0, 50);
          //  sprintf(buffer, "size %d\r\n", h.m_messageSize);
        //underlyingSend((uint8_t *)buffer, strlen(buffer));

        // Receive rest of the message now we know its size.
        ret = underlyingReceive(message->get(), h.m_messageSize);
        if (ret != kErpcStatus_Success)
        {
            return ret;
        }
    }

    // Verify CRC.
    uint16_t computedCrc = m_crcImpl->computeCRC16(message->get(), h.m_messageSize);
    if (computedCrc != h.m_crc)
    {
	    memset(buffer, 0, 50);
            sprintf(buffer, "crc fail\r\n");
underlyingSend((uint8_t *)buffer, strlen(buffer));
        return kErpcStatus_CrcCheckFailed;
    }
//	    memset(buffer, 0, 50);
  //          sprintf(buffer, "crc pass\r\n");
//underlyingSend((uint8_t *)buffer, strlen(buffer));

    message->setUsed(h.m_messageSize);
    return kErpcStatus_Success;
}

erpc_status_t FramedTransport::send(MessageBuffer *message)
{
    assert(m_crcImpl && "Uninitialized Crc16 object.");
#if ERPC_THREADS
    Mutex::Guard lock(m_sendLock);
#endif

    uint16_t messageLength = message->getUsed();

    // Send header first.
    Header h;
    h.m_messageSize = messageLength;
    h.m_crc = m_crcImpl->computeCRC16(message->get(), messageLength);
    erpc_status_t ret = underlyingSend((uint8_t *)&h, sizeof(h));
    if (ret != kErpcStatus_Success)
    {
        return ret;
    }

    // Send the rest of the message.
    return underlyingSend(message->get(), messageLength);
}
