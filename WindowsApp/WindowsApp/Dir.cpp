#include "Dir.h"

// ファイル一覧取得
// folder : フォルダの絶対パスを入力とする 
// 例 : "D:\\Users\\Pictures\\"
vector<string> Dir::read(string folder){
	// 宣言
	vector<string> fileList;
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	// ファイル名検索のためにワイルドカード追加
	// 例 : "D:\\Users\\Pictures\\*.*"
	stringstream ss;
	ss << folder;
	string::iterator itr = folder.end();
	itr--;
	if(*itr != '\\') ss << '\\';
	ss << "*.*";

	// ファイル探索
	// FindFirstFile(ファイル名, &fd);
	hFind = FindFirstFile(ss.str().c_str(), &fd);

	// 検索失敗
	if(hFind == INVALID_HANDLE_VALUE){
		std::cout << "ファイル一覧を取得できませんでした" << std::endl;
		exit(1); // エラー終了
	}

	// ファイル名をリストに格納するためのループ
	do{
		// フォルダは除く
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			//ファイル名をリストに格納
			char *file = fd.cFileName;
			string str = file;
			fileList.push_back(str);
		}
	}while(FindNextFile(hFind, &fd)); //次のファイルを探索
		
	// hFindのクローズ
	FindClose(hFind); 

	return fileList;
}

int FindChar(string str, char ch, int num)
{
	int res = 0;

	for(int i = 0; i < num; i++) {
		res = str.find(ch, (res + 1));
	}
	return res;
}

string Dir::GetStationNum(string fname)
{
	string res = fname;
	int start_pos = 0;
	int end_pos = FindChar(fname, '_', 1);
	
	res = fname.substr(start_pos, (end_pos - start_pos));
	return res;
}

string Dir::GetStationName(string fname)
{
	string res = fname;
	int start_pos = FindChar(fname, '_', 1);
	int end_pos = FindChar(fname, '_', 2);
	
	res = fname.substr(start_pos + 1, (end_pos - start_pos - 1));
	return res;
}

string Dir::GetRouteName(string fname)
{
	string res = fname;
	int start_pos = FindChar(fname, '_', 2);
	int end_pos = FindChar(fname, '_', 3);
	
	res = fname.substr(start_pos + 1, (end_pos - start_pos - 1));
	return res;
}


string Dir::GetDirectionName(string fname)
{
	string res = fname;
	int start_pos = FindChar(fname, '_', 3);
	int end_pos = FindChar(fname, '.', 1);
	
	res = fname.substr(start_pos + 1, (end_pos - start_pos - 1));
	return res;
}

Dir::Dir(void)
{
}


Dir::~Dir(void)
{
}