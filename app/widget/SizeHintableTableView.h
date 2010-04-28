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
 */

#ifndef __SIZE_HINTABLE_TABLE_VIEW_H
#define __SIZE_HINTABLE_TABLE_VIEW_H

#include <QTableView>

namespace ews {
    namespace app {
        namespace widget {
            
            /** Simple specialization of QTableView to allow overriding of the
             * default implementation of sizeHint. 
             */
            class SizeHintableTableView : public QTableView {
                Q_OBJECT
                Q_PROPERTY(QSize sizeHintOverride READ sizeHint WRITE setSizeHintOverride)
                
            public:
                
                SizeHintableTableView(QWidget* parent)
                : QTableView(parent), _hint() {
                    _hint = QTableView::sizeHint();
                }
                
                virtual void setSizeHintOverride(const QSize& size) {
                    _hint = size;
                }
                
                virtual QSize sizeHint() const {
                    return _hint;
                }
                
            private:
                QSize _hint;
            };
        }
    }
}

#endif //__SIZE_HINTABLE_TABLE_VIEW_H

