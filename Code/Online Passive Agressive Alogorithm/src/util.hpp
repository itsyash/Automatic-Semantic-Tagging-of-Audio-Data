#ifndef _UTIL_H_
#define _UTIL_H_
///
/// @brief
/// @author
/// @date
///

#include <iostream>
#include <sstream>
#include <cstdlib>

namespace util {


/*!
 * @brief judge endian
 * @retval int : 0 -> little endian | 1 -> big endian
 */
int endianness_judge();

/*!
 * @brief open that support compressed file
 * @param const std::string &file
 * @retval std::istream& reference to stream of opened file
 */
std::istream& input_file_open(const std::string& file);

/*!
 * @brief close for file stream opened by input_file_open
 * @param std::istream &in_fs
 * @retval void
 */
void input_file_close(const std::istream& in_fs);

/*!
 * @brief convert all characters in given string into lower case ones
 * @param std::string& str
 * @retval void
 */
void to_lower(std::string& str);

/*!
 * @brief convert all characters in given string into upper case ones
 * @param std::string& str
 * @retval void
 */
void to_upper(std::string& str);

/*!
 * @brief remove substring from string 
 * @param std::string& str string
 * @param const std::string& rm substring to be removed
 * @retval void
 */
void remove_str(std::string& str, const std::string& rm);

/*!
 * @brief exit after error massage
 * @param const std::string& errmsg
 * @retval void
 */
void exit_err_message(const std::string& errmsg);

/*!
 * @brief get number at random
 * @param int min
 * @param int max
 * @retval int
 */
int get_random_number(int min, int max);

/*!
 * @brief free area pointed by pointers in given container
 * @param T cont container that contains pointers
 * @retval void
 */
template<class T> void container_free(T &cont)
{
#if 0
	int count = 0;
#endif
	typename T::iterator iter;
	for (iter = cont.begin(); iter != cont.end(); ++ iter) {
#if 0
		cout << count ++ << ":" << *iter << flush;
#endif
		if (*iter != NULL) { delete *iter; }
#if 0
		cout << "\tok" << endl;
#endif
	}
	cont.clear();
}


/*!
 * @brief free array area pointed by pointers in given container
 * @param T cont container that contains pointers to array area
 * @retval void
 */
template<class T> void container_free_vector(T &cont)
{
	typename T::iterator iter;
	for (iter = cont.begin(); iter != cont.end(); ++ iter) {
		if (*iter != NULL) {
			delete [] *iter;
		}
	}
	cont.clear();
}


/*!
 * @brief return value read from string using string stream
 * @param std::string str
 * @retval T value read from string
 */
template<class T> void input_string_value (std::string &str, T *value)
{
        std::stringstream sstream(str);
	sstream >> *value;
}


///
/// @brief swap endian
/// @param T *value value for swap
/// @retval void
///
template <class T> void endian_swap(T *value)
{
        char *p = (char *) value;
        int size = sizeof(*value);
        for (int i = 0; 2 * i < size; ++ i) {
                std::swap(p[i], p[size - i - 1]);
        }
}


template <class T, class S>
T lexical_cast(const S& source)
{
        T target;
        std::stringstream interpreter;
        if (!(interpreter << source && interpreter >> target)) {
                exit_err_message("Can't convert");
        }
        return target;
}

/*!
 * @brief get value at random
 * @param T min
 * @param T max
 * @retval T
 */
template <class T> T get_random_value(T min, T max)
{
	return min + static_cast<T>(1.0 * rand() * (max - min) / RAND_MAX);
}

}

#endif // #ifndef _UTIL_H_
