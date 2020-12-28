/*
 * Copyright (c) 2020 SeeedStudio.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_arduino_uart_transport.h"

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

UartTransport::UartTransport(HardwareSerial *uartDrv, unsigned long baudrate)
    : m_uartDrv(uartDrv), m_baudrate(baudrate)
{
}

UartTransport::~UartTransport(void)
{
}

erpc_status_t UartTransport::init(void)
{

    m_uartDrv->begin(m_baudrate);
    return kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingReceive(uint8_t *data, uint32_t size)
{
    size_t bytesRead = 0;
    while (bytesRead < size)
    {
        while (!m_uartDrv->available()) vTaskDelay(1);

        const int c =  m_uartDrv->read();
        if (c < 0) continue;
        data[bytesRead++] = static_cast<uint8_t>(c);
    }
    return kErpcStatus_Success; // return size != bytesRead ? kErpcStatus_ReceiveFailed : kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
    const uint32_t bytesWritten = m_uartDrv->write(data, size);
    return size != bytesWritten ? kErpcStatus_SendFailed : kErpcStatus_Success;
}

bool UartTransport::hasMessage()
{
    const bool ret = m_uartDrv->available() > 0;
    return ret;
}
