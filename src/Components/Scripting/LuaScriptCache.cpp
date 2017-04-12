/*
 * LuaScriptCache.cpp
 *
 * Created: 04/04/2017 2017 by Ashley
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

#include "LuaScriptCache.h"
#include "../../FileReader.h"

namespace Dream {
    map<string,string> LuaScriptCache::sScriptCache = map<string,string>();
    string LuaScriptCache::getScript(string path) {
      map<string,string>::iterator it;
      for (it=sScriptCache.begin();it!=sScriptCache.end();it++) {
         if (it->first == path) {
             if (DEBUG) {
                 cout << "LuaScriptCache: Found script in cache " << path << endl;
             }
             return it->second;
         }
      }
      return readIntoCache(path);
    }

    string LuaScriptCache::readIntoCache(string path) {
        FileReader reader(path);
        if(!reader.readIntoStringStream()) {
            cerr << "LuaScriptCache: Error reading Lua script into cache" << path << endl;
           return "";
        }
        if (DEBUG) {
            cout << "LuaScriptCache: Inserting script " << path << endl;
        }
        string content = reader.getContentsAsString();
        sScriptCache.insert(pair<string,string>(path,content));
        return content;
    }

    LuaScriptCache::LuaScriptCache() {}
    LuaScriptCache::~LuaScriptCache() {}
}