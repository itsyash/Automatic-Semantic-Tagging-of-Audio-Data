#ifndef _PA_H_
#define _PA_H_
///
/// @brief
/// @author
/// @date
///

#include <iostream>
#include "global.hpp"
#include "param.hpp"

namespace pa {
class IdValue {
public:
        IdValue() : id_(0), value_(0.0) {};
        virtual ~IdValue() {};

	const IdValue& Set(size_t id, Rnum value) {
		id_ = id;
		value_ = value;
		return *this;
	}

        size_t id_;
        Rnum value_;
private:
};
typedef std::vector<IdValue> VIdValue;
typedef std::vector<VIdValue> VVIdValue;
IdValue get_id_value(const std::string& term);


class Sample {
public:
        Sample() {};
        virtual ~Sample() {
#if 0
                std::cout << "Sample is destructed successfully" << std::endl;
#endif
        };

        void Clear() {
                feature_.clear();
        }

        size_t label_;
        VIdValue feature_;
};
typedef std::vector<Sample> VSample;


class Corpus {
public:
        Corpus() : max_id_(0) {};
        virtual ~Corpus() {
#if 0
                std::cout << "Corpus is destructed successfully" << std::endl;
#endif
        };

        void Show();
	size_t get_max_id_() { return max_id_; }

public:
        VSample sample_;
	size_t max_id_;
	std::string file_name_;
};


class PA {
public:
	static const Rnum kC = 1.0;
	static const Rnum kPolynomial_D = 2.0;
	static const Rnum kRBF_C = 1.0;
	static const int kMaxIteration = 1;
	enum RunMode {kTrain, kTest};
	enum Algo {kPA_O, kPA_I, kPA_II};
	enum Kernel {kNone=0x0, kLinear=0x1, kPolynomial=0x2, kRBF=0x3};

	PA() : cost_(kC),
	       polynomial_d_(kPolynomial_D),
	       rbf_c_(kRBF_C),
	       verbose_(1),
	       run_mode_(kTrain),
	       algo_(kPA_I),
	       label_num_(0),
	       feature_id_(0),
	       iter_(kMaxIteration),
	       err_(0),
	       loss_(0.0),
	       kernel_(kNone)
	{};

	PA(const Param& param) : cost_(kC),
				 polynomial_d_(kPolynomial_D),
				 rbf_c_(kRBF_C),
				 verbose_(1),
				 run_mode_(kTrain),
				 algo_(kPA_I),
				 label_num_(0),
				 feature_id_(0),
				 iter_(kMaxIteration),
				 err_(0),
				 loss_(0.0),
				 kernel_(kNone)
	{
		Init(param);
	}
	
	virtual ~PA() {};

	void Init(const Param& param);
	void Start();
	void Train();
	void Test();
	void ReadCorpus(const std::string& file);
	bool ReadSample(std::istream& is);
	void WriteModel(const std::string& outfile);
        void ReadModel(const std::string& readfile);

	size_t GetWeightIndex(size_t l) {
		return l * feature_id_;
	}

	void set_verbose_(short verbose) {
                verbose_ = verbose;
        }

	void set_kernel_(size_t k) {
		switch(k) {
		case kNone:
			kernel_ = kNone;
			break;
		case kLinear:
			kernel_ = kLinear;
			break;
		case kPolynomial:
			kernel_ = kPolynomial;
			break;
		case kRBF:
			kernel_ = kRBF;
			break;
		default:
			kernel_ = kNone;
		}
	}

protected:
	void StoreParam(const Param& param);
	void Update();
	void UpdateSample(const Sample& sample);
	void TrainInit();
	void ExamineModel();
	void WriteModel();
	void ReadModel();
	bool Predict(const Sample& sample, size_t *p_label, Rnum *los);
	Rnum Product(const VIdValue& feature, size_t n);
	Rnum KernelProduct(const VIdValue& f1, const VIdValue& f2, Kernel kernel_);
	void AddIdValue(VIdValue& index, const IdValue& kv);

protected:
	VRnum w_;					///< weight
	Rnum cost_;					///< aggressiveness parameter
	Rnum polynomial_d_;				///< kernel parameter for polynomial
	Rnum rbf_c_;					///< kernel parameter for rbf
	std::string corpus_file_;			///< corpus file name
	std::string model_file_;			///< model file name
	Corpus corpus_;					///< corpus data
	short verbose_;					///< verbose level
	std::map<std::string,size_t> label_map_;	///< label to index mapper
	std::map<std::string,size_t> unk_map_;		///< unkown labels which were not appear in training data to index mapper
	std::vector<std::string> idx_label_;		///< to store label name
	RunMode run_mode_;				///< running mode
	Algo algo_;					///< algorithm type
	size_t label_num_;				///< the number of lables
	size_t feature_id_;				///< the highest id of features
	size_t iter_;					///< max iteration number
	size_t err_;					///< lastly examined error count
	Rnum loss_;					///< lastly examined hinge loss
	VVIdValue kfeature_;				///< samples for kernel computation
	VVIdValue iindex_;				///< inverted index to get samples for required label
	Kernel kernel_;					///< kernel type
};
};
#endif // #ifndef _PA_H_
