#include <iostream>
#include <string>
#include <thread>
#include <vector>

struct datum {
	int start, end;
	double sum;
};

void harm(datum& p) {
	int start = p.start; int end = p.end;
	double sum = 0;
	for (int i = start; i < end; ++i) {
		sum += 1.0/i;
	}
	p.sum = sum;
}

int main(int argc, char** argv) {
	int nterms = (int) 1e9;
	int nthreads = 1;
	bool test = false;

	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--terms" && ++i < argc) nterms = std::stoi(argv[i]);
		else if (arg == "--threads" && ++i < argc) nthreads = std::stoi(argv[i]);
		else if (arg == "--test") test = true;
	}

	std::cerr << "Terms: " << nterms << "\nThreads: " << nthreads << "\n";

	std::vector<std::thread> threads;
	threads.reserve(nthreads);
	std::vector<datum> data(nthreads);

	for (size_t i = 0; i < nthreads; ++i) {
		data[i].start = 1 + (nterms/nthreads) * i;
		data[i].end = 1 + (nterms/nthreads) * (i + 1);
		threads.emplace_back(harm, std::ref(data[i]));
	}
	data.back().end = nterms + 1;

	for (std::thread &thread : threads) thread.join();

	double total = 0;
	for (datum &d : data) total += d.sum;

	std::cout << "Total = " << total << "\n";

	if (test) {
		datum test_data;
		test_data.start = 1; test_data.end = nterms + 1;

		harm(test_data);
		std::cout << "Test = " << test_data.sum << "\n";
	}

	return 0;
}
