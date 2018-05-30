/*******************************************************************************
 * Copyright (C) Gallium Studio LLC. All rights reserved.
 *
 * This program is open source software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Alternatively, this program may be distributed and modified under the
 * terms of Gallium Studio LLC commercial licenses, which expressly supersede
 * the GNU General Public License and are specifically designed for licensees
 * interested in retaining the proprietary status of their code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact information:
 * Website - https://www.galliumstudio.com
 * Source repository - https://github.com/galliumstudio
 * Email - admin@galliumstudio.com
 ******************************************************************************/

/**
  ******************************************************************************
  * @file    UART/UART_Printf/Src/stm32f7xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    22-April-2016
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "fw_log.h"
#include "hsm_id.h"
#include "bsp.h"

using namespace FW;
using namespace APP;

// TX PA.9
// RX PB.7
// TX DMA - DMA2 Stream 7 Channel 4
// RX DMA - DMA2 Stream 5 Channel 4
static void InitUart1(UART_HandleTypeDef *usart) {
    /*
    (void)usart;
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    // 1- Enable peripherals and GPIO Clocks
    // Enable GPIO TX/RX clock
    __HAL_RCC_GPIOA_CLK_ENABLE();     // TX - PA9
    __HAL_RCC_GPIOB_CLK_ENABLE();     // RX - PB7
    // Select SysClk as source of USART1 clocks
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    RCC_PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    // Enable USARTx clock
    __HAL_RCC_USART1_CLK_ENABLE();

    // 2- Configure peripheral GPIO
    // UART TX GPIO pin configuration
    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    // UART RX GPIO pin configuration
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    */

    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    // 1- Enable peripherals and GPIO Clocks
    // Enable GPIO TX/RX clock
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    // Select SysClk as source of USART6 clocks
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    RCC_PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    // Enable USARTx clock
    __HAL_RCC_USART1_CLK_ENABLE();
    // Enable DMA clock
    __HAL_RCC_DMA2_CLK_ENABLE();

    // 2- Configure peripheral GPIO
    // UART TX GPIO pin configuration
    GPIO_InitStruct.Pin       = GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    // UART RX GPIO pin configuration
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //-3- Configure the DMA
    // Configure the DMA handler for Transmission process
    hdma_tx.Instance                 = DMA2_Stream7;
    hdma_tx.Init.Channel             = DMA_CHANNEL_4;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_tx);
    // Associate the initialized DMA handle to the UART handle
    __HAL_LINKDMA(usart, hdmatx, hdma_tx);
    // Configure the DMA handler for reception process
    hdma_rx.Instance                 = DMA2_Stream5;
    hdma_rx.Init.Channel             = DMA_CHANNEL_4;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_CIRCULAR;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_rx);
    // Associate the initialized DMA handle to the the UART handle
    __HAL_LINKDMA(usart, hdmarx, hdma_rx);

    // 4- Configure the NVIC for DMA
    // NVIC for DMA TX
    NVIC_SetPriority(DMA2_Stream7_IRQn, DMA2_STREAM7_PRIO);
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    // NVIC for DMA RX
    // Gallium - disabled for testing.
    //NVIC_SetPriority(DMA2_Stream5_IRQn, DMA2_STREAM5_PRIO);
    //NVIC_EnableIRQ(DMA2_Stream5_IRQn);
    // NVIC for USART
    NVIC_SetPriority(USART1_IRQn, USART1_IRQ_PRIO);
    NVIC_EnableIRQ(USART1_IRQn);

    //Log::Debug(Log::TYPE_LOG, UART6_ACT, "InitUart6 done");
}

static void DeInitUart1(UART_HandleTypeDef *usart) {
    (void)usart;
    // 1- Reset peripherals
    __HAL_RCC_USART1_FORCE_RESET();
    __HAL_RCC_USART1_RELEASE_RESET();

    // 2- Disable peripherals and GPIO Clocks
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
}

// TX PC.6
// RX PC.7
// TX DMA - DMA2 Stream 6 Channel 5
// RX DMA - DMA2 Stream 1 Channel 5
static void InitUart6(UART_HandleTypeDef *usart) {
    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    // 1- Enable peripherals and GPIO Clocks
    // Enable GPIO TX/RX clock
    __GPIOC_CLK_ENABLE();
    // Select SysClk as source of USART6 clocks
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART6;
    RCC_PeriphClkInit.Usart6ClockSelection = RCC_USART6CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    // Enable USARTx clock
    __HAL_RCC_USART6_CLK_ENABLE();
    // Enable DMA clock
    __HAL_RCC_DMA2_CLK_ENABLE();

    // 2- Configure peripheral GPIO
    // UART TX GPIO pin configuration
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    // UART RX GPIO pin configuration
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //-3- Configure the DMA
    // Configure the DMA handler for Transmission process
    hdma_tx.Instance                 = DMA2_Stream6;
    hdma_tx.Init.Channel             = DMA_CHANNEL_5;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_tx);
    // Associate the initialized DMA handle to the UART handle
    __HAL_LINKDMA(usart, hdmatx, hdma_tx);
    // Configure the DMA handler for reception process
    hdma_rx.Instance                 = DMA2_Stream1;
    hdma_rx.Init.Channel             = DMA_CHANNEL_5;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_CIRCULAR;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_rx);
    // Associate the initialized DMA handle to the the UART handle
    __HAL_LINKDMA(usart, hdmarx, hdma_rx);

    // 4- Configure the NVIC for DMA
    // NVIC for DMA TX
    // Gallium - disabled for testing.
    NVIC_SetPriority(DMA2_Stream6_IRQn, DMA2_STREAM6_PRIO);
    NVIC_EnableIRQ(DMA2_Stream6_IRQn);
    // NVIC for DMA RX
    //NVIC_SetPriority(DMA2_Stream1_IRQn, DMA2_STREAM1_PRIO);
    //NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    // NVIC for USART
    // Gallium - disabled for testing.
    NVIC_SetPriority(USART6_IRQn, USART6_IRQ_PRIO);
    NVIC_EnableIRQ(USART6_IRQn);

    //Log::Debug(Log::TYPE_LOG, UART6_ACT, "InitUart6 done");
}

static void DeInitUart6(UART_HandleTypeDef *usart) {
    (void)usart;
    // 1- Reset peripherals
    __HAL_RCC_USART6_FORCE_RESET();
    __HAL_RCC_USART6_RELEASE_RESET();

    // 2- Disable peripherals and GPIO Clocks
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);
}

extern "C" void HAL_UART_MspInit(UART_HandleTypeDef *usart)
{
    if (usart->Instance == USART1) {
        InitUart1(usart);
    } else if (usart->Instance == USART6) {
        InitUart6(usart);
    }
}

extern "C" void HAL_UART_MspDeInit(UART_HandleTypeDef *usart)
{
    if (usart->Instance == USART1) {
        DeInitUart1(usart);
    } else if (usart->Instance == USART6) {
        DeInitUart6(usart);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
