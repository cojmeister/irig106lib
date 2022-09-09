#include <node_api.h>

// The N-API Status Codes
typedef enum
{
    ARGPARSE_FAIL = 0,            // Failed to parse arguments
    INVALID_INT_VAL = 1,          // Invalid number was passed as argument
    INVALID_STRING_VAL = 2,       // Invalid string was passed as argument
    WRAP_FAIL = 3,                // Unable to wrap native function
    POPULATE_EXPORTS_FAIL = 4,    // Unable to populate exports
    CREATE_RETURN_VALUE_FAIL = 5, // Unable to create return value
} NAPI_STATUS_CODES;

void CheckNAPIStatus(napi_env env, NAPI_STATUS_CODES fail_type, napi_status status)
{

    if (status != napi_ok)
    {
        switch (fail_type)
        {
        case ARGPARSE_FAIL:
            napi_throw_error(env, NULL, "Failed to parse arguments");
            break;
        case INVALID_INT_VAL:
            napi_throw_error(env, NULL, "nvalid number was passed as argument");
            break;
        case INVALID_STRING_VAL:
            napi_throw_error(env, NULL, "Invalid string was passed as argument");
            break;
        case WRAP_FAIL:
            napi_throw_error(env, NULL, "Unable to wrap native function");
            break;
        case POPULATE_EXPORTS_FAIL:
            napi_throw_error(env, NULL, "Unable to populate exports");
            break;
        case CREATE_RETURN_VALUE_FAIL:
            napi_throw_error(env, NULL, "Unable to create return value");
            break;
        default:
            napi_throw_error(env, NULL, "Unknown N-API Error");
        }
    }
}