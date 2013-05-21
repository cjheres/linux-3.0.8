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
#ifndef __GENERIC_BUFFER_INC__
#define __GENERIC_BUFFER_INC__

typedef void (*t_buffer_notifier_cb)(HANDLE hBuffer, INT fullness, UINT tag);

#define MV_GENERIC_BUF_TYPE_XBV         1

typedef struct t_buffer_notifier {

    //! consumer use here
    INT     m_Lwm;
    t_buffer_notifier_cb    m_LwmCallback;
    UINT    m_ConsumerInfo;
    UINT    m_LwmCBLast;        // time stamp of last m_LwmCallback call
                                // to avoid too frequent func call

    //! producer register here
    INT     m_Hwm;
    t_buffer_notifier_cb    m_HwmCallback;
    UINT    m_ProducerInfo;
    UINT    m_HwmCBLast;        // time stamp of last m_HwmCallback call
                                // to avoid too frequent func call

}Buffer_Notifier_t;

INT MV_GBUF_ProducerRegisterHighWaterMark(HANDLE hBuffer, void* pCB, UINT uThreshold, UINT uUserData);
INT MV_GBUF_ConsumerRegisterLowWaterMark(HANDLE hBuffer, void* pCB, UINT uThreshold, UINT uUserData);
INT MV_GBUF_NotifyProducer(HANDLE hBuffer, INT fullness, UINT tag);
INT MV_GBUF_NotifyConsumer(HANDLE hBuffer, INT fullness, UINT tag);

    INT MV_GBUF_GetHighWaterMark(HANDLE hBuffer);

    INT MV_GBUF_GetLowWaterMark(HANDLE hBuffer);

    INT MV_GBUF_GetProducerInfo(HANDLE hBuffer);

    INT MV_GBUF_GetConsumerInfo(HANDLE hBuffer);


#endif  //__GENERIC_BUFFER_INC

