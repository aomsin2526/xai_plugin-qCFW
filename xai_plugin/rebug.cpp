#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cell/fs/cell_fs_file_api.h>
#include "gccpch.h"
#include "log.h"
#include "rebug.h"
#include "cfw_settings.h"
#include "functions.h"
#include "xRegistry.h"
#include "cex2dex.h"

void normal_mode_to_rebug_mode()
{
	CellFsStat stat;

	if(cellFsStat(VSH_SWP, &stat) == CELL_OK && cellFsStat(VSH_DSP, &stat) == CELL_OK)
	{
		if(cellFsStat(SYSCONF_SPRX_CEX, &stat) == CELL_OK)
		{
			cellFsRename(SYSCONF_SPRX_DEFAULT, SYSCONF_SPRX_DEX);
			cellFsRename(SYSCONF_SPRX_CEX, SYSCONF_SPRX_DEFAULT);
		}
	}

	cellFsRename(VSH_SELF, VSH_NRM);
	cellFsRename(VSH_SWP, VSH_SELF);

	cellFsRename(IDX_DAT, IDX_NRM);
	cellFsRename(IDX_SWP, IDX_DAT);

	cellFsRename(VER_TXT, VER_NRM);
	cellFsRename(VER_SWP, VER_TXT);
}

void rebug_mode_to_normal_mode()
{
	cellFsRename(VSH_SELF, VSH_SWP);
	cellFsRename(VSH_NRM, VSH_SELF);
	
	cellFsRename(IDX_DAT, IDX_SWP);
	cellFsRename(IDX_NRM, IDX_DAT);
	
	cellFsRename(VER_TXT, VER_SWP);
	cellFsRename(VER_NRM, VER_TXT);
}

/*int toggle_xmb_plugin()
{
	CellFsStat stat;

	mount_dev_blind();

	if(cellFsStat(XMB_PLUGIN_SPRX_DEX, &stat) == CELL_OK)
	{
		cellFsRename(XMB_PLUGIN_SPRX, XMB_PLUGIN_SPRX_CEX);
		cellFsRename(XMB_PLUGIN_SPRX_DEX, XMB_PLUGIN_SPRX);
		customMessage2("msg_host_status", "msg_lower_enabled", TEX_SUCCESS);
		return 0;
	}
	else if(cellFsStat(XMB_PLUGIN_SPRX_CEX, &stat) == CELL_OK)
	{
		cellFsRename(XMB_PLUGIN_SPRX, XMB_PLUGIN_SPRX_DEX);
		cellFsRename(XMB_PLUGIN_SPRX_CEX, XMB_PLUGIN_SPRX);
		customMessage2("msg_host_status", "msg_lower_disabled", TEX_SUCCESS);
		return 0;
	}
	else
		showMessage("msg_switch_error", (char *)XAI_PLUGIN, (char *)TEX_ERROR);

	umount_dev_blind();
	
	return 1;
}*/

int toggle_xmb_mode()
{
	CellFsStat stat;
	
	mount_dev_blind();

	if(cellFsStat(VSH_SWP, &stat) == CELL_OK)
	{		
		showMessage("msg_need_rebug_mode", (char*)XAI_PLUGIN, (char*)TEX_WARNING);			
		return 1;
	}

	if(cellFsStat(VSH_DSP, &stat) == CELL_OK)
	{
		cellFsRename(VSH_SELF, VSH_CSP);
		cellFsRename(VSH_DSP, VSH_SELF);
		customMessage("msg_xmb_status", "DEBUG XMB", TEX_SUCCESS);
		return 0;
	}
	else if(cellFsStat(VSH_CSP, &stat) == CELL_OK)
	{
		cellFsRename(VSH_SELF, VSH_DSP);
		cellFsRename(VSH_CSP, VSH_SELF);
		customMessage("msg_xmb_status", "RETAIL XMB", TEX_SUCCESS);
		return 0;
	}
	else
		showMessage("msg_switch_error", (char*)XAI_PLUGIN, (char*)TEX_ERROR);

	umount_dev_blind();

	return 1;	
}

int normal_mode()
{
	int ret;
	CellFsStat statinfo;

	mount_dev_blind();

	// "/dev_blind/vsh/module/vsh.self.nrm"
	ret = cellFsStat(VSH_NRM, &statinfo); 
	if(ret != CELL_OK)
		log("cellFsStat(vshnrm) = %x\n", ret);

	// "/dev_blind/vsh/etc/index.dat.nrm"
	ret |= cellFsStat(IDX_NRM, &statinfo); 
	if(ret != CELL_OK)
		log("cellFsStat(idxnrm) = %x\n", ret);

	// "/dev_blind/vsh/etc/version.txt.nrm"
	ret |= cellFsStat(VER_NRM, &statinfo); 
	if(ret != CELL_OK)
		log("cellFsStat(vernrm) = %x\n", ret);

	if(ret == CELL_OK)	
		rebug_mode_to_normal_mode();
	else
		showMessage("msg_switch_error", (char *)XAI_PLUGIN, (char *)TEX_ERROR);

	umount_dev_blind();

	return ret;	
}

int rebug_mode()
{	
	int ret;
	CellFsStat statinfo;

	mount_dev_blind();

	// "/dev_blind/vsh/module/vsh.self.swp"
	ret = cellFsStat(VSH_SWP, &statinfo); 
	if(ret != CELL_OK)
		log("cellFsStat(vshswp) = %x\n", ret);

	// "/dev_blind/vsh/etc/index.dat.swp"
	ret |= cellFsStat(IDX_SWP, &statinfo); 
	if(ret != CELL_OK)
		log("cellFsStat(idxswp) = %x\n", ret);

	// "/dev_blind/vsh/etc/version.txt.swp"
	ret |= cellFsStat(VER_SWP, &statinfo); 
	if(ret != CELL_OK)
		log("cellFsStat(verswp) = %x\n", ret);

	if(ret == CELL_OK)	
		normal_mode_to_rebug_mode();	
	else
		showMessage("msg_switch_error", (char *)XAI_PLUGIN, (char *)TEX_ERROR);

	umount_dev_blind();

	return ret;	
}

int debugsettings_mode()
{
	CellFsStat statinfo;

	mount_dev_blind();

	// "/dev_blind/vsh/module/sysconf_plugin.sprx.cex"
	if(cellFsStat(SYSCONF_SPRX_CEX, &statinfo) == CELL_OK)
	{
		cellFsRename(SYSCONF_SPRX_DEFAULT, SYSCONF_SPRX_DEX);
		cellFsRename(SYSCONF_SPRX_CEX, SYSCONF_SPRX_DEFAULT);
		customMessage("msg_debug_settings_status", "MENU CEX QA", TEX_SUCCESS);
	}
	// "/dev_blind/vsh/module/sysconf_plugin.sprx.dex"
	else if(cellFsStat(SYSCONF_SPRX_DEX, &statinfo) == CELL_OK)
	{	
		cellFsRename(SYSCONF_SPRX_DEFAULT, SYSCONF_SPRX_CEX);
		cellFsRename(SYSCONF_SPRX_DEX, SYSCONF_SPRX_DEFAULT);
		customMessage("msg_debug_settings_status", "MENU DEBUG", TEX_SUCCESS);
	}
	else
	{
		showMessage("msg_unable_find_files_error", (char*)XAI_PLUGIN, (char*)TEX_ERROR);	
		return 1;
	}

	umount_dev_blind();

	return CELL_OK;
}

/*void download_toolbox()
{
	showMessage("msg_download_toolbox", (char*)XAI_PLUGIN, (char*)TEX_WARNING);
	downloadPKG(L"https://github.com/Joonie86/Rebug-Toolbox/releases/download/2.03.06/REBUG_TOOLBOX_02.03.06.MULTI.16.pkg.946.v02.03.06_brewology_com.pkg");
}*/

/*void install_toolbox()
{
	char pkgpath[255];
	memset(pkgpath, 0, 255);
	searchDirectory("/dev_hdd0/tmp/downloader/", "REBUG_TOOLBOX_", pkgpath);

	if(pkgpath[0] != 0)
	{
		showMessage("msg_install_toolbox", (char*)XAI_PLUGIN, (char*)TEX_WARNING);
		log("installPKG(%s)\n", pkgpath); 
		installPKG(pkgpath);

		while(FindPlugin("nas_plugin") != 0)		
			wait(2);
		
		cellFsUnlink(pkgpath);
	}
	else
		showMessage("msg_toolbox_not_found", (char*)XAI_PLUGIN, (char*)TEX_WARNING);
}*/