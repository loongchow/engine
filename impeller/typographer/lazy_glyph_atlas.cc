// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/typographer/lazy_glyph_atlas.h"

#include "impeller/base/validation.h"
#include "impeller/typographer/text_render_context.h"

#include <utility>

namespace impeller {

LazyGlyphAtlas::LazyGlyphAtlas(
    std::shared_ptr<TextRenderContext> text_render_context)
    : text_render_context_(std::move(text_render_context)),
      alpha_context_(std::make_shared<GlyphAtlasContext>()),
      color_context_(std::make_shared<GlyphAtlasContext>()) {}

LazyGlyphAtlas::~LazyGlyphAtlas() = default;

void LazyGlyphAtlas::AddTextFrame(const TextFrame& frame, Scalar scale) {
  FML_DCHECK(atlas_map_.empty());
  if (frame.GetAtlasType() == GlyphAtlas::Type::kAlphaBitmap) {
    frame.CollectUniqueFontGlyphPairs(alpha_set_, scale);
  } else {
    frame.CollectUniqueFontGlyphPairs(color_set_, scale);
  }
}

void LazyGlyphAtlas::ResetTextFrames() {
  alpha_set_.clear();
  color_set_.clear();
  atlas_map_.clear();
}

std::shared_ptr<GlyphAtlas> LazyGlyphAtlas::CreateOrGetGlyphAtlas(
    Context& context,
    GlyphAtlas::Type type) const {
  {
    auto atlas_it = atlas_map_.find(type);
    if (atlas_it != atlas_map_.end()) {
      return atlas_it->second;
    }
  }

  if (!text_render_context_) {
    VALIDATION_LOG << "Unable to render text because a TextRenderContext has "
                      "not been set.";
    return nullptr;
  }
  if (!text_render_context_->IsValid()) {
    VALIDATION_LOG
        << "Unable to render text because the TextRenderContext is invalid.";
    return nullptr;
  }

  auto& set = type == GlyphAtlas::Type::kAlphaBitmap ? alpha_set_ : color_set_;
  auto atlas_context =
      type == GlyphAtlas::Type::kAlphaBitmap ? alpha_context_ : color_context_;
  auto atlas =
      text_render_context_->CreateGlyphAtlas(context, type, atlas_context, set);
  if (!atlas || !atlas->IsValid()) {
    VALIDATION_LOG << "Could not create valid atlas.";
    return nullptr;
  }
  atlas_map_[type] = atlas;
  return atlas;
}

}  // namespace impeller
