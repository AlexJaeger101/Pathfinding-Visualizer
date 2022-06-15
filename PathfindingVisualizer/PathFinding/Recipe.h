#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <vector>
#include <string>
#include "Item.h"

class Recipe :public Trackable
{
public:
	Recipe(std::vector<std::pair<ItemType, Uint32>> inputs, ItemType output, double timeToProduce, int VPs, bool requiresUnit = true);
	Recipe(ItemType output, double timeToProduce, int VPs, bool requiresUnit = true);

	void addInput(std::pair<ItemType, Uint32> input);
	Uint32 getNumRequired(ItemType inputType) const;
	Uint32 getNumInputs() const { return mInputs.size(); };
	ItemType getOutputType() const { return mOutput; };
	double getTimeToProduce() const { return mTimeToProduce; };
	int getVPs() const { return mVPs; };
	bool requiresUnit() const { return mRequiresUnit; };
	std::string getDescription() const;

private:
	std::vector<std::pair<ItemType,Uint32>> mInputs;
	ItemType mOutput;
	double mTimeToProduce=0.0;
	int mVPs = 0;
	bool mRequiresUnit=true;
};