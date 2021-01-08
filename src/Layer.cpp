#include "Layer.h"

Layer::Layer(string label, modulationType modType)
{
	_label = label;
	gainMod.modType = modType;
}

Layer::~Layer()
{
}