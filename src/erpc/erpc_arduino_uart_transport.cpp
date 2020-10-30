/*
 * Copyright (c) 2020 SeeedStudio.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_arduino_uart_transport.h"

using namespace erpc;

bool EUart_hasMessage()
{
    if (Serial2.available() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

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
    uint8_t *temp = data;
    uint32_t times = 0;
    while (bytesRead < size)
    {
        while (m_uartDrv->available())
        {
            int c =  m_uartDrv->read();
            if (c < 0)
                break;
            *data++ = (char)c;
            bytesRead++;
            if(bytesRead == size)
                break;
        }
        // times++;
        // if(times % 100 == 0)
        //     printf("*");
        // delay(5);
    }
    // taskENTER_CRITICAL();
    // printf("underlyingReceive: size %d\n\rdata:\n\r", size);
    // for(int i = 0; i < bytesRead; i++)
    // {
    //     printf("0x%02x ", temp[i]);
    //     if(i % 10 == 0 && i != 0)
    //     {
    //         printf("\n\r");
    //     }
    // }
    // printf("\n\r");
    // taskEXIT_CRITICAL();
    return size != bytesRead ? kErpcStatus_ReceiveFailed : kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingSend(const uint8_t *data, uint32_t size)
{

    //taskENTER_CRITICAL();
    const uint8_t *temp = data;
    // printf("underlyingSend: size %d\n\rdata:\n\r", size);
    // for(int i = 0; i < size; i++)
    // {
    //     printf("0x%02x ", temp[i]);
    //     if(i % 10 == 0 && i != 0)
    //     {
    //         printf("\n\r");
    //     }
    // }
    // printf("\n\r");
 
    uint32_t bytesWritten = m_uartDrv->write(data, size);
    //taskEXIT_CRITICAL();
    return size != bytesWritten ? kErpcStatus_SendFailed : kErpcStatus_Success;
}

bool UartTransport::hasMessage()
{
    if (m_uartDrv->available())
    {
        return true;
    }
    return false;
}
