#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print_sin_cos(double x, std::ostream& stream = std::cout) {
	stream << x << " " << std::sin(x) << " " << std::cos(x) << "\n";
}

int main(int argc, char** argv) {
	std::vector<double> cmd_numbers;
	std::string in_file = "";
	std::string out_file = "";

	for(int i = 0; i < argc; i++) {
		std::string arg = argv[i];

		if (arg == "-n" && ++i < argc) { //skips the next argument in the loop
			cmd_numbers.push_back(std::stod(argv[i]));
		}
		else if ((arg == "-o" || arg == "--output") && ++i < argc) {
			out_file = argv[i];
		}
		else if ((arg == "-i" || arg == "--input") && ++i < argc) {
			in_file = argv[i];
		}
	}

	std::cout << "From command line:\n";
	for (double x : cmd_numbers) print_sin_cos(x);

	std::cout << "\nFrom stdin:\n";
	double x;
	while (std::cin >> x) print_sin_cos(x);

	if (!(in_file == "") && !(out_file == "")) {
		std::ifstream input(in_file);
		std::ofstream output(out_file);

		output << "From file " << in_file << "\n";

		double x;
		while (input >> x) {
			print_sin_cos(x, output);
		}
		input.close();
		output.close();
	}

	return 0;
}
