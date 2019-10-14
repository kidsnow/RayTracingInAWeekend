#pragma once

#include "hitable.h"

class HitableList : public Hitable
{
public:
	HitableList() {}
	HitableList(Hitable **l, int n) { list = l; listSize = n; }
	virtual bool Hit(const Ray& r, float tMin,
		float tMax, HitRecord& rec) const;

public:
	Hitable **list;
	int listSize;
};

bool HitableList::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = tMax;
	for (int i = 0; i < listSize; i++)
	{
		if (list[i]->Hit(r, tMin, closestSoFar, tempRec))
		{
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}