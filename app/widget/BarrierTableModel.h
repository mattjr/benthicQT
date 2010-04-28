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

#ifndef __BARRIER_TABLE_MODEL
#define __BARRIER_TABLE_MODEL

#include <QAbstractTableModel>
#include "BarrierSet.h"
#include "BQTDebug.h"

namespace ews {
    namespace app {
        namespace widget {
            using ews::app::model::BarrierSet;
            using ews::app::model::Barrier;
            using namespace ews::util::debug;
            
            /**
             * Mapping wrapper between BarrierSet and Barrier into Qt table and item models. 
             */
            class BarrierTableModel :  public QAbstractTableModel  {
                Q_OBJECT
            public:
                BarrierTableModel(BarrierSet* barriers) 
                : QAbstractTableModel(), _barriers(barriers) {
                    connect(barriers, SIGNAL(barrierAdded(int,Barrier*)), this, SLOT(rowsAdded(int)));
                    connect(barriers, SIGNAL(barrierRemoved(int,Barrier*)), this, SLOT(rowsRemoved(int)));
                }
                
                /** Standard dtor. */
                virtual ~BarrierTableModel() {
                    _barriers = NULL;
                }
                
                /** Enumeration mapping column symbol names to indexes. */
                enum ColType { ENABLED, NAME, NUM_SLITS, SLIT_WIDTH, SLIT_SEPARATION, NUM_COLS };
                
                virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {
                    return _barriers->size();
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
                        default:
                            flags |= Qt::ItemIsEditable;
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
                                return QString(tr("Show"));
                            case NAME:
                                return QString(tr("Name"));
                            default:
                                return QString("Col# %1").arg(section);
                        }
                    }
                    return QVariant();
                }
                
                virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
                    int i = index.row();
                    int j = index.column();
                    Barrier* b = _barriers->barrierAt(i);
                    
                    switch(role) {
                        case Qt::DisplayRole:
                        case Qt::EditRole:
                            switch(j) {
                                case NAME:
                                    return QString(b->objectName());
                                case NUM_SLITS:
                                    return QVariant(b->getNumSlits());
                                case SLIT_WIDTH:
                                    return QVariant(b->getSlitWidth());
                                case SLIT_SEPARATION:
                                    return QVariant(b->getSlitSeparation());
                                default:
                                    break;
                            }
                            break;
                        case Qt::CheckStateRole:
                            switch(j) {
                                case ENABLED:                                    
                                    return QVariant(b->isEnabled() ? Qt::Checked : Qt::Unchecked);
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
                    Barrier* b = _barriers->barrierAt(i);
                    
                    if (b != NULL && index.isValid() && value.isValid()) {
                        QString name;
                        if(role == Qt::EditRole ) {
                            switch(index.column()) {
                                case NAME:
                                    name = value.toString();
                                    if(name.size() > 0) {
                                        b->setObjectName(name);
                                    }
                                    break;
                                case NUM_SLITS:
                                    b->setNumSlits(static_cast<Barrier::NumSlits>(value.toInt()));
                                    break;
                                case SLIT_WIDTH:
                                    b->setSlitWidth(value.toUInt());
                                    break;
                                case SLIT_SEPARATION:
                                    b->setSlitSeparation(value.toUInt());
                                    break;
                            }
                        }
                        else if(role == Qt::CheckStateRole) {
                            switch(index.column()) {
                                case ENABLED:
                                    const bool enabled = (value.toInt() == Qt::Checked);
                                    b->setEnabled(enabled);
                                    break;
                            }                                    
                        }
                        qDebug() << value << "became" << *b;
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
                Q_DISABLE_COPY(BarrierTableModel);
                BarrierSet* _barriers;
                
            };
        }
    }
}

#endif // __BARRIER_TABLE_MODEL
