// gccpch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently.
// gccpch.h.gch will contain the pre-compiled type information

#include <stdio.h>
#include "x3.h"
#include "xRegistry.h"
#include "cfw_settings.h"

#ifndef __GCCPCH__
#define __GCCPCH__

#define __VIEW__ "ACT0"

#define setNIDfunc(name, module, nid) (void*&)(name) = (void*)((int)getNIDfunc(module, nid))

// TODO: reference additional headers your program requires here
void *getNIDfunc(const char *vsh_module, uint32_t fnid);
int GetPluginInterface(const char *pluginname, int interface_);
int LoadPlugin(char *pluginname, void *handler);

int read_product_mode_flag(void *data);

// NID Functions prx
extern int (*FindPlugin)(const char *plugin);
extern int (*plugin_GetInterface)(int view,int interface);
extern int (*plugin_SetInterface)(int view, int interface, void *Handler);
extern int (*plugin_SetInterface2)(int view, int interface, void *Handler);

// NID Functions cfw_settings
extern int (*getDiscHashKey)(void*);
extern int (*authDisc)();
extern int (*cellFsUtilityMount)(const char *device_name, const char *device_fs, const char *device_path, int r6, int write_prot, int r8, int *r9);
extern int (*cellSsAimGetDeviceId)(void *buffer);
extern int (*cellSsAimGetOpenPSID)(void *buffer);
extern int (*Authenticate_BD_Drive)(int cmd);

extern int (*loadModule)(int *fd, char *path, int r5, int r6, int *memorycontainer);
extern int (*ejectDisc)();
extern int (*startJob)(void *job, int(*handler1)(), void *param1, int r6, int r7, uint8_t(*handler2)());
extern uint8_t *(*getLoadedPlugins)();

extern int (*cellCryptoPuAesCbcCfb128Encrypt)(void *out, void *in, uint32_t length, void *user_key, int bits, void *iv);
extern int (*cellCryptoPuAesCbcCfb128Decrypt)(void *out, void *in, uint32_t length, void *user_key, int bits, void *iv);
extern int (*cellCryptoPuAesEncKeySet)(AES_KEY *aes_key, const unsigned char *user_key, const int bits);
extern int (*cellCryptoPuAesOmac1Mode)(uint8_t *out, uint8_t *input, uint32_t length, uint8_t *key, uint32_t keybits);

extern int (*cellCryptoPuSha1Hmac)(uint8_t *hmac_hash, uint8_t *data_in, int32_t data_length, uint8_t *key, int32_t key_length);
extern int (*cellCryptoPuSha1HmacInit)(uint64_t data[160], uint8_t *key, int32_t key_length);
extern int (*cellCryptoPuSha1HmacTransform)(uint64_t data[160], uint8_t *data_in, int32_t data_length);
extern int (*cellCryptoPuSha1HmacFinal)(uint8_t *hmac_hash, uint64_t data[160]);
extern int (*cellCryptoPuSha1Hash)(uint8_t *out_sha1, uint8_t *in, uint32_t length);
extern int (*cellCryptoPuEccEcDsaVeri)(uint64_t ret, uint8_t *hash, uint8_t *public_key, uint8_t *curve);

extern int (*update_mgr_read_eprom)(int offset, void *buffer);      
extern int (*update_mgr_write_eprom)(int offset, int value);
extern int (*vshmain_74A54CBF)(int r3);
extern int (*vshmain_5F5729FB)(int r3);
extern int (*vshmain_25CE539E)(uint64_t *value);

extern int (*xBDVDGetInstance)();

extern xsetting_D0261D72_class*(*xSettingRegistryGetInterface)();
extern xsetting_AF1F161_class*(*xSettingSystemInfoGetInterface)();
extern xsetting_CC56EB2D_class*(*xUserGetInterface)();

extern xsetting_16A8A805_class*(*xSettingBdvdGetInterface)();
extern xsetting_8B69F85A_class*(*xSettingDateGetInterface)();
extern int (*_cellRtcGetCurrentTick)(uint64_t *pTick);
extern int (*_cellRtcSetCurrentTick)(uint64_t *pTick);
extern int (*_cellRtcSetCurrentSecureTick)(const uint64_t *secureTick);
extern int (*_cellRtcGetCurrentSecureTick)(const uint64_t *secureTick);
//static int (*_cellRtcGetCurrentClock)() = 0;

extern void (*NotifyWithTexture)(int32_t, const char *eventName, int32_t, int32_t *texture, int32_t*, const char*, const char*, float, const wchar_t *text, int32_t, int32_t, int32_t);
extern uint32_t (*FindTexture)(int32_t *texptr, uint32_t plugin, const char *name);

extern void (*_free)(void *);
extern void *(*_malloc)(size_t);
extern void *(*_memalign)(size_t boundary, size_t size_arg);

extern size_t (*wcstombs_)(char *dest, const wchar_t *src, size_t max);
extern uint64_t (*_stoull)(const char *ptr, char **endptr, int base);

extern FILE *(*_fopen)(const char *filename, const char *mode);
extern size_t *(*_fread)(void *pointer, size_t size, size_t nmemb, FILE *stream);
extern int (*_fclose)(FILE *stream);
extern int (*_fprintf)(FILE *stream, const char *format, ...);
extern char *(*_ctime)(const time_t *timer);

extern void (*xRegistrySetValue)(unsigned int handle, unsigned int path, unsigned int value, unsigned int size, int unk);
extern int (*xRegistryGetValue)(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6);
extern void (*xGetDefaultInstance)(int a1, int *a2, void *a3, void *a4, void *a5, void *a6);

extern int (*GetString)(int plugin, const char *text);
extern uint32_t (*sceNetCtlGetInfoVsh)(uint32_t size, const char *ip);

extern int (*cellFsUtilUmount)(const char *device_path, int r4);

class xai_plugin_interface
{
	public:	
		static void xai_plugin_init(int view);
		static int xai_plugin_start(void *view);
		static int xai_plugin_stop(void);
		static void xai_plugin_exit(void);
};

#endif
