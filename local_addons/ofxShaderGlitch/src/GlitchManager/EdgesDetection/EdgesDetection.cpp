//
//  EdgesDetection.cpp
//  CellTypes
//
//  Created by Pierre Tardif on 28/03/2020.
//

#include "EdgesDetection.hpp"


void EdgesDetection::setup(){
    initGui();
}

void EdgesDetection::initGui(){

    edgeGroup.setName("Edges");
    edgeGroup.add(thickness.set("Thickness", {1,1}, {0.1,0.1}, {3,3}));
       
}
void EdgesDetection::addUniforms(){
    shader->setUniform2f("u_ThicknessEdge", thickness->x, thickness->y);

}
