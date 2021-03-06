#include "drape_frontend/render_group.hpp"

#include "base/stl_add.hpp"
#include "geometry/screenbase.hpp"

#include "std/bind.hpp"

namespace df
{

RenderGroup::RenderGroup(dp::GLState const & state, df::TileKey const & tileKey)
  : m_state(state)
  , m_tileKey(tileKey)
  , m_pendingOnDelete(false)
{
}

RenderGroup::~RenderGroup()
{
  DeleteRange(m_renderBuckets, dp::MasterPointerDeleter());
}

void RenderGroup::Update(ScreenBase const & modelView)
{
  for_each(m_renderBuckets.begin(), m_renderBuckets.end(), bind(&dp::RenderBucket::Update,
                                                                bind(&dp::NonConstGetter<dp::RenderBucket>, _1),
                                                                modelView));
}

void RenderGroup::CollectOverlay(dp::RefPointer<dp::OverlayTree> tree)
{
  for_each(m_renderBuckets.begin(), m_renderBuckets.end(), bind(&dp::RenderBucket::CollectOverlayHandles,
                                                                bind(&dp::NonConstGetter<dp::RenderBucket>, _1),
                                                                tree));
}

void RenderGroup::Render(ScreenBase const & screen)
{
  ASSERT(m_pendingOnDelete == false, ());
  for_each(m_renderBuckets.begin(), m_renderBuckets.end(), bind(&dp::RenderBucket::Render,
                                                                bind(&dp::NonConstGetter<dp::RenderBucket>, _1), screen));
}

void RenderGroup::PrepareForAdd(size_t countForAdd)
{
  m_renderBuckets.reserve(m_renderBuckets.size() + countForAdd);
}

void RenderGroup::AddBucket(dp::TransferPointer<dp::RenderBucket> bucket)
{
  m_renderBuckets.push_back(dp::MasterPointer<dp::RenderBucket>(bucket));
}

bool RenderGroup::IsLess(RenderGroup const & other) const
{
  return m_state < other.m_state;
}

RenderBucketComparator::RenderBucketComparator(set<TileKey> const & activeTiles)
  : m_activeTiles(activeTiles)
  , m_needGroupMergeOperation(false)
  , m_needBucketsMergeOperation(false)
{
}

void RenderBucketComparator::ResetInternalState()
{
  m_needBucketsMergeOperation = false;
  m_needGroupMergeOperation = false;
}

bool RenderBucketComparator::operator()(RenderGroup const * l, RenderGroup const * r)
{
  dp::GLState const & lState = l->GetState();
  dp::GLState const & rState = r->GetState();

  TileKey const & lKey = l->GetTileKey();
  TileKey const & rKey = r->GetTileKey();

  if (!l->IsPendingOnDelete() && (l->IsEmpty() || m_activeTiles.find(lKey) == m_activeTiles.end()))
    l->DeleteLater();

  if (!r->IsPendingOnDelete() && (r->IsEmpty() || m_activeTiles.find(rKey) == m_activeTiles.end()))
    r->DeleteLater();

  bool lPendingOnDelete = l->IsPendingOnDelete();
  bool rPendingOnDelete = r->IsPendingOnDelete();

  if (lState == rState && lKey == rKey && !lPendingOnDelete)
    m_needGroupMergeOperation = true;

  if (rPendingOnDelete == lPendingOnDelete)
    return lState < rState;

  if (rPendingOnDelete)
    return true;

  return false;
}

} // namespace df
