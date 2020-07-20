#include "stdafx.h"
#include "MyForm.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;


class node
{
public:
	bool value;
	node *next;
	node(bool val, node *nex)
	{
		value = val;
		next = nex;
	};
};