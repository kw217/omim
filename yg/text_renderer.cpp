#include "text_renderer.hpp"
#include "render_state.hpp"
#include "info_layer.hpp"
#include "resource_style.hpp"
#include "resource_manager.hpp"

#include "../geometry/angles.hpp"

#include "../std/bind.hpp"

#include "../base/string_utils.hpp"


namespace yg
{
  namespace gl
  {

    TextRenderer::Params::Params()
      : m_drawTexts(true),
        m_glyphCacheID(-1)
    {}

    TextRenderer::TextRenderer(Params const & params)
      : base_t(params),
        m_drawTexts(params.m_drawTexts),
        m_glyphCacheID(params.m_glyphCacheID)
    {}


    void TextRenderer::drawGlyph(m2::PointD const & ptOrg, m2::PointD const & ptGlyph, ang::AngleD const & angle, float /*blOffset*/, CharStyle const * p, double depth)
    {
      float x0 = ptGlyph.x + (p->m_xOffset - 1);
      float y1 = ptGlyph.y - (p->m_yOffset - 1);
      float y0 = y1 - (p->m_texRect.SizeY() - 2);
      float x1 = x0 + (p->m_texRect.SizeX() - 2);

      drawTexturedPolygon(ptOrg, angle,
                          p->m_texRect.minX() + 1,
                          p->m_texRect.minY() + 1,
                          p->m_texRect.maxX() - 1,
                          p->m_texRect.maxY() - 1,
                          x0, y0, x1, y1,
                          depth,
                          p->m_pipelineID);
    }

    GlyphCache * TextRenderer::glyphCache() const
    {
      return resourceManager()->glyphCache(m_glyphCacheID);
    }
  }
}
