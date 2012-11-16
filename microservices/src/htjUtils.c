/**
 * @file htjUtils.c
 *
 */

/*** Copyright (c), University of Helsinki            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

#include "htjUtils.h"
#include "math.h" // 20111020


int 
msiWriteOpenDisallowed(ruleExecInfo_t *rei)
{
    RE_TEST_MACRO ("    Calling msiWriteOpenDisallowed")

    rei->status = FILE_WRITE_ERR;

    return (rei->status);
}


int 
msiStrReplace(msParam_t *in, msParam_t *find, msParam_t *replace, msParam_t *count, msParam_t *out, ruleExecInfo_t *rei)
{
 	char *inStr;				/* for parsing input string */
 	char *findStr;				/* for parsing string to find*/
 	char *replaceStr;				/* for parsing string to replace with */
    size_t patcnt;       /*For parsing pattern count*/
	
    RE_TEST_MACRO ("    Calling msiStrReplace")

	/* Sanity test */
	if (rei == NULL || rei->rsComm == NULL) {
			rodsLog (LOG_ERROR, "msiStrReplace: input rei or rsComm is NULL.");
			return (SYS_INTERNAL_NULL_INPUT_ERR);
	}
	
	
	/* Check for NULL input */
    if (!in || !find || !replace || ! count || !out) 
    {
    	rei->status = USER__NULL_INPUT_ERR;
		rodsLog (LOG_ERROR, "msiStrReplace: input error. status = %d", rei->status);
		return (rei->status);
    }
    
	
	/* Parse input  'in'*/
	inStr = (char *) in->inOutStruct;
	if ( !inStr || !strlen(inStr) || strlen(inStr) > MAX_NAME_LEN )
	{
		rei->status = USER_INPUT_PATH_ERR;
		rodsLog (LOG_ERROR, "msiStrReplace: input 'in' error. status = %d", rei->status);
		return (rei->status);	
	}
    
	/* Parse input 'find'*/
	findStr = (char *) find->inOutStruct;
	if ( !findStr || !strlen(findStr) || strlen(findStr) > MAX_NAME_LEN )
	{
		rei->status = USER_INPUT_PATH_ERR;
		rodsLog (LOG_ERROR, "msiStrReplace: input 'find' error. status = %d", rei->status);
		return (rei->status);	
	}
    
	/* Parse input 'replace'*/
	replaceStr = (char *) replace->inOutStruct;
//	if ( !replaceStr || !strlen(replaceStr) || strlen(replaceStr) > MAX_NAME_LEN )
	if ( !replaceStr || strlen(replaceStr) > MAX_NAME_LEN )
	{
		rei->status = USER_INPUT_PATH_ERR;
		rodsLog (LOG_ERROR, "msiStrReplace: input 'replace' error. status = %d", rei->status);
		return (rei->status);	
	}

	/* Parse input 'count'*/
	patcnt = (size_t) atoi((const char *)replace->inOutStruct);
//	if ( !patcnt)
//	{
//		rei->status = USER_INPUT_PATH_ERR;
//		rodsLog (LOG_ERROR, "msiStrReplace: input 'count' error. status = %d", rei->status);
//		return (rei->status);	
//	}
    
    size_t const replen = strlen(replaceStr);
    size_t const patlen = strlen(findStr);
    size_t const orilen = strlen(inStr);
    
    const char * oriptr;
    const char * patloc;
    
    // zero count means to replace all
    // find how many times the findStr occurs in the inStr string
    if (patcnt == 0) {
        for ((oriptr = inStr); patloc = strstr(oriptr, findStr); oriptr = patloc + patlen)
        {
            patcnt++;
        }
    }
    
    {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );
    
    if (returned != NULL)
    {
      // copy the inStr string, 
      // replacing all the instances of the findStr
      char * retptr = returned;
      for (oriptr = inStr; patloc = strstr(oriptr, findStr); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the findStr
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replaceStr 
        strncpy(retptr, replaceStr, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    //return returned;


    fillStrInMsParam (out, returned);

    rei->status = 0;

    return (rei->status);
    }
}

int 
msiSystemToHumanTime(msParam_t *epoch, msParam_t *human, ruleExecInfo_t *rei)
{
 	time_t epochTime;				/* for parsing input string */
 	char *humanStr;				/* for output string*/

    RE_TEST_MACRO ("    Calling msiSystemToHumanTime")

	/* Sanity test */
	if (rei == NULL || rei->rsComm == NULL) {
			rodsLog (LOG_ERROR, "msiSystemToHumanTime: input rei or rsComm is NULL.");
			return (SYS_INTERNAL_NULL_INPUT_ERR);
	}
	
	
	/* Check for NULL input */
    if (!epoch || !human) 
    {
    	rei->status = USER__NULL_INPUT_ERR;
		rodsLog (LOG_ERROR, "msiSystemToHumanTime: input error. status = %d", rei->status);
		return (rei->status);
    }
    
	
	/* Parse input  'in'*/
	epochTime = (time_t ) atoi((const char *)epoch->inOutStruct);
//	if ( !epochStr || !strlen(epochStr) || strlen(epochStr) > MAX_NAME_LEN )
//	{
//		rei->status = USER_INPUT_PATH_ERR;
//		rodsLog (LOG_ERROR, "msiSystemToHumanTime: input 'in' error. status = %d", rei->status);
//		return (rei->status);	
//	}
    
    //time_t     now;
    struct tm  ts;
    char       buf[80];

    // Get current time
    //time(&now);

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&epochTime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

    fillStrInMsParam (human, buf);
    //printf("%s\n", buf);

    rei->status = 0;

    return (rei->status);

}

int msiBytesBuf2Str(msParam_t* inBuf, msParam_t* outStr, ruleExecInfo_t *rei)
{
    char *writeId;
    char *writeStr;
    int status;
    
    /* Sanity test */
	if (rei == NULL || rei->rsComm == NULL) {
			rodsLog (LOG_ERROR, "msiSystemToHumanTime: input rei or rsComm is NULL.");
			return (SYS_INTERNAL_NULL_INPUT_ERR);
	}

    
    /* Check for NULL input */
    if (!inBuf || !outStr) 
    {
    	rei->status = USER__NULL_INPUT_ERR;
		rodsLog (LOG_ERROR, "msiBytesBuf2Str: input error. status = %d", rei->status);
		return (rei->status);
    }

    
    if (inBuf->inpOutBuf != NULL) {
        writeStr = (char *) malloc(strlen((const char*)inBuf->inpOutBuf->buf) + MAX_COND_LEN);
        strcpy(writeStr , (const char*)inBuf->inpOutBuf->buf);
    }
    else {
        writeStr = (char *) malloc(strlen(inBuf->label) + MAX_COND_LEN);
        strcpy(writeStr , inBuf->label);
    }

    fillStrInMsParam (outStr, writeStr);
    
    if (writeStr != NULL) {
        free(writeStr);
    }

    rei->status = 0;

    return (rei->status);
}
