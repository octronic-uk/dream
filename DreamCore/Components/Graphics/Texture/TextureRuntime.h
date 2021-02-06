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

#include "Common/GLHeader.h"

#include <iostream>
#include <memory>
#include "Components/SharedAssetRuntime.h"
#include "Components/Graphics/GraphicsComponentTask.h"
#include "TextureTasks.h"

using std::vector;
using std::string;
using std::shared_ptr;

namespace octronic::dream
{
    class TextureDefinition;
    class EntityRuntime;
    class TextureDestructionTask;
    class TextureConstructionTask;
    class SpriteRuntime;

    class TextureRuntime : public SharedAssetRuntime
    {
    public:
        TextureRuntime(TextureDefinition* def, ProjectRuntime*);
        ~TextureRuntime() override;

        bool operator==(const TextureRuntime& other);

        bool useDefinition() override;

        GLuint getGLID() const;
        void setGLID(const GLuint& gLID);

        string getPath() const;
        void setPath(const string& path);

        int getWidth() const;
        void setWidth(int width);

        int getHeight() const;
        void setHeight(int height);

        int getChannels() const;
        void setChannels(int channels);

        unsigned char* getImage() const;
        void setImage(unsigned char* image);

        void pushSpriteInstance(SpriteRuntime* er);
        void popSpriteInstance(SpriteRuntime* er);
        void popSpriteInstanceByUuid(UuidType spriteUuid);
        vector<SpriteRuntime*> getSpriteInstancesVector();


        void pushConstructionTask();

        bool loadIntoGL();

    private:
        GLuint mGLID;
        int mWidth;
        int mHeight;
        int mChannels;
        unsigned char* mImage;
        vector<SpriteRuntime*> mSpriteInstances;
        TextureConstructionTask mTextureConstructionTask;
        shared_ptr<TextureDestructionTask> mTextureDestructionTask;
    };
}
