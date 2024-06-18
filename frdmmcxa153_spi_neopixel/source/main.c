/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpspi_edma.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif

#include "ws2812.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Master related */

#define LPSPI_MASTER_CLK_FREQ                 CLOCK_GetLpspiClkFreq(0u)
#define EXAMPLE_LPSPI_MASTER_BASEADDR         (LPSPI0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define EXAMPLE_LPSPI_MASTER_DMA_BASE         DMA0
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL   0U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL   1U
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL      kDma0RequestLPSPI0Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL       kDma0RequestLPSPI0Rx
#define TRANSFER_SIZE     64U     /* Transfer dataSize */
//#define TRANSFER_BAUDRATE 500000U /* Transfer baudrate - 500k */
#define TRANSFER_BAUDRATE 6000000U /* Transfer baudrate - 500k */


#define LED_STRIP_LENGTH        7
#define COLORS_PER_PIXEL        3
#define SPI_STRIP_BUF_LENGTH    LED_STRIP_LENGTH * COLORS_PER_PIXEL * 8 //8 represents the number of bits


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData);



/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_INIT(uint8_t masterRxData[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t masterTxData[TRANSFER_SIZE]) = {0};
AT_NONCACHEABLE_SECTION_INIT(lpspi_master_edma_handle_t g_m_edma_handle) = {0};

edma_handle_t lpspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t lpspiEdmaMasterTxDataToTxRegHandle;
edma_config_t userConfig = {0};

volatile bool isTransferCompleted  = false;

rgb_pixel_t led_strip [LED_STRIP_LENGTH];
uint8_t spi_ledstrip_data [SPI_STRIP_BUF_LENGTH];  

/*******************************************************************************
 * Code
 ******************************************************************************/

void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is LPSPI master edma transfer completed callback. \r\n\r\n");
    }

    isTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t srcClock_Hz;
    uint32_t loopCount = 1U;

    lpspi_master_config_t masterConfig;
    lpspi_transfer_t masterXfer;

    RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI0, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPSPI0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("LPSPI board to board edma example.\r\n");

    /* EDMA init*/
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASE, &userConfig);

    /*Master config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsToSckDelayInNanoSec        = 0;
    masterConfig.lastSckToPcsDelayInNanoSec    = 0;
    masterConfig.betweenTransferDelayInNanoSec = 0;
    
    srcClock_Hz = LPSPI_MASTER_CLK_FREQ;
    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    /*Set up lpspi master*/
    memset(&(lpspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(lpspiEdmaMasterRxRegToRxDataHandle));
    memset(&(lpspiEdmaMasterTxDataToTxRegHandle), 0, sizeof(lpspiEdmaMasterTxDataToTxRegHandle));

    EDMA_CreateHandle(&(lpspiEdmaMasterRxRegToRxDataHandle), EXAMPLE_LPSPI_MASTER_DMA_BASE, EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL);
    EDMA_CreateHandle(&(lpspiEdmaMasterTxDataToTxRegHandle), EXAMPLE_LPSPI_MASTER_DMA_BASE, EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL);
    EDMA_SetChannelMux(EXAMPLE_LPSPI_MASTER_DMA_BASE, EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL, DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL);
    EDMA_SetChannelMux(EXAMPLE_LPSPI_MASTER_DMA_BASE, EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL, DEMO_LPSPI_RECEIVE_EDMA_CHANNEL);

    LPSPI_MasterTransferCreateHandleEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, LPSPI_MasterUserCallback, NULL, &lpspiEdmaMasterRxRegToRxDataHandle, &lpspiEdmaMasterTxDataToTxRegHandle);
    LPSPI_MasterTransferPrepareEDMALite(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle,EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterByteSwap | kLPSPI_MasterPcsContinuous);

    led_strip[0] = (rgb_pixel_t){.r = 0xff, .g = 0x00, .b = 0x00};
    led_strip[1] = (rgb_pixel_t){.r = 0x00, .g = 0xff, .b = 0x00};
    led_strip[2] = (rgb_pixel_t){.r = 0x00, .g = 0x00, .b = 0xff};

    led_strip[3] = (rgb_pixel_t){.r = 0xff, .g = 0x00, .b = 0x00};
    led_strip[4] = (rgb_pixel_t){.r = 0x00, .g = 0xff, .b = 0x00};
    led_strip[5] = (rgb_pixel_t){.r = 0x00, .g = 0x00, .b = 0xff};
    led_strip[6] = (rgb_pixel_t){.r = 0xff, .g = 0xff, .b = 0xff};

    ws2812_serialize_strip(led_strip, LED_STRIP_LENGTH, spi_ledstrip_data);

    while (1)
    {
        /*Start master transfer*/
        masterXfer.txData   = spi_ledstrip_data;
        masterXfer.rxData   = NULL;
        masterXfer.dataSize = SPI_STRIP_BUF_LENGTH;
        isTransferCompleted = false;
        
        LPSPI_MasterTransferEDMALite(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle,&masterXfer);
        while (!isTransferCompleted);

        /* Wait for press any key */
        PRINTF("\r\n Press any key to run again\r\n");
        GETCHAR();
        /* Increase loop count to change transmit buffer */
        loopCount++;
    }
}
