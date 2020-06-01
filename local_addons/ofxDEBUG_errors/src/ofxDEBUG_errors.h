#pragma once

#include "ofMain.h"

class ofxDEBUG_errors
{

protected:

	string info = "ERROR";
	vector <std::string> errors;

	int x = 10;
	int y = 20;

	int h = 22;

	int period = 30;//blink speed cycle in frames
	float pulse = .5f;
	ofColor c1, c2;

	bool bVisible = true;

public:

	//API
	void setVisible(bool b)
	{
		bVisible = b;
	}
	void setPosition(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	ofxDEBUG_errors()
	{
		c1.set(ofColor::black);
		c2.set(ofColor::red);
	};

	~ofxDEBUG_errors()
	{

	};

	void draw()
	{
		if (bVisible)
			draw(x, y);
	};

	void addError(std::string sClass, std::string sMethod, std::string sMsg)
	{
		std::string str;
		str += info;
		str += " [";
		str += ofToString(errors.size() + 1);
		str += "] ";
		str += sClass;//in what class the error happens
		str += "\t";
		str += sMethod;//in what method of the class
		str += "\t";
		str += sMsg;//path or message that we want to display

		errors.push_back(str);

		ofLogError("ofxDEBUG_errors") << "addError: " << str;
	};

	void draw(int x, int y)
	{
		bool b = ((ofGetFrameNum() % period) > period*pulse);

		for (int i = 0; i < errors.size(); i++)
		{
			ofDrawBitmapStringHighlight(
				errors[i],
				x, y + i * h,
				b ? c2 : c1, b ? c1 : c2);
		}
	};

};

