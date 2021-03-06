#pragma once

#include "GLWidget.h"

namespace octronic::dream
{
  class EntityRuntime;
}

namespace octronic::dream::tool
{
  class SelectionHighlighter : public GLWidget
  {
  public:
    SelectionHighlighter(DreamToolContext& project);
    ~SelectionHighlighter() override;
    void setSelectedEntity(EntityRuntime& selected);
    void init() override;
    void draw() override;
    void updateGeometry();
    void clearSelection();

  protected:
    EntityRuntime& getSelectedEntityRuntime();
  private:
    optional<reference_wrapper<EntityRuntime>> mSelectedEntityRuntime;
    float mOffset;
    vec4 mSelectionColour;
    vec4 mOutlineColor;
    vec4 mXColour;
    vec4 mYColour;
    vec4 mZColour;
  };
}
