/*
 * Copyright (c) 1998-2001 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_IOFIREWIREAVCUSERCLIENTCOMMON_H_
#define _IOKIT_IOFIREWIREAVCUSERCLIENTCOMMON_H_

#include <IOKit/avc/IOFireWireAVCConsts.h>

#define kIOFireWireAVCLibConnection 13

enum IOFWAVCUserClientCommandCodes {
    kIOFWAVCUserClientOpen,						// kIOUCScalarIScalarO 0,0
    kIOFWAVCUserClientClose,					// kIOUCScalarIScalarO 0,0
    kIOFWAVCUserClientOpenWithSessionRef,		// kIOUCScalarIScalarO 1,0
	kIOFWAVCUserClientGetSessionRef,			// kIOUCScalarIScalarO 0,1
    kIOFWAVCUserClientAVCCommand,				// kIOUCStructIStructO -1,-1
    kIOFWAVCUserClientAVCCommandInGen,			// kIOUCStructIStructO -1,-1
    kIOFWAVCUserClientUpdateAVCCommandTimeout,	// kIOUCScalarIScalarO 0,0
    kIOFWAVCUserClientMakeP2PInputConnection,	// KIOUCScalarIScalarO 1, 0
    kIOFWAVCUserClientBreakP2PInputConnection,	// KIOUCScalarIScalarO 1, 0
    kIOFWAVCUserClientMakeP2POutputConnection,	// KIOUCScalarIScalarO 1, 0
    kIOFWAVCUserClientBreakP2POutputConnection,	// KIOUCScalarIScalarO 1, 0
    kIOFWAVCUserClientNumCommands
};


enum IOFWAVCProtocolUserClientCommandCodes {
    kIOFWAVCProtocolUserClientSendAVCResponse,   		// kIOUCScalarIStructI 2, -1
    kIOFWAVCProtocolUserClientFreeInputPlug,			// kIOUCScalarIScalarO 1, 0
    kIOFWAVCProtocolUserClientReadInputPlug,			// kIOUCScalarIScalarO 1, 1
    kIOFWAVCProtocolUserClientUpdateInputPlug,			// kIOUCScalarIScalarO 3, 0
    kIOFWAVCProtocolUserClientFreeOutputPlug,			// kIOUCScalarIScalarO 1, 0
    kIOFWAVCProtocolUserClientReadOutputPlug,			// kIOUCScalarIScalarO 1, 1
    kIOFWAVCProtocolUserClientUpdateOutputPlug,			// kIOUCScalarIScalarO 3, 0
    kIOFWAVCProtocolUserClientReadOutputMasterPlug,		// kIOUCScalarIScalarO 0, 1
    kIOFWAVCProtocolUserClientUpdateOutputMasterPlug,	// kIOUCScalarIScalarO 2, 0
    kIOFWAVCProtocolUserClientReadInputMasterPlug,		// kIOUCScalarIScalarO 0, 1
    kIOFWAVCProtocolUserClientUpdateInputMasterPlug,	// kIOUCScalarIScalarO 2, 0
    kIOFWAVCProtocolUserClientPublishAVCUnitDirectory,	// kIOUCScalarIScalarO 0, 0
	kIOFWAVCProtocolUserClientSetSubunitPlugSignalFormat, // kIOUCScalarIScalarO 4, 0
	kIOFWAVCProtocolUserClientGetSubunitPlugSignalFormat, // kIOUCScalarIScalarO 3, 1
	kIOFWAVCProtocolUserClientConnectTargetPlugs,		// kIOUCStructIStructO
	kIOFWAVCProtocolUserClientDisconnectTargetPlugs,	// kIOUCScalarIScalarO 6, 0
	kIOFWAVCProtocolUserClientGetTargetPlugConnection,	// kIOUCStructIStructO
	kIOFWAVCProtocolUserClientAVCRequestNotHandled,		// kIOUCScalarIStructI 4, -1
    kIOFWAVCProtocolUserClientNumCommands
};

enum IOFWAVCProtocolUserClientAsyncCommandCodes {
    kIOFWAVCProtocolUserClientSetAVCRequestCallback,   		// kIOUCScalarIScalarO 2, 0
    kIOFWAVCProtocolUserClientAllocateInputPlug,			// kIOUCScalarIScalarO 1, 1
    kIOFWAVCProtocolUserClientAllocateOutputPlug,			// kIOUCScalarIScalarO 1, 1
    kIOFWAVCProtocolUserClientInstallAVCCommandHandler,		// kIOUCScalarIScalarO 4, 0
    kIOFWAVCProtocolUserClientAddSubunit,					// kIOUCScalarIScalarO 5, 1
    kIOFWAVCProtocolUserClientNumAsyncCommands
};

typedef struct _AVCConnectTargetPlugsInParams
{
	UInt32 sourceSubunitTypeAndID;
	IOFWAVCPlugTypes sourcePlugType;
	UInt32 sourcePlugNum;
	UInt32 destSubunitTypeAndID;
	IOFWAVCPlugTypes destPlugType;
	UInt32 destPlugNum;
	bool lockConnection;
	bool permConnection;
}AVCConnectTargetPlugsInParams;

typedef struct _AVCConnectTargetPlugsOutParams
{
	UInt32 sourcePlugNum;
	UInt32 destPlugNum;
}AVCConnectTargetPlugsOutParams;

typedef struct _AVCGetTargetPlugConnectionInParams
{
	UInt32 subunitTypeAndID;
	IOFWAVCPlugTypes plugType;
	UInt32 plugNum;
}AVCGetTargetPlugConnectionInParams;

typedef struct _AVCGetTargetPlugConnectionOutParams
{
	UInt32 connectedSubunitTypeAndID;
	IOFWAVCPlugTypes connectedPlugType;
	UInt32 connectedPlugNum;
	bool lockConnection;
	bool permConnection;
}AVCGetTargetPlugConnectionOutParams;

#endif // _IOKIT_IOFIREWIREAVCUSERCLIENTCOMMON_H_