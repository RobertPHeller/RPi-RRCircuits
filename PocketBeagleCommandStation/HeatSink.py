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
#  Last Modified : <210306.1212>
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
        bottom = Part.makePlane(self._HSDepth,self._HSLength,\
                self.origin.add(Base.Vector(-(self._HSDepth),0,0)))\
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
                self.origin.add(Base.Vector(-(self._HSDepth+self._Thickness),0,self._HSHeight+self._Thickness)))\
                .extrude(zExtrude)
        self.fuseto3D(top)
        top = Part.makePlane(self._HSDepth,self._HSLength,\
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
        bottom = Part.makePlane(self._HSDepth-self._Thickness,self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth),0,self._HSHeight*.2))).extrude(zExtrude)        
        self.fuseto3D(bottom)
        bottom = Part.makePlane(self._HSDepth+self._Thickness,self._HSLength,\
                                self.sheetorigin.add(Base.Vector(-(self._HSDepth+self._Thickness)+self._HSHeight*.2,0,0))).extrude(zExtrude)
        self.fuseto2D(bottom)
        top = Part.makePlane(self._HSDepth-self._Thickness,self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth),0,self._Thickness+self._HSHeight*.8))).extrude(zExtrude)
        self.fuseto3D(top)
        top = Part.makePlane(self._HSDepth+self._Thickness,self._HSLength,\
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
        bottom = Part.makePlane(self._HSDepth-(self._Thickness*2),self._HSLength,\
                                self.origin.add(Base.Vector(-(self._HSDepth),0,self._HSHeight*.4))).extrude(zExtrude)        
        self.fuseto3D(bottom)
        bottom = Part.makePlane(self._HSDepth-(self._Thickness*2),self._HSLength,\
                                self.sheetorigin.add(Base.Vector(-(self._HSDepth-self._Thickness*2)+self._HSHeight*.4,0,0)))\
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
    sheet = doc.addObject('Spreadsheet::Sheet','DimensionTable')
    sheet.set("A1",'%-11.11s'%"Dim")
    sheet.set("B1",'%10.10s'%"inch")
    sheet.set("C1",'%10.10s'%"mm")
    ir = 2
    doc.addObject('TechDraw::DrawViewPart','TopView')
    doc.HeatSinkMainPanelPage.addView(doc.TopView)
    doc.TopView.Source = doc.heatsink_main
    doc.TopView.X = 200
    doc.TopView.Y = 135
    doc.TopView.Direction=(0.0,0.0,1.0)
    #doc.TopView.Scale=3.0
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
    doc.addObject('TechDraw::DrawViewDimension','Length')
    doc.Length.Type = 'DistanceY'
    doc.Length.References2D=[(doc.TopView,'Vertex33'),(doc.TopView,'Vertex32')]
    doc.Length.FormatSpec='l'
    doc.Length.Arbitrary = True
    doc.Length.X = -50
    doc.Length.Y = 0
    doc.HeatSinkMainPanelPage.addView(doc.Length)
    sheet.set("A%d"%ir,'%-11.11s'%"l")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._HSLength/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._HSLength)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','SheetTotalLength')
    doc.SheetTotalLength.Type = 'DistanceY'
    doc.SheetTotalLength.References2D=[(doc.TopView,'Vertex33'),\
                                       (doc.TopView,'Vertex0')]
    doc.SheetTotalLength.FormatSpec='tl'
    doc.SheetTotalLength.Arbitrary = True
    doc.SheetTotalLength.X = -60
    doc.SheetTotalLength.Y = 0
    doc.HeatSinkMainPanelPage.addView(doc.SheetTotalLength)
    sheet.set("A%d"%ir,'%-11.11s'%"tl")
    tl = HeatSinkCommon._HSLength+HeatSinkCommon._FanMTab+\
            HeatSinkCommon._Thickness
    sheet.set("B%d"%ir,'%10.6f'%(tl/25.4))
    sheet.set("C%d"%ir,'%10.6f'%tl)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','SheetTotalWidth')
    doc.SheetTotalWidth.Type = 'DistanceX'
    doc.SheetTotalWidth.References2D=[(doc.TopView,'Vertex14'),\
                                     (doc.TopView,'Vertex33')]
    doc.SheetTotalWidth.FormatSpec='tw'
    doc.SheetTotalWidth.Arbitrary = True
    doc.SheetTotalWidth.X = 0
    doc.SheetTotalWidth.Y = 64
    doc.HeatSinkMainPanelPage.addView(doc.SheetTotalWidth)
    sheet.set("A%d"%ir,'%-11.11s'%"tw")
    tw = HeatSinkCommon._HSHeight+\
             ((HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness)*2)
    sheet.set("B%d"%ir,'%10.6f'%(tw/25.4))
    sheet.set("C%d"%ir,'%10.6f'%tw)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','BottomWidth')
    doc.BottomWidth.Type = 'DistanceX'
    doc.BottomWidth.References2D=[(doc.TopView,'Vertex14'),\
                                      (doc.TopView,'Vertex15')]
    doc.BottomWidth.FormatSpec='b'
    doc.BottomWidth.Arbitrary = True
    doc.BottomWidth.X = -25
    doc.BottomWidth.Y = 55
    doc.HeatSinkMainPanelPage.addView(doc.BottomWidth)
    sheet.set("A%d"%ir,'%-11.11s'%"b")
    b = HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness
    sheet.set("B%d"%ir,'%10.6f'%(b/25.4))
    sheet.set("C%d"%ir,'%10.6f'%b)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MainWidth')
    doc.MainWidth.Type = 'DistanceX'
    doc.MainWidth.References2D=[(doc.TopView,'Vertex15'),\
                                 (doc.TopView,'Vertex22')]
    doc.MainWidth.FormatSpec='w'
    doc.MainWidth.Arbitrary = True
    doc.MainWidth.X = 0
    doc.MainWidth.Y = 55
    doc.HeatSinkMainPanelPage.addView(doc.MainWidth)
    sheet.set("A%d"%ir,'%-11.11s'%"w")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._HSHeight/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._HSHeight)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','TopWidth')
    doc.TopWidth.Type = 'DistanceX'
    doc.TopWidth.References2D=[(doc.TopView,'Vertex22'),\
                                 (doc.TopView,'Vertex33')]
    doc.TopWidth.FormatSpec='t'
    doc.TopWidth.Arbitrary = True
    doc.TopWidth.X = 25
    doc.TopWidth.Y = 55
    doc.HeatSinkMainPanelPage.addView(doc.TopWidth)
    sheet.set("A%d"%ir,'%-11.11s'%"t")
    sheet.set("B%d"%ir,'%10.6f'%(b/25.4))
    sheet.set("C%d"%ir,'%10.6f'%b)
    ir += 1
    # 
    # Edge13: mh2 circ
    # Vertex21: mh2 center
    # Edge12: mh1 circ
    # Vertex18: mh1 center
    #
    doc.addObject('TechDraw::DrawViewDimension','MountHoleDiameter')
    doc.MountHoleDiameter.Type = 'Diameter'
    doc.MountHoleDiameter.References2D=[(doc.TopView,'Edge13')]
    doc.MountHoleDiameter.FormatSpec='mhDia (2x)'
    doc.MountHoleDiameter.Arbitrary = True
    doc.MountHoleDiameter.X = -15
    doc.MountHoleDiameter.Y =  26.16
    doc.HeatSinkMainPanelPage.addView(doc.MountHoleDiameter)
    sheet.set("A%d"%ir,'%-11.11s'%"mhDia")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._MHoleDiameter/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._MHoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MountHoleXOffset')
    doc.MountHoleXOffset.Type = 'DistanceX'
    doc.MountHoleXOffset.References2D=[(doc.TopView,'Vertex14'),\
                                       (doc.TopView,'Vertex18')]
    doc.MountHoleXOffset.FormatSpec='mhx'
    doc.MountHoleXOffset.Arbitrary = True
    doc.MountHoleXOffset.X = -30
    doc.MountHoleXOffset.Y = 35
    doc.HeatSinkMainPanelPage.addView(doc.MountHoleXOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"mhx")
    mhx = HeatSinkCommon._HSDepth-\
        (HeatSinkCommon._BoardEdgeToDeviceMountPlane+HeatSinkCommon._Thickness)
    sheet.set("B%d"%ir,'%10.6f'%(mhx/25.4))
    sheet.set("C%d"%ir,'%10.6f'%mhx)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MountHole1YOffset')
    doc.MountHole1YOffset.Type = 'DistanceY'
    doc.MountHole1YOffset.References2D=[(doc.TopView,'Vertex18'),\
                                        (doc.TopView,'Vertex3')]
    doc.MountHole1YOffset.FormatSpec='mh1x'
    doc.MountHole1YOffset.Arbitrary = True
    doc.MountHole1YOffset.X = -21
    doc.MountHole1YOffset.Y = -23
    doc.HeatSinkMainPanelPage.addView(doc.MountHole1YOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"mh1x")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._BoardEdgeMHOffset/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._BoardEdgeMHOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','MountHole1_2YOffset')
    doc.MountHole1_2YOffset.Type = 'DistanceY'
    doc.MountHole1_2YOffset.References2D=[(doc.TopView,'Vertex18'),\
                                        (doc.TopView,'Vertex21')]
    doc.MountHole1_2YOffset.FormatSpec='mh12x'
    doc.MountHole1_2YOffset.Arbitrary = True
    doc.MountHole1_2YOffset.X = -35
    doc.MountHole1_2YOffset.Y =   0
    doc.HeatSinkMainPanelPage.addView(doc.MountHole1_2YOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"mh12x")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._MHoleSpacingY/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._MHoleSpacingY)
    ir += 1
    #
    # Edge18: u4_mh circ
    # Vertex31: u4_mh center
    # Edge17: u3_mh circ
    # Vertex28: u3_mh center
    # Edge16: Q13_mh circ
    # Vertex25: Q13_mh center
    #
    doc.addObject('TechDraw::DrawViewDimension','TO220_11_15_HoleDiameter')
    doc.TO220_11_15_HoleDiameter.Type = 'Diameter'
    doc.TO220_11_15_HoleDiameter.References2D=[(doc.TopView,'Edge18')]
    doc.TO220_11_15_HoleDiameter.FormatSpec='ICMhDia (2x)'
    doc.TO220_11_15_HoleDiameter.Arbitrary = True
    doc.TO220_11_15_HoleDiameter.X =  11.25
    doc.TO220_11_15_HoleDiameter.Y =  37.8
    doc.HeatSinkMainPanelPage.addView(doc.TO220_11_15_HoleDiameter)
    sheet.set("A%d"%ir,'%-11.11s'%"ICMhDia")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._TO220_HoleDiameter/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._TO220_HoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','ShoulderWasherHoleDiameter')
    doc.ShoulderWasherHoleDiameter.Type = 'Diameter'
    doc.ShoulderWasherHoleDiameter.References2D=[(doc.TopView,'Edge16')]
    doc.ShoulderWasherHoleDiameter.FormatSpec='Q13Mh'
    doc.ShoulderWasherHoleDiameter.Arbitrary = True
    doc.ShoulderWasherHoleDiameter.X = -0.81
    doc.ShoulderWasherHoleDiameter.Y = -12.86
    doc.HeatSinkMainPanelPage.addView(doc.ShoulderWasherHoleDiameter)
    sheet.set("A%d"%ir,'%-11.11s'%"Q13Mh")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._Number6Shoulder_Diameter/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._Number6Shoulder_Diameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Q13XOffset')
    doc.Q13XOffset.Type = 'DistanceX'
    doc.Q13XOffset.References2D=[(doc.TopView,'Vertex3'),\
                                 (doc.TopView,'Vertex25')]
    doc.Q13XOffset.FormatSpec='Q13x'
    doc.Q13XOffset.Arbitrary = True
    doc.Q13XOffset.X =  -4
    doc.Q13XOffset.Y = -24
    doc.HeatSinkMainPanelPage.addView(doc.Q13XOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"Q13x")
    Q13x = HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness+\
        (HeatSinkCommon._HSHeight+HeatSinkCommon._Thickness-\
         HeatSinkCommon._Q13CenterOffset)
    sheet.set("B%d"%ir,'%10.6f'%(Q13x/25.4))
    sheet.set("C%d"%ir,'%10.6f'%Q13x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','U3XOffset')
    doc.U3XOffset.Type = 'DistanceX'
    doc.U3XOffset.References2D=[(doc.TopView,'Vertex3'),\
                                 (doc.TopView,'Vertex28')]
    doc.U3XOffset.FormatSpec='U3x'
    doc.U3XOffset.Arbitrary = True
    doc.U3XOffset.X = -4
    doc.U3XOffset.Y = -2
    doc.HeatSinkMainPanelPage.addView(doc.U3XOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"U3x")
    U3x = HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness+\
        (HeatSinkCommon._HSHeight+HeatSinkCommon._Thickness-\
         HeatSinkCommon._TO220_11_MinHoleHeight)
    sheet.set("B%d"%ir,'%10.6f'%(U3x/25.4))
    sheet.set("C%d"%ir,'%10.6f'%U3x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','U4XOffset')
    doc.U4XOffset.Type = 'DistanceX'
    doc.U4XOffset.References2D=[(doc.TopView,'Vertex3'),\
                                 (doc.TopView,'Vertex31')]
    doc.U4XOffset.FormatSpec='U4x'
    doc.U4XOffset.Arbitrary = True
    doc.U4XOffset.X = -4
    doc.U4XOffset.Y = 18
    doc.HeatSinkMainPanelPage.addView(doc.U4XOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"U4x")
    U4x = HeatSinkCommon._HSDepth+HeatSinkCommon._Thickness+\
        (HeatSinkCommon._HSHeight+HeatSinkCommon._Thickness-\
         HeatSinkCommon._TO220_15_MinHoleHeight)
    sheet.set("B%d"%ir,'%10.6f'%(U4x/25.4))
    sheet.set("C%d"%ir,'%10.6f'%U4x)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','Q13YOffset')
    doc.Q13YOffset.Type = 'DistanceY'
    doc.Q13YOffset.References2D=[(doc.TopView,'Vertex3'),\
                                 (doc.TopView,'Vertex25')]
    doc.Q13YOffset.FormatSpec='Q13y'
    doc.Q13YOffset.Arbitrary = True
    doc.Q13YOffset.X =  15
    doc.Q13YOffset.Y = -24
    doc.HeatSinkMainPanelPage.addView(doc.Q13YOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"Q13y")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._Q13CenterOffset/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._Q13CenterOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','U3YOffset')
    doc.U3YOffset.Type = 'DistanceY'
    doc.U3YOffset.References2D=[(doc.TopView,'Vertex3'),\
                                 (doc.TopView,'Vertex28')]
    doc.U3YOffset.FormatSpec='U3y'
    doc.U3YOffset.Arbitrary = True
    doc.U3YOffset.X =  20
    doc.U3YOffset.Y =  10
    doc.HeatSinkMainPanelPage.addView(doc.U3YOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"U3y")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._U3CenterOffset/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._U3CenterOffset)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','U4YOffset')
    doc.U4YOffset.Type = 'DistanceY'
    doc.U4YOffset.References2D=[(doc.TopView,'Vertex3'),\
                                 (doc.TopView,'Vertex31')]
    doc.U4YOffset.FormatSpec='U4y'
    doc.U4YOffset.Arbitrary = True
    doc.U4YOffset.X = 27
    doc.U4YOffset.Y = 23
    doc.HeatSinkMainPanelPage.addView(doc.U4YOffset)
    sheet.set("A%d"%ir,'%-11.11s'%"U4y")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._U4CenterOffset/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._U4CenterOffset)
    ir += 1
    #
    # Edge7: fan1_mh circ
    # Vertex10: fan1_mh center
    # Edge8: fan2_mh circ
    # Vertex13: fan2_mh center
    #
    doc.addObject('TechDraw::DrawViewDimension','FanMHDiameter')
    doc.FanMHDiameter.Type = 'Diameter'
    doc.FanMHDiameter.References2D=[(doc.TopView,'Edge7')]
    doc.FanMHDiameter.FormatSpec='fanmhDia (2x)'
    doc.FanMHDiameter.Arbitrary = True
    doc.FanMHDiameter.X =  -3
    doc.FanMHDiameter.Y = -50
    doc.HeatSinkMainPanelPage.addView(doc.FanMHDiameter)
    sheet.set("A%d"%ir,'%-11.11s'%"fanmhDia")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanMHoleDiameter/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanMHoleDiameter)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','FanMhY')
    doc.FanMhY.Type = 'DistanceY'
    doc.FanMhY.References2D=[(doc.TopView,'Vertex3'),\
                              (doc.TopView,'Vertex10')]
    doc.FanMhY.FormatSpec='fanmy'
    doc.FanMhY.Arbitrary = True
    doc.FanMhY.X = -39
    doc.FanMhY.Y = -38
    doc.HeatSinkMainPanelPage.addView(doc.FanMhY)
    sheet.set("A%d"%ir,'%-11.11s'%"fanmy")
    fanMhY = HeatSinkCommon._Thickness + \
            (HeatSinkCommon._HSDepth-HeatSinkCommon._FanMHoleCenters)/2.0
    sheet.set("B%d"%ir,'%10.6f'%(fanMhY/25.4))
    sheet.set("C%d"%ir,'%10.6f'%fanMhY)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','FanMh1X')
    doc.FanMh1X.Type = 'DistanceX'
    doc.FanMh1X.References2D=[(doc.TopView,'Vertex15'),\
                              (doc.TopView,'Vertex10')]
    doc.FanMh1X.FormatSpec='fanmx1'
    doc.FanMh1X.Arbitrary = True
    doc.FanMh1X.X = -32
    doc.FanMh1X.Y = -43
    doc.HeatSinkMainPanelPage.addView(doc.FanMh1X)
    sheet.set("A%d"%ir,'%-11.11s'%"fanmx1")
    FanMh1X = HeatSinkCommon._Thickness + \
            (HeatSinkCommon._HSDepth-HeatSinkCommon._FanMHoleCenters)/2.0
    sheet.set("B%d"%ir,'%10.6f'%(FanMh1X/25.4))
    sheet.set("C%d"%ir,'%10.6f'%FanMh1X)
    ir += 1
    doc.addObject('TechDraw::DrawViewDimension','FanMh12X')
    doc.FanMh12X.Type = 'DistanceX'
    doc.FanMh12X.References2D=[(doc.TopView,'Vertex10'),\
                              (doc.TopView,'Vertex13')]
    doc.FanMh12X.FormatSpec='fanmx12'
    doc.FanMh12X.Arbitrary = True
    doc.FanMh12X.X =   9
    doc.FanMh12X.Y = -50
    doc.HeatSinkMainPanelPage.addView(doc.FanMh12X)
    sheet.set("A%d"%ir,'%-11.11s'%"fanmx12")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanMHoleCenters/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanMHoleCenters)
    ir += 1
    #
    # Edge6: fan cutout circ
    # Vertex7: fan cutout center
    #
    doc.addObject('TechDraw::DrawViewDimension','FanHoleDiameter')
    doc.FanHoleDiameter.Type = 'Diameter'
    doc.FanHoleDiameter.References2D=[(doc.TopView,'Edge6')]
    doc.FanHoleDiameter.FormatSpec='fanDia'
    doc.FanHoleDiameter.Arbitrary = True
    doc.FanHoleDiameter.X =  41
    doc.FanHoleDiameter.Y = -41
    doc.HeatSinkMainPanelPage.addView(doc.FanHoleDiameter)
    sheet.set("A%d"%ir,'%-11.11s'%"fanDia")
    sheet.set("B%d"%ir,'%10.6f'%(HeatSinkCommon._FanHoleDiameter/25.4))
    sheet.set("C%d"%ir,'%10.6f'%HeatSinkCommon._FanHoleDiameter)
    ir += 1
    
    sheet.recompute()
    doc.addObject('TechDraw::DrawViewSpreadsheet','DimBlock')
    doc.DimBlock.Source = sheet
    doc.DimBlock.TextSize = 8
    doc.DimBlock.Scale=.85
    doc.DimBlock.CellEnd = "C%d"%(ir-1)
    doc.HeatSinkMainPanelPage.addView(doc.DimBlock)
    doc.DimBlock.recompute()
    doc.DimBlock.X =  65
    doc.DimBlock.Y = 110
    doc.HeatSinkMainPanelPage.recompute()
    
    doc.recompute()
