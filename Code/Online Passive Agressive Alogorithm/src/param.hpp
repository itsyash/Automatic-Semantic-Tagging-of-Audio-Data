///
/// @brief
/// @author
/// @date
///
#ifndef _PARAM_H_
#define _PARAM_H_
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "util.hpp"

class Param {
public:
        bool Exist(const std::string& key) const {
                std::map<std::string, std::string>::const_iterator iter = conf_.find(key);
                return (iter == conf_.end() ? false : true);
        }

        template <class T>
        T Get(const std::string& key) const {
                std::map<std::string, std::string>::const_iterator iter = conf_.find(key);
                if (iter == conf_.end()) {
                        util::exit_err_message("Unknown option : " + key);
                }
                return util::lexical_cast<T, std::string>(iter->second);
        }

        template <class T>
        void Set(const std::string& key, const T &value, bool rewrite = true) {
                if (rewrite || (!rewrite && conf_.find(key) == conf_.end())) {
                        conf_[key] = util::lexical_cast<std::string, T>(value);
                }
        }

        virtual void Init() { conf_.clear(); }

	void Show() {
		std::map<std::string, std::string>::iterator iter(conf_.begin());
		for (; iter != conf_.end(); ++ iter) {
			std::cout << iter->first << "=" << iter->second << std::endl;
		}
	}

protected:
        std::map<std::string, std::string> conf_;
};


#endif // #ifndef _PARAM_H_
