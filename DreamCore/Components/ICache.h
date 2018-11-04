#pragma once

#include "../Common/DreamObject.h"
#include <vector>
#include "IAssetDefinition.h"
#include "IAssetInstance.h"

using std::vector;

namespace Dream
{
    class ProjectRuntime;

    class ICache : public DreamObject
    {
    public:
        ICache(ProjectRuntime* parent);
        virtual ~ICache();

        IAssetInstance* getInstance(IAssetDefinition* definition);
        IAssetInstance* getInstance(string uuid);
        vector<IAssetInstance*> getInstanceVector();
        string getAbsolutePath(IAssetDefinition*);
        virtual void clear();
    protected:
        virtual IAssetInstance* loadInstance(IAssetDefinition* def) = 0;
        IAssetDefinition* getAssetDefinitionByUuid(string uuid);
        vector<IAssetInstance*> mInstances;
        ProjectRuntime* mProjectRuntime;
    };
}
