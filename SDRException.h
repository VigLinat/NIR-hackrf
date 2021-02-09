#include <string>

class SDRException
{
public:
	SDRException();
	 
	SDRException(const hackrf_error result);
	
	std::string What() const;

private:
	hackrf_error m_result;
};