#include "vector.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>

class OutputRedirector
{
public:
	OutputRedirector()
	{
		originalCoutBuffer = std::cout.rdbuf();
		std::cout.rdbuf(outputStream.rdbuf());
	}

	~OutputRedirector()
	{
		std::cout.rdbuf(originalCoutBuffer);
	}

	std::string GetOutput() const
	{
		return outputStream.str();
	}

private:
	std::stringstream outputStream;
	std::streambuf* originalCoutBuffer;
};

class InputRedirector
{
public:
	explicit InputRedirector(const std::string& input)
	{
		originalCinBuffer = std::cin.rdbuf();
		inputStream.str(input);
		std::cin.rdbuf(inputStream.rdbuf());
	}

	~InputRedirector()
	{
		std::cin.rdbuf(originalCinBuffer);
	}

private:
	std::istringstream inputStream;
	std::streambuf* originalCinBuffer;
};

TEST(ReadNumbersTest, ValidInput)
{
	std::vector<double> numbers;
	{
		InputRedirector redirect("1.0 2.0 3.5\n4.2 -5.0");
		ASSERT_TRUE(ReadNumbers(numbers));
	}

	ASSERT_EQ(numbers.size(), 5);
	EXPECT_DOUBLE_EQ(numbers[0], 1.0);
	EXPECT_DOUBLE_EQ(numbers[1], 2.0);
	EXPECT_DOUBLE_EQ(numbers[2], 3.5);
	EXPECT_DOUBLE_EQ(numbers[3], 4.2);
	EXPECT_DOUBLE_EQ(numbers[4], -5.0);
}

TEST(MainTest, Test)
{
	{
		InputRedirector inputRedirect("1.0 2 3.659512");
		const OutputRedirector outputRedirect;
		std::vector<double> numbers;
		if (!ReadNumbers(numbers))
		{
			std::cout << "ERROR" << std::endl;
			return;
		}
		ProcessNumbers(numbers);
		PrintSortedNumbers(numbers);
		EXPECT_EQ(outputRedirect.GetOutput(), "3.220 4.220 5.879\n");
	}
}

TEST(PrintSortedNumbersTest, ZeroAndSmallNumbers)
{
	{
		const std::vector<double> numbers = { -0.0001, 0.0, 0.0004, 0.0005 };
		const std::string expected = "-0.000 0.000 0.000 0.001\n";
		const OutputRedirector redirect;
		PrintSortedNumbers(numbers);
		EXPECT_EQ(redirect.GetOutput(), expected);
	}
}

TEST(PrintSortedNumbersTest, LargeNumbers)
{
	{
		const std::vector<double> numbers = { 123456.789, 98765.432, 1000.001 };
		const std::string expected = "123456.789 98765.432 1000.001\n";
		const OutputRedirector redirect;
		PrintSortedNumbers(numbers);
		EXPECT_EQ(redirect.GetOutput(), expected);
	}
}

TEST(PrintSortedNumbersTest, EmptyVector)
{
	{
		const std::vector<double> numbers;
		const std::string expected = "\n";
		const OutputRedirector redirect;
		PrintSortedNumbers(numbers);
		EXPECT_EQ(redirect.GetOutput(), expected);
	}
}

TEST(PrintSortedNumbersTest, SingleElement)
{
	{
		const std::vector<double> numbers = { 42.0 };
		const std::string expected = "42.000\n";
		const OutputRedirector redirect;
		PrintSortedNumbers(numbers);
		EXPECT_EQ(redirect.GetOutput(), expected);
	}
}

TEST(MainTest, EmptyInput)
{
	{
		InputRedirector inputRedirect("");
		const OutputRedirector outputRedirect;
		std::vector<double> numbers;
		if (!ReadNumbers(numbers))
		{
			std::cout << "ERROR" << std::endl;
			return;
		}
		ProcessNumbers(numbers);
		PrintSortedNumbers(numbers);
		EXPECT_EQ(outputRedirect.GetOutput(), "\n");
	}
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
