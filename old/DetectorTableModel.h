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

#ifndef __DETECTOR_TABLE_MODEL
#define __DETECTOR_TABLE_MODEL

#include <QAbstractTableModel>
#include "SimulationState.h"
#include "SamplerSet.h"
#include "EWSDebug.h"

namespace ews {
    namespace app {
        namespace widget {
            using ews::app::model::SimulationState;
            using ews::app::model::PointSampler;
            using ews::app::model::SamplerSet;
            using namespace ews::util::debug;
            
            /**
             * Mapping wrapper between SamplerSet and Barrier into Qt table and item models. 
             */
            class DetectorTableModel :  public QAbstractTableModel  {
                Q_OBJECT
            public:
                DetectorTableModel(SamplerSet* samplers) 
                : QAbstractTableModel(), _samplers(samplers) {
                    connect(samplers, SIGNAL(samplerAdded(int,PointSampler*)), this, SLOT(rowsAdded(int)));
                    connect(samplers, SIGNAL(samplerRemoved(int,PointSampler*)), this, SLOT(rowsRemoved(int)));
                }
                
                /** Standard dtor. */
                virtual ~DetectorTableModel() {
                    _samplers = NULL;
                }
                
                /** Enumeration mapping column symbol names to indexes. */
                enum ColType { ENABLED, NAME, COLOR, NUM_COLS };
                
                virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {
                    return _samplers->size();
                }
                
                virtual int columnCount(const QModelIndex &parent = QModelIndex()) const {
                    
                    return NUM_COLS;
                }
                
                /** Get flags on data. */
                virtual Qt::ItemFlags flags(const QModelIndex& index) const {
                    if (!index.isValid())
                        return Qt::ItemIsEnabled;
                    
                    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
                    
                    switch(index.column()) {
                        case ENABLED:
                            flags |= Qt::ItemIsUserCheckable;
                            break;
                        case NAME:
                            flags |= Qt::ItemIsEditable;
                            break;
                        default:
                            flags = Qt::ItemIsEnabled;
                            break;
                    }
                  
                    
                    return  flags;
                }
                 
                /**
                 * Get header names.
                 */
                virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const {
                    
                    if(orientation == Qt::Vertical) {
                        return QVariant(section + 1);
                    }
                    
                    if(role == Qt::DisplayRole) {
                        switch(section) {
                            case ENABLED:
                                return QString(tr("Enabled"));
                            case NAME:
                                return QString(tr("Name"));
                            case COLOR:
                                return QString(tr("Color"));
                            default:
                                return QString("Col# %1").arg(section);
                        }
                    }
                    return QVariant();
                }
                
                virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
                    int i = index.row();
                    int j = index.column();
                    PointSampler* s = _samplers->samplerAt(i);
                    
                    switch(role) {
                        case Qt::DisplayRole:
                        case Qt::EditRole:
                            switch(j) {
                                case NAME:
                                    return QString(s->objectName());
//                                case COLOR:
//                                    return s->getColor();
                                default:
                                    break;
                            }
                            break;
                            
                        case Qt::DecorationRole:
                            switch(j) {
                                case COLOR:
                                    return s->getColor();
                                default:
                                    break;
                            }
                            break;
                        case Qt::CheckStateRole:
                            switch(j) {
                                case ENABLED:                                    
                                    return QVariant(s->isEnabled() ? Qt::Checked : Qt::Unchecked);
                                default:
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                    
                    return QVariant();
                }

                
                virtual bool setData(const QModelIndex& index, const QVariant& value,
                                     int role = Qt::EditRole) {
                    int i = index.row();
                    PointSampler* s = _samplers->samplerAt(i);
                    
                    if (s != NULL && index.isValid() && value.isValid()) {
                        QString name;
                        if(role == Qt::EditRole ) {
                            switch(index.column()) {
                                case NAME:
                                    name = value.toString();
                                    if(name.size() > 0) {
                                        s->setObjectName(name);
                                    }
                                    break;
                            }
                        }
                        else if(role == Qt::CheckStateRole) {
                            switch(index.column()) {
                                case ENABLED:
                                    const bool enabled = (value.toInt() == Qt::Checked);
                                    s->setEnabled(enabled);
                                    break;
                            }                                    
                        }
                        emit dataChanged(index, index);
                        return true;
                    }
                    return false;
                }

            public slots:
                bool rowsAdded(int position) {
                    beginInsertRows(QModelIndex(), position, position);
                    // Rows already added in model.
                    endInsertRows();
                    return true;
                }
                bool rowsRemoved(int position) {
                    beginRemoveRows(QModelIndex(), position, position);
                    // Rows already removed in model.
                    endRemoveRows();
                    return true;
                }
                bool removeAll(int count) {
                    beginRemoveRows(QModelIndex(), 0, count - 1);
                    // Rows already removed in model.
                    endRemoveRows();
                    return true;
                }
                
            private:
                Q_DISABLE_COPY(DetectorTableModel);
                SamplerSet* _samplers;
                
            };
        }
    }
}

#endif // __DETECTOR_TABLE_MODEL
