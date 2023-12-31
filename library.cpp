#define _WIN32_WINNT _WIN32_WINNT_WIN10
#define WINVER _WIN32_WINNT_WIN10

#include <Windows.h>
#include <winternl.h>
#include <ntstatus.h>
#include <vector>
#include <fstream>
#include <string>
#include <cwctype>
#include <algorithm>

#define MAX_ITERATIONS 10000

bool StringContainsSubstring(PUNICODE_STRING str, PUNICODE_STRING substr);

void CloneString(PUNICODE_STRING src, PUNICODE_STRING dst);

void DeleteString(PUNICODE_STRING string);

std::vector<PUNICODE_STRING> forbiddenWords;

extern "C" __declspec(dllexport) BOOLEAN __stdcall InitializeChangeNotify() {

    std::wifstream file(L"C:\\Windows\\PasswordFilter.txt");
    std::wstring line;
    int index = 0;

    while (std::getline(file, line) && index < MAX_ITERATIONS) {
        std::transform(line.begin(), line.end(), line.begin(), std::towlower);

        auto string = new UNICODE_STRING;
        string->Length = line.length();
        string->MaximumLength = string->Length + 1;
        string->Buffer = new wchar_t[string->MaximumLength + 1];
        string->Buffer[string->MaximumLength] = L'\0';
        wcsncpy_s(string->Buffer, string->MaximumLength + 1, line.c_str(), line.length());
        forbiddenWords.push_back(string);
        index++;
    }

    return TRUE;
}

extern "C" __declspec(dllexport) BOOLEAN __stdcall
PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation) {
    UNICODE_STRING lowercase;
    CloneString(Password, &lowercase);
    _wcslwr_s(lowercase.Buffer, lowercase.Length);
    bool match = false;
    for (auto &forbiddenWord: forbiddenWords) {
        if (StringContainsSubstring(&lowercase, forbiddenWord)) {
            match = true;
        }
    }
    DeleteString(&lowercase);
    return match ? FALSE : TRUE;
}

extern "C" __declspec(dllexport) NTSTATUS __stdcall
PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeId, PUNICODE_STRING NewPassword) {
    return STATUS_SUCCESS;
}

bool StringContainsSubstring(PUNICODE_STRING str, PUNICODE_STRING substr) {
    return wcsstr(str->Buffer, substr->Buffer) != nullptr;
}

void CloneString(PUNICODE_STRING src, PUNICODE_STRING dst) {
    auto *buffer = new wchar_t[src->Length + 1];
    wcsncpy_s(buffer, src->Length, src->Buffer, src->Length);
    buffer[src->Length] = '\0';
    RtlInitUnicodeString(dst, buffer);
}

void DeleteString(PUNICODE_STRING string) {
    SecureZeroMemory(string->Buffer, string->Length);
    delete[] string->Buffer;
}