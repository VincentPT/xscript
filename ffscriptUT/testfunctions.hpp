double square(double a) {
	return a * a;
}

double fx(double x) {
	return 2 * square(x) + 2;
}

double fx1(double x) {
	return 2 * -square(x) + 2;
}

double fa(double x, int n) {
	if (n < 0) {
		while (n < 0) {
			x = x / 10;
			n = n + 1;
		}
	}
	else if (n > 0) {
		while (n > 0) {
			x = x * 10;
			n = n - 1;
		}
	}
	return x;
}

float fx2(int number) {
	return 1.0f * number;
}

long fx3(int number) {
	long res = 1l;
	return res * number;
}

bool fx4(int a, bool b) {
	bool res;
	res = a && b;
	return res;
}

bool fx5(int a) {
	return !a;
}

bool fx6(bool a, float b, double c) {
	return a && b || !c;
}

bool fx7(bool a, float b, double c) {
	return a && b || fx(c) > 1000.0;
}

int fibonacci(int n) {
	if (n < 2) {
		return n;
	}
	return fibonacci(n - 2) + fibonacci(n - 1);
}

bool fx8(bool a, float b, double c) {
	return a && fibonacci(b) > 1000 || c > 10.0;
}