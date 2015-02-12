///
/// @brief
/// @author
/// @date
///

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
using namespace std;
namespace io = boost::iostreams;

#include "util.hpp"

namespace util {

/*!
 * @brief judge endian
 * @retval int : 0 -> little endian | 1 -> big endian
 */
int endianness_judge()
{
	int x = 0x00000001;
	if (*(char*)&x) {
		/* little endian. memory image 01 00 00 00 */
		//cout << "Little endian !" << endl;
		return 0; 
	}else {
		/* big endian. memory image 00 00 00 01 */
		//cout << "Big endian !" << endl;
		return 1;
	}
}


/*!
 * @brief open that support compressed file
 * @param const string &file
 * @retval istream& reference to stream of opened file
 */
istream& input_file_open(const string &file)
{
	istream *in;
	try {
		if (file.find(".gz") != string::npos) {
                        io::filtering_istream *fltr_is = new io::filtering_istream();
                        fltr_is->push(io::gzip_decompressor());
                        fltr_is->push(io::file_source(file));
                        in = fltr_is;
     		}
		else {
			in = (istream *) (new ifstream(file.c_str()));
			if (!*in) {
				cerr << "Failed to open " << file << " !!" << endl;
				cerr << "Does it really exist?" << endl;
				exit(1);
			}
		}
	} catch(out_of_range &er) {
		cerr << er.what() << endl;
		throw;
	}
	return *in;
}


/*!
 * @brief close for file stream opened by input_file_open
 * @param istream &in_fs
 * @retval void
 */
void input_file_close(const istream &in_fs)
{
	delete &in_fs;
}


/*!
 * @brief convert all characters in given string into lower case ones
 * @param string& str
 * @retval void
 */
void to_lower(string& str)
{
	for (string::size_type i = 0; i < str.size(); ++ i) {
		str[i] = tolower(str[i]);
	}
}


/*!
 * @brief convert all characters in given string into upper case ones
 * @param string& str
 * @retval void
 */
void to_upper(string& str)
{
	for (string::size_type i = 0; i < str.size(); ++ i) {
		str[i] = toupper(str[i]);
	}
}


/*!
 * @brief remove substring from string 
 * @param string& str string
 * @param const string& rm substring to be removed
 * @retval void
 */
void remove_str(string& str, const string& rm)
{
	string::size_type pos;
	while (string::npos != (pos = str.find(rm))) {
		str.erase(pos, rm.size());
	}
}


/*!
 * @brief exit after error massage
 * @param const string& errmsg
 * @retval void
 */
void exit_err_message(const string& errmsg)
{
	cout << errmsg << endl;
	exit(1);
}

/*!
 * @brief get number at random
 * @param int min
 * @param int max
 * @retval int
 */
int get_random_number(int min, int max)
{
	return min + static_cast<int>(1.0 * rand() * (max - min) / RAND_MAX);
}

} // namespace util {


#ifdef U_MAIN
int main(int argc, char *argv[])
{
	string file = argv[1];
	istream& ins = util::input_file_open(file);
	string term;
	while (ins >> term) {
		cout << term << endl;
	}
        util::input_file_close(ins);

	return 0;
}
#endif
