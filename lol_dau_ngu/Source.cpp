#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <random>

using namespace std;

template<class T>
using uniform_distribution =
typename conditional<
	is_floating_point<T>::value,
	uniform_real_distribution<T>,
	typename conditional<
	is_integral<T>::value,
	uniform_int_distribution<T>,
	void
	>::type
>::type;

template <typename T>
T GetRand(T lower, T upper)
{
	random_device rd;
	uniform_distribution<T> dist(lower, upper);
	return dist(rd);
}

vector<string> get_all_files_names_within_folder(string folder)
{
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

string ReplaceTexts(string str, string szFind, string NewStr)
{
	return regex_replace(str, regex(szFind), NewStr);
}

int main()
{
	vector<string> listFiles = get_all_files_names_within_folder("input");
	for (auto lFile : listFiles)
	{
		string szDir = "input\\" + lFile;
		int nRandSec = GetRand<int>(65, 95);
		string szBoduoi = ReplaceTexts(lFile, ".mp4", "_");
		char szBuffer[8192] = {};
		sprintf(szBuffer, "ffmpeg -i \"%s\" -c copy -f segment -segment_time %d -reset_timestamps 1 \"output/%s%s\"", szDir.c_str(), nRandSec, szBoduoi.c_str(), "%03d.mp4");
		cout << szBuffer << endl;
		system(szBuffer);
	}
	system("pause");
	return 1;
}