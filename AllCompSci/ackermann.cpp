#include "std_lib_facilities_4.h"

int ackermann(int m,int n) {
  if (m == 0) {
    return n + 1;
  }
  if (n == 0) {
    return ackermann(m - 1, 1);
  }
  return ackermann(m - 1, ackermann(m, n - 1));
}
 
int main() {
	int mmain,nmain;
	cout << "Please enter a value for m\n";
	cin >> mmain;
	cout << "Please enter a value for n\n";
	cin >> nmain;
	
  for (int m = 0; m <= mmain; ++m) {
    for (int n = 0; n <= nmain; ++n) {
      cout << "A(" << m << ", " << n << ") = " << ackermann(m, n) << "\n";
    }
  }
}
