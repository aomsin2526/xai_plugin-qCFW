#include <stdio.h>
#include "cfw_settings.h"
#include "xRegistry.h"

RegionCode regionPS3[14] =
{
	{ 0x00, "msg_default", 0, 0 },
	{ 0x83, "msg_japan", 2, 1 }, 
	{ 0x84, "msg_usa", 1, 1 },  
	{ 0x85, "msg_europe", 2, 2 }, 
	{ 0x86, "msg_korea", 3, 1 },
	{ 0x87, "msg_uk", 2, 2 }, 
	{ 0x88, "msg_mexico", 4, 1 }, 
	{ 0x89, "msg_australia", 4, 2 },
	{ 0x8A, "msg_asia", 3, 1 }, 
	{ 0x8B, "msg_taiwan", 3, 1 }, 
	{ 0x8C, "msg_russia", 5, 4 },
	{ 0x8D, "msg_china", 6, 4 }, 
	{ 0x8E, "msg_hongkong", 3, 1 }, 
	{ 0x8F, "msg_brazil", 4, 1},
};

DVDRegionCode dvd_video_region[6] =
{
	{ 1, "Region 1" }, 
	{ 2, "Region 2" },  
	{ 3, "Region 3" }, 
	{ 4, "Region 4" },
	{ 5, "Region 5" }, 
	{ 6, "Region 6" }, 
};

BDRegionCode bd_video_region[3] =
{
	{ 1, "Region A" }, 
	{ 2, "Region B" },  
	{ 4, "Region C" }, 
};

uint8_t eid2_indiv_seed_[0x40] = 
{		
	0x74, 0x92, 0xE5, 0x7C, 0x2C, 0x7C, 0x63, 0xF4, 0x49, 0x42, 0x26, 0x8F, 0xB4, 0x1C, 0x58, 0xED, 
    0x66, 0x83, 0x41, 0xF9, 0xC9, 0x7B, 0x29, 0x83, 0x96, 0xFA, 0x9D, 0x82, 0x07, 0x51, 0x99, 0xD8, 
    0xBC, 0x1A, 0x93, 0x4B, 0x37, 0x4F, 0xA3, 0x8D, 0x46, 0xAF, 0x94, 0xC7, 0xC3, 0x33, 0x73, 0xB3, 
    0x09, 0x57, 0x20, 0x84, 0xFE, 0x2D, 0xE3, 0x44, 0x57, 0xE0, 0xF8, 0x52, 0x7A, 0x34, 0x75, 0x3D
};

int (*FindPlugin)(const char *plugin);
int (*plugin_GetInterface)(int view,int interface);
int (*plugin_SetInterface)(int view, int interface, void *Handler);
int (*plugin_SetInterface2)(int view, int interface, void *Handler);

int (*getDiscHashKey)(void*);
int (*authDisc)();
int (*cellFsUtilityMount)(const char *device_name, const char *device_fs, const char *device_path, int r6, int write_prot, int r8, int *r9) = 0;
int (*cellSsAimGetDeviceId)(void *buffer) = 0;
int (*cellSsAimGetOpenPSID)(void *buffer) = 0;
int (*Authenticate_BD_Drive)(int cmd) = 0;

int (*loadModule)(int *fd, char *path, int r5, int r6, int *memorycontainer) = 0;
int (*ejectDisc)() = 0;
int (*startJob)(void *job, int(*handler1)(), void *param1, int r6, int r7, uint8_t(*handler2)()) = 0;
uint8_t *(*getLoadedPlugins)() = 0;

int (*cellCryptoPuAesCbcCfb128Encrypt)(void *out, void *in, uint32_t length, void *user_key, int bits, void *iv) = 0;
int (*cellCryptoPuAesCbcCfb128Decrypt)(void *out, void *in, uint32_t length, void *user_key, int bits, void *iv) = 0;
int (*cellCryptoPuAesEncKeySet)(AES_KEY *aes_key, const unsigned char *user_key, const int bits) = 0;
int (*cellCryptoPuAesOmac1Mode)(uint8_t *out, uint8_t *input, uint32_t length, uint8_t *key, uint32_t keybits) = 0;

int (*cellCryptoPuSha1Hmac)(uint8_t *hmac_hash, uint8_t *data_in, int32_t data_length, uint8_t *key, int32_t key_length) = 0;
int (*cellCryptoPuSha1HmacInit)(uint64_t data[160], uint8_t *key, int32_t key_length) = 0;
int (*cellCryptoPuSha1HmacTransform)(uint64_t data[160], uint8_t *data_in, int32_t data_length) = 0;
int (*cellCryptoPuSha1HmacFinal)(uint8_t *hmac_hash, uint64_t data[160]) = 0;
int (*cellCryptoPuSha1Hash)(uint8_t *out_sha1, uint8_t *in, uint32_t length) = 0;
int (*cellCryptoPuEccEcDsaVeri)(uint64_t ret, uint8_t *hash, uint8_t *public_key, uint8_t *curve) = 0;

int (*update_mgr_read_eprom)(int offset, void *buffer);      
int (*update_mgr_write_eprom)(int offset, int value);
int (*vshmain_74A54CBF)(int r3) = 0;
int (*vshmain_5F5729FB)(int r3) = 0;
int (*vshmain_25CE539E)(uint64_t *value) = 0;

int (*xBDVDGetInstance)();

xsetting_D0261D72_class*(*xSettingRegistryGetInterface)() = 0;
xsetting_AF1F161_class*(*xSettingSystemInfoGetInterface)() = 0;
xsetting_CC56EB2D_class*(*xUserGetInterface)() = 0;

xsetting_16A8A805_class*(*xSettingBdvdGetInterface)() = 0;
xsetting_8B69F85A_class*(*xSettingDateGetInterface)() = 0;
int (*_cellRtcGetCurrentTick)(uint64_t *pTick) = 0;
int (*_cellRtcSetCurrentTick)(uint64_t *pTick) = 0;
int (*_cellRtcSetCurrentSecureTick)(const uint64_t *secureTick) = 0;
int (*_cellRtcGetCurrentSecureTick)(const uint64_t *secureTick) = 0;

void (*NotifyWithTexture)(int32_t, const char *eventName, int32_t, int32_t *texture, int32_t*, const char*, const char*, float, const wchar_t *text, int32_t, int32_t, int32_t);
uint32_t (*FindTexture)(int32_t *texptr, uint32_t plugin, const char *name);

void (*_free)(void *);
void *(*_malloc)(size_t);
void *(*_memalign)(size_t boundary, size_t size_arg);

size_t (*wcstombs_)(char *dest, const wchar_t *src, size_t max);
uint64_t (*_stoull)(const char *ptr, char **endptr, int base);

FILE *(*_fopen)(const char *filename, const char *mode);
size_t *(*_fread)(void *pointer, size_t size, size_t nmemb, FILE *stream);
int (*_fclose)(FILE *stream);
int (*_fprintf)(FILE *stream, const char *format, ...);
char *(*_ctime)(const time_t *timer);

void (*xRegistrySetValue)(unsigned int handle, unsigned int path, unsigned int value, unsigned int size, int unk) = 0;
int (*xRegistryGetValue)(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6) = 0;
void (*xGetDefaultInstance)(int a1, int *a2, void *a3, void *a4, void *a5, void *a6) = 0;

int (*GetString)(int plugin, const char *text);
uint32_t (*sceNetCtlGetInfoVsh)(uint32_t size, const char *ip);

int (*cellFsUtilUmount)(const char *device_path, int r4) = 0;
