extern bool is_qcfw();

extern bool qcfw_is_nor();
extern bool qcfw_is_emmc();

extern void qcfw_install_otheros();
extern void qcfw_boot_otheros();

extern bool qcfw_sc_write_fsm_counter(uint8_t value);
extern bool qcfw_sc_write_nobd_toggle_flag(uint8_t value);
extern bool qcfw_sc_write_dex_spoof_flag(uint8_t value);

extern void qcfw_dump_hdd_key();
extern void qcfw_save_hdd_key_to_usb();

extern bool qcfw_dump_nor_to_usb();

extern bool qcfw_dump_emmc_to_usb_256M();
extern bool qcfw_dump_emmc_to_usb_12G();

extern bool qcfw_toggle_nobd_patch();