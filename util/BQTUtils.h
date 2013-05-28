/* Copyright 2010 NVIDIA Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Developed by Mustard Seed Software, LLC 
 * http://mseedsoft.com
 * Modified by Matthew Johnson-Roberson 
 * from original source: Visualize Physics - Wave 
 * Copyright (C) 2012 Matthew Johnson-Roberson
 * See COPYING for license details
 */

#ifndef __EWSUTILS_H
#define __EWSUTILS_H

#include <QFile>
#include <QByteArray>

namespace ews {
    namespace util {
        /** Attempt to read a text resource with the given name. 
         * @param resource name of resource
         * @return resource contents as string, or QString::null if not found. 
         */
        static QByteArray loadTextResource(const char* resource) {
            QFile progResource(resource);
            if (!progResource.open(QIODevice::ReadOnly|QIODevice::Text)) {
                qWarning() << "Couldn't find" << resource;
                return QByteArray();
            }
            else {
                qDebug() << "Loaded" << resource;
            }
            
            QByteArray bytes = progResource.readAll();
            progResource.close();
            
            return bytes;
        }
    }
}

#endif
