/*
 * Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Project.h"

#include <algorithm>
#include <thread>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#include "ProjectRuntime.h"
#include "ProjectDefinition.h"

#include "../Components/IAssetDefinition.h"
#include "../Components/Time.h"
#include "../Components/Transform3D.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/IAssetInstance.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Physics/PhysicsObjectInstance.h"
#include "../Components/Window/IWindowComponent.h"
#include "../Components/Scripting/IScriptComponent.h"

#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneDefinition.h"


#include "../Utilities/ArgumentParser.h"
#include "../Utilities/FileReader.h"
#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    Project::Project
    (IWindowComponent* windowComponent)
        : DreamObject("Project"),
          mDefinition(nullptr),
          mRuntime(nullptr),
          mWindowComponent(windowComponent),
          mProjectPath("")

    {
        getLog()->trace("Constructing");
    }

    Project::~Project()
    {
        auto log = getLog();
        log->trace("Destructing");

        if (mRuntime != nullptr)
        {
            delete mRuntime;
        }

        if (mDefinition != nullptr)
        {
            delete mDefinition;
        }
    }

    bool
    Project::openFromFileReader
    (string projectPath, FileReader &reader)
    {
        auto log = getLog();
        log->debug("Loading project from FileReader", reader.getPath());

        string projectJsonStr = reader.getContentsAsString();

        if (projectJsonStr.empty())
        {
            log->error("Loading Failed. Project Content is Empty");
            return false;
        }

        json projectJson = json::parse(projectJsonStr);

        log->debug("Project path", projectPath);

        mProjectPath = projectPath;
        mDefinition = new ProjectDefinition(projectJson);
        mDefinition->loadChildDefinitions();
        return true;
    }

    bool
    Project::openFromDirectory
    (string directory)
    {
        auto log = getLog();
        vector<string> directoryContents;

#ifndef WIN32
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(directory.c_str())) != nullptr)
        {
            while ((ent = readdir (dir)) != nullptr)
            {
                directoryContents.push_back(string(ent->d_name));
            }
            closedir (dir);
        }
        else
        {
            log->error( "Unable to open directory {}", directory );
            return false;
        }
#else
        WIN32_FIND_DATA data;
        HANDLE hFind = FindFirstFile(directory.c_str(), &data);      // DIRECTORY

        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                directoryContents.push_back(data.cFileName);
            } while (FindNextFile(hFind, &data));
            FindClose(hFind);
        }
#endif

        string projectFileName;
        bool hasAssetDirectory = false;

        for (string filename : directoryContents)
        {
            size_t dotJsonIndex = filename.find(Constants::PROJECT_EXTENSION);
            if (dotJsonIndex != string::npos)
            {
                projectFileName = filename.substr(0,dotJsonIndex);
                log->debug( "Project: openFromDirectory - Found project file ",
                           projectFileName
                           );
            }
            else if (filename.compare(Constants::ASSET_DIR) == 0)
            {
                log->debug( "Project: openFromDirectory - Found asset directory " );
                hasAssetDirectory = true;
            }
        }

        if (projectFileName.size() == 0 || !hasAssetDirectory)
        {
            log->error( "Project: Error {} is not a valid project directory!", directory  );
            return false;
        }

        log->debug( "Project: Loading {}{} from Directory {}", projectFileName , Constants::PROJECT_EXTENSION , directory );

        string projectFilePath = directory + Constants::PROJECT_PATH_SEP + projectFileName + Constants::PROJECT_EXTENSION;

        FileReader projectFileReader(projectFilePath);

        projectFileReader.readIntoString();
        bool loadSuccess = openFromFileReader(directory, projectFileReader);
        return loadSuccess;
    }

    ProjectRuntime*
    Project::createProjectRuntime
    ()
    {
        auto log = getLog();
        mRuntime = new ProjectRuntime(this, mWindowComponent);
        if (!mRuntime->useDefinition())
        {
            log->critical("Failed to create project runtime");
            delete mRuntime;
            mRuntime = nullptr;
        }
        return mRuntime;
    }

    ProjectDefinition*
    Project::createNewProjectDefinition
    (string name)
    {
        json j = json::object();

        j[Constants::NAME] = name;
        j[Constants::UUID] = Uuid::generateUuid();
        j[Constants::PROJECT_AUTHOR] = "";
        j[Constants::PROJECT_DESCRIPTION] = "";
        j[Constants::PROJECT_STARTUP_SCENE] = "";
        j[Constants::PROJECT_CAPTURE_JOYSTICK] = false;
        j[Constants::PROJECT_CAPTURE_MOUSE] = false;
        j[Constants::PROJECT_CAPTURE_KEYBOARD] = false;

        j[Constants::PROJECT_WINDOW_SIZE] = json::object();
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_WIDTH] = Constants::PROJECT_DEFAULT_WINDOW_WIDTH;
        j[Constants::PROJECT_WINDOW_SIZE][Constants::PROJECT_WINDOW_HEIGHT] = Constants::PROJECT_DEFAULT_WINDOW_HEIGHT;

        j[Constants::PROJECT_ASSET_ARRAY] = json::array();
        j[Constants::PROJECT_SCENE_ARRAY] = json::array();

        return new ProjectDefinition(j);
    }

    bool
    Project::hasProjectRuntime
    ()
    {
        return mRuntime != nullptr;
    }

    void Project::resetProjectRuntime()
    {
        auto log = getLog();
        log->debug("Resetting project runtime");
        delete mRuntime;
        mRuntime = nullptr;
    }

    bool
    Project::hasProjectDefinition
    ()
    {
        return mDefinition != nullptr;
    }

    bool
    Project::openFromArgumentParser
    (ArgumentParser &parser)
    {
        auto log = getLog();
        log->debug( "Project: Loading from ArgumentParser" );
        FileReader projectFileReader(parser.getProjectFilePath());
        projectFileReader.readIntoString();
        bool loadSuccess = openFromFileReader(parser.getProjectPath(), projectFileReader);
        return loadSuccess;
    }

    ProjectRuntime*
    Project::getProjectRuntime
    ()
    {
        return mRuntime;
    }

    ProjectDefinition*
    Project::getProjectDefinition
    ()
    {
        return mDefinition;
    }

    string
    Project::getProjectPath
    ()
    {
        return mProjectPath;
    }

    IAssetDefinition* Project::getAssetDefinitionByUuid(string uuid)
    {
        if (mDefinition != nullptr)
        {
            return dynamic_cast<ProjectDefinition*>(mDefinition)
                ->getAssetDefinitionByUuid(uuid);
        }
        return nullptr;
    }


} // End of Dream
