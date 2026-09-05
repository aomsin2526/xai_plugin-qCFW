#include <string.h>
#include <cell/fs/cell_fs_file_api.h>

#include "gccpch.h"

#include "functions.h"
#include "cfw_settings.h"
#include "log.h"

bool is_qcfw()
{
	return (lv1_peek(0x240) == 0x11223344aabbccddULL);
}

uint32_t qcfw_lv1_peek32(uint64_t addr)
{
	system_call_8(10, addr, 0, 0, 0, 0, 0, 0, 37);
	return_to_user_prog(uint32_t);
}

void qcfw_lv1_poke32(uint64_t addr, uint32_t val)
{
	system_call_8(10, addr, (uint64_t)val, 0, 0, 0, 0, 0, 38);
}

bool qcfw_is_nor()
{
	uint8_t flag;

	int32_t res = lv2_ss_get_cache_of_flash_ext_flag(&flag);
	if (res != 0)
		return false;

	return !(flag & 0x1);
}

bool qcfw_is_emmc()
{
	if (qcfw_is_nor())
		return false;

	int32_t res;

	uint64_t dev_id = 0x100000000000001ull;

	struct storage_device_info info;
	res = lv2_storage_get_device_info(dev_id, &info);
	if (res != 0)
		return false;

	return ((info.sector_size == 512) && (info.capacity == 0x1892e00));
}

bool qcfw_sc_read_hdd_key_dumper_flag(uint8_t* outValue)
{
	return update_mgr_read_eprom(0x3003, outValue) == 0;
}

bool qcfw_sc_write_hdd_key_dumper_flag(uint8_t value)
{
	return update_mgr_write_eprom(0x3003, value) == 0;
}

bool qcfw_sc_write_fsm_counter(uint8_t value)
{
	return update_mgr_write_eprom(0x3004, value) == 0;
}

bool qcfw_sc_read_nobd_toggle_flag(uint8_t* outValue)
{
	return update_mgr_read_eprom(0x3011, outValue) == 0;
}

bool qcfw_sc_write_nobd_toggle_flag(uint8_t value)
{
	return update_mgr_write_eprom(0x3011, value) == 0;
}

bool qcfw_sc_write_dex_spoof_flag(uint8_t value)
{
	return update_mgr_write_eprom(0x3012, value) == 0;
}

bool qcfw_sc_read_lv0ldr_region_dump_status(uint8_t* outValue)
{
	return update_mgr_read_eprom(0x3013, outValue) == 0;
}

bool qcfw_sc_read_ata_data_key(uint8_t* outValue) // [32]
{
	for (uint32_t i = 0; i < 32; ++i)
	{
		if (update_mgr_read_eprom((0x3020 + i), &outValue[i]) != 0)
			return false;
	}

	return true;
}

bool qcfw_sc_read_ata_tweak_key(uint8_t* outValue) // [32]
{
	for (uint32_t i = 0; i < 32; ++i)
	{
		if (update_mgr_read_eprom((0x3040 + i), &outValue[i]) != 0)
			return false;
	}

	return true;
}

bool qcfw_sc_read_encdec_data_key(uint8_t* outValue) // [32]
{
	for (uint32_t i = 0; i < 32; ++i)
	{
		if (update_mgr_read_eprom((0x3060 + i), &outValue[i]) != 0)
			return false;
	}

	return true;
}

bool qcfw_sc_read_encdec_tweak_key(uint8_t* outValue) // [32]
{
	for (uint32_t i = 0; i < 32; ++i)
	{
		if (update_mgr_read_eprom((0x3080 + i), &outValue[i]) != 0)
			return false;
	}

	return true;
}

bool qcfw_sc_read_lv0ldr_region_crc32(uint32_t* outValue)
{
	uint8_t* p = (uint8_t*)outValue;

	for (uint32_t i = 0; i < 4; ++i)
	{
		if (update_mgr_read_eprom((0x30c8 + i), &p[i]) != 0)
			return false;
	}

	return true;
}

void qcfw_install_otheros()
{
	if (!is_qcfw())
		return;

	CellFsStat stat;
	if (cellFsStat("/dev_usb000/dtbImage.ps3.zfself", &stat) != CELL_FS_SUCCEEDED)
	{
		PrintString(L"File not found!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	mount_dev_blind();

	cellFsUnlink("/dev_blind/ps2emu/ps2_emu.self");
	cellFsUnlink("/dev_blind/ps2emu/ps2_gxemu.self");

	cellFsUnlink("/dev_blind/sys/dtbImage.ps3.zfself");

	PrintString(L"Copying...", XAI_PLUGIN, TEX_INFO2);

	if (filecopy("/dev_usb000/dtbImage.ps3.zfself", "/dev_blind/sys/dtbImage.ps3.zfself"))
	{
		cellFsUnlink("/dev_blind/sys/dtbImage.ps3.zfself");

		PrintString(L"Copy failed!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	PrintString(L"Success!", XAI_PLUGIN, TEX_SUCCESS);
}

void qcfw_boot_otheros()
{
	if (!is_qcfw())
		return;

	CellFsStat stat;
	if (cellFsStat("/dev_flash/sys/dtbImage.ps3.zfself", &stat) != CELL_FS_SUCCEEDED)
	{
		PrintString(L"Install first!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	PrintString(L"Booting...", XAI_PLUGIN, TEX_INFO2);

	// 0x2F666C68 0x2F6F732F 0x6C76325F 0x6B65726E
	// /flh/os/lv2_kern

	{
		bool foundAddr = false;
		uint64_t addr = 0;

		for (uint64_t i = 0; i < (16 * 1024 * 1024); i += 4)
		{
			uint64_t v = qcfw_lv1_peek32(i);
			if (v != 0x2F666C68)
				continue;

			v = qcfw_lv1_peek32(i + 4);
			if (v != 0x2F6F732F)
				continue;

			v = qcfw_lv1_peek32(i + 8);
			if (v != 0x6C76325F)
				continue;

			v = qcfw_lv1_peek32(i + 12);
			if (v != 0x6B65726E)
				continue;

			foundAddr = true;
			addr = i;
			break;
		}

		if (!foundAddr)
		{
			PrintString(L"Offset not found!", XAI_PLUGIN, TEX_ERROR);
			return;
		}

		// 0x2F6C6F63 0x616C5F73 0x7973302F 0x7379732F 0x64746249 0x6D616765 0x2E707333 0x2E7A6673 0x656C6600
		// /local_sys0/sys/dtbImage.ps3.zfself

		qcfw_lv1_poke32(addr, 0x2F6C6F63);
		qcfw_lv1_poke32(addr + 4, 0x616C5F73);
		qcfw_lv1_poke32(addr + 8, 0x7973302F);
		qcfw_lv1_poke32(addr + 12, 0x7379732F);
		qcfw_lv1_poke32(addr + 16, 0x64746249);
		qcfw_lv1_poke32(addr + 20, 0x6D616765);
		qcfw_lv1_poke32(addr + 24, 0x2E707333);
		qcfw_lv1_poke32(addr + 28, 0x2E7A6673);
		qcfw_lv1_poke32(addr + 32, 0x656C6600);
	}

	{
		// 0xF821FF51 0x7C0802A6 0xFB610088 0xFB810090 0xFBC100A0 0x7C7C1B78

		bool foundAddr = false;
		uint64_t addr = 0;

		for (uint64_t i = 0; i < (16 * 1024 * 1024); i += 4)
		{
			uint64_t v = qcfw_lv1_peek32(i);
			if (v != 0xF821FF51)
				continue;

			v = qcfw_lv1_peek32(i + 4);
			if (v != 0x7C0802A6)
				continue;

			v = qcfw_lv1_peek32(i + 8);
			if (v != 0xFB610088)
				continue;

			v = qcfw_lv1_peek32(i + 12);
			if (v != 0xFB810090)
				continue;

			v = qcfw_lv1_peek32(i + 16);
			if (v != 0xFBC100A0)
				continue;

			v = qcfw_lv1_peek32(i + 20);
			if (v != 0x7C7C1B78)
				continue;

			foundAddr = true;
			addr = i;
			break;
		}

		if (!foundAddr)
		{
			PrintString(L"Offset2 not found!", XAI_PLUGIN, TEX_ERROR);
			return;
		}

		static const uint8_t stage4j[] = {
			0x38, 0x21, 0xFF, 0x80, 0x7C, 0x08, 0x02, 0xA6, 0xF8, 0x01, 0x00, 0x00, 0xF8, 0x61, 0x00, 0x08, 0xF8, 0x81, 0x00, 0x10, 0xF8, 0xA1, 0x00, 0x18, 0x3D, 0x60, 0x00, 0x01, 0x61, 0x6B, 0x00, 0x03, 0x38, 0x80, 0x69, 0x96, 0x38, 0xA0, 0x00, 0x20,
			0x38, 0xC0, 0x11, 0x11, 0x7C, 0x00, 0x04, 0xAC, 0x44, 0x00, 0x00, 0x02, 0xE8, 0x61, 0x00, 0x18, 0x39, 0x00, 0x00, 0x00, 0x39, 0x20, 0x00, 0x00, 0xE9, 0x43, 0x00, 0x10, 0x7D, 0x29, 0x52, 0x14, 0xE9, 0x43, 0x00, 0x18, 0x7D, 0x29, 0x52, 0x14,
			0x38, 0x80, 0x69, 0x96, 0x3C, 0xC8, 0x0C, 0x00, 0x7D, 0x43, 0x42, 0x14, 0x28, 0x29, 0x00, 0x07, 0x41, 0x81, 0x00, 0xEC, 0x38, 0xA0, 0x00, 0x08, 0x88, 0xEA, 0x00, 0x00, 0x39, 0x40, 0x00, 0x01, 0xF8, 0x01, 0x00, 0x20, 0xF9, 0x01, 0x00, 0x28,
			0xF9, 0x21, 0x00, 0x30, 0xF9, 0x41, 0x00, 0x38, 0xF8, 0x61, 0x00, 0x40, 0x7C, 0x00, 0x04, 0xAC, 0x44, 0x00, 0x00, 0x02, 0xE8, 0x61, 0x00, 0x40, 0xE9, 0x41, 0x00, 0x38, 0xE9, 0x21, 0x00, 0x30, 0xE9, 0x01, 0x00, 0x28, 0xE8, 0x01, 0x00, 0x20,
			0x7D, 0x08, 0x52, 0x14, 0x7D, 0x2A, 0x48, 0x50, 0x38, 0x00, 0x00, 0x00, 0x7C, 0x09, 0x00, 0x00, 0x41, 0x82, 0x00, 0x08, 0x4B, 0xFF, 0xFF, 0x9C, 0x3D, 0x60, 0x00, 0x01, 0x61, 0x6B, 0x00, 0x03, 0x38, 0x80, 0x69, 0x96, 0x38, 0xA0, 0x00, 0x20,
			0x38, 0xC0, 0x22, 0x22, 0x7C, 0x00, 0x04, 0xAC, 0x44, 0x00, 0x00, 0x02, 0xE9, 0x41, 0x00, 0x10, 0x3C, 0x60, 0x4C, 0x4F, 0x60, 0x63, 0x41, 0x44, 0x90, 0x6A, 0x00, 0x00, 0x39, 0x4A, 0x00, 0x04, 0x3C, 0x60, 0x4D, 0x45, 0x60, 0x63, 0x58, 0x58,
			0x90, 0x6A, 0x00, 0x00, 0x39, 0x4A, 0x00, 0x04, 0x3C, 0x60, 0x58, 0x58, 0x60, 0x63, 0x4C, 0x4F, 0x90, 0x6A, 0x00, 0x00, 0x39, 0x4A, 0x00, 0x04, 0x3C, 0x60, 0x41, 0x44, 0x60, 0x63, 0x4D, 0x45, 0x90, 0x6A, 0x00, 0x00, 0x38, 0x80, 0x69, 0x96,
			0x38, 0xA0, 0x00, 0x21, 0x7C, 0x00, 0x04, 0xAC, 0x44, 0x00, 0x00, 0x02, 0x38, 0x80, 0x69, 0x96, 0x38, 0xA0, 0x00, 0x20, 0x38, 0xC0, 0x33, 0x33, 0x7C, 0x00, 0x04, 0xAC, 0x44, 0x00, 0x00, 0x02, 0xE8, 0x01, 0x00, 0x00, 0x7C, 0x08, 0x03, 0xA6,
			0x38, 0x21, 0x00, 0x80, 0x38, 0x60, 0x00, 0x00, 0x4E, 0x80, 0x00, 0x20, 0x38, 0xA0, 0x00, 0x02, 0xE8, 0xEA, 0x00, 0x00, 0x39, 0x40, 0x00, 0x08, 0x4B, 0xFF, 0xFF, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7A, 0x52, 0x00, 0x04, 0x78, 0x41, 0x01, 0x1B, 0x0C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x18, 0xFF, 0xFF, 0xFE, 0x7C, 0x00, 0x00, 0x01, 0x68, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		for (uint64_t i = 0; i < sizeof(stage4j); i += 4)
			qcfw_lv1_poke32(addr + i, *(const uint32_t*)(&stage4j[i]));
	}

	rebootXMB(SYS_LV2_REBOOT);
}

void qcfw_dump_hdd_key()
{
	if (!qcfw_sc_write_hdd_key_dumper_flag(0x1))
		return;

	rebootXMB(SYS_SOFT_REBOOT);
}

bool qcfw_save_to_file(const char* fName, const void* data, uint32_t dataSize)
{
	int32_t fd;

	if (cellFsOpen(fName, CELL_FS_O_CREAT | CELL_FS_O_TRUNC | CELL_FS_O_RDWR, &fd, 0, 0) != CELL_FS_SUCCEEDED)
		return false;

	cellFsChmod(fName, 0777);

	uint64_t writeSuccessSize = 0;
	if (dataSize > 0)
		cellFsWrite(fd, data, dataSize, &writeSuccessSize);

	cellFsClose(fd);
	return (writeSuccessSize == dataSize);
}

void qcfw_save_hdd_key_to_usb()
{
	uint8_t hdd_key_dumper_flag = 0xff;

	if (!qcfw_sc_read_hdd_key_dumper_flag(&hdd_key_dumper_flag))
		return;

	if (hdd_key_dumper_flag != 0xfe)
	{
		PrintString(L"Dump first!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	uint8_t ata_data_key[32];
	if (!qcfw_sc_read_ata_data_key(ata_data_key))
		return;

	uint8_t ata_tweak_key[32];
	if (!qcfw_sc_read_ata_tweak_key(ata_tweak_key))
		return;

	uint8_t encdec_data_key[32];
	if (!qcfw_sc_read_encdec_data_key(encdec_data_key))
		return;

	uint8_t encdec_tweak_key[32];
	if (!qcfw_sc_read_encdec_tweak_key(encdec_tweak_key))
		return;

	uint8_t atakey_bin[32];
	memcpy(&atakey_bin[0], &ata_data_key[0], 16);
	memcpy(&atakey_bin[16], &ata_tweak_key[0], 16);

	uint8_t vflashkey_bin[32];
	memcpy(&vflashkey_bin[0], &encdec_data_key[0], 16);
	memcpy(&vflashkey_bin[16], &encdec_tweak_key[0], 16);

	if (!qcfw_save_to_file("/dev_usb000/ata_data_key.bin", ata_data_key, 32))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	if (!qcfw_save_to_file("/dev_usb000/ata_tweak_key.bin", ata_tweak_key, 32))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	if (!qcfw_save_to_file("/dev_usb000/encdec_data_key.bin", encdec_data_key, 32))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	if (!qcfw_save_to_file("/dev_usb000/encdec_tweak_key.bin", encdec_tweak_key, 32))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	if (!qcfw_save_to_file("/dev_usb000/ata_key.bin", atakey_bin, 32))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	if (!qcfw_save_to_file("/dev_usb000/vflash_key.bin", vflashkey_bin, 32))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return;
	}

	PrintString(L"Success!", XAI_PLUGIN, TEX_SUCCESS);
}

// initial crc should be 0
uint32_t qcfw_crc32c(uint32_t crc, const uint8_t* buf, size_t len)
{
	int32_t k;

	crc = ~crc;
	while (len--) {
		crc ^= *buf++;
		for (k = 0; k < 8; k++)
			crc = crc & 1 ? (crc >> 1) ^ 0xedb88320 : crc >> 1;
	}
	return ~crc;
}

bool qcfw_nor_read(uint64_t offset, void* data, uint32_t size, uint32_t burst_size)
{
	uint8_t* dataa = (uint8_t*)data;

	if (data == NULL)
		return false;

	if (size == 0)
		return true;

	if ((offset + size) > (16 * 1024 * 1024))
		return false;

	if (!qcfw_is_nor())
		return false;

	int32_t res;

	uint32_t unknown2;

	uint64_t dev_id = 0x100000000000004ull;
	uint64_t dev_flags = 0x22ull;

	static const uint32_t sector_size = 512;

	if ((burst_size == 0) || ((burst_size % sector_size) != 0))
		return false;

	uint32_t dev_handle;

	res = lv2_storage_open(dev_id, &dev_handle);
	if (res != 0)
		return false;

	uint8_t buf[sector_size];

	uint32_t curOffset = offset;
	uint32_t curDataOffset = 0;

	uint32_t left = size;

	while (left > 0)
	{
		uint32_t processSize = (left > sector_size) ? sector_size : left;
		uint32_t zzz = (curOffset % sector_size);
		uint32_t yyy = (sector_size - zzz);
		uint32_t xxx = (yyy > processSize) ? processSize : yyy;

		uint32_t sector_idx = (curOffset / sector_size);

		while (burst_size > left)
			burst_size -= sector_size;

		if ((zzz != 0) || (processSize != sector_size))
		{
			res = lv2_storage_read(dev_handle, 0, sector_idx, 1, buf, &unknown2, dev_flags);

			if (res != 0)
			{
				lv2_storage_close(dev_handle);
				return false;
			}

			memcpy(&dataa[curDataOffset], &buf[zzz], xxx);

			curOffset += xxx;
			curDataOffset += xxx;

			left -= xxx;
		}
		else if ((burst_size > 0) && (left >= burst_size) && ((burst_size % sector_size) == 0))
		{
			res = lv2_storage_read(dev_handle, 0, sector_idx, (burst_size / sector_size), &dataa[curDataOffset], &unknown2, dev_flags);

			if (res != 0)
			{
				lv2_storage_close(dev_handle);
				return false;
			}

			curOffset += burst_size;
			curDataOffset += burst_size;

			left -= burst_size;
		}
		else
		{
			res = lv2_storage_read(dev_handle, 0, sector_idx, 1, &dataa[curDataOffset], &unknown2, dev_flags);

			if (res != 0)
			{
				lv2_storage_close(dev_handle);
				return false;
			}

			curOffset += processSize;
			curDataOffset += processSize;

			left -= processSize;
		}
	}

	res = lv2_storage_close(dev_handle);
	if (res != 0)
		return false;

	return true;
}

bool qcfw_dump_nor_to_file(uint32_t offset, uint32_t size, const char* filePath, uint32_t chunk_size)
{
	if (chunk_size == 0)
		return false;

	uint8_t* chunkBuf = (uint8_t*)_malloc(chunk_size);
	if (chunkBuf == NULL)
		return false;

	int32_t fd;
	if (cellFsOpen(filePath, CELL_FS_O_CREAT | CELL_FS_O_TRUNC | CELL_FS_O_RDWR, &fd, 0, 0) != CELL_FS_SUCCEEDED)
	{
		_free(chunkBuf);
		return false;
	}

	cellFsChmod(filePath, 0777);

	{
		uint32_t left = size;
		uint32_t curNorOffset = offset;

		while (left > 0)
		{
			uint32_t processSize = (left > chunk_size) ? chunk_size : left;
			uint64_t writeSuccessSize = 0;

			if
				(
					!qcfw_nor_read(curNorOffset, chunkBuf, processSize, (256 * 1024)) ||
					(cellFsWrite(fd, chunkBuf, processSize, &writeSuccessSize) != CELL_FS_SUCCEEDED) ||
					(writeSuccessSize != processSize)
					)
			{
				cellFsClose(fd);
				_free(chunkBuf);

				return false;
			}

			curNorOffset += processSize;
			left -= processSize;
		}
	}

	cellFsClose(fd);
	_free(chunkBuf);

	return true;
}

int32_t qcfw_lv2_storage_read_emmc(uint32_t dev_handle, uint64_t unknown1, uint64_t start_sector, uint64_t sector_count, void *buf, uint32_t *unknown2, uint64_t flags)
{
	if (!qcfw_is_emmc())
		return 1;

	if (sector_count == 0)
		return 0;

	if ((start_sector + sector_count) > 0x1893000)
		return 1;

	static const uint32_t sector_size = 512;
	static const uint32_t masked_sector_count = (0x40000 / sector_size);

	if (start_sector < masked_sector_count)
	{
		uint8_t* buff = (uint8_t*)buf;

		uint32_t leftSectorCount = sector_count;
		uint32_t fillSectorCount = (masked_sector_count - start_sector);

		if (fillSectorCount > leftSectorCount)
			fillSectorCount = leftSectorCount;

		uint32_t totalSizeToFillInBytes = (fillSectorCount * sector_size);
		memset(buff, 0xff, totalSizeToFillInBytes);

		{
			int32_t res = lv2_storage_read(dev_handle, unknown1, ((start_sector + (0xF000000 / sector_size)) - masked_sector_count), fillSectorCount, &buff[0], unknown2, flags);

			if (res != 0)
				return res;
		}

		leftSectorCount -= fillSectorCount;

		if (leftSectorCount > 0)
			return lv2_storage_read(dev_handle, unknown1, 0, leftSectorCount, &buff[totalSizeToFillInBytes], unknown2, flags);

		return 0;
	}

	return lv2_storage_read(dev_handle, unknown1, (start_sector - masked_sector_count), sector_count, buf, unknown2, flags);
}

bool qcfw_emmc_read(uint64_t offset, void* data, uint32_t size, uint32_t burst_size)
{
	uint8_t* dataa = (uint8_t*)data;

	if (data == NULL)
		return false;

	if (size == 0)
		return true;

	if ((offset + size) > 13193183232ull) // 0x1893000 * 512
		return false;

	if (!qcfw_is_emmc())
		return false;

	int32_t res;

	uint32_t unknown2;

	uint64_t dev_id = 0x100000000000001ull;
	uint64_t dev_flags = 0x22ull;

	static const uint32_t sector_size = 512;

	if ((burst_size == 0) || ((burst_size % sector_size) != 0))
		return false;

	uint32_t dev_handle;

	res = lv2_storage_open(dev_id, &dev_handle);
	if (res != 0)
		return false;

	uint8_t buf[sector_size];

	uint64_t curOffset = offset;
	uint32_t curDataOffset = 0;

	uint32_t left = size;

	while (left > 0)
	{
		uint32_t processSize = (left > sector_size) ? sector_size : left;
		uint32_t zzz = (curOffset % sector_size);
		uint32_t yyy = (sector_size - zzz);
		uint32_t xxx = (yyy > processSize) ? processSize : yyy;

		uint32_t sector_idx = (curOffset / sector_size);

		while (burst_size > left)
			burst_size -= sector_size;

		if ((zzz != 0) || (processSize != sector_size))
		{
			res = qcfw_lv2_storage_read_emmc(dev_handle, 0, sector_idx, 1, buf, &unknown2, dev_flags);

			if (res != 0)
			{
				lv2_storage_close(dev_handle);
				return false;
			}

			memcpy(&dataa[curDataOffset], &buf[zzz], xxx);

			curOffset += xxx;
			curDataOffset += xxx;

			left -= xxx;
		}
		else if ((burst_size > 0) && (left >= burst_size) && ((burst_size % sector_size) == 0))
		{
			res = qcfw_lv2_storage_read_emmc(dev_handle, 0, sector_idx, (burst_size / sector_size), &dataa[curDataOffset], &unknown2, dev_flags);

			if (res != 0)
			{
				lv2_storage_close(dev_handle);
				return false;
			}

			curOffset += burst_size;
			curDataOffset += burst_size;

			left -= burst_size;
		}
		else
		{
			res = qcfw_lv2_storage_read_emmc(dev_handle, 0, sector_idx, 1, &dataa[curDataOffset], &unknown2, dev_flags);

			if (res != 0)
			{
				lv2_storage_close(dev_handle);
				return false;
			}

			curOffset += processSize;
			curDataOffset += processSize;

			left -= processSize;
		}
	}

	res = lv2_storage_close(dev_handle);
	if (res != 0)
		return false;

	return true;
}

bool qcfw_dump_emmc_to_file(uint64_t offset, uint64_t size, const char* filePath, uint32_t chunk_size)
{
	if (chunk_size == 0)
		return false;

	uint8_t* chunkBuf = (uint8_t*)_malloc(chunk_size);
	if (chunkBuf == NULL)
		return false;

	int32_t fd;
	if (cellFsOpen(filePath, CELL_FS_O_CREAT | CELL_FS_O_TRUNC | CELL_FS_O_RDWR, &fd, 0, 0) != CELL_FS_SUCCEEDED)
	{
		_free(chunkBuf);
		return false;
	}

	cellFsChmod(filePath, 0777);

	{
		uint64_t left = size;
		uint64_t curEmmcOffset = offset;

		while (left > 0)
		{
			uint32_t processSize = (left > chunk_size) ? chunk_size : left;
			uint64_t writeSuccessSize = 0;

			if
				(
					!qcfw_emmc_read(curEmmcOffset, chunkBuf, processSize, chunk_size) ||
					(cellFsWrite(fd, chunkBuf, processSize, &writeSuccessSize) != CELL_FS_SUCCEEDED) ||
					(writeSuccessSize != processSize)
					)
			{
				cellFsClose(fd);
				_free(chunkBuf);

				return false;
			}

			curEmmcOffset += processSize;
			left -= processSize;
		}
	}

	cellFsClose(fd);
	_free(chunkBuf);

	return true;
}

bool qcfw_dump_nor_to_usb()
{
	if (!qcfw_dump_nor_to_file(0, (16 * 1024 * 1024), "/dev_usb000/NOR.bin", (256 * 1024)))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return false;
	}

	PrintString(L"Success!", XAI_PLUGIN, TEX_SUCCESS);
	return true;
}

bool qcfw_emmc_is_complete()
{
	if (!qcfw_is_emmc())
		return false;

	uint8_t lv0ldr_region_dump_status = 0xff;
	if (!qcfw_sc_read_lv0ldr_region_dump_status(&lv0ldr_region_dump_status))
		return false;

	if (lv0ldr_region_dump_status != 0x27)
		return false;

	uint32_t lv0ldr_region_crc32 = 0;
	if (!qcfw_sc_read_lv0ldr_region_crc32(&lv0ldr_region_crc32))
		return false;

	static const uint32_t dumpSize = 0x40000;

	static const uint32_t tmpDataBuf_MaxSize = dumpSize;
	uint8_t* tmpDataBuf = (uint8_t*)_malloc(tmpDataBuf_MaxSize);

	if (tmpDataBuf == NULL)
		return false;

	if (!qcfw_emmc_read(0xF000000, tmpDataBuf, dumpSize, (256 * 1024)))
	{
		_free(tmpDataBuf);
		return false;
	}

	uint32_t lv0ldr_bottom_region_crc32 = qcfw_crc32c(0, tmpDataBuf, dumpSize);
	if (lv0ldr_bottom_region_crc32 != lv0ldr_region_crc32)
	{
		_free(tmpDataBuf);
		return false;
	}

	_free(tmpDataBuf);
	return true;
}

bool qcfw_dump_emmc_to_usb_256M()
{
	bool is_complete = qcfw_emmc_is_complete();

	if (!qcfw_dump_emmc_to_file(0, (256 * 1024 * 1024), (is_complete ? "/dev_usb000/eMMC_complete_256M.bin" : "/dev_usb000/eMMC_incomplete_256M.bin"), (256 * 1024)))
	{
		PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
		return false;
	}

	if (is_complete)
		PrintString(L"Success! (complete)", XAI_PLUGIN, TEX_SUCCESS);
	else
		PrintString(L"Success! (incomplete)", XAI_PLUGIN, TEX_SUCCESS);

	return true;
}

bool qcfw_dump_emmc_to_usb_12G()
{
	bool is_complete = qcfw_emmc_is_complete();

	static const uint64_t dump_size = 13193183232ull;
	static const uint64_t chunk_size = (3ULL * 1024ULL * 1024ULL * 1024ULL);

	uint64_t cur_offset = 0;
	uint64_t left = dump_size;

	uint32_t i = 0;

	while (left > 0)
	{
		uint64_t processSize = (left > chunk_size) ? chunk_size : left;

		char path[512];

		if (is_complete)
			sprintf_(path, "/dev_usb000/eMMC_complete_12G_%u.bin", i);
		else
			sprintf_(path, "/dev_usb000/eMMC_incomplete_12G_%u.bin", i);

		if (!qcfw_dump_emmc_to_file(cur_offset, processSize, path, (256 * 1024)))
		{
			PrintString(L"Failed!", XAI_PLUGIN, TEX_ERROR);
			return false;
		}

		cur_offset += processSize;
		left -= processSize;

		++i;
	}

	if (is_complete)
		PrintString(L"Success! (complete)", XAI_PLUGIN, TEX_SUCCESS);
	else
		PrintString(L"Success! (incomplete)", XAI_PLUGIN, TEX_SUCCESS);

	return true;
}

bool qcfw_toggle_nobd_patch()
{
	uint8_t old_value = 0xff;
	if (!qcfw_sc_read_nobd_toggle_flag(&old_value))
		return false;

	if (old_value == 0x1)
	{
		if (!qcfw_sc_write_nobd_toggle_flag(0x0))
			return false;

		PrintString(L"NoBD patch disabled.\nReboot to apply changes.", XAI_PLUGIN, TEX_SUCCESS);
	}
	else
	{
		if (!qcfw_sc_write_nobd_toggle_flag(0x1))
			return false;

		PrintString(L"NoBD patch enabled.\nReboot to apply changes.", XAI_PLUGIN, TEX_SUCCESS);
	}

	return true;
}