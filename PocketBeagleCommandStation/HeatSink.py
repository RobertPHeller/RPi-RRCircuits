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
#  Last Modified : <210306.1711>
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
        self.body = Part.makePlane(self._HSHeight,self._HSLength,\
                                    self.origin.add(Base.Vector(0,\
                                                        self._HSLength,\
                                                        self._Thickness)),\
                                    xNorm).extrude(xExtrude)
    
        self.sheet = Part.makePlane(self._HSHeight,self._HSLength,\
                                    self.origin.add(Base.Vector(0,0,0)))\
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
        fanmounttab = Part.makePlane(self._HSHeight,\
                                     self._FanMTab+self._Thickness,\
                                     origin.add(Base.Vector(-(self._FanMTab+self._Thickness),\
                                                            0,self._Thickness)),yNorm)\
                                    .extrude(yExtrude)
        self.fuseto3D(fanmounttab)
        fanmX = -(self._Thickness + (self._HSDepth-self._FanMHoleCenters)/2.0)
        fanmZ1 = self._Thickness+(self._HSDepth-self._FanMHoleCenters)/2.0
        fanmZ2 = fanmZ1+self._FanMHoleCenters
        fanm1 = origin.add(Base.Vector(fanmX,0,fanmZ1))
        fanm2 = origin.add(Base.Vector(fanmX,0,fanmZ2))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm1,yNorm)))\
            .extrude(yExtrude))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanMHoleDiameter/2.0,fanm2,yNorm)))\
            .extrude(yExtrude))
        fanCX = -(self._Thickness + self._HSDepth/2.0)
        fanCZ = self._Thickness+self._HSDepth/2.0
        fanHole = origin.add(Base.Vector(fanCX,0,fanCZ))
        self.cutfrom3D(Part.Face(Part.Wire(\
            Part.makeCircle(self._FanHoleDiameter/2.0,fanHole,yNorm)))\
            .extrude(yExtrude))
        fanmounttab = Part.makePlane(self._HSHeight,self._FanMTab+self._Thickness,\
                                     origin.add(Base.Vector(0,\
                                            -(self._FanMTab+self._Thickness),0)))\
                                     .extrude(zExtrude)
        self.fuseto2D(fanmounttab)
        fanmY = -((self._HSDepth-self._FanMHoleCenters)/2.0)
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
        fanCY = -(self._HSDepth/2.0)
        fanCX = self._HSDepth/2.0
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
    edt['Scale'] = '1:1'
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
    doc.MainTopView.Y = 135
    doc.MainTopView.Direction=(0.0,0.0,1.0)
    #doc.MainTopView.Scale=3.0
    # Vertex14: upper left corner
    # Vertex33: upper right corner
    # Vertex3:  lower left corner
    # Vertex32: lower right corner
    #
    # Vertex15: upper right bottom corner
    # Vertex22: upper left  top corner
    #
    # Vertex0:  lower left fan tab corner
    #
    doc.addObject('TechDraw::DrawViewDimension','MainLength')
    doc.MainLength.Type = 'DistanceY'
    doc.MainLength.References2D=[(doc.MainTopView,'Vertex33'),(doc.MainTopView,'Vertex32')]
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
    doc.MainSheetTotalLength.References2D=[(doc.MainTopView,'Vertex33'),\
                                       (doc.MainTopView,'Vertex0')]
    doc.MainSheetTotalLength.FormatSpec='tl'
    doc.MainSheetTotalLength.Arbitrary = True
    doc.MainSheetTotalLength.X = -60
    doc.MainSheetTotalLength.Y = 0
    doc.HeatSinkMainPanelPage.addView(doc.MainSheetTotalLength)
    mainsheet.set("A%d"%ir,'%-11.11s'%"tl")
    tl = HeatSinkCommon._HSLength+HeatSinkCommon._FanMTab+\
            HeatSinkCommon._Thickness
    mainsheet.set("B%d"%ir,'%10.6f'%(tl/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%tl)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainSheetTotalWidth')
    doc.MainSheetTotalWidth.Type = 'DistanceX'
    doc.MainSheetTotalWidth.References2D=[(doc.MainTopView,'Vertex14'),\
                                     (doc.MainTopView,'Vertex33')]
    doc.MainSheetTotalWidth.FormatSpec='tw'
    doc.MainSheetTotalWidth.Arbitrary = True
    doc.MainSheetTotalWidth.X = 0
    doc.MainSheetTotalWidth.Y = 64
    doc.HeatSinkMainPanelPage.addView(doc.MainSheetTotalWidth)
    mainsheet.set("A%d"%ir,'%-11.11s'%"tw")
    tw = HeatSinkCommon._HSHeight+\
             ((HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness)*2)
    mainsheet.set("B%d"%ir,'%10.6f'%(tw/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%tw)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainBottomWidth')
    doc.MainBottomWidth.Type = 'DistanceX'
    doc.MainBottomWidth.References2D=[(doc.MainTopView,'Vertex14'),\
                                      (doc.MainTopView,'Vertex15')]
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
    doc.MainWidth.References2D=[(doc.MainTopView,'Vertex15'),\
                                 (doc.MainTopView,'Vertex22')]
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
    doc.MainTopWidth.References2D=[(doc.MainTopView,'Vertex22'),\
                                 (doc.MainTopView,'Vertex33')]
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
    # Edge13: mh2 circ
    # Vertex21: mh2 center
    # Edge12: mh1 circ
    # Vertex18: mh1 center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainMountHoleDiameter')
    doc.MainMountHoleDiameter.Type = 'Diameter'
    doc.MainMountHoleDiameter.References2D=[(doc.MainTopView,'Edge13')]
    doc.MainMountHoleDiameter.FormatSpec='mhDia (2x)'
    doc.MainMountHoleDiameter.Arbitrary = True
    doc.MainMountHoleDiameter.X = -15
    doc.MainMountHoleDiameter.Y =  26.16
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mhDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._MHoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._MHoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainMountHoleXOffset')
    doc.MainMountHoleXOffset.Type = 'DistanceX'
    doc.MainMountHoleXOffset.References2D=[(doc.MainTopView,'Vertex14'),\
                                       (doc.MainTopView,'Vertex18')]
    doc.MainMountHoleXOffset.FormatSpec='mhx'
    doc.MainMountHoleXOffset.Arbitrary = True
    doc.MainMountHoleXOffset.X = -30
    doc.MainMountHoleXOffset.Y = 35
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHoleXOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mhx")
    mhx = HeatSinkCommon._HSDepth-\
        (HeatSinkCommon._BoardEdgeToDeviceMountPlane+HeatSinkCommon._Thickness)
    mainsheet.set("B%d"%ir,'%10.6f'%(mhx/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%mhx)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainMountHole1YOffset')
    doc.MainMountHole1YOffset.Type = 'DistanceY'
    doc.MainMountHole1YOffset.References2D=[(doc.MainTopView,'Vertex18'),\
                                        (doc.MainTopView,'Vertex3')]
    doc.MainMountHole1YOffset.FormatSpec='mh1x'
    doc.MainMountHole1YOffset.Arbitrary = True
    doc.MainMountHole1YOffset.X = -21
    doc.MainMountHole1YOffset.Y = -23
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHole1YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mh1x")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._BoardEdgeMHOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._BoardEdgeMHOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainMountHole1_2YOffset')
    doc.MainMountHole1_2YOffset.Type = 'DistanceY'
    doc.MainMountHole1_2YOffset.References2D=[(doc.MainTopView,'Vertex18'),\
                                        (doc.MainTopView,'Vertex21')]
    doc.MainMountHole1_2YOffset.FormatSpec='mh12x'
    doc.MainMountHole1_2YOffset.Arbitrary = True
    doc.MainMountHole1_2YOffset.X = -35
    doc.MainMountHole1_2YOffset.Y =   0
    doc.HeatSinkMainPanelPage.addView(doc.MainMountHole1_2YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"mh12x")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._MHoleSpacingY/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._MHoleSpacingY)
    ir += 1
    #
    # Edge18: u4_mh circ
    # Vertex31: u4_mh center
    # Edge17: u3_mh circ
    # Vertex28: u3_mh center
    # Edge16: Q13_mh circ
    # Vertex25: Q13_mh center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainTO220_11_15_HoleDiameter')
    doc.MainTO220_11_15_HoleDiameter.Type = 'Diameter'
    doc.MainTO220_11_15_HoleDiameter.References2D=[(doc.MainTopView,'Edge18')]
    doc.MainTO220_11_15_HoleDiameter.FormatSpec='ICMhDia (2x)'
    doc.MainTO220_11_15_HoleDiameter.Arbitrary = True
    doc.MainTO220_11_15_HoleDiameter.X =  11.25
    doc.MainTO220_11_15_HoleDiameter.Y =  37.8
    doc.HeatSinkMainPanelPage.addView(doc.MainTO220_11_15_HoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"ICMhDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._TO220_HoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._TO220_HoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainShoulderWasherHoleDiameter')
    doc.MainShoulderWasherHoleDiameter.Type = 'Diameter'
    doc.MainShoulderWasherHoleDiameter.References2D=[(doc.MainTopView,'Edge16')]
    doc.MainShoulderWasherHoleDiameter.FormatSpec='Q13Mh'
    doc.MainShoulderWasherHoleDiameter.Arbitrary = True
    doc.MainShoulderWasherHoleDiameter.X = -0.81
    doc.MainShoulderWasherHoleDiameter.Y = -12.86
    doc.HeatSinkMainPanelPage.addView(doc.MainShoulderWasherHoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"Q13Mh")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._Number6Shoulder_Diameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._Number6Shoulder_Diameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainQ13XOffset')
    doc.MainQ13XOffset.Type = 'DistanceX'
    doc.MainQ13XOffset.References2D=[(doc.MainTopView,'Vertex3'),\
                                 (doc.MainTopView,'Vertex25')]
    doc.MainQ13XOffset.FormatSpec='Q13x'
    doc.MainQ13XOffset.Arbitrary = True
    doc.MainQ13XOffset.X =  -4
    doc.MainQ13XOffset.Y = -24
    doc.HeatSinkMainPanelPage.addView(doc.MainQ13XOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"Q13x")
    Q13x = b+HeatSinkCommon._Q13CenterOffset
    mainsheet.set("B%d"%ir,'%10.6f'%(Q13x/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%Q13x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU3XOffset')
    doc.MainU3XOffset.Type = 'DistanceX'
    doc.MainU3XOffset.References2D=[(doc.MainTopView,'Vertex3'),\
                                 (doc.MainTopView,'Vertex28')]
    doc.MainU3XOffset.FormatSpec='U3x'
    doc.MainU3XOffset.Arbitrary = True
    doc.MainU3XOffset.X = -4
    doc.MainU3XOffset.Y = -2
    doc.HeatSinkMainPanelPage.addView(doc.MainU3XOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U3x")
    U3x = b+HeatSinkCommon._TO220_11_MinHoleHeight
    mainsheet.set("B%d"%ir,'%10.6f'%(U3x/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%U3x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU4XOffset')
    doc.MainU4XOffset.Type = 'DistanceX'
    doc.MainU4XOffset.References2D=[(doc.MainTopView,'Vertex3'),\
                                 (doc.MainTopView,'Vertex31')]
    doc.MainU4XOffset.FormatSpec='U4x'
    doc.MainU4XOffset.Arbitrary = True
    doc.MainU4XOffset.X = -4
    doc.MainU4XOffset.Y = 18
    doc.HeatSinkMainPanelPage.addView(doc.MainU4XOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U4x")
    U4x = b+HeatSinkCommon._TO220_15_MinHoleHeight
    mainsheet.set("B%d"%ir,'%10.6f'%(U4x/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%U4x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainQ13YOffset')
    doc.MainQ13YOffset.Type = 'DistanceY'
    doc.MainQ13YOffset.References2D=[(doc.MainTopView,'Vertex3'),\
                                 (doc.MainTopView,'Vertex25')]
    doc.MainQ13YOffset.FormatSpec='Q13y'
    doc.MainQ13YOffset.Arbitrary = True
    doc.MainQ13YOffset.X =  15
    doc.MainQ13YOffset.Y = -24
    doc.HeatSinkMainPanelPage.addView(doc.MainQ13YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"Q13y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._Q13CenterOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._Q13CenterOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU3YOffset')
    doc.MainU3YOffset.Type = 'DistanceY'
    doc.MainU3YOffset.References2D=[(doc.MainTopView,'Vertex3'),\
                                 (doc.MainTopView,'Vertex28')]
    doc.MainU3YOffset.FormatSpec='U3y'
    doc.MainU3YOffset.Arbitrary = True
    doc.MainU3YOffset.X =  20
    doc.MainU3YOffset.Y =  10
    doc.HeatSinkMainPanelPage.addView(doc.MainU3YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U3y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._U3CenterOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._U3CenterOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainU4YOffset')
    doc.MainU4YOffset.Type = 'DistanceY'
    doc.MainU4YOffset.References2D=[(doc.MainTopView,'Vertex3'),\
                                 (doc.MainTopView,'Vertex31')]
    doc.MainU4YOffset.FormatSpec='U4y'
    doc.MainU4YOffset.Arbitrary = True
    doc.MainU4YOffset.X = 27
    doc.MainU4YOffset.Y = 23
    doc.HeatSinkMainPanelPage.addView(doc.MainU4YOffset)
    mainsheet.set("A%d"%ir,'%-11.11s'%"U4y")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._U4CenterOffset/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._U4CenterOffset)
    ir += 1
    #
    # Edge7: fan1_mh circ
    # Vertex10: fan1_mh center
    # Edge8: fan2_mh circ
    # Vertex13: fan2_mh center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainFanMHDiameter')
    doc.MainFanMHDiameter.Type = 'Diameter'
    doc.MainFanMHDiameter.References2D=[(doc.MainTopView,'Edge7')]
    doc.MainFanMHDiameter.FormatSpec='fanmhDia (2x)'
    doc.MainFanMHDiameter.Arbitrary = True
    doc.MainFanMHDiameter.X =  -3
    doc.MainFanMHDiameter.Y = -50
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMHDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmhDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanMHoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanMHoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMhY')
    doc.MainFanMhY.Type = 'DistanceY'
    doc.MainFanMhY.References2D=[(doc.MainTopView,'Vertex3'),\
                              (doc.MainTopView,'Vertex10')]
    doc.MainFanMhY.FormatSpec='fanmy'
    doc.MainFanMhY.Arbitrary = True
    doc.MainFanMhY.X = -39
    doc.MainFanMhY.Y = -38
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMhY)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmy")
    fanMhY = HeatSinkCommon._Thickness + \
            (HeatSinkCommon._HSDepth-HeatSinkCommon._FanMHoleCenters)/2.0
    mainsheet.set("B%d"%ir,'%10.6f'%(fanMhY/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%fanMhY)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMh1X')
    doc.MainFanMh1X.Type = 'DistanceX'
    doc.MainFanMh1X.References2D=[(doc.MainTopView,'Vertex15'),\
                              (doc.MainTopView,'Vertex10')]
    doc.MainFanMh1X.FormatSpec='fanmx1'
    doc.MainFanMh1X.Arbitrary = True
    doc.MainFanMh1X.X = -32
    doc.MainFanMh1X.Y = -43
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMh1X)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmx1")
    FanMh1X = HeatSinkCommon._Thickness + \
            (HeatSinkCommon._HSDepth-HeatSinkCommon._FanMHoleCenters)/2.0
    mainsheet.set("B%d"%ir,'%10.6f'%(FanMh1X/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%FanMh1X)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainFanMh12X')
    doc.MainFanMh12X.Type = 'DistanceX'
    doc.MainFanMh12X.References2D=[(doc.MainTopView,'Vertex10'),\
                              (doc.MainTopView,'Vertex13')]
    doc.MainFanMh12X.FormatSpec='fanmx12'
    doc.MainFanMh12X.Arbitrary = True
    doc.MainFanMh12X.X =   9
    doc.MainFanMh12X.Y = -50
    doc.HeatSinkMainPanelPage.addView(doc.MainFanMh12X)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanmx12")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanMHoleCenters/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanMHoleCenters)
    ir += 1
    #
    # Edge6: fan cutout circ
    # Vertex7: fan cutout center
    #
    doc.addObject('TechDraw::DrawViewDimension','MainFanHoleDiameter')
    doc.MainFanHoleDiameter.Type = 'Diameter'
    doc.MainFanHoleDiameter.References2D=[(doc.MainTopView,'Edge6')]
    doc.MainFanHoleDiameter.FormatSpec='fanDia'
    doc.MainFanHoleDiameter.Arbitrary = True
    doc.MainFanHoleDiameter.X =  41
    doc.MainFanHoleDiameter.Y = -41
    doc.HeatSinkMainPanelPage.addView(doc.MainFanHoleDiameter)
    mainsheet.set("A%d"%ir,'%-11.11s'%"fanDia")
    mainsheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanHoleDiameter/25.4))
    mainsheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanHoleDiameter)
    ir += 1
    
    mainsheet.recompute()
    doc.addObject('TechDraw::DrawViewSpreadsheet','MainDimBlock')
    doc.MainDimBlock.Source = mainsheet
    doc.MainDimBlock.TextSize = 8
    doc.MainDimBlock.Scale=.85
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
