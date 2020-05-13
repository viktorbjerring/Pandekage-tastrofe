#include "__cf_regulering_model.h"
#include <math.h>
#include "regulering_model_acc.h"
#include "regulering_model_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T regulering_model_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr ,
boolean_T isfixedbuf , boolean_T istransportdelay , int_T * maxNewBufSzPtr )
{ int_T testIdx ; int_T tail = * tailPtr ; int_T bufSz = * bufSzPtr ; real_T
* tBuf = * tBufPtr ; real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if (
istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ; } testIdx = ( tail <
( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] )
&& ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T *
tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024
; if ( newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU
= ( real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if (
tempU == ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * tBufPtr = tempT ; * uBufPtr = tempU ; if (
istransportdelay ) * xBufPtr = tempX ; } else { * tailPtr = testIdx ; }
return ( true ) ; } real_T regulering_model_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz ,
int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput ,
boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ;
real_T yout , t1 , t2 , u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } void rt_ssGetBlockPath (
SimStruct * S , int_T sysIdx , int_T blkIdx , char_T * * path ) {
_ssGetBlockPath ( S , sysIdx , blkIdx , path ) ; } void rt_ssSet_slErrMsg (
SimStruct * S , void * diag ) { if ( ! _ssIsErrorStatusAslErrMsg ( S ) ) {
_ssSet_slErrMsg ( S , diag ) ; } else { _ssDiscardDiagnostic ( S , diag ) ; }
} void rt_ssReportDiagnosticAsWarning ( SimStruct * S , void * diag ) {
_ssReportDiagnosticAsWarning ( S , diag ) ; } static void mdlOutputs (
SimStruct * S , int_T tid ) { real_T B_0_2_0 ; int32_T isHit ; real_T u0 ;
B_regulering_model_T * _rtB ; P_regulering_model_T * _rtP ;
X_regulering_model_T * _rtX ; DW_regulering_model_T * _rtDW ; _rtDW = ( (
DW_regulering_model_T * ) ssGetRootDWork ( S ) ) ; _rtX = ( (
X_regulering_model_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_regulering_model_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_regulering_model_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB -> B_0_0_0 = 0.0 ;
_rtB -> B_0_0_0 += _rtP -> P_1 * _rtX -> TransferFcn_CSTATE ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { { if ( ( _rtDW ->
TAQSigLogging_InsertedFor_TransferFcn_at_outport_0_PWORK . AQHandles || _rtDW
-> TAQSigLogging_InsertedFor_TransferFcn_at_outport_0_PWORK . SlioLTF ) &&
ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_TransferFcn_at_outport_0_PWORK . AQHandles , _rtDW
-> TAQSigLogging_InsertedFor_TransferFcn_at_outport_0_PWORK . SlioLTF , 0 ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_0_0_0 + 0 ) ; } } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { B_0_2_0 = _rtDW ->
UnitDelay_DSTATE ; { if ( ( _rtDW ->
TAQSigLogging_InsertedFor_UnitDelay_at_outport_0_PWORK . AQHandles || _rtDW
-> TAQSigLogging_InsertedFor_UnitDelay_at_outport_0_PWORK . SlioLTF ) &&
ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_UnitDelay_at_outport_0_PWORK . AQHandles , _rtDW ->
TAQSigLogging_InsertedFor_UnitDelay_at_outport_0_PWORK . SlioLTF , 0 ,
ssGetTaskTime ( S , 2 ) , ( char * ) & B_0_2_0 + 0 ) ; } } u0 = ( _rtB ->
B_0_0_0_m - B_0_2_0 ) * _rtP -> P_3 / _rtB -> B_0_0_0_m ; if ( u0 > _rtP ->
P_4 ) { _rtB -> B_0_7_0 = _rtP -> P_4 ; } else if ( u0 < _rtP -> P_5 ) { _rtB
-> B_0_7_0 = _rtP -> P_5 ; } else { _rtB -> B_0_7_0 = u0 ; } } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ]
; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_6 ; _rtB -> B_0_8_0 =
regulering_model_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , *
uBuffer , _rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW ->
TransportDelay_IWORK . Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW ->
TransportDelay_IWORK . Head , _rtP -> P_7 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } UNUSED_PARAMETER ( tid ) ; } static void mdlOutputsTID3 ( SimStruct * S
, int_T tid ) { B_regulering_model_T * _rtB ; P_regulering_model_T * _rtP ;
_rtP = ( ( P_regulering_model_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_regulering_model_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB -> B_0_0_0_m =
_rtP -> P_8 ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { int32_T isHit ;
B_regulering_model_T * _rtB ; P_regulering_model_T * _rtP ;
DW_regulering_model_T * _rtDW ; _rtDW = ( ( DW_regulering_model_T * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( P_regulering_model_T * ) ssGetModelRtp (
S ) ) ; _rtB = ( ( B_regulering_model_T * ) _ssGetModelBlockIO ( S ) ) ;
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay_DSTATE = _rtB -> B_0_0_0 ; } { real_T * * uBuffer = ( real_T * * )
& _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK . Head = ( ( _rtDW ->
TransportDelay_IWORK . Head < ( _rtDW -> TransportDelay_IWORK .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK . Head + 1 ) : 0 )
; if ( _rtDW -> TransportDelay_IWORK . Head == _rtDW -> TransportDelay_IWORK
. Tail ) { if ( ! regulering_model_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> TransportDelay_IWORK . CircularBufSize , & _rtDW -> TransportDelay_IWORK .
Tail , & _rtDW -> TransportDelay_IWORK . Head , & _rtDW ->
TransportDelay_IWORK . Last , simTime - _rtP -> P_6 , tBuffer , uBuffer , (
NULL ) , ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * tBuffer ) [ _rtDW -> TransportDelay_IWORK . Head ] =
simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK . Head ] = _rtB ->
B_0_7_0 ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID3 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_regulering_model_T * _rtB ;
P_regulering_model_T * _rtP ; X_regulering_model_T * _rtX ;
XDot_regulering_model_T * _rtXdot ; _rtXdot = ( ( XDot_regulering_model_T * )
ssGetdX ( S ) ) ; _rtX = ( ( X_regulering_model_T * ) ssGetContStates ( S ) )
; _rtP = ( ( P_regulering_model_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_regulering_model_T * ) _ssGetModelBlockIO ( S ) ) ; _rtXdot ->
TransferFcn_CSTATE = 0.0 ; _rtXdot -> TransferFcn_CSTATE += _rtP -> P_0 *
_rtX -> TransferFcn_CSTATE ; _rtXdot -> TransferFcn_CSTATE += _rtB -> B_0_8_0
; } static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S ,
0 , 2826072916U ) ; ssSetChecksumVal ( S , 1 , 4148139850U ) ;
ssSetChecksumVal ( S , 2 , 3423424549U ) ; ssSetChecksumVal ( S , 3 ,
1693662790U ) ; { mxArray * slVerStructMat = NULL ; mxArray * slStrMat =
mxCreateString ( "simulink" ) ; char slVerChar [ 10 ] ; int status =
mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver" ) ; if ( status
== 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0 , "Version" ) ;
if ( slVerMat == NULL ) { status = 1 ; } else { status = mxGetString (
slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
mxDestroyArray ( slVerStructMat ) ; if ( ( status == 1 ) || ( strcmp (
slVerChar , "10.0" ) != 0 ) ) { return ; } } ssSetOptions ( S ,
SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork ( S ) != sizeof (
DW_regulering_model_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( B_regulering_model_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
P_regulering_model_T ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & regulering_model_rtDefaultP ) ; } static void mdlInitializeSampleTimes (
SimStruct * S ) { slAccRegPrmChangeFcn ( S , mdlOutputsTID3 ) ; } static void
mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
