#pragma once

#include "drape/drape_global.hpp"
#include "drape/color.hpp"
#include "drape/stipple_pen_resource.hpp"

#include "indexer/feature_decl.hpp"
#include "geometry/point2d.hpp"

#include "std/string.hpp"

namespace df
{

struct CommonViewParams
{
  float m_depth;
};

struct PoiSymbolViewParams : CommonViewParams
{
  PoiSymbolViewParams(FeatureID const & id) : m_id(id) {}

  FeatureID m_id;
  string m_symbolName;
};

struct CircleViewParams : CommonViewParams
{
  CircleViewParams(FeatureID const & id) : m_id(id) {}

  FeatureID m_id;
  dp::Color m_color;
  float m_radius;
};

struct AreaViewParams : CommonViewParams
{
  dp::Color m_color;
};

struct LineViewParams : CommonViewParams
{
  dp::Color m_color;
  float m_width;
  dp::LineCap m_cap;
  dp::LineJoin m_join;
  buffer_vector<uint8_t, 8> m_pattern;
  float m_baseGtoPScale;
};

struct FontDecl
{
  FontDecl() = default;
  FontDecl(dp::Color const & color, float size, dp::Color const & outlineColor = dp::Color::Transparent())
    : m_color(color)
    , m_outlineColor(outlineColor)
    , m_size(size)
  {
  }

  dp::Color m_color = dp::Color::Transparent();
  dp::Color m_outlineColor = dp::Color::Transparent();
  float m_size = 0;
};

struct TextViewParams : CommonViewParams
{
  FeatureID m_featureID;
  FontDecl m_primaryTextFont;
  string m_primaryText;
  FontDecl m_secondaryTextFont;
  string m_secondaryText;
  dp::Anchor m_anchor;
};

struct PathTextViewParams : CommonViewParams
{
  FontDecl m_textFont;
  string m_text;
  float m_baseGtoPScale;
};

struct PathSymbolViewParams : CommonViewParams
{
  FeatureID m_featureID;
  string m_symbolName;
  float m_offset;
  float m_step;
  float m_baseGtoPScale;
};

} // namespace df
