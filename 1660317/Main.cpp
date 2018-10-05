#include <iostream>
#include <windows.h>
#include <fcntl.h>
#include <string>
#include <io.h>
#include <fstream>
#include"HonDa.h"
#include"Map.h"
#include"Tien.h"
#include"Xe.h"
#include"XeDich.h"
using namespace std;

#pragma region kbBien
int diem = 0;
bool antien = false;
int tocTangThem = 200;
int soNguoiChoi = 0;
#pragma endregion

#pragma region CaiDat

//Hàm di chuyển con trỏ chuột
void MoveCursorToRoot()
{
	HANDLE consoleHandle; //Định danh của cửa sổ đang thao tác
	COORD coord;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = 0;	//tọa độ di chuyển tới
	coord.Y = 0;
	SetConsoleCursorPosition(consoleHandle, coord);
}


//Hàm chỉnh cỡ chữ
void setFontSize(int FontSize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = FontSize; // leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy_s(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
	_setmode(_fileno(stdout), _O_U16TEXT);
}

//Hàm chỉnh màu
void TextColor(int x)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

//Tạo kiểu dữ liệu người chơi
struct nguoichoi
{
	string ten;
	string diem;
};

//Hàm di chuyển xe
void DiChuyen(int &r, int &c, Map &map, Xe xe)
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		if (map.map[r][c - 3] != '|') //Điều kiện để xe không đi qua
		{					//bản đồ
			//Xóa xe
			map.map[r][c] = ' ';
			map.map[r][c + 1] = ' ';
			map.map[r][c - 1] = ' ';
			map.map[r + 1][c - 1] = ' ';
			map.map[r + 1][c + 1] = ' ';
			map.map[r - 1][c - 1] = ' ';
			map.map[r - 1][c + 1] = ' ';

			//Di chuyển
			c--;

			//Vẽ lại xe
			map.map[r][c] = xe.xe[1][1];
			map.map[r][c+1] = xe.xe[1][0];
			map.map[r][c-1] = xe.xe[1][2];
			map.map[r+1][c-1] = xe.xe[0][0];
			map.map[r+1][c+1] = xe.xe[0][2];
			map.map[r-1][c-1] = xe.xe[2][0];
			map.map[r-1][c+1] = xe.xe[2][2];
		}
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (map.map[r][c + 3] != '|') //Điều kiện để xe không đi qua
		{					//bản đồ
			//Xóa xe
			map.map[r][c] = ' ';
			map.map[r][c + 1] = ' ';
			map.map[r][c - 1] = ' ';
			map.map[r + 1][c - 1] = ' ';
			map.map[r + 1][c + 1] = ' ';
			map.map[r - 1][c - 1] = ' ';
			map.map[r - 1][c + 1] = ' ';

			//Di chuyển
			c++;

			//Vẽ lại xe
			map.map[r][c] = xe.xe[1][1];
			map.map[r][c + 1] = xe.xe[1][0];
			map.map[r][c - 1] = xe.xe[1][2];
			map.map[r + 1][c - 1] = xe.xe[0][0];
			map.map[r + 1][c + 1] = xe.xe[0][2];
			map.map[r - 1][c - 1] = xe.xe[2][0];
			map.map[r - 1][c + 1] = xe.xe[2][2];
		}
	}
	map.veMap();
}

//Hàm di chuyển xe đối thủ
void DiChuyenXeDich(int &r1, int &c1, Map &map, XeDich xedich)
{
	//Xóa xe đối thủ
	map.map[r1][c1] = ' ';
	map.map[r1][c1 + 1] = ' ';
	map.map[r1][c1 - 1] = ' ';
	map.map[r1 + 1][c1 - 1] = ' ';
	map.map[r1 + 1][c1 + 1] = ' ';
	map.map[r1 - 1][c1 - 1] = ' ';
	map.map[r1 - 1][c1 + 1] = ' ';
	
	//Di chuyển
	r1++;

	//Vẽ lại xe đối thủ
	map.map[r1][c1] = xedich.xeDich[1][1];
	map.map[r1][c1 + 1] = xedich.xeDich[1][0];
	map.map[r1][c1 - 1] = xedich.xeDich[1][2];
	map.map[r1 + 1][c1 - 1] = xedich.xeDich[0][0];
	map.map[r1 + 1][c1 + 1] = xedich.xeDich[0][2];
	map.map[r1 - 1][c1 - 1] = xedich.xeDich[2][0];
	map.map[r1 - 1][c1 + 1] = xedich.xeDich[2][2];

	// Đặt điều kiện để reset vị trí xe doi thu
	if (r1 > 20)
	{
		r1 = 0;
		c1 = rand() % 15 + 2;
		diem++;
		if (tocTangThem > 30)
			tocTangThem -= 5;
	}

	map.veMap();
}


void DiChuyenHonDa(int &rk1, int &ck1, Map &map, HonDa honda)
{
	//Xóa hòn đá
	map.map[rk1][ck1] = ' ';
	
	//Di chuyển
	rk1++;

	//Vẽ lại hòn đá
	map.map[rk1][ck1] = honda.honDa;

	// Đặt điều kiện để reset vị trí hon da
	if (rk1 > 20)
	{
		rk1 = 0;
		ck1 = rand() % 15 + 2;
		diem++;
		if (tocTangThem > 30)
			tocTangThem -= 5;
	}

}

void DiChuyenTien(int &rt1, int &ct1, Map &map, Tien tien)
{
	//Xóa tiền
	map.map[rt1][ct1] = ' ';

	//Dịch chuyển
	if (!antien)
		rt1++;
	
	//Vẽ lại tiền
	map.map[rt1][ct1] = tien.tien;

	// Đặt điều kiện để reset vị trí tiền
	if (rt1 > 20 || antien)
	{
		map.map[rt1][ct1] = ' ';
		rt1 = 0;
		ct1 = rand() % 15 + 2;
		antien = false;
	}
}

//Hàm kiểm  tra điều kiện thắng hoặc thua
bool ktThangThua(int &r, int &c, Map &map, HonDa &honda, XeDich &xedich)
{

	fstream BXH;
	BXH.open("BXH.txt", ios::app);
	//Điều kiền ăn tiền
	if (map.map[r - 2][c - 1] == '$' || map.map[r - 2][c + 1] == '$' || map.map[r - 3][c] == '$')
	{
		diem += 2;
		antien = true;
		if (tocTangThem > 30)
			tocTangThem -= 10;
	}
	else

	{
		//Điều kiện thua
		if ((  map.map[r - 1][c - 1] == xedich.xeDich[2][0]
			|| map.map[r - 1][c - 1] == xedich.xeDich[1][1]
			|| map.map[r - 1][c - 1] == xedich.xeDich[2][2]
			|| map.map[r - 1][c - 1] == xedich.xeDich[0][0]
			|| map.map[r - 1][c - 1] == xedich.xeDich[0][2]
			|| map.map[r - 1][c + 1] == xedich.xeDich[2][0]
			|| map.map[r - 1][c + 1] == xedich.xeDich[1][1]
			|| map.map[r - 1][c + 1] == xedich.xeDich[2][2]
			|| map.map[r - 1][c + 1] == xedich.xeDich[0][0]
			|| map.map[r - 1][c + 1] == xedich.xeDich[0][2]
			|| map.map[r][c] == xedich.xeDich[2][0]
			|| map.map[r][c] == xedich.xeDich[1][1]
			|| map.map[r][c] == xedich.xeDich[2][2]
			|| map.map[r][c] == xedich.xeDich[0][0]
			|| map.map[r][c] == xedich.xeDich[0][2])
			||(map.map[r - 1][c - 1] == honda.honDa
			|| map.map[r - 1][c + 1] == honda.honDa 
			|| map.map[r][c]		 == honda.honDa)) 
		{
			MoveCursorToRoot();
			TextColor(1);
			cout << endl << endl << endl << endl << endl << "              GAME OVER \n              SCORE: " << diem << "		     ";
			setFontSize(18);
			// Gọi hàm thiết lập Font và bật chế độ Unicode
			TextColor(4);
			// Hiển thị chuỗi tiếng Việt có dấu
			string name;
			BXH.seekg(0, ios::end);
			wcout << endl << L"\n              Kết thúc" << endl ;
			fflush(stdin);
			cin.ignore(0);
			wcout << L"     Nhập tên của bạn (khong dau)\n     "; getline(cin, name);
			BXH << name << "\n";
			BXH << diem;
			BXH << "\n";
			//system("pause");
			BXH.close();
			return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region BXH
//Hàm sắp xếp người chơi
void SXEP(nguoichoi *ds, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (ds[i].diem < ds[j].diem)
			{
				swap(ds[i], ds[j]);
			}
		}
	}
}

//Hàm lấy bảng xếp hạng người chơi
void layBXH(fstream &file, nguoichoi* ds)
{
	string temp;
	int dem = 0;
	while (file.eof() == false)
	{
		getline(file, temp);
		dem++;
	}
	soNguoiChoi = dem / 2;
	ds = new nguoichoi[soNguoiChoi];
	file.close();
	file.open("BXH.txt", ios::in);
	for (int i = 0; i < soNguoiChoi; i++)
	{
		getline(file, ds[i].ten);
		getline(file, ds[i].diem);
	}
	file.close();
	SXEP(ds, soNguoiChoi);
	file.open("BXH.txt", ios::out);
	for (int i = 0; i < soNguoiChoi; i++)
	{
		file << ds[i].ten;
		file << "\n";
		file << ds[i].diem;
		file << "\n";
	}
	file.close();
}
#pragma endregion


int main()
{
	Map map;
	MoveCursorToRoot();

	Xe xe;
	XeDich xedich;
	HonDa honda;
	Tien tien;
	nguoichoi* ds = NULL;
	fstream file;

#pragma region SetUp

	file.open("BXH.txt", ios::in);
	layBXH(file, ds);

	//Vẽ xe lên map
	int r = 18, c = 9;

	map.map[r][c] = xe.xe[1][1];
	map.map[r][c + 1] = xe.xe[1][0];
	map.map[r][c - 1] = xe.xe[1][2];
	map.map[r + 1][c - 1] = xe.xe[0][0];
	map.map[r + 1][c + 1] = xe.xe[0][2];
	map.map[r - 1][c - 1] = xe.xe[2][0];
	map.map[r - 1][c + 1] = xe.xe[2][2];

	//Vẽ xe địch lên map
	int r1 = 5, c1 = 7;
	map.map[r1][c1] = xedich.xeDich[1][1];
	map.map[r1][c1 + 1] = xedich.xeDich[1][0];
	map.map[r1][c1 - 1] = xedich.xeDich[1][2];
	map.map[r1 + 1][c1 - 1] = xedich.xeDich[0][0];
	map.map[r1 + 1][c1 + 1] = xedich.xeDich[0][2];
	map.map[r1 - 1][c1 - 1] = xedich.xeDich[2][0];
	map.map[r1 - 1][c1 + 1] = xedich.xeDich[2][2];

	//Vẽ hòn đá lên map
	int rk1 = 2, ck1 = 5;
	map.map[rk1][ck1] = honda.honDa;

	//Vẽ tiền lên map
	int rt1 = 0, ct1 = 16;
	map.map[rt1][ct1] = tien.tien;

	fstream filex;
	filex.open("BXH.txt", ios::in);
	string BXH;
	getline(filex, BXH, '\0');
	cout << BXH;
	filex.close();
#pragma endregion
	
	while (true)
	{
		MoveCursorToRoot();
		cout << "\nSCORE: " << diem << endl << "----------BANG XEP HANG--------\n"<< BXH;

		MoveCursorToRoot();
		
		Sleep(tocTangThem);

	MoveCursorToRoot();
	DiChuyen(r,c,map,xe);
	MoveCursorToRoot();
	DiChuyenXeDich(r1, c1, map, xedich);
	MoveCursorToRoot();
	DiChuyenHonDa(rk1, ck1, map, honda);
	MoveCursorToRoot();
	
	MoveCursorToRoot();
	map.veMap();

	if (ktThangThua(r, c, map, honda, xedich))
	{
		break;
	}
	DiChuyenTien(rt1, ct1, map, tien);


	} // end while
	
	system("pause");
	return 0;
}