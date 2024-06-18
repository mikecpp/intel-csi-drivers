/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2015 - 2024 Intel Corporation */

#ifndef IPU_CPD_H
#define IPU_CPD_H

#define IPU_CPD_SIZE_OF_FW_ARCH_VERSION		7
#define IPU_CPD_SIZE_OF_SYSTEM_VERSION		11
#define IPU_CPD_SIZE_OF_COMPONENT_NAME		12

#define IPU_CPD_METADATA_EXTN_TYPE_IUNIT	0x10

#define IPU_CPD_METADATA_IMAGE_TYPE_RESERVED		0
#define IPU_CPD_METADATA_IMAGE_TYPE_BOOTLOADER		1
#define IPU_CPD_METADATA_IMAGE_TYPE_MAIN_FIRMWARE	2

#define IPU_CPD_PKG_DIR_PSYS_SERVER_IDX	0
#define IPU_CPD_PKG_DIR_ISYS_SERVER_IDX	1

#define IPU_CPD_PKG_DIR_CLIENT_PG_TYPE	3

#define IPU6_CPD_METADATA_HASH_KEY_SIZE          48
#define IPU_CPD_METADATA_HASH_KEY_SIZE           32

struct __packed ipu_cpd_module_data_hdr {
	u32 hdr_len;
	u32 endian;
	u32 fw_pkg_date;
	u32 hive_sdk_date;
	u32 compiler_date;
	u32 target_platform_type;
	u8 sys_ver[IPU_CPD_SIZE_OF_SYSTEM_VERSION];
	u8 fw_arch_ver[IPU_CPD_SIZE_OF_FW_ARCH_VERSION];
	u8 rsvd[2];
};

/* ipu_cpd_hdr structure updated as the chksum and
 * sub_partition_name is unused on host side
 * CSE layout version 1.6 for ipu6se (hdr_len = 0x10)
 * CSE layout version 1.7 for ipu6 (hdr_len = 0x14)
 */
struct __packed ipu_cpd_hdr {
	u32 hdr_mark;
	u32 ent_cnt;
	u8 hdr_ver;
	u8 ent_ver;
	u8 hdr_len;
};

struct __packed ipu_cpd_ent {
	u8 name[IPU_CPD_SIZE_OF_COMPONENT_NAME];
	u32 offset;
	u32 len;
	u8 rsvd[4];
};

struct __packed ipu_cpd_metadata_cmpnt {
	u32 id;
	u32 size;
	u32 ver;
	u8 sha2_hash[IPU_CPD_METADATA_HASH_KEY_SIZE];
	u32 entry_point;
	u32 icache_base_offs;
	u8 attrs[16];
};

struct __packed ipu6_cpd_metadata_cmpnt {
	u32 id;
	u32 size;
	u32 ver;
	u8 sha2_hash[IPU6_CPD_METADATA_HASH_KEY_SIZE];
	u32 entry_point;
	u32 icache_base_offs;
	u8 attrs[16];
};

struct __packed ipu_cpd_metadata_extn {
	u32 extn_type;
	u32 len;
	u32 img_type;
	u8 rsvd[16];
};

struct __packed ipu_cpd_client_pkg_hdr {
	u32 prog_list_offs;
	u32 prog_list_size;
	u32 prog_desc_offs;
	u32 prog_desc_size;
	u32 pg_manifest_offs;
	u32 pg_manifest_size;
	u32 prog_bin_offs;
	u32 prog_bin_size;
};

void *ipu_cpd_create_pkg_dir(struct ipu_bus_device *adev,
			     const void *src,
			     dma_addr_t dma_addr_src,
			     dma_addr_t *dma_addr, unsigned int *pkg_dir_size);
void ipu_cpd_free_pkg_dir(struct ipu_bus_device *adev,
			  u64 *pkg_dir,
			  dma_addr_t dma_addr, unsigned int pkg_dir_size);
int ipu_cpd_validate_cpd_file(struct ipu_device *isp,
			      const void *cpd_file,
			      unsigned long cpd_file_size);
unsigned int ipu_cpd_pkg_dir_get_address(const u64 *pkg_dir, int pkg_dir_idx);
unsigned int ipu_cpd_pkg_dir_get_num_entries(const u64 *pkg_dir);
unsigned int ipu_cpd_pkg_dir_get_size(const u64 *pkg_dir, int pkg_dir_idx);
unsigned int ipu_cpd_pkg_dir_get_type(const u64 *pkg_dir, int pkg_dir_idx);

#endif /* IPU_CPD_H */
