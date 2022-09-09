#include <node_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i106_stdint.h"

#include "irig106ch10.h"
#include "i106_decode_1553f1.h"
#include "js_helper_functions.h"

napi_value MyFunction(napi_env env, napi_callback_info info)
{
    /* Declaring the variables */

    // Not sure if this must really be an int or what:
    int iI106Ch10Handle; // No idea what this does; it's a handle

    const char *szInFile; // I believe this is the filename

    EnI106Status enStatus = I106_OK; // Declaring an inital OK status

    SuI106Ch10Header suI106Hdr; // Header for Reading files

    unsigned long ulBuffSize; // Buffer Size - Don't know if we set or it is given

    void *pvBuff; //  Some buffer - No clue what it does

    Su1553F1_CurrMsg su1553Msg; // The address for the current 1553 message

    // NAPI Relevant Variables
    napi_status status; // The status we check
    size_t argc = 3;    // The amount of arguments we are taking from nodeJS
    napi_value argv[3]; // The Variables we are grabbing from N-API

    const char *outputFileName; // Name of output file - probably *.json or *.csv

    int outputValue; // An output value to know when we are done

    /* --------------------- */

    /* Parse N-API Data */
    // In theory we only need to get:
    //  - The input filename
    //  - The output filename
    //  - And a Handle - which I'm not sure about - but we shall parse just in case.

    // Check CallBack Info
    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    CheckNAPIStatus(env, ARGPARSE_FAIL, status);

    // Get InputFileName
    size_t str_size;
    size_t result_size;
    status = napi_get_value_string_utf8(env, argv[0], &szInFile, str_size, &result_size);
    CheckNAPIStatus(env, INVALID_STRING_VAL, status);

    // Get Output FileName
    status = napi_get_value_string_utf8(env, argv[1], &outputFileName, str_size, &result_size);
    CheckNAPIStatus(env, INVALID_STRING_VAL, status);

    // Get Handle - which is an int32 in our assumption
    status = napi_get_value_int32(env, argv[2], &iI106Ch10Handle);
    CheckNAPIStatus(env, INVALID_INT_VAL, status);

    /* --------------------- */

    enI106Ch10Open(&iI106Ch10Handle, szInFile, I106_READ);

    while (1 == 1)
    {

        enStatus = enI106Ch10ReadNextHeader(iI106Ch10Handle, &suI106Hdr);

        if (enStatus == I106_EOF)
        // If we finish reading the file...
        {
            status = napi_create_int32(env, 0, &outputValue);
            CheckNAPIStatus(env, CREATE_RETURN_VALUE_FAIL, status);
            return outputValue;
        };

        enStatus = enI106Ch10ReadData(iI106Ch10Handle, &ulBuffSize, pvBuff);

        switch (suI106Hdr.ubyDataType)
        {

        case I106CH10_DTYPE_1553_FMT_1: // 0x19

            enStatus = enI106_Decode_First1553F1(&suI106Hdr, pvBuff, &su1553Msg);
            while (enStatus == I106_OK)
            {
                // Do some processing...
                enStatus = enI106_Decode_Next1553F1(&su1553Msg);
            }
            break;
        default:
            break;
        } // end switch on packet type

    } // End while

    enI106Ch10Close(iI106Ch10Handle);
    // Create a Success output
    status = napi_create_int32(env, 0, &outputValue);
    CheckNAPIStatus(env, CREATE_RETURN_VALUE_FAIL, status);
    return outputValue;
};

napi_value CheckAPIIsWOrking(napi_env env, napi_callback_info info)
{
    napi_status status;
    size_t argc = 1;
    napi_value argv[1];
    int number = 0;

    // Get Callbsck info
    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    CheckNAPIStatus(env, ARGPARSE_FAIL, status);

    // Get number
    status = napi_get_value_int32(env, argv[0], &number);
    CheckNAPIStatus(env, INVALID_INT_VAL, status);

    // Write the output
    napi_value output_num;
    status = napi_create_int32(env, number * 2, &output_num);
    CheckNAPIStatus(env, INVALID_INT_VAL, status);

    // Return the number
    return output_num;
};

napi_value Init(napi_env env, napi_value exports)
{
    /*
    function to initialize the output into js -needed for the nodeapi
    */
    // Initialization Code
    napi_status status;
    napi_value c_func;

    // Arguments 2 and 3 are function name and length respectively
    // We will leave them as empty - maybe this have another use
    // TODO Check function name and length
    // Register main function - need better name
    status = napi_create_function(env, NULL, 0, MyFunction, NULL, &c_func);
    CheckNAPIStatus(env, WRAP_FAIL, status);

    status = napi_set_named_property(env, exports, "my_function", c_func);
    CheckNAPIStatus(env, POPULATE_EXPORTS_FAIL, status);

    // Register function that checks connection
    status = napi_create_function(env, NULL, 0, CheckAPIIsWOrking, NULL, &c_func);
    CheckNAPIStatus(env, WRAP_FAIL, status);

    status = napi_set_named_property(env, exports, "check_connection", c_func);
    CheckNAPIStatus(env, POPULATE_EXPORTS_FAIL, status);

    return exports;
};

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)