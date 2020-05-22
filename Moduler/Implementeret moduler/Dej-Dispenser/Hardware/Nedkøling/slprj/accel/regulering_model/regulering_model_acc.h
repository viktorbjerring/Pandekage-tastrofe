#include "__cf_regulering_model.h"
#ifndef RTW_HEADER_regulering_model_acc_h_
#define RTW_HEADER_regulering_model_acc_h_
#include <stddef.h>
#include <float.h>
#ifndef regulering_model_acc_COMMON_INCLUDES_
#define regulering_model_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simtarget/slSimTgtSigstreamRTW.h"
#include "simtarget/slSimTgtSlioCoreRTW.h"
#include "simtarget/slSimTgtSlioClientsRTW.h"
#include "simtarget/slSimTgtSlioSdiRTW.h"
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "regulering_model_acc_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rt_defines.h"
typedef struct { real_T B_0_0_0 ; real_T B_0_7_0 ; real_T B_0_8_0 ; real_T
B_0_0_0_m ; } B_regulering_model_T ; typedef struct { real_T UnitDelay_DSTATE
; struct { real_T modelTStart ; } TransportDelay_RWORK ; struct { void *
AQHandles ; void * SlioLTF ; }
TAQSigLogging_InsertedFor_TransferFcn_at_outport_0_PWORK ; struct { void *
AQHandles ; void * SlioLTF ; }
TAQSigLogging_InsertedFor_UnitDelay_at_outport_0_PWORK ; struct { void *
TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; char_T pad_TransportDelay_IWORK [ 4 ] ; }
DW_regulering_model_T ; typedef struct { real_T TransferFcn_CSTATE ; }
X_regulering_model_T ; typedef struct { real_T TransferFcn_CSTATE ; }
XDot_regulering_model_T ; typedef struct { boolean_T TransferFcn_CSTATE ; }
XDis_regulering_model_T ; typedef struct { real_T TransferFcn_CSTATE ; }
CStateAbsTol_regulering_model_T ; struct P_regulering_model_T_ { real_T P_0 ;
real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 ; real_T P_8 ; } ; extern P_regulering_model_T
regulering_model_rtDefaultP ;
#endif
