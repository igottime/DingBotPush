#include "DUtil.h"

string string_To_UTF8(string* str)
{
    int nwLen = MultiByteToWideChar(CP_ACP, 0, str->c_str(), -1, NULL, 0);
    //一定要加1，不然会出现尾巴 
    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    ZeroMemory(pwBuf, nwLen * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, str->c_str(), str->length(), pwBuf, nwLen);
    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);
    WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    string strRet(pBuf);

    delete[]pwBuf;
    delete[]pBuf;
    pwBuf = NULL;
    pBuf = NULL;

    return strRet;
}

void postJson(const char* url, string* jsonBody) {
    string body = string_To_UTF8(jsonBody);
    CURL* curl = nullptr;
    CURLcode res;
    curl = curl_easy_init();
    if (curl != nullptr) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 3000);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3000);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

void redirectLog() {
    char path[MAX_PATH] = { 0 };
    char* cwd = _getcwd(NULL, MAX_PATH);
    if (cwd)
    {
        strcpy_s(path, cwd);
        strcat_s(path, "\\log.txt");
        FILE* file = NULL;
        freopen_s(&file,path,  "w", stdout);
        strcpy_s(path, cwd);
        strcat_s(path, "\\error.txt");
        freopen_s(&file,path, "w", stderr);
        free(cwd);
    }
}