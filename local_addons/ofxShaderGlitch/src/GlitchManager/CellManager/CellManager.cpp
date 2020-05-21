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
    cellGroup.add(amountOfCells.set("Amount of Cells", {5,5}, {1,1}, {40,40}));
    cellGroup.add(offset.set("offset", {offsetValue,offsetValue}, {-offsetValue,-offsetValue}, {offsetValue, offsetValue}));
    cellGroup.add(layers.set("Mask Layers ", 1, 0, 10));
    cellGroup.add(backgroundColor.set("bckGrnd Color ", ofColor::black));
}

//--------------------------------------------------------------
void CellManager::addUniforms(){
    shader->setUniform2f("u_AmntCells", amountOfCells->x, amountOfCells->y);
    shader->setUniform2f("u_OffsetCells", offset->x, offset->y);
    shader->setUniform1f("u_MaskLayers", layers);
    shader->setUniform4f("u_BackGrndColor", backgroundColor->r / 255., backgroundColor->g / 255., backgroundColor->b / 255., backgroundColor->a / 255.);
}
