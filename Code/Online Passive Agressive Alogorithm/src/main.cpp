///
/// @brief The implementation of Online Passive Aggressive Algorithm
/// @author Fujii Yasuhisa
/// @date 2010/06/01
///
/// This program is an implemtation of "Online Passive-Aggressive
/// Algorithms" by Koby Crammer et al. (2006).
///
///

#include <iostream>
#include <iomanip>
using namespace std;

#include "pa.hpp"
#include "util.hpp"

namespace pa {
const static std::string kName = "Classifier based on Online Passive Aggressive algorithm";
const static std::string kVersion = "alpha";
const static std::string kAuthor = "Fujii Yasuhisa <fujii@slp.cs.tut.ac.jp>";

void output_info()
{
	cout << "# " << kName << flush;
        cout << " (ver. " << kVersion << ")" << endl;
        cout << "# Copyright(C) 2010- " << kAuthor << endl;
        cout << endl;
}

void set_parameter(Param *param, const vector<string>& opts);

/// analyze confing file and call get_options
void analyze_config(Param *param, const string& config)
{
#if 0
        cout << "Why" << endl;
#endif
        istream &is = util::input_file_open(config);
        vector<string> opts;
        size_t p;
        string line, term;
        while (getline(is, line)) {
                if ((p = line.find("#")) != string::npos) {
                        line = line.substr(0, p);
                }
                if (line.empty()) { continue; }
                stringstream sstream(line);
                while (sstream >> term) {
                        opts.push_back(term);
                }
        }
        set_parameter(param, opts);
}


/// set parameters from opts
void set_parameter(Param *param, const vector<string>& opts)
{
#if 0
	cout << "Options are below.." << endl;
	for (size_t i = 0; i < opts.size(); ++ i) {
		cout << opts[i]
		     << (opts[i][0] == '-' ? " " : "\n")
		     << flush;
	}
	cout << endl;
#endif
        for (size_t i = 0; i < opts.size(); ++ i) {
#if 0
                cout << i << ":" << opts[i] << endl;
#endif
                if (opts[i] == "-C" || opts[i] == "-config") {
                        analyze_config(param, opts[++ i]);
                } else if (opts[i] == "-corpus") {
                        param->Set<string>("corpus", opts[++ i]);
                } else if (opts[i] == "-model" || opts[i] == "-m") {
                        param->Set<string>("model", opts[++ i]);
                } else if (opts[i] == "-quiet" || opts[i] == "-q") {
                        param->Set<string>("verbose", "0");
		} else if (opts[i] == "-v" || opts[i] == "-verbose") {
			param->Set<string>("verbose", opts[++ i]);
                } else if (opts[i] == "-test") {
                        param->Set<string>("mode", "test");
                } else if (opts[i] == "-train") {
                        param->Set<string>("mode", "train");
                } else if (opts[i] == "-c" || opts[i] == "-cost") {
			param->Set<string>("cost", opts[++ i]);
		} else if (opts[i] == "-iter") {
			param->Set<string>("iter", opts[++ i]);
		} else if (opts[i] == "-a") {
			param->Set<string>("algorithm", opts[++ i]);
		} else if (opts[i] == "-kernel") {
			param->Set<string>("kernel", opts[++ i]);
		} else if (opts[i] == "-polynomial_d") {
			param->Set<string>("polynomial_d", opts[++ i]);
		} else if (opts[i] == "-rbf_c") {
			param->Set<string>("rbf_c", opts[++ i]);
		} else {
                        util::exit_err_message("Error!! Unknown option : " + opts[i]);
                }
        }
}


/// get option from command line
void get_options(int argc, char *argv[], Param *param)
{
        vector<string> opts;
        for (int i = 1; i < argc; ++ i) {
                opts.push_back(argv[i]);
        }
        set_parameter(param, opts);
}
};

int main(int argc, char *argv[])
{
        using namespace pa;
        Param param;
        get_options(argc, argv, &param);
	PA pa(param);
        string mode = param.Get<string>("mode");
        if (mode == "train") {
                output_info();
		cout << "Command line : " << flush;
		for (int i = 0; i < argc; ++ i) {
			cout << argv[i] << " ";
		}
		cout << "\n" << endl;
		cout << "Parameters are below.." << endl;
		param.Show();
		cout << endl;
                time_t t = time(0);
                pa.Start();
                cout << endl
                     << setprecision(2)
                     << "Total time : " << time(0) - t << "s" << endl;
        } else if (mode == "test") {
		pa.set_verbose_(0);
		pa.Start();
        } else {
                util::exit_err_message("You have to specify either one of the -(train|test) ..");
        }

        return 0;
}

