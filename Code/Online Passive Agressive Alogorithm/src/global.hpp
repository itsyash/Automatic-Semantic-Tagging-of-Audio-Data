#ifndef _GLOBAL_H_
#define _GLOBAL_H_
///
/// @brief define global typedef
/// @author Fujii Yasuhisa
/// @date 2010/01/28
///

#include <vector>
#include <deque>
#include <sys/types.h>

typedef double Rnum;
typedef std::vector<Rnum> VRnum;
typedef std::vector<VRnum> VVRnum;
typedef std::vector<VVRnum> VVVRnum;

typedef Rnum* RnumP;
typedef std::vector<RnumP> VRnumP;
typedef std::vector<VRnumP> VVRnumP;
typedef std::vector<VVRnumP> VVVRnumP;

typedef std::vector<int> VInt;
typedef std::vector<VInt> VVInt;
typedef std::vector<VVInt> VVVInt;

typedef std::vector<uint> VUInt;

#endif // #ifndef _GLOBAL_H_
