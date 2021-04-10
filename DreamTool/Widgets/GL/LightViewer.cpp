#include "LightViewer.h"
#include "DreamToolContext.h"

#include <DreamCore.h>

namespace octronic::dream::tool
{
  LightViewer::LightViewer
  (DreamToolContext& p)
    : GLWidget(p)
  {

  }


  LightViewer::~LightViewer
  ()
  {
  }

  void
  LightViewer::init
  ()
  {
    GLWidget::init();
    clearTriangleVertexBuffer();
    for (auto index : LightModelIndices)
    {
      addTriangleVertex(LightModelVertices.at(index));
    }
    submitTriangleVertexBuffer();
  }

  void LightViewer::draw
  ()
  {
    auto& pCtxOpt = getContext().getProjectContext();
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();
      auto& projectRuntimeOpt = pCtx.getProjectRuntime();

      if (projectRuntimeOpt)
      {
        auto& projectRuntime = projectRuntimeOpt.value();

        auto& gc = projectRuntime.getGraphicsComponent();
        size_t num_lights = gc.getLightCount();
        for (size_t i=0; i<num_lights; i++)
        {
          vec3 color = gc.getLightColor(i);
          vec3 position = gc.getLightPosition(i);
          mTransform.setTranslation(position);
          GLWidget::draw();
        }
      }
    }
  }

  const vector<TranslationColorVertex> LightViewer::LightModelVertices =
  {
    {vec3(-0.125f, 0.500f, 0.000f),  vec4(1.0f)},
    {vec3(0.125f, 0.500f, 0.000f),   vec4(1.0f)},
    {vec3(-0.125f, 0.462f, -0.191f), vec4(1.0f)},
    {vec3(0.125f, 0.462f, -0.191f),  vec4(1.0f)},
    {vec3(-0.125f, 0.354f, -0.354f), vec4(1.0f)},
    {vec3(0.125f, 0.354f, -0.354f),  vec4(1.0f)},
    {vec3(-0.125f, 0.191f, -0.462f), vec4(1.0f)},
    {vec3(0.125f, 0.191f, -0.462f),  vec4(1.0f)},
    {vec3(-0.125f,-0.000f, -0.500f), vec4(1.0f)},
    {vec3(0.125f,-0.000f, -0.500f),  vec4(1.0f)},
    {vec3(-0.125f,-0.191f, -0.462f), vec4(1.0f)},
    {vec3(0.125f,-0.191f, -0.462f),  vec4(1.0f)},
    {vec3(-0.125f,-0.354f, -0.354f), vec4(1.0f)},
    {vec3(0.125f,-0.354f, -0.354f),  vec4(1.0f)},
    {vec3(-0.125f,-0.462f, -0.191f), vec4(1.0f)},
    {vec3(0.125f,-0.462f, -0.191f),  vec4(1.0f)},
    {vec3(-0.125f,-0.500f, -0.000f), vec4(1.0f)},
    {vec3(0.125f,-0.500f, -0.000f),  vec4(1.0f)},
    {vec3(-0.125f,-0.462f, 0.191f),  vec4(1.0f)},
    {vec3(0.125f,-0.462f, 0.191f),   vec4(1.0f)},
    {vec3(-0.125f,-0.354f, 0.354f),  vec4(1.0f)},
    {vec3(0.125f,-0.354f, 0.354f),   vec4(1.0f)},
    {vec3(-0.125f,-0.191f, 0.462f),  vec4(1.0f)},
    {vec3(0.125f,-0.191f, 0.462f),   vec4(1.0f)},
    {vec3(-0.125f, 0.000f, 0.500f),  vec4(1.0f)},
    {vec3(0.125f, 0.000f, 0.500f),   vec4(1.0f)},
    {vec3(-0.125f, 0.191f, 0.462f),  vec4(1.0f)},
    {vec3(0.125f, 0.191f, 0.462f),   vec4(1.0f)},
    {vec3(-0.125f, 0.354f, 0.354f),  vec4(1.0f)},
    {vec3(0.125f, 0.354f, 0.354f),   vec4(1.0f)},
    {vec3(-0.125f, 0.462f, 0.191f),  vec4(1.0f)},
    {vec3(0.125f, 0.462f, 0.191f),   vec4(1.0f)},
    {vec3(-0.125f, 0.745f, -0.049f), vec4(1.0f)},
    {vec3(-0.125f, 0.707f, -0.240f), vec4(1.0f)},
    {vec3(0.125f, 0.707f, -0.240f),  vec4(1.0f)},
    {vec3(0.125f, 0.745f, -0.049f),  vec4(1.0f)},
    {vec3(0.125f, 0.492f, -0.561f),  vec4(1.0f)},
    {vec3(-0.125f, 0.492f, -0.561f), vec4(1.0f)},
    {vec3(-0.125f, 0.330f, -0.670f), vec4(1.0f)},
    {vec3(0.125f, 0.330f, -0.670f),  vec4(1.0f)},
    {vec3(0.125f,-0.049f, -0.745f),  vec4(1.0f)},
    {vec3(-0.125f,-0.049f, -0.745f), vec4(1.0f)},
    {vec3(-0.125f,-0.240f, -0.707f), vec4(1.0f)},
    {vec3(0.125f,-0.240f, -0.707f),  vec4(1.0f)},
    {vec3(0.125f,-0.561f, -0.492f),  vec4(1.0f)},
    {vec3(-0.125f,-0.561f, -0.492f), vec4(1.0f)},
    {vec3(-0.125f,-0.670f, -0.330f), vec4(1.0f)},
    {vec3(0.125f,-0.670f, -0.330f),  vec4(1.0f)},
    {vec3(0.125f,-0.745f, 0.049f),   vec4(1.0f)},
    {vec3(-0.125f,-0.745f, 0.049f),  vec4(1.0f)},
    {vec3(-0.125f,-0.707f, 0.240f),  vec4(1.0f)},
    {vec3(0.125f,-0.707f, 0.240f),   vec4(1.0f)},
    {vec3(0.125f,-0.492f, 0.561f),   vec4(1.0f)},
    {vec3(-0.125f,-0.492f, 0.561f),  vec4(1.0f)},
    {vec3(-0.125f,-0.330f, 0.670f),  vec4(1.0f)},
    {vec3(0.125f,-0.330f, 0.670f),   vec4(1.0f)},
    {vec3(0.125f, 0.049f, 0.745f),   vec4(1.0f)},
    {vec3(-0.125f, 0.049f, 0.745f),  vec4(1.0f)},
    {vec3(-0.125f, 0.240f, 0.707f),  vec4(1.0f)},
    {vec3(0.125f, 0.240f, 0.707f),   vec4(1.0f)},
    {vec3(0.125f, 0.561f, 0.492f),   vec4(1.0f)},
    {vec3(-0.125f, 0.561f, 0.492f),  vec4(1.0f)},
    {vec3(-0.125f, 0.670f, 0.330f),  vec4(1.0f)},
    {vec3(0.125f, 0.670f, 0.330f),   vec4(1.0f)},
    {vec3(-0.125f, 0.000f, -0.000f), vec4(1.0f)},
    {vec3(0.125f,-0.000f, 0.000f),   vec4(1.0f)},
  };

  const vector<GLuint> LightViewer::LightModelIndices =
  {
    1, 32, 0,
    2, 5, 4,
    29, 61, 28,
    7, 8, 6,
    10, 43, 11,
    10, 13, 12,
    1, 34, 35,
    14, 17, 16,
    11, 40, 9,
    19, 20, 18,
    18, 51, 19,
    23, 24, 22,
    0, 33, 2,
    27, 28, 26,
    1, 65, 3,
    8, 42, 10,
    30, 1, 0,
    8, 64, 6,
    32, 34, 33,
    36, 38, 37,
    40, 42, 41,
    44, 46, 45,
    48, 50, 49,
    52, 54, 53,
    56, 58, 57,
    61, 63, 62,
    30, 61, 62,
    2, 34, 3,
    21, 53, 20,
    31, 60, 29,
    22, 53, 54,
    13, 45, 12,
    30, 63, 31,
    23, 52, 21,
    12, 46, 14,
    5, 37, 4,
    22, 55, 23,
    13, 47, 44,
    4, 38, 6,
    14, 47, 15,
    5, 39, 36,
    6, 39, 7,
    25, 57, 24,
    26, 57, 58,
    17, 49, 16,
    27, 56, 25,
    16, 50, 18,
    9, 41, 8,
    26, 59, 27,
    17, 51, 48,
    1, 35, 32,
    2, 3, 5,
    29, 60, 61,
    7, 9, 8,
    10, 42, 43,
    10, 11, 13,
    1, 3, 34,
    14, 15, 17,
    11, 43, 40,
    19, 21, 20,
    18, 50, 51,
    23, 25, 24,
    0, 32, 33,
    27, 29, 28,
    13, 65, 15,
    11, 65, 13,
    7, 65, 9,
    5, 65, 7,
    5, 3, 65,
    31, 65, 1,
    29, 65, 31,
    25, 65, 27,
    27, 65, 29,
    23, 65, 25,
    15, 65, 17,
    9, 65, 11,
    21, 65, 23,
    8, 41, 42,
    30, 31, 1,
    4, 64, 2,
    2, 64, 0,
    6, 64, 4,
    12, 64, 10,
    10, 64, 8,
    14, 64, 12,
    32, 35, 34,
    36, 39, 38,
    40, 43, 42,
    44, 47, 46,
    48, 51, 50,
    52, 55, 54,
    56, 59, 58,
    61, 60, 63,
    30, 28, 61,
    2, 33, 34,
    21, 52, 53,
    31, 63, 60,
    22, 20, 53,
    13, 44, 45,
    30, 62, 63,
    23, 55, 52,
    12, 45, 46,
    5, 36, 37,
    22, 54, 55,
    13, 15, 47,
    4, 37, 38,
    14, 46, 47,
    5, 7, 39,
    6, 38, 39,
    25, 56, 57,
    26, 24, 57,
    17, 48, 49,
    27, 59, 56,
    16, 49, 50,
    9, 40, 41,
    26, 58, 59,
    17, 19, 51,
    0, 64, 30,
    30, 64, 28,
    28, 64, 26,
    26, 64, 24,
    24, 64, 22,
    22, 64, 20,
    20, 64, 18,
    18, 64, 16,
    14, 16, 64,
    17, 65, 19,
    19, 65, 21,
  };
}


