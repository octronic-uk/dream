/*
 * TextureDefinition.h
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class TextureDefinition : public AssetDefinition
    {
    public:
        TextureDefinition(const shared_ptr<ProjectDefinition>& pd, const json& js);
        ~TextureDefinition() override;

        bool getIsEnvironmentTexture() const;
        void setIsEnvironmentTexture(bool b);

        bool getFlipVertical() const;
        void setFlipVertical(bool b);

        UuidType getEquiToCubeMapShader() const;
        void setEquiToCubeMapShader(UuidType u);

        UuidType getIrradianceMapShader() const;
        void setIrradianceMapShader(UuidType u);

        UuidType getPreFilterShader() const;
        void setPreFilterShader(UuidType u);

        UuidType getBrdfLutShader() const;
        void setBrdfLutShader(UuidType u);
    };
}

