#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<regex>
using namespace std;
/*
REGEX xususi class dir set metodlarinda istifade olunur.
regex pattern obyekti yaradiriq   new object ile muqayise edirik uygundursa qaliba
deyisdirir. #include<regex>
*/


#pragma region practice Clothing store
class Product;
class Clothing;
class Shoes;
class Accessories;
class MyException;

class MyException {
	string text;
	int line;
	string datetime;
public:
	MyException() {
		this->text = "Unknown exception";
		this->line = 0;
		this->datetime = __DATE__;
	}
	MyException(string text, int line) {
		this->text = text;
		this->line = line;
		this->datetime = __DATE__;
	}

	void what() const {
		cout << "Error Message: " << text << endl;
		cout << "Line: " << line << endl;
		cout << "Date: " << datetime << endl;
	}
};

class Product {
protected:
	string name;
	string id;
	double price;
	int size;
	void setId(const string& Id) {
		regex pattern("^[0-9]+$");
		if (regex_match(Id, pattern)) {
			this->id = Id;
			return;
		}
		MyException ex("invalid Id input", __LINE__);
		throw(ex);
	}
public:
	Product() = default;
	Product(string name, string id, double price, int size) {
		setName(name);
		setId(id);
		setPrice(price);
		setSize(size);
	}
	void setName(string name) {
		if (name.length() > 3) {
			this->name = name;
			return;
		}
		MyException ex("invalid name input", __LINE__);
		throw(ex);
	}
	void setPrice(double price) {
		if (price > 5.0) {
			this->price = price;
			return;
		}
		MyException ex("invalid price input", __LINE__);
		throw(ex);
	}
	virtual void setSize(int size) {
		if (size > 16) {
			this->size = size;
			return;
		}
		MyException ex("invalid size input", __LINE__);
		throw(ex);
	}
	string getName() const {
		return this->name;
	}
	string getId() const {
		return this->id;
	}
	double getPrice() const {
		return this->price;
	}
	int getSize() const {
		return this->size;
	}
	//fayla seliqeli yazma ucun  lazimi metodlar
	virtual string getType() const = 0;
	virtual string getExtra() const = 0;

	virtual void Show() const {
		cout << "Name: " << name << " | Id: " << id << " | Price: " << price << " | Size: " << size;
	}
	virtual ~Product() = default;
};

class Clothing : public Product {
	string clothingMaterial;
public:
	Clothing(string name, string id, double price, int size, string clothingMaterial)
		:Product(name, id, price, size) {
		setClothingMaterial(clothingMaterial);
	}
	void setClothingMaterial(string clothingMaterial) {
		if (!clothingMaterial.empty()) {
			this->clothingMaterial = clothingMaterial;
			return;
		}
		MyException ex("invalid clothing material input", __LINE__);
		throw(ex);
	}
	string getType() const override { return "Clothing"; }
	string getExtra() const override { return clothingMaterial; }

	string getClothingMaterial() const {
		return this->clothingMaterial;
	}
	void Show() const override {
		Product::Show();
		cout << " | Clothing Material: " << clothingMaterial << endl;
	}
};

class Shoes : public Product {
	string shoesType;
public:
	Shoes(string name, string id, double price, int size, string shoesType)
		:Product(name, id, price, size) {
		setShoestype(shoesType);
	}
	void setShoestype(string shoesType) {
		if (!shoesType.empty()) {
			this->shoesType = shoesType;
			return;
		}
		MyException ex("shoes type error input", __LINE__);
		throw(ex);
	}
	string getType() const override { return "Shoes"; }
	string getExtra() const override { return shoesType; }

	string getShoesType() const {
		return this->shoesType;
	}
	void Show() const override {
		Product::Show();
		cout << " | Shoes Type: " << shoesType << endl;
	}
};

class Accessories : public Product {
	string acc_material;
public:
	Accessories(string name, string id, double price, int size, string acc_material)
		:Product(name, id, price, size) {
		setAccMaterial(acc_material);
	}
	void setAccMaterial(string acc_material) {
		if (!acc_material.empty()) {
			this->acc_material = acc_material;
			return;
		}
		MyException ex("accessories material error input", __LINE__);
		throw(ex);
	}
	string getType() const override { return "Accessories"; }
	string getExtra() const override { return acc_material; }

	string getAccMaterial() const {
		return this->acc_material;
	}
	void Show() const override {
		Product::Show();
		cout << " | Accessories material: " << acc_material << endl;
	}
};

class Stock {
	vector<Product*> products;
	string filename;

	void WriteToFile() {
		ofstream fs(filename, ios::out);
		if (!fs.is_open()) {
			MyException ex("file couldn't open...", __LINE__);
			throw(ex);
		}
		for (Product* p : products) {
			//get type methodu ile classlari ayira bilirik
			fs << p->getType() << "," << p->getName() << "," << p->getId() << "," << p->getPrice() << "," << p->getSize() << "," << p->getExtra() << "\n";
		}
		fs.close();
	}

	void ReadFromFile() {
		ifstream fs(filename);
		if (!fs.is_open()) return;

		string line;
		while (getline(fs, line)) {
			if (line.empty()) continue;
			stringstream ss(line);
			string type, name, id, priceStr, sizeStr, extra;

			getline(ss, type, ',');
			getline(ss, name, ',');
			getline(ss, id, ',');
			getline(ss, priceStr, ',');
			getline(ss, sizeStr, ',');
			getline(ss, extra, ',');

			if (type.empty() || id.empty() || priceStr.empty() || sizeStr.empty()) continue;

			double price = stod(priceStr);
			int size = stoi(sizeStr);

			try {
				if (type == "Clothing") {
					products.push_back(new Clothing(name, id, price, size, extra));
				}
				else if (type == "Shoes") {
					products.push_back(new Shoes(name, id, price, size, extra));
				}
				else if (type == "Accessories") {
					products.push_back(new Accessories(name, id, price, size, extra));
				}
			}
			catch (...) {}
		}
		fs.close();
	}

public:
	Stock(string path = "product.txt") : filename(path) {
		ReadFromFile();
	}

	~Stock() {
		for (Product* p : products) {
			delete p;
		}
		products.clear();
	}

	void Create(Product* p) {
		for (Product* existing : products) {
			if (existing->getId() == p->getId()) {
				delete p;
				cout << "product has been already exists" << endl;
				return;
			}
		}
		products.push_back(p);
		WriteToFile();
		cout << "product successfully added..." << endl;
	}

	void ShowAll() const {
		if (products.empty()) {
			cout << "stock is empty..." << endl;
			return;
		}
		for (const Product* p : products) {
			p->Show();
		}
	}

	Product* SearchById(const string& id) {
		for (Product* p : products) {
			if (p->getId() == id) {
				return p;
			}
		}
		return nullptr;
	}

	void UpdatePrice(const string& id, double newPrice) {
		Product* p = SearchById(id);
		if (p == nullptr) {
			cout << "product not found..." << endl;
			return;
		}
		p->setPrice(newPrice);
		WriteToFile();
		cout << "price is changed successfully" << endl;
	}

	void DeleteProduct(const string& id) {
		for (auto it = products.begin(); it != products.end(); ++it) {
			if ((*it)->getId() == id) {
				delete* it;
				products.erase(it);
				WriteToFile();
				cout << "product successfully deleted..." << endl;
				return;
			}
		}
		cout << "product not found..." << endl;
	}
};
void main() {
	try {
		Stock stock("product.txt");
		int choice;

		do {
			cout << "\n--- GEYIM MAGAZASI ---" << endl;
			cout << "1.Create" << endl;
			cout << "2.Show All" << endl;
			cout << "3.Search" << endl;
			cout << "4.Update Price" << endl;
			cout << "5.Delete" << endl;
			cout << "6.Exit" << endl;
			cout << "choice: ";
			cin >> choice;

			if (cin.fail()) { //input daki tiple yazdgmiz tip qarissa loopa dusmesin
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "wrong input!" << endl;
				continue;
			}

			if (choice == 1) {
				int typeChoice;
				cout << "1. Clothing\n2. Shoes\n3. Accessories\n choose type: ";
				cin >> typeChoice;

				string name, id;
				double price;
				int size;

				cout << "Name: ";
				cin >> name;
				cout << "ID: ";
				cin >> id;
				cout << "Price: ";
				cin >> price;
				cout << "Size: ";
				cin >> size;

				if (typeChoice == 1) {
					string mat;
					cout << "Material: ";
					cin >> mat;
					stock.Create(new Clothing(name, id, price, size, mat));
				}
				else if (typeChoice == 2) {
					string stype;
					cout << "Shoes Type: ";
					cin >> stype;
					stock.Create(new Shoes(name, id, price, size, stype));
				}
				else if (typeChoice == 3) {
					string amat;
					cout << "Accessories Material: ";
					cin >> amat;
					stock.Create(new Accessories(name, id, price, size, amat));
				}
			}
			else if (choice == 2) {
				stock.ShowAll();
			}
			else if (choice == 3) {
				string id;
				cout << "Searched ID: ";
				cin >> id;
				Product* p = stock.SearchById(id);
				if (p != nullptr) {
					p->Show();
					cout << endl;
				}
				else {
					cout << "not found!" << endl;
				}
			}
			else if (choice == 4) {
				string id;
				double newPrice;
				cout << "Enter id: ";
				cin >> id;
				cout << "new price: ";
				cin >> newPrice;
				stock.UpdatePrice(id, newPrice);
			}
			else if (choice == 5) {
				string id;
				cout << "deleted ID: ";
				cin >> id;
				stock.DeleteProduct(id);
			}

		} while (choice != 6);

	}
	catch (MyException& ex) {
		ex.what();
	}
	catch (exception& ex) {
		cout << "error: " << ex.what() << endl;
	}
}
#pragma endregion