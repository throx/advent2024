#pragma once
#include <vector>
#include <deque>
#include <cstdint>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

class Cpu
{
private:
	int64_t a;
	int64_t b;
	int64_t c;
	int64_t ip;
	bool one_out;

	const std::vector<int64_t>& memory;
	std::vector<int64_t> out;

public:
	Cpu(int64_t a_init, int64_t b_init, int64_t c_init, const std::vector<int64_t>& m) :
		memory(m),
		a(a_init),
		b(b_init),
		c(c_init),
		ip(0),
		one_out(false)
	{
		out.reserve(32);
	}

	int64_t get_a() { return a; }

	void dump()
	{
		std::cout << "[ A: " << a << ", B: " << b << ", C: " << c << "] ";
	}

	bool step()
	{
		if (ip < 0 || ip >= memory.size()) {
			return false;
		}

		int64_t opcode = memory[ip];
		int64_t operand = memory[ip + 1];

		//dump();

		switch (opcode) {
		case 0:
			// adv
			//std::cout << "ADV " << operand << " {" << combo(operand) << "}";
			a = a / (1LL << combo(operand));
			ip += 2;
			break;
		case 1:
			// bxl
			//std::cout << "BXL " << operand;
			b = b ^ operand;
			ip += 2;
			break;
		case 2:
			// bst
			//std::cout << "BST " << operand << " {" << (combo(operand) & 0x7) << "}";
			b = combo(operand) & 0x7;
			ip += 2;
			break;
		case 3:
			// jnz
			//std::cout << "JNZ " << operand;
			if (a != 0) {
				ip = operand;
			}
			else {
				ip += 2;
			}
			break;
		case 4:
			// bxc
			//std::cout << "BXC " << operand;
			b = b ^ c;
			ip += 2;
			break;
		case 5:
			// out
			//std::cout << "OUT " << operand << " {" << (combo(operand) & 0x7) << "}";
			out.push_back(combo(operand) & 0x7);
			ip += 2;
			break;
		case 6:
			// bdv
			//std::cout << "BDV " << operand << " {" << combo(operand) << "}";
			b = a / (1LL << combo(operand));
			ip += 2;
			break;
		case 7:
			// cdv
			//std::cout << "CDV " << operand << " {" << combo(operand) << "}";
			c = a / (1LL << combo(operand));
			ip += 2;
			break;
		default:
			return false;
		}

		//dump();
		//std::cout << std::endl;
		return true;
	}

	int64_t combo(int64_t operand)
	{
		if (operand <= 3) {
			return operand;
		}
		if (operand == 4) {
			return a;
		}
		if (operand == 5) {
			return b;
		}
		if (operand == 6) {
			return c;
		}
		throw "Invalid Combo";
	}

	std::string output()
	{
		std::stringstream s;
		bool first = true;
		for (auto i : out)
		{
			if (!first)
				s << ",";

			s << i;
			first = false;
		}
		return s.str();
	}

	const std::vector<int64_t>& output_vec()
	{
		return out;
	}
};

