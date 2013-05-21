/********************************************************************************
 * Marvell GPL License Option
 *
 * If you received this File from Marvell, you may opt to use, redistribute and/or
 * modify this File in accordance with the terms and conditions of the General
 * Public License Version 2, June 1991 (the "GPL License"), a copy of which is
 * available along with the File in the license.txt file or by writing to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
 * on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
 * DISCLAIMED.  The GPL License provides additional details about this warranty
 * disclaimer.
 ********************************************************************************/
#ifndef __GALOIS_DV_IOMAPPER__
#define __GALOIS_DV_IOMAPPER__

/* registers definition in IO-mapper */
#define IOMAPPER_REG_MAP_0	0
#define IOMAPPER_REG_MAP_1	1
#define IOMAPPER_REG_MAP_2	2
#define IOMAPPER_REG_MAP_3	3
#define IOMAPPER_REG_MAP_4	4
#define IOMAPPER_REG_MAP_5	5
#define IOMAPPER_REG_MAP_6	6
#define IOMAPPER_REG_MAP_7	7
#define IOMAPPER_REG_MAP_8	8
#define IOMAPPER_REG_MAP_9	9
#define IOMAPPER_REG_MAP_10	10
#define IOMAPPER_REG_MAP_11	11

typedef union {
	UNSG16 u16;
	struct {
		UNSG16 g1:1;
		UNSG16 g2:1;
		UNSG16 g3:1;
		UNSG16 g4:1;
		UNSG16 g5:1;
		UNSG16 g6:1;
		UNSG16 g7:1;
		UNSG16 g8:1;
		UNSG16 g9:1;
		UNSG16 g10:1;
		UNSG16 g17:1;
		UNSG16 g18:1;
		UNSG16 g21:1;
		UNSG16 g22:1;
		UNSG16 g23:1;
		UNSG16 g26:1;
	};
} iomapper_reg_0_t;

typedef union {
	UNSG16 u16;
	struct {
		UNSG16 g31:1;
		UNSG16 g32:1;
		UNSG16 g33:1;
		UNSG16 g34:1;
		UNSG16 g35:1;
		UNSG16 g36:1;
		UNSG16 g37:1;
		UNSG16 reserved:9;
	};
} iomapper_reg_1_t;

typedef union {
	UNSG16 u16;
	struct {
		UNSG16 g11:2;
		UNSG16 g12:2;
		UNSG16 g13:2;
		UNSG16 g14:2;
		UNSG16 g15:2;
		UNSG16 g16:2;
		UNSG16 g19:2;
		UNSG16 g20:2;
	};
} iomapper_reg_2_t;

typedef union {
	UNSG16 u16;
	struct {
		UNSG16 g24:2;
		UNSG16 g25:2;
		UNSG16 g27:2;
		UNSG16 g28:2;
		UNSG16 g29:2;
		UNSG16 reserved:6;
	};
} iomapper_reg_3_t;

#endif /* __GALOIS_DV_IOMAPPER__ */
