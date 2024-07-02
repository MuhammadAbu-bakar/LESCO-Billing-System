#include<iostream>
#include<Windows.h>
#include<fstream>
using namespace std;


//---------------------------------------------------------------------------
//						DATE CLASS	
//---------------------------------------------------------------------------
class Date
{
private:
	int month;
	int year;
	int date;
public:
	Date()
	{
		month = 0;
		year = 0;
		date = 0;
	}
	Date(int m, int y, int d)
	{
		month = m;
		year = y;
		date = d;
	}
	int GetDate()			//Getters for Date, Month and Year
	{
		return date;
	}
	int GetMonth()
	{
		return month;
	}
	int GetYear()
	{
		return year;
	}
	void SetDate(int d)			//Setters for Date, Month and Year
	{
		date = d;
	}
	void SetMonth(int d)
	{
		month = d;
	}
	void SetYear(int d)
	{
		year = d;
	}
};


//---------------------------------------------------------------------------
//						CUSTOMER CLASS
//---------------------------------------------------------------------------
class Customer
{
protected:
	int sizeC;
	char** id;
	int* units;
	Date* dob;						//Aggregation
	Date* connection;
	char** name;
	char** address;
	char** ph;
	int* billpay;

	//friend class Billing;
public:
	Customer()							//Customer Default Constructor
	{
		billpay = NULL;
		id = NULL;
		name = NULL;
		address = NULL;
		ph = NULL;
		sizeC = 0;
		units = NULL;
		dob = NULL;
		connection = NULL;
	}
	~Customer()						//Customer Destructor
	{
		if (sizeC > 0)
		{
			for (int i = 0; i < sizeC; i++)
			{
				delete[]ph[i];
				delete[]name[i];
				delete[]address[i];
			}
			delete[]id;
			delete[]name;
			delete[]address;
			delete[]ph;
		}
	}
	Customer(Customer& a)
	{
		if (sizeC > 0)				//Customer Copy Constructor
		{
			this->~Customer();
		}
		sizeC = a.sizeC;
		copy(name, a.name);
		copy(ph, a.ph);
		copy(address, a.address);
		copy(id, a.id);

		units = new int[sizeC];
		for (int i = 0; i < sizeC; i++)
			units[i] = a.units[i];

		dob = new Date[sizeC];
		connection = new Date[sizeC];

		for (int i = 0; i < sizeC; i++)
		{
			dob[i].SetDate(a.dob[i].GetDate());
			dob[i].SetMonth(a.dob[i].GetMonth());
			dob[i].SetYear(a.dob[i].GetYear());

			connection[i].SetDate(a.connection[i].GetDate());
			connection[i].SetMonth(a.connection[i].GetMonth());
			connection[i].SetYear(a.connection[i].GetYear());
		}
	}
	void copy(char**& des, char** sour)
	{
		des = new char* [sizeC];
		for (int i = 0; i < sizeC; i++)
		{
			des[i] = new char[strlen(sour[i]) + 1];
			for (int j = 0; sour[i][j] != 0; j++)
				des[i][j] = sour[i][j];
			des[i][strlen(sour[i])] = 0;
		}
	}

	void addMeter()					//Function for Adding a New Meter
	{

		int d = 0, m = 0, y = 0;
		sizeC++;
		char data[100];
		GenerateID();
		cout << "Input Name   :  ";
		cin >> data;
		GrowData(name, data);
		cout << "Input DOB" << endl;
		cout << "Date  :  ";
		cin >> d;
		cout << "Month  :  ";
		cin >> m;
		cout << "Year  :  ";
		cin >> y;
		SetDate(d, m, y, dob);
		cout << "Input Address   :  ";
		cin >> data;
		GrowData(address, data);
		cout << "Input Phone Number   :   ";
		cin >> data;
		GrowData(ph, data);
		cout << "Input Connection Date" << endl;
		cout << "Date  :  ";
		cin >> d;
		cout << "Month  :  ";
		cin >> m;
		cout << "Year  :  ";
		cin >> y;
		SetDate(d, m, y, connection);

		int* n = new int[sizeC];
		for (int i = 0; i < sizeC - 1; i++)
			n[i] = units[i];
		n[sizeC - 1] = 0;
		units = n;

		d = 0;
		GrowUnit(billpay, d);

		StoreInCustomerFile();		//Function Call for Writing in File
	}

	void StoreInCustomerFile()			//Writing Data in File
	{
		ofstream out("Customer Data.txt");
		out << sizeC << endl;
		for (int i = 0; i < sizeC; i++)
		{
			out << id[i] << ",";
			out << dob[i].GetYear() << "/" << dob[i].GetMonth() << "/" << dob[i].GetDate();
			out << "," << name[i];
			out << "," << address[i];
			out << "," << ph[i];
			out << "," << connection[i].GetYear() << "/" << connection[i].GetMonth() << "/" << connection[i].GetDate();
			out << "," << units[i];
			if (billpay[i] == 1)
				out << ",P" << endl;
			else
				out << ",U" << endl;

		}
	}
	bool CustomerLogin(char idd[], char Cname[])
	{
		bool exist = false;
		int j = 0;
		for (int i = 0; i < sizeC; i++)
		{
			if (id[i][0] == idd[i])
			{
				exist = true;
				for (int j = 1; j < 5 && exist; j++)
					if (id[i][j] != idd[j])
						exist = false;
			}
			if (exist)
				break;
		}
		if (exist)
		{
			for (int i = 0; i < sizeC; i++)
			{
				if (name[i][0] == Cname[0])
				{
					exist = true;
					j = 1;
					for (; Cname[j] != 0 && exist; j++)
						if (Cname[j] != name[i][j])
							exist = false;
				}
				if (exist && name[i][j] == 0)
					return true;
			}
		}
		return false;
	}

	void GrowData(char**& des, char* sour)
	{
		char** n = new char* [sizeC];
		for (int i = 0; i < sizeC - 1; i++)
		{
			n[i] = new char(strlen(des[i] + 1));
			for (int j = 0; des[i][j] != 0; j++)
				n[i][j] = des[i][j];
			n[i][strlen(des[i])] = 0;
		}
		n[sizeC - 1] = new char[strlen(sour) + 1];
		for (int i = 0; sour[i] != 0; i++)
			n[sizeC - 1][i] = sour[i];
		n[sizeC - 1][strlen(sour)] = 0;
		des = n;
	}
	void GrowUnit(int*& des, int d)
	{
		int* n = new int[sizeC];
		for (int i = 0; i < sizeC - 1; i++)
			n[i] = des[i];
		n[sizeC - 1] = d;
		des = n;
	}
	void SetDate(int d, int m, int y, Date*& dd)
	{
		Date* n = new Date[sizeC];
		int i = 0;
		for (; i < sizeC - 1; i++)
		{
			n[i].SetDate(dd[i].GetDate());
			n[i].SetMonth(dd[i].GetMonth());
			n[i].SetYear(dd[i].GetYear());
		}
		n[i].SetDate(d);
		n[i].SetMonth(m);
		n[i].SetYear(y);
		dd = n;
	}
	void GenerateID()
	{
		if (sizeC == 1)
		{
			id = new char* [1];
			id[0] = new char[5];
			id[0][0] = '0';
			id[0][1] = '0';
			id[0][2] = '0';
			id[0][3] = '0';
			id[0][4] = 0;
		}
		else
		{
			char** n = new char* [sizeC];
			for (int i = 0; i < sizeC - 1; i++)
			{
				n[i] = new char[5];
				for (int j = 0; j < 4; j++)
					n[i][j] = id[i][j];
				n[i][4] = 0;
			}
			n[sizeC - 1] = new char[5];
			if (n[sizeC - 2][3] == '9')
			{
				n[sizeC - 1][3] = '0';
				if (n[sizeC - 2][2] == '9')
				{
					n[sizeC - 1][2] = '0';
					if (n[sizeC - 2][1] == '9')
					{
						n[sizeC - 1][1] = '0';
						if (n[sizeC - 2][0] == '9')
							n[sizeC - 1][0] = '0';
						else
							n[sizeC - 1][0] = n[sizeC - 2][0] + 1;

					}
					else
					{
						n[sizeC - 1][1] = n[sizeC - 2][1] + 1;
						n[sizeC - 1][0] = n[sizeC - 2][0];
					}
				}
				else
				{
					n[sizeC - 1][2] = n[sizeC - 2][2] + 1;
					n[sizeC - 1][1] = n[sizeC - 2][1];
					n[sizeC - 1][0] = n[sizeC - 2][0];
				}
			}
			else
			{
				n[sizeC - 1][3] = n[sizeC - 2][3] + 1;
				n[sizeC - 1][2] = n[sizeC - 2][2];
				n[sizeC - 1][1] = n[sizeC - 2][1];
				n[sizeC - 1][0] = n[sizeC - 2][0];
			}
			n[sizeC - 1][4] = 0;
			id = n;
		}
	}

	void SetBillPayed(char* idd)
	{
		for (int i = 0; i < sizeC; i++)
		{
			if (idd[0] == id[i][0])
			{
				bool exist = true;
				for (int j = 1; j < 4 && exist; j++)
					if (idd[j] != id[i][j])
						exist = false;
				if (exist)
				{
					billpay[i] = 1;
					break;
				}
			}
		}
		StoreInCustomerFile();
	}

	friend istream& operator>>(istream& in, Customer& c)	//Customer Input Operator
	{
		int d = 0, m = 0, y = 0;
		c.sizeC++;
		char data[100];
		c.GenerateID();
		cout << "Input Name   :  ";
		cin >> data;
		c.GrowData(c.name, data);
		cout << "Input DOB" << endl;
		cout << "Date  :  ";
		cin >> d;
		cout << "Month  :  ";
		cin >> m;
		cout << "Year  :  ";
		cin >> y;
		
		while (!(d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1 && y <= 9999))
		{
				cout << "Wrong Input!"<<endl;
				cout << "Enter Again :--"<<endl;
				cout << "Date  :  ";					//Check for Valid Date Input
				cin >> d;
				cout << "Month  :  ";
				cin >> m;
				cout << "Year  :  ";
				cin >> y;
			
		} 
		c.SetDate(d, m, y, c.dob);
		cout << "Input Address   :  ";
		cin >> data;
		c.GrowData(c.address, data);
		cout << "Input Phone Number   :   ";
		cin >> data;
		c.GrowData(c.ph, data);
		cout << "Input Connection Date" << endl;
		cout << "Date  :  ";
		cin >> d;
		cout << "Month  :  ";
		cin >> m;
		cout << "Year  :  ";
		cin >> y;
		c.SetDate(d, m, y, c.connection);
		cout << "Input Regular Units   :   ";
		in >> d;
		c.GrowUnit(c.units, d);

		d = 0;
		c.GrowUnit(c.billpay, d);

		c.StoreInCustomerFile();

		return in;
	}
	void print()
	{
		for (int i = 0; i < sizeC; i++)
		{
			cout << id[i] << ",";
			cout << dob->GetYear() << "/" << dob->GetMonth() << "/" << dob->GetDate();
			cout << "," << name[i];
			cout << "," << address[i];
			cout << "," << ph[i];
			cout << "," << connection->GetYear() << "/" << connection->GetMonth() << "/" << connection->GetDate();
			cout << "," << units[i];
			if (billpay[i] == 1)
				cout << ",P" << endl;
			else
				cout << ",U" << endl;

		}
	}

};

//---------------------------------------------------------------------------
//						LESCO CLASS
//---------------------------------------------------------------------------

class LESCO :protected Customer
{
private:
	char** username;
	char** password;
	int size;
public:


	LESCO()
	{
		username = NULL;
		password = NULL;
		size = 0;
	}
	~LESCO()
	{
		if (size > 0)
		{
			for (int i = 0; i < size; i++)
			{
				delete[]username[i];
				delete[]password[i];
			}
			delete[]username;
			delete[]password;
		}
	}
	void GrowData(char**& des, char* sour)
	{
		char** n = new char* [size];
		for (int i = 0; i < size - 1; i++)
		{
			n[i] = new char(strlen(des[i] + 1));
			for (int j = 0; des[i][j] != 0; j++)
				n[i][j] = des[i][j];
			n[i][strlen(des[i])] = 0;
		}
		n[size - 1] = new char[strlen(sour) + 1];
		for (int i = 0; sour[i] != 0; i++)
			n[size - 1][i] = sour[i];
		n[size - 1][strlen(sour)] = 0;
		des = n;
	}
	bool LescoLogin(char* name, char* pass)
	{
		bool exist = false;
		int j = 0;
		for (int i = 0; i < size; i++)
		{
			if (username[i][0] == name[0])
			{
				j = 0;
				exist = true;
				for (; name[j] != 0 && exist; j++)
					if (username[i][j] != name[j])
						exist = false;
				if (exist && username[i][j] == 0)
				{
					j = 0;
					for (; pass[j] != 0 && exist; j++)
						if (pass[j] != password[i][j])
							exist = false;
					if (exist && password[i][j] == 0)
						return true;
				}
			}
		}
		return false;
	}
	void StoreInFile()
	{
		ofstream out("Employees Data.txt");
		out << size << endl;
		for (int i = 0; i < size; i++)
			out << username[i] << "," << password[i] << endl;
	}
	void NewMeter()
	{
		addMeter();
		StoreInCustomerFile();
	}
	void ChangePassword(char* name, char* pass)
	{
		for (int i = 0; i < size; i++)
		{
			if (username[i][0] == name[0])
			{
				bool find = true;
				int j = 1;
				while (name[j] != 0 && find)
				{
					if (name[j] != username[i][j])
						find = false;
					j++;
				}
				if (find && username[i][j] == 0)
				{
					int k = 0;
					j++;
					for (; pass[k] != 0; k++)
					{
						if (pass[k] != password[i][k])
							find = false;
					}
					if (find && password[i][k] == 0)
					{
						char pas[20];
						cout << "Input new password  :  ";
						cin >> pas;
						j = 0;
						delete[]password[i];
						password[i] = new char[strlen(pas) + 1];
						while (pass[j] != 0)
						{
							password[i][j] = pas[j];
							j++;
						}
						password[i][j] = 0;
						StoreInFile();

						cout << "Password Successfully Updated" << endl;
						return;
					}
					else
						cout << "Wrong Current Password" << endl;
					return;
				}
			}
		}
	}
	friend istream& operator>>(istream& in, LESCO& l)
	{
		int count = 0;
		l.size++;
		char data[50];
	InputEmp:	cout << "Enter your name  :  ";
		cin >> data;
		bool same = true;
		for (int j = 0; j < l.size - 1; j++)
		{
			same = true;
			int k = 0;
			for (; data[k] != 0 && same; k++)
			{
				if (data[k] != l.username[j][k])
					same = false;
			}
			if (same && l.username[j][k] == 0)
			{
				cout << "Username matches with another Employee" << endl;
				cout << "Please Re-" << endl;
				goto InputEmp;
			}
		}
		l.GrowData(l.username, data);
		cout << "Input Password  :  ";
		cin >> data;
		l.GrowData(l.password, data);
		l.StoreInFile();
		return in;
	}
	friend ostream& operator<<(ostream& out, LESCO& l)
	{
		cout << "Employes data" << endl;
		cout << "Username		Password" << endl;
		for (int i = 0; i < l.size; i++)
		{
			for (int j = 0; l.username[i][j] != 0; j++)
			{
				cout << l.username[i] << "\t\t" << l.password[i] << endl;
			}
			out << endl;
		}

		return out;
	}
};

//---------------------------------------------------------------------------
//						BILLING INFO CLASS
//---------------------------------------------------------------------------
class Billing
{
private:
	char** id;
	char** month;
	int* currReading;
	Date* entery;
	int* tBill;
	int* salesTex;
	int* fixCharged;
	int* unitCost;
	Date* due;
	Date* paidDate;
	int* payStatus;
	int size;
	int Bsize;
public:
	
	Billing()
	{
		payStatus = NULL;
		unitCost = NULL;
		salesTex = NULL;
		fixCharged = NULL;
		Bsize = 0;
		size = 0;
		id = NULL;
		month = NULL;
		currReading = NULL;
		entery = NULL;
		tBill = NULL;
		due = NULL;
		paidDate = NULL;
	}
	void SetID()
	{
		if (size > 0)
		{
			for (int i = 0; i < size; i++)
				delete[]id[i];
			delete[]id;
		}
		char d[100];
		ifstream fin("Customer Data.txt");
		fin >> size;
		id = new char* [size];
		fin.getline(d, 99, '\n');
		for (int i = 0; i < size; i++)
		{
			fin.getline(d, 99, '\n');
			id[i] = new char[5];
			for (int j = 0; j < 4; j++)
				id[i][j] = d[j];
			id[i][4] = 0;
		}
	}
	void print()
	{
		for (int i = 0; i < size; i++)
			cout << id[i] << endl;
	}

	void GrowChar(char**& des, char* sour)
	{
		char** n = new char* [Bsize];
		for (int i = 0; i < Bsize - 1; i++)
		{
			n[i] = new char(strlen(des[i] + 1));
			for (int j = 0; des[i][j] != 0; j++)
				n[i][j] = des[i][j];
			n[i][strlen(des[i])] = 0;
		}
		n[Bsize - 1] = new char[strlen(sour) + 1];
		for (int i = 0; sour[i] != 0; i++)
			n[Bsize - 1][i] = sour[i];
		n[Bsize - 1][strlen(sour)] = 0;
		des = n;
	}
	void GrowInt(int*& des, int d)
	{
		int* n = new int[Bsize];
		for (int i = 0; i < Bsize - 1; i++)
			n[i] = des[i];
		n[Bsize - 1] = d;
		des = n;
	}
	void GrowDate(int d, int m, int y, Date*& dd)
	{
		Date* n = new Date[Bsize];
		int i = 0;
		for (; i < Bsize - 1; i++)
		{
			n[i].SetDate(dd[i].GetDate());
			n[i].SetMonth(dd[i].GetMonth());
			n[i].SetYear(dd[i].GetYear());
		}
		n[i].SetDate(d);
		n[i].SetMonth(m);
		n[i].SetYear(y);
		dd = n;
	}

	bool IsRepeatDate(char* idd, int date, int m, int y)
	{
		ifstream fin("Billing Information.txt");
		char d[100];
		bool exist = true;
		fin.getline(d, 99, '\n');
		for (int i = 0; i < Bsize; i++)
		{
			fin.getline(d, 99, '\n');
			if (d[0] == idd[0])
			{
				int j = 0;
				exist = true;
				for (; j < 5 && exist; j++)
				{
					if (d[j] != idd[j])
						exist = false;
				}
				if (exist)
				{
					j = 0;
					int coma = 3;
					while (coma > 0)
					{
						if (d[j] == ',')
							coma--;
						j++;
					}
					j++;
					int year = 0, month = 0, dat = 0;
					year = d[j] - 48;
					j++;
					year = year * 10 + (d[j] - 48);
					j++;
					year = year * 10 + (d[j] - 48);
					j++;
					year = year * 10 + (d[j] - 48);
					j++;
					j++;
					month = d[j] - 48;
					j++;
					month = month * 10 + (d[j] - 48);
					j++;
					j++;
					dat = d[j] - 48;
					j++;
					dat = dat * 10 + (d[j] - 48);
					if (date == dat && month == m && year == y)
						return true;

				}
			}
		}
		return false;
	}

	void SetMonth(int m)
	{
		if (m == 1)
		{
			char d[] = "Jan";
			GrowChar(month, d);
		}
		else if (m == 2)
		{
			char d[] = "Feb";
			GrowChar(month, d);
		}
		else if (m == 3)
		{
			char d[] = "March";
			GrowChar(month, d);
		}
		else if (m == 4)
		{
			char d[] = "April";
			GrowChar(month, d);
		}
		else if (m == 5)
		{
			char d[] = "May";
			GrowChar(month, d);
		}
		else if (m == 6)
		{
			char d[] = "June";
			GrowChar(month, d);
		}
		else if (m == 7)
		{
			char d[] = "July";
			GrowChar(month, d);
		}
		else if (m == 8)
		{
			char d[] = "Aug";
			GrowChar(month, d);
		}
		else if (m == 9)
		{
			char d[] = "Sep";
			GrowChar(month, d);
		}
		else if (m == 10)
		{
			char d[] = "Oct";
			GrowChar(month, d);
		}
		else if (m == 11)
		{
			char d[] = "Nov";
			GrowChar(month, d);
		}
		else if (m == 12)
		{
			char d[] = "Dec";
			GrowChar(month, d);
		}
	}
	void AddBill(char* idd)
	{
		if (size == 0)
		{
			SetID();
		}
		Bsize++;
		char mon[10];
		int d = 0, m = 0, y = 0, data = 0;
	date1:	cout << "Input Reading Entry Date" << endl;
		cout << "Date  :  ";
		cin >> d;
		cout << "Month  :  ";
		cin >> m;
		cout << "Year  :  ";
		cin >> y;

		while (!(d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1 && y <= 9999))
		{
			cout << "Wrong Input!" << endl;
			cout << "Enter Again :--" << endl;
			cout << "Date  :  ";				//Check for Valid Date Input
			cin >> d;
			cout << "Month  :  ";
			cin >> m;
			cout << "Year  :  ";
			cin >> y;
		}
		if (IsRepeatDate(idd, d, m, y) && Bsize > 1)
		{
			cout << "Repeated date for Customer " << idd << "\nRe-";
			goto date1;
		}

		SetMonth(m);
		GrowDate(d, m, y, entery);
		cout << "Input Current Meter Reading  :  ";
		cin >> data;
		GrowInt(currReading, data);

		srand(time(0));
		d = rand() % 10 + 5;
		if (m == 2 && d > 28)
		{
			d %= 28;;
			m++;

		}
		else if (d > 31 && m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
		{
			d %= 31;
			m++;
			if (m == 13)
			{
				m = 1;
				y++;
			}
		}
		else if (d > 30 && m == 4 || m == 6 || m == 9 || m == 11)
		{
			d %= 30;
			m++;
		}
		GrowDate(d, m, y, due);

		cout << "Input Fixed Charges  :  ";
		cin >> data;
		GrowInt(fixCharged, data);

		cout << "Input Sales Text  :  ";
		cin >> data;
		GrowInt(salesTex, data);

		cout << "Input Electricity Cost  :  ";
		cin >> data;
		GrowInt(unitCost, data);

		data = currReading[Bsize - 1] * unitCost[Bsize - 1];
		data += salesTex[Bsize - 1] + fixCharged[Bsize - 1];
		GrowInt(tBill, data);

		GrowInt(payStatus, 0);
		GrowDate(0, 0, 0, paidDate);

		StoreInBillingFile();
	}
	void StoreInBillingFile()
	{
		ofstream out("Billing Information.txt");
		out << Bsize << endl;
		for (int i = 0; i < Bsize; i++)
		{
			out << id[i] << ",";
			out << month[i] << ",";
			out << currReading[i] << ",";
			out << entery[i].GetYear() << "/" << entery[i].GetMonth() << "/" << entery[i].GetDate();
			out << "," << unitCost[i];
			out << "," << salesTex[i];
			out << "," << fixCharged[i];
			out << "," << tBill[i];
			out << "," << due[i].GetYear() << "/" << due[i].GetMonth() << "/" << due[i].GetDate();
			if (payStatus[i] == 1)
				out << ",PAID";
			else
				out << ",UNPAID";
			out << "," << paidDate[i].GetYear() << "/" << paidDate[i].GetMonth() << "/" << paidDate[i].GetDate() << endl;
		}
	}
	void PayBill(char* idd, char* month, int date, int m, int y)
	{
		ifstream fin("Billing Information.txt");
		char d[100];
		bool exist = true;
		fin.getline(d, 99, '\n');
		for (int i = 0; i < Bsize; i++)
		{
			fin.getline(d, 99, '\n');
			if (d[0] == idd[0])
			{
				int j = 1;
				exist = true;
				for (; j < 4 && exist; j++)
				{
					if (d[j] != idd[j])
						exist = false;
				}
				if (exist)
				{
					j++;
					for (int k = 0; month[k] != 0 && exist; k++, j++)
						if (month[k] != d[j])
							exist = false;

					if (exist && d[j] == ',')
					{
						if (entery[i].GetYear() < y)
						{
							payStatus[i] = 1;
							paidDate[i].SetDate(date);
							paidDate[i].SetMonth(m);
							paidDate[i].SetYear(y);
							StoreInBillingFile();
						}
						else if (entery[i].GetYear() == y)
						{
							if (entery[i].GetMonth() < m)
							{
								payStatus[i] = 1;
								paidDate[i].SetDate(date);
								paidDate[i].SetMonth(m);
								paidDate[i].SetYear(y);
								StoreInBillingFile();
							}
							else if (entery[i].GetMonth() == m)
							{
								if (entery[i].GetDate() < date)
								{
									payStatus[i] = 1;
									paidDate[i].SetDate(date);
									paidDate[i].SetMonth(m);
									paidDate[i].SetYear(y);
									StoreInBillingFile();
								}
								else
									cout << "Invalid payment date" << endl;
							}
							else
								cout << "Invalid payment date" << endl;
						}
						else
							cout << "Invalid payment date" << endl;
					}
				}

			}
		}
	}
	void ListPayedUnpayedBills(char* idd)
	{
		ifstream fin("Billing Information.txt");
		char d[100];
		int pay = 0, unpay = 0;
		bool exist = true;
		fin.getline(d, 99, '\n');
		for (int i = 0; i < Bsize; i++)
		{
			fin.getline(d, 99, '\n');
			if (d[0] == idd[0])
			{
				int j = 1;
				exist = true;
				for (; j < 4 && exist; j++)
				{
					if (d[j] != idd[j])
						exist = false;
				}
				if (exist)
				{
					if (payStatus[i] == 1)
						pay++;
					else
						unpay++;
				}
			}
		}
		cout << "Unpaid bills are  :  " << unpay << endl;
		cout << "Paid bills are  :  " << pay << endl;
	}
};

void welcome()				//Function for Printing WELCOME
{
	int i = 0;
	char fname[40], lname[40], l[80];
	cout << "\n\n\n\n\t\t\t";
	cout << "\\    /\\    /" << endl << "\t\t\t";
	cout << " \\  /  \\  /" << endl << "\t\t\t";
	cout << "  \\/    \\/" << endl << "\t\t\t";
	Sleep(500);
	system("cls");
	cout << "\n\n\n\n\t\t\t";
	cout << "             ___" << endl << "\t\t\t";
	cout << "            |___" << endl << "\t\t\t";
	cout << "            |___" << endl << "\t\t\t";
	Sleep(500);
	system("cls");
	cout << "\n\n\n\n\t\t\t";
	cout << "                 " << endl << "\t\t\t";
	cout << "                 |" << endl << "\t\t\t";
	cout << "                 |__" << endl << "\t\t\t";
	Sleep(500);
	system("cls");
	cout << "\n\n\n\n\t\t\t";
	cout << "                      __ " << endl << "\t\t\t";
	cout << "                     | " << endl << "\t\t\t";
	cout << "                     |__ " << endl << "\t\t\t";
	Sleep(500);
	system("cls");
	cout << "\n\n\n\n\t\t\t";
	cout << "                           __" << endl << "\t\t\t";
	cout << "                          |  |" << endl << "\t\t\t";
	cout << "                          |__|" << endl << "\t\t\t";
	Sleep(500);
	system("cls");
	cout << "\n\n\n\n\t\t\t";
	cout << "                                     " << endl << "\t\t\t";
	cout << "                              |\\  /|" << endl << "\t\t\t";
	cout << "                              | \\/ |" << endl << "\t\t\t";
	Sleep(500);
	system("cls");
	cout << "\n\n\n\n\t\t\t";
	cout << "                                      ___" << endl << "\t\t\t";
	cout << "                                     |___" << endl << "\t\t\t";
	cout << "                                     |___" << endl << "\t\t\t";
	Sleep(500);
	system("cls");
	cout << "\n\n\n\n\t\t\t";
	cout << "\\    /\\    / ___       __  __          ___" << endl << "\t\t\t";
	cout << " \\  /  \\  / |___  |   |   |  | |\\   /||___  " << endl << "\t\t\t";
	cout << "  \\/    \\/  |___  |__ |__ |__| | \\ / ||___     " << endl << "\t\t\t";
	Sleep(1000);
	system("cls");
}

int main()
{
	welcome();
	Customer cus;
	LESCO emp;
	Billing bill;
	int choice = 0;
	do
	{
		cout << "Select Option Number for Respective Class Functions" << endl;
		cout << "1 - Employee" << endl;
		cout << "2 - Customer" << endl;
		cout << "3 - Clear Screen" << endl;
		cout << "4 - Exit" << endl;

		cin >> choice;
		if (choice == 1)
		{
			system("CLS");
			char name[20], pass[20];
			int choice = 0;
			do
			{
				cout << "\t\t\t\tWelcome to LESCO Employee Class" << endl;
				Sleep(2000);
				cout << endl << endl << endl;
				cout << "Select Option Number for Respective Functions" << endl;
				cout << "1 - Enter a new Employee" << endl;
				cout << "2 - Change Password" << endl;
				cout << "3 - Recieve bill payement" << endl;
				cout << "4 - Add new Meter" << endl;
				cout << "5 - List of Payed and Unpaid Bills" << endl;
				cout << "6 - Add new Bill" << endl;
				cout << "7 - Clear screen" << endl;
				cout << "8 - Exit" << endl;

				cin >> choice;

				if (choice == 1)
				{
					system("CLS");
					cout << "Input the following data" << endl;
					cin >> emp;
					system("CLS");
				}
				else if (choice == 2)
				{
					system("CLS");
					cout << "Input Username  :  ";
					cin >> name;
					cout << "Input Current Password  :  ";
					cin >> pass;
					emp.ChangePassword(name, pass);
					system("CLS");
				}
				else if (choice == 3)
				{
					system("CLS");
					cout << "Input Username  :  ";
					cin >> name;
					cout << "Input Password  :  ";
					cin >> pass;
					if (emp.LescoLogin(name, pass))
					{
						system("CLS");
						cout << "Welcome" << endl << endl;
						cout << "Input Id of Customer  :  ";
						cin >> name;
						cout << "Input Month name  :  ";
						cin >> pass;
						int d = 0, m = 0, y = 0;
						cout << "Input Date of Payement" << endl;
						cout << "Input date  :  ";
						cin >> d;
						cout << "Input month  :  ";
						cin >> m;
						cout << "Input year  :  ";
						cin >> y;
						

						while (!(d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1 && y <= 9999))
						{
							cout << "Wrong Input!" << endl;
							cout << "Enter Again :--" << endl;
							cout << "Date  :  ";				//Check for Valid Date Input
							cin >> d;
							cout << "Month  :  ";
							cin >> m;
							cout << "Year  :  ";
							cin >> y;
						}

						bill.PayBill(name, pass, d, m, y);
						cus.SetBillPayed(name);
					}
					else
						cout << "Wrong Username or Password\nTry Later" << endl;
					system("CLS");
				}
				else if (choice == 4)
				{
					system("CLS");
					cout << "Input Username  :  ";
					cin >> name;
					cout << "Input Password  :  ";
					cin >> pass;
					if (emp.LescoLogin(name, pass))
					{
						system("CLS");
						cout << "Welcome" << endl << endl;
						cout << "Input the following data" << endl;
						cus.addMeter();
					}
					else
						cout << "Wrong Username or Password\nTry Later" << endl;
					system("CLS");
				}
				else if (choice == 5)
				{
					system("CLS");
					cout << "Input Username  :  ";
					cin >> name;
					cout << "Input Password  :  ";
					cin >> pass;
					if (emp.LescoLogin(name, pass))
					{
						system("CLS");
						cout << "Welcome" << endl << endl;
						cout << "Input id of customer  :  ";
						cin >> name;
						system("CLS");
						cout << "List or Payed and Unpayed bills of Customer " << name << endl;
						bill.ListPayedUnpayedBills(name);
					}
					else
						cout << "Wrong Username or Password\nTry Later" << endl;
					system("CLS");
				}
				else if (choice == 6)
				{
					system("CLS");
					cout << "Input Username  :  ";
					cin >> name;
					cout << "Input Password  :  ";
					cin >> pass;
					if (emp.LescoLogin(name, pass))
					{
						system("CLS");
						cout << "Welcome" << endl << endl;
						cout << "Input Customer id  :  ";
						cin >> name;
						bill.AddBill(name);
					}
					else
						cout << "Wrong Username or Password\nTry Later" << endl;
					system("CLS");
				}
				else if (choice == 7)
					system("CLS");

			} while (choice != 8);
			system("CLS");
		}
		else if (choice == 2)
		{
			system("CLS");
			int choice = 0;
			do
			{
				cout << "\t\t\t\tWelcome to LESCO Customer Class" << endl;
				Sleep(2000);
				cout << endl << endl << endl;
				cout << "Select Option Number for Respective Functions" << endl;
				cout << "1 - New Customer" << endl;
				cout << "2 - Clear Screen" << endl;
				cout << "3 - Exit" << endl;

				cin >> choice;

				if (choice == 1)
				{
					system("CLS");
					cout << "Input the following data" << endl;
					cin >> cus;
					system("CLS");
				}
				else if (choice == 2)
					system("CLS");

			} while (choice != 3);
			system("CLS");
		}
		else if (choice == 3)
			system("CLS");

	} while (choice != 4);

	system("pause");
	return 0;
}