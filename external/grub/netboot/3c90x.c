/*
 * 3c90x.c -- This file implements the 3c90x driver for etherboot.  Written
 * by Greg Beeley, Greg.Beeley@LightSys.org.  Modified by Steve Smith,
 * Steve.Smith@Juno.Com
 *
 * This program Copyright (C) 1999 LightSys Technology Services, Inc.
 * Portions Copyright (C) 1999 Steve Smith
 *
 * This program may be re-distributed in source or binary form, modified,
 * sold, or copied for any purpose, provided that the above copyright message
 * and this text are included with all source copies or derivative works, and
 * provided that the above copyright message and this text are included in the
 * documentation of any binary-only distributions.  This program is distributed
 * WITHOUT ANY WARRANTY, without even the warranty of FITNESS FOR A PARTICULAR
 * PURPOSE or MERCHANTABILITY.  Please read the associated documentation
 * "3c90x.txt" before compiling and using this driver.
 *
 * --------
 *
 * Program written with the assistance of the 3com documentation for
 * the 3c905B-TX card, as well as with some assistance from the 3c59x
 * driver Donald Becker wrote for the Linux kernel, and with some assistance
 * from the remainder of the Etherboot distribution.
 *
 * REVISION HISTORY:
 *
 * v0.10	1-26-1998	GRB	Initial implementation.
 * v0.90	1-27-1998	GRB	System works.
 * v1.00pre1	2-11-1998	GRB	Got prom boot issue fixed.
 * v2.0		9-24-1999	SCS	Modified for 3c905 (from 3c905b code)
 *					Re-wrote poll and transmit for
 *					better error recovery and heavy
 *					network traffic operation
 *
 */

#include "etherboot.h"
#include "nic.h"
#include "pci.h"
#include "cards.h"
#include "timer.h"

#define	XCVR_MAGIC	(0x5A00)
#define	XMIT_RETRIES	250

#undef	virt_to_bus
#define	virt_to_bus(x)	((unsigned long)x)

enum Registers
    {
    regPowerMgmtCtrl_w = 0x7c,        
    regUpMaxBurst_w = 0x7a,           
    regDnMaxBurst_w = 0x78,           
    regDebugControl_w = 0x74,         
    regDebugData_l = 0x70,            
    regRealTimeCnt_l = 0x40,          
    regUpBurstThresh_b = 0x3e,        
    regUpPoll_b = 0x3d,               
    regUpPriorityThresh_b = 0x3c,     
    regUpListPtr_l = 0x38,            
    regCountdown_w = 0x36,            
    regFreeTimer_w = 0x34,            
    regUpPktStatus_l = 0x30,          
    regTxFreeThresh_b = 0x2f,         
    regDnPoll_b = 0x2d,               
    regDnPriorityThresh_b = 0x2c,     
    regDnBurstThresh_b = 0x2a,        
    regDnListPtr_l = 0x24,            
    regDmaCtrl_l = 0x20,              
                                      
    regIntStatusAuto_w = 0x1e,        
    regTxStatus_b = 0x1b,             
    regTimer_b = 0x1a,                
    regTxPktId_b = 0x18,              
    regCommandIntStatus_w = 0x0e,     
    };

enum Registers7
    {
    regPowerMgmtEvent_7_w = 0x0c,     
    regVlanEtherType_7_w = 0x04,      
    regVlanMask_7_w = 0x00,           
    };

enum Registers6
    {
    regBytesXmittedOk_6_w = 0x0c,     
    regBytesRcvdOk_6_w = 0x0a,        
    regUpperFramesOk_6_b = 0x09,      
    regFramesDeferred_6_b = 0x08,     
    regFramesRecdOk_6_b = 0x07,       
    regFramesXmittedOk_6_b = 0x06,    
    regRxOverruns_6_b = 0x05,         
    regLateCollisions_6_b = 0x04,     
    regSingleCollisions_6_b = 0x03,   
    regMultipleCollisions_6_b = 0x02, 
    regSqeErrors_6_b = 0x01,          
    regCarrierLost_6_b = 0x00,        
    };

enum Registers5
    {
    regIndicationEnable_5_w = 0x0c,   
    regInterruptEnable_5_w = 0x0a,    
    regTxReclaimThresh_5_b = 0x09,    
    regRxFilter_5_b = 0x08,           
    regRxEarlyThresh_5_w = 0x06,      
    regTxStartThresh_5_w = 0x00,      
    };

enum Registers4
    {
    regUpperBytesOk_4_b = 0x0d,       
    regBadSSD_4_b = 0x0c,             
    regMediaStatus_4_w = 0x0a,        
    regPhysicalMgmt_4_w = 0x08,       
    regNetworkDiagnostic_4_w = 0x06,  
    regFifoDiagnostic_4_w = 0x04,     
    regVcoDiagnostic_4_w = 0x02,      
    };

enum Registers3
    {
    regTxFree_3_w = 0x0c,             
    regRxFree_3_w = 0x0a,             
    regResetMediaOptions_3_w = 0x08,  
                                      
    regMacControl_3_w = 0x06,         
    regMaxPktSize_3_w = 0x04,         
    regInternalConfig_3_l = 0x00,     
                                      
    };

enum Registers2
    {
    regResetOptions_2_w = 0x0c,       
    regStationMask_2_3w = 0x06,       
    regStationAddress_2_3w = 0x00,    
    };

enum Registers1
    {
    regRxStatus_1_w = 0x0a,           
    };

enum Registers0
    {
    regEepromData_0_w = 0x0c,         
    regEepromCommand_0_w = 0x0a,      
    regBiosRomData_0_b = 0x08,        
    regBiosRomAddr_0_l = 0x04,        
    };


enum Windows
    {
    winPowerVlan7 = 0x07,
    winStatistics6 = 0x06,
    winTxRxControl5 = 0x05,
    winDiagnostics4 = 0x04,
    winTxRxOptions3 = 0x03,
    winAddressing2 = 0x02,
    winUnused1 = 0x01,
    winEepromBios0 = 0x00,
    };


enum Commands
    {
    cmdGlobalReset = 0x00,             
    cmdSelectRegisterWindow = 0x01,    
    cmdEnableDcConverter = 0x02,       
    cmdRxDisable = 0x03,               
    cmdRxEnable = 0x04,                
    cmdRxReset = 0x05,                 
    cmdStallCtl = 0x06,                
    cmdTxEnable = 0x09,                
    cmdTxDisable = 0x0A,               
    cmdTxReset = 0x0B,                 
    cmdRequestInterrupt = 0x0C,        
    cmdAcknowledgeInterrupt = 0x0D,    
    cmdSetInterruptEnable = 0x0E,      
    cmdSetIndicationEnable = 0x0F,     
    cmdSetRxFilter = 0x10,             
    cmdSetRxEarlyThresh = 0x11,        
    cmdSetTxStartThresh = 0x13,        
    cmdStatisticsEnable = 0x15,        
    cmdStatisticsDisable = 0x16,       
    cmdDisableDcConverter = 0x17,      
    cmdSetTxReclaimThresh = 0x18,      
    cmdSetHashFilterBit = 0x19,        
    };


#define	INT_INTERRUPTLATCH	(1<<0)
#define INT_HOSTERROR		(1<<1)
#define INT_TXCOMPLETE		(1<<2)
#define INT_RXCOMPLETE		(1<<4)
#define INT_RXEARLY		(1<<5)
#define INT_INTREQUESTED	(1<<6)
#define INT_UPDATESTATS		(1<<7)
#define INT_LINKEVENT		(1<<8)
#define INT_DNCOMPLETE		(1<<9)
#define INT_UPCOMPLETE		(1<<10)
#define INT_CMDINPROGRESS	(1<<12)
#define INT_WINDOWNUMBER	(7<<13)


typedef struct
    {
    unsigned int	DnNextPtr;
    unsigned int	FrameStartHeader;
    unsigned int	HdrAddr;
    unsigned int	HdrLength;
    unsigned int	DataAddr;
    unsigned int	DataLength;
    }
    TXD;

typedef struct
    {
    unsigned int	UpNextPtr;
    unsigned int	UpPktStatus;
    unsigned int	DataAddr;
    unsigned int	DataLength;
    }
    RXD;

static struct
    {
    unsigned char	isBrev;
    unsigned char	CurrentWindow;
    unsigned int	IOAddr;
    unsigned char	HWAddr[ETH_ALEN];
    TXD			TransmitDPD;
    RXD			ReceiveUPD;
    }
    INF_3C90X;


static int
a3c90x_internal_IssueCommand(int ioaddr, int cmd, int param)
    {
    unsigned int val;

	
	val = cmd;
	val <<= 11;
	val |= param;

	
	outw(val, ioaddr + regCommandIntStatus_w);

	
	while (inw(ioaddr + regCommandIntStatus_w) & INT_CMDINPROGRESS);

    return 0;
    }


static int
a3c90x_internal_SetWindow(int ioaddr, int window)
    {

	
	if (INF_3C90X.CurrentWindow == window) return 0;

	
	a3c90x_internal_IssueCommand(ioaddr, cmdSelectRegisterWindow, window);
	INF_3C90X.CurrentWindow = window;

    return 0;
    }


static unsigned short
a3c90x_internal_ReadEeprom(int ioaddr, int address)
    {
    unsigned short val;

	
        a3c90x_internal_SetWindow(INF_3C90X.IOAddr, winEepromBios0);

	
	while((1<<15) & inw(ioaddr + regEepromCommand_0_w));

	
	outw(address + ((0x02)<<6), ioaddr + regEepromCommand_0_w);
	while((1<<15) & inw(ioaddr + regEepromCommand_0_w));
	val = inw(ioaddr + regEepromData_0_w);

    return val;
    }


static int
a3c90x_internal_WriteEepromWord(int ioaddr, int address, unsigned short value)
    {
	
        a3c90x_internal_SetWindow(ioaddr, winEepromBios0);

	
	while((1<<15) & inw(ioaddr + regEepromCommand_0_w));

	
	outw(0x30, ioaddr + regEepromCommand_0_w);
	while((1<<15) & inw(ioaddr + regEepromCommand_0_w));

	
	outw(address + ((0x03)<<6), ioaddr + regEepromCommand_0_w);
	while((1<<15) & inw(ioaddr + regEepromCommand_0_w));

	
	outw(value, ioaddr + regEepromData_0_w);
	outw(0x30, ioaddr + regEepromCommand_0_w);
	while((1<<15) & inw(ioaddr + regEepromCommand_0_w));

	
	outw(address + ((0x01)<<6), ioaddr + regEepromCommand_0_w);
	while((1<<15) & inw(ioaddr + regEepromCommand_0_w));

    return 0;
    }


static int
a3c90x_internal_WriteEeprom(int ioaddr, int address, unsigned short value)
    {
    int cksum = 0,v;
    int i;
    int maxAddress, cksumAddress;

	if (INF_3C90X.isBrev)
	    {
	    maxAddress=0x1f;
	    cksumAddress=0x20;
	    }
	else
	    {
	    maxAddress=0x16;
	    cksumAddress=0x17;
	    }

	
	if (a3c90x_internal_WriteEepromWord(ioaddr, address, value) == -1)
	    return -1;

	
	for(i=0;i<=maxAddress;i++)
	    {
	    v = a3c90x_internal_ReadEeprom(ioaddr, i);
	    cksum ^= (v & 0xFF);
	    cksum ^= ((v>>8) & 0xFF);
	    }
	
	if (a3c90x_internal_WriteEepromWord(ioaddr, cksumAddress, cksum) == -1)
	    return -1;

    return 0;
    }



static void
a3c90x_reset(struct nic *nic)
    {
    int cfg;

#ifdef	CFG_3C90X_PRESERVE_XCVR
    
    a3c90x_internal_SetWindow(INF_3C90X.IOAddr, winTxRxOptions3);
    cfg = inl(INF_3C90X.IOAddr + regInternalConfig_3_l);
#endif

    
    printf("Issuing RESET:\n");
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdGlobalReset, 0);

    
    while (inw(INF_3C90X.IOAddr + regCommandIntStatus_w) & INT_CMDINPROGRESS);

    a3c90x_internal_SetWindow(INF_3C90X.IOAddr, winAddressing2);
    outw(0, INF_3C90X.IOAddr + regStationMask_2_3w+0);
    outw(0, INF_3C90X.IOAddr + regStationMask_2_3w+2);
    outw(0, INF_3C90X.IOAddr + regStationMask_2_3w+4);

#ifdef	CFG_3C90X_PRESERVE_XCVR
    
    a3c90x_internal_SetWindow(INF_3C90X.IOAddr, winTxRxOptions3);
    outl(cfg, INF_3C90X.IOAddr + regInternalConfig_3_l);

    
    if ((cfg&0x0300) == 0x0300)
	{
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdEnableDcConverter, 0);
	}
#endif

    
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdTxReset, 0);
    while (inw(INF_3C90X.IOAddr + regCommandIntStatus_w) & INT_CMDINPROGRESS)
	;
    if (! INF_3C90X.isBrev)
	outb(0x01, INF_3C90X.IOAddr + regTxFreeThresh_b);
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdTxEnable, 0);

    if (INF_3C90X.isBrev)
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdRxReset, 0x04);
    else
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdRxReset, 0x00);
    while (inw(INF_3C90X.IOAddr + regCommandIntStatus_w) & INT_CMDINPROGRESS);
	;
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdRxEnable, 0);

    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr,
                                 cmdSetInterruptEnable, 0);
    
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr,
                                 cmdSetIndicationEnable, 0x0014);
    
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr,
                                 cmdAcknowledgeInterrupt, 0x661);

    return;
    }



static void
a3c90x_transmit(struct nic *nic, const char *d, unsigned int t,
                unsigned int s, const char *p)
    {

    struct eth_hdr
	{
	unsigned char dst_addr[ETH_ALEN];
	unsigned char src_addr[ETH_ALEN];
	unsigned short type;
	} hdr;

    unsigned char status;
    unsigned i, retries;

    for (retries=0; retries < XMIT_RETRIES ; retries++)
	{
	
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdStallCtl, 2);

	
	inw(INF_3C90X.IOAddr + regCommandIntStatus_w);
	inw(INF_3C90X.IOAddr + regCommandIntStatus_w);

	while (inw(INF_3C90X.IOAddr+regCommandIntStatus_w) &
	       INT_CMDINPROGRESS)
	    ;

	
	hdr.type = htons(t);

	
	memcpy(hdr.dst_addr, d, ETH_ALEN);

	
	memcpy(hdr.src_addr, INF_3C90X.HWAddr, ETH_ALEN);

	
	INF_3C90X.TransmitDPD.DnNextPtr = 0;
	
	INF_3C90X.TransmitDPD.FrameStartHeader = (s + sizeof(hdr)) | 0x8000;
	INF_3C90X.TransmitDPD.HdrAddr = virt_to_bus(&hdr);
	INF_3C90X.TransmitDPD.HdrLength = sizeof(hdr);
	INF_3C90X.TransmitDPD.DataAddr = virt_to_bus(p);
	INF_3C90X.TransmitDPD.DataLength = s + (1<<31);

	
	outl(virt_to_bus(&(INF_3C90X.TransmitDPD)),
	     INF_3C90X.IOAddr + regDnListPtr_l);

	
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdStallCtl, 3);
	while(inl(INF_3C90X.IOAddr + regDnListPtr_l) != 0)
	    ;

	
	load_timer2(10*TICKS_PER_MS);	
	while (!(inw(INF_3C90X.IOAddr + regCommandIntStatus_w)&0x0004) &&
		timer2_running())
		;

	if (!(inw(INF_3C90X.IOAddr + regCommandIntStatus_w)&0x0004))
	    {
	    printf("3C90X: Tx Timeout\n");
	    continue;
	    }

	status = inb(INF_3C90X.IOAddr + regTxStatus_b);

	
	outb(0x00, INF_3C90X.IOAddr + regTxStatus_b);

	
	if ((status & 0xbf) == 0x80)
	    return;

	   printf("3C90X: Status (%hhX)\n", status);
	
	if (status & 0x02)
	    {
	    printf("3C90X: Tx Reclaim Error (%hhX)\n", status);
	    a3c90x_reset(NULL);
	    }
	else if (status & 0x04)
	    {
	    printf("3C90X: Tx Status Overflow (%hhX)\n", status);
	    for (i=0; i<32; i++)
		outb(0x00, INF_3C90X.IOAddr + regTxStatus_b);
	    
	    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdTxEnable, 0);
	    }
	else if (status & 0x08)
	    {
	    printf("3C90X: Tx Max Collisions (%hhX)\n", status);
	    
	    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdTxEnable, 0);
	    }
	else if (status & 0x10)
	    {
	    printf("3C90X: Tx Underrun (%hhX)\n", status);
	    a3c90x_reset(NULL);
	    }
	else if (status & 0x20)
	    {
	    printf("3C90X: Tx Jabber (%hhX)\n", status);
	    a3c90x_reset(NULL);
	    }
	else if ((status & 0x80) != 0x80)
	    {
	    printf("3C90X: Internal Error - Incomplete Transmission (%hhX)\n",
	           status);
	    a3c90x_reset(NULL);
	    }
	}

    
    printf("Failed to send after %d retries\n", retries);
    return;

    }



static int
a3c90x_poll(struct nic *nic)
    {
    int i, errcode;

    if (!(inw(INF_3C90X.IOAddr + regCommandIntStatus_w)&0x0010))
	{
	return 0;
	}


    
    INF_3C90X.ReceiveUPD.UpNextPtr = 0;
    INF_3C90X.ReceiveUPD.UpPktStatus = 0;
    INF_3C90X.ReceiveUPD.DataAddr = virt_to_bus(nic->packet);
    INF_3C90X.ReceiveUPD.DataLength = 1536 + (1<<31);

    
    outl(virt_to_bus(&(INF_3C90X.ReceiveUPD)),
         INF_3C90X.IOAddr + regUpListPtr_l);

    
    for(i=0;i<40000;i++);
    while((INF_3C90X.ReceiveUPD.UpPktStatus & ((1<<14) | (1<<15))) == 0)
	for(i=0;i<40000;i++);

    
    if (INF_3C90X.ReceiveUPD.UpPktStatus & (1<<14))
	{
	errcode = INF_3C90X.ReceiveUPD.UpPktStatus;
	if (errcode & (1<<16))
	    printf("3C90X: Rx Overrun (%hX)\n",errcode>>16);
	else if (errcode & (1<<17))
	    printf("3C90X: Runt Frame (%hX)\n",errcode>>16);
	else if (errcode & (1<<18))
	    printf("3C90X: Alignment Error (%hX)\n",errcode>>16);
	else if (errcode & (1<<19))
	    printf("3C90X: CRC Error (%hX)\n",errcode>>16);
	else if (errcode & (1<<20))
	    printf("3C90X: Oversized Frame (%hX)\n",errcode>>16);
	else
	    printf("3C90X: Packet error (%hX)\n",errcode>>16);
	return 0;
	}

    
    nic->packetlen = (INF_3C90X.ReceiveUPD.UpPktStatus & 0x1FFF);

    return 1;
    }



static void
a3c90x_disable(struct nic *nic)
    {
	
	outw(cmdRxDisable, INF_3C90X.IOAddr + regCommandIntStatus_w);
	outw(cmdTxDisable, INF_3C90X.IOAddr + regCommandIntStatus_w);
    }



struct nic*
a3c90x_probe(struct nic *nic, unsigned short *probeaddrs, struct pci_device *pci)
    {
    int i, c;
    unsigned short eeprom[0x21];
    unsigned int cfg;
    unsigned int mopt;
    unsigned short linktype;

    if (probeaddrs == 0 || probeaddrs[0] == 0)
          return 0;

    adjust_pci_device(pci);

    INF_3C90X.IOAddr = probeaddrs[0] & ~3;
    INF_3C90X.CurrentWindow = 255;
    switch (a3c90x_internal_ReadEeprom(INF_3C90X.IOAddr, 0x03))
	{
	case 0x9000: 
	case 0x9001: 
	case 0x9050: 
	case 0x9051: 
		INF_3C90X.isBrev = 0;
		break;

	case 0x9004: 
	case 0x9005: 
	case 0x9006: 
	case 0x900A: 
	case 0x9055: 
	case 0x9056: 
	case 0x905A: 
	default:
		INF_3C90X.isBrev = 1;
		break;
	}

    
    if (INF_3C90X.isBrev)
	{
	for(i=0;i<=0x20;i++)
	    {
	    eeprom[i] = a3c90x_internal_ReadEeprom(INF_3C90X.IOAddr, i);
	    }

#ifdef	CFG_3C90X_BOOTROM_FIX
	
	
	a3c90x_internal_WriteEeprom(INF_3C90X.IOAddr, 0x13, 0x0160);
#endif

#ifdef	CFG_3C90X_XCVR
	if (CFG_3C90X_XCVR == 255)
	    {
	    
	    a3c90x_internal_WriteEeprom(INF_3C90X.IOAddr, 0x16, 0);
	    }
	else
	    {
	    a3c90x_internal_WriteEeprom(INF_3C90X.IOAddr, 0x16,
	                    XCVR_MAGIC + ((CFG_3C90X_XCVR) & 0x000F));
	    }
#endif
	}
    else
	{
	for(i=0;i<=0x17;i++)
	    {
	    eeprom[i] = a3c90x_internal_ReadEeprom(INF_3C90X.IOAddr, i);
	    }
	}

    
    printf("\n\n3C90X Driver 2.00 "
           "Copyright 1999 LightSys Technology Services, Inc.\n"
           "Portions Copyright 1999 Steve Smith\n");
    printf("Provided with ABSOLUTELY NO WARRANTY.\n");
    printf("-------------------------------------------------------"
           "------------------------\n");

    
    INF_3C90X.HWAddr[0] = eeprom[0]>>8;
    INF_3C90X.HWAddr[1] = eeprom[0]&0xFF;
    INF_3C90X.HWAddr[2] = eeprom[1]>>8;
    INF_3C90X.HWAddr[3] = eeprom[1]&0xFF;
    INF_3C90X.HWAddr[4] = eeprom[2]>>8;
    INF_3C90X.HWAddr[5] = eeprom[2]&0xFF;
    printf("MAC Address = %!\n", INF_3C90X.HWAddr);

    
    a3c90x_internal_SetWindow(INF_3C90X.IOAddr, winAddressing2);
    outw(htons(eeprom[0]), INF_3C90X.IOAddr + regStationAddress_2_3w);
    outw(htons(eeprom[1]), INF_3C90X.IOAddr + regStationAddress_2_3w+2);
    outw(htons(eeprom[2]), INF_3C90X.IOAddr + regStationAddress_2_3w+4);
    outw(0, INF_3C90X.IOAddr + regStationMask_2_3w+0);
    outw(0, INF_3C90X.IOAddr + regStationMask_2_3w+2);
    outw(0, INF_3C90X.IOAddr + regStationMask_2_3w+4);

    
    for(i=0;i<ETH_ALEN;i++)
	nic->node_addr[i] = (eeprom[i/2] >> (8*((i&1)^1))) & 0xff;

    a3c90x_internal_SetWindow(INF_3C90X.IOAddr, winTxRxOptions3);
    mopt = inw(INF_3C90X.IOAddr + regResetMediaOptions_3_w);

    
    if (! INF_3C90X.isBrev)
	{
	mopt &= 0x7F;
	}

    printf("Connectors present: ");
    c = 0;
    linktype = 0x0008;
    if (mopt & 0x01)
	{
	printf("%s100Base-T4",(c++)?", ":"");
	linktype = 0x0006;
	}
    if (mopt & 0x04)
	{
	printf("%s100Base-FX",(c++)?", ":"");
	linktype = 0x0005;
	}
    if (mopt & 0x10)
	{
	printf("%s10Base-2",(c++)?", ":"");
	linktype = 0x0003;
	}
    if (mopt & 0x20)
	{
	printf("%sAUI",(c++)?", ":"");
	linktype = 0x0001;
	}
    if (mopt & 0x40)
	{
	printf("%sMII",(c++)?", ":"");
	linktype = 0x0006;
	}
    if ((mopt & 0xA) == 0xA)
	{
	printf("%s10Base-T / 100Base-TX",(c++)?", ":"");
	linktype = 0x0008;
	}
    else if ((mopt & 0xA) == 0x2)
	{
	printf("%s100Base-TX",(c++)?", ":"");
	linktype = 0x0008;
	}
    else if ((mopt & 0xA) == 0x8)
	{
	printf("%s10Base-T",(c++)?", ":"");
	linktype = 0x0008;
	}
    printf(".\n");

    if (INF_3C90X.isBrev)
	{
	if ((eeprom[0x16] & 0xFF00) == XCVR_MAGIC)
	    {
	    
	    linktype = eeprom[0x16] & 0x000F;
	    }
	}
    else
	{
#ifdef	CFG_3C90X_XCVR
	    if (CFG_3C90X_XCVR != 255)
		linktype = CFG_3C90X_XCVR;
#endif	

	    
	    if (linktype == 0x0009)
		{
		if (INF_3C90X.isBrev)
			printf("WARNING: MII External MAC Mode only supported on B-revision "
			       "cards!!!!\nFalling Back to MII Mode\n");
		linktype = 0x0006;
		}
	}

    
    if (linktype == 0x0003)
	{
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdEnableDcConverter, 0);
	}

    
    a3c90x_internal_SetWindow(INF_3C90X.IOAddr, winTxRxOptions3);
    cfg = inl(INF_3C90X.IOAddr + regInternalConfig_3_l);
    cfg &= ~(0xF<<20);
    cfg |= (linktype<<20);
    outl(cfg, INF_3C90X.IOAddr + regInternalConfig_3_l);

    
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdTxReset, 0x00);
    while (inw(INF_3C90X.IOAddr + regCommandIntStatus_w) & INT_CMDINPROGRESS)
	;

    if (!INF_3C90X.isBrev)
	outb(0x01, INF_3C90X.IOAddr + regTxFreeThresh_b);

    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdTxEnable, 0);

    if (INF_3C90X.isBrev)
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdRxReset, 0x04);
    else
	a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdRxReset, 0x00);
    while (inw(INF_3C90X.IOAddr + regCommandIntStatus_w) & INT_CMDINPROGRESS)
	;

    
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdSetRxFilter, 0x01 + 0x04);
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdRxEnable, 0);


    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr, cmdSetInterruptEnable, 0);
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr,
                                 cmdSetIndicationEnable, 0x0014);
    a3c90x_internal_IssueCommand(INF_3C90X.IOAddr,
                                 cmdAcknowledgeInterrupt, 0x661);

    
    nic->reset    = a3c90x_reset;
    nic->poll     = a3c90x_poll;
    nic->transmit = a3c90x_transmit;
    nic->disable  = a3c90x_disable;

    return nic;
    }


