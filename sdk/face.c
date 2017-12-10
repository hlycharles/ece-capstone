/*
 * Code for Zynq Processing System
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "platform.h"
#include "xstatus.h"
#include "xil_types.h"
#include "xil_assert.h"
#include "xuartps_hw.h"
#include "xuartps.h"
#include "xil_printf.h"

#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"

#include "data.h"
#include "recognition.h"

#include <time.h>

/************************** Constant Definitions *****************************/

#define UART_BASEADDR XPAR_XUARTPS_0_BASEADDR
#define UART_CLOCK_HZ XPAR_XUARTPS_0_CLOCK_HZ

#define BUFFER_SIZE 16

/*  Device hardware build related constants. */

#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID

#define RX_INTR_ID		XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR
#define TX_INTR_ID		XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR

#define INTC_DEVICE_ID          XPAR_SCUGIC_SINGLE_DEVICE_ID

#define INTC		XScuGic
#define INTC_HANDLER	XScuGic_InterruptHandler

#define MEM_BASE_ADDR		0x01000000
#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)


/* Timeout loop counter for reset */
#define RESET_TIMEOUT_COUNTER	100000

/************************** Function Prototypes ******************************/
#ifndef DEBUG
extern void xil_printf(const char *format, ...);
#endif

//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int RunRecognition = 0;
volatile static int ResultAvailRecognition = 0;

int pixels[19200];

static void TxIntrHandler(void *Callback);
static void RxIntrHandler(void *Callback);

static int SetupIntrSystem(INTC * IntcInstancePtr,
			   XAxiDma * AxiDmaPtr, u16 TxIntrId, u16 RxIntrId);
static void DisableIntrSystem(INTC * IntcInstancePtr,
					u16 TxIntrId, u16 RxIntrId);

int processImage(u32 baseAddr, XAxiDma_Config *Config, int firstFlush);

XUartPs Uart_Ps;
u8 sendBuffer[BUFFER_SIZE];
void writeUart(char *str, int len, u32 baseAddr);
void writeUart_int(int num, u32 baseAddr);

/************************** Variable Definitions *****************************/
/*
 * Device instance definitions
 */

static XAxiDma AxiDma;		/* Instance of the XAxiDma */
static INTC Intc;	/* Instance of the Interrupt Controller */

/*
 * Flags interrupt handlers use to notify the application context the events.
 */
volatile int TxDone;
volatile int RxDone;
volatile int Error;

u32 *TxBufferPtr;
u32 *RxBufferPtr;

int Data[19200];

/*****************************************************************************/
/**
*
* Main function
*
* This function is the main entry of the interrupt test. It does the following:
*	Set up the output terminal if UART16550 is in the hardware build
*	Initialize the DMA engine
*	Set up Tx and Rx channels
*	Set up the interrupt system for the Tx and Rx interrupts
*	Submit a transfer
*	Wait for the transfer to finish
*	Check transfer status
*	Disable Tx and Rx interrupts
*	Print test status and exit
*
* @param	None
*
* @return
*		- XST_SUCCESS if example finishes successfully
*		- XST_FAILURE if example fails.
*
* @note		None.
*
******************************************************************************/

int main() {
	u8 recvChar;
	u32 cntrlRegister;

	int Status;
	XAxiDma_Config *Config;

	u32 baseAddr = UART_BASEADDR;

	cntrlRegister = XUartPs_ReadReg(baseAddr, XUARTPS_CR_OFFSET);
	XUartPs_WriteReg(baseAddr, XUARTPS_CR_OFFSET,
					  ((cntrlRegister & ~XUARTPS_CR_EN_DIS_MASK) |
					   XUARTPS_CR_TX_EN | XUARTPS_CR_RX_EN));

	Config = XAxiDma_LookupConfig(DMA_DEV_ID);
	if (!Config) {
		xil_printf("No config found for %d\r\n", DMA_DEV_ID);

		return XST_FAILURE;
	}

	/* Initialize DMA engine */
	Status = XAxiDma_CfgInitialize(&AxiDma, Config);

	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	if(XAxiDma_HasSg(&AxiDma)){
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

	/* Set up Interrupt system  */
	Status = SetupIntrSystem(&Intc, &AxiDma, TX_INTR_ID, RX_INTR_ID);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed intr setup\r\n");
		return XST_FAILURE;
	}

	/* Disable all interrupts before setup */
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
							XAXIDMA_DMA_TO_DEVICE);

	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						    XAXIDMA_DEVICE_TO_DMA);

	XAxiDma_IntrEnable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
					   XAXIDMA_DMA_TO_DEVICE);


	XAxiDma_IntrEnable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
					   XAXIDMA_DEVICE_TO_DMA);


	for (int i = 0; i < 120; i++) {
		for (int j = 0; j < 160; j++) {
			Data[i * 160 + j] = (int)(data[i][j]);
		}
	}

	int pixelCount = 0;
	// int currentPixel = 0;

	TxBufferPtr = (u32 *)TX_BUFFER_BASE;
	RxBufferPtr = (u32 *)RX_BUFFER_BASE;

	while (1) {

		while(!XUartPs_IsReceiveData(baseAddr));
		recvChar = XUartPs_ReadReg(baseAddr, XUARTPS_FIFO_OFFSET);

		TxBufferPtr[pixelCount] = recvChar;
		pixelCount++;

		if (pixelCount == 400) {
			pixelCount = 0;
			processImage(baseAddr, Config, 0);
		}
	}

	DisableIntrSystem(&Intc, TX_INTR_ID, RX_INTR_ID);

	return 0;
}

int processImage(u32 baseAddr, XAxiDma_Config *Config, int firstFlush)
{
	int Status;

	int ite;
	for (ite = 0; ite < 1; ite++) {

		/* Enable all interrupts */
		XAxiDma_IntrEnable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						   XAXIDMA_DMA_TO_DEVICE);


		XAxiDma_IntrEnable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						   XAXIDMA_DEVICE_TO_DMA);


		/* Initialize flags before start transfer test  */
		TxDone = 0;
		RxDone = 0;
		Error = 0;

		/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache is enabled */
		Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, 1600);

		Status = XAxiDma_SimpleTransfer(&AxiDma,(UINTPTR) TxBufferPtr, 1600, XAXIDMA_DMA_TO_DEVICE);
						if (Status != XST_SUCCESS) {
							xil_printf("Send failed\n");
							return XST_FAILURE;
						}

		Status = XAxiDma_SimpleTransfer(&AxiDma,(UINTPTR) RxBufferPtr, 20, XAXIDMA_DEVICE_TO_DMA);
		if (Status != XST_SUCCESS) {
			xil_printf("Receive failed\n");
			return XST_FAILURE;
		}

		/* Wait TX done and RX done */
		while ((TxDone == 0 || RxDone == 0) && (Error == 0)) {}

		/* Invalidate the DestBuffer before checking the data, in case the Data Cache is enabled */
		Xil_DCacheInvalidateRange((u32)RxBufferPtr, 20);

		for (int i = 0; i < 5; i++) {
			xil_printf("%d\n", RxBufferPtr[i]);
		}
		xil_printf("&");
	}


	return XST_SUCCESS;
}

/*****************************************************************************/
/*
*
* This is the DMA TX Interrupt handler function.
*
* It gets the interrupt status from the hardware, acknowledges it, and if any
* error happens, it resets the hardware. Otherwise, if a completion interrupt
* is present, then sets the TxDone.flag
*
* @param	Callback is a pointer to TX channel of the DMA engine.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void TxIntrHandler(void *Callback)
{

	u32 IrqStatus;
	int TimeOut;
	XAxiDma *AxiDmaInst = (XAxiDma *)Callback;

	/* Read pending interrupts */
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DMA_TO_DEVICE);

	/* Acknowledge pending interrupts */


	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DMA_TO_DEVICE);

	/*
	 * If no interrupt is asserted, we do not do anything
	 */
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {

		return;
	}

	/*
	 * If error interrupt is asserted, raise error flag, reset the
	 * hardware to recover from the error, and return with no further
	 * processing.
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK)) {

		Error = 1;

		/*
		 * Reset should never fail for transmit channel
		 */
		XAxiDma_Reset(AxiDmaInst);

		TimeOut = RESET_TIMEOUT_COUNTER;

		while (TimeOut) {
			if (XAxiDma_ResetIsDone(AxiDmaInst)) {
				break;
			}

			TimeOut -= 1;
		}

		return;
	}

	/*
	 * If Completion interrupt is asserted, then set the TxDone flag
	 */
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK)) {

		TxDone = 1;
	}
}

/*****************************************************************************/
/*
*
* This is the DMA RX interrupt handler function
*
* It gets the interrupt status from the hardware, acknowledges it, and if any
* error happens, it resets the hardware. Otherwise, if a completion interrupt
* is present, then it sets the RxDone flag.
*
* @param	Callback is a pointer to RX channel of the DMA engine.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void RxIntrHandler(void *Callback)
{
	u32 IrqStatus;
	int TimeOut;
	XAxiDma *AxiDmaInst = (XAxiDma *)Callback;

	/* Read pending interrupts */
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DEVICE_TO_DMA);

	/* Acknowledge pending interrupts */
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DEVICE_TO_DMA);

	/*
	 * If no interrupt is asserted, we do not do anything
	 */
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {
		return;
	}

	/*
	 * If error interrupt is asserted, raise error flag, reset the
	 * hardware to recover from the error, and return with no further
	 * processing.
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK)) {

		Error = 1;

		/* Reset could fail and hang
		 * NEED a way to handle this or do not call it??
		 */
		XAxiDma_Reset(AxiDmaInst);

		TimeOut = RESET_TIMEOUT_COUNTER;

		while (TimeOut) {
			if(XAxiDma_ResetIsDone(AxiDmaInst)) {
				break;
			}

			TimeOut -= 1;
		}

		return;
	}

	/*
	 * If completion interrupt is asserted, then set RxDone flag
	 */
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK)) {

		RxDone = 1;
	}
}

/*****************************************************************************/
/*
*
* This function setups the interrupt system so interrupts can occur for the
* DMA, it assumes INTC component exists in the hardware system.
*
* @param	IntcInstancePtr is a pointer to the instance of the INTC.
* @param	AxiDmaPtr is a pointer to the instance of the DMA engine
* @param	TxIntrId is the TX channel Interrupt ID.
* @param	RxIntrId is the RX channel Interrupt ID.
*
* @return
*		- XST_SUCCESS if successful,
*		- XST_FAILURE.if not succesful
*
* @note		None.
*
******************************************************************************/
static int SetupIntrSystem(INTC * IntcInstancePtr,
			   XAxiDma * AxiDmaPtr, u16 TxIntrId, u16 RxIntrId)
{
	int Status;

#ifdef XPAR_INTC_0_DEVICE_ID

	/* Initialize the interrupt controller and connect the ISRs */
	Status = XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed init intc\r\n");
		return XST_FAILURE;
	}

	Status = XIntc_Connect(IntcInstancePtr, TxIntrId,
			       (XInterruptHandler) TxIntrHandler, AxiDmaPtr);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed tx connect intc\r\n");
		return XST_FAILURE;
	}

	Status = XIntc_Connect(IntcInstancePtr, RxIntrId,
			       (XInterruptHandler) RxIntrHandler, AxiDmaPtr);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed rx connect intc\r\n");
		return XST_FAILURE;
	}

	/* Start the interrupt controller */
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed to start intc\r\n");
		return XST_FAILURE;
	}

	XIntc_Enable(IntcInstancePtr, TxIntrId);
	XIntc_Enable(IntcInstancePtr, RxIntrId);

#else

	XScuGic_Config *IntcConfig;


	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	XScuGic_SetPriorityTriggerType(IntcInstancePtr, TxIntrId, 0xA0, 0x3);

	XScuGic_SetPriorityTriggerType(IntcInstancePtr, RxIntrId, 0xA0, 0x3);
	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XScuGic_Connect(IntcInstancePtr, TxIntrId,
				(Xil_InterruptHandler)TxIntrHandler,
				AxiDmaPtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	Status = XScuGic_Connect(IntcInstancePtr, RxIntrId,
				(Xil_InterruptHandler)RxIntrHandler,
				AxiDmaPtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	XScuGic_Enable(IntcInstancePtr, TxIntrId);
	XScuGic_Enable(IntcInstancePtr, RxIntrId);


#endif

	/* Enable interrupts from the hardware */

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)INTC_HANDLER,
			(void *)IntcInstancePtr);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function disables the interrupts for DMA engine.
*
* @param	IntcInstancePtr is the pointer to the INTC component instance
* @param	TxIntrId is interrupt ID associated w/ DMA TX channel
* @param	RxIntrId is interrupt ID associated w/ DMA RX channel
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void DisableIntrSystem(INTC * IntcInstancePtr,
					u16 TxIntrId, u16 RxIntrId)
{
#ifdef XPAR_INTC_0_DEVICE_ID
	/* Disconnect the interrupts for the DMA TX and RX channels */
	XIntc_Disconnect(IntcInstancePtr, TxIntrId);
	XIntc_Disconnect(IntcInstancePtr, RxIntrId);
#else
	XScuGic_Disconnect(IntcInstancePtr, TxIntrId);
	XScuGic_Disconnect(IntcInstancePtr, RxIntrId);
#endif
}

void writeUart(char *str, int len, u32 baseAddr) {
	int i;
	for (i = 0; i < len; i++) {
		XUartPs_WriteReg(baseAddr, XUARTPS_FIFO_OFFSET, str[i]);
	}
}

void writeUart_int(int num, u32 baseAddr) {
	char digits[30];
	int count = 0;
	while (num >= 10) {
		char digit = (num % 10) + '0';
		digits[count] = digit;
		count++;
		num /= 10;
	}
	digits[count] = num + '0';
	while (count >= 0) {
		XUartPs_WriteReg(baseAddr, XUARTPS_FIFO_OFFSET, digits[count]);
		count--;
	}
}


