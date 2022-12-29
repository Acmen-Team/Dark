/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-9-21 8:15
Description:Windows TaskBarProgress Tools
**************************************************/
#pragma once

#include <shobjidl.h>

namespace Dark {

  // http://shanekirk.com/2014/12/making-progress-on-the-windows-taskbar/

  class TaskBarProgress
  {
  public:
    //! Constructor.
    TaskBarProgress(HWND hWnd)
        : m_hWnd(hWnd),
          m_pTaskBarList3(NULL) {}
    //! Destructor.
    virtual ~TaskBarProgress() { DK_CORE_ASSERT(m_pTaskBarList3 == NULL, "m_pTaskBarList3 is null"); }

    //! Starts "progress mode".
    void startProgressMode();
    //! Ends "progress mode".
    void endProgressMode();
    //! Sets the current progress.
    void setProgress(ULONGLONG progressValue, ULONGLONG progressTotal);

  private:
    // We don't want the default implementations of the copy constructor
    // and assignment operator because we have a COM pointer involved.
    // Copying that without doing the appropriate AddRef'ing would be
    // hazardous to our health. Let's just leave these out unless we
    // find we need them at a later date.

    //! Copy constructor. NOT IMPLEMENTED.
    //TaskBarProgress(const TaskBarProgress&);
    //! Assignment operator. NOT IMPLEMENTED.
    //const TaskBarProgress& operator=(const TaskBarProgress&);

    //! The window for which we're showing progress.
    HWND m_hWnd;
    //! The ITaskbarList3 implementer.
    ITaskbarList3* m_pTaskBarList3;
  };

} // namespace Dark