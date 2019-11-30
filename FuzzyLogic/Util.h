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

// Gets all the combinations of elements of different sets.
// Params:
// - inputs: A vector in which each position corresponds to a set of elements (represented as a
// vector itself).
// Returns: A vector in which each position represents a different combination of elements from the
// different sets in the 'inputs' parameter.
template<typename T>
static inline vector<vector<T>> Util::getCombinations(vector<vector<T>>& inputs)
{
	vector<vector<T>> combinations;
	vector<T> currentComb;
	getCombinations(inputs, combinations, currentComb, 0);
	return combinations;
}

// Recursively updates a vector so that it holds all the combinations of elements from different
// sets.
// Params:
// - inputs: A vector in which each position corresponds to a set of elements (represented as a
// vector itself).
// - combinations: Vector where all the combinations of elements from the different sets in the
// 'inputs' parameter will be stored when the recursion ends.
// - currentComb: Vector with the current combination of elements from the different sets in the
// 'inputs' parameter that is being formed as the recursion goes.
// - index: Position (that identifies a set of elements) within the 'inputs' parameter that is
// going to be explored in this instance of the recursive calls.
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

// Gets all the keys from a standard C++ map.
// Params:
// - mapIn: map from which to get all its keys.
// Returns: A vector containing all the keys from the given map.
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

// Gets all the values from a standard C++ map.
// Params:
// - mapIn: map from which to get all its values.
// Returns: A vector containing all the values from the given map.
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