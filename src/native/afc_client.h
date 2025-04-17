#ifndef AFC_CLIENT_H
#define AFC_CLIENT_H

#include <node_api.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/afc.h>

// Global variables
extern idevice_t device;
extern lockdownd_client_t lockdown;
extern afc_client_t afc;

// Function declarations
static napi_value Initialize(napi_env env, napi_callback_info info);
static napi_value PutFile(napi_env env, napi_callback_info info);
// static napi_value ListDirectory(napi_env env, napi_callback_info info);
// static napi_value ReadFile(napi_env env, napi_callback_info info);
static napi_value Init(napi_env env, napi_value exports);

#endif // AFC_CLIENT_H 