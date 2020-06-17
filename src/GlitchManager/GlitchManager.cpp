//
//  GlitchManager.cpp
//  CellTypes
//
//  Created by Pierre Tardif on 27/03/2020.
//

#include "GlitchManager.hpp"

void GlitchManager::setup() {
	setup(glm::vec2(ofGetWidth(), ofGetHeight()));
}

void GlitchManager::setup(glm::vec2 imgSize) {

	gui.clear();
	initGui();

	// ==== main shader ====
	string path = PATHTOSHADERS;
	path.append("main");

	shader.load(path);
	shader.setMillisBetweenFileCheck(200);

	cells.shader = &shader;

	resImg = imgSize;

	// ==== edge detection : gaussian ====
	f.allocate(resImg.x, resImg.y);

	path = PATHTOSHADERS;
	path.append("Gaussian/gaussian");
	gaussian.load(path);
	//edge detection non maximum / continuity
	nonMaxAndContinuity = false;

	// ==== feedback ====
	feedbackEdge.init(resImg);

}

//--------------------------------------------------------------
void GlitchManager::doReset() {
	typeT = 0;
	speedMoves = ofVec2f(1, 1);
	amntLinesColumns = ofVec2f(2, 2);
	typeE = 0;
	props.set(ofVec4f(0.25, 0.25, 0.75, 0.75));
	alphaGradiant = (0.8);
	gradiantColor = (ofColor(200, 255, 0));
	backgroundColor = (ofColor::turquoise);
	continuous = (false);
	thresholdNoise = (.7);

	cells.offset = ofVec2f(1.0, 1.0);
	cells.layers = 1;
	feedbackEdge.activateFeedback = false;
}

//--------------------------------------------------------------
void GlitchManager::initGui() {

	guiON = false;
	gui.setup();

	typeTiling.setName("Type of Tyling");
	typeEffect.setName("Type of Effect");

	typeTiling.add(typeT.set("type of tyling", 0, 0, 10));
	typeTiling.add(speedMoves.set("Speed", { 1,1 }, { -10, -10 }, { 10,10 }));
	typeTiling.add(amntLinesColumns.set("amnt of Lines // Columns", { 2,2 }, { 1,1 }, { 300,300 }));


	typeEffect.add(typeE.set("type of effect", 0, 0, 10));
	typeEffect.add(props.set("rectangle Size", ofVec4f(0.25, 0.25, 0.75, 0.75), ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1)));
	typeEffect.add(alphaGradiant.set("alphaGradiant", 0.8, 0, 1));
	typeEffect.add(gradiantColor.set("gradiantColor", ofColor(200, 255, 0)));
	typeEffect.add(backgroundColor.set("backgroundColor", ofColor::turquoise));
	typeEffect.add(continuous.set("continuous", false));
	typeEffect.add(thresholdNoise.set("thresholdNoise", .7, 0., 1.0));

	typeTiling.add(cells.cellGroup);
	typeEffect.add(feedbackEdge.feedbackGUI);
	bReset.set("reset", false);
	bEnable.set("enable", true);
	bEnableBlur.set("blur", true);

	params.setName("ofxShaderGlitch");
	params.add(bReset);
	params.add(bEnable);
	params.add(bEnableBlur);
	params.add(typeTiling);
	params.add(typeEffect);

	gui.add(params);


}

//--------------------------------------------------------------
void GlitchManager::begin() {

	//TODO:
	if (bReset)
	{
		bReset = false;
		doReset();
	}


	if (bEnable)
	{
		f.begin();
		ofClear(0);

		if (bEnableBlur) gaussian.begin();
	}

}

//--------------------------------------------------------------
void GlitchManager::end() {
	if (bEnable)
	{
		if (bEnableBlur) gaussian.end();

		f.end();

		// feedback
		if (feedbackEdge.activateFeedback)typeT = typeE = -1;

		feedbackEdge.begin();
		shader.begin();
		shader.setUniform2f("u_resImg", resImg);
		shader.setUniform1f("u_time", ofGetElapsedTimef());

		shader.setUniform1i("u_tilingType", typeT);
		shader.setUniform1i("u_effectType", typeE);
		shader.setUniform2f("u_speedLinesColumns", speedMoves->x, speedMoves->y);
		shader.setUniform2f("u_amntLinesColumns", amntLinesColumns->x, amntLinesColumns->y);
		shader.setUniform2f("u_gradStart", props->x, props->y);
		shader.setUniform2f("u_gradFinish", props->z, props->w);
		shader.setUniform1f("u_alphaGradiant", alphaGradiant);
		shader.setUniform4f("u_gradiantColor", gradiantColor->r / 255., gradiantColor->g / 255., gradiantColor->b / 255., gradiantColor->a / 255.);
		shader.setUniform4f("u_backgroundColor", backgroundColor->r / 255., backgroundColor->g / 255., backgroundColor->b / 255., backgroundColor->a / 255.);
		shader.setUniform1f("u_continuous", (continuous == true) ? 1. : 0.);
		shader.setUniform1f("u_maxContinuity", (nonMaxAndContinuity == true) ? 1. : 0.);
		shader.setUniform1f("u_thresholdNoise", thresholdNoise);
		cells.addUniforms();

		f.draw(0, 0);
		shader.end();
		feedbackEdge.end(resImg);
	}
}

