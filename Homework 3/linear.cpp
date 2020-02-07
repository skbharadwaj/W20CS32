#include <iostream>
using namespace std;

bool anyFalse(const double a[], int n) {
        if(n == 0 || n < 0) return false;
	    if(!predicate(a[n-1])) return true;
        return anyFalse(a, n-1);
        return false;
}
int countTrue(const double a[], int n) {
        if(n == 0) return 0;
	    if(predicate(a[n-1])) return 1 + countTrue(a, n- 1);
        return countTrue(a, n - 1);
}
=
int firstTrue(const double a[], int n) {
        if(n == 0) return -1;
        if(predicate(a[0])) return 0;
        return 1 + firstTrue(a+1,n-1);
}
int positionOfSmallest(const double a[], int n)
{
        if(n <= 0) return -1;
        if(a == &a[n-1]) return 0;
        if(a[0] < a[n-1]) return positionOfSmallest(a, n - 1);
        else return 1 + positionOfSmallest(a + 1,n);
}
bool contains(const double a1[], int n1, const double a2[], int n2)
{
        if(a2 + 1 == &a2[n2] && a1[0] == a2[0]) return true;       
        if(a1 + 1 == &a1[n1]) return false;
        if(a1[0] == a2[0]) return contains(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
        else if(a1[0] != a2[0]) return contains(a1+1, n1 - 1, a2, n2);
	    return true;
}

