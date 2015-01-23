#include "map/pin_click_manager.hpp"
#include "map/framework.hpp"

#include "search/result.hpp"

#include "anim/task.hpp"
#include "anim/controller.hpp"

#include "geometry/transformations.hpp"

PinClickManager::PinClickManager(Framework & f)
  : m_f(f)
{}

void PinClickManager::Hide()
{
  ///@TODO UVR
  //m_f.Invalidate();
}

void PinClickManager::OnShowMark(UserMark const * mark)
{
  if (mark != nullptr && m_userMarkListener != nullptr)
    m_userMarkListener(mark->Copy());
  SetBalloonVisible(mark != nullptr);
}

void PinClickManager::SetBalloonVisible(bool isVisible)
{
  if (!isVisible && m_f.HasActiveUserMark())
    OnDismiss();

  ///@TODO UVR
  //m_f.Invalidate();
}

void PinClickManager::RemovePin()
{
  m_f.ActivateUserMark(NULL);
  ///@TODO UVR
  //m_f.Invalidate();
}

void PinClickManager::Dismiss()
{
  OnDismiss();
}

void PinClickManager::ClearListeners()
{
  m_userMarkListener = TUserMarkListener();
  m_dismissListener = TDismissListener();
}

void PinClickManager::OnDismiss()
{
  // Can be called before the listeners will be attached (clearing on activity start).
  if (m_dismissListener)
    m_dismissListener();
}
