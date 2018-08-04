#ifndef QSORT_H
#define QSORT_H
#include <vector>
#include <iostream>

template <class T>
void swap(std::vector<T> &mylist, int loc1, int loc2)
{
	T temp1 = mylist[loc1];
	T temp2 = mylist[loc2];
	mylist[loc1] = temp2;
	mylist[loc2] = temp1;
}

template <class T, class Comparator>
int partition(std::vector<T> &mylist, int start, int end, int p, Comparator comp)
{
	T pivot = mylist[p];
	swap(mylist, p, end);
	int left = start; 
	int right = end - 1;
	while(left < right)
	{
		while(!comp(pivot, mylist[left]) && left < right)
			left++;
		while(!comp(mylist[right],pivot) && left < right)
			right--;
		if(left < right)
			swap(mylist, left, right);
	}
	if(comp(mylist[end], mylist[right]))
	{
		swap(mylist, right, end);
		return right;
	}
	else
	{
		return end;
	}
}

template <class T, class Comparator>
void qsort(std::vector<T> &mylist, int start, 
	      int end, Comparator comp)
{
	if(start >= end) 
		return;

	//MEDIAN OF 3
	int mid = ((int)start + (int)end)/2; 
	int p = mid;
	if((comp(mylist[start],mylist[mid]) && comp(mylist[end],mylist[start])) ||
		(comp(mylist[mid],mylist[start]) && comp(mylist[start],mylist[end])))
	{
		p = start;//end < start < mid   mid < start < end
	}
	else if((comp(mylist[start],mylist[mid]) && comp(mylist[mid],mylist[end])) ||
		(comp(mylist[end],mylist[mid]) && comp(mylist[mid],mylist[start])))
	{
		p = mid;// start < mid < end  or end < mid< start
	}
	else if((comp(mylist[start],mylist[end]) && comp(mylist[end],mylist[mid])) ||
		(comp(mylist[mid],mylist[end]) && comp(mylist[end],mylist[start])))
	{
		p = end;// start<end<mid  or  mid<end<start
	}
	int loc = partition(mylist, (int)start, (int)end, p, comp);
	qsort(mylist,(int)start,loc-1,comp);
	qsort(mylist,loc+1,(int)end,comp);
}

template <class T, class Comparator>
void QuickSort(std::vector<T> &myArray, Comparator comp)
{
	qsort(myArray,0, (int)myArray.size()-1, comp);
}

#endif