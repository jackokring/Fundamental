#include "Fundamental.hpp"


struct VCA : Module {
	enum ParamIds {
		LEVEL1_PARAM,
		LEVEL2_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		EXP1_INPUT,
		LIN1_INPUT,
		IN1_INPUT,
		EXP2_INPUT,
		LIN2_INPUT,
		IN2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};

	VCA();
	void step();
};


VCA::VCA() {
	params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}

static void stepChannel(const float *in, float level, const float *lin, const float *exp, float *out) {
	float v = getf(in) * level;
	if (lin)
		v *= clampf(*lin / 10.0, 0.0, 1.0);
	const float expBase = 50.0;
	if (exp)
		v *= rescalef(powf(expBase, clampf(*exp / 10.0, 0.0, 1.0)), 1.0, expBase, 0.0, 1.0);
	setf(out, v);
}

void VCA::step() {
	stepChannel(inputs[IN1_INPUT], params[LEVEL1_PARAM], inputs[LIN1_INPUT], inputs[EXP1_INPUT], outputs[OUT1_OUTPUT]);
	stepChannel(inputs[IN2_INPUT], params[LEVEL2_PARAM], inputs[LIN2_INPUT], inputs[EXP2_INPUT], outputs[OUT2_OUTPUT]);
}


VCAWidget::VCAWidget() {
	VCA *module = new VCA();
	setModule(module);
	box.size = Vec(15*6, 380);

	{
		Panel *panel = new LightPanel();
		panel->box.size = box.size;
		panel->backgroundImage = Image::load("plugins/Fundamental/res/VCA.png");
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 365)));

	addParam(createParam<Davies1900hBlackKnob>(Vec(27, 57), module, VCA::LEVEL1_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<Davies1900hBlackKnob>(Vec(27, 222), module, VCA::LEVEL2_PARAM, 0.0, 1.0, 0.5));

	addInput(createInput<PJ301MPort>(Vec(11, 113), module, VCA::EXP1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(54, 113), module, VCA::LIN1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(11, 156), module, VCA::IN1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(11, 276), module, VCA::EXP2_INPUT));
	addInput(createInput<PJ301MPort>(Vec(54, 276), module, VCA::LIN2_INPUT));
	addInput(createInput<PJ301MPort>(Vec(11, 320), module, VCA::IN2_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(54, 156), module, VCA::OUT1_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(54, 320), module, VCA::OUT2_OUTPUT));
}
