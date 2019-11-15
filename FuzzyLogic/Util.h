#pragma once
#include <map>
#include <vector>

using namespace std;

class Util
{
public:
	template <typename T>
	static vector<vector<T>> getCombinations(vector<vector<T>> &inputs);
	template <typename T, typename K>
	static vector<T> getKeysFromMap(map<T, K> mapIn);
	template <typename T, typename K>
	static vector<K> getValuesFromMap(map<T, K> mapIn);
private:
	template <typename T>
	static void getCombinations(vector<vector<T>> &inputs, vector<vector<T>> &combinations, vector<T> &currentComb, unsigned int index);
};

template<typename T>
static inline vector<vector<T>> Util::getCombinations(vector<vector<T>>& inputs)
{
	vector<vector<T>> combinations;
	vector<T> currentComb;
	getCombinations(inputs, combinations, currentComb, 0);
	return combinations;
}

template<typename T>
static inline void Util::getCombinations(vector<vector<T>>& inputs, vector<vector<T>>& combinations, vector<T>& currentComb, unsigned int index)
{
	for (unsigned int i = 0; i < inputs[index].size(); i++)
	{
		currentComb.push_back(inputs[index][i]);
		if (index == inputs.size() - 1)
		{
			combinations.push_back(currentComb);
		}
		else
		{
			getCombinations(inputs, combinations, currentComb, index + 1);
		}
		currentComb.pop_back();
	}
}

template <typename T, typename K>
static inline vector<T> Util::getKeysFromMap(map<T, K> mapIn)
{
	vector<T> keys;
	for (auto it = mapIn.begin(); it != mapIn.end(); ++it)
	{
		keys.push_back(it->first);
	}
	return keys;
}

template <typename T, typename K>
static inline vector<K> Util::getValuesFromMap(map<T, K> mapIn)
{
	vector<K> values;
	for (auto it = mapIn.begin(); it != mapIn.end(); ++it)
	{
		values.push_back(it->second);
	}
	return values;
}