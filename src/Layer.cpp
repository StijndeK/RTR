#include "Layer.h"

Layer::Layer(string label, modulationType modType = linear)
{
	_label = label;
	gainMod.modType = modType;
}

Layer::~Layer()
{
}