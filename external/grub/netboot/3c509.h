/*
 * Copyright (c) 1993 Herb Peyerl (hpeyerl@novatel.ca) All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer. 2. The name
 * of the author may not be used to endorse or promote products derived from
 * this software withough specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * if_epreg.h,v 1.4 1994/11/13 10:12:37 gibbs Exp Modified by:
 *
 October 2, 1994

 Modified by: Andres Vega Garcia

 INRIA - Sophia Antipolis, France
 e-mail: avega@sophia.inria.fr
 finger: avega@pax.inria.fr

 */


#define TX_INIT_RATE		16
#define TX_INIT_MAX_RATE	64
#define RX_INIT_LATENCY		64
#define RX_INIT_EARLY_THRESH	64
#define MIN_RX_EARLY_THRESHF	16	
#define MIN_RX_EARLY_THRESHL	4

#define EEPROMSIZE	0x40
#define MAX_EEPROMBUSY	1000
#define EP_LAST_TAG	0xd7
#define EP_MAX_BOARDS	16
#define EP_ID_PORT	0x100

#define IS_BASE (eth_nic_base)
#define BASE	(eth_nic_base)

#define EEPROM_CMD_RD	0x0080	
#define EEPROM_CMD_WR	0x0040	
#define EEPROM_CMD_ERASE 0x00c0	
#define EEPROM_CMD_EWEN	0x0030	

#define EEPROM_BUSY		(1<<15)
#define EEPROM_TST_MODE		(1<<14)

#define is_eeprom_busy(b) (inw((b)+EP_W0_EEPROM_COMMAND)&EEPROM_BUSY)
#define GO_WINDOW(x)	outw(WINDOW_SELECT|(x), BASE+EP_COMMAND)


#define EEPROM_NODE_ADDR_0	0x0	
#define EEPROM_NODE_ADDR_1	0x1	
#define EEPROM_NODE_ADDR_2	0x2	
#define EEPROM_PROD_ID		0x3	
#define EEPROM_MFG_ID		0x7	
#define EEPROM_ADDR_CFG		0x8	
#define EEPROM_RESOURCE_CFG	0x9	


#define EP_COMMAND		0x0e	
#define EP_STATUS		0x0e	
#define EP_WINDOW		0x0f	
#define EP_W0_EEPROM_DATA	0x0c
#define EP_W0_EEPROM_COMMAND	0x0a
#define EP_W0_RESOURCE_CFG	0x08
#define EP_W0_ADDRESS_CFG	0x06
#define EP_W0_CONFIG_CTRL	0x04
#define EP_W0_PRODUCT_ID	0x02
#define EP_W0_MFG_ID		0x00

#define EP_W1_TX_PIO_WR_2	0x02
#define EP_W1_TX_PIO_WR_1	0x00
#define EP_W1_FREE_TX		0x0c
#define EP_W1_TX_STATUS		0x0b	
#define EP_W1_TIMER		0x0a	
#define EP_W1_RX_STATUS		0x08
#define EP_W1_RX_PIO_RD_2	0x02
#define EP_W1_RX_PIO_RD_1	0x00

#define EP_W2_ADDR_5		0x05
#define EP_W2_ADDR_4		0x04
#define EP_W2_ADDR_3		0x03
#define EP_W2_ADDR_2		0x02
#define EP_W2_ADDR_1		0x01
#define EP_W2_ADDR_0		0x00

#define EP_W3_FREE_TX		0x0c
#define EP_W3_FREE_RX		0x0a

#define EP_W4_MEDIA_TYPE	0x0a
#define EP_W4_CTRLR_STATUS	0x08
#define EP_W4_NET_DIAG		0x06
#define EP_W4_FIFO_DIAG		0x04
#define EP_W4_HOST_DIAG		0x02
#define EP_W4_TX_DIAG		0x00

#define EP_W5_READ_0_MASK	0x0c
#define EP_W5_INTR_MASK		0x0a
#define EP_W5_RX_FILTER		0x08
#define EP_W5_RX_EARLY_THRESH	0x06
#define EP_W5_TX_AVAIL_THRESH	0x02
#define EP_W5_TX_START_THRESH	0x00

#define TX_TOTAL_OK		0x0c
#define RX_TOTAL_OK		0x0a
#define TX_DEFERRALS		0x08
#define RX_FRAMES_OK		0x07
#define TX_FRAMES_OK		0x06
#define RX_OVERRUNS		0x05
#define TX_COLLISIONS		0x04
#define TX_AFTER_1_COLLISION	0x03
#define TX_AFTER_X_COLLISIONS	0x02
#define TX_NO_SQE		0x01
#define TX_CD_LOST		0x00


#define GLOBAL_RESET		(unsigned short) 0x0000	
#define WINDOW_SELECT		(unsigned short) (0x1<<11)
#define START_TRANSCEIVER	(unsigned short) (0x2<<11)	
#define RX_DISABLE		(unsigned short) (0x3<<11)	
#define RX_ENABLE		(unsigned short) (0x4<<11)
#define RX_RESET		(unsigned short) (0x5<<11)
#define RX_DISCARD_TOP_PACK	(unsigned short) (0x8<<11)
#define TX_ENABLE		(unsigned short) (0x9<<11)
#define TX_DISABLE		(unsigned short) (0xa<<11)
#define TX_RESET		(unsigned short) (0xb<<11)
#define REQ_INTR		(unsigned short) (0xc<<11)
#define SET_INTR_MASK		(unsigned short) (0xe<<11)
#define SET_RD_0_MASK		(unsigned short) (0xf<<11)
#define SET_RX_FILTER		(unsigned short) (0x10<<11)
#define FIL_INDIVIDUAL	(unsigned short) (0x1)
#define FIL_GROUP		(unsigned short) (0x2)
#define FIL_BRDCST	(unsigned short) (0x4)
#define FIL_ALL		(unsigned short) (0x8)
#define SET_RX_EARLY_THRESH	(unsigned short) (0x11<<11)
#define SET_TX_AVAIL_THRESH	(unsigned short) (0x12<<11)
#define SET_TX_START_THRESH	(unsigned short) (0x13<<11)
#define STATS_ENABLE		(unsigned short) (0x15<<11)
#define STATS_DISABLE		(unsigned short) (0x16<<11)
#define STOP_TRANSCEIVER	(unsigned short) (0x17<<11)
#define ACK_INTR		(unsigned short) (0x6800)
#define C_INTR_LATCH	(unsigned short) (ACK_INTR|0x1)
#define C_CARD_FAILURE	(unsigned short) (ACK_INTR|0x2)
#define C_TX_COMPLETE	(unsigned short) (ACK_INTR|0x4)
#define C_TX_AVAIL	(unsigned short) (ACK_INTR|0x8)
#define C_RX_COMPLETE	(unsigned short) (ACK_INTR|0x10)
#define C_RX_EARLY	(unsigned short) (ACK_INTR|0x20)
#define C_INT_RQD		(unsigned short) (ACK_INTR|0x40)
#define C_UPD_STATS	(unsigned short) (ACK_INTR|0x80)

#define S_INTR_LATCH		(unsigned short) (0x1)
#define S_CARD_FAILURE		(unsigned short) (0x2)
#define S_TX_COMPLETE		(unsigned short) (0x4)
#define S_TX_AVAIL		(unsigned short) (0x8)
#define S_RX_COMPLETE		(unsigned short) (0x10)
#define S_RX_EARLY		(unsigned short) (0x20)
#define S_INT_RQD		(unsigned short) (0x40)
#define S_UPD_STATS		(unsigned short) (0x80)
#define S_5_INTS		(S_CARD_FAILURE|S_TX_COMPLETE|\
				 S_TX_AVAIL|S_RX_COMPLETE|S_RX_EARLY)
#define S_COMMAND_IN_PROGRESS	(unsigned short) (0x1000)

#define ERR_RX_INCOMPLETE	(unsigned short) (0x1<<15)
#define ERR_RX			(unsigned short) (0x1<<14)
#define ERR_RX_OVERRUN		(unsigned short) (0x8<<11)
#define ERR_RX_RUN_PKT		(unsigned short) (0xb<<11)
#define ERR_RX_ALIGN		(unsigned short) (0xc<<11)
#define ERR_RX_CRC		(unsigned short) (0xd<<11)
#define ERR_RX_OVERSIZE		(unsigned short) (0x9<<11)
#define ERR_RX_DRIBBLE		(unsigned short) (0x2<<11)

#define TXS_COMPLETE		0x80
#define TXS_SUCCES_INTR_REQ		0x40
#define TXS_JABBER		0x20
#define TXS_UNDERRUN		0x10
#define TXS_MAX_COLLISION	0x8
#define TXS_STATUS_OVERFLOW	0x4

#define IS_AUI				(1<<13)
#define IS_BNC				(1<<12)
#define IS_UTP				(1<<9)
#define ENABLE_DRQ_IRQ			0x0001
#define W0_P4_CMD_RESET_ADAPTER		0x4
#define W0_P4_CMD_ENABLE_ADAPTER	0x1
#define ENABLE_UTP			0xc0
#define DISABLE_UTP			0x0


#define SET_IRQ(i)	( ((i)<<12) | 0xF00) 


#define RX_BYTES_MASK			(unsigned short) (0x07ff)
#define RX_ERROR	0x4000
#define RX_INCOMPLETE	0x8000


#define ACTIVATE_ADAPTER_TO_CONFIG	0xff 
#define MFG_ID				0x6d50 
#define PROD_ID				0x9150

#define AUI				0x1
#define BNC				0x2
#define UTP				0x4

#define RX_BYTES_MASK			(unsigned short) (0x07ff)

 
#define EP_EISA_START			0x1000
#define EP_EISA_W0			0x0c80

#ifdef	INCLUDE_3C529
 
#define MCA_MOTHERBOARD_SETUP_REG	0x94
#define MCA_ADAPTER_SETUP_REG		0x96
#define MCA_MAX_SLOT_NR			8
#define MCA_POS_REG(n)			(0x100+(n))
#endif

