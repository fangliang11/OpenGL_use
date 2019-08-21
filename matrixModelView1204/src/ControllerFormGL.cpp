///////////////////////////////////////////////////////////////////////////////
// ControllerFormGL.cpp
// ====================
// Derived Controller class for OpenGL dialog window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2008-09-15
// UPDATED: 2018-03-01
///////////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;

#include "Window.h"
#include "ControllerGL.h"
#include "ControllerFormGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;

INT_PTR CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerFormGL::ControllerFormGL(ModelGL* model, ViewFormGL* view) : model(model), view(view)
{
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CLOSE
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::close()
{
    ::DestroyWindow(handle);                    // close it
    Win::log("Form dialog is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::create()
{
    // initialize all controls
    view->initControls(handle);

    // init the matrices
    model->setViewMatrix(0, 0, 10, 0, 0, 0);
    view->setViewMatrix(0, 0, 10, 0, 0, 0);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND 选择数据文件对话框
///////////////////////////////////////////////////////////////////////////////


int ControllerFormGL::command(int id, int command, LPARAM msg)
{
    switch(id)
    {
    case IDC_BUTTON_VIEW_RESET:
        if(command == BN_CLICKED)
        {
            model->setViewMatrix(0, 0, 10, 0, 0, 0);
            view->setViewMatrix(0, 0, 10, 0, 0, 0);
        }
        break;

    case IDC_BUTTON_MODEL_RESET:
        if(command == BN_CLICKED)
        {
            model->setModelMatrix(0, 0, 0, 0, 0, 0);
            view->setModelMatrix(0, 0, 0, 0, 0, 0);
        }
        break;

    case IDC_BUTTON_ABOUT:
        if(command == BN_CLICKED)
        {
            // open About dialog
            ::DialogBox((HINSTANCE)::GetWindowLongPtr(handle, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUT), handle, aboutDialogProcedure);
        }
        break;

	case IDC_BUTTON_OPEN:    
		if (command == BN_CLICKED)   //打开文件按钮
		{
			//ModelGL modelGL;
			//ViewGL viewGL;
			//Win::ControllerGL myControllerGL(&modelGL, &viewGL);

			//modelGL.CTRDRAWFLAG = false;  //设置回初始值

			//打开数据文件
			selectData();

			HWND hwnd = FindWindow(L"三维点云图生成软件", NULL);
			HWND hwndGL = GetWindow(hwnd, GW_CHILD);//获取 glWin 窗口的句柄, 即OpenGL窗口的句柄
			::SendMessage(hwndGL, WM_MBUTTONDOWN, 0, 0); //绘图标志位复位

		}
		break;
	case IDC_BUTTON_DRAW:
		if (command == BN_CLICKED)    // 重绘按钮
		{
			//ModelGL modelGL;
			//ViewGL viewGL;
			//Win::ControllerGL myControllerGL(&modelGL, &viewGL);

			//数据读取成功并解析后发送绘图消息
			if (READFINISHFLAG) {
				HWND hwnd = FindWindow(L"三维点云图生成软件", NULL);// 获取主窗口句柄 mainWin
				HWND hwndGL = GetWindow(hwnd, GW_CHILD);//获取 glWin 窗口的句柄, 即OpenGL窗口的句柄
				::SendMessage(hwndGL, WM_PAINT, 0, 0);//发送绘图消息

			}

			//InvalidateRect(hwnd, NULL, true);  //使用InvalidateRect函数触发WM_PAINT消息

		}
	case IDC_BUTTON_CALCULATE:
		if (command == BN_CLICKED)
		{
			//计算坐标最大最小值

		}
		break;
    }

    return 0;
}

int ControllerFormGL::selectData() {

	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
	ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
	ofn.lpstrFilter = TEXT("所有文件\0*.*\0.txt/.dat Flie\0*.txt;*.dat\0\0");//设置过滤
	ofn.nFilterIndex = 1;//过滤器索引
	ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(strFilename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("请选择数据文件");//使用系统默认标题留空即可
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项
	if (GetOpenFileName(&ofn))
	{
		readData("D:\\MYdata1126.dat");
		SELECTFINISHFLAG = true;
		MessageBox(NULL, strFilename, TEXT("已选择数据"), 0);
	}

	return 0;
}

int ControllerFormGL::readData(string filename) {

	ifstream myfile(filename);
	if (!myfile.is_open()) {
		MessageBox(NULL, TEXT("文件打开失败"), TEXT("错误"), 0);

		return 1;
	}

	vector<string> vec;
	vector<float> vectorX;
	vector<float> vectorY;
	vector<float> vectorZ;

	string temp;
	while (getline(myfile, temp))                    //利用getline（）读取每一行，并按照行为单位放入到vector
	{
		vec.push_back(temp);
	}
	myfile.close();
	ROWNUM = vec.size();
	//cout << " the num of row is: " << ROWNUM << endl;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		//cout << *it << endl;
		istringstream is(*it);                    //用每一行的数据初始化一个字符串输入流；
		string s;
		int pam = 1;                              //从第一列开始

		while (is >> s)                          //以空格为界，把istringstream中数据取出放入到依次s中
		{
			if (pam == 3)                       //获取第 P 列的数据
			{
				float r = atof(s.c_str());     //做数据类型转换，将string类型转换成float
				vectorX.push_back(r);
			}
			if (pam == 4) {
				float y = atof(s.c_str());
				vectorY.push_back(y);
			}
			if (pam == 5) {
				float z = atof(s.c_str());
				vectorZ.push_back(z);
			}

			pam++;

		}
	}

	float maxX, maxY, maxZ, minX, minY, minZ;
	maxX = *max_element(begin(vectorX), end(vectorX));
	minX = *min_element(begin(vectorX), end(vectorX));

	maxY = *max_element(begin(vectorY), end(vectorY));
	minY = *min_element(begin(vectorY), end(vectorY));

	maxZ = *max_element(begin(vectorZ), end(vectorZ));
	minZ = *min_element(begin(vectorZ), end(vectorZ));

	for (int i = 0; i < ROWNUM; i++) {
		ModelGL  myModelGL;
		float coeffX = 1.5*myModelGL.AXES_LEN / (maxX - minX); //求坐标系下的坐标数值
		coordinateX.push_back(coeffX * vectorX[i]);

		float coeffY = 1.5*myModelGL.AXES_LEN / (maxY - minY);
		coordinateY.push_back(coeffY * vectorY[i]);

		float coeffZ = 2 / (maxZ - minZ);  ////深度的变化范围对应坐标系Z轴2个单位的长度
		coordinateZ.push_back(coeffZ * (vectorZ[i] - minZ));

	}

	if (ROWNUM = sizeof(coordinateX)) {
		if(ROWNUM = sizeof(coordinateY)){
			if (ROWNUM = sizeof(coordinateZ)) {
				READFINISHFLAG = true;

			}
		}
	}
	else READFINISHFLAG = false;
	//cout << "MaxX and MinX is " << maxX << "    " << minX << endl;
	//cout << "MaxY and MinY is " << maxY << "    " << minY << endl;
	//cout << "MaxZ and MinZ is " << maxZ << "    " << minZ << endl;

	return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle horizontal scroll notification
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::hScroll(WPARAM wParam, LPARAM lParam)
{
    // check if the message comming from trackbar
    HWND trackbarHandle = (HWND)lParam;

    int position = HIWORD(wParam);              // current tick mark position
    if(trackbarHandle)
    {
        // get control ID
        int trackbarId = ::GetDlgCtrlID(trackbarHandle);

        switch(LOWORD(wParam))
        {
        case TB_THUMBTRACK:     // user dragged the slider
            //Win::log("trackbar: %d", position);
            // NOTE: view will update model component
            view->updateTrackbars(trackbarHandle, position);
            break;

        case TB_THUMBPOSITION:  // by WM_LBUTTONUP
            break;

        case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
            break;

        case TB_LINEDOWN:       // by VK_LEFT, VK_UP
            break;

        case TB_TOP:            // by VK_HOME
            break;

        case TB_BOTTOM:         // by VK_END
            break;

        case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
            break;

        case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
            break;

        case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
            // NOTE: view will update model component
            position = (int)::SendMessage(trackbarHandle, TBM_GETPOS, 0, 0);
            view->updateTrackbars(trackbarHandle, position);
            break;
        }
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_NOTIFY
// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::notify(int id, LPARAM lParam)
{
    // first cast lParam to NMHDR* to know what the control is
    NMHDR* nmhdr = (NMHDR*)lParam;
    HWND from = nmhdr->hwndFrom;
    NMUPDOWN* nmUpDown = 0;

    switch(nmhdr->code)
    {
    // UpDownBox notifications =========
    case UDN_DELTAPOS:         // the change of position has begun
        // cast again lParam to NMUPDOWN*
        //nmUpDown = (NMUPDOWN*)lParam;
        //return view->changeUpDownPosition(from, nmUpDown->iPos + nmUpDown->iDelta);
        break;

    default:
        break;
    }

    // handled notifications
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_TIMER notification
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::timer(WPARAM eventId, LPARAM callback)
{
    switch(eventId)
    {
    case IDT_TIMER:
        // not needed
        //view->updateMatrices();
        break;
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// dialog procedure for About window帮助对话框的处理
///////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        break;

    case WM_CLOSE:
        {
            ::EndDialog(hwnd, 0);
        }
        break;

    case WM_COMMAND:
        if(LOWORD(wParam) == IDC_OK && HIWORD(wParam) == BN_CLICKED)
        {
            ::EndDialog(hwnd, 0);
        }
        break;

    case WM_NOTIFY:
        NMHDR* nmhdr = (NMHDR*)lParam;
        HWND from = nmhdr->hwndFrom;
        if(from == ::GetDlgItem(hwnd, IDC_SYSLINK1) && (nmhdr->code == NM_CLICK || nmhdr->code == NM_RETURN))
        {
            // cast again lParam to NMLINK*
            NMLINK* nmlink = (NMLINK*)lParam;
            ::ShellExecute(0, L"open", nmlink->item.szUrl, 0, 0, SW_SHOW);
        }
        break;
    }

    return false;
}
