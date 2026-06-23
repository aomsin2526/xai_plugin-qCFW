#ifndef _CFW_SETTINGS_H
#define _CFW_SETTINGS_H

#define DEV_BLIND						"/dev_blind"

#define IDPS_TMP_FILE					"/dev_hdd0/tmp/IDPS.txt"
#define PSID_TMP_FILE					"/dev_hdd0/tmp/PSID.txt"
#define MAC_TMP_FILE					"/dev_hdd0/tmp/MAC.txt"

#define USB_KERNEL						"/dev_usb000/lv2_kernel.self"
#define DBLIND_KERNEL					"/dev_blind/lv2_kernel.self"
#define FLASH_KERNEL					"/dev_flash/lv2_kernel.self"

#define EXT_COBRA						"/dev_blind/sys/ext_cobra"

#define STAGE2_BIN_ENABLED				"/dev_blind/sys/stage2.bin"
#define STAGE2_BIN_DISABLED				"/dev_blind/sys/stage2.bin.bak"
#define STAGE2_BIN_RELEASE				"/dev_blind/sys/stage2.bin.release"
#define STAGE2_BIN_DEBUG				"/dev_blind/sys/stage2.bin.debug"

#define STAGE2_EVILNAT_CEX_ENABLED		"/dev_blind/sys/stage2.cex"
#define STAGE2_EVILNAT_CEX_DISABLED		"/dev_blind/sys/stage2.cex.bak"
#define STAGE2_EVILNAT_CEX_RELEASE		"/dev_blind/sys/stage2.cex.release"
#define STAGE2_EVILNAT_CEX_DEBUG  		"/dev_blind/sys/stage2.cex.debug"
#define STAGE2_EVILNAT_DEX_ENABLED		"/dev_blind/sys/stage2.dex"
#define STAGE2_EVILNAT_DEX_DISABLED		"/dev_blind/sys/stage2.dex.bak"
#define STAGE2_EVILNAT_DEX_RELEASE		"/dev_blind/sys/stage2.dex.release"
#define STAGE2_EVILNAT_DEX_DEBUG  		"/dev_blind/sys/stage2.dex.debug"

#define NPSIGNIN_LCK					"/dev_blind/vsh/resource/npsignin_plugin.lck"
#define NPSIGNIN_LCK_DISABLED			"/dev_blind/vsh/resource/npsignin_plugin.lck.bak"

#define RCO_DATE						"/dev_blind/vsh/resource/explore_plugin_full.rco.date"
#define RCO_ORI							"/dev_blind/vsh/resource/explore_plugin_full.rco.ori"
#define RCO_TEMP						"/dev_blind/vsh/resource/explore_plugin_full.rco.tmp"
#define RCO_DEFAULT						"/dev_blind/vsh/resource/explore_plugin_full.rco"

#define REGISTORE_XML					"/dev_blind/vsh/resource/explore/xmb/registory.xml"
#define REGISTORE_XML_TMP				"/dev_blind/vsh/resource/explore/xmb/registory.xml.tmp"

#define TMP_FOLDER						"/dev_hdd0/tmp"
#define LV2_DUMP						"LV2-FW%X.%X%X.bin"
#define LV1_DUMP						"LV1-FW%X.%X%X.bin"
#define RAM_DUMP						"RAM-FW%X.%X%X.bin"
#define VFLASH_DUMP						"VFLASH-FW%X.%X%X.bin"
#define NOR_DUMP						"FLASH-NOR-FW%X.%X%X.bin"
#define NAND_DUMP						"FLASH-NAND-FW%X.%X%X.bin"

#define XREGISTRY_FILE					"/dev_flash2/etc/xRegistry.sys"
#define XREGISTRY_BACKUP_FILE			"/dev_flash2/etc/backup/xRegistry.sys"
#define ACT_DAT_PATH					"/dev_hdd0/home/%08d/exdata/act.dat"

#define TEX_ERROR						"tex_error_ws"
#define TEX_SUCCESS						"tex_check2_ws"
#define TEX_WARNING						"tex_warning"
#define TEX_INFO						"tex_notification_info"
#define TEX_INFO2						"tex_notification_info2"
#define XAI_PLUGIN						"xai_plugin"

#define FTP_SRPX						"/dev_flash/vsh/module/ftp.sprx"
#define FTPD							"FTPD"
#define TROPHYUNLOCKER_SRPX				"/dev_flash/vsh/module/trophy_unlocker_plugin.sprx"
#define TROPHYUNLOCKER					"pluginLoader"
#define MAX_BOOT_PLUGINS 				7

#define WEBMAN_CFG						"/dev_hdd0/tmp/wm_config.bin"

#define LIBAUDIO_SPRX					"/dev_blind/sys/external/libaudio.sprx"
#define LIBAUDIO_ORIGINAL				"/dev_blind/sys/external/libaudio.sprx.ori"
#define LIBAUDIO_PATCHED				"/dev_blind/sys/external/libaudio.sprx.patched"
#define RAP_BIN_HDD_PATH				"/dev_hdd0/exdata/rap.bin"

#define DEV_USB_FIND					"/dev_usb%03d"
#define DEV_USB_EXDATA					"/dev_usb%03d/exdata"
#define DEV_HDD0_EXDATA					"/dev_hdd0/exdata"
#define HOME_EXDATA						"/dev_hdd0/home/%08d/exdata"

#define DEV_HDD0_ACT					"/dev_hdd0/tmp/act.dat"
#define EXDATA_RIF_FILE					"/dev_hdd0/home/%08d/exdata/%s.rif"

#define LINES_QRC_DEFAULT				"/dev_blind/vsh/resource/qgl/lines.qrc"
#define LINES_QRC_ORI					"/dev_blind/vsh/resource/qgl/lines.qrc.ori"
#define LINES_QRC_MOD					"/dev_blind/vsh/resource/qgl/lines.qrc.mod"

#define FAN_BIN_NE_FILE					"/dev_hdd0/vm/ne/fan.bin"
#define FAN_BIN_GX_FILE					"/dev_hdd0/vm/gx/fan.bin"

#define BDVD_DEV_ID					0x101000000000006ULL

#define LV2							0
#define LV1							1
#define RAM							2

#define SYSCALL_TABLE				0x8000000000363BE0ULL
#define DISABLED					0xFFFFFFFF80010003ULL

#define PRODUCT_MODE_FLAG_OFFSET	0x48C07
#define RECOVERY_MODE_FLAG_OFFSET	0x48C61

#define DUMP_OFFSET					0x2401FC00000ULL
#define DUMP_SIZE					0x40000ULL

#define PRODG_PATCH_OFFSET			0x8000000000003B38ULL
#define PRODG_PATCH					0x386000014E800020ULL

#define START_OFFSET_MAC			0x8000000000070000ULL
#define END_OFFSET_MAC     			0x8000000000100000ULL

#define REDUMP_WATERMARK_OFFSET		0xF70	
#define REDUMP_KEY_OFFSET			0xF80	

#define RAP2BIN_HDD					0
#define RAP2BIN_USB					1

#define ACT_DAT_SIZE				0x1038

typedef struct
{
	int ps3_region;
	char *region;
	uint32_t dvd_region;
	uint32_t bd_region;
} RegionCode;

typedef struct
{
	uint32_t dvd_region;
	char *region;
} DVDRegionCode;

typedef struct
{
	uint32_t bd_region;
	char *region;
} BDRegionCode;

typedef struct _PS3RegionInfo
{
	int isEncrypted;
	uint64_t first_address_region;
	uint64_t last_address_region;
} PS3RegionInfo;

extern RegionCode regionPS3[14];
extern DVDRegionCode dvd_video_region[6];
extern BDRegionCode bd_video_region[3];
extern uint8_t eid2_indiv_seed_[0x40];

struct pblock_aes_struct
{
	uint8_t pblock_hdr[0x10];
	uint8_t pblock_des[0x60];
	uint8_t pblock_hash[0x10];
};

struct sblock_aes_struct
{
	uint8_t sblock_hdr[0x10];
	uint8_t sblock_des[0x670];
	uint8_t sblock_hash[0x10];
};

struct eid2_struct
{
	unsigned short pblock_size;
	unsigned short sblock_size;
	uint8_t padding[0x1C];  // 00.... 00 00 / 00 03
	pblock_aes_struct pblock_aes;
	sblock_aes_struct sblock_aes;
};

struct inquiry_block 
{
    uint8_t pkt[0x20];		/* packet command block */ 
    uint32_t pktlen;  
    uint32_t blocks;					
    uint32_t block_size;				
    uint32_t proto;			/* transfer mode */ 
    uint32_t in_out;		/* transfer direction */ 
    uint32_t unknown;
};

enum lv2_atapi_proto 
{
    NON_DATA_PROTO = 0,
    PIO_DATA_IN_PROTO = 1,
    PIO_DATA_OUT_PROTO = 2,
    DMA_PROTO = 3
};

enum lv2_atapi_in_out 
{
    DIR_WRITE = 0,		/* memory -> device */
    DIR_READ = 1		/* device -> memory */
};

struct lv2_atapi_cmnd_block 
{
    uint8_t pkt[0x20];		/* packet command block  */ 
    uint32_t pktlen;  
    uint32_t blocks;					
    uint32_t block_size;				
    uint32_t proto;			/* transfer mode */ 
    uint32_t in_out;		/* transfer direction */ 
    uint32_t unknown;
} __attribute__((packed));

typedef struct _SHACtx
{
	uint8_t data[0x100];
} SHACtx;

#define AES_MAXNR		14

struct aes_key_st 
{
	uint32_t rd_key[4 * (AES_MAXNR + 1)];
	int rounds;
};

typedef struct aes_key_st AES_KEY;

typedef struct function_descriptor 
{
	void	*addr;
	void    *toc;	
} f_desc_t;

int AesCbcCfbEncrypt(void *out, void *in, uint32_t length, void *user_key, int bits, void *iv);
int AesCbcCfbDecrypt(void *out, void *in, uint32_t length, void *user_key, int bits, void *iv);
int aes_omac1(uint8_t *out, uint8_t *input, uint32_t length, uint8_t *key, uint32_t keybits);

int sha1_hmac(uint8_t *hmac_hash, uint8_t *data_in, int32_t data_length, uint8_t *key, int32_t key_length);
int sha1_hmac_starts(uint64_t data[160], uint8_t *key, int32_t key_length);
int sha1_hmac_update(uint64_t data[160], uint8_t *data_in, int32_t data_length);
int sha1_hmac_finish(uint8_t *hmac_hash, uint64_t data[160]);
int sha1_hash(uint8_t *out_sha1, uint8_t *in, uint32_t length);
int verify_ecdsa(uint8_t signature, uint8_t *hash, uint8_t *public_key, int curve);

void load_cfw_functions();
int RetrieveString(const char *string, const char *plugin);
void PrintString(wchar_t *string, const char *plugin, const char *tex_icon);

int saveFile(const char *path, void *data, size_t size);
int readFile(const char *file, uint8_t *buffer, size_t size);
int get_usb_device();

void showMessage(const char *string, const char *plugin, const char *tex_icon);
void customMessage(const char *msg_string, const char *text, const char *mode);
void customMessage(const char *msg_string, int data, const char *mode);
void customMessage2(const char *msg_string, const char *msg_string2, const char *mode);

int patch_savedata();
int create_rifs();
int getAccountID();
void changeAccountID(int mode, int force);

void backup_license();
void remove_license();

int create_syscalls();
int dump_lv(int lv);
int dumpERK(int mode);
int dump_sysrom();
int removeSysHistory();
//void checkSyscall(int syscall);

int set_qa(int value);

void show_cobra_info();
int save_cobra_fan_cfg(int mode);
int save_ps2_fan_cfg(int mode);
int save_ps2_dynamic_cfg(int mode);

//void allow_restore_sc();
//void skip_existing_rif();
//void toogle_PS2_disc_icon();
void toggle_ofw_mode();
void toggle_gameboot();
void toggle_epilepsy_warning();
void toggle_coldboot_animation();
void toggle_hidden_trophy_patch();

void clean_log();
void log_klic();

void log_secureid();
void enable_recording();
void enable_screenshot();

void override_sfo();

int toggle_coldboot();
void button_assignment();

void toggle_dlna();

bool enable_hvdbg();

void backup_registry();
void usb_firm_loader();

bool rsod_fix();
int loadKernel();
int filecopy(const char *src, const char *dst);

void remarry_bd();
void toggle_devblind();
void dump_disc_key();

int dump_ids();

void rebuild_db();

void recovery_mode();
int service_mode();

void applicable_version();
int activate_account();

void check_8th_spe();
void toggle_8th_spe();

void enable_WhatsNew();

void setLed(const char *mode);

void close_xml_list();

//int load_ftp();
//int unload_ftp();
int toggle_trophy_unlocker(int mode);

void spoof_idps();
void spoof_psid();

int Patch_ProDG();
void unlock_hdd_space();

void getPS3Lifetime();

int toggle_npsignin_lck();
void sm_error_log();
void get_token_seed();
void check_ros_bank();

int get_temperature_data();

int spoof_mac();
void show_ids();
void show_ip();
int patch_xreg_value(char *str, uint32_t value);

void set_region_default();
void toggle_dvdtvsys();
void check_region_values();
void set_region(int region, uint32_t dvd_region, uint32_t bd_region, uint32_t tvSystem);

void Fix_CBOMB();
int toggle_demo();
int toggle_screenshot();
void decryptRedumpISO(int src);
int swap_libaudio();

void show_bd_info();

int rap2bin();
int bin2rap();

void toggle_xmb_waves();

#endif /* _CFW_SETTINGS_H */
