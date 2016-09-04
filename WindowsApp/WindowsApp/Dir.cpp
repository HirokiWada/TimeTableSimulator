#include "Dir.h"

// �t�@�C���ꗗ�擾
// folder : �t�H���_�̐�΃p�X����͂Ƃ��� 
// �� : "D:\\Users\\Pictures\\"
vector<string> Dir::read(string folder){
	// �錾
	vector<string> fileList;
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	// �t�@�C���������̂��߂Ƀ��C���h�J�[�h�ǉ�
	// �� : "D:\\Users\\Pictures\\*.*"
	stringstream ss;
	ss << folder;
	string::iterator itr = folder.end();
	itr--;
	if(*itr != '\\') ss << '\\';
	ss << "*.*";

	// �t�@�C���T��
	// FindFirstFile(�t�@�C����, &fd);
	hFind = FindFirstFile(ss.str().c_str(), &fd);

	// �������s
	if(hFind == INVALID_HANDLE_VALUE){
		std::cout << "�t�@�C���ꗗ���擾�ł��܂���ł���" << std::endl;
		exit(1); // �G���[�I��
	}

	// �t�@�C���������X�g�Ɋi�[���邽�߂̃��[�v
	do{
		// �t�H���_�͏���
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			//�t�@�C���������X�g�Ɋi�[
			char *file = fd.cFileName;
			string str = file;
			fileList.push_back(str);
		}
	}while(FindNextFile(hFind, &fd)); //���̃t�@�C����T��
		
	// hFind�̃N���[�Y
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