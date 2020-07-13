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
	shader.setMillisBetweenFileCheck(200);//TODO:?required if we are not live editing?
	//shader.disableWatchFiles();//disable live reloading bc I will not edit the shaders..

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
void GlitchManager::initGui() {

	typeTiling.setName("A TYPE OF TYLING");
	typeEffect.setName("B TYPE OF EFFECT");

	typeTiling.add(typeT.set("TYPE OF TYLING", 0, 0, 10));
	typeTiling.add(speedMoves.set("SPEED", { 1,1 }, { -10, -10 }, { 10,10 }));
	typeTiling.add(amntLinesColumns.set("LINES / COLUMNS AMNT", { 2,2 }, { 1,1 }, { 300,300 }));

	typeEffect.add(typeE.set("TYPE OF EFFECT", 0, 0, 10));
	typeEffect.add(props.set("RECTANGLE SIZE", ofVec4f(0.25, 0.25, 0.75, 0.75), ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1)));
	typeEffect.add(alphaGradiant.set("ALPHA GRADIANT", 0.8, 0, 1));
	typeEffect.add(gradiantColor.set("GRADIANT COLOR", ofColor(200, 255, 0)));
	typeEffect.add(backgroundColor.set("BACKGROUND COLOR", ofColor::turquoise));
	typeEffect.add(continuous.set("CONTINUOUS", false));
	typeEffect.add(thresholdNoise.set("THRESHOLD NOISE", .7, 0., 1.0));

	typeTiling.add(cells.cellGroup);
	typeEffect.add(feedbackEdge.feedbackGUI);

	bEnable.set("ENABLE", true);
	bEnableBlur.set("BLUR", true);
	bReset.set("RESET", false);

	params.setName("ofxShaderGlitch");
	params.add(bEnable);
	params.add(bEnableBlur);
	params.add(bReset);
	params.add(typeEffectName.set("NAME", ""));
	params.add(typeTiling);
	params.add(typeEffect);

	//params_Control.setName("CONTROLS");
	//params_Control.add(bEnable);
	//params_Control.add(bReset);
	//params_Control.add(typeEffectName.set("NAME", ""));

	bReset.setSerializable(false);
	typeEffectName.setSerializable(false);

	//gui
	gui.setup();
	gui.add(params);
	guiON = false;
}

//--------------------------------------------------------------
void GlitchManager::refresh() {
	/*
	0 : glitch
	1 : texture Shift - works best withj tyling 7.
	2 : texture shift prop - shift on texture proportional to prop.Works best with tyling 8.
	3 : lateral shift : shift the texture horizontally following the amnt of lines // columns, to change the direction to horizontal, change the last argument of lateralSlider to true.
	4 : shift with noise per cell direction change controlled by continuous.
	5 & 6 : grad center + inner flip coords - rect boundaries are controlled by the rectangle size, and the amount of sections by amnt of lines // columns. The functions texFlipV in main.frag are used here, with the last 2 arguments adding a flip on the section coordinate, and a gradiant on the rectangle ( in this case true and true, therefore grad + flip is processed on the image).
	7 : bright lines sections of lines of different colors.the first argument gives the direction of the lines - in this case y - .The parameter continuous de activates the thresholding on gb, if ON, which makes it go less flashy.The Mask Layers control the amount of colors layers added.Works best with 2 layers.
	8: background color // invert background color - outside pattern detection.
	9 : gradiant color controlled by rectangle size and gradiant color, and alphaGradiant.
	10 : gradiant color + invert same as above with inverted color for inner rectangle section.
	*/

	switch (typeE.get())
	{
	case 0:
		typeEffectName = "GLITCH";
		break;
	case 1:
		typeEffectName = "TEXTURE SHIFT";
		break;
	case 2:
		typeEffectName = "TEXTURE SHIFT PROP";
		break;
	case 3:
		typeEffectName = "LATERAL SHIFT";
		break;
	case 4:
		typeEffectName = "SHIFT WITH NOISE";
		break;
	case 5:
	case 6:
		typeEffectName = "GRAD CENTER FLIP";
		break;
	case 7:
		typeEffectName = "BRIGHT LINES";
		break;
	case 8:
		typeEffectName = "BG INV PATTERN";
		break;
	case 9:
		typeEffectName = "GRADIANT COLOR RECT";
		break;
	case 10:
		typeEffectName = "GRADIANT COLOR INV";
		break;
	}
}

//--------------------------------------------------------------
void GlitchManager::begin() {
	//changed
	if (typeE.get() != typeE_PRE) {
		typeE_PRE = typeE;
		refresh();
	}

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

		//feedback
		if (feedbackEdge.activateFeedback)
			typeT = typeE = -1;

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

//--------------------------------------------------------------
void GlitchManager::doReset() {
	bEnableBlur = false;
	bEnable = false;

	typeT = 0;
	speedMoves = ofVec2f(1, 1);
	amntLinesColumns = ofVec2f(1, 1);

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

