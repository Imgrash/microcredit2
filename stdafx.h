// stdafx.h: Includedatei für Standardsystem-Includedateien
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>


namespace Buhgalter
{
	using namespace std;
	public class payment
	{
	public:
		int number;

		payment *next;

		payment(int numb, payment *nex)
		{
			number = numb;
			next = nex;
		};
	};
	public class paymentContract
	{
	public:
		string value;
		int number;
		payment * payfirst;
		payment * paylast;
		paymentContract *next;

		paymentContract(string val, int numb, paymentContract *nex)
		{
			value = val;
			number = numb;
			next = nex;
			payfirst = NULL;
			paylast = NULL;
		};
	};
	public class node
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
	public class interestRate
	{
	public:
		float value;
		interestRate *next;
		interestRate(float val)
		{
			value = val;
			next = NULL;
		};
	};
};
