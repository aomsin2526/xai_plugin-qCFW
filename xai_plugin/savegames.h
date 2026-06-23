#ifndef _SAVEGAMES_H__
#define _SAVEGAMES_H__

#define AUTOSIGN						0
#define ACCOUNTID						1

#define READ 							0
#define WRITE 							1
#define EMPTY							2

#define HEADER_SIZE 					0x60
#define Y_TABLE_OFFSET  				0x7B60

#define COPY_PROTECTION_OFFSET 			0x150
#define USER_ID_1_OFFSET 				0x570
#define USER_ID_2_OFFSET  				0x584

#define XREGISTRY_FILE 					"/dev_flash2/etc/xRegistry.sys"
#define XREGISTRY_FILE_SIZE				0x40000

#define LOGINUSERID 					"/setting/user/lastLoginUserId"
#define SETTING_AUTOSIGN 				"/setting/user/%08d/npaccount/autoSignInEnable"
#define SETTING_ACCOUNTID 				"/setting/user/%08d/npaccount/accountid"

  #define SWAP32(value)               \
   ((value & 0xff000000ull) >> 24)    \
   | ((value & 0x00ff0000ull) >> 8)   \
   | ((value & 0x0000ff00ull) << 8)   \
   | ((value & 0x000000ffull) << 24)

#define SWAP64(value)  						   \
	((value & 0xff00000000000000ull) >> 56)    \
	| ((value & 0x00ff000000000000ull) >> 40)  \
	| ((value & 0x0000ff0000000000ull) >> 24)  \
	| ((value & 0x000000ff00000000ull) >> 8 )  \
	| ((value & 0x00000000ff000000ull) << 8 )  \
	| ((value & 0x0000000000ff0000ull) << 24)  \
	| ((value & 0x000000000000ff00ull) << 40)  \
	| ((value & 0x00000000000000ffull) << 56)

struct rif_t
{
   uint32_t version;           // version
   uint32_t licenseType;       // license type
   uint64_t accountid;         // accountID
   char titleid[0x30];		   // Content ID
   uint8_t padding[0xC];       // Padding for randomness
   uint32_t actDatIndex;       // Key index on act.dat between 0x00 and 0x7F
   uint8_t key[0x10];          // encrypted klicensee
   uint64_t start_timestamp;   // timestamp of when the content was bought
   uint64_t expire_timestamp;  // timestamp for expiration of content (PS+ for example)
   uint8_t r[0x14];            // Unknown
   uint8_t s[0x14];            // Unknown
};

struct actdat_t
{
    uint32_t version;        // version
    uint32_t licenseType;    // license type
    uint64_t accountId;      // accountID
    uint8_t keyTable[0x800]; // Key Table
    uint8_t unknown[0x800];  // Unknown (timestamp,...)
    uint8_t signature[0x28]; // Signature
};

extern uint8_t empty[0x10];
extern uint8_t fake_accountid[0x10];

void load_saves_functions();
int search_data(char *buf, char *str, int type, int mode, int overwrite, int checkEmpty, uint8_t output[16]);
int patch_savedatas(const char *path);
int set_accountID(int mode, int overwrite);

int export_rap();

#endif /* _SAVEGAMES_H__ */