/*** Copyright (c), University of Helsinki           ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* collInfoMS.h - header file for collInfoMS.c
 */

#ifndef HTJUTILSMS_H
#define HTJUTILSMS_H

#include "apiHeaderAll.h"
#include "objMetaOpr.h"
#include "miscUtil.h"

int msiWriteOpenDisallowed(ruleExecInfo_t *rei);
int msiBytesBuf2Str(msParam_t *bbuf, msParam_t *str, ruleExecInfo_t *rei);
int msiStrReplace(msParam_t *in, msParam_t *find, msParam_t *replace, msParam_t *count, msParam_t *out, ruleExecInfo_t *rei);
int msiSystemToHumanTime(msParam_t *epoch, msParam_t *human, ruleExecInfo_t *rei);
#endif	/* HTJUTILSMS_H */

