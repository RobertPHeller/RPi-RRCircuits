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
#  Last Modified : <210308.1105>
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
    _FanTabExtension = .25*25.4
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
    def cutfrom3D(self,shape):
        self.body = self.body.cut(shape)
    def fuseto3D(self,shape):
        self.body = self.body.fuse(shape)
    def cutfrom2D(self,shape):
        self.sheet = self.sheet.cut(shape)
    def fuseto2D(self,shape):
        self.sheet = self.sheet.fuse(shape)
    def show3D(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.85,.85,.85])
    def show2D(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.sheet
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.85,.85,.85])
    def TO220_Holes3D(self,xoff=0):
        xNorm = Base.Vector(1,0,0)
        xExtrude = Base.Vector(-self._Thickness,0,0)
        origin = self.origin
        q13_mh = origin.add(Base.Vector(xoff,self._Q13CenterOffset,\
                                        self._TO220_3_MinHoleHeight))
        self.cutfrom3D(Part.Face(Part.Wire(\
                Part.makeCircle(self._Number6Shoulder_Diameter/2,q13_mh,xNorm)))\
                .extrude(xExtrude))
        u3_mh = origin.add(Base.Vector(xoff,self._U3CenterOffset,\
                                        self._TO220_11_MinHoleHeight))
        self.cutfrom3D(Part.Face(Part.Wire(\
                Part.makeCircle(self._TO220_HoleDiameter/2,u3_mh,xNorm)))\
                .extrude(xExtrude))
        u4_mh = origin.add(Base.Vector(xoff,self._U4CenterOffset,\
                                        self._TO220_15_MinHoleHeight))
        self.cutfrom3D(Part.Face(Part.Wire(\
                Part.makeCircle(self._TO220_HoleDiameter/2,u4_mh,xNorm)))\
                .extrude(xExtrude))
    def TO220_Holes2D(self,origin):
        zExtrude = Base.Vector(0,0,self._Thickness)
        q13_mh = origin.add(Base.Vector(self._TO220_3_MinHoleHeight-self._Thickness,self._Q13CenterOffset,0))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._Number6Shoulder_Diameter/2,q13_mh)))\
            .extrude(zExtrude))
        u3_mh = origin.add(Base.Vector(self._TO220_11_MinHoleHeight-self._Thickness,self._U3CenterOffset,0))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._TO220_HoleDiameter/2,u3_mh)))\
            .extrude(zExtrude))
        u4_mh = origin.add(Base.Vector(self._TO220_15_MinHoleHeight-self._Thickness,self._U4CenterOffset,0))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._TO220_HoleDiameter/2,u4_mh)))\
            .extrude(zExtrude))

class HeatSinkMainPanel(HeatSinkCommon):
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
        self.body = Part.makePlane(self._HSHeight,self._HSLength+self._FanTabExtension,\
                                    self.origin.add(Base.Vector(0,\
                                                        self._HSLength,\
                                                        self._Thickness)),\
                                    xNorm).extrude(xExtrude)
    
        self.sheet = Part.makePlane(self._HSHeight,self._HSLength+self._FanTabExtension,\
                                    self.origin.add(Base.Vector(0,-self._FanTabExtension,0)))\
                                    .extrude(zExtrude)
        self.TO220_Holes3D()
        self.TO220_Holes2D(origin)
        bottom = Part.makePlane(self._HSDepth+self._Thickness,self._HSLength,\
                self.origin.add(Base.Vector(-(self._HSDepth+self._Thickness),0,0)))\
                .extrude(zExtrude)
        self.fuseto3D(bottom)
        bottom = Part.makePlane(self._HSDepth+self._Thickness,self._HSLength,\
                                self.origin.add(\
                                    Base.Vector(-(self._HSHeight+self._Thickness),0,0)))\
                                 .extrude(zExtrude)
        self.fuseto2D(bottom)
        mh1 = origin.add(Base.Vector(-(self._BoardEdgeToDeviceMountPlane-\
                                        self._BoardEdgeMHOffset),\
                                     self._BoardEdgeMHOffset,0))
        mh2 = mh1.add(Base.Vector(0,self._MHoleSpacingY,0))
        mh1_hole = Part.Face(Part.Wire(\
                Part.makeCircle(self._MHoleDiameter/2.0,mh1)))\
                .extrude(zExtrude)
        self.cutfrom3D(mh1_hole)
        mh1 = mh1.add(Base.Vector(-self._Thickness,0,0))
        mh1_hole = Part.Face(Part.Wire(\
                Part.makeCircle(self._MHoleDiameter/2.0,mh1)))\
                .extrude(zExtrude)
        self.cutfrom2D(mh1_hole)
        mh2 = mh2.add(Base.Vector(-self._Thickness,0,0))
        mh2_hole = Part.Face(Part.Wire(\
                        Part.makeCircle(self._MHoleDiameter/2.0,mh2)))\
                        .extrude(zExtrude)
        self.cutfrom3D(mh2_hole)
        mh2.add(Base.Vector(-self._Thickness,0,0))
        mh2_hole = Part.Face(Part.Wire(\
                        Part.makeCircle(self._MHoleDiameter/2.0,mh2)))\
                        .extrude(zExtrude)
        self.cutfrom2D(mh2_hole)
        top = Part.makePlane(self._HSDepth+self._Thickness,self._HSLength,\
                self.origin.add(Base.Vector(-(self._HSDepth+self._Thickness),\
                                            0,self._HSHeight+self._Thickness)))\
                .extrude(zExtrude)
        self.fuseto3D(top)
        top = Part.makePlane(self._HSDepth+self._Thickness,self._HSLength,\
                             self.origin.add(\
                                    Base.Vector(self._HSHeight,0,0)))\
                             .extrude(zExtrude)
        self.fuseto2D(top)
        fanmountpanel = Part.makePlane(self._HSHeight+self._Thickness*2,\
                                       self._HSDepth+self._Thickness*2,\
                                       origin.add(Base.Vector(-(self._HSHeight+self._Thickness*2),-self._FanTabExtension,0)),
                                       yNorm).extrude(yExtrude)
        self.fuseto3D(fanmountpanel)
        fanmountpaneltop = Part.makePlane(self._HSDepth+self._Thickness*2,\
                                          self._FanTabExtension*2+self._Thickness*1,\
                                           origin.add(Base.Vector(-(self._HSDepth+self._Thickness*2),-(self._FanTabExtension+self._Thickness),self._HSHeight+self._Thickness*2)))\
                             .extrude(zExtrude)
        self.fuseto3D(fanmountpaneltop)
        fanmX = -(self._Thickness + (self._HSDepth-self._FanMHoleCenters)/2.0)
        fanmZ1 = self._Thickness+(self._HSDepth-self._FanMHoleCenters)/2.0
        fanmZ2 = fanmZ1+self._FanMHoleCenters
        fanm1 = origin.add(Base.Vector(fanmX,-self._FanTabExtension,fanmZ1))
        fanm2 = origin.add(Base.Vector(fanmX,-self._FanTabExtension,fanmZ2))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm1,yNorm)))\
            .extrude(yExtrude))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm2,yNorm)))\
            .extrude(yExtrude))
        fanm1 = fanm1.add(Base.Vector(-self._FanMHoleCenters,0,0))
        fanm2 = fanm2.add(Base.Vector(-self._FanMHoleCenters,0,0))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm1,yNorm)))\
            .extrude(yExtrude))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm2,yNorm)))\
            .extrude(yExtrude))
        
        fanCX = -(self._Thickness + self._HSDepth/2.0)
        fanCZ = self._Thickness+self._HSDepth/2.0
        fanHole = origin.add(Base.Vector(fanCX,-self._FanTabExtension,fanCZ))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanHoleDiameter/2.0,fanHole,yNorm)))\
            .extrude(yExtrude))
        fanmountpanel = Part.makePlane(self._HSHeight+self._Thickness*2,\
                                       self._HSDepth+self._Thickness*2,\
                                       origin.add(Base.Vector(-self._Thickness,\
                                                              -(self._FanTabExtension+self._HSDepth+self._Thickness*2),0)))\
                                      .extrude(zExtrude)
        self.fuseto2D(fanmountpanel)
        fanmountpaneltop = Part.makePlane(self._FanTabExtension*2+self._Thickness*1,\
                                          self._HSDepth+self._Thickness*2,\
                                          origin.add(Base.Vector(+(self._HSDepth+self._Thickness*1),\
                                            -(self._FanTabExtension+self._HSDepth+self._Thickness*2),0)))\
                                      .extrude(zExtrude)
        self.fuseto2D(fanmountpaneltop)
        fanmY = -((self._HSDepth-self._FanMHoleCenters)/2.0+self._FanTabExtension+self._Thickness)
        fanmX1 = (self._HSDepth-self._FanMHoleCenters)/2.0
        fanmX2 = fanmX1+self._FanMHoleCenters
        fanm1 = origin.add(Base.Vector(fanmX1,fanmY,0))
        fanm2 = origin.add(Base.Vector(fanmX2,fanmY,0))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm1)))\
            .extrude(zExtrude))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm2)))\
            .extrude(zExtrude))
        fanm1 = fanm1.add(Base.Vector(0,-self._FanMHoleCenters,0))
        fanm2 = fanm2.add(Base.Vector(0,-self._FanMHoleCenters,0))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm1)))\
            .extrude(zExtrude))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm2)))\
            .extrude(zExtrude))
        fanCY = -((self._HSDepth/2.0)+self._FanTabExtension+self._Thickness)
        fanCX = (self._HSDepth/2.0)
        fanHole = origin.add(Base.Vector(fanCX,fanCY,0))
        self.cutfrom2D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanHoleDiameter/2.0,fanHole)))\
            .extrude(zExtrude))

class HeatSinkFin1(HeatSinkCommon):
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
        self.TO220_Holes3D(-self._Thickness)
        self.sheetorigin = origin.add(Base.Vector(0,0,25.4))
        self.sheet = \
            Part.makePlane(self._HSHeight*.6,\
                           self._HSLength,\
                           self.sheetorigin.add(Base.Vector(self._HSHeight*.2,0,0)))\
                           .extrude(zExtrude)
        self.TO220_Holes2D(self.sheetorigin)
        bottom = Part.makePlane(self._HSDepth,self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth+self._Thickness),0,self._HSHeight*.2))).extrude(zExtrude)        
        self.fuseto3D(bottom)
        bottom = Part.makePlane(self._HSDepth,self._HSLength,\
                                self.sheetorigin.add(Base.Vector(-(self._HSDepth)+self._HSHeight*.2,0,0))).extrude(zExtrude)
        self.fuseto2D(bottom)
        top = Part.makePlane(self._HSDepth,self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth+self._Thickness),0,self._Thickness+self._HSHeight*.8))).extrude(zExtrude)
        self.fuseto3D(top)
        top = Part.makePlane(self._HSDepth,self._HSLength,\
                                self.sheetorigin.add(Base.Vector(self._HSHeight*.8,0,0)))\
                                .extrude(zExtrude)
        self.fuseto2D(top)
            
class HeatSinkFin2(HeatSinkCommon):
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
        self.TO220_Holes3D(-self._Thickness*2)
        self.sheetorigin = origin.add(Base.Vector(0,0,50.8))
        self.sheet = \
            Part.makePlane(self._HSHeight*.4,\
                           self._HSLength,\
                           self.sheetorigin.add(Base.Vector(self._HSHeight*.4,0,0)))\
                           .extrude(zExtrude)
        self.TO220_Holes2D(self.sheetorigin)
        bottom = Part.makePlane(self._HSDepth-(self._Thickness*1),self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth+self._Thickness),0,self._HSHeight*.4))).extrude(zExtrude)        
        self.fuseto3D(bottom)
        bottom = Part.makePlane(self._HSDepth-(self._Thickness*1),self._HSLength,\
                                self.sheetorigin.add(Base.Vector(-(self._HSDepth-self._Thickness*1)+self._HSHeight*.4,0,0)))\
                                .extrude(zExtrude)
        self.fuseto2D(bottom)
            
class HeatSink(HeatSinkCommon):
    def show3D(self):
        self.main.show3D()
        self.fin1.show3D()
        self.fin2.show3D()
    def show2D(self):
        self.main.show2D()
        self.fin1.show2D()
        self.fin2.show2D()
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        self.main = HeatSinkMainPanel(name+"_main",self.origin)
        self.fin1 = HeatSinkFin1(name+"_fin1",self.origin)
        self.fin2 = HeatSinkFin2(name+"_fin1",self.origin)

if __name__ == '__main__':
    if "HeatSink2D" in App.listDocuments().keys():
        App.closeDocument("HeatSink2D")
    if "HeatSink3D" in App.listDocuments().keys():
        App.closeDocument("HeatSink3D")
    if "HeatSinkMainPanelPage" in App.listDocuments().keys():
        App.closeDocument("HeatSinkMainPanelPage")
    if "HeatSinkFin1PanelPage" in App.listDocuments().keys():
        App.closeDocument("HeatSinkFin1PanelPage")
    App.ActiveDocument=App.newDocument("HeatSink3D")
    doc = App.activeDocument()
    hs = HeatSink("heatsink",Base.Vector(0,0,0))
    hs.show3D()
    doc3d = doc
    Gui.SendMsgToActiveView("ViewFit")
    App.ActiveDocument=App.newDocument("HeatSink2D")
    doc = App.activeDocument()
    hs.show2D()
    doc2d = doc
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewIsometric()
    Gui.activeDocument().activeView().viewIsometric()
    doc.addObject('TechDraw::DrawSVGTemplate','USLetterTemplate')
    doc.USLetterTemplate.Template = App.getResourceDir()+"Mod/TechDraw/Templates/USLetter_Landscape.svg"
    edt = doc.USLetterTemplate.EditableTexts
    doc.addObject('TechDraw::DrawPage','HeatSinkMainPanelPage')
    doc.HeatSinkMainPanelPage.Template = doc.USLetterTemplate
    edt = doc.HeatSinkMainPanelPage.Template.EditableTexts
    edt['CompanyName'] = "Deepwoods Software"
    edt['CompanyAddress'] = '51 Locke Hill Road, Wendell, MA 01379 USA'
    edt['DrawingTitle1']= 'Heat Sink Main Panel'
    edt['DrawingTitle3']= ""
    edt['DrawnBy'] = "Robert Heller"
    edt['CheckedBy'] = ""
    edt['Approved1'] = ""
    edt['Approved2'] = ""
    edt['Code'] = ""
    edt['Weight'] = ''
    edt['DrawingNumber'] = datetime.datetime.now().ctime()
    edt['Revision'] = "A"
    edt['DrawingTitle2']= ""
    edt['Scale'] = 'Scale: .75'
    edt['Sheet'] = "Sheet 1 of 3"
    doc.HeatSinkMainPanelPage.Template.EditableTexts = edt
    doc.HeatSinkMainPanelPage.ViewObject.show()
    mainsheet = doc.addObject('Spreadsheet::Sheet','MainDimensionTable')
    mainsheet.set("A1",'%-11.11s'%"Dim")
    mainsheet.set("B1",'%10.10s'%"inch")
    mainsheet.set("C1",'%10.10s'%"mm")
    ir = 2
    doc.addObject('TechDraw::DrawViewPart','MainTopView')
    doc.HeatSinkMainPanelPage.addView(doc.MainTopView)
    doc.MainTopView.Source = doc.heatsink_main
    doc.MainTopView.X = 200
    doc.MainTopView.Y = 130
    doc.MainTopView.Direction=(0.0,0.0,1.0)
    doc.MainTopView.Scale=.75
    # Vertex37: upper left corner
    # Vertex45: upper right corner
    # Vertex36:  lower left corner
    # Vertex44: lower right corner
    #
    # Vertex24: upper right bottom corner
    # Vertex25: upper left  top corner
    #
    # Vertex6:  upper left fan mount panel
    # Vertex7:  upper right fan mount panel
    # Vertex2:  lower left fan mount panel
    # Vertex0:  lower right fan mount panel
    #
    # Vertex3:  upper right fan mount panel top
    # Vertex1:  lower right fan mount panel top
    #
    doc.addObject('TechDraw::DrawViewDimension','MainLength')
    doc.MainLength.Type = 'DistanceY'
    doc.MainLength.References2D=[(doc.MainTopView,'Vertex37'),\
                                 (doc.MainTopView,'Vertex36')]
    doc.MainLength.FormatSpec='l'
    doc.MainLength.Arbitrary = True
    doc.MainLength.X = -50
    doc.MainLength.Y = 0
    doc.HeatSinkMainPanelPage.addView(doc.MainLength)
    mainsheet.set("A%d"%ir,'%-11.11s'%"l")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._HSLength/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._HSLength)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainSheetTotalLength')
    doc.MainSheetTotalLength.Type = 'DistanceY'
    doc.MainSheetTotalLength.References2D=[(doc.MainTopView,'Vertex37'),\
                                       (doc.MainTopView,'Vertex2')]
    doc.MainSheetTotalLength.FormatSpec='tl'
    doc.MainSheetTotalLength.Arbitrary = True
    doc.MainSheetTotalLength.X = -60
    doc.MainSheetTotalLength.Y = 0
    doc.HeatSinkMainPanelPage.addView(doc.MainSheetTotalLength)
    mainsheet.set("A%d"%ir,'%-11.11s'%"tl")
    tl = HeatSinkCommon._HSLength+HeatSinkCommon._FanTabExtension+\
            HeatSinkCommon._HSDepth+\
            HeatSinkCommon._Thickness
    mainsheet.set("B%d"%ir,'%10.6f'%(tl/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%tl)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainSheetTotalWidth')
    doc.MainSheetTotalWidth.Type = 'DistanceX'
    doc.MainSheetTotalWidth.References2D=[(doc.MainTopView,'Vertex37'),\
                                     (doc.MainTopView,'Vertex45')]
    doc.MainSheetTotalWidth.FormatSpec='tw'
    doc.MainSheetTotalWidth.Arbitrary = True
    doc.MainSheetTotalWidth.X = 0
    doc.MainSheetTotalWidth.Y = 65
    doc.HeatSinkMainPanelPage.addView(doc.MainSheetTotalWidth)
    mainsheet.set("A%d"%ir,'%-11.11s'%"tw")
    tw = HeatSinkCommon._HSHeight+\
             ((HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness)*2)
    mainsheet.set("B%d"%ir,'%10.6f'%(tw/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%tw)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainBottomWidth')
    doc.MainBottomWidth.Type = 'DistanceX'
    doc.MainBottomWidth.References2D=[(doc.MainTopView,'Vertex37'),\
                                      (doc.MainTopView,'Vertex24')]
    doc.MainBottomWidth.FormatSpec='b'
    doc.MainBottomWidth.Arbitrary = True
    doc.MainBottomWidth.X = -25
    doc.MainBottomWidth.Y = 55
    doc.HeatSinkMainPanelPage.addView(doc.MainBottomWidth)
    mainsheet.set("A%d"%ir,'%-11.11s'%"b")
    b = HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness
    mainsheet.set("B%d"%ir,'%10.6f'%(b/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%b)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainWidth')
    doc.MainWidth.Type = 'DistanceX'
    doc.MainWidth.References2D=[(doc.MainTopView,'Vertex24'),\
                                 (doc.MainTopView,'Vertex25')]
    doc.MainWidth.FormatSpec='w'
    doc.MainWidth.Arbitrary = True
    doc.MainWidth.X = 0
    doc.MainWidth.Y = 55
    doc.HeatSinkMainPanelPage.addView(doc.MainWidth)
    mainsheet.set("A%d"%ir,'%-11.11s'%"w")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._HSHeight/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._HSHeight)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainTopWidth')
    doc.MainTopWidth.Type = 'DistanceX'
    doc.MainTopWidth.References2D=[(doc.MainTopView,'Vertex25'),\
                                 (doc.MainTopView,'Vertex44')]
    doc.MainTopWidth.FormatSpec='t'
    doc.MainTopWidth.Arbitrary = True
    doc.MainTopWidth.X = 25
    doc.MainTopWidth.Y = 55
    doc.HeatSinkMainPanelPage.addView(doc.MainTopWidth)
    mainsheet.set("A%d"%ir,'%-11.11s'%"t")
    mainsheet.set("B%d"%ir,'%10.6f'%(b/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%b)
    ir += 1
    # 
    # Edge26: mh2 circ
    # Vertex43: mh2 center
    # Edge25: mh1 circ
    # Vertex40: mh1 center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainMountHoleDiameter')
    doc.MainMountHoleDiameter.Type = 'Diameter'
    doc.MainMountHoleDiameter.References2D=[(doc.MainTopView,'Edge26')]
    doc.MainMountHoleDiameter.FormatSpec='mhDia (2x)'
    doc.MainMountHoleDiameter.Arbitrary = True
    doc.MainMountHoleDiameter.X =   5
    doc.MainMountHoleDiameter.Y =  30
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mhDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._MHoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._MHoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainMountHoleXOffset')
    doc.MainMountHoleXOffset.Type = 'DistanceX'
    doc.MainMountHoleXOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                       (doc.MainTopView,'Vertex40')]
    doc.MainMountHoleXOffset.FormatSpec='mhx'
    doc.MainMountHoleXOffset.Arbitrary = True
    doc.MainMountHoleXOffset.X = -40
    doc.MainMountHoleXOffset.Y =  29
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHoleXOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mhx")
    mhx = HeatSinkCommon._HSDepth-\
        (HeatSinkCommon._BoardEdgeToDeviceMountPlane+HeatSinkCommon._Thickness)
    mainsheet.set("B%d"%ir,'%10.6f'%(mhx/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%mhx)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainMountHole1YOffset')
    doc.MainMountHole1YOffset.Type = 'DistanceY'
    doc.MainMountHole1YOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                        (doc.MainTopView,'Vertex40')]
    doc.MainMountHole1YOffset.FormatSpec='mh1y'
    doc.MainMountHole1YOffset.Arbitrary = True
    doc.MainMountHole1YOffset.X = -41
    doc.MainMountHole1YOffset.Y = -28
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHole1YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mh1y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._BoardEdgeMHOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._BoardEdgeMHOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainMountHole1_2YOffset')
    doc.MainMountHole1_2YOffset.Type = 'DistanceY'
    doc.MainMountHole1_2YOffset.References2D=[(doc.MainTopView,'Vertex43'),\
                                        (doc.MainTopView,'Vertex40')]
    doc.MainMountHole1_2YOffset.FormatSpec='mh12y'
    doc.MainMountHole1_2YOffset.Arbitrary = True
    doc.MainMountHole1_2YOffset.X = -38
    doc.MainMountHole1_2YOffset.Y =   0
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHole1_2YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mh12y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._MHoleSpacingY/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._MHoleSpacingY)
    ir += 1
    #
    # Edge21: u4_mh circ
    # Vertex35: u4_mh center
    # Edge20: u3_mh circ
    # Vertex32: u3_mh center
    # Edge19: Q13_mh circ
    # Vertex29: Q13_mh center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainTO220_11_15_HoleDiameter')
    doc.MainTO220_11_15_HoleDiameter.Type = 'Diameter'
    doc.MainTO220_11_15_HoleDiameter.References2D=[(doc.MainTopView,'Edge21')]
    doc.MainTO220_11_15_HoleDiameter.FormatSpec='ICMhDia (2x)'
    doc.MainTO220_11_15_HoleDiameter.Arbitrary = True
    doc.MainTO220_11_15_HoleDiameter.X =  44
    doc.MainTO220_11_15_HoleDiameter.Y =  39
    doc.HeatSinkMainPanelPage.addView(doc.MainTO220_11_15_HoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"ICMhDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._TO220_HoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._TO220_HoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainShoulderWasherHoleDiameter')
    doc.MainShoulderWasherHoleDiameter.Type = 'Diameter'
    doc.MainShoulderWasherHoleDiameter.References2D=[(doc.MainTopView,'Edge19')]
    doc.MainShoulderWasherHoleDiameter.FormatSpec='Q13Mh'
    doc.MainShoulderWasherHoleDiameter.Arbitrary = True
    doc.MainShoulderWasherHoleDiameter.X = 45
    doc.MainShoulderWasherHoleDiameter.Y =  1
    doc.HeatSinkMainPanelPage.addView(doc.MainShoulderWasherHoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"Q13Mh")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._Number6Shoulder_Diameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._Number6Shoulder_Diameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainQ13XOffset')
    doc.MainQ13XOffset.Type = 'DistanceX'
    doc.MainQ13XOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                 (doc.MainTopView,'Vertex29')]
    doc.MainQ13XOffset.FormatSpec='Q13x'
    doc.MainQ13XOffset.Arbitrary = True
    doc.MainQ13XOffset.X =  -3
    doc.MainQ13XOffset.Y =   5
    doc.HeatSinkMainPanelPage.addView(doc.MainQ13XOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"Q13x")
    Q13x = b+HeatSinkCommon._Q13CenterOffset
    mainsheet.set("B%d"%ir,'%10.6f'%(Q13x/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%Q13x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU3XOffset')
    doc.MainU3XOffset.Type = 'DistanceX'
    doc.MainU3XOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                 (doc.MainTopView,'Vertex32')]
    doc.MainU3XOffset.FormatSpec='U3x'
    doc.MainU3XOffset.Arbitrary = True
    doc.MainU3XOffset.X = -2
    doc.MainU3XOffset.Y = 20
    doc.HeatSinkMainPanelPage.addView(doc.MainU3XOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U3x")
    U3x = b+HeatSinkCommon._TO220_11_MinHoleHeight
    mainsheet.set("B%d"%ir,'%10.6f'%(U3x/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%U3x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU4XOffset')
    doc.MainU4XOffset.Type = 'DistanceX'
    doc.MainU4XOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                 (doc.MainTopView,'Vertex35')]
    doc.MainU4XOffset.FormatSpec='U4x'
    doc.MainU4XOffset.Arbitrary = True
    doc.MainU4XOffset.X = -2
    doc.MainU4XOffset.Y = 39
    doc.HeatSinkMainPanelPage.addView(doc.MainU4XOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U4x")
    U4x = b+HeatSinkCommon._TO220_15_MinHoleHeight
    mainsheet.set("B%d"%ir,'%10.6f'%(U4x/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%U4x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainQ13YOffset')
    doc.MainQ13YOffset.Type = 'DistanceY'
    doc.MainQ13YOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                 (doc.MainTopView,'Vertex29')]
    doc.MainQ13YOffset.FormatSpec='Q13y'
    doc.MainQ13YOffset.Arbitrary = True
    doc.MainQ13YOffset.X =  -6
    doc.MainQ13YOffset.Y = -11
    doc.HeatSinkMainPanelPage.addView(doc.MainQ13YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"Q13y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._Q13CenterOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._Q13CenterOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU3YOffset')
    doc.MainU3YOffset.Type = 'DistanceY'
    doc.MainU3YOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                 (doc.MainTopView,'Vertex32')]
    doc.MainU3YOffset.FormatSpec='U3y'
    doc.MainU3YOffset.Arbitrary = True
    doc.MainU3YOffset.X =  34
    doc.MainU3YOffset.Y =  16
    doc.HeatSinkMainPanelPage.addView(doc.MainU3YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U3y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._U3CenterOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._U3CenterOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU4YOffset')
    doc.MainU4YOffset.Type = 'DistanceY'
    doc.MainU4YOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                 (doc.MainTopView,'Vertex35')]
    doc.MainU4YOffset.FormatSpec='U4y'
    doc.MainU4YOffset.Arbitrary = True
    doc.MainU4YOffset.X = 16
    doc.MainU4YOffset.Y =  3
    doc.HeatSinkMainPanelPage.addView(doc.MainU4YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U4y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._U4CenterOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._U4CenterOffset)
    ir += 1
    #
    # Edge11: fan1_mh circ
    # Vertex16: fan1_mh center
    # Edge13: fan2_mh circ
    # Vertex22: fan2_mh center
    # Edge9:  fan3_mh circ
    # Vertex10: fan3_mh center
    # Edge10: fan4_mh circ
    # Vertex13: fan4_mh center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainFanMHDiameter')
    doc.MainFanMHDiameter.Type = 'Diameter'
    doc.MainFanMHDiameter.References2D=[(doc.MainTopView,'Edge11')]
    doc.MainFanMHDiameter.FormatSpec='fanmhDia (4x)'
    doc.MainFanMHDiameter.Arbitrary = True
    doc.MainFanMHDiameter.X = -60
    doc.MainFanMHDiameter.Y = -18
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMHDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmhDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanMHoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanMHoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMhY')
    doc.MainFanMhY.Type = 'DistanceY'
    doc.MainFanMhY.References2D=[(doc.MainTopView,'Vertex36'),\
                              (doc.MainTopView,'Vertex16')]
    doc.MainFanMhY.FormatSpec='fanmy'
    doc.MainFanMhY.Arbitrary = True
    doc.MainFanMhY.X = -22
    doc.MainFanMhY.Y = -33
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMhY)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmy")
    fanMhY = HeatSinkCommon._Thickness + \
            (HeatSinkCommon._HSDepth-HeatSinkCommon._FanMHoleCenters)/2.0
    mainsheet.set("B%d"%ir,'%10.6f'%(fanMhY/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fanMhY)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMh1X')
    doc.MainFanMh1X.Type = 'DistanceX'
    doc.MainFanMh1X.References2D=[(doc.MainTopView,'Vertex24'),\
                              (doc.MainTopView,'Vertex16')]
    doc.MainFanMh1X.FormatSpec='fanmx1'
    doc.MainFanMh1X.Arbitrary = True
    doc.MainFanMh1X.X = -28
    doc.MainFanMh1X.Y = -45
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMh1X)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmx1")
    FanMh1X = HeatSinkCommon._Thickness + \
            (HeatSinkCommon._HSDepth-HeatSinkCommon._FanMHoleCenters)/2.0
    mainsheet.set("B%d"%ir,'%10.6f'%(FanMh1X/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%FanMh1X)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMh12X')
    doc.MainFanMh12X.Type = 'DistanceX'
    doc.MainFanMh12X.References2D=[(doc.MainTopView,'Vertex16'),\
                              (doc.MainTopView,'Vertex22')]
    doc.MainFanMh12X.FormatSpec='fanmx12'
    doc.MainFanMh12X.Arbitrary = True
    doc.MainFanMh12X.X =  37
    doc.MainFanMh12X.Y = -60
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMh12X)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmx12")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanMHoleCenters/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanMHoleCenters)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMh23Y')
    doc.MainFanMh23Y.Type = 'DistanceY'
    doc.MainFanMh23Y.References2D=[(doc.MainTopView,'Vertex22'),\
                                   (doc.MainTopView,'Vertex10')]
    doc.MainFanMh23Y.FormatSpec='fanmy23'
    doc.MainFanMh23Y.Arbitrary = True
    doc.MainFanMh23Y.X = -32
    doc.MainFanMh23Y.Y = -34
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMh23Y)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmy23")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanMHoleCenters/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanMHoleCenters)
    ir += 1
    #
    # Vertex5: fanmount panel upper left
    # Vertex4: fanmount panel upper right
    # Vertex2: fanmount panel lower left
    # Vertex0: fanmount panel lower right
    #
    # Vertex3: fanmount panel toptab upper right
    # Vertex1: fanmount panel toptab lower right
    #
    # Edge12: fan cutout circ
    # Vertex19: fan cutout center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainFanMountPanelWidth')
    doc.MainFanMountPanelWidth.Type = 'DistanceX'
    doc.MainFanMountPanelWidth.References2D=[(doc.MainTopView,'Vertex5'),\
                                             (doc.MainTopView,'Vertex4')]
    doc.MainFanMountPanelWidth.FormatSpec='fpw'
    doc.MainFanMountPanelWidth.Arbitrary = True
    doc.MainFanMountPanelWidth.X =   3
    doc.MainFanMountPanelWidth.Y = -53
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMountPanelWidth)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fpw")
    fpw = HeatSinkCommon._HSHeight+HeatSinkCommon._Thickness*2
    mainsheet.set("B%d"%ir,'%10.6f'%(fpw/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fpw)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMountPanelHeight')
    doc.MainFanMountPanelHeight.Type = 'DistanceY'
    doc.MainFanMountPanelHeight.References2D=[(doc.MainTopView,'Vertex5'),\
                                              (doc.MainTopView,'Vertex2')]
    doc.MainFanMountPanelHeight.FormatSpec='fph'
    doc.MainFanMountPanelHeight.Arbitrary = True
    doc.MainFanMountPanelHeight.X =  50
    doc.MainFanMountPanelHeight.Y = -32
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMountPanelHeight)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fph")
    fph=HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness*2
    mainsheet.set("B%d"%ir,'%10.6f'%(fph/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fph)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMountPanelYOffset')
    doc.MainFanMountPanelYOffset.Type = 'DistanceY'
    doc.MainFanMountPanelYOffset.References2D=[(doc.MainTopView,'Vertex36'),\
                                                (doc.MainTopView,'Vertex5')]
    doc.MainFanMountPanelYOffset.FormatSpec='fpy'
    doc.MainFanMountPanelYOffset.Arbitrary = True
    doc.MainFanMountPanelYOffset.X =  46
    doc.MainFanMountPanelYOffset.Y = -12
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMountPanelYOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fpy")
    fpy = HeatSinkCommon._FanTabExtension
    mainsheet.set("B%d"%ir,'%10.6f'%(fpy/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fpy)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMountPanelXOffset')
    doc.MainFanMountPanelXOffset.Type = 'DistanceX'
    doc.MainFanMountPanelXOffset.References2D=[(doc.MainTopView,'Vertex24'),\
                                                (doc.MainTopView,'Vertex5')]
    doc.MainFanMountPanelXOffset.FormatSpec = 'fpx'
    doc.MainFanMountPanelXOffset.Arbitrary = True
    doc.MainFanMountPanelXOffset.X = -24
    doc.MainFanMountPanelXOffset.Y = -58
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMountPanelXOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fpx")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._Thickness/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._Thickness)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMountPanelToptabWidth')
    doc.MainFanMountPanelToptabWidth.Type = 'DistanceX'
    doc.MainFanMountPanelToptabWidth.References2D=[(doc.MainTopView,'Vertex0'),\
                                                   (doc.MainTopView,'Vertex1')]
    doc.MainFanMountPanelToptabWidth.FormatSpec = 'fpttw'
    doc.MainFanMountPanelToptabWidth.Arbitrary = True
    doc.MainFanMountPanelToptabWidth.X =  38
    doc.MainFanMountPanelToptabWidth.Y = -50
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMountPanelToptabWidth)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fpttw")
    fpttw = HeatSinkCommon._FanTabExtension*2+HeatSinkCommon._Thickness*1
    mainsheet.set("B%d"%ir,'%10.6f'%(fpttw/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fpttw)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanHoleDiameter')
    doc.MainFanHoleDiameter.Type = 'Diameter'
    doc.MainFanHoleDiameter.References2D=[(doc.MainTopView,'Edge12')]
    doc.MainFanHoleDiameter.FormatSpec='fanDia'
    doc.MainFanHoleDiameter.Arbitrary = True
    doc.MainFanHoleDiameter.X =  38
    doc.MainFanHoleDiameter.Y = -26
    doc.HeatSinkMainPanelPage.addView(doc.MainFanHoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanHoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanHoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanHoleCX')
    doc.MainFanHoleCX.Type = 'DistanceX'
    doc.MainFanHoleCX.References2D=[(doc.MainTopView,'Vertex5'),\
                                    (doc.MainTopView,'Vertex19')]
    doc.MainFanHoleCX.FormatSpec='fanhx'
    doc.MainFanHoleCX.Arbitrary = True
    doc.MainFanHoleCX.X = -68
    doc.MainFanHoleCX.Y = -34
    doc.HeatSinkMainPanelPage.addView(doc.MainFanHoleCX)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanhx")
    fanhx = HeatSinkCommon._HSDepth/2.0+HeatSinkCommon._Thickness
    mainsheet.set("B%d"%ir,'%10.6f'%(fanhx/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fanhx)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanHoleCY')
    doc.MainFanHoleCY.Type = 'DistanceY'
    doc.MainFanHoleCY.References2D=[(doc.MainTopView,'Vertex5'),\
                                    (doc.MainTopView,'Vertex19')]
    doc.MainFanHoleCY.FormatSpec='fanhy'
    doc.MainFanHoleCY.Arbitrary = True
    doc.MainFanHoleCY.X =  25
    doc.MainFanHoleCY.Y = -49
    doc.HeatSinkMainPanelPage.addView(doc.MainFanHoleCY)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanhy")
    fanhy = (HeatSinkCommon._HSDepth/2.0)+HeatSinkCommon._Thickness
    mainsheet.set("B%d"%ir,'%10.6f'%(fanhy/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fanhy)
    ir += 1
    #
    mainsheet.recompute()
    doc.addObject('TechDraw::DrawViewSpreadsheet','MainDimBlock')
    doc.MainDimBlock.Source = mainsheet
    doc.MainDimBlock.TextSize = 8
    doc.MainDimBlock.Scale=.50
    doc.MainDimBlock.CellEnd = "C%d"%(ir-1)
    doc.HeatSinkMainPanelPage.addView(doc.MainDimBlock)
    doc.MainDimBlock.recompute()
    doc.MainDimBlock.X =  65
    doc.MainDimBlock.Y = 110
    doc.HeatSinkMainPanelPage.recompute()
    #
    doc.addObject('TechDraw::DrawPage','HeatSinkFin1PanelPage')
    doc.HeatSinkFin1PanelPage.Template = doc.USLetterTemplate
    edt = doc.HeatSinkFin1PanelPage.Template.EditableTexts
    edt['CompanyName'] = "Deepwoods Software"
    edt['CompanyAddress'] = '51 Locke Hill Road, Wendell, MA 01379 USA'
    edt['DrawingTitle1']= 'Heat Sink Fin1 Panel'
    edt['DrawingTitle3']= ""
    edt['DrawnBy'] = "Robert Heller"
    edt['CheckedBy'] = ""
    edt['Approved1'] = ""
    edt['Approved2'] = ""
    edt['Code'] = ""
    edt['Weight'] = ''
    edt['DrawingNumber'] = datetime.datetime.now().ctime()
    edt['Revision'] = "A"
    edt['DrawingTitle2']= ""
    edt['Scale'] = '1:1'
    edt['Sheet'] = "Sheet 2 of 3"
    doc.HeatSinkFin1PanelPage.Template.EditableTexts = edt
    doc.HeatSinkFin1PanelPage.ViewObject.show()
    fin1sheet = doc.addObject('Spreadsheet::Sheet','Fin1DimensionTable')
    fin1sheet.set("A1",'%-11.11s'%"Dim")
    fin1sheet.set("B1",'%10.10s'%"inch")
    fin1sheet.set("C1",'%10.10s'%"mm")
    ir = 2
    doc.addObject('TechDraw::DrawViewPart','Fin1TopView')
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1TopView)
    doc.Fin1TopView.Source = doc.heatsink_fin1
    doc.Fin1TopView.X = 200
    doc.Fin1TopView.Y = 135
    doc.Fin1TopView.Direction=(0.0,0.0,1.0)
    #doc.Fin1TopView.Scale=3.0
    #
    # Vertex16: upper left corner
    # Vertex3:  upper right corner
    # Vertex5:  lower left corner
    # Vertex1:  lower right corner
    #
    # Vertex6:  upper right bottom corner
    # Vertex4:  upper left  top corner
    #
    # Edge8:    Q13_mh circ
    # Vertex9:  Q13_mh center
    # Edge9:    u3_mh circ
    # Vertex12: u3_mh center
    # Edge10:   u4_mh circ
    # Vertex15: u4_mh center
    #
    doc.addObject('TechDraw::DrawViewDimension','Fin1SheetTotalWidth')
    doc.Fin1SheetTotalWidth.Type = 'DistanceX'
    doc.Fin1SheetTotalWidth.References2D=[(doc.Fin1TopView,'Vertex16'),\
                                     (doc.Fin1TopView,'Vertex1')]
    doc.Fin1SheetTotalWidth.FormatSpec='tw'
    doc.Fin1SheetTotalWidth.Arbitrary = True
    doc.Fin1SheetTotalWidth.X = 0
    doc.Fin1SheetTotalWidth.Y = 64
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1SheetTotalWidth)
    fin1sheet.set("A%d"%ir,'%-11.11s'%"tw")
    tw = HeatSinkCommon._HSHeight*.6+\
             ((HeatSinkCommon._HSDepth)*2)
    fin1sheet.set("B%d"%ir,'%10.6f'%(tw/25.4))
    fin1sheet.set("C%d"%ir,'%10.6f'%tw)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Fin1BottomWidth')
    doc.Fin1BottomWidth.Type = 'DistanceX'
    doc.Fin1BottomWidth.References2D=[(doc.Fin1TopView,'Vertex16'),\
                                      (doc.Fin1TopView,'Vertex6')]
    doc.Fin1BottomWidth.FormatSpec='b'
    doc.Fin1BottomWidth.Arbitrary = True
    doc.Fin1BottomWidth.X = -25
    doc.Fin1BottomWidth.Y = 55
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1BottomWidth)
    fin1sheet.set("A%d"%ir,'%-11.11s'%"b")
    b = HeatSinkCommon._HSDepth
    fin1sheet.set("B%d"%ir,'%10.6f'%(b/25.4))
    fin1sheet.set("C%d"%ir,'%10.6f'%b)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Fin1Width')
    doc.Fin1Width.Type = 'DistanceX'
    doc.Fin1Width.References2D=[(doc.Fin1TopView,'Vertex6'),\
                                 (doc.Fin1TopView,'Vertex4')]
    doc.Fin1Width.FormatSpec='w'
    doc.Fin1Width.Arbitrary = True
    doc.Fin1Width.X = 0
    doc.Fin1Width.Y = 55
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1Width)
    fin1sheet.set("A%d"%ir,'%-11.11s'%"w")
    w = HeatSinkCommon._HSHeight*.6
    fin1sheet.set("B%d"%ir,'%10.6f'%(w/25.4))
    fin1sheet.set("C%d"%ir,'%10.6f'%w)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Fin1TopWidth')
    doc.Fin1TopWidth.Type = 'DistanceX'
    doc.Fin1TopWidth.References2D=[(doc.Fin1TopView,'Vertex4'),\
                                 (doc.Fin1TopView,'Vertex1')]
    doc.Fin1TopWidth.FormatSpec='t'
    doc.Fin1TopWidth.Arbitrary = True
    doc.Fin1TopWidth.X = 25
    doc.Fin1TopWidth.Y = 55
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1TopWidth)
    fin1sheet.set("A%d"%ir,'%-11.11s'%"t")
    fin1sheet.set("B%d"%ir,'%10.6f'%(b/25.4))
    fin1sheet.set("C%d"%ir,'%10.6f'%b)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Fin1Q13XOffset')
    doc.Fin1Q13XOffset.Type = 'DistanceX'
    doc.Fin1Q13XOffset.References2D=[(doc.Fin1TopView,'Vertex16'),\
                                 (doc.Fin1TopView,'Vertex9')]
    doc.Fin1Q13XOffset.FormatSpec='Q13x'
    doc.Fin1Q13XOffset.Arbitrary = True
    doc.Fin1Q13XOffset.X =  -4
    doc.Fin1Q13XOffset.Y = -24
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1Q13XOffset)
    fin1sheet.set("A%d"%ir,'%-11.11s'%"Q13x")
    Q13x = b+HeatSinkCommon._Q13CenterOffset-(HeatSinkCommon._HSHeight*.2)
    fin1sheet.set("B%d"%ir,'%10.6f'%(Q13x/25.4))
    fin1sheet.set("C%d"%ir,'%10.6f'%Q13x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Fin1U3XOffset')
    doc.Fin1U3XOffset.Type = 'DistanceX'
    doc.Fin1U3XOffset.References2D=[(doc.Fin1TopView,'Vertex16'),\
                                 (doc.Fin1TopView,'Vertex12')]
    doc.Fin1U3XOffset.FormatSpec='U3x'
    doc.Fin1U3XOffset.Arbitrary = True
    doc.Fin1U3XOffset.X = -4
    doc.Fin1U3XOffset.Y = -2
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1U3XOffset)
    fin1sheet.set("A%d"%ir,'%-11.11s'%"U3x")
    U3x = b+HeatSinkCommon._TO220_11_MinHoleHeight-(HeatSinkCommon._HSHeight*.2)
    fin1sheet.set("B%d"%ir,'%10.6f'%(U3x/25.4))
    fin1sheet.set("C%d"%ir,'%10.6f'%U3x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Fin1U4XOffset')
    doc.Fin1U4XOffset.Type = 'DistanceX'
    doc.Fin1U4XOffset.References2D=[(doc.Fin1TopView,'Vertex16'),\
                                 (doc.Fin1TopView,'Vertex15')]
    doc.Fin1U4XOffset.FormatSpec='U4x'
    doc.Fin1U4XOffset.Arbitrary = True
    doc.Fin1U4XOffset.X = -4
    doc.Fin1U4XOffset.Y = 18
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1U4XOffset)
    fin1sheet.set("A%d"%ir,'%-11.11s'%"U4x")
    U4x = b+HeatSinkCommon._TO220_15_MinHoleHeight-(HeatSinkCommon._HSHeight*.2)
    fin1sheet.set("B%d"%ir,'%10.6f'%(U4x/25.4))
    fin1sheet.set("C%d"%ir,'%10.6f'%U4x)
    ir += 1
    fin1sheet.set("A%d"%ir,'All Other measurements')
    fin1sheet.set("B%d"%ir,'are from sheet 1')
    ir += 1
    fin1sheet.recompute()
    doc.addObject('TechDraw::DrawViewSpreadsheet','Fin1DimBlock')
    doc.Fin1DimBlock.Source = fin1sheet
    doc.Fin1DimBlock.TextSize = 8
    #doc.Fin1DimBlock.Scale=.85
    doc.Fin1DimBlock.CellEnd = "C%d"%(ir-1)
    doc.HeatSinkFin1PanelPage.addView(doc.Fin1DimBlock)
    doc.Fin1DimBlock.recompute()
    doc.Fin1DimBlock.X =  65
    doc.Fin1DimBlock.Y = 110
    doc.HeatSinkFin1PanelPage.recompute()

    doc.recompute()
