#pragma once

#include "../Cache.h"

namespace Dream
{
    class AudioCache : public Cache
    {
    public:
        AudioCache(ProjectRuntime* parent);
        ~AudioCache() override;
        void clear() override;

    protected:
        SharedAssetRuntime* loadInstance(AssetDefinition* definition) override;
    };
}
