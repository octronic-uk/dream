/*
 * TextureManager.cpp
 *
 * Created: 30/11/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
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
#include "TextureManager.h"

namespace Dream {
    vector<Texture> TextureManager::sTextureCache = vector<Texture>();

    TextureManager::TextureManager() {}

    vector<Texture> TextureManager::getTextureCache() {
      return sTextureCache;
    }

    GLuint TextureManager::loadTextureFromFile(const char* file_c, const char* directory_c) {
      cout << "TextureManager: Loading from dir " << directory_c << endl;
      cout << "TextureManager:             file " << file_c << endl;
      //Generate texture ID and load texture data
      string filename = string(file_c);
      string directory = string(directory_c);
      filename = directory + '/' + filename;
      GLuint textureID;
      glGenTextures(1, &textureID);
      int width,height;
      unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
      // Assign texture to ID
      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      glGenerateMipmap(GL_TEXTURE_2D);

      // Parameters
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      SOIL_free_image_data(image);
      return textureID;
    }
} // End of Dream