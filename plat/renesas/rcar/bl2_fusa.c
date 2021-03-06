/*
 * bl2_ecc_setup.c -
 *
 * Copyright (C) 2018 Renesas Electronics Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <debug.h>
#include <mmio.h>

#define	DFUSAAREACR		0xE6785000	/* DRAM FuSa Area Conf */
#define	DECCAREACR		0xE6785040	/* DRAM ECC Area Conf */
#define	NUM_DAREA		16
#define	FUSACR			0xE6784020	/* FuSa Ctrl */

/* As the saddr, specify high-memory address (> 4 GB) */
#define	FUSAAREACR(en, size, saddr)	\
	(((uint32_t)en << 31) | ((uint32_t)size << 24) | (uint32_t)(((uintptr_t)saddr) >> 12))
#define	ECCAREACR(ecc, saddr) \
	(((uint32_t)ecc << 31) | (uint32_t)(((uintptr_t)saddr) >> 12))

#define	EFUSASEL(x)	((uint32_t)x & 0xff) << 24	/*Setting for Extra Split mode*/
#define	DFUSASEL(x)	((uint32_t)x & 0xff) << 16	/*Setting for DRAM*/
#define	SFUSASEL(x)	((uint32_t)x & 0x3f)		/*Setting for SRAM*/

#ifndef ARRAY_SIZE
#define	ARRAY_SIZE(a)		(sizeof(a) / sizeof(a[0]))
#endif

static const struct rzg2_ecc_conf {
	uint32_t fusaareacr;
	uint32_t eccareacr;
} rzg2_ek874_conf[] = {
#if (RZG_DRAM_ECC_FULL == 1)
	{ FUSAAREACR(1, 7, 0x408000000),    ECCAREACR(0, 0x448000000)   }, /* 128+128 MB */
	{ FUSAAREACR(1, 8, 0x410000000),    ECCAREACR(0, 0x450000000)   }, /* 256+256 MB */
	{ FUSAAREACR(1, 8, 0x420000000),    ECCAREACR(0, 0x460000000)   }, /* 256+256 MB */
	{ FUSAAREACR(1, 8, 0x430000000),    ECCAREACR(0, 0x470000000)   }, /* 256+256 MB */
	{ FUSAAREACR(1, 6, 0x440000000),    ECCAREACR(0, 0x444000000)   }, /* 64+64 MB */
#else //(RZG_DRAM_ECC_FULL == 1)
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
#endif //(RZG_DRAM_ECC_FULL == 1)
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
};

static const struct rzg2_ecc_conf rzg2_hihope_rzg2n_conf[] = {
#if (RZG_DRAM_ECC_FULL == 1)
        { FUSAAREACR(1, 7, 0x408000000),    ECCAREACR(0, 0x488000000)   }, /* 128+128 MB */
        { FUSAAREACR(1, 8, 0x410000000),    ECCAREACR(0, 0x490000000)   }, /* 256+256 MB */
        { FUSAAREACR(1, 9, 0x420000000),    ECCAREACR(0, 0x4A0000000)   }, /* 512+512 MB */
        { FUSAAREACR(1, 9, 0x440000000),    ECCAREACR(0, 0x4C0000000)   }, /* 512+512 MB */
        { FUSAAREACR(1, 9, 0x460000000),    ECCAREACR(0, 0x4E0000000)   }, /* 512+512 MB */
        { FUSAAREACR(1, 6, 0x480000000),    ECCAREACR(0, 0x484000000)   }, /* 64+64 MB */
#else //(RZG_DRAM_ECC_FULL == 1)
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
#endif //(RZG_DRAM_ECC_FULL == 1)
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
        { FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
};

/* Setup DRAM ECC configuration registers */
void bl2_enable_ecc_ek874(void)
{
	const struct rzg2_ecc_conf *conf;
	int n;
	int nb_of_conf;

	conf = rzg2_ek874_conf;
	nb_of_conf = ARRAY_SIZE(rzg2_ek874_conf);

	for (n = 0; n < nb_of_conf; n++, conf++) {
		mmio_write_32((uintptr_t)((uint32_t *)DFUSAAREACR + n), conf->fusaareacr);
		mmio_write_32((uintptr_t)((uint32_t *)DECCAREACR + n), conf->eccareacr);
		if(conf->fusaareacr & (1 << 31))
			NOTICE("BL2: DRAM ECC area=%d, FuSa=0x%x ECC=0x%x, size=%d MB\n",
					n, conf->fusaareacr, conf->eccareacr,
					1 << ((conf->fusaareacr >> 24) & 0xf));

	}

}

void bl2_enable_ecc_hihope_rzg2n(void)
{
	const struct rzg2_ecc_conf *conf;
	int n;
	int nb_of_conf;

	conf = rzg2_hihope_rzg2n_conf;
	nb_of_conf = ARRAY_SIZE(rzg2_hihope_rzg2n_conf);

	for (n = 0; n < nb_of_conf; n++, conf++) {
		mmio_write_32((uintptr_t)((uint32_t *)DFUSAAREACR + n), conf->fusaareacr);
		mmio_write_32((uintptr_t)((uint32_t *)DECCAREACR + n), conf->eccareacr);
		if(conf->fusaareacr & (1 << 31))
			NOTICE("BL2: DRAM ECC area=%d, FuSa=0x%x ECC=0x%x, size=%d MB\n",
					n, conf->fusaareacr, conf->eccareacr,
					1 << ((conf->fusaareacr >> 24) & 0xf));
	}

}

void bl2_enable_ecc_hihope_rzg2m(void)
{
	mmio_write_32((uintptr_t)((uint32_t *)FUSACR), EFUSASEL(0)
#if (RZG_DRAM_ECC_FULL == 1)
						     | DFUSASEL(0xff)
#else //(RZG_DRAM_ECC_FULL == 1)
						     | DFUSASEL(0x00)
#endif //(RZG_DRAM_ECC_FULL == 1)
						     | SFUSASEL(0));
#if (RZG_DRAM_ECC_FULL == 1)
	NOTICE("BL2: DRAM FULL ECC Configured (DFUSASEL=0x%x)\n",DFUSASEL(0xff));
#endif //(RZG_DRAM_ECC_FULL == 1)
}
