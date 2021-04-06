/*
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

    class MaterialDefinition : public AssetDefinition
    {
    public:
        MaterialDefinition(ProjectDefinition& pd, const json& js);

				MaterialDefinition(MaterialDefinition&&) = default;
        MaterialDefinition& operator=(MaterialDefinition&&) = default;

        // Shader
        UuidType getShaderUuid() const;
        void setShaderUuid(UuidType);

        // Textures
        UuidType getAlbedoTextureUuid() const;
        void setAlbedoTextureUuid(UuidType);

        UuidType getNormalTextureUuid() const;
        void setNormalTextureUuid(UuidType);

        UuidType getMetallicTextureUuid() const;
        void setMetallicTextureUuid(UuidType);

        UuidType getRoughnessTextureUuid() const;
        void setRoughnessTextureUuid(UuidType);

        UuidType getAoTextureUuid() const;
        void setAoTextureUuid(UuidType);
    };
}

