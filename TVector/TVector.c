#include <stdlib.h>
#include <assert.h>
#include "TVector.h"
#include "check.h"

bool vector_init(struct TVector* aVector, size_t aSize)
	{
	if (aVector == NULL)
		return false;
	TVectorElement* new_values = NULL;
	if (aSize)
		{
		new_values = calloc(aSize, sizeof(TVectorElement));//prvni pocet prvku a pak veliksot bytech
		if (!new_values)
			return false;
		}

	aVector->iValues = new_values;
	aVector->iSize = aSize;
	free(new_values);
	return true;
	}

bool vector_init_random(struct TVector* aVector, size_t aSize)
	{
	if (aVector == NULL)
		return false;
	TVectorElement* new_values = NULL;
	if (aSize)
		{
		new_values = malloc(aSize * sizeof(TVectorElement));//prvni pocet prvku a pak veliksot bytech
		if (!new_values)
			{
			free(new_values);
			return false;
			}
		for (size_t i = 0; i < aSize; i++)
			new_values[i] = vector_element_random_value();
		}
	aVector->iValues = new_values;
	aVector->iSize = aSize;
	return true;
	}

bool vector_init_file(struct TVector* aVector, FILE* aInputFile)
	{
	if (!aVector || !aInputFile)
		return false;
	size_t new_size = 0;
	if (fscanf(aInputFile, "%zu", &new_size) != 1)
		{
		return false;
		}

	TVectorElement* new_values = NULL;
	if (new_size)
		{
		new_values = malloc(new_size * sizeof(TVectorElement));//prvni pocet prvku a pak veliksot bytech
		if (!new_values)
			return false;

		for (size_t i = 0; i < new_size; i++)
			if (!vector_element_load_file(&new_values[i], aInputFile))
				{
				free(new_values);
				return false;
				}

		}


	aVector->iValues = new_values;
	aVector->iSize = new_size;
	return true;
	free(new_values);
	}

bool vector_clone(const struct TVector* aVector, struct TVector* aVectorClone)
	{
	if (aVector == NULL || aVectorClone == NULL)
		return false;
	if (aVector == aVectorClone)
		return true;
	TVectorElement* new_values = NULL;
	if (aVector->iSize)
		{
		new_values = malloc(aVector->iSize * sizeof(TVectorElement));
		if (new_values == NULL)
			return false;
		for (size_t i = 0; i < aVector->iSize; i++)
			{
			new_values[i] = aVector->iValues[i];
			}

		}
	free(aVectorClone->iValues);
	aVectorClone->iValues = new_values;
	aVectorClone->iSize = aVector->iSize;
	free(new_values);
	return true;
	}

bool vector_store_file(const struct TVector* aVector, FILE* aOutputFile)
	{
	if (!aVector || !aOutputFile)
		return false;
	size_t new_size = 0;
	if (fprintf(aOutputFile, "%zu\n", aVector->iSize) > 0)
		{
		return false;
		}
	if (aVector->iValues)
		for (size_t i = 0; i < aVector->iSize; i++)
			if (!vector_element_store_file(aVector->iValues[i], aOutputFile))
				return false;
	return true;
	}

TVectorElement vector_value(const struct TVector* aVector, size_t aPos)
	{
	assert(aVector);
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);


	return  aVector->iValues[aPos];
	}

void vector_set_value(struct TVector* aVector, size_t aPos, TVectorElement aValue)
	{
	assert(aVector);
	assert(aVector->iValues);
	assert(aVector->iSize > aPos);

	aVector->iValues[aPos] = aValue;
	}

size_t vector_size(const struct TVector* aVector)
	{
	if (aVector)
		if (aVector->iValues)
			return aVector->iSize;



	return 0;
	}

bool vector_resize(struct TVector* aVector, size_t aNewSize)
	{
	if (!aVector)
		return false;
	if (aVector->iSize == aNewSize)
		{
		return true;
		}
	if (aNewSize == 0)
		{
		free(aVector->iValues);
		aVector->iSize = 0;
		aVector->iValues == NULL;
		return true;

		}
	TVectorElement* new_values = NULL;
	if (aVector->iSize > aNewSize)
		new_values = malloc(aNewSize * sizeof(TVectorElement));
	else
		new_values = calloc(aNewSize, sizeof(TVectorElement));
	if (!new_values)
		return false;
	size_t copy_size = (aVector->iSize > aNewSize) ? aNewSize : aVector->iSize;
	for (size_t i = 0; i < copy_size; i++)
		{
		new_values[i] = aVector->iValues[i];

		}
	aVector->iValues = new_values;


	}


void vector_destroy(struct TVector* aVector)
	{
	if (aVector)
		{
		free(aVector->iValues);
		aVector->iSize = 0;
		aVector->iValues = NULL;
		}



	}

struct TVectorIterator vector_iterator_begin(const struct TVector* aVector)
	{
	if (aVector)
		{
		return (struct TVectorIterator) { .iVector = aVector, .iPos = 0 };
		}
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_pos(const struct TVector* aVector, size_t aPos)
	{
	if (aVector && aPos < aVector->iSize)
		{
		return (struct TVectorIterator) { .iVector = aVector, .iPos = aPos };
		}
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_last(const struct TVector* aVector)
	{
	if (aVector && aVector->iSize != 0)
		{
		return (struct TVectorIterator) { .iVector = aVector, .iPos = aVector->iSize - 1 };
		}
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

bool vector_iterator_is_valid(const struct TVectorIterator* aIter)
	{
	if (aIter)
		if (aIter->iVector)
			if (aIter->iPos < aIter->iVector->iSize)
				return true;
	return false;
	}

bool vector_iterator_to_next(struct TVectorIterator* aIter)
	{
	if (!vector_iterator_is_valid(aIter))
		return false;
	++aIter->iPos;
	if (aIter->iPos >= aIter->iVector->iSize)
		{
		*aIter = (struct TVectorIterator){ .iVector = NULL, .iPos = 0 };
		return false;
		}
	return true;

	}

bool vector_iterator_to_prev(struct TVectorIterator* aIter)
	{
	if (!vector_iterator_is_valid(aIter))
		return false;

	if (aIter->iPos == 0)
		{
		*aIter = (struct TVectorIterator){ .iVector = NULL, .iPos = 0 };
		return false;
		}
	--aIter->iPos;
	return true;

	}

TVectorElement vector_iterator_value(const struct TVectorIterator* aIter)
	{
	if (!vector_iterator_is_valid(aIter))
		return (TVectorElement) { 0 };
	return aIter->iVector->iValues[aIter->iPos];
	}

bool vector_iterator_set_value(const struct TVectorIterator* aIter, TVectorElement aValue)
	{
	if (!vector_iterator_is_valid(aIter))
		return false;
	aIter->iVector->iValues[aIter->iPos] = aValue;
	return true;
	}
