#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Mar 4 11:40:45 2021
#  Last Modified : <210304.1607>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
#*****************************************************************************


import Part, TechDraw, Spreadsheet, TechDrawGui
import FreeCADGui
from FreeCAD import Console
from FreeCAD import Base
import FreeCAD as App
import os
import sys
sys.path.append(os.path.dirname(__file__))

import datetime

import csv

from abc import ABCMeta, abstractmethod, abstractproperty

class HeatSinkCommon(object):
    __metaclass__ = ABCMeta
    _MHoleDiameter = 3.5
    _BoardEdgeMHOffset = 3.810
    _MHoleSpacingY = 52.070
    _BoardEdgeToDeviceMountPlane = 16.51
    _HSLength = 78.74
    _HSHeight = 25
    _HSDepth  = 25
    _Thickness = .05*25.4
    # Fan
    _FanThick = 10
    _FanMHoleDiameter = 2.8
    _FanMHoleCenters = 20
    _FanMTab = (25-20)+2
    _FanHoleDiameter = 24
    # Q13: TIP120 driving the fan
    _TO220_3_MinHoleHeight = 16.40
    _TO220_HoleDiameter = 3.8
    _Number6Shoulder_Diameter = 5.4
    _Q13CenterOffset = 16.51
    # U3: LMD18200 driver for OPS track power
    _TO220_11_MinHoleHeight = 17.78
    _U3CenterOffset = 35.56
    # U7: TMP36 heatsink temporature sensor
    _TO_92_HCenter = (4.83/2)
    _TO_92_BodyDiameter = 4.83
    _U7CenterOffset = 52.07
    # U4: L298N driver for prog track power and ALT H/L
    _TO220_15_MinHoleHeight = 17.65
    _U4CenterOffset = 67.31
    def TO220_Holes(self,xoff=0):
        xNorm = Base.Vector(1,0,0)
        xExtrude = Base.Vector(-self._Thickness,0,0)
        origin = self.origin
        q13_mh = origin.add(Base.Vector(xoff,self._Q13CenterOffset,\
                                        self._TO220_3_MinHoleHeight))
        self.cutfrom(Part.Face(Part.Wire(\
                Part.makeCircle(self._Number6Shoulder_Diameter/2,q13_mh,xNorm)))\
                .extrude(xExtrude))
        u3_mh = origin.add(Base.Vector(xoff,self._U3CenterOffset,\
                                        self._TO220_11_MinHoleHeight))
        self.cutfrom(Part.Face(Part.Wire(\
                Part.makeCircle(self._TO220_HoleDiameter/2,u3_mh,xNorm)))\
                .extrude(xExtrude))
        u4_mh = origin.add(Base.Vector(xoff,self._U4CenterOffset,\
                                        self._TO220_15_MinHoleHeight))
        self.cutfrom(Part.Face(Part.Wire(\
                Part.makeCircle(self._TO220_HoleDiameter/2,u4_mh,xNorm)))\
                .extrude(xExtrude))
    
class HeatSinkMainPanel(HeatSinkCommon):
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.85,.85,.85])
    def cutfrom(self,shape):
        self.body = self.body.cut(shape)
    def fuseto(self,shape):
        self.body = self.body.fuse(shape)
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        zExtrude = Base.Vector(0,0,self._Thickness)
        yExtrude = Base.Vector(0,-self._Thickness,0)
        xExtrude = Base.Vector(-self._Thickness,0,0)
        yNorm = Base.Vector(0,1,0)
        xNorm = Base.Vector(1,0,0)
        self.body = Part.makePlane(self._HSHeight,self._HSLength,\
                                    self.origin.add(Base.Vector(0,\
                                                        self._HSLength,\
                                                        self._Thickness)),\
                                    xNorm).extrude(xExtrude)
        self.TO220_Holes()
        bottom = Part.makePlane(self._HSDepth,self._HSLength,\
                self.origin.add(Base.Vector(-(self._HSDepth),0,0)))\
                .extrude(zExtrude)
        self.fuseto(bottom)
        top = Part.makePlane(self._HSDepth,self._HSLength,\
                self.origin.add(Base.Vector(-(self._HSDepth),0,self._HSHeight+self._Thickness)))\
                .extrude(zExtrude)
        self.fuseto(top)
        mh1 = origin.add(Base.Vector(-(self._BoardEdgeToDeviceMountPlane-\
                                        self._BoardEdgeMHOffset),\
                                     self._BoardEdgeMHOffset,0))
        mh2 = mh1.add(Base.Vector(0,self._MHoleSpacingY,0))
        self.cutfrom(Part.Face(Part.Wire(\
                Part.makeCircle(self._MHoleDiameter/2.0,mh1)))\
                .extrude(zExtrude))
        self.cutfrom(Part.Face(Part.Wire(\
                Part.makeCircle(self._MHoleDiameter/2.0,mh2)))\
                .extrude(zExtrude))
        fanmounttab = Part.makePlane(self._HSHeight,self._FanMTab+self._Thickness,\
                                     origin.add(Base.Vector(-(self._FanMTab+self._Thickness),\
                                                            0,self._Thickness)),yNorm)\
                                    .extrude(yExtrude)
        self.fuseto(fanmounttab)
        fanmX = -(self._Thickness + (self._HSDepth-self._FanMHoleCenters)/2.0)
        fanmZ1 = self._Thickness+(self._HSDepth-self._FanMHoleCenters)/2.0
        fanmZ2 = fanmZ1+self._FanMHoleCenters
        fanm1 = origin.add(Base.Vector(fanmX,0,fanmZ1))
        fanm2 = origin.add(Base.Vector(fanmX,0,fanmZ2))
        self.cutfrom(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm1,yNorm)))\
            .extrude(yExtrude))
        self.cutfrom(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm2,yNorm)))\
            .extrude(yExtrude))
        fanCX = -(self._Thickness + self._HSDepth/2.0)
        fanCZ = self._Thickness+self._HSDepth/2.0
        fanHole = origin.add(Base.Vector(fanCX,0,fanCZ))
        self.cutfrom(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanHoleDiameter/2.0,fanHole,yNorm)))\
            .extrude(yExtrude))

class HeatSinkFin1(HeatSinkCommon):
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.85,.85,.85])
    def cutfrom(self,shape):
        self.body = self.body.cut(shape)
    def fuseto(self,shape):
        self.body = self.body.fuse(shape)
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        zExtrude = Base.Vector(0,0,self._Thickness)
        yExtrude = Base.Vector(0,-self._Thickness,0)
        xExtrude = Base.Vector(-self._Thickness,0,0)
        yNorm = Base.Vector(0,1,0)
        xNorm = Base.Vector(1,0,0)
        self.body = \
            Part.makePlane(self._HSHeight*.6,\
                           self._HSLength,\
                           self.origin.add(Base.Vector(-self._Thickness,\
                                                       self._HSLength,\
                                                       self._Thickness+\
                                                        (self._HSHeight*.2))),\
                           xNorm).extrude(xExtrude)
        self.TO220_Holes(-self._Thickness)
        bottom = Part.makePlane(self._HSDepth-self._Thickness,self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth),0,self._HSHeight*.2))).extrude(zExtrude)        
        self.fuseto(bottom)
        top = Part.makePlane(self._HSDepth-self._Thickness,self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth),0,self._Thickness+self._HSHeight*.8))).extrude(zExtrude)
        self.fuseto(top)
            
class HeatSinkFin2(HeatSinkCommon):
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.85,.85,.85])
    def cutfrom(self,shape):
        self.body = self.body.cut(shape)
    def fuseto(self,shape):
        self.body = self.body.fuse(shape)
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        zExtrude = Base.Vector(0,0,self._Thickness)
        yExtrude = Base.Vector(0,-self._Thickness,0)
        xExtrude = Base.Vector(-self._Thickness,0,0)
        yNorm = Base.Vector(0,1,0)
        xNorm = Base.Vector(1,0,0)
        self.body = \
            Part.makePlane(self._HSHeight*.4,\
                           self._HSLength,\
                           self.origin.add(Base.Vector(-self._Thickness*2,\
                                                       self._HSLength,\
                                                       self._Thickness+\
                                                        (self._HSHeight*.4))),\
                           xNorm).extrude(xExtrude)
        self.TO220_Holes(-self._Thickness*2)
        bottom = Part.makePlane(self._HSDepth-(self._Thickness*2),self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth),0,self._HSHeight*.4))).extrude(zExtrude)        
        self.fuseto(bottom)
            
class HeatSink(HeatSinkCommon):
    def show(self):
        self.main.show()
        self.fin1.show()
        self.fin2.show()
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        self.main = HeatSinkMainPanel(name+"_main",self.origin)
        self.fin1 = HeatSinkFin1(name+"_fin1",self.origin)
        self.fin2 = HeatSinkFin2(name+"_fin1",self.origin)

if __name__ == '__main__':
    if "HeatSink" in App.listDocuments().keys():
        App.closeDocument("HeatSink")
    App.ActiveDocument=App.newDocument("HeatSink")
    doc = App.activeDocument()
    hs = HeatSink("heatsink",Base.Vector(0,0,0))
    hs.show()
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewIsometric()
