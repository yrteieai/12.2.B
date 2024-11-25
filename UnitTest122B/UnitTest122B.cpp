#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <string>
#include "../12.2.B/12.2.B.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest122B
{
	TEST_CLASS(UnitTest122B)
	{
	public:

		TEST_METHOD(TestInputSaveAndRead)
		{
			const char* testFile = "test_students.dat";

			{
				ofstream fout(testFile, ios::binary);
				Assert::IsTrue(fout.is_open(), L"������� �������� ����� ��� ������.");

				Student student1 = { "Ivanov", 1, KN, 5, 4, {5} };
				Student student2 = { "Petrov", 2, INF, 3, 4, {4} };
				fout.write(reinterpret_cast<char*>(&student1), sizeof(Student));
				fout.write(reinterpret_cast<char*>(&student2), sizeof(Student));
				fout.close();
			}

			{
				ifstream fin(testFile, ios::binary);
				Assert::IsTrue(fin.is_open(), L"������� �������� ����� ��� �������.");

				Student student;
				fin.read(reinterpret_cast<char*>(&student), sizeof(Student));

				Assert::AreEqual(std::string("Ivanov"), std::string(student.prizv), L"����������� �������.");
				Assert::AreEqual(1, student.kurs, L"������������ ����.");
				Assert::AreEqual(static_cast<int>(KN), static_cast<int>(student.spec), L"����������� ������������.");
				Assert::AreEqual(5, student.mark_physics, L"����������� ������ � ������.");
				Assert::AreEqual(4, student.mark_math, L"����������� ������ � ����������.");
				Assert::AreEqual(5, student.third_mark.mark_prog, L"����������� ������ � �������������.");

				fin.read(reinterpret_cast<char*>(&student), sizeof(Student));

				Assert::AreEqual(std::string("Petrov"), std::string(student.prizv), L"����������� �������.");
				Assert::AreEqual(2, student.kurs, L"������������ ����.");
				Assert::AreEqual(static_cast<int>(INF), static_cast<int>(student.spec), L"����������� ������������.");
				Assert::AreEqual(3, student.mark_physics, L"����������� ������ � ������.");
				Assert::AreEqual(4, student.mark_math, L"����������� ������ � ����������.");
				Assert::AreEqual(4, student.third_mark.mark_num_methods, L"����������� ������ � ��������� ������.");

				fin.close();
			}

			remove(testFile);
		}

		TEST_METHOD(TestCalculateMarks)
		{
			const char* testFile = "test_calculate_marks.dat";

			{
				ofstream fout(testFile, ios::binary);
				Assert::IsTrue(fout.is_open(), L"������� �������� ����� ��� ������.");

				Student student1 = { "Ivanov", 1, KN, 5, 4, {5} };
				Student student2 = { "Petrov", 2, KN, 3, 3, {4} };
				Student student3 = { "Sidorov", 3, KN, 2, 3, {3} };
				fout.write(reinterpret_cast<char*>(&student1), sizeof(Student));
				fout.write(reinterpret_cast<char*>(&student2), sizeof(Student));
				fout.write(reinterpret_cast<char*>(&student3), sizeof(Student));
				fout.close();
			}

			{
				ifstream fin(testFile, ios::binary);
				Assert::IsTrue(fin.is_open(), L"������� �������� ����� ��� �������.");

				int count5 = 0, count4 = 0, count3 = 0;
				Student student;

				while (fin.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
					if (student.spec == KN) {
						if (student.third_mark.mark_prog == 5) count5++;
						else if (student.third_mark.mark_prog == 4) count4++;
						else if (student.third_mark.mark_prog == 3) count3++;
					}
				}

				Assert::AreEqual(1, count5, L"����������� ������� ������ '5'.");
				Assert::AreEqual(1, count4, L"����������� ������� ������ '4'.");
				Assert::AreEqual(1, count3, L"����������� ������� ������ '3'.");

				fin.close();
			}

			remove(testFile);
		}
	};
}
