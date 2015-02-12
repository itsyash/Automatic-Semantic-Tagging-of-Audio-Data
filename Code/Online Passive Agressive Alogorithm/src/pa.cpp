///
/// @brief
/// @author
/// @date
///

#include <iostream>
#include <fstream>
#include <cmath>
#include "my_math.hpp"
#include "pa.hpp"
#include "util.hpp"
using namespace std;

namespace pa {
Rnum operator* (const Rnum* vr, const VIdValue& viv)
{
        Rnum x(0.0);
        for (size_t i = 0; i < viv.size(); ++ i) {
                x += viv[i].value_ * vr[viv[i].id_];
        }
        return x;
}

Rnum get_norm_square(const VIdValue& viv)
{
	Rnum x(0.0);
        for (size_t i = 0; i < viv.size(); ++ i) {
                x += viv[i].value_ * viv[i].value_;
        }
        return x;
}

void update_weight(Rnum* w, const VIdValue& viv, Rnum t)
{
        for (size_t i = 0; i < viv.size(); ++ i) {
                w[viv[i].id_] += t * viv[i].value_;
        }
}


void PA::Init(const Param& param)
{
	StoreParam(param);
}


void PA::StoreParam(const Param& param)
{
	if (param.Exist("corpus")) {
		corpus_file_ = param.Get<string>("corpus");
	}
	if (param.Exist("model")) {
		model_file_ = param.Get<string>("model");
	}
	if (param.Exist("verbose")) {
		verbose_ = param.Get<short>("verbose");
	}
	if (param.Exist("mode")) {
		if (param.Get<string>("mode") == "train") {
			run_mode_ = kTrain;
		} else if (param.Get<string>("mode") == "test") {
			run_mode_ = kTest;
		} else {
			util::exit_err_message("Error!! Unknown mode : " + param.Get<string>("mode"));
		}
	}
	if (param.Exist("cost")) {
		cost_ = param.Get<Rnum>("cost");
	}
	if (param.Exist("iter")) {
		iter_ = param.Get<size_t>("iter");
	}
	if (param.Exist("algorithm")) {
		if (param.Get<string>("algorithm") == "PA") {
			algo_ = kPA_O;
		} else if (param.Get<string>("algorithm") == "PA-I") {
			algo_ = kPA_I;
		} else if (param.Get<string>("algorithm") == "PA-II") {
			algo_ = kPA_II;
		} else {
			util::exit_err_message("Error!! Unknown algorithm : " + param.Get<string>("algorithm"));
		}
	}
	if (param.Exist("kernel")) {
		if (param.Get<string>("kernel") == "Linear") {
			kernel_ = kLinear;
		} else if (param.Get<string>("kernel") == "Polynomial") {
			kernel_ = kPolynomial;
		} else if (param.Get<string>("kernel") == "RBF") {
			kernel_ = kRBF;
		} else {
			util::exit_err_message("Error!! Unknown kernel : " + param.Get<string>("kernel"));
		}
	}
	if (param.Exist("polynomial_d")) {
		polynomial_d_ = param.Get<Rnum>("polynomial_d");
	}
	if (param.Exist("rbf_c")) {
		rbf_c_ = param.Get<Rnum>("rbf_c");
	}
}


void PA::Start()
{
#if 0
	corpus_.Show();
#endif
	if (run_mode_ == kTrain) {
		ReadCorpus(corpus_file_);
		Train();
	} else {
		ReadModel();
		ReadCorpus(corpus_file_);
		Test();
	}
}


void PA::Update()
{
	err_ = 0;
	loss_ = 0.0;
	vector<Sample>::iterator s(corpus_.sample_.begin());
	for (; s != corpus_.sample_.end(); ++ s) {
		UpdateSample(*s);
	}
}


Rnum PA::KernelProduct(const VIdValue& f1, const VIdValue& f2, Kernel kernel_)
{
	Rnum x(0.0);
	size_t i, j;
	if (kernel_ == kLinear || kernel_ == kPolynomial) {
		for (i = 0, j = 0; i < f1.size() && j < f2.size();) {
			if (f1[i].id_ < f2[j].id_) { ++ i; continue; }
			if (f1[i].id_ > f2[j].id_) { ++ j; continue; }
			if (f1[i].id_ == f2[j].id_) {
				x += f1[i].value_ * f2[j].value_;
				++i; ++ j;
			}
		}
		if (kernel_ == kPolynomial) {
			x = pow(x, polynomial_d_);
		}
	} else if (kernel_ == kRBF) {
		for (i = 0, j = 0; i < f1.size() && j < f2.size();) {
			if (f1[i].id_ < f2[j].id_) { 
				x += math::square(f1[i].value_);
				++ i;
				continue;
			}
			if (f1[i].id_ > f2[j].id_) {
				x += math::square(f2[j].value_);
				++ j;
				continue;
			}
			if (f1[i].id_ == f2[j].id_) {
				x += math::square(f1[i].value_ - f2[j].value_);
				++i; ++ j;
			}
		}
		x = pow(exp(-x), 2) / rbf_c_;
	}
	return x;
}


Rnum PA::Product(const VIdValue& feature, size_t n)
{
	Rnum x(0.0);
	VIdValue::iterator viv(iindex_[n].begin());
	for (; viv != iindex_[n].end(); ++ viv) {
		x += viv->value_ * KernelProduct(kfeature_[viv->id_], feature, kernel_);
	}
	return x;
}


bool PA::Predict(const Sample& sample, size_t *p_label, Rnum *loss)
{
#if 0
	cout << endl;
#endif
	Rnum *w, x, c_score(0.0), p_score(-1.0);
	for (size_t i = 0; i < label_num_; ++ i) {
		if (kernel_ == kNone) {
			w = &(w_[GetWeightIndex(i)]);
			x = w * sample.feature_;
		} else {
			x = Product(sample.feature_, i);
		}
		cout<<"-> "<<i<<"  "<<p_score<<endl;
		if (i == sample.label_) {
			c_score = x;
		} else if (x > p_score) {
			*p_label = i;
			p_score = x;
		}
#if 0
		cout << "[" << i << "]" << "," << x << endl;
#endif
	}
	cout<<endl;
	*loss = c_score - p_score;
	*loss = (*loss >= 1.0 ? 0.0 : 1 - *loss);
#if 0
	cout << sample.label_ << "," << *p_label << endl;
	cout << (c_score > p_score) << " : " << c_score << " > " << p_score << endl;
#endif
	return c_score > p_score;
}


void PA::AddIdValue(VIdValue& index, const IdValue& kv)
{
	index.push_back(kv);
}


void PA::UpdateSample(const Sample& sample)
{
	IdValue kv;
	size_t p_label(0);
	Rnum loss;
	Predict(sample, &p_label, &loss);
	Rnum *w, t(0);
	if (loss != 0) {
		if (algo_ == kPA_II) {
			t = loss / (2.0 * get_norm_square(sample.feature_));
		} else if (algo_ == kPA_I) {
			t = min(cost_, loss / (2.0 * get_norm_square(sample.feature_)));
		} else if (algo_ == kPA_O) {
			t = loss / (2.0 * get_norm_square(sample.feature_) + 1.0 / 2.0 / cost_);
		}
		if (kernel_ == kNone) {
			w = &(w_[GetWeightIndex(sample.label_)]);
			update_weight(w, sample.feature_, t);
			w = &(w_[GetWeightIndex(p_label)]);
			update_weight(w, sample.feature_, -t);
		} else {
			kv.Set(kfeature_.size(), t);
			AddIdValue(iindex_[sample.label_], kv);
			kv.value_ = -t;
			AddIdValue(iindex_[p_label], kv);
			kfeature_.push_back(sample.feature_);
		}
	}
	loss_ += loss;
	err_ += (loss < 1.0 ? 0 : 1);
#if 0
	cout.precision(4);
	cout << (c_score > score ? "O " : "X ");
	cout << idx_label_[sample.label_] << "/"
	     << c_score << ", "
	     << idx_label_[label] << "/"
	     << score
	     << ", loss=" << l << endl;
#endif
}


void PA::ExamineModel()
{
	size_t p_label(0);
	Rnum loss(0.0);
	err_ = 0;
	loss_ = 0.0;
	vector<Sample>::iterator s(corpus_.sample_.begin());
	for (; s != corpus_.sample_.end(); ++ s) {
		if (!Predict(*s, &p_label, &loss)) {
			++ err_;
		}
		loss_ += loss;
	}
}


void PA::TrainInit()
{
	w_.resize(feature_id_ * label_num_);
	fill(w_.begin(), w_.end(), 0.0);
	if (kernel_ != kNone) {
		iindex_.resize(label_num_);
	}
}


void PA::Train()
{
	TrainInit();
	cout << "\nTraining start : " << iter_ << " iteration" << endl;
	ExamineModel();
	cout.setf(ios_base::fixed);
	cout.precision(3);
	cout << "iter=0"
	     << ", cumerr=0"
	     << ", cumloss=0"
	     << ", err=" << err_
	     << ", loss=" << loss_
	     << endl;
	for (size_t i = 1; i <= iter_; ++ i) {
		cout << "iter=" << i;
		//random_shuffle(corpus_.sample_.begin(), corpus_.sample_.end());
		Update();
		cout.setf(ios_base::fixed);
		cout.precision(3);
		cout << ", cumerr=" << err_
		     << ", cumloss=" << loss_;
		ExamineModel();
		cout << ", err=" << err_
		     << ", loss=" << loss_
		     << endl;
	}
	cout << endl;
	WriteModel();
}


void PA::Test()
{
	size_t p_label(0);
	Rnum loss(0.0);
	vector<Sample>::iterator s(corpus_.sample_.begin());
	for (; s != corpus_.sample_.end(); ++ s) {
		if (Predict(*s, &p_label, &loss)) {
			p_label = s->label_;
		}
		cout.setf(ios_base::fixed);
		cout.precision(3);
		cout << loss << " "
		     << idx_label_[s->label_] << " "
		     << idx_label_[p_label] << endl;
	}
}


/// write model to outfile
void PA::WriteModel()
{
        cout << "Write model to \"" << model_file_ << "\"" << endl;
        ofstream ofs(model_file_.c_str());
	ofs << "# Training data : " << corpus_file_ << endl;
	ofs << "# Algorithm : " << (algo_ == kPA_O ? "PA" :
				    algo_ == kPA_I ? "PA-I" :
				    algo_ == kPA_II ? "PA-II" : "UNKNOWN") << endl;
	ofs << "# C = " << cost_ << endl;
	ofs << "# iteration = " << iter_ << endl;
        ofs << "kernel = " << kernel_ << endl;
	if (kernel_ != kNone) {
		ofs << "Polynomial_D = " << polynomial_d_ << endl;
		ofs << "RBF_C = " << rbf_c_ << endl;
	}
	ofs << "maxid = " << feature_id_ << endl;
        ofs << "label = " << label_num_ << endl;
	for (size_t i = 0; i < idx_label_.size(); ++ i) {
		ofs << idx_label_[i] << " " << i << endl;
	}
	ofs.setf(ios_base::scientific);
	if (kernel_ == kNone) {
		size_t l(0);
		for (size_t i = 0; i < label_num_; ++ i) {
			ofs << i << flush;
			for (size_t j = 0; j < feature_id_; ++ j) {
				ofs.setf(ios_base::fixed);
				ofs << " " << w_[l ++] << flush;
			}
			ofs << endl;
		}
	} else {
		ofs << kfeature_.size() << endl;
		for (size_t i = 0; i < kfeature_.size(); ++ i) {
			ofs << i;
			for (size_t j = 0; j < kfeature_[i].size(); ++ j) {
				ofs << " " << kfeature_[i][j].id_ << ":" << kfeature_[i][j].value_;
			}
			ofs << endl;
		}
		ofs << label_num_ << endl;
		for (size_t i = 0; i < label_num_; ++ i) {
			ofs << i;
			for (size_t j = 0; j < iindex_[i].size(); ++ j) {
				ofs << " " << iindex_[i][j].id_ << ":" << iindex_[i][j].value_;
			}
			ofs << endl;
		}
	}
        ofs.close();
}


/// read model from readfile
void PA::ReadModel()
{
        if (verbose_ > 0) {
                cout << "Read model from \"" << model_file_ << "\"" << endl;
        }
        istream& is = util::input_file_open(model_file_);
        string line, term;
	size_t label, n;
	while ('#' == is.peek()) {
		getline(is, line);
	}
	is >> term >> term >> label;
	set_kernel_(label);
	if (kernel_ != kNone) {
		is >> term >> term >> polynomial_d_;
		is >> term >> term >> rbf_c_;
	}
	is >> term >> term >> feature_id_;
	corpus_.max_id_ = feature_id_;
        is >> term >> term >> label_num_;
	idx_label_.resize(label_num_);
	for (size_t i = 0; i < label_num_; ++ i) {
		is >> term >> label;
		label_map_.insert(pair<string, size_t>(term, label));
		idx_label_[label] = term;
	}
	if (kernel_ == kNone) {
		w_.resize(label_num_ * feature_id_);
		size_t l(0);
		Rnum w;
		while (getline(is, line)) {
			stringstream sstream(line);
			sstream >> label;
			while (sstream >> w) {
				w_[l ++] = w;
			}
		}
	} else {
		IdValue feature;
		while (getline(is, line), line == "");
#if 0
		cout << line << endl;
#endif
		n = util::lexical_cast<size_t,string>(line);
		kfeature_.resize(n);
#if 0
		cout << n << endl;
#endif		
		for (size_t i = 0; i < kfeature_.size(); ++ i) {
			getline(is, line);
#if 0
			cout << line << endl;
#endif
			stringstream sstream(line);
			sstream >> n;
			while (sstream >> term) {
				feature = get_id_value(term);
				kfeature_[i].push_back(feature);
#if 0
				cout << term << "=" << feature.id_ << ":" << feature.value_ << " ";
#endif
			}
#if 0
			cout << endl;
#endif
		}
		getline(is, line);
		n = util::lexical_cast<size_t,string>(line);
		iindex_.resize(n);
#if 0
		cout << n << endl;
#endif
		for (size_t i = 0; i < iindex_.size(); ++ i) {
			getline(is, line);
#if 0
			cout << line << endl;
#endif
			stringstream sstream(line);
			sstream >> n;
			while (sstream >> term) {
				feature = get_id_value(term);
				iindex_[i].push_back(feature);
#if 0
				cout << term << "=" << feature.id_ << ":" << feature.value_ << " ";
#endif
			}
#if 0
			cout << endl;
#endif
		}
	}
        util::input_file_close(is);
}


/// read corpus data
void PA::ReadCorpus(const string& file)
{
        if (verbose_ > 0) {
                cout << "Read samples from : " << file << endl;
        }
	corpus_.file_name_ = file;
        istream& is = util::input_file_open(file);
        int c(0);
        if (verbose_ > 0) {
                cout << "Read samples : " << flush;
        }
        while (ReadSample(is)) {
                if (verbose_ && c ++ % 100 == 0) {
                        cout << "." << flush;
                }
        }
	corpus_.max_id_ = feature_id_;
	if (verbose_) {
                cout << corpus_.sample_.size() << endl;
	}
	if (run_mode_ == kTrain) {
		label_num_ = label_map_.size();
	}
        if (verbose_ > 0) {
		cout << "Label : " << label_num_ << endl;
		cout << "Max id : " << feature_id_ << endl;
	}
#if 0
        train_data_.Show();
#endif        
        util::input_file_close(is);
}

/// read one trainig sample and store it in corpus_
bool PA::ReadSample(istream& is)
{
        string line, label;
        Sample sample;
	size_t size(0);
	IdValue feature;
	map<string,size_t>::iterator m;
	getline(is, line);
#if 0
	cout << "@" << line << endl;
#endif
	if (line == "") return !is.eof();
	stringstream sstream(line);
	string term;
	sstream >> label;
	if (run_mode_ == kTrain) {
		m = label_map_.find(label);
		if (m == label_map_.end()) {
			idx_label_.push_back(label);
			size = idx_label_.size() - 1;
			label_map_.insert(pair<string,size_t>(label, size));
		} else {
			size = m->second;
		}
	} else {
		m = label_map_.find(label);
		if (m == label_map_.end()) {
			idx_label_.push_back(label);
			size = idx_label_.size() - 1;
			unk_map_.insert(pair<string,size_t>(label, size));
		} else {
			size = m->second;
		}
	}
	sample.label_ = size;
#if 0
	cout << sample.label_ << flush;
#endif
	while (sstream >> term) {
#if 0
		cout << " " << term << flush;
#endif
		feature = get_id_value(term);
		if (run_mode_ == kTrain) {
			if (feature.id_ >= feature_id_) {
				feature_id_ = feature.id_ + 1;
			}
		}
		sample.feature_.push_back(feature);
	}
#if 0
	cout << endl;
#endif
	corpus_.sample_.push_back(sample);
	return !is.eof();
}


void Corpus::Show()
{
	size_t i, j;
	for (i = 0; i < sample_.size(); ++ i) {
		cout << sample_[i].label_ << flush;
		for (j = 0; j < sample_[i].feature_.size(); ++ j) {
			cout << " "
			     << sample_[i].feature_[j].id_
			     << ":"
			     << sample_[i].feature_[j].value_;
		}
		cout << endl;
	}
}


/// extract id-value pair from string term
IdValue get_id_value(const string& term)
{
        size_t p;
        IdValue idvalue;
        if ((p = term.find(":")) == string::npos) {
                util::exit_err_message(term + " is not correct format..");
        } else {
                string a = term.substr(0, p);
                util::input_string_value(a, &(idvalue.id_));
                a = term.substr(p + 1);
                util::input_string_value(a, &(idvalue.value_));
#if 0
                cout << " " << idvalue.id_ << ":" << idvalue.value_ << flush;
#endif
        }
        return idvalue;
}
};
