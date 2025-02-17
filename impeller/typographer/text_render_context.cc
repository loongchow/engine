// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/typographer/text_render_context.h"

#include <utility>

namespace impeller {

TextRenderContext::TextRenderContext() {
  is_valid_ = true;
}

TextRenderContext::~TextRenderContext() = default;

bool TextRenderContext::IsValid() const {
  return is_valid_;
}

}  // namespace impeller
