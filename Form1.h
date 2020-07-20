#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <windows.h>
#include <msclr\marshal_cppstd.h>
#include "libxl.h"
#include <sstream>
#include <cstdlib>
#include <cwchar>
#include <memory>

using namespace libxl;
using namespace std;


namespace CppCLR_WinformsProjekt {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Buhgalter;

	paymentContract * CashFirst = NULL;
	paymentContract * CashLast = NULL;
	
	paymentContract * ContractFirst = NULL;
	paymentContract * ContractLast = NULL;
	interestRate *IRfirst;			//8
	interestRate *IRlast;			//

	int *colorNumber;

	int *FieldsMaxLength;
	ifstream f;
	string fname1 = "file1.txt";
	string fname2 = "file2.txt";
	string ** Fields;
	bool *isDeleted;
	bool *isChanged;
	int *numberOfNoteThatDeletedThis;
	int *numberOfNoteThatChangedThis;
	unsigned int * CRCField;
	string codeNumber = "";			//1
	string Prefix;					//2
	int formerNumber; int tempNumber;//3
	float maxSummFloat = 0;			//4
	float minSummFloat = 0;			//5
	int maxTermInDays = 0;			//6
	int minTermInDays = 0;			//7

	int state;
	
	string FirstDate;				//9
	int formerDD = 0;
	int formerMM = 0;
	int formerYYYY = 0;
	

	int lastYear = 0;
	int lastMonth = 0;
	int lastDay = 0;
	int lastHour = 0;
	int lastMin = 0;
	int lastSec = 0;

	int poryadok = 32;

	int maxQuantOfNotes = 100000;
	int ptr = 0;

	//для редактирования/удаления/новой строки
	int numberRUN = -1;
	int typeRUN;
	string contractNumberRUN;
	string date8RUN;
	string date9RUN;
	string FIORUN;
	string summRUN;
	string percentRUN;
	string textRUN;
	int stateRUN=0;
	Book* book;
	Sheet* sheet;

	uint_least32_t crc_table[256];

	uint_least32_t Crc32(unsigned char *buf, size_t len)
	{
		uint_least32_t crc;
		crc = 0xFFFFFFFFUL;
		while (len--)
			crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
		return crc ^ 0xFFFFFFFFUL;
	}

	//конвертирует символ c в восьмибитовый массив
	bool * converttobinary(char c)
	{
		int cc = c;
		int *result1;
		result1 = new int[8];
		int i = 0;
		do
		{
			result1[i] = cc % 2;
			cc = cc / 2;
			i++;
		} while (cc > 0);
		bool *result = new bool[8];
		int j = 0;
		for (; j < 8 - i; j++)
			result[j] = 0;
		for (; j < 8; j++)
		{
			if (result1[8 - j - 1] == 1)
				result[j] = 1;
			else
				result[j] = 0;
		}
		return result;
	}
	//проверяет, является ли символ цифрой в десятичной системе
	bool checkIfDecimal(char c)
	{
		switch (c)
		{
		case '1':
			return true;
			break;
		case '2':
			return true;
			break;
		case '3':
			return true;
			break;
		case '4':
			return true;
			break;
		case '5':
			return true;
			break;
		case '6':
			return true;
			break;
		case '7':
			return true;
			break;
		case '8':
			return true;
			break;
		case '9':
			return true;
			break;
		case '0':
			return true;
			break;
		default:
			return false;
		}
	};
	//возвращает цифру вместо символа, либо 10, если символ не цифра
	int retDecimal(char c)
	{
		switch (c)
		{
		case '1':
			return 1;
			break;
		case '2':
			return 2;
			break;
		case '3':
			return 3;
			break;
		case '4':
			return 4;
			break;
		case '5':
			return 5;
			break;
		case '6':
			return 6;
			break;
		case '7':
			return 7;
			break;
		case '8':
			return 8;
			break;
		case '9':
			return 9;
			break;
		case '0':
			return 0;
			break;
		default:
			return 10;
		}
	};

	bool ifNumber(string tempString)
	{
		int i;
		int N = tempString.length();
		if (N == 0)
			return false;
		bool ifDecimal;
		for (i = 0; i < N; i++)
		{
			ifDecimal = checkIfDecimal(tempString[i]);
			if (!ifDecimal)
				return false;
		}
		if (N > 1)
		{
			if (retDecimal(tempString[0]) == 0)
				return false;
		}
		return true;
	};
	int retNumber(string tempString)
	{
		int i, number = 0;
		int N = tempString.length();
		for (i = 0; i < N; i++)
		{
			number *= 10;
			number += retDecimal(tempString[i]);
		}
		return number;
	};
	
	//проверяет, является ли строка датой ДД.ММ.ГГГГ и меньше ли эта дата нынешней даты
	bool checkFirstDate(string s, bool needToCheckIfLessOrEqualThanNow)
	{
		if (s.length() != 10)
			return false;
		if (!(checkIfDecimal(s[0]) && checkIfDecimal(s[1]) && checkIfDecimal(s[3]) && checkIfDecimal(s[4])
			&& checkIfDecimal(s[6]) && checkIfDecimal(s[7]) && checkIfDecimal(s[8]) && checkIfDecimal(s[9])
			&& s[2] == '.' && s[5] == '.'))
			return false;
		int tempDD = 0;
		int tempMM = 0;
		int tempYYYY = 0;
		tempDD += retDecimal(s[0]) * 10;
		tempDD += retDecimal(s[1]);

		tempMM += retDecimal(s[3]) * 10;
		tempMM += retDecimal(s[4]);

		tempYYYY += retDecimal(s[6]); tempYYYY *= 10;
		tempYYYY += retDecimal(s[7]); tempYYYY *= 10;
		tempYYYY += retDecimal(s[8]); tempYYYY *= 10;
		tempYYYY += retDecimal(s[9]);

		if (needToCheckIfLessOrEqualThanNow)
		{
			time_t rawtime;
			struct tm * timeinfo;
			time(&rawtime);                         // текущая дата, выраженная в секундах
			timeinfo = localtime(&rawtime);        // текущая дата, представленная в нормальной форме

			if (1900 + timeinfo->tm_year < tempYYYY)
				return false;
			else if (1900 + timeinfo->tm_year == tempYYYY && 1 + timeinfo->tm_mon < tempMM)
				return false;
			else if ((1900 + timeinfo->tm_year == tempYYYY) && (1 + timeinfo->tm_mon == tempMM) && timeinfo->tm_mday < tempDD)
				return false;
		}
		if (tempMM < 1 || tempMM > 12)
			return false;

		int fDD = 28;
		switch (tempMM)
		{
		case 1:
			if (tempDD < 1 || tempDD > 31)
				return false;
			break;
		case 2:
			if (tempYYYY % 400 == 0)
				fDD = 29;
			else if (tempYYYY % 100 == 0)
				fDD = 28;
			else if (tempYYYY % 4 == 0)
				fDD = 29;
			if (tempDD < 1 || tempDD > fDD)
				return false;
			break;
		case 3:
			if (tempDD < 1 || tempDD > 31)
				return false;
			break;
		case 4:
			if (tempDD < 1 || tempDD > 30)
				return false;
			break;
		case 5:
			if (tempDD < 1 || tempDD > 31)
				return false;
			break;
		case 6:
			if (tempDD < 1 || tempDD > 30)
				return false;
			break;
		case 7:
			if (tempDD < 1 || tempDD > 31)
				return false;
			break;
		case 8:
			if (tempDD < 1 || tempDD > 31)
				return false;
			break;
		case 9:
			if (tempDD < 1 || tempDD > 30)
				return false;
			break;
		case 10:
			if (tempDD < 1 || tempDD > 31)
				return false;
			break;
		case 11:
			if (tempDD < 1 || tempDD > 30)
				return false;
			break;
		case 12:
			if (tempDD < 1 || tempDD > 31)
				return false;
			break;
		default:
			return false;
		}
		return true;
	};
	//проверяет, является ли строка датой ДД.ММ.ГГГГ, меньше ли эта дата нынешней даты и больше ли эта строка контрольной даты FirstDate
	bool checkDate(string s, bool needToCheckIfLessOrEqualThanNow)
	{
		if (!checkFirstDate(s, needToCheckIfLessOrEqualThanNow))
			return false;
		int tempDD = 0;
		int tempMM = 0;
		int tempYYYY = 0;
		tempDD += retDecimal(s[0]) * 10;
		tempDD += retDecimal(s[1]);

		tempMM += retDecimal(s[3]) * 10;
		tempMM += retDecimal(s[4]);

		tempYYYY += retDecimal(s[6]); tempYYYY *= 10;
		tempYYYY += retDecimal(s[7]); tempYYYY *= 10;
		tempYYYY += retDecimal(s[8]); tempYYYY *= 10;
		tempYYYY += retDecimal(s[9]);

		if (formerYYYY > tempYYYY)
			return false;
		else if (formerYYYY == tempYYYY && formerMM > tempMM)
			return false;
		else if (formerYYYY == tempYYYY && formerMM == tempMM && formerDD > tempDD)
			return false;

		return true;
	};
	//проверяет, является ли строка временем ЧЧ:ММ:СС
	bool checkTime(string s)
	{
		if (s.length() != 8)
			return false;
		if (!(checkIfDecimal(s[0]) && checkIfDecimal(s[1]) && checkIfDecimal(s[3]) && checkIfDecimal(s[4])
			&& checkIfDecimal(s[6]) && checkIfDecimal(s[7])
			&& s[2] == ':' && s[5] == ':'))
			return false;
		int tempHH = 0;
		int tempMM = 0;
		int tempSS = 0;
		tempHH += retDecimal(s[0]) * 10;
		tempHH += retDecimal(s[1]);

		tempMM += retDecimal(s[3]) * 10;
		tempMM += retDecimal(s[4]);

		tempSS += retDecimal(s[6]) * 10;
		tempSS += retDecimal(s[7]);

		if (tempHH < 0 || tempHH >23)
			return false;
		if (tempMM < 0 || tempMM >59)
			return false;
		if (tempSS < 0 || tempSS >59)
			return false;
		return true;
	};
	//проверяет, является ли строка суммой денег ХХХХХ.ХХ
	bool checkIfSumm(string s)
	{
		int N = s.length();
		if (N == 0)
			return false;
		int i, dot=-1;

		for (i = 0; i < N; i++)
		{
			if (s[i] == '.')
			{
				dot = i;
				break;
			}
		}
		if (dot != -1)
		{
			if (N < 3)
				return false;
			if (dot == 0)
				return false;
			if (dot == N - 1)
				return false;
						
			if (s[0] == '0')
				if (dot != 1)
					return false;
			for (i = 0; i < dot; i++)
				if (!checkIfDecimal(s[i]))
					return false;
			for (i = dot + 1; i < N; i++)
				if (!checkIfDecimal(s[i]))
					return false;
		}
		else
		{
			for (i = 0; i < N; i++)
				if (!checkIfDecimal(s[i]))
					return false;
		}

		return true;
	};
	//возвращает вместо строки сумму XXXXXXX.XX
	float retSumm(string s)
	{
		int N = s.length();
		int i, dot = -1;
		string beforeDot = "", afterDot = "";

		for (i = 0; i < N; i++)
		{
			if (s[i] == '.')
				dot = i;
			else if (dot == -1)
				beforeDot += s[i];
			else
				afterDot += s[i];
		}
		afterDot += "00";
		string rAfterDot = "";
		rAfterDot += afterDot[0];
		rAfterDot += afterDot[1];
		int beforeDotInt = retNumber(beforeDot);
		int afterDotInt = retNumber(rAfterDot);
		float summ = afterDotInt;
		summ /= 100;
		summ += beforeDotInt;
		return summ;
	};
	//проверяет, является ли введенная сумма подходящей (меньше максимальной и больше минимальной)
	bool checkSumm(string s)
	{
		if (!checkIfSumm(s))
			return false;
		float summ = retSumm(s);
		if (!(summ >= minSummFloat && summ <= maxSummFloat))
			return false;
		return true;
	};
	/*проверяет, чтобы в строке содержались только большие/маленькие символы русского алфавита, пробелы, дефисы и апострофы
	также проверяет, чтобы содержались три строчки, разделенные пробелом, каждая с большой буквы
	*/
	bool checkIfFIO(string s)
	{
		string rusAlphabetLittle = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
		string rusAlphabetBig = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
		int N = s.length();
		int i, j;
		int probel1=-1, probel2=-1;
		for (i = 0; i < N; i++)
		{
			if (s[i] == ' ')
			{
				if (probel1 == -1)
				{
					probel1 = i;
				}
				else if (probel2 == -1)
				{
					probel2 = i;
				}
				else
					return false;
			}
		}
		
		bool ret = false;
		for (i = 0; i < 33; i++)
		{
			if (s[0] == rusAlphabetBig[i])
			{
				ret = true;
				break;
			}
		}
		if (ret == false)
			return false;
		
		ret = false;
		for (i = 0; i < 33; i++)
		{
			if (s[probel1+1] == rusAlphabetBig[i])
			{
				ret = true;
				break;
			}
		}
		if (ret == false)
			return false;
		
		ret = false;
		for (i = 0; i < 33; i++)
		{
			if (s[probel2+1] == rusAlphabetBig[i])
			{
				ret = true;
				break;
			}
		}
		if (ret == false)
			return false;
		
		for (i = 1; i < N; i++)
		{
			ret = false;
			if (!(i == probel1 || i == probel1 + 1 || i == probel2 || i == probel2 + 1))
			{
				for (j = 0; j < 33; j++)
				{
					if (s[i] == rusAlphabetLittle[j])
					{
						ret = true;
						break;
					}
				}
				if (ret == false)
				{
					if (s[i] == '-' || s[i] == '\'')
						ret = true;
				}
				if (ret == false)
					return false;
			}
		}

		return true;
	};
	//проверяет, чтобы в строке не содержались символы /
	bool checkPrefix(string s)
	{
		int i;
		int N = s.length();
		for (i = 0; i < N; i++)
		{
			if (s[i] == '/')
				return false;
		}
		return true;
	};
	/*проверяет, являются ли Date датой, Time временем, больше ли Date чем FirstDate,
	а еще при необходимости проверяет, больше ли дата и время нынешней записи чем у предыдущей
	(чем lastYear+lastMonth+lastDay+lastHour+lastMin+lastSec )
	*/

	void makeLastDateTime(string Date, string Time)
	{
		lastDay = 0;
		lastMonth = 0;
		lastYear = 0;
		lastDay += retDecimal(Date[0]) * 10;
		lastDay += retDecimal(Date[1]);

		lastMonth += retDecimal(Date[3]) * 10;
		lastMonth += retDecimal(Date[4]);

		lastYear += retDecimal(Date[6]); lastYear *= 10;
		lastYear += retDecimal(Date[7]); lastYear *= 10;
		lastYear += retDecimal(Date[8]); lastYear *= 10;
		lastYear += retDecimal(Date[9]);

		lastHour = 0;
		lastMin = 0;
		lastSec = 0;
		lastHour += retDecimal(Time[0]) * 10;
		lastHour += retDecimal(Time[1]);

		lastMin += retDecimal(Time[3]) * 10;
		lastMin += retDecimal(Time[4]);

		lastSec += retDecimal(Time[6]) * 10;
		lastSec += retDecimal(Time[7]);
		return;
	};
	bool checkIfFirstDateEarlier(string fDate, string sDate)
	{
		int fDD=0, fMM=0, fYYYY=0, sDD=0, sMM=0, sYYYY=0;
		fDD += retDecimal(fDate[0]) * 10;
		fDD += retDecimal(fDate[1]);
		fMM += retDecimal(fDate[3]) * 10;
		fMM += retDecimal(fDate[4]);
		fYYYY += retDecimal(fDate[6]); fYYYY *= 10;
		fYYYY += retDecimal(fDate[7]); fYYYY *= 10;
		fYYYY += retDecimal(fDate[8]); fYYYY *= 10;
		fYYYY += retDecimal(fDate[9]);

		sDD += retDecimal(sDate[0]) * 10;
		sDD += retDecimal(sDate[1]);
		sMM += retDecimal(sDate[3]) * 10;
		sMM += retDecimal(sDate[4]);
		sYYYY += retDecimal(sDate[6]); sYYYY *= 10;
		sYYYY += retDecimal(sDate[7]); sYYYY *= 10;
		sYYYY += retDecimal(sDate[8]); sYYYY *= 10;
		sYYYY += retDecimal(sDate[9]);

		if (fYYYY > sYYYY)
			return false;
		else if (fYYYY == sYYYY && fMM > sMM)
			return false;
		else if (fYYYY == sYYYY && fMM == sMM && fDD > sDD)
			return false;
		return true;
	};
	
	//если fDate <= sDate, возвращает количество дней между fDate и sDate
	//если fDate > sDate, возвращает -1
	int retDaysBetween(string fDate, string sDate)
	{
		int fDD = 0, fMM = 0, fYYYY = 0, sDD = 0, sMM = 0, sYYYY = 0;
		fDD += retDecimal(fDate[0]) * 10;
		fDD += retDecimal(fDate[1]);
		fMM += retDecimal(fDate[3]) * 10;
		fMM += retDecimal(fDate[4]);
		fYYYY += retDecimal(fDate[6]); fYYYY *= 10;
		fYYYY += retDecimal(fDate[7]); fYYYY *= 10;
		fYYYY += retDecimal(fDate[8]); fYYYY *= 10;
		fYYYY += retDecimal(fDate[9]);

		sDD += retDecimal(sDate[0]) * 10;
		sDD += retDecimal(sDate[1]);
		sMM += retDecimal(sDate[3]) * 10;
		sMM += retDecimal(sDate[4]);
		sYYYY += retDecimal(sDate[6]); sYYYY *= 10;
		sYYYY += retDecimal(sDate[7]); sYYYY *= 10;
		sYYYY += retDecimal(sDate[8]); sYYYY *= 10;
		sYYYY += retDecimal(sDate[9]);


		int dCount = 0;
		int i;
		if (fYYYY > sYYYY)
			return -1;
		else if (fYYYY == sYYYY && fMM > sMM)
			return -1;
		else if (fYYYY == sYYYY && fMM == sMM && fDD > sDD)
			return -1;

		for (i = fYYYY; i <= sYYYY; i++)
		{
			if (i % 400 == 0)
				dCount += 366;
			else if (i % 100 == 0)
				dCount += 365;
			else if (i % 4 == 0)
				dCount += 366;
			else
				dCount += 365;
		}
		for (i = 1; i < fMM; i++)
		{
			switch (i)
			{
			case 1:
				dCount -= 31;
				break;
			case 2:
				if (fYYYY % 400 == 0)
					dCount -= 29;
				else if (fYYYY % 100 == 0)
					dCount -= 28;
				else if (fYYYY % 4 == 0)
					dCount -= 29;
				else
					dCount -= 28;
				break;
			case 3:
				dCount -= 31;
				break;
			case 4:
				dCount -= 30;
				break;
			case 5:
				dCount -= 31;
				break;
			case 6:
				dCount -= 30;
				break;
			case 7:
				dCount -= 31;
				break;
			case 8:
				dCount -= 31;
				break;
			case 9:
				dCount -= 30;
				break;
			case 10:
				dCount -= 31;
				break;
			case 11:
				dCount -= 30;
				break;
			}
		}
		
		for (i = sMM+1; i <= 12; i++)
		{
			switch (i)
			{
			case 2:
				if (sYYYY % 400 == 0)
					dCount -= 29;
				else if (sYYYY % 100 == 0)
					dCount -= 28;
				else if (sYYYY % 4 == 0)
					dCount -= 29;
				else
					dCount -= 28;
				break;
			case 3:
				dCount -= 31;
				break;
			case 4:
				dCount -= 30;
				break;
			case 5:
				dCount -= 31;
				break;
			case 6:
				dCount -= 30;
				break;
			case 7:
				dCount -= 31;
				break;
			case 8:
				dCount -= 31;
				break;
			case 9:
				dCount -= 30;
				break;
			case 10:
				dCount -= 31;
				break;
			case 11:
				dCount -= 30;
				break;
			case 12:
				dCount -= 31;
				break;
			}
		}
	
		int daysInMonthF;
		switch (fMM)
		{
		case 1:
			daysInMonthF = 31;
			break;
		case 2:
			if (fYYYY % 400 == 0)
				daysInMonthF = 29;
			else if (fYYYY % 100 == 0)
				daysInMonthF = 28;
			else if (fYYYY % 4 == 0)
				daysInMonthF = 29;
			else
				daysInMonthF = 28;
			break;
		case 3:
			daysInMonthF = 31;
			break;
		case 4:
			daysInMonthF = 30;
			break;
		case 5:
			daysInMonthF = 31;
			break;
		case 6:
			daysInMonthF = 30;
			break;
		case 7:
			daysInMonthF = 31;
			break;
		case 8:
			daysInMonthF = 31;
			break;
		case 9:
			daysInMonthF = 30;
			break;
		case 10:
			daysInMonthF = 31;
			break;
		case 11:
			daysInMonthF = 30;
			break;
		case 12:
			daysInMonthF = 31;
			break;
		}

		dCount -= fDD;
		
		dCount -= (daysInMonthF - sDD);
		return dCount;
	};
	bool checkDateTime(string Date, string Time, bool needToCheckPrevious, bool needToCheckLessThanNow)
	{
		if (!checkDate(Date, true))
			return false;
		int tempDD = 0;
		int tempMM = 0;
		int tempYYYY = 0;
		tempDD += retDecimal(Date[0]) * 10;
		tempDD += retDecimal(Date[1]);

		tempMM += retDecimal(Date[3]) * 10;
		tempMM += retDecimal(Date[4]);

		tempYYYY += retDecimal(Date[6]); tempYYYY *= 10;
		tempYYYY += retDecimal(Date[7]); tempYYYY *= 10;
		tempYYYY += retDecimal(Date[8]); tempYYYY *= 10;
		tempYYYY += retDecimal(Date[9]);
		if (!checkTime(Time))
			return false;
		int tempHH = 0;
		int tempMIN = 0;
		int tempSS = 0;
		tempHH += retDecimal(Time[0]) * 10;
		tempHH += retDecimal(Time[1]);

		tempMIN += retDecimal(Time[3]) * 10;
		tempMIN += retDecimal(Time[4]);

		tempSS += retDecimal(Time[6]) * 10;
		tempSS += retDecimal(Time[7]);

		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);                         // текущая дата, выраженная в секундах
		timeinfo = localtime(&rawtime);        // текущая дата, представленная в нормальной форме
		if (needToCheckLessThanNow)
		{
			if (1900 + timeinfo->tm_year == tempYYYY)
			{
				if (1 + timeinfo->tm_mon == tempMM)
				{
					if (timeinfo->tm_mday == tempDD)
					{
						if (timeinfo->tm_hour < tempHH)
							return false;
						if (timeinfo->tm_hour == tempHH && timeinfo->tm_min < tempMIN)
							return false;
						if (timeinfo->tm_hour == tempHH && timeinfo->tm_min == tempMIN && timeinfo->tm_sec < tempSS)
							return false;
					}
				}
			}
		}

		if (needToCheckPrevious)
		{
			if (lastYear > tempYYYY)
				return false;
			else if (lastYear == tempYYYY && lastMonth > tempMM)
				return false;
			else if (lastYear == tempYYYY && lastMonth == tempMM && lastDay > tempDD)
				return false;
			else if (lastYear == tempYYYY && lastMonth == tempMM && lastDay == tempDD && lastHour > tempHH)
				return false;
			else if (lastYear == tempYYYY && lastMonth == tempMM && lastDay == tempDD && lastHour == tempHH && lastMin > tempMIN)
				return false;
			else if (lastYear == tempYYYY && lastMonth == tempMM && lastDay == tempDD && lastHour == tempHH && lastMin == tempMIN && lastSec > tempSS)
				return false;
		}
		//if (!checkIfFirstDateEarlier(FirstDate, Date))
			//return false;
		return true;
	};
	/*проверяет, чтобы в контракте было "префикс/ГГММДДЧЧММ"
	и если надо, чтобы ГГММДДЧЧММ было больше, чем у предыдущей записи
	*/
	bool checkContract(string s, bool needToCheckPrevious)
	{
		int i;
		int N = s.length();
		string tempPrefix = "", tempDateTime = "";
		for (i = 0; i < N; i++)
		{
			if (s[i] != '/')
				tempPrefix += s[i];
			else
				break;
		}
		if (tempPrefix != Prefix)
			return false;

		string Date = "";
		Date += s[i + 5];
		Date += s[i + 6];
		Date += ".";
		Date += s[i + 3];
		Date += s[i + 4];
		Date += ".";
		Date += "20";
		Date += s[i + 1];
		Date += s[i + 2];

		string Time = "";
		Time += s[i + 7];
		Time += s[i + 8];
		Time += ":";
		Time += s[i + 9];
		Time += s[i + 10];
		Time += ":59";

		if (!checkDateTime(Date, Time, needToCheckPrevious, false))
			return false;
		return true;
	};


	string retDate()
	{
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);                         // текущая дата, выраженная в секундах
		timeinfo = localtime(&rawtime);        // текущая дата, представленная в нормальной форме

		string DD = to_string(timeinfo->tm_mday);
		if (timeinfo->tm_mday < 10)
		{
			string DDs = DD;
			DD = "0";
			DD += DDs;			
		}
		string MM = to_string(timeinfo->tm_mon+1);
		if (timeinfo->tm_mon + 1 < 10)
		{
			string MMs = MM;
			MM = "0";
			MM += MMs;
		}
		string YYYY = to_string(timeinfo->tm_year + 1900);

		string date = DD;
		date += ".";
		date += MM;
		date += ".";
		date += YYYY;
		return date;
	};
	string retTime()
	{
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);                         // текущая дата, выраженная в секундах
		timeinfo = localtime(&rawtime);        // текущая дата, представленная в нормальной форме

		string HH = to_string(timeinfo->tm_hour);
		if (timeinfo->tm_hour < 10)
		{
			string HHs = HH;
			HH = "0";
			HH += HHs;
		}
		string MM = to_string(timeinfo->tm_min);
		if (timeinfo->tm_min < 10)
		{
			string MMs = MM;
			MM = "0";
			MM += MMs;
		}
		string SS = to_string(timeinfo->tm_sec);
		if (timeinfo->tm_sec < 10)
		{
			string SSs = SS;
			SS = "0";
			SS += SSs;
		}

		string time = HH;
		time += ":";
		time += MM;
		time += ":";
		time += SS;
		return time;
	};

	void fillStandardFields(int tempNumber)
	{
		Fields[tempNumber - formerNumber][0] = codeNumber;
		Fields[tempNumber - formerNumber][1] = to_string(tempNumber);
		Fields[tempNumber - formerNumber][2] = retDate();
		Fields[tempNumber - formerNumber][3] = retTime();
		return;
	};
	/*
	напоминание - хочешь сделать CRC - скрепи запись и предыдущее CRC (для первой записи - 32 нуля)
	хочешь проверить CRC - скрепи запись и CRC этой записи и сделай CRC,
		   после чего проверь на соответствие с предыдущим CRC
	*/

	/// <summary>
	/// Zusammenfassung fьr Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufьgen.
			//
		}
	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  удалитьЗаписьToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  добавитьЗаписьToolStripMenuItem;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode fьr die Designerunterstьtzung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geдndert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->удалитьЗаписьToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->добавитьЗаписьToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->contextMenuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->listBox1->ContextMenuStrip = this->contextMenuStrip1;
			this->listBox1->DrawMode = System::Windows::Forms::DrawMode::OwnerDrawFixed;
			this->listBox1->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->listBox1->FormattingEnabled = true;
			this->listBox1->HorizontalExtent = 2048;
			this->listBox1->HorizontalScrollbar = true;
			this->listBox1->ItemHeight = 18;
			this->listBox1->Location = System::Drawing::Point(31, 46);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(1395, 382);
			this->listBox1->TabIndex = 0;
			this->listBox1->DrawItem += gcnew System::Windows::Forms::DrawItemEventHandler(this, &Form1::listBox1_DrawItem);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->toolStripMenuItem1,
					this->удалитьЗаписьToolStripMenuItem, this->добавитьЗаписьToolStripMenuItem
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(234, 76);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(233, 24);
			this->toolStripMenuItem1->Text = L"редактировать запись";
			this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::toolStripMenuItem1_Click);
			// 
			// удалитьЗаписьToolStripMenuItem
			// 
			this->удалитьЗаписьToolStripMenuItem->Name = L"удалитьЗаписьToolStripMenuItem";
			this->удалитьЗаписьToolStripMenuItem->Size = System::Drawing::Size(233, 24);
			this->удалитьЗаписьToolStripMenuItem->Text = L"удалить запись";
			this->удалитьЗаписьToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::удалитьЗаписьToolStripMenuItem_Click);
			// 
			// добавитьЗаписьToolStripMenuItem
			// 
			this->добавитьЗаписьToolStripMenuItem->Name = L"добавитьЗаписьToolStripMenuItem";
			this->добавитьЗаписьToolStripMenuItem->Size = System::Drawing::Size(233, 24);
			this->добавитьЗаписьToolStripMenuItem->Text = L"добавить запись";
			this->добавитьЗаписьToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::добавитьЗаписьToolStripMenuItem_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(31, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(265, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Вывести все записи";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(312, 13);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(265, 23);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Вывести займы и оплату займов";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(594, 13);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(265, 23);
			this->button3->TabIndex = 3;
			this->button3->Text = L"Вывести займы и задолженности";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->groupBox1->Controls->Add(this->button6);
			this->groupBox1->Controls->Add(this->button5);
			this->groupBox1->Controls->Add(this->button4);
			this->groupBox1->Controls->Add(this->textBox6);
			this->groupBox1->Controls->Add(this->comboBox2);
			this->groupBox1->Controls->Add(this->textBox5);
			this->groupBox1->Controls->Add(this->textBox4);
			this->groupBox1->Controls->Add(this->textBox3);
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->comboBox1);
			this->groupBox1->Location = System::Drawing::Point(31, 489);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(792, 228);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"добавление записи";
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(72, 47);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(75, 23);
			this->button6->TabIndex = 17;
			this->button6->Text = L"Выбрать тип записи";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(112, 188);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 16;
			this->button5->Text = L"отмена";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(7, 188);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(89, 23);
			this->button4->TabIndex = 5;
			this->button4->Text = L"добавить";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// textBox6
			// 
			this->textBox6->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->textBox6->Location = System::Drawing::Point(6, 160);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(758, 22);
			this->textBox6->TabIndex = 15;
			// 
			// comboBox2
			// 
			this->comboBox2->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(685, 45);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(48, 24);
			this->comboBox2->TabIndex = 14;
			// 
			// textBox5
			// 
			this->textBox5->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->textBox5->Location = System::Drawing::Point(498, 47);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(114, 22);
			this->textBox5->TabIndex = 13;
			// 
			// textBox4
			// 
			this->textBox4->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->textBox4->Location = System::Drawing::Point(241, 100);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(523, 22);
			this->textBox4->TabIndex = 12;
			// 
			// textBox3
			// 
			this->textBox3->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->textBox3->Location = System::Drawing::Point(340, 47);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(114, 22);
			this->textBox3->TabIndex = 11;
			// 
			// textBox2
			// 
			this->textBox2->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->textBox2->Location = System::Drawing::Point(191, 48);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(114, 22);
			this->textBox2->TabIndex = 10;
			// 
			// textBox1
			// 
			this->textBox1->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->textBox1->Location = System::Drawing::Point(10, 100);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(208, 22);
			this->textBox1->TabIndex = 9;
			// 
			// label8
			// 
			this->label8->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(337, 140);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(98, 17);
			this->label8->TabIndex = 8;
			this->label8->Text = L"Комментарий";
			// 
			// label7
			// 
			this->label7->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(640, 22);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(137, 17);
			this->label7->TabIndex = 7;
			this->label7->Text = L"Процентная ставка";
			// 
			// label6
			// 
			this->label6->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(529, 22);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(50, 17);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Сумма";
			// 
			// label5
			// 
			this->label5->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(480, 80);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(42, 17);
			this->label5->TabIndex = 5;
			this->label5->Text = L"ФИО";
			// 
			// label4
			// 
			this->label4->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(321, 22);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(159, 17);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Дата окончания срока";
			// 
			// label3
			// 
			this->label3->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(188, 22);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(92, 17);
			this->label3->TabIndex = 3;
			this->label3->Text = L"Дата записи";
			// 
			// label2
			// 
			this->label2->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(7, 74);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(211, 17);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Номер договора без префикса";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(36, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(83, 17);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Тип записи";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"1", L"2", L"5", L"6" });
			this->comboBox1->Location = System::Drawing::Point(7, 45);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(48, 24);
			this->comboBox1->TabIndex = 0;
			// 
			// button7
			// 
			this->button7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button7->Location = System::Drawing::Point(31, 447);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(383, 23);
			this->button7->TabIndex = 5;
			this->button7->Text = L"Создать файл xls для займов";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// button8
			// 
			this->button8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button8->Location = System::Drawing::Point(425, 447);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(383, 23);
			this->button8->TabIndex = 6;
			this->button8->Text = L"Создать файл xls для расхода/внесения денег в кассу";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::button8_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1452, 729);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->listBox1);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->contextMenuStrip1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		string p;
		int i, j;
		
		FieldsMaxLength = new int[13];
		/*	надо сразу установить максимумы длины для всех полей,
			которые будут выводиться в листбокс					*/
		// номер записи в пункте - 10
		FieldsMaxLength[1] = 10;
		// дата ввода - 10
		FieldsMaxLength[2] = 10;
		// время ввода - 8
		FieldsMaxLength[3] = 8;
		// тип записи - 1
		FieldsMaxLength[4] = 1;
		// номер записи в пункте - 10
		FieldsMaxLength[5] = 10;
		// номер договора - 20
		FieldsMaxLength[6] = 20;
		// дата - 10
		FieldsMaxLength[7] = 10;
		// дата - 10
		FieldsMaxLength[8] = 10;
		// фио - 50
		FieldsMaxLength[9] = 50;
		// сумма денег - 9
		FieldsMaxLength[10] = 9;
		// процентная ставка - 5
		FieldsMaxLength[11] = 5;
		// произвольный текст - 65
		FieldsMaxLength[12] = 65;

		
		f.open(fname1);

		int countFieldSetting = 1;
		if (f.is_open())
		{
			string tempString = "";
			char c;
			while (!f.eof())
			{
				f.get(c);
				if (!f.eof())
				{
					if (c == 0x0D || c == 0x0A)
					{
						if (countFieldSetting == 9)
						{
							MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							goto metka1;
						}
						switch (countFieldSetting)
						{
						case 1:
							if (ifNumber(tempString))
							{
								codeNumber = tempString;
							}
							else
							{
								MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							tempString = "";
							countFieldSetting++;
							break;
						case 2:
							if (checkPrefix(tempString))
							{
								if (tempString.length() > FieldsMaxLength[4] - 11)
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
								Prefix = tempString;
							}
							else
							{
								MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							tempString = "";
							countFieldSetting++;
							break;
						case 3:
							if (ifNumber(tempString))
							{
								formerNumber = retNumber(tempString);
								tempNumber = formerNumber;
							}
							else
							{
								MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							tempString = "";
							countFieldSetting++;
							break;
						case 5:
							//проверка maxSumm и minSumm
							if (checkIfSumm(tempString))
							{
								minSummFloat = retSumm(tempString);
								if (minSummFloat < 0)
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
								if (maxSummFloat < minSummFloat)
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else
							{
								MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							tempString = "";
							countFieldSetting++;
							break;
						case 7:
							if (ifNumber(tempString))
							{
								minTermInDays = retNumber(tempString);
								if (minTermInDays < 0)
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
								if (maxTermInDays < minTermInDays)
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else
							{
								MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							tempString = "";
							countFieldSetting++;
							break;
						case 8:
							float tempFloat;
							//проверка этой процентной ставки
							if (checkIfSumm(tempString))
							{
								tempFloat = retSumm(tempString);
								if (IRfirst == NULL)
								{
									IRfirst = new interestRate(tempFloat);
									IRlast = IRfirst;
								}
								else
								{
									IRlast->next = new interestRate(tempFloat);
									IRlast = IRlast->next;
								}
							}
							else
							{
								MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							tempString = "";
							countFieldSetting++;
							break;
						}
					}
					else if (c == ';')
					{
						MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						goto metka1;
					}
					else
					{
						switch (countFieldSetting)
						{
						case 4:
							if (c == ' ')
							{
								//проверка maxSumm и переход на ввод minSumm
								if (checkIfSumm(tempString))
								{
									maxSummFloat = retSumm(tempString);
									countFieldSetting++;
									tempString = "";
								}
								else
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else
								tempString += c;
							break;
						case 6:
							if (c == ' ')
							{
								//проверка maxTermInDays и переход на ввод minTermInDays
								if (ifNumber(tempString))
									maxTermInDays = retNumber(tempString);
								else
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
								countFieldSetting++;
								tempString = "";
							}
							else
								tempString += c;
							break;
						case 8:
							if (c == ' ')
							{
								float tempFloat;
								//проверка этой процентной ставки и переход на ввод следующей
								if (checkIfSumm(tempString))
								{
									tempFloat = retSumm(tempString);
									if (IRfirst == NULL)
									{
										IRfirst = new interestRate(tempFloat);
										IRlast = IRfirst;
									}
									else
									{
										IRlast->next = new interestRate(tempFloat);
										IRlast = IRlast->next;
									}
								}
								else
								{
									MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
								tempString = "";
							}
							else
								tempString += c;
							break;
						default:
							tempString += c;
						}
					}
				}
				else if (countFieldSetting == 9)
				{

					if (checkFirstDate(tempString, true))
					{
						FirstDate = tempString;
						formerDD = 0;
						formerMM = 0;
						formerYYYY = 0;
						formerDD += retDecimal(FirstDate[0]) * 10;
						formerDD += retDecimal(FirstDate[1]);

						formerMM += retDecimal(FirstDate[3]) * 10;
						formerMM += retDecimal(FirstDate[4]);

						formerYYYY += retDecimal(FirstDate[6]); formerYYYY *= 10;
						formerYYYY += retDecimal(FirstDate[7]); formerYYYY *= 10;
						formerYYYY += retDecimal(FirstDate[8]); formerYYYY *= 10;
						formerYYYY += retDecimal(FirstDate[9]);
					}

				}
				else
				{
					MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					goto metka1;
				}
			}
		}
		else
		{
			MessageBox::Show("Ошибка в файле настроек", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			goto metka1;
		}
		f.close();
		goto metka2;
	metka1:
		Application::Exit();
	metka2:
		uint_least32_t crc;
		for (i = 0; i < 256; i++)
		{
			crc = i;
			for (j = 0; j < 8; j++)
				if (crc & 1)
					crc = (crc >> 1) ^ 0xEDB88320UL;
				else
					crc = crc >> 1;
			crc_table[i] = crc;
		};

		interestRate *tempInterestRate = IRfirst;
		float ir = tempInterestRate->value;
		while (tempInterestRate != NULL)
		{
			string IRV = to_string(tempInterestRate->value);
			int i, dot = -1, N = IRV.length();
			string beforeDot = "", afterDot = "";

			for (i = 0; i < N; i++)
			{
				if (IRV[i] == '.')
					dot = i;
				else if (dot == -1)
					beforeDot += IRV[i];
				else
					afterDot += IRV[i];
			}
			afterDot += "00";
			IRV = beforeDot;
			IRV += '.';
			IRV += afterDot[0];
			IRV += afterDot[1];

			comboBox2->Items->Add( gcnew String (IRV.c_str()) );
			tempInterestRate = tempInterestRate->next;
		}

		DeactivateEditMode();

		Fields = new string *[maxQuantOfNotes];
		isDeleted = new bool[maxQuantOfNotes];
		colorNumber = new int[maxQuantOfNotes];
		isChanged = new bool[maxQuantOfNotes];
		numberOfNoteThatDeletedThis = new int[maxQuantOfNotes];
		numberOfNoteThatChangedThis = new int[maxQuantOfNotes];
		CRCField = new unsigned int [maxQuantOfNotes];

		Fields[0] = new string[13];
		f.open(fname2);
		countFieldSetting = 1;
		if (f.is_open())
		{
			int type;
			int prevType;
			int prevNumb;
			string tempString = "";
			char c;
			while (!f.eof())
			{
				f.get(c);
				if (!f.eof())
				{
					if (c == 0x0D || c == 0x0A)
					{
						switch (countFieldSetting)
						{
						case 1:
							if (tempString != codeNumber)
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 2:
							if (ifNumber(tempString))
							{
								int tempInt = retNumber(tempString);
								if (tempInt != tempNumber)
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 3:
							if (!checkDate(tempString, true))
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 4:
							if (checkTime(tempString))
							{
								if (checkDateTime(Fields[tempNumber - formerNumber][2], tempString, true, true))
									makeLastDateTime(Fields[tempNumber - formerNumber][2], tempString);
								else
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 5:
							if (ifNumber(tempString))
							{
								type = retNumber(tempString);
								if (!(type > 0 && type < 7))
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 6:
							if (type == 3 || type == 4)
							{
								if (ifNumber(tempString))
								{
									prevNumb = retNumber(tempString);
									if (prevNumb >= formerNumber && prevNumb < tempNumber)
									{
										prevType = retNumber(Fields[prevNumb - formerNumber][4]);
										if (prevType == 3 || prevType == 4)
										{
											MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
											goto metka1;
										}
									}
									else
									{
										MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
										goto metka1;
									}
									if (type == 3)
									{
										isDeleted[prevNumb - formerNumber] = true;
										numberOfNoteThatDeletedThis[prevNumb - formerNumber] = tempNumber;
									}
									else
									{
										isChanged[prevNumb - formerNumber] = true;
										numberOfNoteThatChangedThis[prevNumb - formerNumber] = tempNumber;
									}
								}
								else
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else if (tempString.length() != 0)
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 7:
							//проверка договора
							if ((type == 1 || type == 2) || ((type == 3 || type == 4) && (prevType == 1 || prevType == 2)))
							{
								if (!checkContract(tempString, false))
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else if (tempString.length() != 0)
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 8:
							if (!checkDate(tempString, true))
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 9:
							if (type == 1 || ((type == 3 || type == 4) && prevType == 1))
							{
								if (!checkDate(tempString, false))
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else if (tempString.length() != 0)
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 10:
							if (type == 2 || type == 5 || type == 6)
							{
								if (tempString.length() != 0)
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else if ( tempString.length() > FieldsMaxLength[9])
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 11:
							if (!checkIfSumm(tempString))
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 12:
							if (type == 1 || ((type == 3 || type == 4) && prevType == 1))
							{
								if (!checkIfSumm(tempString))
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							else if (tempString.length() != 0)
							{
								MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
								goto metka1;
							}
							break;
						case 13:
							if (!(type == 1 || type == 2))
							{
								if (tempString.length() == 0 || tempString.length() > FieldsMaxLength[12])
								{
									MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
									goto metka1;
								}
							}
							break;
						case 14:
							MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							goto metka1;
						
						}
						Fields[tempNumber - formerNumber][countFieldSetting - 1] = tempString;
						tempString = "";
						countFieldSetting++;
					}
					else if (c == ';')
					{
						if (countFieldSetting != 14)
						{
							MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							goto metka1;
						}
						if(!ifNumber(tempString))
						{
							MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							goto metka1;
						}
						
						string forcrc = "";
						for (i = 0; i < 13; i++)
							forcrc += Fields[tempNumber - formerNumber][i];
						if(tempNumber != formerNumber)
							forcrc += to_string( CRCField[tempNumber - 1 - formerNumber] );
						unsigned char *buf = new unsigned char[forcrc.length()];
						unsigned int newcrc;
						for (i = 0; i < forcrc.length(); i++)
							buf[i] = forcrc[i];
						newcrc = Crc32(buf, forcrc.length());
						if (retNumber(tempString) == newcrc)
							CRCField[tempNumber - formerNumber] = newcrc;

						
						tempString = "";
						countFieldSetting = 1;
						isDeleted[tempNumber - formerNumber] = false;
						isChanged[tempNumber - formerNumber] = false;
						tempNumber++;
						if (tempNumber == maxQuantOfNotes)
						{
							MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							goto metka1;
						}
						Fields[tempNumber - formerNumber] = new string[13];
					}
					else
					{
						tempString += c;
					}
				}
				else
				{
					if (!(tempString == "" && countFieldSetting == 1))
					{
						MessageBox::Show("Ошибка в файле c данными", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						goto metka1;
					}
				}
			}
			
		}
		f.close();

		/*на этом этапе у нас есть массив со всеми записями. осталось вывести это дело в листбокс*/
		/*но прежде надо подготовить список договоров и платежей по ним*/

		for (i = 0; i < tempNumber-formerNumber; i++)
		{
			int typeTemp = retNumber(Fields[i][4]);
			if (typeTemp == 1)
			{
				int tcn = i;
				if (isChanged[i])
				{
					tcn = numberOfNoteThatChangedThis[i];
					tcn - formerNumber;
				}
				if (ContractFirst == NULL)
				{
					ContractFirst = new paymentContract(Fields[tcn][6], i+formerNumber, NULL);
					ContractLast = ContractFirst;
				}
				else
				{
					ContractLast->next = new paymentContract(Fields[tcn][6], i+formerNumber, NULL);
					ContractLast = ContractLast->next;
				}
			}
			if (typeTemp == 2)
			{				
				bool found = false;
				paymentContract * tempContract = ContractFirst;
				if (tempContract == NULL)
				{
					goto metka1;
				}
				int tpn = i;
				if (isChanged[i])
				{
					tpn = numberOfNoteThatChangedThis[i];
					tpn - formerNumber;
				}
				if (tempContract->value == Fields[tpn][6])
					found = true;
				while (!found)
				{
					tempContract = tempContract->next;
					if (tempContract == NULL)
					{
						goto metka1;
					}
					if (tempContract->value == Fields[tpn][6])
						found = true;
				}

				if (tempContract->payfirst == NULL)
				{
					tempContract->payfirst = new payment(i+formerNumber, NULL);
					tempContract->paylast = tempContract->payfirst;
				}
				else
				{
					tempContract->paylast->next = new payment(i+formerNumber, NULL);
					tempContract->paylast = tempContract->paylast->next;
				}
			}
			if (typeTemp == 5 || typeTemp == 6)
			{
				if (CashFirst == NULL)
				{
					CashFirst = new paymentContract(Fields[i][6], i + formerNumber, NULL);
					CashLast = CashFirst;
				}
				else
				{
					CashLast->next = new paymentContract(Fields[i][6], i + formerNumber, NULL);
					CashLast = CashLast->next;
				}
			}
		}

		return;
	}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{

		listBox1->Items->Clear();
		state = 1;
		int i,k,j;
		for (i = 0; i < tempNumber - formerNumber; i++)
		{
			int typeTemp = retNumber(Fields[i][4]);
			
			string s = "";
			
			for (k = 1; k <= 12; k++)
			{
				s += Fields[i][k];
				for (j = Fields[i][k].length(); j < FieldsMaxLength[k] + 1; j++)
					s += ' ';
			}
			String^ mmm = gcnew String(s.c_str());
			colorNumber[i] = 1;
			listBox1->Items->Add(mmm);
		}
		return;
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		listBox1->Items->Clear();
		state = 2;
		int k, j,count=0;
		paymentContract *tempContract = ContractFirst;
		while (tempContract != NULL)
		{
			int i = tempContract->number;
			if (!isDeleted[i - formerNumber])
			{
				if (isChanged[i - formerNumber])
					i = numberOfNoteThatChangedThis[i - formerNumber];
				string s = "";
				s += Fields[tempContract->number - formerNumber][1];
				for (j = Fields[tempContract->number - formerNumber][1].length(); j < FieldsMaxLength[1] + 1; j++)
					s += " ";
				for (k = 2; k <= 12; k++)
				{
					if (k == 4)
					{
						s += Fields[tempContract->number - formerNumber][k];
						for (j = Fields[tempContract->number - formerNumber][k].length(); j < FieldsMaxLength[k] + 1; j++)
							s += " ";
					}
					else if (k != 5)
					{
						s += Fields[i - formerNumber][k];
						for (j = Fields[i - formerNumber][k].length(); j < FieldsMaxLength[k] + 1; j++)
							s += " ";
					}

				}
				String^ mmm = gcnew String(s.c_str());
				colorNumber[count++] = 1;
				listBox1->Items->Add(mmm);

				payment *tempPayment = tempContract->payfirst;
				while (tempPayment != NULL)
				{
					int i = tempPayment->number;
					if (!isDeleted[i - formerNumber])
					{
						if (isChanged[i - formerNumber])
							i = numberOfNoteThatChangedThis[i - formerNumber];
						string s = "";
						s += Fields[tempPayment->number - formerNumber][1];
						for (j = Fields[tempPayment->number - formerNumber][1].length(); j < FieldsMaxLength[1] + 1; j++)
							s += " ";
						for (k = 2; k <= 12; k++)
						{
							if (k == 4)
							{
								s += Fields[tempPayment->number - formerNumber][k];
								for (j = Fields[tempPayment->number - formerNumber][k].length(); j < FieldsMaxLength[k] + 1; j++)
									s += " ";
							}
							else if (k != 5)
							{
								s += Fields[i - formerNumber][k];
								for (j = Fields[i - formerNumber][k].length(); j < FieldsMaxLength[k] + 1; j++)
									s += " ";
							}

						}
						String^ mmm = gcnew String(s.c_str());
						colorNumber[count++] = 1;
						listBox1->Items->Add(mmm);

					}
					tempPayment = tempPayment->next;
				}

			}
				tempContract = tempContract->next;
		}

		return;
	}
		
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e)
		{
			listBox1->Items->Clear();
			state = 3;
			int i, k, j;

			paymentContract * tempContract = ContractFirst;
			i = 0;
			float summZaim, summVyplat, percent;
			string tempDate = "";

			while (tempContract != NULL)
			{
				int numb = tempContract->number;

				if (!isDeleted[tempContract->number - formerNumber])
				{
					if (isChanged[tempContract->number - formerNumber])
						numb = numberOfNoteThatChangedThis[numb - formerNumber];

					summZaim = retSumm(Fields[numb - formerNumber][10]);
					summVyplat = 0;
					percent = retSumm(Fields[numb - formerNumber][11]);
					tempDate = Fields[numb - formerNumber][7];
					payment *tempVyplata = tempContract->payfirst;
					string temptempDate = Fields[numb - formerNumber][8];
					while (tempVyplata != NULL)
					{
						if (!isDeleted[tempVyplata->number - formerNumber])
						{
							int numb2 = tempVyplata->number;
							if (isChanged[numb2 - formerNumber])
								numb2 = numberOfNoteThatChangedThis[tempVyplata->number - formerNumber];


							summVyplat += retSumm(Fields[numb2 - formerNumber][10]);
						}
						tempVyplata = tempVyplata->next;
					}
					summZaim += summZaim * percent*0.01 * retDaysBetween(Fields[numb - formerNumber][7], temptempDate);

					if (summVyplat >= summZaim)
						colorNumber[i] = 2;
					else if (!checkIfFirstDateEarlier(retDate(),Fields[numb - formerNumber][8]))
						colorNumber[i] = 3;
					else
						colorNumber[i] = 1;

					string s = "";
					s += Fields[tempContract->number - formerNumber][1];
					for (j = Fields[tempContract->number - formerNumber][1].length(); j < FieldsMaxLength[1] + 1; j++)
						s += " ";
					for (k = 2; k <= 11; k++)
					{
						if (k != 4 && k != 5)
						{
							s += Fields[numb - formerNumber][k];
							for (j = Fields[numb - formerNumber][k].length(); j < FieldsMaxLength[k] + 1; j++)
								s += " ";
						}
					}
					string rest = "";
					if (colorNumber[i] != 2)
					{
						string IRV = to_string(summZaim - summVyplat);
						int i, dot = -1, N = IRV.length();
						string beforeDot = "", afterDot = "";

						for (i = 0; i < N; i++)
						{
							if (IRV[i] == '.')
								dot = i;
							else if (dot == -1)
								beforeDot += IRV[i];
							else
								afterDot += IRV[i];
						}
						afterDot += "00";
						IRV = beforeDot;
						IRV += '.';
						IRV += afterDot[0];
						IRV += afterDot[1];

						rest += IRV;
					}
					rest += " ";
					s += rest;
					s += Fields[numb - formerNumber][k];

					
					listBox1->Items->Add(gcnew String((s).c_str()));
					i++;
				}
				tempContract = tempContract->next;

			}
			return;
		}
	//включает объекты для редактирования/ввода строки
	void ActivateEditMode()
	{
		toolStripMenuItem1->Enabled = false;
		добавитьЗаписьToolStripMenuItem->Enabled = false;
		удалитьЗаписьToolStripMenuItem->Enabled = false;
		listBox1->Enabled = false;
		
		groupBox1->Enabled = true;
		
		label1->Visible = true;
		label2->Visible = true;
		label3->Visible = true;
		label4->Visible = true;
		label5->Visible = true;
		label6->Visible = true;
		label7->Visible = true;
		label8->Visible = true;

		textBox1->Visible = true;
		textBox2->Visible = true;
		textBox3->Visible = true;
		textBox4->Visible = true;
		textBox5->Visible = true;
		textBox6->Visible = true;

		comboBox1->Visible = true;
		comboBox2->Visible = true;

		button4->Visible = true;
		button5->Visible = true;
		button1->Visible = false;
		button2->Visible = false;
		button3->Visible = false;
		return;
	}
	//выключает объекты для редактирования/ввода строки
	void DeactivateEditMode()
	{
		toolStripMenuItem1->Enabled = true;
		добавитьЗаписьToolStripMenuItem->Enabled = true;
		удалитьЗаписьToolStripMenuItem->Enabled = true;
		listBox1->Enabled = true;

		groupBox1->Enabled = false;

		label1->Visible = false;
		label2->Visible = false;
		label3->Visible = false;
		label4->Visible = false;
		label5->Visible = false;
		label6->Visible = false;
		label7->Visible = false;
		label8->Visible = false;

		textBox1->Text = "";
		textBox2->Text = "";
		textBox3->Text = "";
		textBox4->Text = "";
		textBox5->Text = "";
		textBox6->Text = "";
		comboBox1->Text = "";
		comboBox2->Text = "";

		textBox1->Visible = false;
		textBox2->Visible = false;
		textBox3->Visible = false;
		textBox4->Visible = false;
		textBox5->Visible = false;
		textBox6->Visible = false;

		comboBox1->Visible = false;
		comboBox2->Visible = false;

		button4->Visible = false;
		button5->Visible = false;
		button6->Visible = false;
		button1->Visible = true;
		button2->Visible = true;
		button3->Visible = true;
		return;
	}
	//
	string retContractDateTime()
	{
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);                         // текущая дата, выраженная в секундах
		timeinfo = localtime(&rawtime);        // текущая дата, представленная в нормальной форме

		string DD = to_string(timeinfo->tm_mday);
		if (timeinfo->tm_mday < 10)
		{
			string DDs = DD;
			DD = "0";
			DD += DDs;
		}
		string MM = to_string(timeinfo->tm_mon + 1);
		if (timeinfo->tm_mon + 1 < 10)
		{
			string MMs = MM;
			MM = "0";
			MM += MMs;
		}
		string YYYY = to_string(timeinfo->tm_year + 1900);
		string HH = to_string(timeinfo->tm_hour);
		if (timeinfo->tm_hour < 10)
		{
			string HHs = HH;
			HH = "0";
			HH += HHs;
		}
		string MIN = to_string(timeinfo->tm_min);
		if (timeinfo->tm_min < 10)
		{
			string MINs = MIN;
			MIN = "0";
			MIN += MINs;
		}
		string date = "";
		date += YYYY[2];
		date += YYYY[3];
		date += MM;
		date += DD;
		date += HH;
		date += MIN;
		
		return date;
	}

private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) 
{
	if (tempNumber - formerNumber == maxQuantOfNotes)
	{
		MessageBox::Show("Достигнуто максимальное число записей", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}
	

	if (listBox1->Items->Count == 0 || listBox1->SelectedIndex == -1)
	{
		if (listBox1->Items->Count == 0)
			MessageBox::Show("Список пуст", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		else
			if (listBox1->SelectedIndex == -1)
				MessageBox::Show("Строка не выбрана", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}
	else
	{
		int i;
		string Note = msclr::interop::marshal_as<std::string>(listBox1->SelectedItem->ToString());
		string c = "";
		for (i = 0; i < 15; i++)
		{
			if (Note[i] != ' ')
				c += Note[i];
			else
				break;
		}
		numberRUN = retNumber(c);
		if (Fields[numberRUN - formerNumber][4] == "3" || Fields[numberRUN - formerNumber][4] == "4")
		{
			numberRUN = -1;
			MessageBox::Show("Нельзя редактировать записи об удалении или об изменении", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}
		//есть номер number. чтобы найти номер строки в Fields, надо от него отнять formerNumber
		if (isDeleted[numberRUN - formerNumber])
		{
			numberRUN = -1;
			stateRUN = 0;
			MessageBox::Show("Запись уже удалена", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}
		else
		{
			int numb = numberRUN;
			if (isChanged[numb - formerNumber])
				numb = numberOfNoteThatChangedThis[numb - formerNumber];
			int slash = -1;
			string contract = "";
			if (Fields[numb - formerNumber][6].length() > 10)
			{
				for (i = 0; i < Fields[numb - formerNumber][6].length(); i++)
					if (Fields[numb - formerNumber][6][i] == '/')
					{
						slash = i;
					}
				
				for (i = slash + 1; i < Fields[numb - formerNumber][6].length(); i++)
					contract += Fields[numb - formerNumber][6][i];
			}
			textBox1->Text = gcnew String(contract.c_str());
			textBox2->Text = gcnew String(Fields[numb - formerNumber][7].c_str());
			textBox3->Text = gcnew String(Fields[numb - formerNumber][8].c_str());
			textBox4->Text = gcnew String(Fields[numb - formerNumber][9].c_str());
			textBox5->Text = gcnew String(Fields[numb - formerNumber][10].c_str());
			textBox6->Text = gcnew String(Fields[numb - formerNumber][12].c_str());
			comboBox2->Text = gcnew String(Fields[numb - formerNumber][11].c_str());


			ActivateEditMode();
			comboBox1->Enabled = false;
			label1->Visible = false;
			button6->Visible = false;
			stateRUN = 1;
		}
		typeRUN = 4;
		
	}

	return;
}
private: System::Void добавитьЗаписьToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
	if (tempNumber - formerNumber == maxQuantOfNotes)
	{
		MessageBox::Show("Достигнуто максимальное число записей", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	stateRUN = 3;
	toolStripMenuItem1->Enabled = false;
	добавитьЗаписьToolStripMenuItem->Enabled = false;
	удалитьЗаписьToolStripMenuItem->Enabled = false;
	listBox1->Enabled = false;

	groupBox1->Enabled = true;
	label1->Visible = true;
	comboBox1->Visible = true;
	button6->Visible = true;
	comboBox1->Text = "";

	return;
}
private: System::Void удалитьЗаписьToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (tempNumber - formerNumber == maxQuantOfNotes)
	{
		MessageBox::Show("Достигнуто максимальное число записей", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}
	
	if (listBox1->Items->Count == 0 || listBox1->SelectedIndex == -1)
	{
		if (listBox1->Items->Count == 0)
			MessageBox::Show("Список пуст", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		else
			if (listBox1->SelectedIndex == -1)
				MessageBox::Show("Строка не выбрана", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}
	else
	{
		
		int i;
		string Note = msclr::interop::marshal_as<std::string>(listBox1->SelectedItem->ToString());
		int number = 0;
		string c = "";
		for (i = 0; i < 15; i++)
		{			
			if (Note[i] != ' ')
				c += Note[i];
			else
				break;
		}
		numberRUN = retNumber(c);
		if(Fields[numberRUN-formerNumber][4] == "3" || Fields[numberRUN - formerNumber][4] == "3")
		{
			numberRUN = -1;
			MessageBox::Show("Нельзя удалять записи об удалении или об изменении", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}
		//есть номер number. чтобы найти номер строки в Fields, надо от него отнять formerNumber
		if (isDeleted[numberRUN - formerNumber])
		{
			numberRUN = -1;
			MessageBox::Show("Запись уже удалена", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}
		stateRUN = 2;
		ActivateEditMode();
		
		label1->Visible = false;
		label2->Visible = false;
		label3->Visible = false;
		label4->Visible = false;
		label5->Visible = false;
		label6->Visible = false;
		label7->Visible = false;

		textBox1->Visible = false;
		textBox2->Visible = false;
		textBox3->Visible = false;
		textBox4->Visible = false;
		textBox5->Visible = false;

		comboBox1->Visible = false;
		comboBox2->Visible = false;
		button6->Visible = false;
	}

	return;
}


	bool checkGroupBoxNotes(int stateRUN)
	{
		string s;
		int typeT;

		string text7 = msclr::interop::marshal_as<std::string>(textBox1->Text);
		string text8 = msclr::interop::marshal_as<std::string>(textBox2->Text);
		string text9 = msclr::interop::marshal_as<std::string>(textBox3->Text);
		string text10 = msclr::interop::marshal_as<std::string>(textBox4->Text);
		string text11 = msclr::interop::marshal_as<std::string>(textBox5->Text);
		string text12 = msclr::interop::marshal_as<std::string>(comboBox2->Text);
		string text13 = msclr::interop::marshal_as<std::string>(textBox6->Text);

		s = text7 + text8 + text9 + text10 + text11 + text12 + text13;
		for (int i = 0; i < s.length(); i++)
			if (s[i] == ';' || s[i] == 0x0D || s[i] == 0x0A)
				return false;
		
		string Date;
		string Time;
		if (stateRUN == 1)
		{
			typeT = retNumber(Fields[numberRUN - formerNumber][4]);
			int daysBetween;
			switch (typeT)
			{
			case 1:
				if (text7.length() != 10)
					return false;
				if (!ifNumber(text7))
					return false;
				Date = "";
				Date += text7[4];
				Date += text7[5];
				Date += ".";
				Date += text7[2];
				Date += text7[3];
				Date += ".";
				Date += "20";
				Date += text7[0];
				Date += text7[1];

				Time = "";
				Time += text7[6];
				Time += text7[7];
				Time += ":";
				Time += text7[8];
				Time += text7[9];
				Time += ":59";
				if (!checkDateTime(Date, Time, false,false))
					return false;

				if (!checkDate(text8, true))
					return false;

				if (!checkDate(text9, false))
					return false;

				if (!checkIfFirstDateEarlier(text8, text9))
					return false;

				
				daysBetween = retDaysBetween(text8, text9);
				if (daysBetween > maxTermInDays)
					return false;

				if (daysBetween < minTermInDays)
					return false;

				if (text10.length() == 0)
					return false;
				if (!checkIfFIO(text10))
				{
					if (MessageBox::Show("ФИО не в формате Фамилия Имя Отчество. Всё равно принять?", "Внимание", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::No)
						return false;
				}

				if (!checkSumm(text11))
					return false;

				if (!checkIfSumm(text12))
					return false;

				if (text13.length() > FieldsMaxLength[12])
					return false;
				break;
			case 2:
				if (text7.length() != 10)
					return false;
				if (!ifNumber(s))
					return false;
				Date = "";
				Date += text7[4];
				Date += text7[5];
				Date += ".";
				Date += text7[2];
				Date += text7[3];
				Date += ".";
				Date += "20";
				Date += text7[0];
				Date += text7[1];

				Time = "";
				Time += text7[6];
				Time += text7[7];
				Time += ":";
				Time += text7[8];
				Time += text7[9];
				Time += ":59";
				
				if (!checkDateTime(Date, Time, false,false))
					return false;
				
				if (!checkDate(text8, true))
					return false;
				if (text9.length() != 0)
					return false;
				if (text10.length() != 0)
					return false;
				if (!checkIfSumm(text11))
					return false;
				if (text12.length() != 0)
					return false;
				if (text13.length() > FieldsMaxLength[12])
					return false;
				break;
			case 5:
				if (text7.length() != 0)
					return false;
				if (text8.length() != 0)
					return false;
				if (text9.length() != 0)
					return false;
				if (text10.length() != 0)
					return false;
				if (!checkIfSumm(text11))
					return false;
				if (text12.length() != 0)
					return false;
				if (text13.length() == 0 || text13.length() > FieldsMaxLength[12])
					return false;
				break;
			case 6:
				if (text7.length() != 0)
					return false;
				if (text8.length() != 0)
					return false;
				if (text9.length() != 0)
					return false;
				if (text10.length() != 0)
					return false;
				if (!checkIfSumm(text11))
					return false;
				if (text12.length() != 0)
					return false;
				if (text13.length() == 0 || text13.length() > FieldsMaxLength[12])
					return false;
			}

		}
		else if (stateRUN == 3)
		{
			typeT = retNumber(msclr::interop::marshal_as<std::string>(comboBox1->Text));
			int daysBetween;
			switch (typeT)
			{
			case 1:				
				if (!checkIfFirstDateEarlier(retDate(), text9))
					return false;
				daysBetween = retDaysBetween(retDate(), text9);
				if (daysBetween > maxTermInDays)
					return false;
				if (daysBetween < minTermInDays)
					return false;
				if (!checkDate(text9, false))
					return false;
				if (text10.length() == 0)
					return false;
				if (!checkIfFIO(text10))
				{
					if (MessageBox::Show("ФИО не в формате Фамилия Имя Отчество. Всё равно принять?", "Внимание", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::No)
						return false;
				}
				if (!checkSumm(text11))
					return false;
				if (!checkIfSumm(text12))
					return false;
				if (text13.length() > FieldsMaxLength[12])
					return false;
				break;
			case 2:
				
				if (text7.length() != 10)
					return false;
				if (!ifNumber(text7))
					return false;
				Date = "";
				Date += text7[4];
				Date += text7[5];
				Date += ".";
				Date += text7[2];
				Date += text7[3];
				Date += ".";
				Date += "20";
				Date += text7[0];
				Date += text7[1];

				Time = "";
				Time += text7[6];
				Time += text7[7];
				Time += ":";
				Time += text7[8];
				Time += text7[9];
				Time += ":59";
				if (!checkDateTime(Date, Time, false,false))
					return false;				
				if (!checkIfSumm(text11))
					return false;				
				if (text13.length() > FieldsMaxLength[12])
					return false;
				break;
			case 5:
				if (!checkIfSumm(text11))
					return false;
				if (text13.length() == 0 || text13.length() > FieldsMaxLength[12])
					return false;
				break;
			case 6:
				if (!checkIfSumm(text11))
					return false;
				if (text13.length() == 0 || text13.length() > FieldsMaxLength[12])
					return false;
			}
			
		}
		else if (stateRUN == 2)
		{
			string text = msclr::interop::marshal_as<std::string>(textBox6->Text);
			if (text.length() == 0 || text.length() > FieldsMaxLength[12])
				return false;
			return true;
		}
		
		return true;
	};
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		int i,j;
		bool checkNotes = checkGroupBoxNotes(stateRUN);
		if (checkNotes == false)
		{
			MessageBox::Show("Ошибка при введении данных", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}
		
		string prefixNote = "";
		string percentage = ""; 
		
		string summ = "";
		
		int typeT;
		string s;

		if (stateRUN == 1)
		{
			percentage = msclr::interop::marshal_as<std::string>(comboBox2->Text);
			if (checkIfSumm(percentage))
			{
				int i, dot = -1, N = percentage.length();
				string beforeDot = "", afterDot = "";

				for (i = 0; i < N; i++)
				{
					if (percentage[i] == '.')
						dot = i;
					else if (dot == -1)
						beforeDot += percentage[i];
					else
						afterDot += percentage[i];
				}
				afterDot += "00";
				percentage = beforeDot;
				percentage += '.';
				percentage += afterDot[0];
				percentage += afterDot[1];

			}
			summ = msclr::interop::marshal_as<std::string>(textBox5->Text);
			if (checkIfSumm(summ))
			{
				int i, dot = -1, N = summ.length();
				string beforeDot = "", afterDot = "";

				for (i = 0; i < N; i++)
				{
					if (summ[i] == '.')
						dot = i;
					else if (dot == -1)
						beforeDot += summ[i];
					else
						afterDot += summ[i];
				}
				afterDot += "00";
				summ = beforeDot;
				summ += '.';
				summ += afterDot[0];
				summ += afterDot[1];
			}

			typeT = retNumber(Fields[numberRUN - formerNumber][4]);
			paymentContract *tempContract = ContractFirst;
			paymentContract *thisContract = ContractFirst;
			if (typeT == 1 || typeT == 2)
			{	
				prefixNote = Prefix;
				prefixNote += '/';
				prefixNote += msclr::interop::marshal_as<std::string>(textBox1->Text);
				

				while (tempContract != NULL)
				{
					if (isDeleted[tempContract->number - formerNumber])
						tempContract = tempContract->next;
					else if (tempContract->value == prefixNote)
						break;
					else
						tempContract = tempContract->next;
				}

				while (thisContract != NULL)
				{
					if (thisContract->number == numberRUN)
						break;
					else
						thisContract = thisContract->next;
				}
			}

			if (typeT == 1)
			{
				if (tempContract == thisContract)
				{
					isChanged[numberRUN - formerNumber] = true;
					numberOfNoteThatChangedThis[thisContract->number - formerNumber] = tempNumber;
				}
				else if (tempContract == NULL)
				{
					payment *tempPayment = thisContract->payfirst;
					bool noPayment = true;
					while (tempPayment != NULL)
					{
						if (!isDeleted[tempPayment->number - formerNumber])
						{
							noPayment = false;
							break;
						}
						else
							tempPayment = tempPayment->next;
					}

					//выводим ошибку
					if (!noPayment)
					{
						MessageBox::Show("По этому договору есть записи об оплате", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						return;
					}
					else
					{
						isChanged[numberRUN - formerNumber] = true;
						numberOfNoteThatChangedThis[thisContract->number - formerNumber] = tempNumber;
						thisContract->value = prefixNote;
					}
				}
				else if (tempContract != NULL)
				{
					//выводим ошибку
					MessageBox::Show("Дублирование номера договора", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}

			}
			if (typeT == 2)
			{
				if (tempContract == thisContract)
				{
					int aaaa;
					if (isChanged[tempContract->number])
						aaaa = numberOfNoteThatChangedThis[tempContract->number];
					else
						aaaa = tempContract->number;
					if (!checkIfFirstDateEarlier(msclr::interop::marshal_as<std::string>(textBox3->Text), Fields[aaaa][8]))
					{
						MessageBox::Show("Срок договора окончен", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						return;
					}
					isChanged[numberRUN - formerNumber] = true;
					numberOfNoteThatChangedThis[thisContract->number - formerNumber] = tempNumber;
				}
				else if (tempContract == NULL)
				{
					MessageBox::Show("Нет договора с таким номером", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}
				else if (tempContract != NULL)
				{
					int aaaa;
					if (isChanged[tempContract->number])
						aaaa = numberOfNoteThatChangedThis[tempContract->number];
					else
						aaaa = tempContract->number;
					if (!checkIfFirstDateEarlier(msclr::interop::marshal_as<std::string>(textBox3->Text), Fields[aaaa][8]))
					{
						MessageBox::Show("Срок договора окончен", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						return;
					}
					payment *beforeThisPayment = thisContract->payfirst;
					payment *thisPayment = beforeThisPayment;
					if (beforeThisPayment->number == numberRUN)
					{
						thisContract->payfirst = NULL;
						thisContract->paylast = NULL;
					}
					else
					{
						while (beforeThisPayment->next->number != numberRUN)
							beforeThisPayment = beforeThisPayment->next;
						thisPayment = beforeThisPayment->next;
						payment *afterThisPayment = thisPayment->next;
						if (afterThisPayment == NULL)
							thisContract->paylast = thisContract->payfirst;
						beforeThisPayment->next = afterThisPayment;
						beforeThisPayment = thisPayment;
					}
					thisPayment = beforeThisPayment;

					isChanged[numberRUN - formerNumber] = true;
					numberOfNoteThatChangedThis[thisPayment->number - formerNumber] = tempNumber;

					tempContract->paylast->next = thisPayment;
					tempContract->paylast = tempContract->paylast->next;
				}
			}

			Fields[tempNumber - formerNumber][4] = to_string(4);
			Fields[tempNumber - formerNumber][5] = to_string(numberRUN);
			Fields[tempNumber - formerNumber][6] = prefixNote;
			Fields[tempNumber - formerNumber][7] = msclr::interop::marshal_as<std::string>(textBox2->Text->ToString());
			Fields[tempNumber - formerNumber][8] = msclr::interop::marshal_as<std::string>(textBox3->Text->ToString());
			Fields[tempNumber - formerNumber][9] = msclr::interop::marshal_as<std::string>(textBox4->Text->ToString());
			Fields[tempNumber - formerNumber][10] = summ;
			Fields[tempNumber - formerNumber][11] = percentage;
			Fields[tempNumber - formerNumber][12] = msclr::interop::marshal_as<std::string>(textBox6->Text->ToString());
			fillStandardFields(tempNumber);

			Fields[tempNumber - formerNumber + 1] = new string[13];

			string forcrc = "";
			for (i = 0; i < 13; i++)
				forcrc += Fields[tempNumber - formerNumber][i];
			if (tempNumber != formerNumber)
				forcrc += to_string(CRCField[tempNumber - 1 - formerNumber]);
			unsigned char *buf = new unsigned char[forcrc.length()];
			unsigned int newcrc;
			for (i = 0; i < forcrc.length(); i++)
				buf[i] = forcrc[i];
			newcrc = Crc32(buf, forcrc.length());
			CRCField[tempNumber - formerNumber] = newcrc;


			ofstream f(fname2, ios_base::app);
			char razdel = 0x0D;
			for (i = 0; i < 13; i++)
			{
				for (j = 0; j < Fields[tempNumber - formerNumber][i].length(); j++)
				{
					f << Fields[tempNumber - formerNumber][i][j];
				}
				f << razdel;
			}
			razdel = ';';
			string crcout = to_string(CRCField[tempNumber - formerNumber]);
			for (j = 0; j < crcout.length(); j++)
			{
				f << crcout[j];
			}
			f << razdel;
			f.close();
			isChanged[numberRUN - formerNumber] = true;
			tempNumber++;
		}
		else if (stateRUN == 2)
		{
			isDeleted[numberRUN - formerNumber] = true;
			numberOfNoteThatDeletedThis[numberRUN - formerNumber] = tempNumber;
			Fields[tempNumber - formerNumber][4] = to_string(3);
			Fields[tempNumber - formerNumber][5] = to_string(numberRUN);
			Fields[tempNumber - formerNumber][6] = Fields[numberRUN - formerNumber][6];
			Fields[tempNumber - formerNumber][7] = Fields[numberRUN - formerNumber][7];
			Fields[tempNumber - formerNumber][8] = Fields[numberRUN - formerNumber][8];
			Fields[tempNumber - formerNumber][9] = Fields[numberRUN - formerNumber][9];
			Fields[tempNumber - formerNumber][10] = Fields[numberRUN - formerNumber][10];
			Fields[tempNumber - formerNumber][11] = Fields[numberRUN - formerNumber][11];
			Fields[tempNumber - formerNumber][12] = msclr::interop::marshal_as<std::string>(textBox6->Text);;
			fillStandardFields(tempNumber);
			Fields[tempNumber - formerNumber + 1] = new string[13];

			string forcrc = "";
			for (i = 0; i < 13; i++)
				forcrc += Fields[tempNumber - formerNumber][i];
			if (tempNumber != formerNumber)
				forcrc += to_string(CRCField[tempNumber - 1 - formerNumber]);
			unsigned char *buf = new unsigned char[forcrc.length()];
			unsigned int newcrc;
			for (i = 0; i < forcrc.length(); i++)
				buf[i] = forcrc[i];
			newcrc = Crc32(buf, forcrc.length());
			CRCField[tempNumber - formerNumber] = newcrc;


			ofstream f(fname2, ios_base::app);
			char razdel = 0x0D;
			for (i = 0; i < 13; i++)
			{
				for (j = 0; j < Fields[tempNumber - formerNumber][i].length(); j++)
				{
					f << Fields[tempNumber - formerNumber][i][j];
				}
				f << razdel;
			}
			razdel = ';';
			string crcout = to_string(CRCField[tempNumber - formerNumber]);
			for (j = 0; j < crcout.length(); j++)
			{
				f << crcout[j];
			}
			f << razdel;
			f.close();
			tempNumber++;
			stateRUN = 0;
			DeactivateEditMode();
		}
		else if(stateRUN == 3)
		{

			s = msclr::interop::marshal_as<std::string>(comboBox1->Text);
			typeT = retNumber(s);
			percentage = msclr::interop::marshal_as<std::string>(comboBox2->Text);
			if (checkIfSumm(percentage))
			{
				int i, dot = -1, N = percentage.length();
				string beforeDot = "", afterDot = "";

				for (i = 0; i < N; i++)
				{
					if (percentage[i] == '.')
						dot = i;
					else if (dot == -1)
						beforeDot += percentage[i];
					else
						afterDot += percentage[i];
				}
				afterDot += "00";
				percentage = beforeDot;
				percentage += '.';
				percentage += afterDot[0];
				percentage += afterDot[1];

			}
			summ = msclr::interop::marshal_as<std::string>(textBox5->Text);
			if (checkIfSumm(summ))
			{
				int i, dot = -1, N = summ.length();
				string beforeDot = "", afterDot = "";

				for (i = 0; i < N; i++)
				{
					if (summ[i] == '.')
						dot = i;
					else if (dot == -1)
						beforeDot += summ[i];
					else
						afterDot += summ[i];
				}
				afterDot += "00";
				summ = beforeDot;
				summ += '.';
				summ += afterDot[0];
				summ += afterDot[1];
			}
			paymentContract *tempContract = ContractFirst;
			if (typeT == 2)
			{
				prefixNote = Prefix;
				prefixNote += '/';
				prefixNote += msclr::interop::marshal_as<std::string>(textBox1->Text);
			}
			else if (typeT == 1)
			{
				prefixNote = Prefix;
				prefixNote += '/';
				prefixNote += retContractDateTime();
				
			}
			
			if (tempContract != NULL)
			{
				
				if (typeT == 1 || typeT == 2)
				{		
					while (tempContract != NULL)
					{
						int numb = tempContract->number;
						if (isDeleted[numb - formerNumber])
							tempContract = tempContract->next;
						else
						{
							if (isChanged[numb - formerNumber])
								numb = numberOfNoteThatChangedThis[numb - formerNumber];
							if (Fields[numb - formerNumber][6] == prefixNote)
							{
								
								break;
								
							}
							else
								tempContract = tempContract->next;
						}
					}
				}
				if (typeT == 1)
				{
					if (tempContract != NULL)
					{
						MessageBox::Show("Уже есть договор с таким номером", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						return;
					}
					else
					{
						paymentContract *newContract = new paymentContract(prefixNote, tempNumber, NULL);
						if (ContractFirst == NULL)
						{
							ContractFirst = newContract;
							ContractLast = newContract;
						}
						else
						{
							ContractLast->next = newContract;
							ContractLast = ContractLast->next;
						}						
					}
				}
				else if (typeT == 2)
				{
					
					if (tempContract != NULL)
					{
						int aaaa;
						if (isChanged[tempContract->number - formerNumber])
							aaaa = numberOfNoteThatChangedThis[tempContract->number - formerNumber];
						else
							aaaa = tempContract->number;
						if (!checkIfFirstDateEarlier(retDate(), Fields[aaaa - formerNumber][8]))
						{
							MessageBox::Show("Срок договора окончен", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							return;
						}
						payment *newPayment = new payment(tempNumber, NULL);
						if (tempContract->payfirst == NULL)
						{
							tempContract->payfirst = newPayment;
							tempContract->paylast = newPayment;
						}
						else
						{
							tempContract->paylast->next = newPayment;
							tempContract->paylast = tempContract->paylast->next;
						}
						
						isChanged[tempNumber - formerNumber] = false;
						isDeleted[tempNumber - formerNumber] = false;
					}
					else
					{
						MessageBox::Show("Договора с таким номером нет", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						return;
					}
				}
			}
			else
			{
				if (typeT == 1)
				{
					ContractFirst = new paymentContract(prefixNote, tempNumber, NULL);
					ContractLast = ContractFirst;
				}
				else if (typeT == 2)
				{
					MessageBox::Show("Договора с таким номером нет", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return;
				}
			}

			if (typeT == 5 || typeT == 6)
			{
				if (CashFirst == NULL)
				{
					CashFirst = new paymentContract(Fields[i][6], tempNumber, NULL);
					CashLast = CashFirst;
				}
				else
				{
					CashLast->next = new paymentContract(Fields[i][6], tempNumber, NULL);
					CashLast = CashLast->next;
				}
			}
			
			isChanged[tempNumber - formerNumber] = false;
			isDeleted[tempNumber - formerNumber] = false;
			Fields[tempNumber - formerNumber][4] = to_string(typeT);
			Fields[tempNumber - formerNumber][5] = "";
			Fields[tempNumber - formerNumber][6] = prefixNote;
			Fields[tempNumber - formerNumber][7] = retDate();
			Fields[tempNumber - formerNumber][8] = msclr::interop::marshal_as<std::string>(textBox3->Text);
			Fields[tempNumber - formerNumber][9] = msclr::interop::marshal_as<std::string>(textBox4->Text);
			Fields[tempNumber - formerNumber][10] = summ;
			Fields[tempNumber - formerNumber][11] = percentage;
			Fields[tempNumber - formerNumber][12] = msclr::interop::marshal_as<std::string>(textBox6->Text);
			fillStandardFields(tempNumber);


			Fields[tempNumber - formerNumber + 1] = new string[13];

			string forcrc = "";
			for (i = 0; i < 13; i++)
				forcrc += Fields[tempNumber - formerNumber][i];
			if (tempNumber != formerNumber)
				forcrc += to_string(CRCField[tempNumber - 1 - formerNumber]);
			unsigned char *buf = new unsigned char[forcrc.length()];
			unsigned int newcrc;
			for (i = 0; i < forcrc.length(); i++)
				buf[i] = forcrc[i];
			newcrc = Crc32(buf, forcrc.length());
			CRCField[tempNumber - formerNumber] = newcrc;


			ofstream f(fname2, ios_base::app);
			char razdel = 0x0D;
			for (i = 0; i < 13; i++)
			{
				for (j = 0; j < Fields[tempNumber - formerNumber][i].length(); j++)
				{
					f << Fields[tempNumber - formerNumber][i][j];
				}
				f << razdel;
			}
			razdel = ';';
			string crcout = to_string(CRCField[tempNumber - formerNumber]);
			for (j = 0; j < crcout.length(); j++)
			{
				f << crcout[j];
			}
			f << razdel;
			f.close();
			tempNumber++;
		}
		stateRUN = 0;
		DeactivateEditMode();
		if (state == 1)
			button1->PerformClick();
		else if (state == 2)
			button2->PerformClick();
		else if (state == 3)
			button3->PerformClick();
		return;
	}
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		stateRUN = 0;
		DeactivateEditMode();
		return;
	}
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
	
	if (ifNumber(msclr::interop::marshal_as<std::string>(comboBox1->Text)))
	{
		ActivateEditMode();
		textBox1->Text = "";
		textBox2->Text = "";
		textBox3->Text = "";
		textBox4->Text = "";
		textBox5->Text = "";
		textBox6->Text = "";
		comboBox2->Text = "";
		int typeOfNote = retNumber(msclr::interop::marshal_as<std::string>(comboBox1->Text));
		switch (typeOfNote)
		{
		case 1:
			label2->Visible = false;
			label3->Visible = false;
			textBox1->Visible = false;
			textBox2->Visible = false;

			break;
		case 2:
			label3->Visible = false;
			label4->Visible = false;
			label5->Visible = false;
			label7->Visible = false;

			textBox2->Visible = false;
			textBox3->Visible = false;
			textBox4->Visible = false;
			comboBox2->Visible = false;

			break;
		case 5:
			label2->Visible = false;
			label3->Visible = false;
			label4->Visible = false;
			label5->Visible = false;
			label7->Visible = false;
			textBox1->Visible = false;
			textBox2->Visible = false;
			textBox3->Visible = false;
			textBox4->Visible = false;
			comboBox2->Visible = false;
			break;
		case 6:
			label2->Visible = false;
			label3->Visible = false;
			label4->Visible = false;
			label5->Visible = false;
			label7->Visible = false;
			textBox1->Visible = false;
			textBox2->Visible = false;
			textBox3->Visible = false;
			textBox4->Visible = false;
			comboBox2->Visible = false;
		}
		return;
	}
	else
	{
		MessageBox::Show("Выберите тип записи", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

}
	private: System::Void listBox1_DrawItem(System::Object^  sender, System::Windows::Forms::DrawItemEventArgs^  e) 
	{
		// Draw the background of the ListBox control for each item.
		e->DrawBackground();
		// Define the default color of the brush as black.

		//вот здесь надо проверить, надо ли элемент красить в зеленый
		bool ifGreen = false;

		// Determine the color of the brush to draw each item based 
		// on the index of the item to draw.

		switch (colorNumber[e->Index])
		{
		case 1:
			e->Graphics->DrawString(listBox1->Items[e->Index]->ToString(), e->Font, Brushes::Black, e->Bounds, StringFormat::GenericDefault);
			break;
		case 2:
			e->Graphics->DrawString(listBox1->Items[e->Index]->ToString(), e->Font, Brushes::Green, e->Bounds, StringFormat::GenericDefault);
			break;
		case 3:
			e->Graphics->DrawString(listBox1->Items[e->Index]->ToString(), e->Font, Brushes::Red, e->Bounds, StringFormat::GenericDefault);
		}

		// Draw the current item text based on the current Font 
		// and the custom brush settings.

		// If the ListBox has focus, draw a focus rectangle around the selected item.
		e->DrawFocusRectangle();
		return;
	}
			
	std::wstring to_wstring(const char* str) {
		std::unique_ptr<wchar_t[]> tmp = nullptr;
		size_t sz, len;
		len = mbstowcs(nullptr, str, 0); // получить размер
		sz = len + 1;
		tmp.reset(new wchar_t[sz]);      // выделить память
		mbstowcs(tmp.get(), str, sz);    // перекодировать
		return std::wstring(tmp.get());
	}
	std::wstring to_wstring(string str) {
		return to_wstring(str.c_str());
	}
	void printInCell(int row, int col, string str, Format* textFormat)
	{
		sheet->writeStr(row, col, (to_wstring(str.c_str())).c_str(),textFormat );
		//xlSheetSetCol(sheet, col, col, -1, NULL, 0);
		return;
	};

	string summ_to_string(float str)
	{

		string IRV = to_string(str);
		int i, dot = -1, N = IRV.length();
		string beforeDot = "", afterDot = "";

		for (i = 0; i < N; i++)
		{
			if (IRV[i] == '.' || IRV[i] == ',')
				dot = i;
			else if (dot == -1)
				beforeDot += IRV[i];
			else
				afterDot += IRV[i];
		}
		afterDot += "00";
		IRV = beforeDot;
		IRV += '.';
		IRV += afterDot[0];
		IRV += afterDot[1];
		return IRV;
	}

private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) 
{
	book = xlCreateBook();
   // formats

	Format** textFormat = new Format*[5];

	textFormat[0] = book->addFormat();
	textFormat[0]->setAlignH(ALIGNH_CENTER);
	textFormat[0]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[0]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[0]->setPatternForegroundColor(COLOR_GRAY25);

	textFormat[1] = book->addFormat();
	textFormat[1]->setAlignH(ALIGNH_CENTER);
	textFormat[1]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[1]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[1]->setPatternForegroundColor(COLOR_BLUE);

	textFormat[2] = book->addFormat();
	textFormat[2]->setAlignH(ALIGNH_CENTER);
	textFormat[2]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[2]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[2]->setPatternForegroundColor(COLOR_PINK);

	textFormat[3] = book->addFormat();
	textFormat[3]->setAlignH(ALIGNH_CENTER);
	textFormat[3]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[3]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[3]->setPatternForegroundColor(COLOR_YELLOW);

	textFormat[4] = book->addFormat();
	textFormat[4]->setAlignH(ALIGNH_CENTER);
	textFormat[4]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[4]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[4]->setPatternForegroundColor(COLOR_GREEN);


   // actions

  sheet = book->addSheet(L"Лист 1");

   //sheet->setDisplayGridlines(false);

  int tf = 0;
   sheet->writeStr(1, 0, L"Договор", textFormat[tf]);
   sheet->setMerge(1, 1, 0, 9);
   sheet->writeStr(2, 0, L"Номер договора", textFormat[tf]);
   sheet->writeStr(2, 1, L"Дата оформления", textFormat[tf]);
   sheet->writeStr(2, 2, L"Срок возврата", textFormat[tf]);
   sheet->writeStr(2, 3, L"ФИО", textFormat[tf]);
   sheet->writeStr(2, 4, L"Сумма займа", textFormat[tf]);
   sheet->writeStr(2, 5, L"Процентная ставка в день", textFormat[tf]);
   sheet->writeStr(2, 6, L"Годовая процентная ставка", textFormat[tf]);
   sheet->writeStr(2, 7, L"Процентов в срок возврата, руб.", textFormat[tf]);
   sheet->writeStr(2, 8, L"Итого в срок возврата", textFormat[tf]);
   sheet->writeStr(2, 9, L"Комментарий", textFormat[tf]);
   tf = 1;
   sheet->writeStr(1, 10, L"Погашено", textFormat[tf]);
   sheet->setMerge(1, 1, 10, 12);
   sheet->writeStr(2, 10, L"Основная сумма", textFormat[tf]);
   sheet->writeStr(2, 11, L"Проценты", textFormat[tf]);
   sheet->writeStr(2, 12, L"Итого", textFormat[tf]);
   tf = 2;
   sheet->writeStr(1, 13, L"Задолженность", textFormat[tf]);
   sheet->setMerge(1, 1, 13, 15);
   sheet->writeStr(2, 13, L"Остаток основной суммы", textFormat[tf]);
   sheet->writeStr(2, 14, L"Остаток процентов", textFormat[tf]);
   sheet->writeStr(2, 15, L"Итого", textFormat[tf]);
   tf = 0;
   int i;
   for (i = 0; i < 50; i++)
   {
	   sheet->writeStr(1, 16 + i * 3, L"Платеж", textFormat[tf]);
	   sheet->setMerge(1, 1, 16 + i * 3, 16+i*3+2);

	   sheet->writeStr(2, 16 + i * 3, L"Дата платежа", textFormat[tf]);
	   sheet->writeStr(2, 16 + i * 3 + 1, L"Сумма платежа", textFormat[tf]);
	   sheet->writeStr(2, 16 + i * 3 + 2, L"Комментарий", textFormat[tf]);
   }
   int numbOfContract = 0;
   paymentContract *tempContract = ContractFirst;
   while (tempContract != NULL)
   {
	   if (!isDeleted[tempContract->number - formerNumber])
	   {
		   int tcn = tempContract->number;
		   if (isChanged[tcn - formerNumber])
			   tcn = numberOfNoteThatChangedThis[tcn-formerNumber];
		   float summInRoubles = retSumm(Fields[tcn - formerNumber][10]);
		   float percentInDay = retSumm(Fields[tcn - formerNumber][11]);
		   int daysBetween = retDaysBetween(Fields[tcn - formerNumber][7], Fields[tcn - formerNumber][8]);
		   float percentInRoublesInPayday = 0.01 * summInRoubles * percentInDay * daysBetween;
		   int tempYY = 0;
		   tempYY += retDecimal(Fields[tcn - formerNumber][7][6]); tempYY *= 10;
		   tempYY += retDecimal(Fields[tcn - formerNumber][7][7]); tempYY *= 10;
		   tempYY += retDecimal(Fields[tcn - formerNumber][7][8]); tempYY *= 10;
		   tempYY += retDecimal(Fields[tcn - formerNumber][7][9]);

		   if (tempYY % 400 == 0)
			   tempYY = 366;
		   else if (tempYY % 100 == 0)
			   tempYY = 365;
		   else if (tempYY % 4 == 0)
			   tempYY = 366;
		   else
			   tempYY = 365;

		  
		   float paymentSumm = 0;
		   
		   payment * tempPayment = tempContract->payfirst;
		   
		   while (tempPayment != NULL)
		   {
			   int tpn = tempPayment->number;
			   if (!isDeleted[tpn - formerNumber])
			   {
				   if (isChanged[tpn - formerNumber])
					   tpn = numberOfNoteThatChangedThis[tpn-formerNumber];
				   paymentSumm += retSumm(Fields[tpn - formerNumber][10]);
			   }
			   tempPayment = tempPayment->next;
		   }

		   float pogSumm;
		   if (percentInRoublesInPayday > paymentSumm)
			   pogSumm = 0;
		   else
			   pogSumm = paymentSumm - percentInRoublesInPayday;

		   float pogPercent;
		   if (percentInRoublesInPayday > paymentSumm)
			   pogPercent = paymentSumm;
		   else
			   pogPercent = percentInRoublesInPayday;
		   float ostSumm;
		   if (percentInRoublesInPayday > paymentSumm)
			   ostSumm = summInRoubles;
		   else
			   ostSumm = summInRoubles - (paymentSumm - percentInRoublesInPayday);

		   float ostPercent;
		   if (percentInRoublesInPayday > paymentSumm)
			   ostPercent = percentInRoublesInPayday - paymentSumm;
		   else
			   ostPercent = 0;

		   tf = 4;
		   if (ostPercent + ostSumm > 0)
			   tf = 3;

		   float yearPercent = tempYY * retSumm(Fields[tcn - formerNumber][11]);

		   printInCell(3 + numbOfContract, 0, Fields[tcn - formerNumber][6], textFormat[tf]);
		   printInCell(3 + numbOfContract, 1, Fields[tcn - formerNumber][7], textFormat[tf]);
		   printInCell(3 + numbOfContract, 2, Fields[tcn - formerNumber][8], textFormat[tf]);
		   printInCell(3 + numbOfContract, 3, Fields[tcn - formerNumber][9], textFormat[tf]);
		   printInCell(3 + numbOfContract, 4, Fields[tcn - formerNumber][10], textFormat[tf]);
		   printInCell(3 + numbOfContract, 5, Fields[tcn - formerNumber][11], textFormat[tf]);
		   printInCell(3 + numbOfContract, 6, summ_to_string(yearPercent), textFormat[tf]);
		   printInCell(3 + numbOfContract, 7, summ_to_string(percentInRoublesInPayday), textFormat[tf]);
		   printInCell(3 + numbOfContract, 8, summ_to_string(percentInRoublesInPayday + summInRoubles), textFormat[tf]);
		   printInCell(3 + numbOfContract, 9, Fields[tcn - formerNumber][12], textFormat[tf]);
		   printInCell(3 + numbOfContract, 10, summ_to_string(pogSumm), textFormat[tf]);
		   printInCell(3 + numbOfContract, 11, summ_to_string(pogPercent), textFormat[tf]);
		   printInCell(3 + numbOfContract, 12, summ_to_string(paymentSumm), textFormat[tf]);
		   printInCell(3 + numbOfContract, 13, summ_to_string(ostSumm), textFormat[tf]);
		   printInCell(3 + numbOfContract, 14, summ_to_string(ostPercent), textFormat[tf]);
		   printInCell(3 + numbOfContract, 15, summ_to_string(ostSumm + ostPercent), textFormat[tf]);

		   tempPayment = tempContract->payfirst;
		   int numbOfPayment = 0;
		   while (tempPayment != NULL)
		   {
			   int tpn = tempPayment->number;
			   if (!isDeleted[tpn - formerNumber])
			   {
				   if (isChanged[tpn - formerNumber])
					   tpn = numberOfNoteThatChangedThis[tpn - formerNumber];
				   printInCell(3 + numbOfContract, 16 + numbOfPayment * 3, Fields[tpn - formerNumber][7], textFormat[tf]);
				   printInCell(3 + numbOfContract, 16 + numbOfPayment * 3 + 1, Fields[tpn - formerNumber][10], textFormat[tf]);
				   printInCell(3 + numbOfContract, 16 + numbOfPayment * 3 + 2, Fields[tpn - formerNumber][12], textFormat[tf]);
				   numbOfPayment++;
			   }
			   tempPayment = tempPayment->next;
		   }
		   numbOfContract++;
	   }
	   tempContract = tempContract->next;
   }

   book->save(L"Займы.xls");
   book->release();
   
   return;
}

private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
	book = xlCreateBook();
	// formats
	int k, j, count = 0;
	paymentContract *tempContract;

	Format** textFormat = new Format*[3];

	textFormat[0] = book->addFormat();
	textFormat[0]->setAlignH(ALIGNH_CENTER);
	textFormat[0]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[0]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[0]->setPatternForegroundColor(COLOR_GRAY25);

	textFormat[1] = book->addFormat();
	textFormat[1]->setAlignH(ALIGNH_CENTER);
	textFormat[1]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[1]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[1]->setPatternForegroundColor(COLOR_BLUE);

	textFormat[2] = book->addFormat();
	textFormat[2]->setAlignH(ALIGNH_CENTER);
	textFormat[2]->setFillPattern(FILLPATTERN_SOLID);
	textFormat[2]->setBorder(BORDERSTYLE_MEDIUM);
	textFormat[2]->setPatternForegroundColor(COLOR_PINK);
	   
	// actions

	sheet = book->addSheet(L"Лист 1");

	//sheet->setDisplayGridlines(false);

	int tf = 0;
	sheet->writeStr(1, 0, L"Тип", textFormat[tf]);
	sheet->writeStr(1, 1, L"Дата внесения/расхода", textFormat[tf]);
	sheet->writeStr(1, 2, L"Сумма", textFormat[tf]);
	sheet->writeStr(1, 3, L"Комментарий", textFormat[tf]);

	int numbOfContract = 0;
	tempContract = CashFirst;
	while (tempContract != NULL)
	{
		int i = tempContract->number;
		if (!isDeleted[i - formerNumber])
		{
			int type = retNumber(Fields[i-formerNumber][4]);
			if (isChanged[i - formerNumber])
				i = numberOfNoteThatChangedThis[i - formerNumber];

			if (type == 5)
			{
				tf = 1;
				printInCell(2 + numbOfContract, 0, "Внесение", textFormat[tf]);
			}
			else
			{
				tf = 2;
				printInCell(2 + numbOfContract, 0, "Расход", textFormat[tf]);
			}
			printInCell(2 + numbOfContract, 1, Fields[i - formerNumber][7], textFormat[tf]);
			printInCell(2 + numbOfContract, 2, Fields[i - formerNumber][10], textFormat[tf]);
			printInCell(2 + numbOfContract, 3, Fields[i - formerNumber][12], textFormat[tf]);

			numbOfContract++;
		}
		tempContract = tempContract->next;
	}
	
	book->save(L"Приход и расход.xls");
	book->release();

	return;
}
};
}
