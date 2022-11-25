
#include <iostream>
#include <fstream>
#include <typeinfo>  
#include <type_traits>


using namespace std;

class ILoggable
{
protected:
	~ILoggable() = default;

public:
	virtual void logToScreen() = 0;
	virtual void logToFile(const string& filename) = 0;
};

class IShuffle
{
protected:
	~IShuffle() = default;

public:
	virtual void shuffle() = 0;
	virtual void shuffle(size_t i, size_t j) = 0;
};

class ExpressionEvaluator : public ILoggable {
public:
	ExpressionEvaluator() {
		this->operandCount = 20;
		this->operands = new double[this->operandCount] {0};
	}

	ExpressionEvaluator(size_t n) {
		this->operandCount = n;
		this->operands = new double[this->operandCount] {0};
	}

	~ExpressionEvaluator() {
		delete[] operands;
	}

	virtual double calculate() = 0;

	void setOperand(size_t pos, double value) {
		this->operands[pos] = value;
	}

	void setOperands(double ops[], size_t n) {
		for (size_t i = 0; i < n; i++)
		{
			this->operands[i] = ops[i];
		}
	}

	size_t operandCount;
	double* operands;
};

class CustomExpressionEvaluator : public ExpressionEvaluator, public IShuffle {
public:
	CustomExpressionEvaluator() : ExpressionEvaluator(), IShuffle() {}
	CustomExpressionEvaluator(size_t n) : ExpressionEvaluator(n), IShuffle() {}

	double calculate() override {
		double result = this->operands[0] + this->operands[1];
		for (size_t i = 2; i < operandCount; i++)
		{
			result *= operands[i];
		}

		return result;
	}

	void shuffle() override {
		int first = -1;
		int last = -1;

		for (size_t i = 0; i < operandCount; i++)
		{
			if (isFractional(operands[i])) {
				if (first == -1)
					first = i;
				else
					last = i;
			}
		}

		if (first != -1 && last != -1) {
			double temp = operands[last];
			operands[last] = operands[first];
			operands[first] = temp;

		}
	}

	void shuffle(size_t i, size_t j) override {
		if (isFractional(operands[i]) ||
			isFractional(operands[j])) {
			double temp = operands[j];
			operands[j] = operands[i];
			operands[i] = temp;
		}
	}

	void logToScreen() override {
		cout << "( ";
		if (operands[0] < 0)
			cout << "(" << operands[0] << ")";
		else
			cout << operands[0];

		cout << " + ";

		if (operands[1] < 0)
			cout << "(" << operands[1] << ")";
		else
			cout << operands[1];
		cout << " )";
		for (size_t i = 2; i < operandCount; i++)
		{
			if (operands[i] < 0)
				cout << " * " << "(" << operands[i] << ")";
			else
				cout << " * " << operands[i];

		}

		cout << endl;
	}

	void logToFile(const std::string& filename) override {
		std::ofstream fout(filename);
		fout << "( ";
		if (operands[0] < 0)
			fout << "(" << operands[0] << ")";
		else
			fout << operands[0];

		fout << " + ";

		if (operands[1] < 0)
			fout << "(" << operands[1] << ")";
		else
			fout << operands[1];
		fout << " )";
		for (size_t i = 2; i < operandCount; i++)
		{
			if (operands[i] < 0)
				fout << " * " << "(" << operands[i] << ")";
			else
				fout << " * " << operands[i];

		}

		fout << endl;
	}

private:
	bool isFractional(double n) {
		return (double)((int)n) != n;
	}
};

class Subtractor : public ExpressionEvaluator, public IShuffle {
public:
	Subtractor() : ExpressionEvaluator(), IShuffle() {}
	Subtractor(size_t n) : ExpressionEvaluator(n), IShuffle() {}

	double calculate() override {
		double result = this->operands[0];

		for (size_t i = 1; i < this->operandCount; i++)
		{
			result -= this->operands[i];
		}

		return result;
	}

	void shuffle() override {
		int first = -1;
		int last = -1;

		for (size_t i = 0; i < operandCount; i++)
		{
			if (isFractional(operands[i])) {
				if (first == -1)
					first = i;
				else
					last = i;
			}
		}

		if (first != -1 && last != -1) {
			double temp = operands[last];
			operands[last] = operands[first];
			operands[first] = temp;

		}
	}

	void shuffle(size_t i, size_t j) override {
		if (isFractional(operands[i]) ||
			isFractional(operands[j])) {
			double temp = operands[j];
			operands[j] = operands[i];
			operands[i] = temp;
		}
	}

	void logToScreen() override {
		if (operands[0] < 0)
			cout << "(" << this->operands[0] << ")";
		else
			cout << this->operands[0];

		for (size_t i = 1; i < this->operandCount; i++)
		{
			if (operands[i] < 0)
				cout << " - " << "(" << this->operands[i] << ")";
			else
				cout << " - " << this->operands[i];
		}

		cout << endl;
	}

	void logToFile(const std::string& filename) override {
		std::ofstream fout(filename);

		if (operands[0] < 0)
			fout << "(" << this->operands[0] << ")";
		else
			fout << this->operands[0];

		for (size_t i = 1; i < this->operandCount; i++)
		{
			if (operands[i] < 0)
				fout << " - " << "(" << this->operands[i] << ")";
			else
				fout << " - " << this->operands[i];
		}

		fout << endl;
	}

private:
	bool isFractional(double n) {
		return (double)((int)n) != n;
	}
};

class Multiplier : public ExpressionEvaluator {
public:
	Multiplier() : ExpressionEvaluator()  {}
	Multiplier(size_t n) : ExpressionEvaluator(n) {}

	double calculate() override {
		double result = operands[0];

		for (size_t i = 1; i < operandCount; i++)
		{
			result *= operands[i];
		}

		return result;
	}

	void logToScreen() override {
		if (operands[0] < 0)
			cout << "(" << operands[0] << ")";
		else
			cout << operands[0];

		for (size_t i = 1; i < operandCount; i++)
		{
			if (operands[i] < 0)
				cout << " * " << "(" << operands[i] << ")";
			else
				cout << " * " << operands[i];
		}

		cout << endl;
	}

	void logToFile(const std::string& filename) override {
		std::ofstream fout(filename);

		if (operands[0] < 0)
			fout << "(" << operands[0] << ")";
		else
			fout << operands[0];

		for (size_t i = 1; i < operandCount; i++)
		{
			if (operands[i] < 0)
				fout << " * " << "(" << operands[i] << ")";
			else
				fout << " * " << operands[i];
		}

		fout << endl;
	}

private:
	bool isFractional(double n) {
		return (double)((int)n) == n;
	}
};

int main()
{
	ExpressionEvaluator* evaluators[3];

	evaluators[0] = new Subtractor(2);
	evaluators[0]->setOperand(0, 23.65);
	evaluators[0]->setOperand(1, -12.15);

	evaluators[1] = new CustomExpressionEvaluator(5);
	double customExpressionOperands[] = { 2.5, -5, -3, 2, 10 };
	evaluators[1]->setOperands(customExpressionOperands, 5);

	evaluators[2] = new Multiplier(4);
	evaluators[2]->setOperand(0, 2.5);
	evaluators[2]->setOperand(1, -3);
	evaluators[2]->setOperand(2, 4);
	evaluators[2]->setOperand(3, -1);


	for (size_t i = 0; i < 3; ++i)
	{
		evaluators[i]->logToFile("Lab3.log");
		evaluators[i]->logToScreen();
		std::cout << evaluators[i]->calculate() << std::endl;
	}


	cout << endl;
	for (int i = 0; i < 3; ++i)
	{
		IShuffle* shuffle = dynamic_cast<IShuffle*>(evaluators[i]);
		if (shuffle)
		{
			shuffle->shuffle();
			
			evaluators[i]->logToFile("Lab3.log");
			evaluators[i]->logToScreen();
			std::cout << evaluators[i]->calculate() << std::endl;
		}
	}

}
