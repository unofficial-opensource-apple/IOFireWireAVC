/*
 * Copyright (c) 1998-2001 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_IOFIREWIREAVCUNIT_H
#define _IOKIT_IOFIREWIREAVCUNIT_H

#include <IOKit/IOService.h>
#include <IOKit/firewire/IOFWRegs.h>
#include <IOKit/firewire/IOFWAddressSpace.h>
#include "IOFireWireAVCConsts.h"

extern const OSSymbol *gIOAVCUnitType;

class IOFireWireNub;
class IOFireWireAVCCommand;
class IOFireWirePCRSpace;

/*!
    @class IOFireWireAVCNub
    @abstract nub for AVC devices
*/
class IOFireWireAVCNub : public IOService
{
    OSDeclareDefaultStructors(IOFireWireAVCNub)

protected:
    IOFireWireNub * fDevice;

public:
    // execute AVC command
/*!
    @function AVCCommand
    @abstract Sends an AVC command to the device and stores the response.
    @param command Pointer to command to send.
    @param cmdLen Length of the command.
    @param response Pointer to place to store the response.
    @param responseLen Pointer to response length - initialize to the size of the buffer pointed to by response,
    updated to the number of bytes returned by the device.
*/
    virtual IOReturn AVCCommand(const UInt8 * command, UInt32 cmdLen, UInt8 * response, UInt32 *responseLen) = 0;

/*!
    @function AVCCommandInGeneration
    @abstract Sends an AVC command to the device and stores the response. The command must complete in the specified
    FireWire bus generation otherwise kIOFireWireBusReset is returned.
    @param generation The bus generation that the command must execute in.
    @param command Pointer to command to send.
    @param cmdLen Length of the command.
    @param response Pointer to place to store the response.
    @param responseLen Pointer to response length - initialize to the size of the buffer pointed to by response,
    updated to the number of bytes returned by the device.
*/
    virtual IOReturn AVCCommandInGeneration(UInt32 generation,
                const UInt8 * command, UInt32 cmdLen, UInt8 * response, UInt32 *responseLen) = 0;

/*!
    @function getDevice
    @abstract Returns the FireWire device nub that is this object's provider .
*/
    IOFireWireNub* getDevice() const
        {return fDevice;};

/*!
    @function updateAVCCommandTimeout
    @abstract By default, AVCCommands timeout 10 seconds after receiving an Interim response.
    This function resets the timeout of the current command to 10 seconds from the current time.
    Call this repeatedly for AVC commands that take a very long time to execute to prevent premature
    timeout.
*/
    virtual IOReturn updateAVCCommandTimeout() = 0;
    
private:
    OSMetaClassDeclareReservedUsed(IOFireWireAVCNub, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCNub, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCNub, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCNub, 3);

};

/*!
    @class IOFireWireAVCUnit
    @abstract nub for AVC devices
*/
class IOFireWireAVCUnit : public IOFireWireAVCNub
{
    OSDeclareDefaultStructors(IOFireWireAVCUnit)

protected:
    IOFWPseudoAddressSpace *fFCPResponseSpace;
    IOLock *avcLock;
    IOFireWireAVCCommand *fCommand;

    UInt8 fSubUnitCount[kAVCNumSubUnitTypes];
    
	bool fStarted;
    IOLock *cmdLock;
	
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;
    
    static UInt32 AVCResponse(void *refcon, UInt16 nodeID, IOFWSpeed &speed,
                    FWAddress addr, UInt32 len, const void *buf, IOFWRequestRefCon requestRefcon);

    static void rescanSubUnits(void *arg);
    
    virtual void free(void);
    
    virtual void updateSubUnits(bool firstTime);
    
public:
    // IOService overrides
    virtual bool start(IOService *provider);
    virtual IOReturn message(UInt32 type, IOService *provider, void *argument);

/*! @function handleOpen
    @abstract Overrideable method to control the open / close behaviour of an IOService.
    @discussion See IOService for discussion.		
    @param forClient Designates the client of the provider requesting the open.
    @param options Options for the open, may be interpreted by the implementor of handleOpen.
    @result Return true if the open was successful, false otherwise.
*/

    virtual bool handleOpen(  IOService *	  forClient,
                              IOOptionBits	  options,
                              void *		  arg );
/*! 
    @function handleClose
    @abstract Overrideable method to control the open / close behaviour of an IOService.
    @discussion See IOService for discussion.
    @param forClient Designates the client of the provider requesting the close.
    @param options Options for the close, may be interpreted by the implementor of handleOpen. 
*/

    virtual void handleClose(   IOService *		forClient,
                                IOOptionBits	options );
    

/*!
    @function matchPropertyTable
    @abstract Matching language support
	Match on the following properties of the unit:
	Vendor_ID
	GUID
	Unit_Type
	and available sub-units, match if the device has at least the requested number of a sub-unit type:
	AVCSubUnit_0 -> AVCSubUnit_1f
*/
    virtual bool matchPropertyTable(OSDictionary * table);

    // execute AVC command
/*!
    @function AVCCommand
    @abstract Sends an AVC command to the device and stores the response.
    @param command Pointer to command to send.
    @param cmdLen Length of the command.
    @param response Pointer to place to store the response.
    @param responseLen Pointer to response length - initialize to the size of the buffer pointed to by response,
    updated to the number of bytes returned by the device.
*/
    virtual IOReturn AVCCommand(const UInt8 * command, UInt32 cmdLen, UInt8 * response, UInt32 *responseLen);

/*!
    @function AVCCommandInGeneration
    @abstract Sends an AVC command to the device and stores the response. The command must complete in the specified
    FireWire bus generation otherwise kIOFireWireBusReset is returned.
    @param generation The bus generation that the command must execute in.
    @param command Pointer to command to send.
    @param cmdLen Length of the command.
    @param response Pointer to place to store the response.
    @param responseLen Pointer to response length - initialize to the size of the buffer pointed to by response,
    updated to the number of bytes returned by the device.
*/
    virtual IOReturn AVCCommandInGeneration(UInt32 generation,
                const UInt8 * command, UInt32 cmdLen, UInt8 * response, UInt32 *responseLen);

/*!
    @function updateAVCCommandTimeout
    @abstract By default, AVCCommands timeout 10 seconds after receiving an Interim response.
    This function resets the timeout of the current command to 10 seconds from the current time.
    Call this repeatedly for AVC commands that take a very long time to execute to prevent premature
    timeout.
*/
    virtual IOReturn updateAVCCommandTimeout();

private:
    OSMetaClassDeclareReservedUnused(IOFireWireAVCUnit, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCUnit, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCUnit, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCUnit, 3);

};

/*!
    @class IOFireWireAVCSubUnit
    @abstract nub for sub unit of AVC devices. Just for matching, calls the AVC unit for all functions.
*/
class IOFireWireAVCSubUnit : public IOFireWireAVCNub
{
    OSDeclareDefaultStructors(IOFireWireAVCSubUnit)

protected:
    IOFireWireAVCUnit *fAVCUnit;
    
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;
    
public:
    virtual bool init(OSDictionary *propTable, IOFireWireAVCUnit *provider);

    // IOService overrides
    virtual IOReturn message(UInt32 type, IOService *provider, void *argument);

/*! @function handleOpen
    @abstract Overrideable method to control the open / close behaviour of an IOService.
    @discussion See IOService for discussion.		
    @param forClient Designates the client of the provider requesting the open.
    @param options Options for the open, may be interpreted by the implementor of handleOpen.
    @result Return true if the open was successful, false otherwise.
*/

    virtual bool handleOpen(  IOService *	  forClient,
                              IOOptionBits	  options,
                              void *		  arg );
/*! 
    @function handleClose
    @abstract Overrideable method to control the open / close behaviour of an IOService.
    @discussion See IOService for discussion.
    @param forClient Designates the client of the provider requesting the close.
    @param options Options for the close, may be interpreted by the implementor of handleOpen. 
*/

    virtual void handleClose(   IOService *		forClient,
                                IOOptionBits	options );
    

/*!
    @function matchPropertyTable
    @abstract Matching language support
	Match on the following properties of the sub unit:
	Vendor_ID
	GUID
	SubUnit_Type
*/
    virtual bool matchPropertyTable(OSDictionary * table);

    // execute AVC command
/*!
    @function AVCCommand
    @abstract Sends an AVC command to the device and stores the response.
    @param command Pointer to command to send.
    @param cmdLen Length of the command.
    @param response Pointer to place to store the response.
    @param responseLen Pointer to response length - initialize to the size of the buffer pointed to by response,
    updated to the number of bytes returned by the device.
*/
    virtual IOReturn AVCCommand(const UInt8 * command, UInt32 cmdLen, UInt8 * response, UInt32 *responseLen);

/*!
    @function AVCCommandInGeneration
    @abstract Sends an AVC command to the device and stores the response. The command must complete in the specified
    FireWire bus generation otherwise kIOFireWireBusReset is returned.
    @param generation The bus generation that the command must execute in.
    @param command Pointer to command to send.
    @param cmdLen Length of the command.
    @param response Pointer to place to store the response.
    @param responseLen Pointer to response length - initialize to the size of the buffer pointed to by response,
    updated to the number of bytes returned by the device.
*/
    virtual IOReturn AVCCommandInGeneration(UInt32 generation,
                const UInt8 * command, UInt32 cmdLen, UInt8 * response, UInt32 *responseLen);

/*!
    @function updateAVCCommandTimeout
    @abstract By default, AVCCommands timeout 10 seconds after receiving an Interim response.
    This function resets the timeout of the current command to 10 seconds from the current time.
    Call this repeatedly for AVC commands that take a very long time to execute to prevent premature
    timeout.
*/
    virtual IOReturn updateAVCCommandTimeout();

private:
    OSMetaClassDeclareReservedUnused(IOFireWireAVCSubUnit, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCSubUnit, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCSubUnit, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCSubUnit, 3);

};

#endif // _IOKIT_IOFIREWIREAVCUNIT_H

