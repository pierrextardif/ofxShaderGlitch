//
//  CellManager.cpp
//  CellTypes
//
//  Created by Pierre Tardif on 27/03/2020.
//

#include "CellManager.hpp"

void CellManager::setup(){
    initGui();
}

//--------------------------------------------------------------
void CellManager::initGui(){
    
    float offsetValue = 1.0;
    cellGroup.setName("Cell");
    cellGroup.add(offset.set("offset", {offsetValue,offsetValue}, {-offsetValue,-offsetValue}, {offsetValue, offsetValue}));
    cellGroup.add(layers.set("Mask Layers ", 1, 0, 10));
}

//--------------------------------------------------------------
void CellManager::addUniforms(){
    shader->setUniform2f("u_OffsetCells", offset->x, offset->y);
    shader->setUniform1i("u_MaskLayers", layers);
}
