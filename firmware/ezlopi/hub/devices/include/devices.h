#ifndef __HUB_DEVICE_LIST_H__
#define __HUB_DEVICE_LIST_H__

#include <string.h>
#include "frozen.h"

#ifdef __cplusplus
extern "C"
{
#endif

    char *devices_list(const char *payload, uint32_t len, struct json_token *method, uint32_t msg_count);
    char *devices_settings_list(const char *payload, uint32_t len, struct json_token *method, uint32_t msg_count);
    char *devices_name_set(const char *payload, uint32_t len, struct json_token *method, uint32_t msg_count);

#ifdef __cplusplus
}
#endif

#endif // __HUB_DEVICE_LIST_H__