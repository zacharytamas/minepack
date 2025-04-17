#include "afc_client.h"
#include <node_api.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/house_arrest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define client data structure
typedef struct
{
  idevice_t device;
  lockdownd_client_t lockdown;
  lockdownd_service_descriptor_t lockdown_service;
  afc_client_t afc;
  house_arrest_client_t house_arrest;
} ClientData;

const char *idev_afc_strerror(afc_error_t errnum)
{
  switch (errnum)
  {
  case AFC_E_SUCCESS:
    return "SUCCESS";
  case AFC_E_UNKNOWN_ERROR:
    return "UNKNOWN_ERROR";
  case AFC_E_OP_HEADER_INVALID:
    return "OP_HEADER_INVALID";
  case AFC_E_NO_RESOURCES:
    return "NO_RESOURCES";
  case AFC_E_READ_ERROR:
    return "READ_ERROR";
  case AFC_E_WRITE_ERROR:
    return "WRITE_ERROR";
  case AFC_E_UNKNOWN_PACKET_TYPE:
    return "UNKNOWN_PACKET_TYPE";
  case AFC_E_INVALID_ARG:
    return "INVALID_ARG";
  case AFC_E_OBJECT_NOT_FOUND:
    return "OBJECT_NOT_FOUND";
  case AFC_E_OBJECT_IS_DIR:
    return "OBJECT_IS_DIR";
  case AFC_E_PERM_DENIED:
    return "PERM_DENIED";
  case AFC_E_SERVICE_NOT_CONNECTED:
    return "SERVICE_NOT_CONNECTED";
  case AFC_E_OP_TIMEOUT:
    return "OP_TIMEOUT";
  case AFC_E_TOO_MUCH_DATA:
    return "TOO_MUCH_DATA";
  case AFC_E_END_OF_DATA:
    return "END_OF_DATA";
  case AFC_E_OP_NOT_SUPPORTED:
    return "OP_NOT_SUPPORTED";
  case AFC_E_OBJECT_EXISTS:
    return "OBJECT_EXISTS";
  case AFC_E_OBJECT_BUSY:
    return "OBJECT_BUSY";
  case AFC_E_NO_SPACE_LEFT:
    return "NO_SPACE_LEFT";
  case AFC_E_OP_WOULD_BLOCK:
    return "OP_WOULD_BLOCK";
  case AFC_E_IO_ERROR:
    return "IO_ERROR";
  case AFC_E_OP_INTERRUPTED:
    return "OP_INTERRUPTED";
  case AFC_E_OP_IN_PROGRESS:
    return "OP_IN_PROGRESS";
  case AFC_E_INTERNAL_ERROR:
    return "INTERNAL_ERROR";
  case AFC_E_MUX_ERROR:
    return "MUX_ERROR";
  case AFC_E_NO_MEM:
    return "NO_MEM";
  case AFC_E_NOT_ENOUGH_DATA:
    return "NOT_ENOUGH_DATA";
  case AFC_E_DIR_NOT_EMPTY:
    return "DIR_NOT_EMPTY";
  default:
    return "UNKNOWN_EROR";
  }
}

const char *idev_house_arrest_strerror(house_arrest_error_t errnum)
{
  switch (errnum)
  {
  case HOUSE_ARREST_E_SUCCESS:
    return "SUCCESS";
  case HOUSE_ARREST_E_INVALID_ARG:
    return "INVALID_ARG";
  case HOUSE_ARREST_E_PLIST_ERROR:
    return "PLIST_ERROR";
  case HOUSE_ARREST_E_CONN_FAILED:
    return "CONN_FAILED";
  case HOUSE_ARREST_E_INVALID_MODE:
    return "INVALID_MODE";
  case HOUSE_ARREST_E_UNKNOWN_ERROR:
  default:
    return "UNKNOWN_EROR";
  }
}

const char *idev_lockdownd_strerror(lockdownd_error_t errnum)
{
  switch (errnum)
  {
  case LOCKDOWN_E_SUCCESS:
    return "SUCCESS";
  case LOCKDOWN_E_INVALID_ARG:
    return "INVALID_ARG";
  case LOCKDOWN_E_INVALID_CONF:
    return "INVALID_CONF";
  case LOCKDOWN_E_PLIST_ERROR:
    return "PLIST_ERROR";
  case LOCKDOWN_E_PAIRING_FAILED:
    return "PAIRING_FAILED";
  case LOCKDOWN_E_SSL_ERROR:
    return "SSL_ERROR";
  case LOCKDOWN_E_DICT_ERROR:
    return "DICT_ERROR";
    //        case LOCKDOWN_E_NOT_ENOUGH_DATA:
    //            return "NOT_ENOUGH_DATA";
  case LOCKDOWN_E_MUX_ERROR:
    return "MUX_ERROR";
  case LOCKDOWN_E_NO_RUNNING_SESSION:
    return "NO_RUNNING_SESSION";
  case LOCKDOWN_E_INVALID_RESPONSE:
    return "INVALID_RESPONSE";
  case LOCKDOWN_E_MISSING_KEY:
    return "MISSING_KEY";
  case LOCKDOWN_E_MISSING_VALUE:
    return "MISSING_VALUE";
  case LOCKDOWN_E_GET_PROHIBITED:
    return "GET_PROHIBITED";
  case LOCKDOWN_E_SET_PROHIBITED:
    return "SET_PROHIBITED";
  case LOCKDOWN_E_REMOVE_PROHIBITED:
    return "REMOVE_PROHIBITED";
  case LOCKDOWN_E_IMMUTABLE_VALUE:
    return "IMMUTABLE_VALUE";
  case LOCKDOWN_E_PASSWORD_PROTECTED:
    return "PASSWORD_PROTECTED";
  case LOCKDOWN_E_USER_DENIED_PAIRING:
    return "USER_DENIED_PAIRING";
  case LOCKDOWN_E_PAIRING_DIALOG_RESPONSE_PENDING:
    return "PAIRING_DIALOG_RESPONSE_PENDING";
  case LOCKDOWN_E_MISSING_HOST_ID:
    return "MISSING_HOST_ID";
  case LOCKDOWN_E_INVALID_HOST_ID:
    return "INVALID_HOST_ID";
  case LOCKDOWN_E_SESSION_ACTIVE:
    return "SESSION_ACTIVE";
  case LOCKDOWN_E_SESSION_INACTIVE:
    return "SESSION_INACTIVE";
  case LOCKDOWN_E_MISSING_SESSION_ID:
    return "MISSING_SESSION_ID";
  case LOCKDOWN_E_INVALID_SESSION_ID:
    return "INVALID_SESSION_ID";
  case LOCKDOWN_E_MISSING_SERVICE:
    return "MISSING_SERVICE";
  case LOCKDOWN_E_INVALID_SERVICE:
    return "INVALID_SERVICE";
  case LOCKDOWN_E_SERVICE_LIMIT:
    return "SERVICE_LIMIT";
  case LOCKDOWN_E_MISSING_PAIR_RECORD:
    return "MISSING_PAIR_RECORD";
  case LOCKDOWN_E_SAVE_PAIR_RECORD_FAILED:
    return "SAVE_PAIR_RECORD_FAILED";
  case LOCKDOWN_E_INVALID_PAIR_RECORD:
    return "INVALID_PAIR_RECORD";
  case LOCKDOWN_E_INVALID_ACTIVATION_RECORD:
    return "INVALID_ACTIVATION_RECORD";
  case LOCKDOWN_E_MISSING_ACTIVATION_RECORD:
    return "MISSING_ACTIVATION_RECORD";
  case LOCKDOWN_E_SERVICE_PROHIBITED:
    return "SERVICE_PROHIBITED";
  case LOCKDOWN_E_ESCROW_LOCKED:
    return "ESCROW_LOCKED";
  case LOCKDOWN_E_UNKNOWN_ERROR:
  default:
    return "UNKNOWN_EROR";
  }
}

// Cleanup function for when the JS object is garbage collected
static void Finalize(napi_env env, void *data, void *hint)
{
  ClientData *client_data = (ClientData *)data;

  if (client_data->afc)
  {
    afc_client_free(client_data->afc);
  }

  if (client_data->lockdown)
  {
    lockdownd_client_free(client_data->lockdown);
  }

  if (client_data->device)
  {
    idevice_free(client_data->device);
  }

  free(client_data);
}

// Function to initialize libimobiledevice and connect to a device
static napi_value Initialize(napi_env env, napi_callback_info info)
{
  napi_value jsobj;

  // Allocate memory for client data
  ClientData *client_data = malloc(sizeof(ClientData));
  if (!client_data)
  {
    napi_throw_error(env, NULL, "Failed to allocate memory");
    return NULL;
  }

  client_data->device = NULL;
  client_data->lockdown = NULL;
  client_data->afc = NULL;
  client_data->lockdown_service = NULL;
  client_data->house_arrest = NULL;

  idevice_error_t ret = idevice_new(&client_data->device, NULL);
  if (ret != IDEVICE_E_SUCCESS)
  {
    Finalize(env, client_data, NULL);
    napi_throw_error(env, NULL, "Failed to initialize libimobiledevice");
    return NULL;
  }

  // Create a lockdown client
  lockdownd_error_t lderr = lockdownd_client_new_with_handshake(client_data->device, &client_data->lockdown, "minepack");
  if (lderr != LOCKDOWN_E_SUCCESS)
  {
    Finalize(env, client_data, NULL);
    char error_msg[256];
    sprintf(error_msg, "Failed to create lockdown client: %s", idev_lockdownd_strerror(lderr));
    napi_throw_error(env, NULL, error_msg);
    return NULL;
  }

  // Create a lockdown service descriptor for house arrest
  lderr = lockdownd_start_service(client_data->lockdown, HOUSE_ARREST_SERVICE_NAME, &client_data->lockdown_service);
  if (lderr != LOCKDOWN_E_SUCCESS)
  {
    Finalize(env, client_data, NULL);
    char error_msg[256];
    sprintf(error_msg, "Failed to create lockdown service descriptor: %s", idev_lockdownd_strerror(lderr));
    napi_throw_error(env, NULL, error_msg);
    return NULL;
  }

  // Create an AFC client
  afc_error_t afcerr = afc_client_new(client_data->device, client_data->lockdown_service, &client_data->afc);
  if (afcerr != AFC_E_SUCCESS)
  {
    Finalize(env, client_data, NULL);
    napi_throw_error(env, NULL, "Failed to create AFC client");
    return NULL;
  }

  if (true)
  {

    // house_arrest_client_t ha_client=NULL;
    house_arrest_error_t ha_err = house_arrest_client_new(client_data->device, client_data->lockdown_service, &client_data->house_arrest);

    if (ha_err == HOUSE_ARREST_E_SUCCESS && client_data->house_arrest)
    {

      ha_err = house_arrest_send_command(client_data->house_arrest, "VendDocuments", "com.mojang.minecraftpe");

      if (ha_err == HOUSE_ARREST_E_SUCCESS)
      {
        plist_t dict = NULL;
        ha_err = house_arrest_get_result(client_data->house_arrest, &dict);

        if (ha_err == HOUSE_ARREST_E_SUCCESS && dict)
        {
          plist_t errnode = plist_dict_get_item(dict, "Error");

          if (!errnode)
          {
            // afc_client_t afc=NULL;
            afc_error_t afc_err = afc_client_new_from_house_arrest_client(client_data->house_arrest, &client_data->afc);

            if (afc_err == AFC_E_SUCCESS && client_data->afc)
            {
            }
            else
            {
              fprintf(stderr, "Error: could not get afc client from house arrest: %s\n", idev_afc_strerror(afc_err));
            }

            if (client_data->afc)
              afc_client_free(client_data->afc);
          }
          else
          {
            char *str = NULL;
            plist_get_string_val(errnode, &str);
            fprintf(stderr, "Error: house_arrest service responded: %s\n", str);
            if (str)
              free(str);
          }
        }
        else
        {
          fprintf(stderr, "Error: Could not get result form house_arrest service: %s\n",
                  idev_house_arrest_strerror(ha_err));
        }

        if (dict)
          plist_free(dict);
      }
      else
      {
        fprintf(stderr, "Error: Could not send %s command with argument:%s - %s\n",
                "VendDocuments", "com.mojang.minecraftpe", idev_house_arrest_strerror(ha_err));
      }
    }
    else
    {
      fprintf(stderr, "Error: Unable to create house arrest client: %s\n", idev_house_arrest_strerror(ha_err));
    }

    if (client_data->house_arrest)
      house_arrest_client_free(client_data->house_arrest);
  }
  else
  {
    // fprintf(stderr, "Error: unable to start service: %s - %s\n", HOUSE_ARREST_SERVICE_NAME, idev_lockdownd_strerror(lret));
  }

  // Create JS object and wrap client data
  napi_create_object(env, &jsobj);
  napi_status status = napi_wrap(env, jsobj, client_data, Finalize, NULL, NULL);
  if (status != napi_ok)
  {
    Finalize(env, client_data, NULL);
    napi_throw_error(env, NULL, "Failed to wrap client data");
    return NULL;
  }

  return jsobj;
}

int static putFile(afc_client_t afc, char *srcPath, char *destPath)
{
  uint64_t handle = 0;

  FILE *file = fopen(srcPath, "rb");

  if (file)
  {
    afc_error_t err = afc_file_open(afc, destPath, AFC_FOPEN_WRONLY, &handle);

    if (err != AFC_E_SUCCESS)
    {
      printf("Failed to open file: %s\n", destPath);
      return -1;
    }

    char buffer[4096];
    uint32_t bytesRead;
    uint32_t bytesWritten = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
      uint32_t _written = 0;
      err = afc_file_write(afc, handle, buffer, bytesRead, &_written);
      bytesWritten += _written;

      if (err != AFC_E_SUCCESS)
      {
        break;
      }
    }

    afc_file_close(afc, handle);
    fclose(file);

    if (err != AFC_E_SUCCESS || bytesWritten != bytesRead)
    {
      printf("Failed to write file: %s\n", destPath);
      return err;
    }
  }
  else
  {
    printf("Failed to open file: %s\n", srcPath);
    return -1;
  }

  return 0;
}

static napi_value PutFile(napi_env env, napi_callback_info info)
{
  napi_value result;
  size_t argc = 3; // Expecting 3 arguments now: client object, src path, and dest path
  napi_value argv[3];
  napi_value jsthis;

  // Get callback info
  napi_get_cb_info(env, info, &argc, argv, &jsthis, NULL);

  // Verify we received the expected number of arguments
  if (argc < 3)
  {
    napi_throw_error(env, NULL, "Wrong number of arguments. Expected 3 arguments (client, src path, dest path).");
    return NULL;
  }

  // Unwrap the client data from the first argument
  ClientData *client_data;
  napi_status status = napi_unwrap(env, argv[0], (void **)&client_data);
  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Failed to unwrap client data");
    return NULL;
  }

  // Get the src path string
  char srcPath[1024];
  char destPath[1024];
  size_t str_len;
  napi_get_value_string_utf8(env, argv[1], srcPath, sizeof(srcPath), &str_len);
  napi_get_value_string_utf8(env, argv[2], destPath, sizeof(destPath), &str_len);

  napi_create_string_utf8(env, srcPath, NAPI_AUTO_LENGTH, &result);

  // Now we can use client_data->device, client_data->lockdown, etc.
  // to implement the file sending functionality

  int err = putFile(client_data->afc, srcPath, destPath);

  if (err != 0)
  {
    napi_throw_error(env, NULL, "Failed to put file");
    return NULL;
  }

  return result;
}

static napi_value Init(napi_env env, napi_value exports)
{
  napi_property_descriptor desc[] = {
      {"initialize", NULL, Initialize, NULL, NULL, NULL, napi_default, NULL},
      {"putFile", NULL, PutFile, NULL, NULL, NULL, napi_default, NULL},
      // { "startAFC", NULL, StartAFC, NULL, NULL, NULL, napi_default, NULL },
      // { "listDirectory", NULL, ListDirectory, NULL, NULL, NULL, napi_default, NULL },
      // { "readFile", NULL, ReadFile, NULL, NULL, NULL, napi_default, NULL }
  };

  napi_define_properties(env, exports, 2, desc);
  return exports;
}

// Register the module
NAPI_MODULE(afc_client, Init)