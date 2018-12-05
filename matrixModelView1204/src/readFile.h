#ifndef READFILE_H
#define READFILE_H

const int row_MAX = 10955;
const int column_MAX = 13;
float arry[row_MAX][column_MAX] = { 0 };  //定义文件的行数
float arryX[row_MAX] = { 0 };
float arryY[row_MAX] = { 0 };
float arryZ[row_MAX] = { 0 };
float maxX = 90.968;
float maxY = 82.571;
float maxZ = 3122.6;
float minX = -115.93;
float minY = -45.529;
float minZ = 2892.3;
float cvalueX, cvalueY, cvalueZ;

int readFiles(int AXES_LEN);


#endif