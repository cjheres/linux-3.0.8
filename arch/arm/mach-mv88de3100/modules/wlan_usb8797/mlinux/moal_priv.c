/** @file  moal_priv.c
  *
  * @brief This file contains standard ioctl functions
  *
  * Copyright (C) 2008-2011, Marvell International Ltd.
  *
  * This software file (the "File") is distributed by Marvell International
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991
  * (the "License").  You may use, redistribute and/or modify this File in
  * accordance with the terms and conditions of the License, a copy of which
  * is available by writing to the Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
  * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
  * this warranty disclaimer.
  *
  */

/************************************************************************
Change log:
    10/30/2008: initial version
************************************************************************/

#include	"moal_main.h"

/********************************************************
                Local Variables
********************************************************/
/** Bands supported in Infra mode */
static t_u8 SupportedInfraBand[] = {
    BAND_B,
    BAND_B | BAND_G, BAND_G,
    BAND_GN, BAND_B | BAND_G | BAND_GN, BAND_G | BAND_GN,
    BAND_A, BAND_B | BAND_A, BAND_B | BAND_G | BAND_A, BAND_G | BAND_A,
    BAND_A | BAND_B | BAND_G | BAND_AN | BAND_GN,
        BAND_A | BAND_G | BAND_AN | BAND_GN, BAND_A | BAND_AN,
};

/** Bands supported in Ad-Hoc mode */
static t_u8 SupportedAdhocBand[] = {
    BAND_B, BAND_B | BAND_G, BAND_G,
    BAND_GN, BAND_B | BAND_G | BAND_GN, BAND_G | BAND_GN,
    BAND_A,
    BAND_AN, BAND_A | BAND_AN,
};

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/

/**
 *  @brief Copy Rates
 *
 *  @param dest    A pointer to destination buffer
 *  @param pos     The position for copy
 *  @param src     A pointer to source buffer
 *  @param len     Length of the source buffer
 *
 *  @return        Number of rates copied
 */
static inline int
woal_copy_rates(t_u8 * dest, int pos, t_u8 * src, int len)
{
    int i;

    for (i = 0; i < len && src[i]; i++, pos++) {
        if (pos >= MLAN_SUPPORTED_RATES)
            break;
        dest[pos] = src[i];
    }
    return pos;
}

/**
 *  @brief Performs warm reset
 *
 *  @param priv         A pointer to moal_private structure
 *
 *  @return             0/MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static int
woal_warm_reset(moal_private * priv)
{
    int ret = 0;
    int intf_num;
    moal_handle *handle = priv->phandle;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *misc = NULL;

    ENTER();

    if (handle->is_suspended && woal_exit_usb_suspend(handle)) {
        PRINTM(MERROR, "Failed to resume the suspended device\n");
        LEAVE();
        return -EFAULT;
    }

    woal_cancel_cac_block(priv);

    /* Reset all interfaces */
    ret = woal_reset_intf(priv, MOAL_IOCTL_WAIT, MTRUE);

    /* Initialize private structures */
    for (intf_num = 0; intf_num < handle->priv_num; intf_num++)
        woal_init_priv(handle->priv[intf_num], MOAL_IOCTL_WAIT);

    /* Restart the firmware */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req) {
        misc = (mlan_ds_misc_cfg *) req->pbuf;
        misc->sub_command = MLAN_OID_MISC_WARM_RESET;
        req->req_id = MLAN_IOCTL_MISC_CFG;
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            kfree(req);
            goto done;
        }
        kfree(req);
    }

    /* Enable interfaces */
    for (intf_num = 0; intf_num < handle->priv_num; intf_num++) {
        netif_device_attach(handle->priv[intf_num]->netdev);
        woal_start_queue(handle->priv[intf_num]->netdev);
    }

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Get signal
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return 	  	0 --success, otherwise fail
 */
static int
woal_get_signal(moal_private * priv, struct iwreq *wrq)
{
/** Input data size */
#define IN_DATA_SIZE	2
/** Output data size */
#define OUT_DATA_SIZE	12
    int ret = 0;
    int in_data[IN_DATA_SIZE];
    int out_data[OUT_DATA_SIZE];
    mlan_ds_get_signal signal;
    int data_length = 0;

    ENTER();

    memset(in_data, 0, sizeof(in_data));
    memset(out_data, 0, sizeof(out_data));

    if (priv->media_connected == MFALSE) {
        PRINTM(MERROR, "Can not get RSSI in disconnected state\n");
        ret = -ENOTSUPP;
        goto done;
    }

    if (wrq->u.data.length) {
        if (sizeof(int) * wrq->u.data.length > sizeof(in_data)) {
            PRINTM(MERROR, "Too many arguments\n");
            ret = -EINVAL;
            goto done;
        }
        if (copy_from_user
            (in_data, wrq->u.data.pointer, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

    switch (wrq->u.data.length) {
    case 0:                    /* No checking, get everything */
        break;
    case 2:                    /* Check subtype range */
        if (in_data[1] < 1 || in_data[1] > 4) {
            ret = -EINVAL;
            goto done;
        }
        /* Fall through */
    case 1:                    /* Check type range */
        if (in_data[0] < 1 || in_data[0] > 3) {
            ret = -EINVAL;
            goto done;
        }
        break;
    default:
        ret = -EINVAL;
        goto done;
    }

    memset(&signal, 0, sizeof(mlan_ds_get_signal));
    if (MLAN_STATUS_SUCCESS !=
        woal_get_signal_info(priv, MOAL_IOCTL_WAIT, &signal)) {
        ret = -EFAULT;
        goto done;
    }
    PRINTM(MINFO, "RSSI Beacon Last   : %d\n", (int) signal.bcn_rssi_last);
    PRINTM(MINFO, "RSSI Beacon Average: %d\n", (int) signal.bcn_rssi_avg);
    PRINTM(MINFO, "RSSI Data Last     : %d\n", (int) signal.data_rssi_last);
    PRINTM(MINFO, "RSSI Data Average  : %d\n", (int) signal.data_rssi_avg);
    PRINTM(MINFO, "SNR Beacon Last    : %d\n", (int) signal.bcn_snr_last);
    PRINTM(MINFO, "SNR Beacon Average : %d\n", (int) signal.bcn_snr_avg);
    PRINTM(MINFO, "SNR Data Last      : %d\n", (int) signal.data_snr_last);
    PRINTM(MINFO, "SNR Data Average   : %d\n", (int) signal.data_snr_avg);
    PRINTM(MINFO, "NF Beacon Last     : %d\n", (int) signal.bcn_nf_last);
    PRINTM(MINFO, "NF Beacon Average  : %d\n", (int) signal.bcn_nf_avg);
    PRINTM(MINFO, "NF Data Last       : %d\n", (int) signal.data_nf_last);
    PRINTM(MINFO, "NF Data Average    : %d\n", (int) signal.data_nf_avg);

    /* Check type */
    switch (in_data[0]) {
    case 0:                    /* Send everything */
        out_data[data_length++] = signal.bcn_rssi_last;
        out_data[data_length++] = signal.bcn_rssi_avg;
        out_data[data_length++] = signal.data_rssi_last;
        out_data[data_length++] = signal.data_rssi_avg;
        out_data[data_length++] = signal.bcn_snr_last;
        out_data[data_length++] = signal.bcn_snr_avg;
        out_data[data_length++] = signal.data_snr_last;
        out_data[data_length++] = signal.data_snr_avg;
        out_data[data_length++] = signal.bcn_nf_last;
        out_data[data_length++] = signal.bcn_nf_avg;
        out_data[data_length++] = signal.data_nf_last;
        out_data[data_length++] = signal.data_nf_avg;
        break;
    case 1:                    /* RSSI */
        /* Check subtype */
        switch (in_data[1]) {
        case 0:                /* Everything */
            out_data[data_length++] = signal.bcn_rssi_last;
            out_data[data_length++] = signal.bcn_rssi_avg;
            out_data[data_length++] = signal.data_rssi_last;
            out_data[data_length++] = signal.data_rssi_avg;
            break;
        case 1:                /* bcn last */
            out_data[data_length++] = signal.bcn_rssi_last;
            break;
        case 2:                /* bcn avg */
            out_data[data_length++] = signal.bcn_rssi_avg;
            break;
        case 3:                /* data last */
            out_data[data_length++] = signal.data_rssi_last;
            break;
        case 4:                /* data avg */
            out_data[data_length++] = signal.data_rssi_avg;
            break;
        default:
            break;
        }
        break;
    case 2:                    /* SNR */
        /* Check subtype */
        switch (in_data[1]) {
        case 0:                /* Everything */
            out_data[data_length++] = signal.bcn_snr_last;
            out_data[data_length++] = signal.bcn_snr_avg;
            out_data[data_length++] = signal.data_snr_last;
            out_data[data_length++] = signal.data_snr_avg;
            break;
        case 1:                /* bcn last */
            out_data[data_length++] = signal.bcn_snr_last;
            break;
        case 2:                /* bcn avg */
            out_data[data_length++] = signal.bcn_snr_avg;
            break;
        case 3:                /* data last */
            out_data[data_length++] = signal.data_snr_last;
            break;
        case 4:                /* data avg */
            out_data[data_length++] = signal.data_snr_avg;
            break;
        default:
            break;
        }
        break;
    case 3:                    /* NF */
        /* Check subtype */
        switch (in_data[1]) {
        case 0:                /* Everything */
            out_data[data_length++] = signal.bcn_nf_last;
            out_data[data_length++] = signal.bcn_nf_avg;
            out_data[data_length++] = signal.data_nf_last;
            out_data[data_length++] = signal.data_nf_avg;
            break;
        case 1:                /* bcn last */
            out_data[data_length++] = signal.bcn_nf_last;
            break;
        case 2:                /* bcn avg */
            out_data[data_length++] = signal.bcn_nf_avg;
            break;
        case 3:                /* data last */
            out_data[data_length++] = signal.data_nf_last;
            break;
        case 4:                /* data avg */
            out_data[data_length++] = signal.data_nf_avg;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    wrq->u.data.length = data_length;
    if (copy_to_user(wrq->u.data.pointer, out_data,
                     wrq->u.data.length * sizeof(out_data[0]))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
  done:
    LEAVE();
    return ret;
}

/** Heart Beat Config Input: No change */
#define HB_NO_CHANGE         -1
/** Heart Beat Config Input: Debug */
#define HB_DEBUG             -2
/** Heart Beat Config Input: Maximum value */
#define HB_MAX_VALUE         0xFFFE
/** Heart Beat Config Input: Minimum value */
#define HB_MIN_VALUE         10

/**
 *  @brief Enable/disable Heart beat feature
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return 	  	0 --success, otherwise fail
 */
static int
woal_heart_beat_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int user_data_len;
    int data[2];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *misc = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    misc->sub_command = MLAN_OID_MISC_HEART_BEAT;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    user_data_len = wrq->u.data.length;
    if (!user_data_len) {
        req->action = MLAN_ACT_GET;
    } else if (user_data_len == 3) {
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * user_data_len)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (data[0] < HB_DEBUG || data[0] > HB_MAX_VALUE ||
            data[1] < HB_DEBUG || data[1] > HB_MAX_VALUE) {
            ret = -EINVAL;
            goto done;
        }
        if ((data[0] > 0 && data[0] < HB_MIN_VALUE) ||
            (data[1] > 0 && data[1] < HB_MIN_VALUE)) {
            PRINTM(MERROR,
                   "The minimum time interval of heart beat is 10 (1s)\n");
            ret = -EINVAL;
            goto done;
        }

        req->action = MLAN_ACT_SET;

        misc->param.heart_beat.h2d_timer = data[0];
        if (!data[0])
            misc->param.heart_beat.h2d_timer = HB_CFG_DISABLE;
        else if (data[0] == HB_NO_CHANGE || data[0] == HB_DEBUG)
            misc->param.heart_beat.h2d_timer = HB_CFG_NO_CHANGE;
        if (data[0] == HB_DEBUG)
            misc->param.heart_beat.debug_for_host = MTRUE;
        misc->param.heart_beat.d2h_timer = data[1];
        if (!data[1] || data[1] == HB_DEBUG)
            misc->param.heart_beat.d2h_timer = HB_CFG_DISABLE;
        else if (data[1] == HB_NO_CHANGE)
            misc->param.heart_beat.d2h_timer = HB_CFG_NO_CHANGE;
        if (data[1] == HB_DEBUG)
            misc->param.heart_beat.debug_for_device = MTRUE;
        misc->param.heart_beat.reconnect_delay = data[2];

        /* Remember the settings */
        if (misc->param.heart_beat.h2d_timer == HB_CFG_DISABLE)
            priv->is_h2d_active = MFALSE;
        else if (misc->param.heart_beat.h2d_timer != HB_CFG_NO_CHANGE)
            priv->is_h2d_active = MTRUE;
        if (misc->param.heart_beat.d2h_timer == HB_CFG_DISABLE)
            priv->is_d2h_active = MFALSE;
        else if (misc->param.heart_beat.d2h_timer != HB_CFG_NO_CHANGE)
            priv->is_d2h_active = MTRUE;
    } else {
        PRINTM(MERROR, "Invalid number of args!\n");
        ret = -EINVAL;
        goto done;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    data[0] = misc->param.heart_beat.h2d_timer;
    data[1] = misc->param.heart_beat.d2h_timer;
    data[2] = misc->param.heart_beat.reconnect_delay;
    wrq->u.data.length = 2;
    if (copy_to_user
        (wrq->u.data.pointer, data, sizeof(int) * wrq->u.data.length)) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Get Deep Sleep
 *
 *  @param priv         Pointer to the moal_private driver data struct
 *  @param deep_sleep   Pointer to return deep_sleep setting
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_get_deep_sleep(moal_private * priv, t_u32 * data)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pm = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    pm = (mlan_ds_pm_cfg *) req->pbuf;
    pm->sub_command = MLAN_OID_PM_CFG_DEEP_SLEEP;
    req->req_id = MLAN_IOCTL_PM_CFG;

    req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    *data = pm->param.auto_deep_sleep.auto_ds;
    *(data + 1) = pm->param.auto_deep_sleep.idletime;

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Get/Set DeepSleep mode
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wreq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_deep_sleep_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    t_u32 deep_sleep = DEEP_SLEEP_OFF;
    t_u32 data[2];
    t_u16 idletime = DEEP_SLEEP_IDLE_TIME;

    ENTER();

    if (wrq->u.data.length == 1 || wrq->u.data.length == 2) {
        if (copy_from_user
            (&data, wrq->u.data.pointer, wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            LEAVE();
            return -EFAULT;
        }
        deep_sleep = data[0];
        if (deep_sleep == DEEP_SLEEP_OFF) {
            PRINTM(MINFO, "Exit Deep Sleep Mode\n");
            ret = woal_set_deep_sleep(priv, MOAL_IOCTL_WAIT, MFALSE, 0);
            if (ret != MLAN_STATUS_SUCCESS) {
                LEAVE();
                return -EINVAL;
            }
        } else if (deep_sleep == DEEP_SLEEP_ON) {
            PRINTM(MINFO, "Enter Deep Sleep Mode\n");
            if (wrq->u.data.length == 2)
                idletime = data[1];
            else
                idletime = 0;
            ret = woal_set_deep_sleep(priv, MOAL_IOCTL_WAIT, MTRUE, idletime);
            if (ret != MLAN_STATUS_SUCCESS) {
                LEAVE();
                return -EINVAL;
            }
        } else {
            PRINTM(MERROR, "Unknown option = %lu\n", deep_sleep);
            LEAVE();
            return -EINVAL;
        }
    } else if (wrq->u.data.length > 2) {
        PRINTM(MERROR, "Invalid number of arguments %d\n", wrq->u.data.length);
        LEAVE();
        return -EINVAL;
    } else {                    /* Display Deep Sleep settings */
        PRINTM(MINFO, "Get Deep Sleep Mode\n");
        if (MLAN_STATUS_SUCCESS != woal_get_deep_sleep(priv, data)) {
            LEAVE();
            return -EFAULT;
        }
        if (data[0] == 0)
            wrq->u.data.length = 1;
        else
            wrq->u.data.length = 2;
    }

    /* Copy the Deep Sleep setting to user */
    if (copy_to_user
        (wrq->u.data.pointer, data, wrq->u.data.length * sizeof(int))) {
        PRINTM(MERROR, "Copy to user failed\n");
        LEAVE();
        return -EINVAL;
    }

    LEAVE();
    return 0;
}

/**
 *  @brief Set/Get Usr 11n configuration request
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_htcap_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[2];
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;
    int data_length = wrq->u.data.length;

    ENTER();

    if (data_length > 2) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }

    if (((req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg))) == NULL)) {
        ret = -ENOMEM;
        goto done;
    }

    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_HTCAP_CFG;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (data_length == 0) {
        /* Get 11n tx parameters from MLAN */
        req->action = MLAN_ACT_GET;
        cfg_11n->param.htcap_cfg.misc_cfg = BAND_SELECT_BG;
    } else {
        if (copy_from_user
            (data, wrq->u.data.pointer, data_length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg_11n->param.htcap_cfg.htcap = data[0];
        PRINTM(MINFO, "SET: htcapinfo:0x%x\n", data[0]);
        cfg_11n->param.htcap_cfg.misc_cfg = BAND_SELECT_BOTH;
        if (data_length == 2) {
            if (data[1] != BAND_SELECT_BG &&
                data[1] != BAND_SELECT_A && data[1] != BAND_SELECT_BOTH) {
                PRINTM(MERROR, "Invalid band selection\n");
                ret = -EINVAL;
                goto done;
            }
            cfg_11n->param.htcap_cfg.misc_cfg = data[1];
            PRINTM(MINFO, "SET: htcapinfo band:0x%x\n", data[1]);
        }
        /* Update 11n tx parameters in MLAN */
        req->action = MLAN_ACT_SET;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    data[0] = cfg_11n->param.htcap_cfg.htcap;

    if (req->action == MLAN_ACT_GET) {
        data_length = 1;
        cfg_11n->param.htcap_cfg.htcap = 0;
        cfg_11n->param.htcap_cfg.misc_cfg = BAND_SELECT_A;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        if (cfg_11n->param.htcap_cfg.htcap != data[0]) {
            data_length = 2;
            data[1] = cfg_11n->param.htcap_cfg.htcap;
            PRINTM(MINFO, "GET: htcapinfo for 2.4GHz:0x%x\n", data[0]);
            PRINTM(MINFO, "GET: htcapinfo for 5GHz:0x%x\n", data[1]);
        } else
            PRINTM(MINFO, "GET: htcapinfo:0x%x\n", data[0]);
    }

    if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

    wrq->u.data.length = data_length;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Enable/Disable amsdu_aggr_ctrl
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_amsdu_aggr_ctrl(moal_private * priv, struct iwreq *wrq)
{
    int data[2];
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();

    if ((wrq->u.data.length != 0) && (wrq->u.data.length != 1)) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_AMSDU_AGGR_CTRL;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get 11n tx parameters from MLAN */
        req->action = MLAN_ACT_GET;
    } else if (wrq->u.data.length == 1) {
        if (copy_from_user(data, wrq->u.data.pointer,
                           wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        cfg_11n->param.amsdu_aggr_ctrl.enable = data[0];
        /* Update 11n tx parameters in MLAN */
        req->action = MLAN_ACT_SET;
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    data[0] = cfg_11n->param.amsdu_aggr_ctrl.enable;
    data[1] = cfg_11n->param.amsdu_aggr_ctrl.curr_buf_size;

    if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 2;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get 11n configuration request
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_tx_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[2];
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;
    int data_length = wrq->u.data.length;

    ENTER();

    if (data_length > 2) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_TX;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get 11n tx parameters from MLAN */
        req->action = MLAN_ACT_GET;
        cfg_11n->param.tx_cfg.misc_cfg = BAND_SELECT_BG;
    } else {
        if (copy_from_user
            (data, wrq->u.data.pointer, data_length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg_11n->param.tx_cfg.httxcap = data[0];
        PRINTM(MINFO, "SET: httxcap:0x%x\n", data[0]);
        cfg_11n->param.tx_cfg.misc_cfg = BAND_SELECT_BOTH;
        if (data_length == 2) {
            if (data[1] != BAND_SELECT_BG &&
                data[1] != BAND_SELECT_A && data[1] != BAND_SELECT_BOTH) {
                PRINTM(MERROR, "Invalid band selection\n");
                ret = -EINVAL;
                goto done;
            }
            cfg_11n->param.tx_cfg.misc_cfg = data[1];
            PRINTM(MINFO, "SET: httxcap band:0x%x\n", data[1]);
        }
        /* Update 11n tx parameters in MLAN */
        req->action = MLAN_ACT_SET;
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    data[0] = cfg_11n->param.tx_cfg.httxcap;

    if (req->action == MLAN_ACT_GET) {
        data_length = 1;
        cfg_11n->param.tx_cfg.httxcap = 0;
        cfg_11n->param.tx_cfg.misc_cfg = BAND_SELECT_A;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        if (cfg_11n->param.tx_cfg.httxcap != data[0]) {
            data_length = 2;
            data[1] = cfg_11n->param.tx_cfg.httxcap;
            PRINTM(MINFO, "GET: httxcap for 2.4GHz:0x%x\n", data[0]);
            PRINTM(MINFO, "GET: httxcap for 5GHz:0x%x\n", data[1]);
        } else
            PRINTM(MINFO, "GET: httxcap:0x%x\n", data[0]);
    }

    if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = data_length;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Enable/Disable TX Aggregation
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_prio_tbl(moal_private * priv, struct iwreq *wrq)
{
    int data[MAX_NUM_TID * 2], i, j;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();

    if ((wrq->u.data.pointer == NULL)) {
        LEAVE();
        return -EINVAL;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_AGGR_PRIO_TBL;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get aggr priority table from MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
        wrq->u.data.length = MAX_NUM_TID * 2;
        for (i = 0, j = 0; i < (wrq->u.data.length); i = i + 2, ++j) {
            data[i] = cfg_11n->param.aggr_prio_tbl.ampdu[j];
            data[i + 1] = cfg_11n->param.aggr_prio_tbl.amsdu[j];
        }

        if (copy_to_user(wrq->u.data.pointer, data,
                         sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto error;
        }
    } else if (wrq->u.data.length == 16) {
        if (copy_from_user(data, wrq->u.data.pointer,
                           sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }
        for (i = 0, j = 0; i < (wrq->u.data.length); i = i + 2, ++j) {
            if ((data[i] > 7 && data[i] != 0xff) ||
                (data[i + 1] > 7 && data[i + 1] != 0xff)) {
                PRINTM(MERROR, "Invalid priority, valid value 0-7 or 0xff.\n");
                ret = -EFAULT;
                goto error;
            }
            cfg_11n->param.aggr_prio_tbl.ampdu[j] = data[i];
            cfg_11n->param.aggr_prio_tbl.amsdu[j] = data[i + 1];
        }

        /* Update aggr priority table in MLAN */
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
    } else {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto error;
    }

  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get add BA Reject parameters
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_addba_reject(moal_private * priv, struct iwreq *wrq)
{
    int data[MAX_NUM_TID], ret = 0, i;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_ADDBA_REJECT;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        PRINTM(MERROR, "Addba reject moal\n");
        /* Get aggr priority table from MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }

        wrq->u.data.length = MAX_NUM_TID;
        for (i = 0; i < (wrq->u.data.length); ++i) {
            data[i] = cfg_11n->param.addba_reject[i];
        }

        if (copy_to_user(wrq->u.data.pointer, data,
                         sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto error;
        }
    } else if (wrq->u.data.length == 8) {
        if (copy_from_user(data, wrq->u.data.pointer,
                           sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }
        for (i = 0; i < (wrq->u.data.length); ++i) {
            if (data[i] != 0 && data[i] != 1) {
                PRINTM(MERROR, "addba reject only takes argument as 0 or 1\n");
                ret = -EFAULT;
                goto error;
            }
            cfg_11n->param.addba_reject[i] = data[i];
        }

        /* Update aggr priority table in MLAN */
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
    } else {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto error;
    }
  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get add BA parameters
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_addba_para_updt(moal_private * priv, struct iwreq *wrq)
{
    int data[5], ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_ADDBA_PARAM;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get Add BA parameters from MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
        data[0] = cfg_11n->param.addba_param.timeout;
        data[1] = cfg_11n->param.addba_param.txwinsize;
        data[2] = cfg_11n->param.addba_param.rxwinsize;
        data[3] = cfg_11n->param.addba_param.txamsdu;
        data[4] = cfg_11n->param.addba_param.rxamsdu;
        PRINTM(MINFO,
               "GET: timeout:%d txwinsize:%d rxwinsize:%d txamsdu=%d, rxamsdu=%d\n",
               data[0], data[1], data[2], data[3], data[4]);
        wrq->u.data.length = 5;
        if (copy_to_user(wrq->u.data.pointer, data,
                         wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto error;
        }
    } else if (wrq->u.data.length == 5) {
        if (copy_from_user
            (data, wrq->u.data.pointer, wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }
        if (data[0] < 0 || data[0] > MLAN_DEFAULT_BLOCK_ACK_TIMEOUT) {
            PRINTM(MERROR, "Incorrect addba timeout value.\n");
            ret = -EFAULT;
            goto error;
        }
        if (data[1] <= 0 || data[1] > MLAN_AMPDU_MAX_TXWINSIZE || data[2] <= 0
            || data[2] > MLAN_AMPDU_MAX_RXWINSIZE) {
            PRINTM(MERROR, "Incorrect Tx/Rx window size.\n");
            ret = -EFAULT;
            goto error;
        }
        cfg_11n->param.addba_param.timeout = data[0];
        cfg_11n->param.addba_param.txwinsize = data[1];
        cfg_11n->param.addba_param.rxwinsize = data[2];
        if (data[3] < 0 || data[3] > 1 || data[4] < 0 || data[4] > 1) {
            PRINTM(MERROR, "Incorrect Tx/Rx amsdu.\n");
            ret = -EFAULT;
            goto error;
        }
        cfg_11n->param.addba_param.txamsdu = data[3];
        cfg_11n->param.addba_param.rxamsdu = data[4];
        PRINTM(MINFO,
               "SET: timeout:%d txwinsize:%d rxwinsize:%d txamsdu=%d rxamsdu=%d\n",
               data[0], data[1], data[2], data[3], data[4]);
        /* Update Add BA parameters in MLAN */
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = MLAN_STATUS_FAILURE;
            goto error;
        }
    } else {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto error;
    }

  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Transmit buffer size
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_txbuf_cfg(moal_private * priv, struct iwreq *wrq)
{
    int buf_size;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_MAX_TX_BUF_SIZE;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get Tx buffer size from MLAN */
        req->action = MLAN_ACT_GET;
    } else {
        ret = -EINVAL;
        PRINTM(MERROR,
               "Don't support set Tx buffer size after driver loaded!\n");
        goto done;
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    buf_size = cfg_11n->param.tx_buf_size;
    if (copy_to_user(wrq->u.data.pointer, &buf_size, sizeof(buf_size))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Host Sleep configuration
 *
 *  @param priv             A pointer to moal_private structure
 *  @param wrq	            A pointer to iwreq structure
 *  @param invoke_hostcmd	MTRUE --invoke HostCmd, otherwise MFALSE
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_hs_cfg(moal_private * priv, struct iwreq *wrq, BOOLEAN invoke_hostcmd)
{
    int data[3];
    int ret = 0;
    mlan_ds_hs_cfg hscfg;
    t_u16 action;
    mlan_bss_info bss_info;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));
    memset(&hscfg, 0, sizeof(mlan_ds_hs_cfg));

    if (data_length == 0) {
        action = MLAN_ACT_GET;
    } else {
        action = MLAN_ACT_SET;
        if (data_length >= 1 && data_length <= 3) {
            if (copy_from_user
                (data, wrq->u.data.pointer, sizeof(int) * data_length)) {
                PRINTM(MERROR, "Copy from user failed\n");
                ret = -EFAULT;
                goto done;
            }
        } else {
            PRINTM(MERROR, "Invalid arguments\n");
            ret = -EINVAL;
            goto done;
        }
    }

    /* HS config is blocked if HS is already activated */
    if (data_length &&
        (data[0] != HOST_SLEEP_CFG_CANCEL || invoke_hostcmd == MFALSE)) {
        memset(&bss_info, 0, sizeof(bss_info));
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);
        if (bss_info.is_hs_configured) {
            PRINTM(MERROR, "HS already configured\n");
            ret = -EFAULT;
            goto done;
        }
    }

    /* Do a GET first if some arguments are not provided */
    if (data_length >= 1 && data_length < 3) {
        woal_set_get_hs_params(priv, MLAN_ACT_GET, MOAL_IOCTL_WAIT, &hscfg);
    }

    if (data_length)
        hscfg.conditions = data[0];
    if (data_length >= 2)
        hscfg.gpio = data[1];
    if (data_length == 3)
        hscfg.gap = data[2];

    if ((invoke_hostcmd == MTRUE) && (action == MLAN_ACT_SET)) {
        /* Need to issue an extra IOCTL first to set up parameters */
        hscfg.is_invoke_hostcmd = MFALSE;
        if (MLAN_STATUS_SUCCESS !=
            woal_set_get_hs_params(priv, MLAN_ACT_SET, MOAL_IOCTL_WAIT,
                                   &hscfg)) {
            ret = -EFAULT;
            goto done;
        }
    }
    hscfg.is_invoke_hostcmd = invoke_hostcmd;
    if (MLAN_STATUS_SUCCESS !=
        woal_set_get_hs_params(priv, action, MOAL_IOCTL_WAIT, &hscfg)) {
        ret = -EFAULT;
        goto done;
    }

    if (action == MLAN_ACT_GET) {
        data[0] = hscfg.conditions;
        data[1] = hscfg.gpio;
        data[2] = hscfg.gap;
        wrq->u.data.length = 3;
        if (copy_to_user
            (wrq->u.data.pointer, data, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Set Host Sleep parameters
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_hs_setpara(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    int data_length = wrq->u.data.length;

    ENTER();

    if (data_length >= 1 && data_length <= 3) {
        ret = woal_hs_cfg(priv, wrq, MFALSE);
        goto done;
    } else {
        PRINTM(MERROR, "Invalid arguments\n");
        ret = -EINVAL;
        goto done;
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Get/Set inactivity timeout extend
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_inactivity_timeout_ext(moal_private * priv, struct iwreq *wrq)
{
    int data[4];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pmcfg = NULL;
    pmlan_ds_inactivity_to inac_to = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    inac_to = &pmcfg->param.inactivity_to;
    pmcfg->sub_command = MLAN_OID_PM_CFG_INACTIVITY_TO;
    req->req_id = MLAN_IOCTL_PM_CFG;

    if ((data_length != 0 && data_length != 3 && data_length != 4) ||
        sizeof(int) * data_length > sizeof(data)) {
        PRINTM(MERROR, "Invalid number of parameters\n");
        ret = -EINVAL;
        goto done;
    }

    req->action = MLAN_ACT_GET;
    if (data_length) {
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * data_length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        inac_to->timeout_unit = data[0];
        inac_to->unicast_timeout = data[1];
        inac_to->mcast_timeout = data[2];
        inac_to->ps_entry_timeout = data[3];
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    /* Copy back current values regardless of GET/SET */
    data[0] = inac_to->timeout_unit;
    data[1] = inac_to->unicast_timeout;
    data[2] = inac_to->mcast_timeout;
    data[3] = inac_to->ps_entry_timeout;

    if (req->action == MLAN_ACT_GET) {
        wrq->u.data.length = 4;
        if (copy_to_user
            (wrq->u.data.pointer, data, wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get system clock
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_ecl_sys_clock(moal_private * priv, struct iwreq *wrq)
{
    int data[64];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *cfg = NULL;
    int data_length = wrq->u.data.length;
    int i = 0;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg = (mlan_ds_misc_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_MISC_SYS_CLOCK;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    if (!data_length)
        req->action = MLAN_ACT_GET;
    else if (data_length <= MLAN_MAX_CLK_NUM) {
        req->action = MLAN_ACT_SET;
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * data_length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    } else {
        PRINTM(MERROR, "Invalid arguments\n");
        ret = -EINVAL;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        /* Get configurable clocks */
        cfg->param.sys_clock.sys_clk_type = MLAN_CLK_CONFIGURABLE;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        /* Current system clock */
        data[0] = (int) cfg->param.sys_clock.cur_sys_clk;
        wrq->u.data.length = 1;

        data_length = MIN(cfg->param.sys_clock.sys_clk_num, MLAN_MAX_CLK_NUM);

        /* Configurable clocks */
        for (i = 0; i < data_length; i++) {
            data[i + wrq->u.data.length] =
                (int) cfg->param.sys_clock.sys_clk[i];
        }
        wrq->u.data.length += data_length;

        /* Get supported clocks */
        cfg->param.sys_clock.sys_clk_type = MLAN_CLK_SUPPORTED;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        data_length = MIN(cfg->param.sys_clock.sys_clk_num, MLAN_MAX_CLK_NUM);

        /* Supported clocks */
        for (i = 0; i < data_length; i++) {
            data[i + wrq->u.data.length] =
                (int) cfg->param.sys_clock.sys_clk[i];
        }

        wrq->u.data.length += data_length;

        if (copy_to_user
            (wrq->u.data.pointer, data, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    } else {
        /* Set configurable clocks */
        cfg->param.sys_clock.sys_clk_type = MLAN_CLK_CONFIGURABLE;
        cfg->param.sys_clock.sys_clk_num = MIN(MLAN_MAX_CLK_NUM, data_length);
        for (i = 0; i < cfg->param.sys_clock.sys_clk_num; i++) {
            cfg->param.sys_clock.sys_clk[i] = (t_u16) data[i];
        }

        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Band and Adhoc-band setting
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_band_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    unsigned int i;
    int data[4];
    int user_data_len = wrq->u.data.length;
    t_u32 infra_band = 0;
    t_u32 adhoc_band = 0;
    t_u32 adhoc_channel = 0;
    t_u32 sec_chan_offset = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_radio_cfg *radio_cfg = NULL;

    ENTER();

    if (sizeof(int) * user_data_len > sizeof(data)) {
        PRINTM(MERROR, "Too many arguments\n");
        LEAVE();
        return -EINVAL;
    }

    if (user_data_len > 0) {
        if (priv->media_connected == MTRUE) {
            LEAVE();
            return -EOPNOTSUPP;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_radio_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto error;
    }
    radio_cfg = (mlan_ds_radio_cfg *) req->pbuf;
    radio_cfg->sub_command = MLAN_OID_BAND_CFG;
    req->req_id = MLAN_IOCTL_RADIO_CFG;

    if (wrq->u.data.length == 0) {
        /* Get config_bands, adhoc_start_band and adhoc_channel values from
           MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
        data[0] = radio_cfg->param.band_cfg.config_bands;       /* Infra Band */
        data[1] = radio_cfg->param.band_cfg.adhoc_start_band;   /* Adhoc Band */
        data[2] = radio_cfg->param.band_cfg.adhoc_channel;      /* Adhoc
                                                                   Channel */
        wrq->u.data.length = 3;
        if (radio_cfg->param.band_cfg.adhoc_start_band & BAND_GN
            || radio_cfg->param.band_cfg.adhoc_start_band & BAND_AN) {
            data[3] = radio_cfg->param.band_cfg.sec_chan_offset;        /* secondary
                                                                           offset
                                                                         */
            wrq->u.data.length = 4;
        }

        if (copy_to_user
            (wrq->u.data.pointer, data, sizeof(int) * wrq->u.data.length)) {
            ret = -EFAULT;
            goto error;
        }
    } else {
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }

        /* To support only <b/bg/bgn/n> */
        infra_band = data[0];
        for (i = 0; i < sizeof(SupportedInfraBand); i++)
            if (infra_band == SupportedInfraBand[i])
                break;
        if (i == sizeof(SupportedInfraBand)) {
            ret = -EINVAL;
            goto error;
        }

        /* Set Adhoc band */
        if (user_data_len >= 2) {
            adhoc_band = data[1];
            for (i = 0; i < sizeof(SupportedAdhocBand); i++)
                if (adhoc_band == SupportedAdhocBand[i])
                    break;
            if (i == sizeof(SupportedAdhocBand)) {
                ret = -EINVAL;
                goto error;
            }
        }

        /* Set Adhoc channel */
        if (user_data_len >= 3) {
            adhoc_channel = data[2];
            if (adhoc_channel == 0) {
                /* Check if specified adhoc channel is non-zero */
                ret = -EINVAL;
                goto error;
            }
        }
        if (user_data_len == 4) {
            if (!(adhoc_band & (BAND_GN | BAND_AN))) {
                PRINTM(MERROR,
                       "11n is not enabled for adhoc, can not set secondary channel offset\n");
                ret = -EINVAL;
                goto error;
            }
            sec_chan_offset = data[3];
            if ((sec_chan_offset != NO_SEC_CHANNEL) &&
                (sec_chan_offset != SEC_CHANNEL_ABOVE) &&
                (sec_chan_offset != SEC_CHANNEL_BELOW)) {
                PRINTM(MERROR,
                       "Invalid secondary channel offset, only allowed 0, 1 or 3\n");
                ret = -EINVAL;
                goto error;
            }
        }
        /* Set config_bands and adhoc_start_band values to MLAN */
        req->action = MLAN_ACT_SET;
        radio_cfg->param.band_cfg.config_bands = infra_band;
        radio_cfg->param.band_cfg.adhoc_start_band = adhoc_band;
        radio_cfg->param.band_cfg.adhoc_channel = adhoc_channel;
        radio_cfg->param.band_cfg.sec_chan_offset = sec_chan_offset;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
    }

  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Read/Write adapter registers value
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_reg_read_write(moal_private * priv, struct iwreq *wrq)
{
    int data[3];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_reg_mem *reg = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_reg_mem));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    reg = (mlan_ds_reg_mem *) req->pbuf;
    reg->sub_command = MLAN_OID_REG_RW;
    req->req_id = MLAN_IOCTL_REG_MEM;

    if (data_length == 2) {
        req->action = MLAN_ACT_GET;
    } else if (data_length == 3) {
        req->action = MLAN_ACT_SET;
    } else {
        ret = -EINVAL;
        goto done;
    }
    if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) * data_length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    reg->param.reg_rw.type = (t_u32) data[0];
    reg->param.reg_rw.offset = (t_u32) data[1];
    if (data_length == 3)
        reg->param.reg_rw.value = (t_u32) data[2];

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        if (copy_to_user
            (wrq->u.data.pointer, &reg->param.reg_rw.value, sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Read the EEPROM contents of the card
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_read_eeprom(moal_private * priv, struct iwreq *wrq)
{
    int data[2];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_reg_mem *reg = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_reg_mem));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    reg = (mlan_ds_reg_mem *) req->pbuf;
    reg->sub_command = MLAN_OID_EEPROM_RD;
    req->req_id = MLAN_IOCTL_REG_MEM;

    if (data_length == 2) {
        req->action = MLAN_ACT_GET;
    } else {
        ret = -EINVAL;
        goto done;
    }
    if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) * data_length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    reg->param.rd_eeprom.offset = (t_u16) data[0];
    reg->param.rd_eeprom.byte_count = (t_u16) data[1];

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        wrq->u.data.length = reg->param.rd_eeprom.byte_count;
        if (copy_to_user
            (wrq->u.data.pointer, reg->param.rd_eeprom.value,
             MIN(wrq->u.data.length, MAX_EEPROM_DATA))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Read/Write device memory value
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_mem_read_write(moal_private * priv, struct iwreq *wrq)
{
    t_u32 data[2];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_reg_mem *reg_mem = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_reg_mem));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    reg_mem = (mlan_ds_reg_mem *) req->pbuf;
    reg_mem->sub_command = MLAN_OID_MEM_RW;
    req->req_id = MLAN_IOCTL_REG_MEM;

    if (data_length == 1) {
        PRINTM(MINFO, "MEM_RW: GET\n");
        req->action = MLAN_ACT_GET;
    } else if (data_length == 2) {
        PRINTM(MINFO, "MEM_RW: SET\n");
        req->action = MLAN_ACT_SET;
    } else {
        ret = -EINVAL;
        goto done;
    }
    if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) * data_length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    reg_mem->param.mem_rw.addr = (t_u32) data[0];
    if (data_length == 2)
        reg_mem->param.mem_rw.value = (t_u32) data[1];

    PRINTM(MINFO, "MEM_RW: Addr=0x%x, Value=0x%x\n",
           (int) reg_mem->param.mem_rw.addr, (int) reg_mem->param.mem_rw.value);

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        if (copy_to_user
            (wrq->u.data.pointer, &reg_mem->param.mem_rw.value, sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get LOG
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wrq			A pointer to iwreq structure
 *
 *  @return 	   	 	0 --success, otherwise fail
 */
static int
woal_get_log(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_get_stats stats;
    char *buf = NULL;

    ENTER();

    PRINTM(MINFO, " GET STATS\n");
    if (!(buf = kmalloc(GETLOG_BUFSIZE, GFP_KERNEL))) {
        PRINTM(MERROR, "kmalloc failed!\n");
        ret = -ENOMEM;
        goto done;
    }

    memset(&stats, 0, sizeof(mlan_ds_get_stats));
    if (MLAN_STATUS_SUCCESS !=
        woal_get_stats_info(priv, MOAL_IOCTL_WAIT, &stats)) {
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.pointer) {
        sprintf(buf, "\n"
                "mcasttxframe     %lu\n"
                "failed           %lu\n"
                "retry            %lu\n"
                "multiretry       %lu\n"
                "framedup         %lu\n"
                "rtssuccess       %lu\n"
                "rtsfailure       %lu\n"
                "ackfailure       %lu\n"
                "rxfrag           %lu\n"
                "mcastrxframe     %lu\n"
                "fcserror         %lu\n"
                "txframe          %lu\n"
                "wepicverrcnt-1   %lu\n"
                "wepicverrcnt-2   %lu\n"
                "wepicverrcnt-3   %lu\n"
                "wepicverrcnt-4   %lu\n",
                stats.mcast_tx_frame,
                stats.failed,
                stats.retry,
                stats.multi_retry,
                stats.frame_dup,
                stats.rts_success,
                stats.rts_failure,
                stats.ack_failure,
                stats.rx_frag,
                stats.mcast_rx_frame,
                stats.fcs_error,
                stats.tx_frame,
                stats.wep_icv_error[0],
                stats.wep_icv_error[1],
                stats.wep_icv_error[2], stats.wep_icv_error[3]);
        wrq->u.data.length = strlen(buf) + 1;
        if (copy_to_user(wrq->u.data.pointer, buf, wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
        }
    }
  done:
    if (buf)
        kfree(buf);
    LEAVE();
    return ret;
}

/**
 *  @brief Deauthenticate
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wrq			A pointer to iwreq structure
 *
 *  @return 	   	 	0 --success, otherwise fail
 */
static int
woal_deauth(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    struct sockaddr saddr;

    ENTER();
    if (wrq->u.data.length) {
        /* Deauth mentioned BSSID */
        if (copy_from_user(&saddr, wrq->u.data.pointer, sizeof(saddr))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (MLAN_STATUS_SUCCESS !=
            woal_disconnect(priv, MOAL_IOCTL_WAIT, (t_u8 *) saddr.sa_data)) {
            ret = -EFAULT;
            goto done;
        }
    } else {
        if (MLAN_STATUS_SUCCESS != woal_disconnect(priv, MOAL_IOCTL_WAIT, NULL))
            ret = -EFAULT;
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get TX power configurations
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_tx_power_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[5], user_data_len;
    int ret = 0;
    mlan_bss_info bss_info;
    mlan_ds_power_cfg *pcfg = NULL;
    mlan_ioctl_req *req = NULL;
    ENTER();

    memset(&bss_info, 0, sizeof(bss_info));
    woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);

    memset(data, 0, sizeof(data));
    user_data_len = wrq->u.data.length;

    if (user_data_len) {
        if (sizeof(int) * user_data_len > sizeof(data)) {
            PRINTM(MERROR, "Too many arguments\n");
            ret = -EINVAL;
            goto done;
        }
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * user_data_len)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        switch (user_data_len) {
        case 1:
            if (data[0] != 0xFF)
                ret = -EINVAL;
            break;
        case 2:
        case 4:
            if (data[0] == 0xFF) {
                ret = -EINVAL;
                break;
            }
            if ((unsigned int) data[1] < bss_info.min_power_level) {
                PRINTM(MERROR,
                       "The set powercfg rate value %d dBm is out of range (%d dBm-%d dBm)!\n",
                       data[1], (int) bss_info.min_power_level,
                       (int) bss_info.max_power_level);
                ret = -EINVAL;
                break;
            }
            if (user_data_len == 4) {
                if (data[1] > data[2]) {
                    PRINTM(MERROR, "Min power should be less than maximum!\n");
                    ret = -EINVAL;
                    break;
                }
                if (data[3] < 0) {
                    PRINTM(MERROR, "Step should not less than 0!\n");
                    ret = -EINVAL;
                    break;
                }
                if ((unsigned int) data[2] > bss_info.max_power_level) {
                    PRINTM(MERROR,
                           "The set powercfg rate value %d dBm is out of range (%d dBm-%d dBm)!\n",
                           data[2], (int) bss_info.min_power_level,
                           (int) bss_info.max_power_level);
                    ret = -EINVAL;
                    break;
                }
                if (data[3] > data[2] - data[1]) {
                    PRINTM(MERROR,
                           "Step should not greater than power difference!\n");
                    ret = -EINVAL;
                    break;
                }
            }
            break;
        default:
            ret = -EINVAL;
            break;
        }
        if (ret)
            goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_power_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    pcfg = (mlan_ds_power_cfg *) req->pbuf;
    pcfg->sub_command = MLAN_OID_POWER_CFG_EXT;
    req->req_id = MLAN_IOCTL_POWER_CFG;
    if (!user_data_len)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        pcfg->param.power_ext.len = user_data_len;
        memcpy((t_u8 *) & pcfg->param.power_ext.power_data, (t_u8 *) data,
               sizeof(data));
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!user_data_len) {
        if (copy_to_user
            (wrq->u.data.pointer, (t_u8 *) & pcfg->param.power_ext.power_data,
             sizeof(int) * pcfg->param.power_ext.len)) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = pcfg->param.power_ext.len;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get Tx/Rx data rates
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_get_txrx_rate(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_rate *rate = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    rate = (mlan_ds_rate *) req->pbuf;
    rate->sub_command = MLAN_OID_GET_DATA_RATE;
    req->req_id = MLAN_IOCTL_RATE;
    req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & rate->param.data_rate,
         sizeof(int) * 2)) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 2;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get beacon interval
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_beacon_interval(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    int bcn = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&bcn, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((bcn < MLAN_MIN_BEACON_INTERVAL) ||
            (bcn > MLAN_MAX_BEACON_INTERVAL)) {
            ret = -EINVAL;
            goto done;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_IBSS_BCN_INTERVAL;
    req->req_id = MLAN_IOCTL_BSS;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        bss->param.bcn_interval = bcn;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & bss->param.bcn_interval,
         sizeof(int))) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get ATIM window
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_atim_window(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    int atim = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&atim, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((atim < 0) || (atim > MLAN_MAX_ATIM_WINDOW)) {
            ret = -EINVAL;
            goto done;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_IBSS_ATIM_WINDOW;
    req->req_id = MLAN_IOCTL_BSS;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        bss->param.atim_window = atim;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & bss->param.atim_window, sizeof(int))) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get TX data rate
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return           0 --success, otherwise fail
 */
static int
woal_set_get_txrate(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_rate *rate = NULL;
    mlan_ioctl_req *req = NULL;
    int rateindex = 0;

    ENTER();
    if (wrq->u.data.length) {
        if (copy_from_user(&rateindex, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    rate = (mlan_ds_rate *) req->pbuf;
    rate->param.rate_cfg.rate_type = MLAN_RATE_INDEX;
    rate->sub_command = MLAN_OID_RATE_CFG;
    req->req_id = MLAN_IOCTL_RATE;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        if (rateindex == AUTO_RATE)
            rate->param.rate_cfg.is_rate_auto = 1;
        else {
            if ((rateindex != MLAN_RATE_INDEX_MCS32) &&
                ((rateindex < 0) || (rateindex > MLAN_RATE_INDEX_MCS15))) {
                ret = -EINVAL;
                goto done;
            }
        }
        rate->param.rate_cfg.rate = rateindex;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    } else {
        if (wrq->u.data.length)
            priv->rate_index = rateindex;
    }
    if (!wrq->u.data.length) {
        if (rate->param.rate_cfg.is_rate_auto)
            rateindex = AUTO_RATE;
        else
            rateindex = rate->param.rate_cfg.rate;
        wrq->u.data.length = 1;
        if (copy_to_user(wrq->u.data.pointer, &rateindex, sizeof(int))) {
            ret = -EFAULT;
        }
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get region code
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return           0 --success, otherwise fail
 */
static int
woal_set_get_regioncode(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_misc_cfg *cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int region = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&region, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg = (mlan_ds_misc_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_MISC_REGION;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        cfg->param.region_code = region;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (!wrq->u.data.length) {
        wrq->u.data.length = 1;
        if (copy_to_user
            (wrq->u.data.pointer, &cfg->param.region_code, sizeof(int)))
            ret = -EFAULT;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get radio
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return           0 --success, otherwise fail
 */
static int
woal_set_get_radio(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_bss_info bss_info;
    int option = 0;

    ENTER();

    memset(&bss_info, 0, sizeof(bss_info));

    if (wrq->u.data.length) {
        /* Set radio */
        if (copy_from_user(&option, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (MLAN_STATUS_SUCCESS != woal_set_radio(priv, (t_u8) option))
            ret = -EFAULT;
    } else {
        /* Get radio status */
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);
        wrq->u.data.length = 1;
        if (copy_to_user
            (wrq->u.data.pointer, &bss_info.radio_on,
             sizeof(bss_info.radio_on))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
        }
    }
  done:
    LEAVE();
    return ret;
}

#ifdef DEBUG_LEVEL1
/**
 *  @brief Get/Set the bit mask of driver debug message control
 *
 *  @param priv			A pointer to moal_private structure
 *  @param wrq			A pointer to wrq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_drv_dbg(moal_private * priv, struct iwreq *wrq)
{
    int data[4];
    int ret = 0;

    ENTER();

    if (!wrq->u.data.length) {
        data[0] = drvdbg;
        /* Return the current driver debug bit masks */
        if (copy_to_user(wrq->u.data.pointer, data, sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto drvdbgexit;
        }
        wrq->u.data.length = 1;
    } else if (wrq->u.data.length < 3) {
        /* Get the driver debug bit masks */
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto drvdbgexit;
        }
        drvdbg = data[0];
    } else {
        PRINTM(MERROR, "Invalid parameter number\n");
        goto drvdbgexit;
    }

    printk(KERN_ALERT "drvdbg = 0x%08lx\n", drvdbg);
#ifdef DEBUG_LEVEL2
    printk(KERN_ALERT "MINFO  (%08lx) %s\n", MINFO,
           (drvdbg & MINFO) ? "X" : "");
    printk(KERN_ALERT "MWARN  (%08lx) %s\n", MWARN,
           (drvdbg & MWARN) ? "X" : "");
    printk(KERN_ALERT "MENTRY (%08lx) %s\n", MENTRY,
           (drvdbg & MENTRY) ? "X" : "");
#endif
    printk(KERN_ALERT "MIF_D  (%08lx) %s\n", MIF_D,
           (drvdbg & MIF_D) ? "X" : "");
    printk(KERN_ALERT "MFW_D  (%08lx) %s\n", MFW_D,
           (drvdbg & MFW_D) ? "X" : "");
    printk(KERN_ALERT "MEVT_D (%08lx) %s\n", MEVT_D,
           (drvdbg & MEVT_D) ? "X" : "");
    printk(KERN_ALERT "MCMD_D (%08lx) %s\n", MCMD_D,
           (drvdbg & MCMD_D) ? "X" : "");
    printk(KERN_ALERT "MDAT_D (%08lx) %s\n", MDAT_D,
           (drvdbg & MDAT_D) ? "X" : "");
    printk(KERN_ALERT "MIOCTL (%08lx) %s\n", MIOCTL,
           (drvdbg & MIOCTL) ? "X" : "");
    printk(KERN_ALERT "MINTR  (%08lx) %s\n", MINTR,
           (drvdbg & MINTR) ? "X" : "");
    printk(KERN_ALERT "MEVENT (%08lx) %s\n", MEVENT,
           (drvdbg & MEVENT) ? "X" : "");
    printk(KERN_ALERT "MCMND  (%08lx) %s\n", MCMND,
           (drvdbg & MCMND) ? "X" : "");
    printk(KERN_ALERT "MDATA  (%08lx) %s\n", MDATA,
           (drvdbg & MDATA) ? "X" : "");
    printk(KERN_ALERT "MERROR (%08lx) %s\n", MERROR,
           (drvdbg & MERROR) ? "X" : "");
    printk(KERN_ALERT "MFATAL (%08lx) %s\n", MFATAL,
           (drvdbg & MFATAL) ? "X" : "");
    printk(KERN_ALERT "MMSG   (%08lx) %s\n", MMSG, (drvdbg & MMSG) ? "X" : "");

  drvdbgexit:
    LEAVE();
    return ret;
}
#endif /* DEBUG_LEVEL1 */

/**
 * @brief Set/Get QoS configuration
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_qos_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_wmm_cfg *cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg = (mlan_ds_wmm_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_WMM_CFG_QOS;
    req->req_id = MLAN_IOCTL_WMM_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        cfg->param.qos_cfg = (t_u8) data;
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = (int) cfg->param.qos_cfg;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get WWS mode
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return         0 --success, otherwise fail
 */
static int
woal_wws_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_misc_cfg *wws = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    wws = (mlan_ds_misc_cfg *) req->pbuf;
    wws->sub_command = MLAN_OID_MISC_WWS;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (data != CMD_DISABLED && data != CMD_ENABLED) {
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        wws->param.wws_cfg = data;
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = wws->param.wws_cfg;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get sleep period
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return         0 --success, otherwise fail
 */
static int
woal_sleep_pd(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_pm_cfg *pm_cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    pm_cfg = (mlan_ds_pm_cfg *) req->pbuf;
    pm_cfg->sub_command = MLAN_OID_PM_CFG_SLEEP_PD;
    req->req_id = MLAN_IOCTL_PM_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data <= MAX_SLEEP_PERIOD && data >= MIN_SLEEP_PERIOD) ||
            (data == 0)
            || (data == SLEEP_PERIOD_RESERVED_FF)
            ) {
            req->action = MLAN_ACT_SET;
            pm_cfg->param.sleep_period = data;
        } else {
            ret = -EINVAL;
            goto done;
        }
    } else
        req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = pm_cfg->param.sleep_period;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Configure sleep parameters
 *
 * @param priv         A pointer to moal_private structure
 * @param req          A pointer to ifreq structure
 *
 * @return             0 --success, otherwise fail
 */
static int
woal_sleep_params_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pm = NULL;
    mlan_ds_sleep_params *psleep_params = NULL;
    int data[6] = { 0 }, i;
#ifdef DEBUG_LEVEL1
    char err_str[][35] = { {"sleep clock error in ppm"},
    {"wakeup offset in usec"},
    {"clock stabilization time in usec"},
    {"value of reserved for debug"}
    };
#endif

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }

    pm = (mlan_ds_pm_cfg *) req->pbuf;
    pm->sub_command = MLAN_OID_PM_CFG_SLEEP_PARAMS;
    req->req_id = MLAN_IOCTL_PM_CFG;
    psleep_params = (pmlan_ds_sleep_params) & pm->param.sleep_params;

    if (wrq->u.data.length == 0) {
        req->action = MLAN_ACT_GET;
    } else if (wrq->u.data.length == 6) {
        if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) *
                           wrq->u.data.length)) {
            /* copy_from_user failed */
            PRINTM(MERROR, "S_PARAMS: copy from user failed\n");
            LEAVE();
            return -EINVAL;
        }
#define MIN_VAL 0x0000
#define MAX_VAL 0xFFFF
        for (i = 0; i < 6; i++) {
            if ((i == 3) || (i == 4)) {
                /* These two cases are handled below the loop */
                continue;
            }
            if (data[i] < MIN_VAL || data[i] > MAX_VAL) {
                PRINTM(MERROR, "Invalid %s (0-65535)!\n", err_str[i]);
                ret = -EINVAL;
                goto done;
            }
        }
        if (data[3] < 0 || data[3] > 2) {
            PRINTM(MERROR, "Invalid control periodic calibration (0-2)!\n");
            ret = -EINVAL;
            goto done;
        }
        if (data[4] < 0 || data[4] > 2) {
            PRINTM(MERROR, "Invalid control of external sleep clock (0-2)!\n");
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        psleep_params->error = data[0];
        psleep_params->offset = data[1];
        psleep_params->stable_time = data[2];
        psleep_params->cal_control = data[3];
        psleep_params->ext_sleep_clk = data[4];
        psleep_params->reserved = data[5];
    } else {
        ret = -EINVAL;
        goto done;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    data[0] = psleep_params->error;
    data[1] = psleep_params->offset;
    data[2] = psleep_params->stable_time;
    data[3] = psleep_params->cal_control;
    data[4] = psleep_params->ext_sleep_clk;
    data[5] = psleep_params->reserved;
    wrq->u.data.length = 6;

    if (copy_to_user(wrq->u.data.pointer, data, sizeof(int) *
                     wrq->u.data.length)) {
        PRINTM(MERROR, "QCONFIG: copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set/get user provisioned local power constraint
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_set_get_11h_local_pwr_constraint(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, data = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11h_cfg *ds_11hcfg = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11h_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    ds_11hcfg = (mlan_ds_11h_cfg *) req->pbuf;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MINFO, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        ds_11hcfg->param.usr_local_power_constraint = (t_s8) data;
        req->action = MLAN_ACT_SET;
    } else
        req->action = MLAN_ACT_GET;

    ds_11hcfg->sub_command = MLAN_OID_11H_LOCAL_POWER_CONSTRAINT;
    req->req_id = MLAN_IOCTL_11H_CFG;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    /* Copy response to user */
    if (req->action == MLAN_ACT_GET) {
        data = (int) ds_11hcfg->param.usr_local_power_constraint;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            PRINTM(MINFO, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/get HT stream configurations
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_ht_stream_cfg_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, data = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg = (mlan_ds_11n_cfg *) req->pbuf;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MINFO, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (data != HT_STREAM_MODE_1X1 && data != HT_STREAM_MODE_2X2) {
            PRINTM(MINFO, "Invalid argument\n");
            ret = -EINVAL;
            goto done;
        }
        cfg->param.stream_cfg = data;
        req->action = MLAN_ACT_SET;
    } else
        req->action = MLAN_ACT_GET;

    cfg->sub_command = MLAN_OID_11N_CFG_STREAM_CFG;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    /* Copy response to user */
    data = (int) cfg->param.stream_cfg;
    if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
        PRINTM(MINFO, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/get MAC control configuration
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_mac_control_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, data = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *cfg = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg = (mlan_ds_misc_cfg *) req->pbuf;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MINFO, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        /* Validation will be done later */
        cfg->param.mac_ctrl = data;
        req->action = MLAN_ACT_SET;
    } else
        req->action = MLAN_ACT_GET;

    cfg->sub_command = MLAN_OID_MISC_MAC_CONTROL;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    /* Copy response to user */
    data = (int) cfg->param.mac_ctrl;
    if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
        PRINTM(MINFO, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get thermal reading
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_thermal_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, data = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *cfg = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg = (mlan_ds_misc_cfg *) req->pbuf;
    if (wrq->u.data.length) {
        PRINTM(MERROR, "Set is not supported for this command\n");
        ret = -EINVAL;
        goto done;
    }
    req->action = MLAN_ACT_GET;

    cfg->sub_command = MLAN_OID_MISC_THERMAL;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    /* Copy response to user */
    data = (int) cfg->param.thermal;
    if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
        PRINTM(MINFO, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

#if defined(REASSOCIATION)
/**
 * @brief Set/Get reassociation settings
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_reassoc(moal_private * priv, struct iwreq *wrq)
{
    moal_handle *handle = priv->phandle;
    int ret = 0;
    int data = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (data == 0) {
            handle->reassoc_on &= ~MBIT(priv->bss_index);
            priv->reassoc_on = MFALSE;
            priv->reassoc_required = MFALSE;
            if (!handle->reassoc_on && handle->is_reassoc_timer_set == MTRUE) {
                woal_cancel_timer(&handle->reassoc_timer);
                handle->is_reassoc_timer_set = MFALSE;
            }
        } else {
            handle->reassoc_on |= MBIT(priv->bss_index);
            priv->reassoc_on = MTRUE;
        }
    } else {
        data = (int) (priv->reassoc_on);
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    LEAVE();
    return ret;
}
#endif /* REASSOCIATION */

/**
 *  @brief implement WMM enable command
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq      Pointer to user data
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_wmm_enable_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_wmm_cfg *wmm = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    wmm = (mlan_ds_wmm_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_WMM_CFG;
    wmm->sub_command = MLAN_OID_WMM_CFG_ENABLE;

    if (wrq->u.data.length) {
        /* Set WMM configuration */
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data < CMD_DISABLED) || (data > CMD_ENABLED)) {
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        if (data == CMD_DISABLED)
            wmm->param.wmm_enable = MFALSE;
        else
            wmm->param.wmm_enable = MTRUE;
    } else {
        /* Get WMM status */
        req->action = MLAN_ACT_GET;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.pointer) {
        if (copy_to_user
            (wrq->u.data.pointer, &wmm->param.wmm_enable,
             sizeof(wmm->param.wmm_enable))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Implement 802.11D enable command
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq      Pointer to user data
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_11d_enable_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_11d_cfg *pcfg_11d = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11d_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pcfg_11d = (mlan_ds_11d_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_11D_CFG;
    pcfg_11d->sub_command = MLAN_OID_11D_CFG_ENABLE;
    if (wrq->u.data.length) {
        /* Set 11D configuration */
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data < CMD_DISABLED) || (data > CMD_ENABLED)) {
            ret = -EINVAL;
            goto done;
        }
        if (data == CMD_DISABLED)
            pcfg_11d->param.enable_11d = MFALSE;
        else
            pcfg_11d->param.enable_11d = MTRUE;
        req->action = MLAN_ACT_SET;
    } else {
        req->action = MLAN_ACT_GET;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.pointer) {
        if (copy_to_user
            (wrq->u.data.pointer, &pcfg_11d->param.enable_11d,
             sizeof(pcfg_11d->param.enable_11d))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Implement 802.11D clear chan table command
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq      Pointer to user data
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_11d_clr_chan_table(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_11d_cfg *pcfg_11d = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11d_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pcfg_11d = (mlan_ds_11d_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_11D_CFG;
    pcfg_11d->sub_command = MLAN_OID_11D_CLR_CHAN_TABLE;
    req->action = MLAN_ACT_SET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Control WPS Session Enable/Disable
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq      Pointer to user data
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_wps_cfg_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_wps_cfg *pwps = NULL;
    mlan_ioctl_req *req = NULL;
    char buf[8];
    struct iwreq *wreq = (struct iwreq *) wrq;

    ENTER();

    PRINTM(MINFO, "WOAL_WPS_SESSION\n");

    memset(buf, 0, sizeof(buf));
    if (copy_from_user(buf, wreq->u.data.pointer,
                       MIN(sizeof(buf) - 1, wreq->u.data.length))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wps_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pwps = (mlan_ds_wps_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_WPS_CFG;
    req->action = MLAN_ACT_SET;
    pwps->sub_command = MLAN_OID_WPS_CFG_SESSION;
    if (buf[0] == 1)
        pwps->param.wps_session = MLAN_WPS_CFG_SESSION_START;
    else
        pwps->param.wps_session = MLAN_WPS_CFG_SESSION_END;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set WPA passphrase and SSID
 *
 *  @param priv	    A pointer to moal_private structure
 *  @param wrq	    A pointer to user data
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_passphrase(moal_private * priv, struct iwreq *wrq)
{
    t_u16 len = 0;
    static char buf[256];
    char *begin, *end, *opt;
    int ret = 0, action = -1, i;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;
    t_u8 zero_mac[] = { 0, 0, 0, 0, 0, 0 };
    t_u8 *mac = NULL;

    ENTER();

    if (!wrq->u.data.length || wrq->u.data.length >= sizeof(buf)) {
        PRINTM(MERROR, "Argument missing or too long for setpassphrase\n");
        ret = -EINVAL;
        goto done;
    }

    if (copy_from_user(buf, wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    buf[wrq->u.data.length] = '\0';

    /* Parse the buf to get the cmd_action */
    begin = buf;
    end = woal_strsep(&begin, ';', '/');
    if (end)
        action = woal_atox(end);
    if (action < 0 || action > 2 || end[1] != '\0') {
        PRINTM(MERROR, "Invalid action argument %s\n", end);
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    if (action == 0)
        req->action = MLAN_ACT_GET;
    else
        req->action = MLAN_ACT_SET;
    while (begin) {
        end = woal_strsep(&begin, ';', '/');
        opt = woal_strsep(&end, '=', '/');
        if (!opt || !end || !end[0]) {
            PRINTM(MERROR, "Invalid option\n");
            ret = -EINVAL;
            break;
        } else if (!strnicmp(opt, "ssid", strlen(opt))) {
            if (strlen(end) > MLAN_MAX_SSID_LENGTH) {
                PRINTM(MERROR, "SSID length exceeds max length\n");
                ret = -EFAULT;
                break;
            }
            sec->param.passphrase.ssid.ssid_len = strlen(end);
            strncpy((char *) sec->param.passphrase.ssid.ssid, end, strlen(end));
            PRINTM(MINFO, "ssid=%s, len=%d\n", sec->param.passphrase.ssid.ssid,
                   (int) sec->param.passphrase.ssid.ssid_len);
        } else if (!strnicmp(opt, "bssid", strlen(opt))) {
            woal_mac2u8((t_u8 *) & sec->param.passphrase.bssid, end);
        } else if (!strnicmp(opt, "psk", strlen(opt)) &&
                   req->action == MLAN_ACT_SET) {
            if (strlen(end) != MLAN_PMK_HEXSTR_LENGTH) {
                PRINTM(MERROR, "Invalid PMK length\n");
                ret = -EINVAL;
                break;
            }
            woal_ascii2hex((t_u8 *) (sec->param.passphrase.psk.pmk.pmk), end,
                           MLAN_PMK_HEXSTR_LENGTH / 2);
            sec->param.passphrase.psk_type = MLAN_PSK_PMK;
        } else if (!strnicmp(opt, "passphrase", strlen(opt)) &&
                   req->action == MLAN_ACT_SET) {
            if (strlen(end) < MLAN_MIN_PASSPHRASE_LENGTH ||
                strlen(end) > MLAN_MAX_PASSPHRASE_LENGTH) {
                PRINTM(MERROR, "Invalid length for passphrase\n");
                ret = -EINVAL;
                break;
            }
            sec->param.passphrase.psk_type = MLAN_PSK_PASSPHRASE;
            strncpy(sec->param.passphrase.psk.passphrase.passphrase, end,
                    sizeof(sec->param.passphrase.psk.passphrase.passphrase));
            sec->param.passphrase.psk.passphrase.passphrase_len = strlen(end);
            PRINTM(MINFO, "passphrase=%s, len=%d\n",
                   sec->param.passphrase.psk.passphrase.passphrase,
                   (int) sec->param.passphrase.psk.passphrase.passphrase_len);
        } else {
            PRINTM(MERROR, "Invalid option %s\n", opt);
            ret = -EINVAL;
            break;
        }
    }
    if (ret)
        goto done;

    if (action == 2)
        sec->param.passphrase.psk_type = MLAN_PSK_CLEAR;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (action == 0) {
        memset(buf, 0, sizeof(buf));
        if (sec->param.passphrase.ssid.ssid_len) {
            len += sprintf(buf + len, "ssid:");
            memcpy(buf + len, sec->param.passphrase.ssid.ssid,
                   sec->param.passphrase.ssid.ssid_len);
            len += sec->param.passphrase.ssid.ssid_len;
            len += sprintf(buf + len, " ");
        }
        if (memcmp(&sec->param.passphrase.bssid, zero_mac, sizeof(zero_mac))) {
            mac = (t_u8 *) & sec->param.passphrase.bssid;
            len += sprintf(buf + len, "bssid:");
            for (i = 0; i < ETH_ALEN - 1; ++i)
                len += sprintf(buf + len, "%02x:", mac[i]);
            len += sprintf(buf + len, "%02x ", mac[i]);
        }
        if (sec->param.passphrase.psk_type == MLAN_PSK_PMK) {
            len += sprintf(buf + len, "psk:");
            for (i = 0; i < MLAN_MAX_KEY_LENGTH; ++i)
                len +=
                    sprintf(buf + len, "%02x",
                            sec->param.passphrase.psk.pmk.pmk[i]);
            len += sprintf(buf + len, "\n");
        }
        if (sec->param.passphrase.psk_type == MLAN_PSK_PASSPHRASE) {
            len +=
                sprintf(buf + len, "passphrase:%s \n",
                        sec->param.passphrase.psk.passphrase.passphrase);
        }
        if (wrq->u.data.pointer) {
            if (copy_to_user(wrq->u.data.pointer, buf, MIN(len, sizeof(buf)))) {
                PRINTM(MERROR, "Copy to user failed, len %d\n", len);
                ret = -EFAULT;
                goto done;
            }
            wrq->u.data.length = len;
        }

    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get esupp mode
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_get_esupp_mode(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ESUPP_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & sec->param.esupp_mode,
         sizeof(int) * 3)) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 3;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/** AES key length */
#define AES_KEY_LEN 16
/**
 *  @brief Adhoc AES control
 *
 *  @param priv	    A pointer to moal_private structure
 *  @param wrq	    A pointer to user data
 *
 *  @return 	    0 --success, otherwise fail
 */
static int
woal_adhoc_aes_ioctl(moal_private * priv, struct iwreq *wrq)
{
    static char buf[256];
    int ret = 0, action = -1;
    unsigned int i;
    t_u8 key_ascii[32];
    t_u8 key_hex[16];
    t_u8 *tmp;
    mlan_bss_info bss_info;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;
    t_u8 bcast_addr[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    ENTER();

    memset(key_ascii, 0x00, sizeof(key_ascii));
    memset(key_hex, 0x00, sizeof(key_hex));

    /* Get current BSS information */
    memset(&bss_info, 0, sizeof(bss_info));
    woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);
    if (bss_info.bss_mode != MLAN_BSS_MODE_IBSS ||
        bss_info.media_connected == MTRUE) {
        PRINTM(MERROR, "STA is connected or not in IBSS mode.\n");
        ret = -EOPNOTSUPP;
        goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    if (wrq->u.data.length) {
        if (wrq->u.data.length >= sizeof(buf)) {
            PRINTM(MERROR, "Too many arguments\n");
            ret = -EINVAL;
            goto done;
        }
        if (copy_from_user(buf, wrq->u.data.pointer, wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        buf[wrq->u.data.length] = '\0';

        if (wrq->u.data.length == 1) {
            /* Get Adhoc AES Key */
            req->req_id = MLAN_IOCTL_SEC_CFG;
            req->action = MLAN_ACT_GET;
            sec = (mlan_ds_sec_cfg *) req->pbuf;
            sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;
            sec->param.encrypt_key.key_len = AES_KEY_LEN;
            sec->param.encrypt_key.key_index = MLAN_KEY_INDEX_UNICAST;
            if (MLAN_STATUS_SUCCESS !=
                woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
                ret = -EFAULT;
                goto done;
            }

            memcpy(key_hex, sec->param.encrypt_key.key_material,
                   sizeof(key_hex));
            HEXDUMP("Adhoc AES Key (HEX)", key_hex, sizeof(key_hex));

            wrq->u.data.length = sizeof(key_ascii) + 1;

            tmp = key_ascii;
            for (i = 0; i < sizeof(key_hex); i++)
                tmp += sprintf((char *) tmp, "%02x", key_hex[i]);
        } else if (wrq->u.data.length >= 2) {
            /* Parse the buf to get the cmd_action */
            action = woal_atox(&buf[0]);
            if (action < 1 || action > 2) {
                PRINTM(MERROR, "Invalid action argument %d\n", action);
                ret = -EINVAL;
                goto done;
            }

            req->req_id = MLAN_IOCTL_SEC_CFG;
            req->action = MLAN_ACT_SET;
            sec = (mlan_ds_sec_cfg *) req->pbuf;
            sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;

            if (action == 1) {
                /* Set Adhoc AES Key */
                memcpy(key_ascii, &buf[2], sizeof(key_ascii));
                woal_ascii2hex(key_hex, (char *) key_ascii, sizeof(key_hex));
                HEXDUMP("Adhoc AES Key (HEX)", key_hex, sizeof(key_hex));

                sec->param.encrypt_key.key_len = AES_KEY_LEN;
                sec->param.encrypt_key.key_index = MLAN_KEY_INDEX_UNICAST;
                sec->param.encrypt_key.key_flags =
                    KEY_FLAG_SET_TX_KEY | KEY_FLAG_GROUP_KEY;
                memcpy(sec->param.encrypt_key.mac_addr, (u8 *) bcast_addr,
                       ETH_ALEN);
                memcpy(sec->param.encrypt_key.key_material, key_hex,
                       sec->param.encrypt_key.key_len);

                if (MLAN_STATUS_SUCCESS !=
                    woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
                    ret = -EFAULT;
                    goto done;
                }
            } else if (action == 2) {
                /* Clear Adhoc AES Key */
                sec->param.encrypt_key.key_len = AES_KEY_LEN;
                sec->param.encrypt_key.key_index = MLAN_KEY_INDEX_UNICAST;
                sec->param.encrypt_key.key_flags = KEY_FLAG_REMOVE_KEY;
                memcpy(sec->param.encrypt_key.mac_addr, (u8 *) bcast_addr,
                       ETH_ALEN);
                memset(sec->param.encrypt_key.key_material, 0,
                       sizeof(sec->param.encrypt_key.key_material));

                if (MLAN_STATUS_SUCCESS !=
                    woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
                    ret = -EFAULT;
                    goto done;
                }
            } else {
                PRINTM(MERROR, "Invalid argument\n");
                ret = -EINVAL;
                goto done;
            }
        }

        HEXDUMP("Adhoc AES Key (ASCII)", key_ascii, sizeof(key_ascii));
        wrq->u.data.length = sizeof(key_ascii);
        if (wrq->u.data.pointer) {
            if (copy_to_user(wrq->u.data.pointer, &key_ascii,
                             sizeof(key_ascii))) {
                PRINTM(MERROR, "copy_to_user failed\n");
                ret = -EFAULT;
                goto done;
            }
        }
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief arpfilter ioctl function
 *
 *  @param priv		A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *  @return    		0 --success, otherwise fail
 */
static int
woal_arp_filter(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    misc->sub_command = MLAN_OID_MISC_GEN_IE;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    req->action = MLAN_ACT_SET;
    misc->param.gen_ie.type = MLAN_IE_TYPE_ARP_FILTER;
    misc->param.gen_ie.len = wrq->u.data.length;

    /* get the whole command from user */
    if (copy_from_user
        (misc->param.gen_ie.ie_data, wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(MERROR, "copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/get IP address
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq          A pointer to iwreq structure
 *  @return             0 --success, otherwise fail
 */
static int
woal_set_get_ip_addr(moal_private * priv, struct iwreq *wrq)
{
    char buf[IPADDR_MAX_BUF];
    struct iwreq *wreq = (struct iwreq *) wrq;
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_misc_cfg *misc = NULL;
    int ret = 0, op_code = 0, data_length = wrq->u.data.length;

    ENTER();

    memset(buf, 0, IPADDR_MAX_BUF);
    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) ioctl_req->pbuf;

    if (data_length <= 1) {     /* GET */
        ioctl_req->action = MLAN_ACT_GET;
    } else {
        if (copy_from_user(buf, wreq->u.data.pointer,
                           MIN(IPADDR_MAX_BUF - 1, wreq->u.data.length))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        /* Make sure we have the operation argument */
        if (data_length > 2 && buf[1] != ';') {
            PRINTM(MERROR, "No operation argument. Separate with ';'\n");
            ret = -EINVAL;
            goto done;
        } else {
            buf[1] = '\0';
        }
        ioctl_req->action = MLAN_ACT_SET;
        /* only one IP is supported in current firmware */
        memset(misc->param.ipaddr_cfg.ip_addr[0], 0, IPADDR_LEN);
        in4_pton(&buf[2], MIN((IPADDR_MAX_BUF - 3), (wreq->u.data.length - 2)),
                 misc->param.ipaddr_cfg.ip_addr[0], ' ', NULL);
        /* only one IP is supported in current firmware */
        misc->param.ipaddr_cfg.ip_addr_num = 1;
        misc->param.ipaddr_cfg.ip_addr_type = IPADDR_TYPE_IPV4;
    }
    if (woal_atoi(&op_code, &buf[0]) != MLAN_STATUS_SUCCESS) {
        ret = -EINVAL;
        goto done;
    }
    misc->param.ipaddr_cfg.op_code = (t_u32) op_code;
    ioctl_req->req_id = MLAN_IOCTL_MISC_CFG;
    misc->sub_command = MLAN_OID_MISC_IP_ADDR;

    /* Send ioctl to mlan */
    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (ioctl_req->action == MLAN_ACT_GET) {
        snprintf(buf, IPADDR_MAX_BUF, "%ld;%d.%d.%d.%d",
                 misc->param.ipaddr_cfg.op_code,
                 misc->param.ipaddr_cfg.ip_addr[0][0],
                 misc->param.ipaddr_cfg.ip_addr[0][1],
                 misc->param.ipaddr_cfg.ip_addr[0][2],
                 misc->param.ipaddr_cfg.ip_addr[0][3]);
        wrq->u.data.length = IPADDR_MAX_BUF;
        if (copy_to_user(wrq->u.data.pointer, buf, IPADDR_MAX_BUF)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
        }
    }

  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Transmit beamforming capabilities
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 -- success, otherwise fail
 */
static int
woal_tx_bf_cap_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, data_length = wrq->u.data.length;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *bf_cfg = NULL;
    int bf_cap = 0;

    ENTER();

    if (data_length > 1) {
        PRINTM(MERROR, "Invalid no of arguments!\n");
        ret = -EINVAL;
        goto done;
    }
    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    bf_cfg = (mlan_ds_11n_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_11N_CFG;
    bf_cfg->sub_command = MLAN_OID_11N_CFG_TX_BF_CAP;
    req->action = MLAN_ACT_GET;
    if (data_length) {          /* SET */
        if (copy_from_user(&bf_cap, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        bf_cfg->param.tx_bf_cap = bf_cap;
        req->action = MLAN_ACT_SET;
    }

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    bf_cap = bf_cfg->param.tx_bf_cap;
    if (copy_to_user(wrq->u.data.pointer, &bf_cap, sizeof(int))) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/* Maximum input output characters in group WOAL_SET_GET_256_CHAR */
#define MAX_IN_OUT_CHAR     256
/** Tx BF Global conf argument index */
#define BF_ENABLE_PARAM     1
#define SOUND_ENABLE_PARAM  2
#define FB_TYPE_PARAM       3
#define SNR_THRESHOLD_PARAM 4
#define SOUND_INTVL_PARAM   5
#define BF_MODE_PARAM       6
#define MAX_TX_BF_GLOBAL_ARGS   6
#define BF_CFG_ACT_GET      0
#define BF_CFG_ACT_SET      1

/**
 *  @brief Set/Get Transmit beamforming configuration
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 -- success, otherwise fail
 */
static int
woal_tx_bf_cfg_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, data_length = wrq->u.data.length;
    int bf_action = 0, interval = 0;
    int snr = 0, i, tmp_val;
    t_u8 buf[MAX_IN_OUT_CHAR], char_count = 0;
    t_u8 *str, *token, *pos;
    t_u16 action = 0;

    mlan_ds_11n_tx_bf_cfg bf_cfg;
    mlan_trigger_sound_args *bf_sound = NULL;
    mlan_tx_bf_peer_args *tx_bf_peer = NULL;
    mlan_snr_thr_args *bf_snr = NULL;
    mlan_bf_periodicity_args *bf_periodicity = NULL;
    mlan_bf_global_cfg_args *bf_global = NULL;

    ENTER();

    memset(&bf_cfg, 0, sizeof(bf_cfg));
    /* Pointer to corresponding buffer */
    bf_sound = bf_cfg.body.bf_sound;
    tx_bf_peer = bf_cfg.body.tx_bf_peer;
    bf_snr = bf_cfg.body.bf_snr;
    bf_periodicity = bf_cfg.body.bf_periodicity;
    bf_global = &bf_cfg.body.bf_global_cfg;

    /* Total characters in buffer */
    char_count = data_length - 1;
    memset(buf, 0, sizeof(buf));
    if (char_count) {
        if (data_length > sizeof(buf)) {
            PRINTM(MERROR, "Too many arguments\n");
            ret = -EINVAL;
            goto done;
        }
        if (copy_from_user(buf, wrq->u.data.pointer, data_length)) {
            PRINTM(MERROR, "copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        if (char_count > 1 && buf[1] != ';') {
            PRINTM(MERROR, "No action argument. Separate with ';'\n");
            ret = -EINVAL;
            goto done;
        }
        /* Replace ';' with NULL in the string to separate args */
        for (i = 0; i < char_count; i++) {
            if (buf[i] == ';')
                buf[i] = '\0';
        }
        /* The first byte represents the beamforming action */
        if (woal_atoi(&bf_action, &buf[0]) != MLAN_STATUS_SUCCESS) {
            ret = -EINVAL;
            goto done;
        }
        switch (bf_action) {
        case BF_GLOBAL_CONFIGURATION:
            if (char_count == 1) {
                action = MLAN_ACT_GET;
                bf_cfg.action = BF_CFG_ACT_GET;
            } else {
                action = MLAN_ACT_SET;
                bf_cfg.action = BF_CFG_ACT_SET;
                /* Eliminate action field */
                token = &buf[2];
                for (i = 1, str = &buf[2]; token != NULL; i++) {
                    token = strstr(str, " ");
                    pos = str;
                    if (token != NULL) {
                        *token = '\0';
                        str = token + 1;
                    }
                    woal_atoi(&tmp_val, pos);
                    switch (i) {
                    case BF_ENABLE_PARAM:
                        bf_global->bf_enbl = (t_u8) tmp_val;
                        break;
                    case SOUND_ENABLE_PARAM:
                        bf_global->sounding_enbl = (t_u8) tmp_val;
                        break;
                    case FB_TYPE_PARAM:
                        bf_global->fb_type = (t_u8) tmp_val;
                        break;
                    case SNR_THRESHOLD_PARAM:
                        bf_global->snr_threshold = (t_u8) tmp_val;
                        break;
                    case SOUND_INTVL_PARAM:
                        bf_global->sounding_interval = (t_u16) tmp_val;
                        break;
                    case BF_MODE_PARAM:
                        bf_global->bf_mode = (t_u8) tmp_val;
                        break;
                    default:
                        PRINTM(MERROR, "Invalid Argument\n");
                        ret = -EINVAL;
                        goto done;
                    }
                }
            }
            break;
        case TRIGGER_SOUNDING_FOR_PEER:
            /* First arg = 2 BfAction Second arg = 17 MAC "00:50:43:20:BF:64" */
            if (char_count != 19) {
                PRINTM(MERROR, "Invalid argument\n");
                ret = -EINVAL;
                goto done;
            }
            woal_mac2u8(bf_sound->peer_mac, &buf[2]);
            action = MLAN_ACT_SET;
            bf_cfg.action = BF_CFG_ACT_SET;
            break;
        case SET_GET_BF_PERIODICITY:
            /* First arg = 2 BfAction Second arg = 18 MAC "00:50:43:20:BF:64;"
               Third arg = 1 (min char) TX BF interval 10 (max char) u32
               maximum value 4294967295 */
            if (char_count < 19 || char_count > 30) {
                PRINTM(MERROR, "Invalid argument\n");
                ret = -EINVAL;
                goto done;
            }

            woal_mac2u8(bf_periodicity->peer_mac, &buf[2]);
            if (char_count == 19) {
                action = MLAN_ACT_GET;
                bf_cfg.action = BF_CFG_ACT_GET;
            } else {
                action = MLAN_ACT_SET;
                bf_cfg.action = BF_CFG_ACT_SET;
                if (woal_atoi(&interval, &buf[20]) != MLAN_STATUS_SUCCESS) {
                    ret = -EINVAL;
                    goto done;
                }
                bf_periodicity->interval = interval;
            }
            break;
        case TX_BF_FOR_PEER_ENBL:
            /* Handle only SET operation here First arg = 2 BfAction Second arg
               = 18 MAC "00:50:43:20:BF:64;" Third arg = 2 enable/disable bf
               Fourth arg = 2 enable/disable sounding Fifth arg = 1 FB Type */
            if (char_count != 25 && char_count != 1) {
                PRINTM(MERROR, "Invalid argument\n");
                ret = -EINVAL;
                goto done;
            }
            if (char_count == 1) {
                action = MLAN_ACT_GET;
                bf_cfg.action = BF_CFG_ACT_GET;
            } else {
                woal_mac2u8(tx_bf_peer->peer_mac, &buf[2]);
                woal_atoi(&tmp_val, &buf[20]);
                tx_bf_peer->bf_enbl = (t_u8) tmp_val;
                woal_atoi(&tmp_val, &buf[22]);
                tx_bf_peer->sounding_enbl = (t_u8) tmp_val;
                woal_atoi(&tmp_val, &buf[24]);
                tx_bf_peer->fb_type = (t_u8) tmp_val;
                action = MLAN_ACT_SET;
                bf_cfg.action = BF_CFG_ACT_SET;
            }
            break;
        case SET_SNR_THR_PEER:
            /* First arg = 2 BfAction Second arg = 18 MAC "00:50:43:20:BF:64;"
               Third arg = 1/2 SNR u8 - can be 1/2 charerters */
            if (char_count != 1 && !(char_count == 21 || char_count == 22)) {
                PRINTM(MERROR, "Invalid argument\n");
                ret = -EINVAL;
                goto done;
            }
            if (char_count == 1) {
                action = MLAN_ACT_GET;
                bf_cfg.action = BF_CFG_ACT_GET;
            } else {
                woal_mac2u8(bf_snr->peer_mac, &buf[2]);
                if (woal_atoi(&snr, &buf[20]) != MLAN_STATUS_SUCCESS) {
                    ret = -EINVAL;
                    goto done;
                }
                bf_snr->snr = snr;
                action = MLAN_ACT_SET;
                bf_cfg.action = BF_CFG_ACT_SET;
            }
            break;
        default:
            ret = -EINVAL;
            goto done;
        }

        /* Save the value */
        bf_cfg.bf_action = bf_action;
        if (MLAN_STATUS_SUCCESS !=
            woal_set_get_tx_bf_cfg(priv, action, &bf_cfg)) {
            ret = -EFAULT;
            goto done;
        }
    } else {
        ret = -EINVAL;
        goto done;
    }

    if (action == MLAN_ACT_GET) {
        data_length = 0;
        memset(buf, 0, sizeof(buf));
        switch (bf_action) {
        case BF_GLOBAL_CONFIGURATION:
            data_length +=
                sprintf(buf + data_length, "%d ", (int) bf_global->bf_enbl);
            data_length +=
                sprintf(buf + data_length, "%d ",
                        (int) bf_global->sounding_enbl);
            data_length +=
                sprintf(buf + data_length, "%d ", (int) bf_global->fb_type);
            data_length +=
                sprintf(buf + data_length, "%d ",
                        (int) bf_global->snr_threshold);
            data_length +=
                sprintf(buf + data_length, "%d ",
                        (int) bf_global->sounding_interval);
            data_length +=
                sprintf(buf + data_length, "%d ", (int) bf_global->bf_mode);
            break;
        case TRIGGER_SOUNDING_FOR_PEER:
            data_length += sprintf(buf + data_length,
                                   "%02x:%02x:%02x:%02x:%02x:%02x",
                                   bf_sound->peer_mac[0], bf_sound->peer_mac[1],
                                   bf_sound->peer_mac[2], bf_sound->peer_mac[3],
                                   bf_sound->peer_mac[4],
                                   bf_sound->peer_mac[5]);
            data_length += sprintf(buf + data_length, "%c", ';');
            data_length += sprintf(buf + data_length, "%d", bf_sound->status);
            break;
        case SET_GET_BF_PERIODICITY:
            data_length += sprintf(buf + data_length,
                                   "%02x:%02x:%02x:%02x:%02x:%02x",
                                   bf_periodicity->peer_mac[0],
                                   bf_periodicity->peer_mac[1],
                                   bf_periodicity->peer_mac[2],
                                   bf_periodicity->peer_mac[3],
                                   bf_periodicity->peer_mac[4],
                                   bf_periodicity->peer_mac[5]);
            data_length += sprintf(buf + data_length, "%c", ' ');
            data_length +=
                sprintf(buf + data_length, "%d", bf_periodicity->interval);
            break;
        case TX_BF_FOR_PEER_ENBL:
            for (i = 0; i < bf_cfg.no_of_peers; i++) {
                data_length += sprintf(buf + data_length,
                                       "%02x:%02x:%02x:%02x:%02x:%02x",
                                       tx_bf_peer->peer_mac[0],
                                       tx_bf_peer->peer_mac[1],
                                       tx_bf_peer->peer_mac[2],
                                       tx_bf_peer->peer_mac[3],
                                       tx_bf_peer->peer_mac[4],
                                       tx_bf_peer->peer_mac[5]);
                data_length += sprintf(buf + data_length, "%c", ' ');
                data_length +=
                    sprintf(buf + data_length, "%d;", tx_bf_peer->bf_enbl);
                data_length +=
                    sprintf(buf + data_length, "%d;",
                            tx_bf_peer->sounding_enbl);
                data_length +=
                    sprintf(buf + data_length, "%d ", tx_bf_peer->fb_type);
                tx_bf_peer++;
            }
            break;
        case SET_SNR_THR_PEER:
            for (i = 0; i < bf_cfg.no_of_peers; i++) {
                data_length += sprintf(buf + data_length,
                                       "%02x:%02x:%02x:%02x:%02x:%02x",
                                       bf_snr->peer_mac[0], bf_snr->peer_mac[1],
                                       bf_snr->peer_mac[2], bf_snr->peer_mac[3],
                                       bf_snr->peer_mac[4],
                                       bf_snr->peer_mac[5]);
                data_length += sprintf(buf + data_length, "%c", ';');
                data_length += sprintf(buf + data_length, "%d", bf_snr->snr);
                data_length += sprintf(buf + data_length, "%c", ' ');
                bf_snr++;
            }
            break;
        default:
            ret = -EINVAL;
            goto done;
        }
        buf[data_length] = '\0';
    }

    wrq->u.data.length = data_length;
    if (copy_to_user(wrq->u.data.pointer, buf, wrq->u.data.length)) {
        ret = -EFAULT;
        goto done;
    }

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Create a brief scan resp to relay basic BSS info to the app layer
 *
 *  When the beacon/probe response has not been buffered, use the saved BSS
 *    information available to provide a minimum response for the application
 *    ioctl retrieval routines.  Include:
 *        - Timestamp
 *        - Beacon Period
 *        - Capabilities (including WMM Element if available)
 *        - SSID
 *
 *  @param ppbuffer  Output parameter: Buffer used to create basic scan rsp
 *  @param pbss_desc Pointer to a BSS entry in the scan table to create
 *                   scan response from for delivery to the application layer
 *
 *  @return          N/A
 */
static void
wlan_scan_create_brief_table_entry(t_u8 ** ppbuffer,
                                   BSSDescriptor_t * pbss_desc)
{
    t_u8 *ptmp_buf = *ppbuffer;
    t_u8 tmp_ssid_hdr[2];
    t_u8 ie_len = 0;

    ENTER();

    if (copy_to_user(ptmp_buf, pbss_desc->time_stamp,
                     sizeof(pbss_desc->time_stamp))) {
        PRINTM(MINFO, "Copy to user failed\n");
        LEAVE();
        return;
    }
    ptmp_buf += sizeof(pbss_desc->time_stamp);

    if (copy_to_user(ptmp_buf, &pbss_desc->beacon_period,
                     sizeof(pbss_desc->beacon_period))) {
        PRINTM(MINFO, "Copy to user failed\n");
        LEAVE();
        return;
    }
    ptmp_buf += sizeof(pbss_desc->beacon_period);

    if (copy_to_user
        (ptmp_buf, &pbss_desc->cap_info, sizeof(pbss_desc->cap_info))) {
        PRINTM(MINFO, "Copy to user failed\n");
        LEAVE();
        return;
    }
    ptmp_buf += sizeof(pbss_desc->cap_info);

    tmp_ssid_hdr[0] = 0;        /* Element ID for SSID is zero */
    tmp_ssid_hdr[1] = pbss_desc->ssid.ssid_len;
    if (copy_to_user(ptmp_buf, tmp_ssid_hdr, sizeof(tmp_ssid_hdr))) {
        PRINTM(MINFO, "Copy to user failed\n");
        LEAVE();
        return;
    }
    ptmp_buf += sizeof(tmp_ssid_hdr);

    if (copy_to_user(ptmp_buf, pbss_desc->ssid.ssid, pbss_desc->ssid.ssid_len)) {
        PRINTM(MINFO, "Copy to user failed\n");
        LEAVE();
        return;
    }
    ptmp_buf += pbss_desc->ssid.ssid_len;

    if (pbss_desc->wmm_ie.vend_hdr.element_id == WMM_IE) {
        ie_len = sizeof(IEEEtypes_Header_t) + pbss_desc->wmm_ie.vend_hdr.len;
        if (copy_to_user(ptmp_buf, &pbss_desc->wmm_ie, ie_len)) {
            PRINTM(MINFO, "Copy to user failed\n");
            LEAVE();
            return;
        }

        ptmp_buf += ie_len;
    }

    if (pbss_desc->pwpa_ie) {
        if ((*(pbss_desc->pwpa_ie)).vend_hdr.element_id == WPA_IE) {
            ie_len =
                sizeof(IEEEtypes_Header_t) +
                (*(pbss_desc->pwpa_ie)).vend_hdr.len;
            if (copy_to_user(ptmp_buf, pbss_desc->pwpa_ie, ie_len)) {
                PRINTM(MINFO, "Copy to user failed\n");
                LEAVE();
                return;
            }
        }

        ptmp_buf += ie_len;
    }

    if (pbss_desc->prsn_ie) {
        if ((*(pbss_desc->prsn_ie)).ieee_hdr.element_id == RSN_IE) {
            ie_len =
                sizeof(IEEEtypes_Header_t) +
                (*(pbss_desc->prsn_ie)).ieee_hdr.len;
            if (copy_to_user(ptmp_buf, pbss_desc->prsn_ie, ie_len)) {
                PRINTM(MINFO, "Copy to user failed\n");
                LEAVE();
                return;
            }
        }

        ptmp_buf += ie_len;
    }

    *ppbuffer = ptmp_buf;
    LEAVE();
}

/**
 *  @brief Create a wlan_ioctl_get_scan_table_entry for a given BSS
 *         Descriptor for inclusion in the ioctl response to the user space
 *         application.
 *
 *
 *  @param pbss_desc   Pointer to a BSS entry in the scan table to form
 *                     scan response from for delivery to the application layer
 *  @param ppbuffer    Output parameter: Buffer used to output scan return struct
 *  @param pspace_left Output parameter: Number of bytes available in the
 *                     response buffer.
 *
 *  @return MLAN_STATUS_SUCCESS, or < 0 with IOCTL error code
 */
static int
wlan_get_scan_table_ret_entry(BSSDescriptor_t * pbss_desc,
                              t_u8 ** ppbuffer, int *pspace_left)
{
    wlan_ioctl_get_scan_table_entry *prsp_entry;
    wlan_ioctl_get_scan_table_entry tmp_rsp_entry;
    int space_needed;
    t_u8 *pcurrent;
    int variable_size;

    const int fixed_size = sizeof(wlan_ioctl_get_scan_table_entry);

    ENTER();

    pcurrent = *ppbuffer;

    /* The variable size returned is the stored beacon size */
    variable_size = pbss_desc->beacon_buf_size;

    /* If we stored a beacon and its size was zero, set the variable size
       return value to the size of the brief scan response
       wlan_scan_create_brief_table_entry creates.  Also used if we are not
       configured to store beacons in the first place */
    if (!variable_size) {
        variable_size = pbss_desc->ssid.ssid_len + 2;
        variable_size += (sizeof(pbss_desc->beacon_period)
                          + sizeof(pbss_desc->time_stamp)
                          + sizeof(pbss_desc->cap_info));
        if (pbss_desc->wmm_ie.vend_hdr.element_id == WMM_IE) {
            variable_size += (sizeof(IEEEtypes_Header_t)
                              + pbss_desc->wmm_ie.vend_hdr.len);
        }

        if (pbss_desc->pwpa_ie) {
            if ((*(pbss_desc->pwpa_ie)).vend_hdr.element_id == WPA_IE) {
                variable_size += (sizeof(IEEEtypes_Header_t)
                                  + (*(pbss_desc->pwpa_ie)).vend_hdr.len);
            }
        }

        if (pbss_desc->prsn_ie) {
            if ((*(pbss_desc->prsn_ie)).ieee_hdr.element_id == RSN_IE) {
                variable_size += (sizeof(IEEEtypes_Header_t)
                                  + (*(pbss_desc->prsn_ie)).ieee_hdr.len);
            }
        }
    }

    space_needed = fixed_size + variable_size;

    PRINTM(MINFO, "GetScanTable: need(%d), left(%d)\n",
           space_needed, *pspace_left);

    if (space_needed >= *pspace_left) {
        *pspace_left = 0;
        LEAVE();
        return -E2BIG;
    }

    *pspace_left -= space_needed;

    tmp_rsp_entry.fixed_field_length = (sizeof(tmp_rsp_entry)
                                        -
                                        sizeof(tmp_rsp_entry.fixed_field_length)
                                        -
                                        sizeof(tmp_rsp_entry.bss_info_length));

    memcpy(tmp_rsp_entry.fixed_fields.bssid,
           pbss_desc->mac_address, sizeof(prsp_entry->fixed_fields.bssid));

    tmp_rsp_entry.fixed_fields.rssi = pbss_desc->rssi;
    tmp_rsp_entry.fixed_fields.channel = pbss_desc->channel;
    tmp_rsp_entry.fixed_fields.network_tsf = pbss_desc->network_tsf;
    tmp_rsp_entry.bss_info_length = variable_size;

    /*
     *  Copy fixed fields to user space
     */
    if (copy_to_user(pcurrent, &tmp_rsp_entry, fixed_size)) {
        PRINTM(MINFO, "Copy to user failed\n");
        LEAVE();
        return -EFAULT;
    }

    pcurrent += fixed_size;

    if (pbss_desc->pbeacon_buf) {
        /*
         *  Copy variable length elements to user space
         */
        if (copy_to_user(pcurrent, pbss_desc->pbeacon_buf,
                         pbss_desc->beacon_buf_size)) {
            PRINTM(MINFO, "Copy to user failed\n");
            LEAVE();
            return -EFAULT;
        }

        pcurrent += pbss_desc->beacon_buf_size;
    } else {
        wlan_scan_create_brief_table_entry(&pcurrent, pbss_desc);
    }

    *ppbuffer = pcurrent;

    LEAVE();

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Retrieve the scan response/beacon table
 *
 *  @param wrq          A pointer to iwreq structure
 *  @param scan_resp    A pointer to mlan_scan_resp structure
 *  @param scan_start   argument
 *
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static int
moal_ret_get_scan_table_ioctl(struct iwreq *wrq,
                              mlan_scan_resp * scan_resp, t_u32 scan_start)
{
    pBSSDescriptor_t pbss_desc, scan_table;
    wlan_ioctl_get_scan_table_info *prsp_info;
    int ret_code;
    int ret_len;
    int space_left;
    t_u8 *pcurrent;
    t_u8 *pbuffer_end;
    t_u32 num_scans_done;

    ENTER();

    num_scans_done = 0;
    ret_code = MLAN_STATUS_SUCCESS;

    prsp_info = (wlan_ioctl_get_scan_table_info *) wrq->u.data.pointer;
    pcurrent = (t_u8 *) prsp_info->scan_table_entry_buf;

    pbuffer_end = wrq->u.data.pointer + wrq->u.data.length - 1;
    space_left = pbuffer_end - pcurrent;
    scan_table = (BSSDescriptor_t *) (scan_resp->pscan_table);

    PRINTM(MINFO, "GetScanTable: scan_start req = %ld\n", scan_start);
    PRINTM(MINFO, "GetScanTable: length avail = %d\n", wrq->u.data.length);

    if (!scan_start) {
        PRINTM(MINFO, "GetScanTable: get current BSS Descriptor\n");

        /* Use to get current association saved descriptor */
        pbss_desc = scan_table;

        ret_code = wlan_get_scan_table_ret_entry(pbss_desc,
                                                 &pcurrent, &space_left);

        if (ret_code == MLAN_STATUS_SUCCESS) {
            num_scans_done = 1;
        }
    } else {
        scan_start--;

        while (space_left
               && (scan_start + num_scans_done < scan_resp->num_in_scan_table)
               && (ret_code == MLAN_STATUS_SUCCESS)) {

            pbss_desc = (scan_table + (scan_start + num_scans_done));

            PRINTM(MINFO, "GetScanTable: get current BSS Descriptor [%ld]\n",
                   scan_start + num_scans_done);

            ret_code = wlan_get_scan_table_ret_entry(pbss_desc,
                                                     &pcurrent, &space_left);

            if (ret_code == MLAN_STATUS_SUCCESS) {
                num_scans_done++;
            }
        }
    }

    prsp_info->scan_number = num_scans_done;
    ret_len = pcurrent - (t_u8 *) wrq->u.data.pointer;

    wrq->u.data.length = ret_len;

    /* Return ret_code (EFAULT or E2BIG) in the case where no scan results were
       successfully encoded. */
    LEAVE();
    return (num_scans_done ? MLAN_STATUS_SUCCESS : ret_code);
}

/**
 *  @brief Get scan table ioctl
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *
 *  @return         MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING -- success, otherwise fail
 */
static mlan_status
woal_get_scan_table_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_scan *scan = NULL;
    t_u32 scan_start;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    scan = (mlan_ds_scan *) req->pbuf;
    req->req_id = MLAN_IOCTL_SCAN;
    req->action = MLAN_ACT_GET;

    /* get the whole command from user */
    if (copy_from_user(&scan_start, wrq->u.data.pointer, sizeof(scan_start))) {
        PRINTM(MERROR, "copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    if (scan_start) {
        scan->sub_command = MLAN_OID_SCAN_NORMAL;
    } else {
        scan->sub_command = MLAN_OID_SCAN_GET_CURRENT_BSS;
    }
    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        status = moal_ret_get_scan_table_ioctl(wrq,
                                               &scan->param.scan_resp,
                                               scan_start);
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/**
 *  @brief Set user scan
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *
 *  @return         MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING -- success, otherwise fail
 */
static mlan_status
woal_set_user_scan_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_scan *scan = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    union iwreq_data wrqu;
    moal_handle *handle = priv->phandle;

    ENTER();

    if (handle->scan_pending_on_block == MTRUE) {
        PRINTM(MINFO, "scan already in processing...\n");
        LEAVE();
        return ret;
    }
    if (MOAL_ACQ_SEMAPHORE_BLOCK(&handle->async_sem)) {
        PRINTM(MERROR, "Acquire semaphore error, request_scan\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    handle->scan_pending_on_block = MTRUE;

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan) + wrq->u.data.length);
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    scan = (mlan_ds_scan *) req->pbuf;
    scan->sub_command = MLAN_OID_SCAN_USER_CONFIG;
    req->req_id = MLAN_IOCTL_SCAN;
    req->action = MLAN_ACT_SET;

    if (copy_from_user(scan->param.user_scan.scan_cfg_buf,
                       wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(MINFO, "Copy from user failed\n");
        LEAVE();
        return -EFAULT;
    }

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        memset(&wrqu, 0, sizeof(union iwreq_data));
        wireless_send_event(priv->netdev, SIOCGIWSCAN, &wrqu, NULL);
    }
    handle->scan_pending_on_block = MFALSE;
    MOAL_REL_SEMAPHORE(&handle->async_sem);

  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/**
 * @brief Set/Get scan configuration parameters
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_scan_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    int arg_len = 7;
    int data[arg_len];
    mlan_ds_scan *scan = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    if (wrq->u.data.length > arg_len) {
        ret = -EINVAL;
        goto done;
    }
    scan = (mlan_ds_scan *) req->pbuf;
    scan->sub_command = MLAN_OID_SCAN_CONFIG;
    req->req_id = MLAN_IOCTL_SCAN;
    memset(data, 0, sizeof(data));
    if (wrq->u.data.length) {
        if (copy_from_user
            (data, wrq->u.data.pointer, (wrq->u.data.length * sizeof(int)))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data[0] < 0) || (data[0] > MLAN_SCAN_TYPE_PASSIVE)) {
            PRINTM(MERROR, "Invalid argument for scan type\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[1] < 0) || (data[1] > MLAN_SCAN_MODE_ANY)) {
            PRINTM(MERROR, "Invalid argument for scan mode\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[2] < 0) || (data[2] > MAX_PROBES)) {
            PRINTM(MERROR, "Invalid argument for scan probes\n");
            ret = -EINVAL;
            goto done;
        }
        if (((data[3] < 0) || (data[3] > MRVDRV_MAX_ACTIVE_SCAN_CHAN_TIME)) ||
            ((data[4] < 0) || (data[4] > MRVDRV_MAX_ACTIVE_SCAN_CHAN_TIME)) ||
            ((data[5] < 0) || (data[5] > MRVDRV_MAX_PASSIVE_SCAN_CHAN_TIME))) {
            PRINTM(MERROR, "Invalid argument for scan time\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[6] < 0) || (data[6] > 1)) {
            PRINTM(MERROR, "Invalid argument for extended scan\n");
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        memcpy(&scan->param.scan_cfg, data, sizeof(data));
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        memcpy(data, &scan->param.scan_cfg, sizeof(data));
        if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = sizeof(data) / sizeof(int);
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get PS configuration parameters
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_ps_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[7], ret = 0;
    mlan_ds_pm_cfg *pm_cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int allowed = 3;
    int i = 3;

    ENTER();

    allowed++;                  /* For ad-hoc awake period parameter */
    allowed++;                  /* For beacon missing timeout parameter */
    allowed += 2;               /* For delay to PS and PS mode parameters */

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    if (wrq->u.data.length > allowed) {
        ret = -EINVAL;
        goto done;
    }
    pm_cfg = (mlan_ds_pm_cfg *) req->pbuf;
    pm_cfg->sub_command = MLAN_OID_PM_CFG_PS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;
    memset(data, 0, sizeof(data));
    if (wrq->u.data.length) {
        if (copy_from_user
            (data, wrq->u.data.pointer, (wrq->u.data.length * sizeof(int)))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data[0] < PS_NULL_DISABLE)) {
            PRINTM(MERROR, "Invalid argument for PS null interval\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[1] != MRVDRV_IGNORE_MULTIPLE_DTIM)
            && (data[1] != MRVDRV_MATCH_CLOSEST_DTIM)
            && ((data[1] < MRVDRV_MIN_MULTIPLE_DTIM)
                || (data[1] > MRVDRV_MAX_MULTIPLE_DTIM))) {
            PRINTM(MERROR, "Invalid argument for multiple DTIM\n");
            ret = -EINVAL;
            goto done;
        }

        if ((data[2] < MRVDRV_MIN_LISTEN_INTERVAL)
            && (data[2] != MRVDRV_LISTEN_INTERVAL_DISABLE)) {
            PRINTM(MERROR, "Invalid argument for listen interval\n");
            ret = -EINVAL;
            goto done;
        }

        if ((data[i] != SPECIAL_ADHOC_AWAKE_PD) &&
            ((data[i] < MIN_ADHOC_AWAKE_PD) ||
             (data[i] > MAX_ADHOC_AWAKE_PD))) {
            PRINTM(MERROR, "Invalid argument for adhoc awake period\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        if ((data[i] != DISABLE_BCN_MISS_TO) &&
            ((data[i] < MIN_BCN_MISS_TO) || (data[i] > MAX_BCN_MISS_TO))) {
            PRINTM(MERROR, "Invalid argument for beacon miss timeout\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        if (wrq->u.data.length < allowed - 1)
            data[i] = DELAY_TO_PS_UNCHANGED;
        else if ((data[i] < MIN_DELAY_TO_PS) || (data[i] > MAX_DELAY_TO_PS)) {
            PRINTM(MERROR, "Invalid argument for delay to PS\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        if ((data[i] != PS_MODE_UNCHANGED) && (data[i] != PS_MODE_AUTO) &&
            (data[i] != PS_MODE_POLL) && (data[i] != PS_MODE_NULL)) {
            PRINTM(MERROR, "Invalid argument for PS mode\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        req->action = MLAN_ACT_SET;
        memcpy(&pm_cfg->param.ps_cfg, data,
               MIN(sizeof(pm_cfg->param.ps_cfg), sizeof(data)));
    } else
        req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    memcpy(data, &pm_cfg->param.ps_cfg,
           MIN((sizeof(int) * allowed), sizeof(pm_cfg->param.ps_cfg)));
    if (copy_to_user(wrq->u.data.pointer, data, sizeof(int) * allowed)) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = allowed;

    if (req->action == MLAN_ACT_SET) {
        pm_cfg = (mlan_ds_pm_cfg *) req->pbuf;
        pm_cfg->sub_command = MLAN_OID_PM_CFG_IEEE_PS;
        pm_cfg->param.ps_mode = 1;
        req->req_id = MLAN_IOCTL_PM_CFG;
        req->action = MLAN_ACT_SET;
        woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to send an ADDTS TSPEC
 *
 *  Receive a ADDTS command from the application.  The command structure
 *    contains a TSPEC and timeout in milliseconds.  The timeout is performed
 *    in the firmware after the ADDTS command frame is sent.
 *
 *  The TSPEC is received in the API as an opaque block. The firmware will
 *    send the entire data block, including the bytes after the TSPEC.  This
 *    is done to allow extra IEs to be packaged with the TSPEC in the ADDTS
 *    action frame.
 *
 *  The IOCTL structure contains two return fields:
 *    - The firmware command result, which indicates failure and timeouts
 *    - The IEEE Status code which contains the corresponding value from
 *      any ADDTS response frame received.
 *
 *  In addition, the opaque TSPEC data block passed in is replaced with the
 *    TSPEC received in the ADDTS response frame.  In case of failure, the
 *    AP may modify the TSPEC on return and in the case of success, the
 *    medium time is returned as calculated by the AP.  Along with the TSPEC,
 *    any IEs that are sent in the ADDTS response are also returned and can be
 *    parsed using the IOCTL length as an indicator of extra elements.
 *
 *  The return value to the application layer indicates a driver execution
 *    success or failure.  A successful return could still indicate a firmware
 *    failure or AP negotiation failure via the commandResult field copied
 *    back to the application.
 *
 *  @param priv    Pointer to the mlan_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_addts_req_t struct for this ADDTS request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_addts_req_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *cfg = NULL;
    wlan_ioctl_wmm_addts_req_t addts_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    cfg = (mlan_ds_wmm_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_WMM_CFG_ADDTS;

    memset(&addts_ioctl, 0x00, sizeof(addts_ioctl));

    if (wrq->u.data.length) {
        if (copy_from_user(&addts_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(addts_ioctl)))) {
            PRINTM(MERROR, "TSPEC: ADDTS copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg->param.addts.timeout = addts_ioctl.timeout_ms;
        cfg->param.addts.ie_data_len = (t_u8) addts_ioctl.ie_data_len;

        memcpy(cfg->param.addts.ie_data,
               addts_ioctl.ie_data, cfg->param.addts.ie_data_len);

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        addts_ioctl.cmd_result = cfg->param.addts.result;
        addts_ioctl.ieee_status_code = (t_u8) cfg->param.addts.status_code;
        addts_ioctl.ie_data_len = cfg->param.addts.ie_data_len;

        memcpy(addts_ioctl.ie_data,
               cfg->param.addts.ie_data, cfg->param.addts.ie_data_len);

        wrq->u.data.length = (sizeof(addts_ioctl)
                              - sizeof(addts_ioctl.ie_data)
                              + cfg->param.addts.ie_data_len);

        if (copy_to_user(wrq->u.data.pointer, &addts_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "TSPEC: ADDTS copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to send a DELTS TSPEC
 *
 *  Receive a DELTS command from the application.  The command structure
 *    contains a TSPEC and reason code along with space for a command result
 *    to be returned.  The information is packaged is sent to the wlan_cmd.c
 *    firmware command prep and send routines for execution in the firmware.
 *
 *  The reason code is not used for WMM implementations but is indicated in
 *    the 802.11e specification.
 *
 *  The return value to the application layer indicates a driver execution
 *    success or failure.  A successful return could still indicate a firmware
 *    failure via the cmd_result field copied back to the application.
 *
 *  @param priv    Pointer to the mlan_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_delts_req_t struct for this DELTS request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_delts_req_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *cfg = NULL;
    wlan_ioctl_wmm_delts_req_t delts_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    cfg = (mlan_ds_wmm_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_WMM_CFG_DELTS;

    memset(&delts_ioctl, 0x00, sizeof(delts_ioctl));

    if (wrq->u.data.length) {
        if (copy_from_user(&delts_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(delts_ioctl)))) {
            PRINTM(MERROR, "TSPEC: DELTS copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg->param.delts.status_code = (t_u32) delts_ioctl.ieee_reason_code;
        cfg->param.delts.ie_data_len = (t_u8) delts_ioctl.ie_data_len;

        memcpy(cfg->param.delts.ie_data,
               delts_ioctl.ie_data, cfg->param.delts.ie_data_len);

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        /* Return the firmware command result back to the application layer */
        delts_ioctl.cmd_result = cfg->param.delts.result;
        wrq->u.data.length = sizeof(delts_ioctl);

        if (copy_to_user(wrq->u.data.pointer, &delts_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "TSPEC: DELTS copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get/set a specified AC Queue's parameters
 *
 *  Receive a AC Queue configuration command which is used to get, set, or
 *    default the parameters associated with a specific WMM AC Queue.
 *
 *  @param priv    Pointer to the mlan_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_queue_config_t struct
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_queue_config_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    mlan_ds_wmm_queue_config *pqcfg = NULL;
    wlan_ioctl_wmm_queue_config_t qcfg_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_QUEUE_CONFIG;

    memset(&qcfg_ioctl, 0x00, sizeof(qcfg_ioctl));
    pqcfg = (mlan_ds_wmm_queue_config *) & pwmm->param.q_cfg;

    if (wrq->u.data.length) {
        if (copy_from_user(&qcfg_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(qcfg_ioctl)))) {
            PRINTM(MERROR, "QCONFIG: copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        pqcfg->action = qcfg_ioctl.action;
        pqcfg->access_category = qcfg_ioctl.access_category;
        pqcfg->msdu_lifetime_expiry = qcfg_ioctl.msdu_lifetime_expiry;

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        memset(&qcfg_ioctl, 0x00, sizeof(qcfg_ioctl));
        qcfg_ioctl.action = pqcfg->action;
        qcfg_ioctl.access_category = pqcfg->access_category;
        qcfg_ioctl.msdu_lifetime_expiry = pqcfg->msdu_lifetime_expiry;
        wrq->u.data.length = sizeof(qcfg_ioctl);

        if (copy_to_user(wrq->u.data.pointer, &qcfg_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "QCONFIG: copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get and start/stop queue stats on a WMM AC
 *
 *  Receive a AC Queue statistics command from the application for a specific
 *    WMM AC.  The command can:
 *         - Turn stats on
 *         - Turn stats off
 *         - Collect and clear the stats
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_queue_stats_t struct
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_queue_stats_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    mlan_ds_wmm_queue_stats *pqstats = NULL;
    wlan_ioctl_wmm_queue_stats_t qstats_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_QUEUE_STATS;

    memset(&qstats_ioctl, 0x00, sizeof(qstats_ioctl));
    pqstats = (mlan_ds_wmm_queue_stats *) & pwmm->param.q_stats;

    if (wrq->u.data.length) {
        if (copy_from_user(&qstats_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(qstats_ioctl)))) {
            PRINTM(MERROR, "QSTATS: copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        memcpy((void *) pqstats, (void *) &qstats_ioctl, sizeof(qstats_ioctl));
        PRINTM(MINFO, "QSTATS: IOCTL [%d,%d]\n", qstats_ioctl.action,
               qstats_ioctl.user_priority);

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        memset(&qstats_ioctl, 0x00, sizeof(qstats_ioctl));
        memcpy((void *) &qstats_ioctl, (void *) pqstats, sizeof(qstats_ioctl));
        wrq->u.data.length = sizeof(qstats_ioctl);

        if (copy_to_user
            (wrq->u.data.pointer, &qstats_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "QSTATS: copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get the status of the WMM queues
 *
 *  Return the following information for each WMM AC:
 *        - WMM IE Acm Required
 *        - Firmware Flow Required
 *        - Firmware Flow Established
 *        - Firmware Queue Enabled
 *        - Firmware Delivery Enabled
 *        - Firmware Trigger Enabled
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_queue_status_t struct for request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_queue_status_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    wlan_ioctl_wmm_queue_status_t qstatus_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_QUEUE_STATUS;

    if (wrq->u.data.length == sizeof(qstatus_ioctl)) {
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        memset(&qstatus_ioctl, 0x00, sizeof(qstatus_ioctl));
        memcpy((void *) &qstatus_ioctl, (void *) &pwmm->param.q_status,
               sizeof(qstatus_ioctl));
        wrq->u.data.length = sizeof(qstatus_ioctl);
    } else {
        wrq->u.data.length = 0;
    }

    if (copy_to_user(wrq->u.data.pointer, &qstatus_ioctl, wrq->u.data.length)) {
        PRINTM(MERROR, "QSTATUS: copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get the status of the WMM Traffic Streams
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_ts_status_t struct for request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_ts_status_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    wlan_ioctl_wmm_ts_status_t ts_status_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_TS_STATUS;

    memset(&ts_status_ioctl, 0x00, sizeof(ts_status_ioctl));

    if (wrq->u.data.length == sizeof(ts_status_ioctl)) {
        if (copy_from_user(&ts_status_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(ts_status_ioctl)))) {
            PRINTM(MERROR, "TS_STATUS: copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        memset(&pwmm->param.ts_status, 0x00, sizeof(ts_status_ioctl));
        pwmm->param.ts_status.tid = ts_status_ioctl.tid;

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        memset(&ts_status_ioctl, 0x00, sizeof(ts_status_ioctl));
        memcpy((void *) &ts_status_ioctl, (void *) &pwmm->param.ts_status,
               sizeof(ts_status_ioctl));
        wrq->u.data.length = sizeof(ts_status_ioctl);
    } else {
        wrq->u.data.length = 0;
    }

    if (copy_to_user(wrq->u.data.pointer, &ts_status_ioctl, wrq->u.data.length)) {
        PRINTM(MERROR, "TS_STATUS: copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get the By-passed TX packet from upper layer
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the packet
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_bypassed_packet_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    struct sk_buff *skb = NULL;
    struct ethhdr *eth;
    t_u16 moreLen = 0, copyLen = 0;
    ENTER();

#define MLAN_BYPASS_PKT_EXTRA_OFFSET        (4)

    copyLen = wrq->u.data.length;
    moreLen = MLAN_MIN_DATA_HEADER_LEN + MLAN_BYPASS_PKT_EXTRA_OFFSET
        + sizeof(mlan_buffer);

    skb = alloc_skb(copyLen + moreLen, GFP_KERNEL);
    if (skb == NULL) {
        PRINTM(MERROR, "kmalloc no memory !!\n");
        LEAVE();
        return -ENOMEM;
    }

    skb_reserve(skb, moreLen);

    if (copy_from_user(skb_put(skb, copyLen), wrq->u.data.pointer, copyLen)) {
        PRINTM(MERROR, "PortBlock: copy from user failed\n");
        dev_kfree_skb_any(skb);
        ret = -EFAULT;
        goto done;
    }

    eth = (struct ethhdr *) skb->data;
    eth->h_proto = __constant_htons(eth->h_proto);
    skb->dev = priv->netdev;

    HEXDUMP("Bypass TX Data", skb->data, MIN(skb->len, 100));

    woal_hard_start_xmit(skb, priv->netdev);
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get auth type
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_auth_type(moal_private * priv, struct iwreq *wrq)
{
    int auth_type;
    t_u32 auth_mode;
    int ret = 0;

    ENTER();
    if (wrq->u.data.length == 0) {
        if (MLAN_STATUS_SUCCESS !=
            woal_get_auth_mode(priv, MOAL_IOCTL_WAIT, &auth_mode)) {
            ret = -EFAULT;
            goto done;
        }
        auth_type = auth_mode;
        if (copy_to_user(wrq->u.data.pointer, &auth_type, sizeof(auth_type))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    } else {
        if (copy_from_user(&auth_type, wrq->u.data.pointer, sizeof(auth_type))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        PRINTM(MINFO, "SET: auth_type %d\n", auth_type);
        if (((auth_type < MLAN_AUTH_MODE_OPEN) ||
             (auth_type > MLAN_AUTH_MODE_SHARED))
            && (auth_type != MLAN_AUTH_MODE_AUTO)) {
            ret = -EINVAL;
            goto done;
        }
        auth_mode = auth_type;
        if (MLAN_STATUS_SUCCESS !=
            woal_set_auth_mode(priv, MOAL_IOCTL_WAIT, auth_mode)) {
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Port Control mode
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_port_ctrl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    int ret = 0;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_PORT_CTRL_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;

    if (wrq->u.data.length) {
        if (copy_from_user(&sec->param.port_ctrl_enabled,
                           wrq->u.data.pointer, sizeof(int)) != 0) {
            PRINTM(MERROR, "port_ctrl:Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        req->action = MLAN_ACT_SET;
    } else {
        req->action = MLAN_ACT_GET;
    }

    /* Send IOCTL request to MLAN */
    if (woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT) != MLAN_STATUS_SUCCESS) {
        ret = -EFAULT;
        goto done;
    }

    if (!wrq->u.data.length) {
        if (copy_to_user(wrq->u.data.pointer, &sec->param.port_ctrl_enabled,
                         sizeof(int))) {
            PRINTM(MERROR, "port_ctrl:Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

#if defined(DFS_TESTING_SUPPORT)
/**
 *  @brief Set/Get DFS Testing settings
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_dfs_testing(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_11h_cfg *ds_11hcfg = NULL;
    int ret = 0;
    int data[4];
    int data_length = wrq->u.data.length;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11h_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    ds_11hcfg = (mlan_ds_11h_cfg *) req->pbuf;
    ds_11hcfg->sub_command = MLAN_OID_11H_DFS_TESTING;
    req->req_id = MLAN_IOCTL_11H_CFG;

    if (!data_length) {
        req->action = MLAN_ACT_GET;
    } else if (data_length == 4) {
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * data_length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((unsigned) data[0] > 0xFFFF) {
            PRINTM(MERROR, "The maximum user CAC is 65535 msec.\n");
            ret = -EINVAL;
            goto done;
        }
        if ((unsigned) data[1] > 0xFFFF) {
            PRINTM(MERROR, "The maximum user NOP is 65535 sec.\n");
            ret = -EINVAL;
            goto done;
        }
        if ((unsigned) data[3] > 0xFF) {
            PRINTM(MERROR, "The maximum user fixed channel is 255.\n");
            ret = -EINVAL;
            goto done;
        }
        ds_11hcfg->param.dfs_testing.usr_cac_period_msec = (t_u16) data[0];
        ds_11hcfg->param.dfs_testing.usr_nop_period_sec = (t_u16) data[1];
        ds_11hcfg->param.dfs_testing.usr_no_chan_change = data[2] ? 1 : 0;
        ds_11hcfg->param.dfs_testing.usr_fixed_new_chan = (t_u8) data[3];
        priv->phandle->cac_period_jiffies = (t_u16) data[0] * HZ / 1000;
        req->action = MLAN_ACT_SET;
    } else {
        PRINTM(MERROR, "Invalid number of args!\n");
        ret = -EINVAL;
        goto done;
    }

    /* Send IOCTL request to MLAN */
    if (woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT) != MLAN_STATUS_SUCCESS) {
        ret = -EFAULT;
        goto done;
    }

    if (!data_length) {
        data[0] = ds_11hcfg->param.dfs_testing.usr_cac_period_msec;
        data[1] = ds_11hcfg->param.dfs_testing.usr_nop_period_sec;
        data[2] = ds_11hcfg->param.dfs_testing.usr_no_chan_change;
        data[3] = ds_11hcfg->param.dfs_testing.usr_fixed_new_chan;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int) * 4)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 4;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}
#endif /* DFS_SUPPORT && DFS_TESTING_SUPPORT */

/**
 *  @brief Set/Get Mgmt Frame passthru mask
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *
 *  @return         0 -- success, otherwise fail
 */
static int
woal_mgmt_frame_passthru_ctrl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, data_length = wrq->u.data.length;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *mgmt_cfg = NULL;
    int mask = 0;

    ENTER();

    if (data_length > 1) {
        PRINTM(MERROR, "Invalid no of arguments!\n");
        ret = -EINVAL;
        goto done;
    }
    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    mgmt_cfg = (mlan_ds_misc_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    mgmt_cfg->sub_command = MLAN_OID_MISC_RX_MGMT_IND;

    if (data_length) {          /* SET */
        if (copy_from_user(&mask, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        mgmt_cfg->param.mgmt_subtype_mask = mask;
        req->action = MLAN_ACT_SET;
    } else {
        req->action = MLAN_ACT_GET;
    }

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    mask = mgmt_cfg->param.mgmt_subtype_mask;
    if (copy_to_user(wrq->u.data.pointer, &mask, sizeof(int))) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get Tx/Rx antenna
 *
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 *
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_tx_rx_ant(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_radio_cfg *radio = NULL;
    mlan_ioctl_req *req = NULL;
    int data[2] = { 0 };

    ENTER();

    if (wrq->u.data.length > 2) {
        PRINTM(MERROR, "Invalid number of argument!\n");
        ret = -EFAULT;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_radio_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    radio = (mlan_ds_radio_cfg *) req->pbuf;
    radio->sub_command = MLAN_OID_ANT_CFG;
    req->req_id = MLAN_IOCTL_RADIO_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(data, wrq->u.data.pointer, sizeof(data))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        radio->param.ant_cfg.tx_antenna = data[0];
        radio->param.ant_cfg.rx_antenna = data[0];
        if (wrq->u.data.length == 2)
            radio->param.ant_cfg.rx_antenna = data[1];
        req->action = MLAN_ACT_SET;
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        wrq->u.data.length = 1;
        data[0] = radio->param.ant_cfg.tx_antenna;
        data[1] = radio->param.ant_cfg.rx_antenna;
        if (data[0] && data[1] && (data[0] != data[1]))
            wrq->u.data.length = 2;
        if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/********************************************************
		Global Functions
********************************************************/

/**
 *  @brief Get response pointer in the scan table
 *
 *  @param scan_table   Scan table pointer
 *  @param resp         response pointer to update
 *  @param i            Index at which the response pointer is needed.
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
t_u32
wlan_get_scan_resp_index(t_u8 * scan_table, t_u8 ** resp, int i)
{
    wlan_ioctl_get_scan_table_entry *prsp_info;
    int j, variable_size;
    const int fixed_size = sizeof(wlan_ioctl_get_scan_table_entry);

    ENTER();

    prsp_info = (wlan_ioctl_get_scan_table_entry *) scan_table;

    for (j = 0; j < i; ++j) {
        variable_size = prsp_info->bss_info_length + fixed_size;
        prsp_info = (wlan_ioctl_get_scan_table_entry *) ((t_u8 *) prsp_info
                                                         + variable_size);
    }

    *resp = (t_u8 *) prsp_info;

    LEAVE();

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief ioctl function - entry point
 *
 *  @param dev		A pointer to net_device structure
 *  @param req	   	A pointer to ifreq structure
 *  @param cmd 		Command
 *
 *  @return          0 --success, otherwise fail
 */
int
woal_do_ioctl(struct net_device *dev, struct ifreq *req, int cmd)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    struct iwreq *wrq = (struct iwreq *) req;
    int ret = 0;

    ENTER();

    PRINTM(MINFO, "woal_do_ioctl: ioctl cmd = 0x%x\n", cmd);
    switch (cmd) {
    case WOAL_SETONEINT_GETWORDCHAR:
        switch (wrq->u.data.flags) {
        case WOAL_VERSION:     /* Get driver version */
            ret = woal_get_driver_version(priv, req);
            break;
        case WOAL_VEREXT:      /* Get extended driver version */
            ret = woal_get_driver_verext(priv, req);
            break;
        default:
            ret = -EOPNOTSUPP;
            break;
        }
        break;
    case WOAL_SETNONE_GETNONE:
        switch (wrq->u.data.flags) {
        case WOAL_WARMRESET:
            ret = woal_warm_reset(priv);
            break;
        case WOAL_USB_SUSPEND:
            ret = woal_enter_usb_suspend(priv->phandle);
            break;
        case WOAL_USB_RESUME:
            ret = woal_exit_usb_suspend(priv->phandle);
            break;
        case WOAL_11D_CLR_CHAN_TABLE:
            ret = woal_11d_clr_chan_table(priv, wrq);
            break;
        default:
            ret = -EOPNOTSUPP;
            break;
        }
        break;
    case WOAL_SETONEINT_GETONEINT:
        switch (wrq->u.data.flags) {
        case WOAL_SET_GET_TXRATE:
            ret = woal_set_get_txrate(priv, wrq);
            break;
        case WOAL_SET_GET_REGIONCODE:
            ret = woal_set_get_regioncode(priv, wrq);
            break;
        case WOAL_SET_RADIO:
            ret = woal_set_get_radio(priv, wrq);
            break;
        case WOAL_WMM_ENABLE:
            ret = woal_wmm_enable_ioctl(priv, wrq);
            break;
        case WOAL_11D_ENABLE:
            ret = woal_11d_enable_ioctl(priv, wrq);
            break;
        case WOAL_SET_GET_QOS_CFG:
            ret = woal_set_get_qos_cfg(priv, wrq);
            break;
#if defined(REASSOCIATION)
        case WOAL_SET_GET_REASSOC:
            ret = woal_set_get_reassoc(priv, wrq);
            break;
#endif /* REASSOCIATION */
        case WOAL_TXBUF_CFG:
            ret = woal_txbuf_cfg(priv, wrq);
            break;
        case WOAL_SET_GET_WWS_CFG:
            ret = woal_wws_cfg(priv, wrq);
            break;
        case WOAL_SLEEP_PD:
            ret = woal_sleep_pd(priv, wrq);
            break;
        case WOAL_AUTH_TYPE:
            ret = woal_auth_type(priv, wrq);
            break;
        case WOAL_PORT_CTRL:
            ret = woal_port_ctrl(priv, wrq);
            break;
#if defined(WIFI_DIRECT_SUPPORT)
#if defined(STA_SUPPORT) && defined(UAP_SUPPORT)
        case WOAL_SET_GET_BSS_ROLE:
            ret = woal_set_get_bss_role(priv, wrq);
            break;
#endif
#endif
        case WOAL_SET_GET_11H_LOCAL_PWR_CONSTRAINT:
            ret = woal_set_get_11h_local_pwr_constraint(priv, wrq);
            break;
        case WOAL_HT_STREAM_CFG:
            ret = woal_ht_stream_cfg_ioctl(priv, wrq);
            break;
        case WOAL_MAC_CONTROL:
            ret = woal_mac_control_ioctl(priv, wrq);
            break;
        case WOAL_THERMAL:
            ret = woal_thermal_ioctl(priv, wrq);
            break;
        default:
            ret = -EOPNOTSUPP;
            break;
        }
        break;

    case WOAL_SET_GET_SIXTEEN_INT:
        switch ((int) wrq->u.data.flags) {
        case WOAL_TX_POWERCFG:
            ret = woal_tx_power_cfg(priv, wrq);
            break;
#ifdef DEBUG_LEVEL1
        case WOAL_DRV_DBG:
            ret = woal_drv_dbg(priv, wrq);
            break;
#endif
        case WOAL_BEACON_INTERVAL:
            ret = woal_beacon_interval(priv, wrq);
            break;
        case WOAL_ATIM_WINDOW:
            ret = woal_atim_window(priv, wrq);
            break;
        case WOAL_SIGNAL:
            ret = woal_get_signal(priv, wrq);
            break;
        case WOAL_HEART_BEAT:
            ret = woal_heart_beat_ioctl(priv, wrq);
            break;
        case WOAL_DEEP_SLEEP:
            ret = woal_deep_sleep_ioctl(priv, wrq);
            break;
        case WOAL_11N_TX_CFG:
            ret = woal_11n_tx_cfg(priv, wrq);
            break;
        case WOAL_11N_AMSDU_AGGR_CTRL:
            ret = woal_11n_amsdu_aggr_ctrl(priv, wrq);
            break;
        case WOAL_11N_HTCAP_CFG:
            ret = woal_11n_htcap_cfg(priv, wrq);
            break;
        case WOAL_PRIO_TBL:
            ret = woal_11n_prio_tbl(priv, wrq);
            break;
        case WOAL_ADDBA_UPDT:
            ret = woal_addba_para_updt(priv, wrq);
            break;
        case WOAL_ADDBA_REJECT:
            ret = woal_addba_reject(priv, wrq);
            break;
        case WOAL_TX_BF_CAP:
            ret = woal_tx_bf_cap_ioctl(priv, wrq);
            break;
        case WOAL_HS_CFG:
            ret = woal_hs_cfg(priv, wrq, MTRUE);
            break;
        case WOAL_HS_SETPARA:
            ret = woal_hs_setpara(priv, wrq);
            break;
        case WOAL_REG_READ_WRITE:
            ret = woal_reg_read_write(priv, wrq);
            break;
        case WOAL_INACTIVITY_TIMEOUT_EXT:
            ret = woal_inactivity_timeout_ext(priv, wrq);
            break;
        case WOAL_BAND_CFG:
            ret = woal_band_cfg(priv, wrq);
            break;
        case WOAL_SCAN_CFG:
            ret = woal_set_get_scan_cfg(priv, wrq);
            break;
        case WOAL_PS_CFG:
            ret = woal_set_get_ps_cfg(priv, wrq);
            break;
        case WOAL_MEM_READ_WRITE:
            ret = woal_mem_read_write(priv, wrq);
            break;
        case WOAL_SLEEP_PARAMS:
            ret = woal_sleep_params_ioctl(priv, wrq);
            break;
#if defined(DFS_TESTING_SUPPORT)
        case WOAL_DFS_TESTING:
            ret = woal_dfs_testing(priv, wrq);
            break;
#endif
        case WOAL_MGMT_FRAME_CTRL:
            ret = woal_mgmt_frame_passthru_ctrl(priv, wrq);
            break;
        case WOAL_SET_GET_TX_RX_ANT:
            ret = woal_set_get_tx_rx_ant(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOALGETLOG:
        ret = woal_get_log(priv, wrq);
        break;

    case WOAL_SET_GET_256_CHAR:
        switch (wrq->u.data.flags) {
        case WOAL_PASSPHRASE:
            ret = woal_passphrase(priv, wrq);
            break;
        case WOAL_ADHOC_AES:
            ret = woal_adhoc_aes_ioctl(priv, wrq);
            break;
        case WOAL_WMM_QUEUE_STATUS:
            ret = woal_wmm_queue_status_ioctl(priv, wrq);
            break;

        case WOAL_WMM_TS_STATUS:
            ret = woal_wmm_ts_status_ioctl(priv, wrq);
            break;
        case WOAL_IP_ADDRESS:
            ret = woal_set_get_ip_addr(priv, wrq);
            break;
        case WOAL_TX_BF_CFG:
            ret = woal_tx_bf_cfg_ioctl(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOAL_SETADDR_GETNONE:
        switch ((int) wrq->u.data.flags) {
        case WOAL_DEAUTH:
            ret = woal_deauth(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOAL_SETNONE_GETTWELVE_CHAR:
        /*
         * We've not used IW_PRIV_TYPE_FIXED so sub-ioctl number is
         * in flags of iwreq structure, otherwise it will be in
         * mode member of iwreq structure.
         */
        switch ((int) wrq->u.data.flags) {
        case WOAL_WPS_SESSION:
            ret = woal_wps_cfg_ioctl(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;
    case WOAL_SETNONE_GET_FOUR_INT:
        switch ((int) wrq->u.data.flags) {
        case WOAL_DATA_RATE:
            ret = woal_get_txrx_rate(priv, wrq);
            break;
        case WOAL_ESUPP_MODE:
            ret = woal_get_esupp_mode(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOAL_SET_GET_64_INT:
        switch ((int) wrq->u.data.flags) {
        case WOAL_ECL_SYS_CLOCK:
            ret = woal_ecl_sys_clock(priv, wrq);
            break;
        }

        break;

    case WOAL_HOST_CMD:
        ret = woal_host_command(priv, wrq);
        break;
    case WOAL_ARP_FILTER:
        ret = woal_arp_filter(priv, wrq);
        break;
    case WOAL_SET_INTS_GET_CHARS:
        switch ((int) wrq->u.data.flags) {
        case WOAL_READ_EEPROM:
            ret = woal_read_eeprom(priv, wrq);
            break;
        }
        break;
    case WOAL_SET_GET_2K_BYTES:
        switch ((int) wrq->u.data.flags) {
        case WOAL_SET_USER_SCAN:
            ret = woal_set_user_scan_ioctl(priv, wrq);
            break;
        case WOAL_GET_SCAN_TABLE:
            ret = woal_get_scan_table_ioctl(priv, wrq);
            break;
        case WOAL_WMM_ADDTS:
            ret = woal_wmm_addts_req_ioctl(priv, wrq);
            break;
        case WOAL_WMM_DELTS:
            ret = woal_wmm_delts_req_ioctl(priv, wrq);
            break;
        case WOAL_WMM_QUEUE_CONFIG:
            ret = woal_wmm_queue_config_ioctl(priv, wrq);
            break;
        case WOAL_WMM_QUEUE_STATS:
            ret = woal_wmm_queue_stats_ioctl(priv, wrq);
            break;
        case WOAL_BYPASSED_PACKET:
            ret = woal_bypassed_packet_ioctl(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

#ifdef WIFI_DIRECT_SUPPORT
    case WOAL_WIFIDIRECT_HOST_CMD:
        ret = woal_hostcmd_ioctl(dev, req);
        break;
#endif
    case WOAL_FROYO_START:
        break;
    case WOAL_FROYO_WL_FW_RELOAD:
        break;
    case WOAL_FROYO_STOP:
        if (MLAN_STATUS_SUCCESS != woal_disconnect(priv, MOAL_IOCTL_WAIT, NULL)) {
            ret = -EFAULT;
        }
        break;
    case WOAL_CUSTOM_IE_CFG:
        ret = woal_custom_ie_ioctl(dev, req);
        break;
    case WOAL_MGMT_FRAME_TX:
        ret = woal_mgmt_frame_tx(dev, req);
        break;
    case WOAL_GET_BSS_TYPE:
        ret = woal_get_bss_type(dev, req);
        break;
    default:
        ret = -EINVAL;
        break;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Get mode
 *
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option   Wait option (MOAL_WAIT or MOAL_NO_WAIT)
 *
 *  @return              Wireless mode
 */
t_u32
woal_get_mode(moal_private * priv, t_u8 wait_option)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    t_u32 mode = priv->w_stats.status;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MODE;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        switch (bss->param.bss_mode) {
        case MLAN_BSS_MODE_INFRA:
            mode = IW_MODE_INFRA;
            break;
        case MLAN_BSS_MODE_IBSS:
            mode = IW_MODE_ADHOC;
            break;
        default:
            mode = IW_MODE_AUTO;
            break;
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return mode;
}

/**
 *  @brief Get statistics information
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wait_option  Wait option
 *  @param stats        A pointer to mlan_ds_get_stats structure
 *
 *  @return             MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING -- success, otherwise fail
 */
mlan_status
woal_get_stats_info(moal_private * priv, t_u8 wait_option,
                    mlan_ds_get_stats * stats)
{
    int ret = 0;
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_STATS;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (stats)
            memcpy(stats, &info->param.stats, sizeof(mlan_ds_get_stats));
        priv->w_stats.discard.fragment = info->param.stats.fcs_error;
        priv->w_stats.discard.retries = info->param.stats.retry;
        priv->w_stats.discard.misc = info->param.stats.ack_failure;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/**
 *  @brief Get data rates
 *
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option   Wait option
 *  @param m_rates       A pointer to moal_802_11_rates structure
 *
 *  @return              MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING -- success, otherwise fail
 */
mlan_status
woal_get_data_rates(moal_private * priv, t_u8 wait_option,
                    moal_802_11_rates * m_rates)
{
    int ret = 0;
    mlan_ds_rate *rate = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    rate = (mlan_ds_rate *) req->pbuf;
    rate->sub_command = MLAN_OID_SUPPORTED_RATES;
    req->req_id = MLAN_IOCTL_RATE;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (m_rates)
            m_rates->num_of_rates =
                woal_copy_rates(m_rates->rates, m_rates->num_of_rates,
                                rate->param.rates, MLAN_SUPPORTED_RATES);
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/**
 *  @brief Get channel list
 *
 *  @param priv            A pointer to moal_private structure
 *  @param wait_option     Wait option
 *  @param chan_list       A pointer to mlan_chan_list structure
 *
 *  @return                MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING -- success, otherwise fail
 */
mlan_status
woal_get_channel_list(moal_private * priv, t_u8 wait_option,
                      mlan_chan_list * chan_list)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_CHANNEL_LIST;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (chan_list) {
            memcpy(chan_list, &bss->param.chanlist, sizeof(mlan_chan_list));
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/**
 *  @brief Handle get info resp
 *
 *  @param priv 	Pointer to moal_private structure
 *  @param info 	Pointer to mlan_ds_get_info structure
 *
 *  @return    		N/A
 */
void
woal_ioctl_get_info_resp(moal_private * priv, mlan_ds_get_info * info)
{
    ENTER();
    switch (info->sub_command) {
    case MLAN_OID_GET_STATS:
        priv->w_stats.discard.fragment = info->param.stats.fcs_error;
        priv->w_stats.discard.retries = info->param.stats.retry;
        priv->w_stats.discard.misc = info->param.stats.ack_failure;
        break;
    case MLAN_OID_GET_SIGNAL:
        if (info->param.signal.selector & BCN_RSSI_AVG_MASK)
            priv->w_stats.qual.level = info->param.signal.bcn_rssi_avg;
        if (info->param.signal.selector & BCN_NF_AVG_MASK)
            priv->w_stats.qual.noise = info->param.signal.bcn_nf_avg;
        break;
    default:
        break;
    }
    LEAVE();
}

/**
 *  @brief Handle get BSS resp
 *
 *  @param priv 	Pointer to moal_private structure
 *  @param bss 		Pointer to mlan_ds_bss structure
 *
 *  @return    		N/A
 */
void
woal_ioctl_get_bss_resp(moal_private * priv, mlan_ds_bss * bss)
{
    t_u32 mode = 0;

    ENTER();

    switch (bss->sub_command) {
    case MLAN_OID_BSS_MODE:
        if (bss->param.bss_mode == MLAN_BSS_MODE_INFRA)
            mode = IW_MODE_INFRA;
        else if (bss->param.bss_mode == MLAN_BSS_MODE_IBSS)
            mode = IW_MODE_ADHOC;
        else
            mode = IW_MODE_AUTO;
        priv->w_stats.status = mode;
        break;
    default:
        break;
    }

    LEAVE();
    return;
}
