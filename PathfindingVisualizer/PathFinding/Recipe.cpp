#include "Recipe.h"

using namespace std;

Recipe::Recipe(vector<pair<ItemType, Uint32>> inputs, ItemType output, double timeToProduce, int VPs, bool requiresUnit)
	:mInputs(inputs)
	,mOutput(output)
	,mTimeToProduce(timeToProduce)
	,mVPs(VPs)
	,mRequiresUnit(requiresUnit)
{
}

Recipe::Recipe(ItemType output, double timeToProduce, int VPs, bool requiresUnit)
	:mOutput(output)
	,mTimeToProduce(timeToProduce)
	,mVPs(VPs)
	,mRequiresUnit(requiresUnit)
{
}

void Recipe::addInput(pair<ItemType, Uint32> input)
{
	mInputs.push_back(input);
}

Uint32 Recipe::getNumRequired(ItemType inputType) const
{
	for (auto it : mInputs)
	{
		if (it.first == inputType)
			return it.second;
	}
	return 0;
}

string Recipe::getDescription() const
{
	string theString;
	for (Uint32 i = 0; i < mInputs.size(); i++)
	{
		Uint32 numItems = mInputs[i].second;
		for (Uint32 j = 0; j < numItems; j++)
		{
			theString += ITEM_CHARS[(Uint32)mInputs[i].first];
		}
	}
	return theString;
}
