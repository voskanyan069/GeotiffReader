#include <map>
#include <string>
#include <vector>

class CMDArguments;
class GeotiffReceiver;
class DigitalElevationMgr;

const int RANDOM_TESTS_COUNT = 100;
typedef std::vector<std::string> Strings;

class TestApplication
{
public:
    TestApplication();
	~TestApplication();

	void reset();
	template <typename T>
	void set_option(const std::string& name, T& value);
	void filename2path(const std::string& filename, std::string& path);
	void execute_command(const std::string &cmd, const Strings& argv,
			std::string& return_value);
	void exec_py_test(const std::string& name, const std::string& point,
			std::string& return_value);
	GeotiffReceiver* get_receiver();
	DigitalElevationMgr& get_dem();

private:
	void add_tests_path();

private:
    std::string m_path;
    std::string m_tests_path;
    std::map<std::string, std::string> m_tests;
    GeotiffReceiver* m_receiver;
    CMDArguments& m_cmdargs;
    DigitalElevationMgr& m_dem;
};
