/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
 */

#include "CameraController.h"
#include <osg/Quat>
#include <osg/Notify>
#include <osg/io_utils>
#include <osgUtil/LineSegmentIntersector>
#include "MathUtils.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            const Vec3 DEFAULT_OFFSET(200, 300, 200);
            const Vec3 UP(0, 1, 0);
            
            CameraController::CameraController() 
            : TerrainManipulator()
            {
                setMinimumDistance(MIN_ZOOM_DISTANCE);
                computeHomePosition();
            }
            
            
            CameraController::~CameraController() {
            }
            
            /** Compute the home position.*/        
            void CameraController::computeHomePosition() {
                if(getNode()) {
                    const BoundingSphere& boundingSphere = getNode()->getBound();
                    const Vec3& center = boundingSphere.center();

                    setHomePosition(center+DEFAULT_OFFSET, center, UP,
                                    getAutoComputeHomePosition());
                }
                else {
                    notify(NOTICE) << "Can't compute home position. No node assigned to camera" << std::endl;
                }
            }
            
            bool CameraController::handle(const GUIEventAdapter& ea, GUIActionAdapter& us) {
                if (ea.getHandled()) return false;
                
                if (ea.getEventType() == GUIEventAdapter::FRAME) return false;
                
                switch(ea.getEventType())
                {
                    case(GUIEventAdapter::PUSH):
                    case(GUIEventAdapter::RELEASE):
                    {
                        flushMouseEventStack();
                        addMouseEvent(ea);
                        if (calcMovement()) us.requestRedraw();
                        us.requestContinuousUpdate(false);
                        return true;
                    }
                        
                    case(GUIEventAdapter::DRAG):
                    {
                        addMouseEvent(ea);
                        if (calcMovement()) us.requestRedraw();
                        us.requestContinuousUpdate(false);
                        return true;
                    }
                        
                    case(GUIEventAdapter::MOVE):
                    {
                        return false;
                    }
                        
                    case(GUIEventAdapter::KEYDOWN):
                        if (ea.getKey()== GUIEventAdapter::KEY_Space) {
                            flushMouseEventStack();
                            home(ea,us);
                            us.requestRedraw();
                            us.requestContinuousUpdate(false);
                            return true;
                        }
                        return false;
                    default:
                        return false;
                }
            }

            void CameraController::computePosition(const osg::Vec3d& eye,const osg::Vec3d& center,const osg::Vec3d& up) {
                if (!_node) return;
                
                // compute rotation matrix
                osg::Vec3d lv(center - eye);
                _distance = lv.length();
                _center = center;
                
                osg::notify(osg::INFO) << "In compute"<< std::endl;
                
                if (_node.valid()) {
                    bool hitFound = false;
                    
                    const double distance = lv.length();
                    const double maxDistance = distance + 2 * (eye - _node->getBound().center()).length();
                    const osg::Vec3d farPosition = eye + lv * (maxDistance / distance);
                    osg::Vec3d endPoint = center;
                    for (int i = 0; !hitFound && i < 2; ++i, endPoint = farPosition) {
                        // compute the intersection with the scene.
                        osg::Vec3d ip;
                        if (intersect(eye, endPoint, ip)) {
                            _center = ip;
                            _distance = (ip - eye).length();
                            hitFound = true;
                        }
                    }
                }
                
                // note LookAt = inv(CF)*inv(RM)*inv(T) which is equivalent to:
                // inv(R) = CF*LookAt.
                
                osg::Matrixd rotation_matrix = osg::Matrixd::lookAt(eye, center, up);
                
                _rotation = rotation_matrix.getRotate().inverse();
                
                CoordinateFrame coordinateFrame = getCoordinateFrame(_center);
                _previousUp = getUpVector(coordinateFrame);
                clampOrientation();
            }
            
            void CameraController::doRotate(double dx, double dy) {
                // rotate camera.
                osg::Vec3 axis;
                double angle;
                
                double px0 = _ga_t0->getXnormalized();
                double py0 = _ga_t0->getYnormalized();
                
                double px1 = _ga_t1->getXnormalized();
                double py1 = _ga_t1->getYnormalized();
                
                trackball(axis, angle, px1, py1, px0, py0);
                
                osg::Quat new_rotate;
                new_rotate.makeRotate(angle, axis);
                
                _rotation = _rotation * new_rotate;
                clampOrientation();
            }
            
            void CameraController::doZoom(double dx, double dy) {
                using namespace ews::util;
                // zoom model.
                const double scale = 1.0f + dy;
                _distance = imposeBounds(getMinimumDistance(), _distance * scale, MAX_ZOOM_DISTANCE);
            }
            
            void CameraController::doPan(double dx, double dy) {
                // pan model.
                double scale = -0.3f*_distance;
                
                osg::Matrixd rotation_matrix;
                rotation_matrix.makeRotate(_rotation);
                
                // compute look vector.
                osg::Vec3d lookVector = -getUpVector(rotation_matrix);
                osg::Vec3d sideVector = getSideVector(rotation_matrix);
                osg::Vec3d upVector = getFrontVector(rotation_matrix);
                
                // CoordinateFrame coordinateFrame = getCoordinateFrame(_center);
                // osg::Vec3d localUp = getUpVector(coordinateFrame);
                osg::Vec3d localUp = _previousUp;
                
                osg::Vec3d forwardVector =localUp^sideVector;
                sideVector = forwardVector^localUp;
                
                forwardVector.normalize();
                sideVector.normalize();
                
                osg::Vec3d dv = forwardVector * (dy*scale) + sideVector * (dx*scale);
                
                _center += dv;
                
                // need to recompute the intersection point along the look vector.
                
                bool hitFound = false;
                
                if (_node.valid()) {
                    
                    // now reorientate the coordinate frame to the frame coords.
                    CoordinateFrame coordinateFrame =  getCoordinateFrame(_center);
                    
                    // need to reintersect with the terrain
                    double distance = _node->getBound().radius()*0.25f;
                    
                    osg::Vec3d ip1;
                    osg::Vec3d ip2;
                    bool hit_ip1 = intersect(_center, _center + getUpVector(coordinateFrame) * distance, ip1);
                    bool hit_ip2 = intersect(_center, _center - getUpVector(coordinateFrame) * distance, ip2);
                    if (hit_ip1)  {
                        if (hit_ip2) {
                            _center = (_center-ip1).length2() < (_center-ip2).length2() ?
                            ip1 :
                            ip2;
                            
                            hitFound = true;
                        }
                        else {
                            _center = ip1;
                            hitFound = true;
                        }
                    }
                    else if (hit_ip2) {
                        _center = ip2;
                        hitFound = true;
                    }
                    
                    if (!hitFound) {
                        // ??
                        osg::notify(INFO)<<"CameraController unable to intersect"<<std::endl;
                    }
                    
                    coordinateFrame = getCoordinateFrame(_center);
                    osg::Vec3d new_localUp = getUpVector(coordinateFrame);
                    
                    osg::Quat pan_rotation;
                    pan_rotation.makeRotate(localUp,new_localUp);
                    
                    if (!pan_rotation.zeroRotation()) {
                        _rotation = _rotation * pan_rotation;
                        _previousUp = new_localUp;
//                        osg::notify(osg::NOTICE)<<"Rotating from "<<localUp<<" to "<<new_localUp<<"  angle = "<<acos(localUp*new_localUp/(localUp.length()*new_localUp.length()))<<std::endl;
//                        clampOrientation();
                    }
                    else {
                        osg::notify(osg::INFO)<<"New up orientation nearly inline - no need to rotate"<<std::endl;
                    }
                }
            }
            
            bool CameraController::calcMovement() {
                // return if less then two events have been added.
                if (_ga_t0.get() == NULL || _ga_t1.get() == NULL) return false;
                
                double dx = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
                double dy = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();
                
                
                // return if there is no movement.
                if (dx==0 && dy==0) return false;
                
                unsigned int buttonMask = _ga_t1->getButtonMask();
                if (buttonMask==GUIEventAdapter::LEFT_MOUSE_BUTTON) {
                    doRotate(dx, dy);
                    return true;
                }
                else if (buttonMask==GUIEventAdapter::MIDDLE_MOUSE_BUTTON ||
                         buttonMask==(GUIEventAdapter::LEFT_MOUSE_BUTTON|GUIEventAdapter::RIGHT_MOUSE_BUTTON)) {
                    doPan(dx, dy);
                    return true;
                }
                else if (buttonMask==GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
                    doZoom(dx, dy);
                    return true;   
                }
                
                return false;
            }
            
            void CameraController::clampOrientation() {
                osg::Matrixd rotation_matrix;
                rotation_matrix.makeRotate(_rotation);
                
                osg::Vec3d lookVector = -getUpVector(rotation_matrix);
                osg::Vec3d upVector = getFrontVector(rotation_matrix);

                // Don't let it rotate past the X-Y plane
                if (lookVector.z() > 0) {
                    Vec3d newLookVector(lookVector.x(), lookVector.y(), 0);
                    newLookVector.normalize();
                    osg::Quat keepNegative;
                    keepNegative.makeRotate(lookVector, newLookVector);
                    _rotation = _rotation * keepNegative;
                    rotation_matrix.makeRotate(_rotation);
                    lookVector = -getUpVector(rotation_matrix);
                    upVector = getFrontVector(rotation_matrix);
                }
                // Don't let it rotate beyond the Z-axis
                if (upVector.z() < 0) {
                    Vec3d newUpVector(upVector.x(), upVector.y(), 0);
                    newUpVector.normalize();
                    osg::Quat keepPositive;
                    keepPositive.makeRotate(upVector, newUpVector);
                    _rotation = _rotation * keepPositive;
                    rotation_matrix.makeRotate(_rotation);
                    lookVector = -getUpVector(rotation_matrix);
                    upVector = getFrontVector(rotation_matrix);
                }
                
                CoordinateFrame coordinateFrame = getCoordinateFrame(_center);
                osg::Vec3d localUp = getUpVector(coordinateFrame);
                //osg::Vec3d localUp = _previousUp;
                
                osg::Vec3d sideVector = lookVector ^ localUp;
                
                if (sideVector.length()<0.1) {
                    osg::notify(osg::INFO)<<"Side vector short "<<sideVector.length()<<std::endl;
                    
                    sideVector = upVector ^ localUp;
                    sideVector.normalize();
                    
                }
                    
                Vec3d newUpVector = sideVector ^ lookVector;
                newUpVector.normalize();
                osg::Quat rotate_roll;
                rotate_roll.makeRotate(upVector, newUpVector);
                if (!rotate_roll.zeroRotation()) {
                    _rotation = _rotation * rotate_roll;
                }
            }
        }
    }
}
