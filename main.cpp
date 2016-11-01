#include <iostream>
#include <iomanip>
#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#if defined ANDROID || defined __linux__
#  include <linux/auxvec.h>
#endif
#include <cstdlib>

#ifdef __aarch64__
const int cFill = 16;
typedef Elf64_auxv_t AUXV_t;
#elif defined( __arm__ )
const int cFill = 8;
typedef Elf32_auxv_t AUXV_t;
#endif

const char CPUINFO[] = "cat /proc/cpuinfo";
const char UNAME[] = "uname -a";
const char LSBRELEASE[] = "lsb_release -a";

void dumpAuxv(bool format = false)
{
#if defined ANDROID || defined __linux__
	int cpufile = open("/proc/self/auxv", O_RDONLY);

	if (cpufile >= 0)
	{
		AUXV_t auxv;
		const size_t size_auxv_t = sizeof(auxv);
		using namespace std;
		{
			ios::fmtflags flagSaved = cout.flags();
			char fillSaved = cout.fill('=');
			cout << setw(10) << "=" << endl;
			cout.fill(fillSaved);
			cout.flags(flagSaved);
		}

		while ((size_t)read(cpufile, &auxv, size_auxv_t) == size_auxv_t)
		{
			if(format)
			{
				cout << "0x" << hex << auxv.a_un.a_val << endl;
			}
			else
			{
				char fillSaved = cout.fill('0');
				cout << "0x" << hex << setw(cFill) << auxv.a_type << '\t';
				cout << "0x" << hex << setw(cFill) << auxv.a_un.a_val << endl;
				cout.fill(fillSaved);
			}
		}

		close(cpufile);
	}
#endif

}

void dumpSystem(const char* str)
{
	using namespace std;
	cout << str << endl;
	system(str);
}

int main(int argc, char** argv)
{
	dumpSystem(CPUINFO);
	dumpSystem(UNAME);
	dumpSystem(LSBRELEASE);
	dumpAuxv();
	dumpAuxv(true);
	
	return 0;
}
