/* Xilinx PCS/PMA Core phy driver
 *
 * Copyright (C) 2015 Xilinx, Inc.
 *
 * Description:
 * This driver is developed for PCS/PMA Core.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/of.h>

#include <linux/hrtimer.h>
#include <linux/ktime.h>

//#include <linux/tb_phy.h>

#define TB_PHY_ID_MASK		0xfffffff0
#define TB_PHY_ID			0x00040001			/* for Zynq board */
//#define TB_PHY_ID			0x01000000			/* for Zed board */

#define XAE_PHY_TYPE_MII	0

static int tbphy_read_status(struct phy_device *phydev)
{
	int err;
	int status = 0;

	phydev->link = 1;

	if (phydev->dev_flags == XAE_PHY_TYPE_MII) {
		phydev->duplex = DUPLEX_HALF;
		phydev->speed = SPEED_100;
	}

//	printk("%s, dev_flags:%d, link:%d, duplex:%d, speed:%d\n", 
//			__FUNCTION__, phydev->dev_flags, phydev->link, phydev->duplex, phydev->speed);

	return 0;
}

static int tbphy_of_init(struct phy_device *phydev)
{
	struct device *dev = &phydev->mdio.dev;
	struct device_node *of_node = dev->of_node;
	u32 phytype;

	if (!IS_ENABLED(CONFIG_OF_MDIO))
		return 0;

	if (!of_node)
		return -ENODEV;

	if (!of_property_read_u32(of_node, "tb,phy-type", &phytype)) {
		if (phytype == XAE_PHY_TYPE_MII)
			phydev->dev_flags |= XAE_PHY_TYPE_MII;
	}

	return 0;
}

static int tbphy_config_init(struct phy_device *phydev)
{
	tbphy_of_init(phydev);
	return 0;
}

static int tbphy_reset(struct phy_device *phydev)
{
	return 0;
}

static int tbphy_config_aneg(struct phy_device *phydev)
{
	return 0;
}

static int tbphy_resume(struct phy_device *phydev)
{
	return 0;
}

static int tbphy_suspend(struct phy_device *phydev)
{
	return 0;
}

static int tbphy_aneg_done(struct phy_device *phydev)
{
	return 1;
}

static struct phy_driver tb_drivers[] = {
	{
		.phy_id = TB_PHY_ID,
		.phy_id_mask = TB_PHY_ID_MASK,
		.name = "Thunderbus PHY",
		.features = PHY_BASIC_FEATURES,
		.config_init = &tbphy_config_init,
		.aneg_done = &tbphy_aneg_done,
		.soft_reset = &tbphy_reset,
		.config_aneg = &tbphy_config_aneg,
		.read_status = &tbphy_read_status,
		.resume = &tbphy_resume,
		.suspend = &tbphy_suspend,
	},
};

module_phy_driver(tb_drivers);

static struct mdio_device_id __maybe_unused tb_tbl[] = {
	{ TB_PHY_ID, TB_PHY_ID_MASK },
	{ }
};

MODULE_DEVICE_TABLE(mdio, tb_tbl);
MODULE_DESCRIPTION("Thunderbus PHY driver");
MODULE_LICENSE("GPL");
