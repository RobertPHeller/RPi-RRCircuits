#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri May 3 11:36:09 2024
#  Last Modified : <240503.2306>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2024  Robert Heller D/B/A Deepwoods Software
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


import Part
from FreeCAD import Base
import FreeCAD as App
import os
import sys
sys.path.append(os.path.dirname(__file__))
import Mesh

import datetime

class ContactWipers(object):
    __PadWidth = 13.5
    __PadHeight = 3.5
    __fold = 6.26
    __Thick = .008*25.4
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        wiper = Part.makePlane(self.__PadWidth,self.__PadHeight+self.__fold,self.origin)\
            .extrude(Base.Vector(0,0,self.__Thick))
        self.wiper = wiper
        #line = Part.makeLine(self.origin.add(Base.Vector(0,self.__PadHeight,\
        #                                                 0)),\
        #                     self.origin.add(Base.Vector(self.__PadWidth,\
        #                                                 self.__PadHeight,\
        #                                                 0)))
        #self.wiper.fuse(Part.Face(Part.Wire(line))\
        #    .extrude(Base.Vector(0,0,self.__Thick)))
    def show(self,doc=None):
        if doc==None:
            doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name);
        obj.Shape=self.wiper
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.72156,.52549,.04313])
        
class ContactWipersBent(object):
    __PadWidth = 13.5
    __PadHeight = 3.5
    __fold = 6.26
    __Thick = .008*25.4
    def __init__(self,name,origin,dir='Left'):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        wiper = Part.makePlane(self.__PadWidth,self.__PadHeight,self.origin)\
            .extrude(Base.Vector(0,0,self.__Thick))
        self.wiper = wiper
        bent=None
        if dir=='Left':
            orient = Base.Vector(0,1,1)
            bent=Part.makePlane(self.__fold,self.__PadWidth, \
                                self.origin.add(Base.Vector(self.__PadWidth,\
                                                            0,\
                                                            0)),\
                                orient).extrude(orient.multiply(self.__Thick))
        elif dir=='Right':
            orient = Base.Vector(0,1,-1)
            bent=Part.makePlane(self.__fold,self.__PadWidth, \
                                self.origin.add(Base.Vector(self.__PadWidth,\
                                                            self.__PadHeight,\
                                                            0)),\
                                orient).extrude(orient.multiply(self.__Thick))
        else:
            raise RuntimeError("Unknown direction: "+dir)
        self.wiper = self.wiper.fuse(bent)
    def show(self,doc=None):
        if doc==None:
            doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name);
        obj.Shape=self.wiper
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.72156,.52549,.04313])
        

if __name__ == '__main__':
    doc = App.getDocument('FRED')
    Gui.setActiveDocument(doc)
    Left = ContactWipers("LeftWiper",Base.Vector(-9.67+.25,-8.02-(3.5+6.255+.25),-0.781-(.008*25.4)))
    Left.show(doc)
    Right =  ContactWipers("RightWiper",Base.Vector(-9.67+.25,4.838,-0.781-(.008*25.4)))
    Right.show(doc)
    #Left = ContactWipersBent("LeftWiper",\
    #                         Base.Vector(-9.67+.25,\
    #                                     -8.02-(3.5+.25),\
    #                                     -0.781-(.008*25.4)),\
    #                         'Left')
    #Left.show(doc)
    #Right =  ContactWipersBent("RightWiper",\
    #                           Base.Vector(-9.67+.25,\
    #                                       4.838,\
    #                                       -0.781-(.008*25.4)),\
    #                           'Right')
    #Right.show(doc)
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewTop()
    
