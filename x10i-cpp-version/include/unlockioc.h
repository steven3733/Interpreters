/**********************************************************************
*
*     Project:      Firefly X10i Board
*
*     File:         unlockioc.h
*
*     Version:      1.0
*
*     Description:  Unlock X10i C API header file.
*
*     Changes:
*     1.0 MJB       First version, 17/1/2008
*
**********************************************************************/

#ifndef UNLOCKIOC_H
#define UNLOCKIOC_H

#ifdef __cplusplus
extern "C"  {
#endif

	BOOL UnlockX10c( void *x10Board );
	BOOL UnlockX10Recheckc( void *x10Board );
	BOOL VerifyX10UnlockLibraryc( void *x10Board );

#ifdef __cplusplus
}
#endif
#endif



