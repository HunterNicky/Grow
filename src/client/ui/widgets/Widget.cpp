#include "chroma/client/ui/widgets/Widget.h"

#include <raylib.h>

namespace chroma::client::ui::widget {
void Widget::SetBounds(const Rectangle &bounds)
{
    bounds_ = bounds;
}

Rectangle Widget::GetBounds() const
{
    return bounds_;
}
}// namespace chroma::client::ui::widget