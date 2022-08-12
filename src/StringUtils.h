
#pragma once
#include <string>
#include <vector>
//#include "Windows.h"
class STRING_UTILS
{
public:
    //https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
    static std::vector<std::string> dirEntries(std::string& folder)
    {
        std::vector<std::string> names;
        std::string search_path = folder + "/*.*";
        WIN32_FIND_DATA fd;
        HANDLE hFind = ::FindFirstFile(string_toWchar(search_path), &fd);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                // read all (real) files in current folder
                // , delete '!' read other 2 default folder . and ..
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    names.push_back(wChar_toString(fd.cFileName));
                }
            } while (::FindNextFile(hFind, &fd));
            ::FindClose(hFind);
        }
        return names;
    }

    static wchar_t* string_toWchar(std::string a)
    {
        const size_t s = strlen(a.c_str()) + 1;
        wchar_t* ct = new wchar_t[s];
        mbstowcs(ct, a.c_str(), s);
        return ct;
    }

    static std::string wChar_toString(wchar_t* wt)
    {
        std::wstring ws(wt);
        std::string s(ws.begin(), ws.end());
        return s;
    }
};