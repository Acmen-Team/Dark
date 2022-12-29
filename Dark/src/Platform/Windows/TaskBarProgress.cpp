/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-9-21 8:23
Description:Windows TaskBar Progress
**************************************************/
#include "dkpch.h"

#include "TaskBarProgress.h"

namespace Dark {

  void TaskBarProgress::startProgressMode()
  {
    if (!m_hWnd)
      return;

    if (!m_pTaskBarList3)
    {
      HRESULT hr = ::CoCreateInstance(CLSID_TaskbarList, NULL,
                                      CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (void**)&m_pTaskBarList3);

      if (hr != S_OK)
        return; // Not a supported platform. Nothing we can do.
    }

    // "Turning on" progress mode and setting the initial progress value to 0.
    m_pTaskBarList3->SetProgressState(m_hWnd, TBPF_NORMAL);
    m_pTaskBarList3->SetProgressValue(m_hWnd, 0, 100);
  }

  void TaskBarProgress::endProgressMode()
  {
    if (!m_pTaskBarList3 || !m_hWnd)
      return;

    m_pTaskBarList3->SetProgressState(m_hWnd, TBPF_NOPROGRESS);
    m_pTaskBarList3->Release();
    m_pTaskBarList3 = NULL;
  }

  void TaskBarProgress::setProgress(ULONGLONG progressValue, ULONGLONG progressTotal)
  {
    if (!m_pTaskBarList3 || !m_hWnd)
      return;

    m_pTaskBarList3->SetProgressValue(m_hWnd, progressValue, progressTotal);
  }

  //TaskBarProgress::TaskBarProgress(const TaskBarProgress&)
  //{
  //}

  //const Dark::TaskBarProgress& TaskBarProgress::operator=(const TaskBarProgress&)
  //{
  //}

} // namespace Dark